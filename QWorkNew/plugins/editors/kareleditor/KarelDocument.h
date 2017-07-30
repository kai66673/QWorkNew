#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QString>
#include <QDateTime>

#include "DiagnosticMessage.h"
#include "TranslationUnit.h"
#include "Control.h"

namespace Karel {

class Document
{
    Document( const Document &other );
    void operator =( const Document &other );
    Document( const QString &fileName );

public:
    typedef QSharedPointer<Document> Ptr;
    static Ptr create( const QString &fileName );

public:
    virtual ~Document();

    unsigned revision() const;
    void setRevision(const unsigned &revision);

    unsigned editorRevision() const;
    void setEditorRevision(unsigned editorRevision);

    QString fileName() const;

    QDateTime lastModified() const;
    void setLastModified(const QDateTime &lastModified);

    Control *control() const;
    TranslationUnit *translationUnit() const;

    QByteArray source() const;
    void setSource(const QByteArray &source);
    void setSource(const char *source, unsigned size);
    
    bool isTokenized() const;
    void tokenize();

    bool isParsed() const;
    bool parse(TranslationUnit::ParseMode mode = TranslationUnit::ParseModule);

    enum CheckMode {
        FullCheck,
        FastCheck
    };
    void check(CheckMode mode = FullCheck);

    void addDiagnosticMessage( const Utils::DiagnosticMessage &d )
    { _diagnosticMessages.append(d); }

    QList<Utils::DiagnosticMessage> diagnosticMessages() const
    { return _diagnosticMessages; }

    Scope *globalScope() const;

private:
    QString _fileName;
    Control *_control;
    TranslationUnit *_translationUnit;
    QByteArray _source;
    QDateTime _lastModified;
    unsigned _revision;
    unsigned _editorRevision;
    QList<Utils::DiagnosticMessage> _diagnosticMessages;
    Scope *_globalScope;
};

}   // namespace Karel

#endif // DOCUMENT_H
