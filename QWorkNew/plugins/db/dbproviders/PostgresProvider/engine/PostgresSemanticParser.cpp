#include "PostgresSemanticParser.h"
#include "PostgresSqlStatementsBindVisitor.h"
#include "PostgresSqlCompletionAssistVisitor.h"
#include "PostgresSqlLinkFinder.h"
#include "PostgresAST.h"
#include "PostgresDriver.h"

namespace Postgres {

PostgresSemanticParser::PostgresSemanticParser( Sql::TranslationUnit *translationUnit )
    : Sql::ISemanticParser()
    , m_translationUnit(translationUnit)
{ }

PostgresSemanticParser::~PostgresSemanticParser()
{ }

Sql::BaseWalker *PostgresSemanticParser::parseDocument( Sql::ParseTarget target )
{
    switch ( target ) {
        case Sql::ParseSqlNames: {
            PostgresDriver driver(m_translationUnit);
            if ( TranslationUnitAST *node = driver.parse() )
                return new SqlCheckNamesWalker(node, m_translationUnit);
            return 0;
        }
    }

    return 0;
}

void PostgresSemanticParser::fillSqlStatements( QList<Sql::SqlStatement *> *statements,
                                               Database::ExecuteType executeType, ITextDocument *textDocument )
{
    PostgresDriver driver(m_translationUnit);
    if ( TranslationUnitAST *node = driver.parse() ) {
        SqlStatementsBindVisitor visitor(m_translationUnit);
        visitor.fillStatementsList(statements, node, executeType, textDocument);
    }
}

void PostgresSemanticParser::fillStatementFromText( QList<Sql::SqlStatement *> *statements,
                                                   const QString &statementText )
{
    PostgresDriver driver(m_translationUnit);
    if ( TranslationUnitAST *node = driver.parse() ) {
        SqlStatementsBindVisitor visitor(m_translationUnit);
        visitor.fillStatementFromText(statements, node, statementText);
    }
}

int PostgresSemanticParser::fillCompletions( QList<TextEditor::BasicProposalItem *> *completions,
                                            unsigned position, int startPosition, unsigned completionOperator )
{
    PostgresDriver driver(m_translationUnit);
    if ( TranslationUnitAST *node = driver.parse() ) {
        SqlCompletionAssistVisitor visitor(m_translationUnit);
        return visitor.fillCompletions(completions, node, position, startPosition, completionOperator);
    }
    return -1;
}

Core::ILink *PostgresSemanticParser::findLinkAt( const QTextCursor &tc )
{
    PostgresDriver driver(m_translationUnit);
    if ( TranslationUnitAST *node = driver.parse() ) {
        SqlLinkFinder finder(m_translationUnit);
        return finder.findLinkAt(node, tc.position());
    }
    return 0;
}

}   // Postgres
