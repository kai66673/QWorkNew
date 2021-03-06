#include "PostgresSqlStatementsBindVisitor.h"

namespace Postgres {

void SqlStatementsBindVisitor::fillStatementsList( QList<Sql::SqlStatement *> *statements, TranslationUnitAST *ast,
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

    for ( StatementListAST *iter = ast->statements; iter; iter = iter->next ) {
        StatementAST *statement = iter->value;
        qDebug() << "~>>" << statement;
        if ( !statement )
            continue;
        qDebug() << "~~" << statement->start_line << start_line << end_line << statement->end_line;
        qDebug() << "~~" << statement->start_pos << start_pos << end_pos << statement->end_pos;
        if ( executeType != Database::AllDocument ) {
            if ( statement->end_line < start_line || (statement->end_line == start_line && statement->end_pos < start_pos) )
                continue;

            if ( statement->start_line > end_line || (statement->start_line == end_line && statement->start_pos > end_pos) )
                break;
        }
        QString txt = textDocument->getText( statement->start_line, statement->start_pos,
                                             statement->end_line, statement->end_pos ).trimmed();
//        if ( txt.endsWith(QLatin1Char(';')) && !statement->asPlSqlBlock() )
//            txt.chop(1);
        bool isCommit = false;
        bool isRollback = false;
        if ( statement->asTransactionStmt() ) {
            isCommit = statement->asCommitTransactionStmt() || statement->asReleaseTransactionStmt();
            isRollback = statement->asRollbackTransactionStmt();
        }
        m_currentStatement = new Sql::SqlStatement( statement->asSelectStmt(),
                                                    statement->asDdlStatement(),
                                                    /*statement->asBlockStatement()*/false,
                                                    isCommit,
                                                    isRollback,
                                                    txt );
        m_statements->append(m_currentStatement);
        statement->accept(this);
    }
}

void SqlStatementsBindVisitor::fillStatementFromText( QList<Sql::SqlStatement *> *statements, TranslationUnitAST *ast,
                                                      const QString &statementText )
{
    if ( !ast )
        return;

    m_statements = statements;
    m_currentStatement = 0;

    for ( StatementListAST *iter = ast->statements; iter; iter = iter->next ) {
        StatementAST *statement = iter->value;
        if ( !statement )
            continue;
        bool isCommit = false;
        bool isRollback = false;
        if ( statement->asTransactionStmt() ) {
            isCommit = statement->asCommitTransactionStmt() || statement->asReleaseTransactionStmt();
            isRollback = statement->asRollbackTransactionStmt();
        }
        m_currentStatement = new Sql::SqlStatement( statement->asSelectStmt(),
                                                    statement->asDdlStatement(),
                                                    /*statement->asBlockStatement()*/false,
                                                    isCommit,
                                                    isRollback,
                                                    statementText /* TODO: split text to Statements */ );
        m_statements->append(m_currentStatement);
        statement->accept(this);
    }
}

bool SqlStatementsBindVisitor::visit( NameAST *ast )
{
    if ( m_currentStatement && ast->kind == NameAST::Binding )
        m_currentStatement->bindNames.append( QString(ast->name->chars()) );
    return true;
}

}   // namespace Postgres
