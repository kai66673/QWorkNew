#ifndef ORACLESQLCHECKNAMESVISITOR_H
#define ORACLESQLCHECKNAMESVISITOR_H

#include "OracleAST.h"
#include "OracleASTVisitor.h"
#include "OracleASTfwd.h"
#include "SqlNameUse.h"

namespace Oracle {

class SqlCheckNamesVisitor: public ASTVisitor
{
public:
    SqlCheckNamesVisitor( Sql::TranslationUnit *unit, Sql::BaseWalkReflector *reflector )
        : ASTVisitor(unit)
        , m_reflector(reflector)
    {}

protected:
    virtual bool preVisit( AST *ast );
    virtual void postVisit( AST *ast );

    virtual bool visit( SchemaNameAST *ast )
    {
        addUse(ast->name_token, Sql::CheckName::Use::Schema);
        return true;
    }

    virtual bool visit( TableNameAST *ast )
    {
        addUse(ast->name_token, Sql::CheckName::Use::Table);
        return true;
    }

    virtual bool visit( TableAliasAST *ast )
    {
        addUse(ast->name_token, Sql::CheckName::Use::TableAlias);
        return true;
    }

    virtual bool visit( ColumnNameAST *ast )
    {
        addUse(ast->name_token, Sql::CheckName::Use::Field);
        return true;
    }

    virtual bool visit( IndexNameAST *ast )
    {
        addUse(ast->name_token, Sql::CheckName::Use::Index);
        return true;
    }

    virtual bool visit( TriggerNameAST *ast )
    {
        addUse(ast->name_token, Sql::CheckName::Use::Trigger);
        return true;
    }

    virtual bool visit( ConstraintNameAST *ast )
    {
        addUse(ast->name_token, Sql::CheckName::Use::Constraint);
        return true;
    }

//    virtual bool visit( CursorNameAST *ast )
//    {
//        addUse(ast->name_token, Sql::CheckName::Use::Cursor);
//        return true;
//    }

//    virtual bool visit( StorageMapNameAST *ast )
//    {
//        addUse(ast->name_token, Sql::CheckName::Use::StorageMap);
//        return true;
//    }

//    virtual bool visit( AreaNameAST *ast )
//    {
//        addUse(ast->name_token, Sql::CheckName::Use::Area);
//        return true;
//    }

    virtual bool visit( BindingNameAST *ast )
    {
        addUse(ast->name_token, Sql::CheckName::Use::Bind);
        return true;
    }

//    virtual bool visit( IdentifierNameAST *ast )
//    {
//        addUse(ast->name_token, Sql::CheckName::Use::Identifier);
//        return true;
//    }

    virtual bool visit( PlSqlNameAST *ast )
    {
        if ( ast->sphere == PlSqlNameAST::RecordField )
            addUse(ast->name_token, Sql::CheckName::Use::Field);
        else
            addUse(ast->name_token, Sql::CheckName::Use::Identifier);
        return true;
    }

    virtual bool visit( PlSqlTypeNameAST *ast )
    {
        addUse(ast->name_token, Sql::CheckName::Use::Table);    // TODO: Sql::CheckName::Use::Type
        return true;
    }

    virtual bool visit( LabelNameAST *ast )
    {
        addUse(ast->name_token, Sql::CheckName::Use::Label);
        return true;
    }

//    virtual bool visit( RoutineNameAST *ast )
//    {
//        addUse(ast->name_token, Sql::CheckName::Use::Function);
//        return true;
//    }

//    virtual bool visit( ModuleNameAST *ast )
//    {
//        addUse(ast->name_token, Sql::CheckName::Use::Module);
//        return true;
//    }

private:
    void addUse( unsigned tokenIndex, Sql::CheckName::Use::Kind kind );
    Sql::BaseWalkReflector *m_reflector;
};

} // namespace Oracle

#endif // ORACLESQLCHECKNAMESVISITOR_H
