/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**************************************************************************/

#include "cppfindreferences.h"
#include "cpptoolsconstants.h"

#include <texteditor.h>
#include <basefilefind.h>
#include <searchresultwindow.h>
#include <filesearch.h>
#include <qtcfileutils.h>
#include <progressmanager.h>
#include <futureprogress.h>

#include <ASTVisitor.h>
#include <AST.h>
#include <Control.h>
#include <Literals.h>
#include <TranslationUnit.h>
#include <Symbols.h>
#include <Names.h>
#include <Scope.h>

#include <ModelManagerInterface.h>
#include <CppDocument.h>
#include <Overview.h>
#include <FindUsages.h>

#include "Core.h"
#include "IDocument.h"
#include "ITextDocument.h"
#include "SearchEngine.h"

#include <QtCore/QTime>
#include <QtCore/QTimer>
#ifndef HAVE_QT5
#include <QtCore/QtConcurrentRun>
#include <QtCore/QtConcurrentMap>
#else
#include <QtConcurrent/QtConcurrentRun>
#include <QtConcurrent/QtConcurrentMap>
#endif
#include <QtCore/QDir>
#include <QApplication>
#include <runextensions.h>

#include <functional>

#include "CppReplaceUsagesDialog.h"

using namespace CppTools::Internal;
using namespace CPlusPlus;

static QString getSource(const QString &fileName,
                         const CppModelManagerInterface::WorkingCopy &workingCopy)
{
    if (workingCopy.contains(fileName)) {
        return workingCopy.source(fileName);
    } else {
        Utils::FileReader reader;
        if (!reader.fetch(fileName, QFile::Text)) // ### FIXME error reporting
            return QString();

        return QString::fromLocal8Bit(reader.data()); // ### FIXME encoding
    }
}

namespace {

class ProcessFile: public std::unary_function<QString, QList<Usage> >
{
    const CppModelManagerInterface::WorkingCopy workingCopy;
    const Snapshot snapshot;
    Document::Ptr symbolDocument;
    Symbol *symbol;

public:
    ProcessFile(const CppModelManagerInterface::WorkingCopy &workingCopy,
                const Snapshot snapshot,
                Document::Ptr symbolDocument,
                Symbol *symbol)
        : workingCopy(workingCopy), snapshot(snapshot), symbolDocument(symbolDocument), symbol(symbol)
    { }

    QList<Usage> operator()(const QString &fileName)
    {
        QList<Usage> usages;
        const Identifier *symbolId = symbol->identifier();

        if (Document::Ptr previousDoc = snapshot.document(fileName)) {
            Control *control = previousDoc->control();
            if (! control->findIdentifier(symbolId->chars(), symbolId->size()))
                return usages; // skip this document, it's not using symbolId.
        }

        Document::Ptr doc;
        QByteArray source;
        const QString unpreprocessedSource = getSource(fileName, workingCopy);

        if (symbolDocument && fileName == symbolDocument->fileName())
            doc = symbolDocument;
        else {
            source = snapshot.preprocessedCode(unpreprocessedSource, fileName);
            doc = snapshot.documentFromSource(source, fileName);
            doc->tokenize();
        }

        Control *control = doc->control();
        if (control->findIdentifier(symbolId->chars(), symbolId->size()) != 0) {
            if (doc != symbolDocument)
                doc->check();

            FindUsages process(unpreprocessedSource.toUtf8(), doc, snapshot);
            process(symbol);

            usages = process.usages();
        }

        return usages;
    }
};

class UpdateUI: public std::binary_function<QList<Usage> &, QList<Usage>, void>
{
    QFutureInterface<Usage> *future;

public:
    UpdateUI(QFutureInterface<Usage> *future): future(future) {}

    void operator()(QList<Usage> &, const QList<Usage> &usages)
    {
        foreach (const Usage &u, usages)
            future->reportResult(u);

        future->setProgressValue(future->progressValue() + 1);
    }
};

} // end of anonymous namespace

