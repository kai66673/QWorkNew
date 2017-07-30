#ifndef RDBSQLSTATEMENTSBINDVISITOR_H
#define RDBSQLSTATEMENTSBINDVISITOR_H

#include "ITextDocument.h"
#include "SqlStatement.h"
#include "RdbASTVisitor.h"
#include "RdbAST.h"
#include "sql_constants.h"
#include "TranslationUnit.h"

class QItext;

namespace RDB {

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
    virtual bool visit( ParamNameAST *ast );

private:
    QList<Sql::SqlStatement *> *m_statements;
    Sql::SqlStatement *m_currentStatement;
};

}   // namespace RDB

#endif // RDBSQLSTATEMENTSBINDVISITOR_H
