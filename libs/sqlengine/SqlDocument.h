#ifndef SQLDOCUMENT_H
#define SQLDOCUMENT_H

#include <QSharedPointer>
#include <QDateTime>

#include "sqlengine_global.h"
#include "Control.h"
#include "SqlStatement.h"
#include "IParserEngine.h"
#include "ILink.h"
#include "DiagnosticMessage.h"

namespace Sql {

class SQL_EXPORT Document
{
    Document( const Document &other );
    void operator =( const Document &other );
    Document( const QString &fileName );

public:
    typedef QSharedPointer<Document> Ptr;
    static Ptr create( const QString &fileName );

public:
    ~Document();

    unsigned revision() const;
    void setRevision( unsigned revision );

    unsigned editorRevision() const;
    void setEditorRevision( unsigned editorRevision );

    QDateTime lastModified() const;
    void setLastModified( const QDateTime &lastModified );

    QString fileName() const;

    Control *control() const;
    TranslationUnit *translationUnit() const;

    QByteArray source() const;
    void setSource( const QByteArray &source );
    void setSource( const char *source, unsigned size );

    enum ParseMode { // ### keep in sync with CPlusPlus::TranslationUnit
        ParseTranlationUnit,
        ParseStatement,
        ParseDmlStatement
    };

    bool isTokenized() const;
    void tokenize( IParserEngine *engine );

    bool isParsed() const;
    void parseDocument( ParseTarget target, IParserEngine *engine );
    void fillSqlStatements( QList<SqlStatement *> *statements, IParserEngine *engine,
                            Database::ExecuteType executeType, ITextDocument *textDocument );
    void fillStatementFromText( QList<SqlStatement *> *statements, IParserEngine *engine,
                                const QString &statementText );
    int fillCompletions( QList<TextEditor::BasicProposalItem *> *completions, IParserEngine *engine,
                         unsigned position, int startPosition, unsigned completionOperator );
    Core::ILink *findLinkAt( const QTextCursor &tc, IParserEngine *engine );

    void releaseSource();
    void releaseTranslationUnit();

    void addDiagnosticMessage( const Utils::DiagnosticMessage &d )
    { _diagnosticMessages.append(d); }

    QList<Utils::DiagnosticMessage> diagnosticMessages() const
    { return _diagnosticMessages; }

private:
    QString _fileName;
    Control *_control;
    TranslationUnit *_translationUnit;
    QByteArray _source;
    QDateTime _lastModified;
    unsigned _revision;
    unsigned _editorRevision;
    QList<Utils::DiagnosticMessage> _diagnosticMessages;
};

} // namespace Sql

Q_DECLARE_METATYPE(Sql::Document::Ptr *)

#endif // SQLDOCUMENT_H