CppFindReferences::CppFindReferences(CppModelManagerInterface *modelManager)
    : /*QObject(modelManager),*/
      ISearchProcess(),
      _modelManager(modelManager),
      _resultWindow(Find::SearchResultWindow::instance())
{
    m_watcher.setPendingResultsLimit(1);
    connect(&m_watcher, SIGNAL(resultsReadyAt(int,int)), this, SLOT(displayResults(int,int)));
    connect(&m_watcher, SIGNAL(finished()), this, SIGNAL(finished()));
    connect(&m_watcher, SIGNAL(finished()), this, SLOT(searchFinished()));
}

CppFindReferences::~CppFindReferences()
{ }

QList<int> CppFindReferences::references(Symbol *symbol, const LookupContext &context) const
{
    QList<int> references;

    FindUsages findUsages(context);
    findUsages(symbol);
    references = findUsages.references();

    return references;
}

static void find_helper(QFutureInterface<Usage> &future,
                        const CppModelManagerInterface::WorkingCopy workingCopy,
                        const LookupContext context,
                        CppFindReferences *findRefs,
                        Symbol *symbol)
{
    const Identifier *symbolId = symbol->identifier();
    Q_ASSERT(symbolId != 0);

    const Snapshot snapshot = context.snapshot();

    const QString sourceFile = QString::fromUtf8(symbol->fileName(), symbol->fileNameLength());
    QStringList files(sourceFile);

    if (symbol->isClass() || symbol->isForwardClassDeclaration() || (symbol->enclosingScope() && ! symbol->isStatic() &&
                                                                     symbol->enclosingScope()->isNamespace())) {
        foreach (const Document::Ptr &doc, context.snapshot()) {
            if (doc->fileName() == sourceFile)
                continue;

            Control *control = doc->control();

            if (control->findIdentifier(symbolId->chars(), symbolId->size()))
                files.append(doc->fileName());
        }
    } else {
        DependencyTable dependencyTable = findRefs->updateDependencyTable(snapshot);
        files += dependencyTable.filesDependingOn(sourceFile);
    }
    files.removeDuplicates();

    future.setProgressRange(0, files.size());

    ProcessFile process(workingCopy, snapshot, context.thisDocument(), symbol);
    UpdateUI reduce(&future);

    QtConcurrent::blockingMappedReduced<QList<Usage> > (files, process, reduce);

    future.setProgressValue(files.size());
}

void CppFindReferences::findUsages(CPlusPlus::Symbol *symbol, const CPlusPlus::LookupContext &context)
{
    _resultWindow = SearchEngine::getInstance()->newSearchResultWindow(symbol->identifier()->chars(), QIcon(":/cppeditor/images/find_usages.png"), this);

    Find::SearchResult *search = _resultWindow->startNewSearch(Find::SearchResultWindow::SearchOnly);

    connect(search, SIGNAL(activated(Find::SearchResultItem)),
            this, SLOT(openEditor(Find::SearchResultItem)));

    findAll_helper(symbol, context, false);
}

void CppFindReferences::renameUsages(CPlusPlus::Symbol *symbol, const CPlusPlus::LookupContext &context,
                                     const QString &replacement, Find::SearchResultWindow *resultWindow)
{
    if (const Identifier *id = symbol->identifier()) {
        const QString textToReplace = replacement.isEmpty()
                ? QString::fromUtf8(id->chars(), id->size()) : replacement;

        if ( !resultWindow ) {
            CppReplaceUsagesDialog replaceDialog(symbol, context, textToReplace, this, Core::Storage::mainWindow()->widget());
            connect(&replaceDialog, SIGNAL(searchCanceled()), this, SLOT(cancelSearch()));
            connect( &replaceDialog, SIGNAL(replaceButtonClicked(QString,QList<Find::SearchResultItem>)),
                     this, SLOT(replaceButtonClicked(QString,QList<Find::SearchResultItem>)));
            replaceDialog.exec();
            return;
        }

        _resultWindow = resultWindow;

        Find::SearchResult *search = _resultWindow->startNewSearch(Find::SearchResultWindow::SearchAndReplace);
        connect(search, SIGNAL(activated(Find::SearchResultItem)),
                this, SLOT(openEditor(Find::SearchResultItem)));

        findAll_helper(symbol, context);
    }
}

