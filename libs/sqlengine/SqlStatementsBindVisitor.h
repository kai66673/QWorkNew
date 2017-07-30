#ifndef SQLSTATEMENTSBINDVISITOR_H
#define SQLSTATEMENTSBINDVISITOR_H

#include "SqlStatement.h"
#include "ASTVisitor.h"
#include "AST.h"
#include "sql_constants.h"
#include "sqlengine_global.h"

class QItext;

namespace Sql {

class SQL_EXPORT SqlStatementsBindVisitor: public ASTVisitor
{
public:
    SqlStatementsBindVisitor( TranslationUnit *unit )
        : ASTVisitor(unit)
    {}

    void fillStatementsList( QList<SqlStatement *> *statements, TranslationUnitAST *ast,
                             Database::ExecuteType executeType, ITextDocument *textDocument );
    void fillStatementFromText( QList<SqlStatement *> *statements, TranslationUnitAST *ast,
                                const QString &statementText );

protected:
    virtual bool visit( ParamNameAST *ast );

private:
    QList<SqlStatement *> *m_statements;
    SqlStatement *m_currentStatement;
};

}   // namespace Sql

#endif // SQLSTATEMENTSBINDVISITOR_H
