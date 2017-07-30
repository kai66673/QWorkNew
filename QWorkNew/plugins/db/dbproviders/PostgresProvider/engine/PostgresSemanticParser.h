#ifndef POTGRESSEMANTICPARSER_H
#define POTGRESSEMANTICPARSER_H

#include "ISemanticParser.h"

namespace Postgres {

class PostgresSemanticParser: public Sql::ISemanticParser
{
public:
    PostgresSemanticParser( Sql::TranslationUnit *translationUnit );
    virtual ~PostgresSemanticParser();

    virtual Sql::BaseWalker *parseDocument( Sql::ParseTarget target );
    virtual void fillSqlStatements( QList<Sql::SqlStatement *> *statements,
                                    Database::ExecuteType executeType, ITextDocument *textDocument );
    virtual void fillStatementFromText( QList<Sql::SqlStatement *> *statements, const QString &statementText );
    virtual int fillCompletions( QList<TextEditor::BasicProposalItem *> *completions, unsigned position,
                                 int startPosition, unsigned completionOperator );
    virtual Core::ILink *findLinkAt( const QTextCursor &tc );

private:
    Sql::TranslationUnit *m_translationUnit;
};

}   // namespace Postgres

#endif // POTGRESSEMANTICPARSER_H