void CppFindReferences::findAll_helper(Symbol *symbol, const LookupContext &context, bool signAppStatusBar)
{
    if (! (symbol && symbol->identifier()))
        return;

    _resultWindow->popup(true);

    const CppModelManagerInterface::WorkingCopy workingCopy = _modelManager->workingCopy();

    QFuture<Usage> result;

    result = QtConcurrent::run(&find_helper, workingCopy, context, this, symbol);
    m_watcher.setFuture(result);

    QString taskTitle = tr("Searching refs \"%1\"").arg(symbol->name()->identifier()->chars());
    Core::FutureProgress *progress = Core::Storage::mainWindow()->addTask( result, taskTitle,
                                                                           CppTools::Constants::TASK_SEARCH,
                                                                           signAppStatusBar );

    if ( !signAppStatusBar ) {
        SearchEngine::getInstance()->setCurrentSearchProgressWidget(progress);
        connect(progress, SIGNAL(removeMe()), SearchEngine::getInstance(), SLOT(onAnySearchProgressFinished()));
    }

    connect(progress, SIGNAL(clicked()), _resultWindow, SLOT(popup()));
}

void CppFindReferences::replaceButtonClicked( const QString &text,
                                              const QList<Find::SearchResultItem> &items )
{
//    Core::EditorManager::instance()->hideEditorInfoBar(QLatin1String("CppEditor.Rename"));

    const QStringList fileNames = TextEditor::BaseFileFind::replaceAll(text, items);
    if (!fileNames.isEmpty()) {
        _modelManager->updateSourceFiles(fileNames);
        _resultWindow->hide();
    }
}

void CppFindReferences::displayResults(int first, int last)
{
    //if (!m_currentSearch) {
    //    m_watcher.cancel();
    //    return;
    //}
    for (int index = first; index != last; ++index) {
        Usage result = m_watcher.future().resultAt(index);
        _resultWindow->addResult(result.path,
                                 result.line,
                                 result.lineText,
                                 result.col,
                                 result.len);
    }
}

void CppFindReferences::searchFinished()
{
    _resultWindow->finishSearch();
    emit changed();
}

void CppFindReferences::cancel()
{
    m_watcher.cancel();
}

void CppFindReferences::openEditor(const Find::SearchResultItem &item)
{
    if (item.path.size() > 0) {
        DocumentManager::getInstance()->openDocument( item.path.first(),
                                                      new TextDocumentCursor(item.lineNumber - 1, item.textMarkPos) );
    } else {
        DocumentManager::getInstance()->openDocument(item.text);
    }
}


namespace {

class FindMacroUsesInFile: public std::unary_function<QString, QList<Usage> >
{
    const CppModelManagerInterface::WorkingCopy workingCopy;
    const Snapshot snapshot;
    const Macro &macro;

public:
    FindMacroUsesInFile(const CppModelManagerInterface::WorkingCopy &workingCopy,
                        const Snapshot snapshot,
                        const Macro &macro)
        : workingCopy(workingCopy), snapshot(snapshot), macro(macro)
    { }

    QList<Usage> operator()(const QString &fileName)
    {
        QList<Usage> usages;

        const Document::Ptr &doc = snapshot.document(fileName);
        QByteArray source;

        foreach (const Document::MacroUse &use, doc->macroUses()) {
            const Macro &useMacro = use.macro();
            if (useMacro.line() == macro.line()
                && useMacro.fileName() == macro.fileName())
                {
                if (source.isEmpty())
                    source = getSource(fileName, workingCopy).toLatin1(); // ### FIXME: Encoding?

                unsigned lineStart;
                const QString &lineSource = matchingLine(use.begin(), source, &lineStart);
                usages.append(Usage(fileName, lineSource, use.beginLine(),
                                    use.begin() - lineStart, use.length()));
            }
        }

        return usages;
    }

