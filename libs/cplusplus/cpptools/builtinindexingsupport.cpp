#include "builtinindexingsupport.h"

#include "cppmodelmanager.h"
#include "searchsymbols.h"
#include "cpptoolsconstants.h"
#include "indexitem.h"
#include "ModelManagerInterface.h"
#include "Symbols.h"

#include <runextensions.h>

#include <QCoreApplication>
#include <QApplication>

#include "Core.h"

using namespace CppTools;
using namespace CppTools::Internal;

static const bool DumpFileNameWhileParsing = qgetenv("QTC_DUMP_FILENAME_WHILE_PARSING") == "1";

namespace {

static const char pp_configuration_file[] = "<configuration>";

static void parse(QFutureInterface<void> &future,
                  CppPreprocessor *sourceProcessor,
                  QStringList files,
                  const QStringList *sourceExtensions,
                  const QStringList *headerExtensions)
{
    if (files.isEmpty())
        return;

    QStringList sources;
    QStringList headers;

    foreach (const QString &file, files) {
        QFileInfo info(file);
        QString extension = info.suffix().toLower();

        sourceProcessor->snapshot.remove(file);

        if ( sourceExtensions->contains(extension) )
            sources.append(file);
        else if ( headerExtensions->contains(extension) )
            headers.append(file);
    }

    const int sourceCount = sources.size();
    files = sources;
    files += headers;

    sourceProcessor->setTodo(files);

    future.setProgressRange(0, files.size());

    QString conf = QLatin1String(pp_configuration_file);

    bool processingHeaders = false;

    for (int i = 0; i < files.size(); ++i) {
        if (future.isPaused())
            future.waitForResume();

        if (future.isCanceled())
            break;

        const QString fileName = files.at(i);

        const bool isSourceFile = i < sourceCount;
        if (isSourceFile)
            (void) sourceProcessor->run(conf);

        else if (! processingHeaders) {
            (void) sourceProcessor->run(conf);

            processingHeaders = true;
        }

        sourceProcessor->run(fileName);

        future.setProgressValue(files.size() - sourceProcessor->todo().size());

        if (isSourceFile)
            sourceProcessor->resetEnvironment();
    }

    future.setProgressValue(files.size());
    sourceProcessor->modelManager()->finishedRefreshingSourceFiles(files);

    delete sourceProcessor;
}

class BuiltinSymbolSearcher: public SymbolSearcher
{
public:
    BuiltinSymbolSearcher( const CPlusPlus::Snapshot &snapshot,
                           Parameters parameters, QSet<QString> fileNames,
                           const QStringList *sourceExtensions,
                           const QStringList *headerExtensions )
        : m_snapshot(snapshot)
        , m_parameters(parameters)
        , m_fileNames(fileNames)
        , m_sourceExtensions(sourceExtensions)
        , m_headerExtensions(headerExtensions)
    {}

    ~BuiltinSymbolSearcher()
    {}

