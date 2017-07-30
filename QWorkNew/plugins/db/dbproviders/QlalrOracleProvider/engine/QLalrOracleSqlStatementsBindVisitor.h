#ifndef QLALR_ORACLESQLSTATEMENTSBINDVISITOR_H
#define QLALR_ORACLESQLSTATEMENTSBINDVISITOR_H

#include "ITextDocument.h"
#include "SqlStatement.h"
#include "QLalrOracleASTVisitor.h"
#include "QLalrOracleAST.h"
#include "sql_constants.h"
#include "TranslationUnit.h"

class QItext;

namespace QLalrOracle {

class SqlStatementsBindVisitor: public ASTVisitor
{
public:
    SqlStatementsBindVisitor( Sql::TranslationUnit *unit )
        : ASTVisitor(unit)
    {}

    void fillStatementsList( QList<Sql::SqlStatement *> *statements, TranslationUnitAST *ast,
                             Database::ExecuteType executeType, ITextDocument *textDocument );
    void fillStatementFromText( QList<Sql::SqlStatement *> *statements, TranslationUnitAST *ast,
                                const QString &statementText );

protected:
    virtual bool visit( NameAST *ast );

private:
    QList<Sql::SqlStatement *> *m_statements;
    Sql::SqlStatement *m_currentStatement;
};

}   // namespace QLalrOracle

#endif // QLALR_ORACLESQLSTATEMENTSBINDVISITOR_H