    // ### FIXME: Pretty close to FindUsages::matchingLine.
    static QString matchingLine(unsigned position, const QByteArray &source,
                                unsigned *lineStart = 0)
    {
        const char *beg = source.constData();
        const char *start = beg + position;
        for (; start != beg - 1; --start) {
            if (*start == '\n')
                break;
        }

        ++start;

        const char *end = start + 1;
        for (; *end; ++end) {
            if (*end == '\n')
                break;
        }

        if (lineStart)
            *lineStart = start - beg;

        // ### FIXME: Encoding?
        const QString matchingLine = QString::fromUtf8(start, end - start);
        return matchingLine;
    }
};

} // end of anonymous namespace

static void findMacroUses_helper(QFutureInterface<Usage> &future,
                                 const CppModelManagerInterface::WorkingCopy workingCopy,
                                 const Snapshot snapshot,
                                 CppFindReferences *findRefs,
                                 const Macro macro)
{
    // ensure the dependency table is updated
    DependencyTable dependencies = findRefs->updateDependencyTable(snapshot);

    const QString& sourceFile = macro.fileName();
    QStringList files(sourceFile);
    files += dependencies.filesDependingOn(sourceFile);
    files.removeDuplicates();

    future.setProgressRange(0, files.size());

    FindMacroUsesInFile process(workingCopy, snapshot, macro);
    UpdateUI reduce(&future);
    QtConcurrent::blockingMappedReduced<QList<Usage> > (files, process, reduce);

    future.setProgressValue(files.size());
}

void CppFindReferences::findMacroUses(const Macro &macro)
{
    _resultWindow = SearchEngine::getInstance()->newSearchResultWindow(macro.name(), QIcon(":/cppeditor/images/find_usages.png"), this);

    Find::SearchResult *search = _resultWindow->startNewSearch(Find::SearchResultWindow::SearchOnly);

    _resultWindow->popup(true);

    connect(search, SIGNAL(activated(Find::SearchResultItem)),
            this, SLOT(openEditor(Find::SearchResultItem)));

    const Snapshot snapshot = _modelManager->snapshot();
    const CppModelManagerInterface::WorkingCopy workingCopy = _modelManager->workingCopy();

    // add the macro definition itself
    {
        // ### FIXME: Encoding?
        const QByteArray &source = getSource(macro.fileName(), workingCopy).toLatin1();
        _resultWindow->addResult(macro.fileName(), macro.line(),
                                 source.mid(macro.offset(), macro.length()), 0, macro.length());
    }

    QFuture<Usage> result;
    result = QtConcurrent::run(&findMacroUses_helper, workingCopy, snapshot, this, macro);
    m_watcher.setFuture(result);

    QString taskTitle = tr("Searching macro \"%1\" uses").arg(QString::fromLatin1(macro.name()));
    Core::FutureProgress *progress = Core::Storage::mainWindow()->addTask( result, taskTitle,
                                                                           CppTools::Constants::TASK_SEARCH, false );

    SearchEngine::getInstance()->setCurrentSearchProgressWidget(progress);
    connect(progress, SIGNAL(removeMe()), SearchEngine::getInstance(), SLOT(onAnySearchProgressFinished()));
    connect(progress, SIGNAL(clicked()), _resultWindow, SLOT(popup()));
}

DependencyTable CppFindReferences::updateDependencyTable(CPlusPlus::Snapshot snapshot)
{
    DependencyTable oldDeps = dependencyTable();
    if (oldDeps.isValidFor(snapshot))
        return oldDeps;

    DependencyTable newDeps;
    newDeps.build(snapshot);
    setDependencyTable(newDeps);
    return newDeps;
}

DependencyTable CppFindReferences::dependencyTable() const
{
    QMutexLocker locker(&m_depsLock);
    Q_UNUSED(locker);
    return m_deps;
}

void CppFindReferences::setDependencyTable(const CPlusPlus::DependencyTable &newTable)
{
    QMutexLocker locker(&m_depsLock);
    Q_UNUSED(locker);
    m_deps = newTable;
}

void CppFindReferences::cancelSearch()
{

    disconnect(&m_watcher, SIGNAL(finished()), this, SLOT(searchFinished()));
    disconnect(&m_watcher, SIGNAL(resultsReadyAt(int,int)), this, SLOT(displayResults(int,int)));
    disconnect(&m_watcher, SIGNAL(finished()), this, SIGNAL(finished()));

    m_watcher.cancel();
}
