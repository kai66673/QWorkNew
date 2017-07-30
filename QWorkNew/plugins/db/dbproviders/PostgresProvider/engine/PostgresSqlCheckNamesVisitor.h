#ifndef POSTGRES_ORACLESQLCHECKNAMESVISITOR_H
#define POSTGRES_ORACLESQLCHECKNAMESVISITOR_H

#include "PostgresAST.h"
#include "PostgresASTVisitor.h"
#include "PostgresASTfwd.h"
#include "SqlNameUse.h"

namespace Postgres {

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

    virtual bool visit( ColumnRefAST *ast );

    virtual bool visit( NameAST *ast ) {
        if ( ast ) {
            switch ( ast->kind ) {
                case NameAST::Schema:       addUse(ast->name_token, Sql::CheckName::Use::Schema);       break;
                case NameAST::Table:        addUse(ast->name_token, Sql::CheckName::Use::Table);        break;
                case NameAST::TableAlias:   addUse(ast->name_token, Sql::CheckName::Use::TableAlias);   break;
                case NameAST::Column:       addUse(ast->name_token, Sql::CheckName::Use::Field);        break;
                case NameAST::Index:        addUse(ast->name_token, Sql::CheckName::Use::Index);        break;
                case NameAST::Trigger:      addUse(ast->name_token, Sql::CheckName::Use::Trigger);      break;
                case NameAST::Constraint:   addUse(ast->name_token, Sql::CheckName::Use::Constraint);   break;
                case NameAST::Binding:      addUse(ast->name_token, Sql::CheckName::Use::Bind);         break;
                case NameAST::Label:        addUse(ast->name_token, Sql::CheckName::Use::Label);        break;
            }
        }
        return true;
    }

//    virtual bool visit( PlSqlNameAST *ast )
//    {
//        if ( ast->sphere == PlSqlNameAST::RecordField )
//            addUse(ast->name_token, Sql::CheckName::Use::Field);
//        else
//            addUse(ast->name_token, Sql::CheckName::Use::Identifier);
//        return true;
//    }

//    virtual bool visit( PlSqlTypeNameAST *ast )
//    {
//        addUse(ast->name_token, Sql::CheckName::Use::Table);    // TODO: Sql::CheckName::Use::Type
//        return true;
//    }

private:
    void addUse( unsigned tokenIndex, Sql::CheckName::Use::Kind kind );
    Sql::BaseWalkReflector *m_reflector;
};

} // namespace Postgres

#endif // POSTGRES_ORACLESQLCHECKNAMESVISITOR_H
