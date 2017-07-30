#include <QDir>
#include <QDebug>

#include "KarelDocument.h"
#include "KarelAST.h"
#include "KarelBind.h"

namespace Karel {

class DocumentDiagnosticClient : public DiagnosticClient
{
    enum { MAX_MESSAGE_COUNT = 20 };

public:
    DocumentDiagnosticClient(Document *doc, QList<Utils::DiagnosticMessage> *messages_)
        : doc(doc)
        , messages(messages_)
        , errorCount(0)
    { }

    virtual void report( int level,
                         const char * /*fileId*/,
                         unsigned line, unsigned column, unsigned length,
                         const char *format, va_list ap)
    {
        if (level == Error) {
            ++errorCount;

            if (errorCount >= MAX_MESSAGE_COUNT)
                return; // ignore the error
        }

        QString message;
        message.vsprintf(format, ap);

        Utils::DiagnosticMessage m( convertLevel(level), doc->fileName(),
                                    line, column, message, length );
        messages->append(m);
    }

    virtual void reportString( int level,
                               const char * /*fileId*/,
                               unsigned line, unsigned column, unsigned length,
                               const QString &message )
    {
        if (level == Error) {
            ++errorCount;

            if (errorCount >= MAX_MESSAGE_COUNT)
                return; // ignore the error
        }

        Utils::DiagnosticMessage m( convertLevel(level), doc->fileName(),
                                    line, column, message, length );
        messages->append(m);
    }

    static int convertLevel(int level) {
        switch (level) {
            case Warning: return Utils::DiagnosticMessage::Warning;
            case Error:   return Utils::DiagnosticMessage::Error;
            case Fatal:   return Utils::DiagnosticMessage::Fatal;
            default:      return Utils::DiagnosticMessage::Error;
        }
    }

private:
    Document *doc;
    QList<Utils::DiagnosticMessage> *messages;
    int errorCount;
};

Document::Document(const QString &fileName)
    : _fileName(QDir::cleanPath(fileName))
    , _revision(0)
    , _editorRevision(0)
    , _globalScope(0)
{
    _control = new Control();

    _control->setDiagnosticClient(new DocumentDiagnosticClient(this, &_diagnosticMessages));

    const QByteArray localFileName = fileName.toUtf8();
    const StringLiteral *fileId = new StringLiteral(localFileName.constData(), localFileName.size());
    _translationUnit = new TranslationUnit(_control, fileId);
    (void) _control->switchTranslationUnit(_translationUnit);
}

Document::Ptr Document::create(const QString &fileName)
{
    Document::Ptr doc(new Document(fileName));
    return doc;
}

Document::~Document()
{
    delete _control;
}

unsigned Document::revision() const
{ return _revision; }

void Document::setRevision(const unsigned &revision)
{ _revision = revision; }

unsigned Document::editorRevision() const
{ return _editorRevision; }

void Document::setEditorRevision(unsigned editorRevision)
{ _editorRevision = editorRevision; }

QString Document::fileName() const
{ return _fileName; }

QDateTime Document::lastModified() const
{ return _lastModified; }

void Document::setLastModified(const QDateTime &lastModified)
{ _lastModified = lastModified; }

Control *Document::control() const
{ return _control; }

TranslationUnit *Document::translationUnit() const
{ return _translationUnit; }

QByteArray Document::source() const
{ return _source; }

void Document::setSource(const QByteArray &source)
{
    _source = source;
    _translationUnit->setSource(_source.constBegin(), _source.size());
}

void Document::setSource(const char *source, unsigned size)
{ _translationUnit->setSource(source, size); }

bool Document::isTokenized() const
{ return _translationUnit->isTokenized(); }

void Document::tokenize()
{ _translationUnit->tokenize(); }

bool Document::isParsed() const
{ return _translationUnit->isParsed(); }

bool Document::parse(TranslationUnit::ParseMode mode)
{ return _translationUnit->parse(mode); }

void Document::check(Document::CheckMode mode)
{
    if ( !isParsed() )
        parse();

    if ( !_translationUnit->ast() )
        return;

    if ( ModuleAST *ast = _translationUnit->ast()->asModule() ) {
        _globalScope = _control->scope(ast->firstToken());
        _globalScope->setAst(ast);
        ast->scope = _globalScope;
        Bind semantic(_translationUnit);
        semantic(ast, _globalScope);
//        _globalScope->dump();
    }
}

Scope *Document::globalScope() const
{ return _globalScope; }

}   // namespace Karel