    virtual void fillCppHelperItems( QList<CppHelperItem> &items )
    {
        SearchSymbols search(CPlusPlus::CppModelManagerInterface::instance()->stringTable());
        search.setSymbolsToSearchFor(m_parameters.types);
        CPlusPlus::Snapshot::const_iterator it = m_snapshot.begin();

        QString findString = (m_parameters.flags & Find::FindRegularExpression
                              ? m_parameters.text : QRegExp::escape(m_parameters.text));
        if (m_parameters.flags & Find::FindWholeWords)
            findString = QString::fromLatin1("\\b%1\\b").arg(findString);
        QRegExp matcher(findString, (m_parameters.flags & Find::FindCaseSensitively
                                     ? Qt::CaseSensitive : Qt::CaseInsensitive));

        auto filter = [&](bool isSourceFile, const IndexItem::Ptr &info) -> IndexItem::VisitorResult {
            QStringList symbolNameParts = info->symbolName().split("::", QString::SkipEmptyParts);
            if ( symbolNameParts.last()[0] == '~' )
                return IndexItem::Recurse;                          // Skip destructors
            int matchingPosition = matcher.indexIn(symbolNameParts.last());
            if ( matchingPosition == -1 )
                return IndexItem::Recurse;                          // Filtering
            if ( CPlusPlus::Symbol *symbol = info->symbol() ) {
                if ( symbol->enclosingClass() && !symbol->isStatic() && !symbol->isClass() && !symbol->isEnum() )
                    return IndexItem::Recurse;                      // Skip nonstatic members in class-scope
                if ( !symbol->enclosingClass() && (symbol->isStatic() || symbolNameParts.size() > 1) )
                    return IndexItem::Recurse;                      // Skip static symbols in global scope
            }
            if ( symbolNameParts.size() > 1 &&
                 symbolNameParts[symbolNameParts.size() - 1] == symbolNameParts[symbolNameParts.size() - 2] )
                return IndexItem::Recurse;                          // Skip constructors
            if ( info->symbolScope().contains("<") )
                return IndexItem::Recurse;                          // Skip anonimous namespaces
            if ( isSourceFile && info->type() != IndexItem::Declaration && info->type() != IndexItem::Function )
                return IndexItem::Recurse;                          // Skip non-functions & non-declarations from source-files

            QString text = info->symbolName();
            QString scope = info->symbolScope();
            if ( !scope.isEmpty() ) {
                if ( info->symbol()->asDeclaration() && info->symbol()->asDeclaration()->asEnumeratorDeclarator() ) {
                    QStringList scopePath = scope.split("::", QString::SkipEmptyParts);
                    scopePath.removeLast();
                    scope = scopePath.join("::");
                }
                if ( !scope.isEmpty() ) {
                    text.prepend(scope + "::");
                    matchingPosition += scope.length() + 2;
                }
            }
            QString qualifiedName = text;
            QString functionReturnType = "";
            switch ( info->type() ) {
            case IndexItem::Declaration:
                if ( !info->symbol()->asDeclaration()->asEnumeratorDeclarator() ) {
                    int oldLength = text.length();
                    text = IndexItem::representDeclaration(text, info->symbolType());
                    matchingPosition += text.length() - oldLength;
                }
                break;
            case IndexItem::Function:
                text.append(info->symbolType());
                if ( isSourceFile ) {
                    if ( CPlusPlus::Function *func = info->symbol()->asFunction() ) {
                        CPlusPlus::Overview o;
                        functionReturnType = o.prettyType(func->returnType());
                    }
                }
                break;
            }

            if ( !text.isEmpty() ) {
                CppHelperItem item( CppHelperItem::fromIndexType(info->type()),
                                    info->icon(),
                                    text,
                                    qualifiedName,
                                    isSourceFile,
                                    info->fileName(),
                                    functionReturnType,
                                    matchingPosition );
                items << item;
            }
            return IndexItem::Recurse;
        };

        while (it != m_snapshot.end()) {
            bool isSrcFile = isSourceFile(it.value()->fileName());
            search(it.value())->visitAllChildren(isSrcFile, filter);
            if ( !isSrcFile ) {
                const QList<CPlusPlus::Macro> &macros = it.value()->definedMacros();
                foreach ( const CPlusPlus::Macro &macro, macros )
                    if ( !matcher.indexIn(macro.name()) ) {
                        CppHelperItem item( CppHelperItem::Macro,
                                            QIcon(QLatin1String(":/codemodel/images/macro.png")),
                                            macro.name(),
                                            macro.name(),
                                            isSrcFile,
                                            it.value()->fileName(),
                                            "" );
                        items << item;
                    }
            }
            ++it;
            QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        }
    }

    void runSearch(QFutureInterface<Find::SearchResultItem> &future)
    {
        future.setProgressRange(0, m_snapshot.size());
        future.setProgressValue(0);
        int progress = 0;

        SearchSymbols search(CPlusPlus::CppModelManagerInterface::instance()->stringTable());
        search.setSymbolsToSearchFor(m_parameters.types);
        CPlusPlus::Snapshot::const_iterator it = m_snapshot.begin();

        QString findString = (m_parameters.flags & Find::FindRegularExpression
                              ? m_parameters.text : QRegExp::escape(m_parameters.text));
        if (m_parameters.flags & Find::FindWholeWords)
            findString = QString::fromLatin1("\\b%1\\b").arg(findString);
        QRegExp matcher(findString, (m_parameters.flags & Find::FindCaseSensitively
                                     ? Qt::CaseSensitive : Qt::CaseInsensitive));
        while (it != m_snapshot.end()) {
            if (future.isPaused())
                future.waitForResume();
            if (future.isCanceled())
                break;
            if (m_fileNames.isEmpty() || m_fileNames.contains(it.value()->fileName())) {
                QVector<Find::SearchResultItem> resultItems;
                auto filter = [&](bool /*isSourceFile*/, const IndexItem::Ptr &info) -> IndexItem::VisitorResult {
                    if (matcher.indexIn(info->symbolName()) != -1) {
                        QString text = info->symbolName();
                        QString scope = info->symbolScope();
                        if (info->type() == IndexItem::Function) {
                            QString name;
                            info->unqualifiedNameAndScope(info->symbolName(), &name, &scope);
                            text = name + info->symbolType();
                        } else if (info->type() == IndexItem::Declaration){
                            text = IndexItem::representDeclaration(info->symbolName(),
                                                                       info->symbolType());
                        }

                        Find::SearchResultItem item;
                        item.path = scope.split(QLatin1String("::"), QString::SkipEmptyParts);
                        item.text = text;
                        item.textMarkPos = -1;
                        item.textMarkLength = 0;
                        item.icon = info->icon();
                        item.lineNumber = -1;
                        item.userData = qVariantFromValue(info);
                        resultItems << item;
                    }

                    return IndexItem::Recurse;
                };
                search(it.value())->visitAllChildren(isSourceFile(it.value()->fileName()), filter);
                if (!resultItems.isEmpty())
                    future.reportResults(resultItems);
            }
            ++it;
            ++progress;
            future.setProgressValue(progress);
        }
        if (future.isPaused())
            future.waitForResume();
    }

private:
    const CPlusPlus::Snapshot m_snapshot;
    const Parameters m_parameters;
    const QSet<QString> m_fileNames;

