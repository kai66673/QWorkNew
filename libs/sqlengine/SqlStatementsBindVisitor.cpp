#include "SqlStatementsBindVisitor.h"
#include "ITextDocument.h"

namespace Sql {

void SqlStatementsBindVisitor::fillStatementsList( QList<SqlStatement *> *statements, TranslationUnitAST *ast,
                                                   Database::ExecuteType executeType, ITextDocument *textDocument )
{
    if ( !ast )
        return;

    m_statements = statements;
    m_currentStatement = 0;

    int startPos = textDocument->selectionStart();
    int endPos   = textDocument->selectionEnd();
    unsigned start_line;
    unsigned start_pos;
    unsigned end_line;
    unsigned end_pos;
    textDocument->convertPosition(startPos, &start_line, &start_pos);
    textDocument->convertPosition(endPos, &end_line, &end_pos);
    start_line--; end_line--;
    start_pos++; end_pos++;

    for ( StatementListAST *iter = ast->statement_list; iter; iter = iter->next ) {
        StatementAST *statement = iter->value;
        if ( !statement )
            continue;
        if ( executeType != Database::AllDocument ) {
            if ( statement->end_line < start_line || (statement->end_line == start_line && statement->end_pos < start_pos) )
                continue;

            if ( statement->start_line > end_line || (statement->start_line == end_line && statement->start_pos > end_pos) )
                break;
        }
        m_currentStatement = new SqlStatement( statement->isSelectStatement(),
                                               statement->asDdlStatement(),
                                               false,
                                               statement->asCommitStatement(),
                                               statement->asRollbackStatement(),
                                               textDocument->getText( statement->start_line, statement->start_pos,
                                                                      statement->end_line, statement->end_pos ) );
        m_statements->append(m_currentStatement);
        statement->accept(this);
    }
}

void SqlStatementsBindVisitor::fillStatementFromText( QList<SqlStatement *> *statements, TranslationUnitAST *ast,
                                                      const QString &statementText )
{
    if ( !ast )
        return;

    m_statements = statements;
    m_currentStatement = 0;

    for ( StatementListAST *iter = ast->statement_list; iter; iter = iter->next ) {
        StatementAST *statement = iter->value;
        if ( !statement )
            continue;
        m_currentStatement = new SqlStatement( statement->isSelectStatement(),
                                               statement->asDdlStatement(),
                                               false,
                                               statement->asCommitStatement(),
                                               statement->asRollbackStatement(),
                                               statementText /* TODO: split text to Statements */ );
        m_statements->append(m_currentStatement);
        statement->accept(this);
    }
}

bool SqlStatementsBindVisitor::visit( ParamNameAST *ast )
{
    if ( m_currentStatement )
        m_currentStatement->bindNames.append( QString(ast->name->chars()) );
    return true;
}

}   // namespace Sql
