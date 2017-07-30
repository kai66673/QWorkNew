#ifndef SQLCHECKNAMESVISITOR_H
#define SQLCHECKNAMESVISITOR_H

#include "sqlengine_global.h"
#include "AST.h"
#include "ASTVisitor.h"
#include "ASTfwd.h"
#include "SqlNameUse.h"

namespace Sql {

class SQL_EXPORT SqlCheckNamesVisitor: public ASTVisitor
{
public:

    SqlCheckNamesVisitor( TranslationUnit *unit, BaseWalkReflector *reflector )
        : ASTVisitor(unit)
        , m_reflector(reflector)
    {}

protected:
    virtual bool preVisit( AST *ast );
    virtual void postVisit( AST *ast );

    virtual bool visit( SchemaNameAST *ast )
    {
        addUse(ast->name_token, CheckName::Use::Schema);
        return true;
    }

    virtual bool visit( TableNameAST *ast )
    {
        addUse(ast->name_token, CheckName::Use::Table);
        return true;
    }

    virtual bool visit( TableAliasAST *ast )
    {
        addUse(ast->name_token, CheckName::Use::TableAlias);
        return true;
    }

    virtual bool visit( ColumnNameAST *ast )
    {
        addUse(ast->name_token, CheckName::Use::Field);
        return true;
    }

    virtual bool visit( IndexNameAST *ast )
    {
        addUse(ast->name_token, CheckName::Use::Index);
        return true;
    }

    virtual bool visit( TriggerNameAST *ast )
    {
        addUse(ast->name_token, CheckName::Use::Trigger);
        return true;
    }

    virtual bool visit( ConstraintNameAST *ast )
    {
        addUse(ast->name_token, CheckName::Use::Constraint);
        return true;
    }

    virtual bool visit( ParamNameAST *ast )
    {
        addUse(ast->name_token, CheckName::Use::Bind);
        return true;
    }

    virtual bool visit( LabelNameAST *ast )
    {
        addUse(ast->name_token, CheckName::Use::Label);
        return true;
    }

    virtual bool visit( FunctionNameAST *ast )
    {
        addUse(ast->name_token, CheckName::Use::Function);
        return true;
    }

private:
    void addUse( unsigned tokenIndex, CheckName::Use::Kind kind );
    BaseWalkReflector *m_reflector;
};

} // namespace Sql

#endif // SQLCHECKNAMESVISITOR_H
