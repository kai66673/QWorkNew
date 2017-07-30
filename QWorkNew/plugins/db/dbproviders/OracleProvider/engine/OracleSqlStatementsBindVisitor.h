#ifndef ORACLESQLSTATEMENTSBINDVISITOR_H
#define ORACLESQLSTATEMENTSBINDVISITOR_H

#include "SqlStatement.h"
#include "OracleASTVisitor.h"
#include "OracleAST.h"
#include "sql_constants.h"
#include "TranslationUnit.h"
#include "ITextDocument.h"

class QItext;

namespace Oracle {

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
    virtual bool visit( BindingNameAST *ast );

private:
    QList<Sql::SqlStatement *> *m_statements;
    Sql::SqlStatement *m_currentStatement;
};

}   // namespace Oracle

#endif // ORACLESQLSTATEMENTSBINDVISITOR_H