    bool isSourceFile( const QString &fileName ) {
        QFileInfo info(fileName);
        QString extension = info.suffix().toLower();
        return m_sourceExtensions->contains(extension);
    }

    const QStringList *m_sourceExtensions;
    const QStringList *m_headerExtensions;
};

} // anonymous namespace

BuiltinIndexingSupport::BuiltinIndexingSupport()
    : m_revision(0)
    , m_extensionsInitialized(false)
{
    m_synchronizer.setCancelOnWait(true);
    m_dumpFileNameWhileParsing = DumpFileNameWhileParsing;
}

BuiltinIndexingSupport::~BuiltinIndexingSupport()
{}

QFuture<void> BuiltinIndexingSupport::refreshSourceFiles(const QStringList &sourceFiles)
{
    CppModelManager *mgr = CppModelManager::instance();
    const WorkingCopy workingCopy = mgr->workingCopy();

    CppPreprocessor *preproc = new CppPreprocessor(mgr);
    preproc->setRevision(++m_revision);
    preproc->setProjectFiles(mgr->projectFiles());
    preproc->setHeaderPaths(mgr->headerPaths());
    preproc->setWorkingCopy(workingCopy);

    QFuture<void> result = QtConcurrent::run(&parse, preproc, sourceFiles,
                                             sourceExtensions(), headerExtensions());

    if (m_synchronizer.futures().size() > 10) {
        QList<QFuture<void> > futures = m_synchronizer.futures();

        m_synchronizer.clearFutures();

        foreach (const QFuture<void> &future, futures) {
            if (!(future.isFinished() || future.isCanceled()))
                m_synchronizer.addFuture(future);
        }
    }

    m_synchronizer.addFuture(result);

    if (sourceFiles.count() > 1) {
        Core::Storage::mainWindow()->addTask( result, QObject::tr("Parsing C++ Files"),
                                              CppTools::Constants::TASK_PARSE );
    }

    return result;
}

SymbolSearcher *BuiltinIndexingSupport::createSymbolSearcher(SymbolSearcher::Parameters parameters, QSet<QString> fileNames)
{
    return new BuiltinSymbolSearcher( CppModelManager::instance()->snapshot(), parameters, fileNames,
                                      sourceExtensions(), headerExtensions() );
}

const QStringList *BuiltinIndexingSupport::sourceExtensions()
{
    initializeExtensions();
    return &m_sourceExtensions;
}

const QStringList *BuiltinIndexingSupport::headerExtensions()
{
    initializeExtensions();
    return &m_headerExtensions;
}

void BuiltinIndexingSupport::reinitializeExtensions()
{
    m_extensionsInitialized = false;
    initializeExtensions();
}

void BuiltinIndexingSupport::initializeExtensions()
{
    if ( m_extensionsInitialized )
        return;

    const Core::MimeDatabase *mimeDb = Core::Storage::mainWindow()->mimeDatabase();
    Core::MimeType cSourceTy = mimeDb->findByType(QLatin1String("text/x-csrc"));
    Core::MimeType cppSourceTy = mimeDb->findByType(QLatin1String("text/x-c++src"));
    Core::MimeType mSourceTy = mimeDb->findByType(QLatin1String("text/x-objcsrc"));
    Core::MimeType scSourceTy = mimeDb->findByType(QLatin1String("text/x-sc-src"));

    m_sourceExtensions = cSourceTy.suffixes();
    m_sourceExtensions += cppSourceTy.suffixes();
    m_sourceExtensions += mSourceTy.suffixes();
    m_sourceExtensions += scSourceTy.suffixes();
    m_sourceExtensions += "yy";   //! TODO:

    Core::MimeType hSourceTy = mimeDb->findByType(QLatin1String("text/x-chdr"));
    Core::MimeType hppSourceTy = mimeDb->findByType(QLatin1String("text/x-c++hdr"));

    m_headerExtensions = hSourceTy.suffixes();
    m_headerExtensions += hppSourceTy.suffixes();
    m_headerExtensions += "inl";
    m_headerExtensions += "";

    m_extensionsInitialized = true;
}
