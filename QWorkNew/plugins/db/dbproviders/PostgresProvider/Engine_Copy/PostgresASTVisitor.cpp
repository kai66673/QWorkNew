#include "PostgresASTVisitor.h"
#include "PostgresAST.h"

namespace Postgres {

ASTVisitor::ASTVisitor( Sql::TranslationUnit *translationUnit )
    : m_translationUnit(translationUnit)
{
}

const Sql::Token &ASTVisitor::tokenAt( unsigned index ) const
{ return m_translationUnit->tokenAt(index); }

void ASTVisitor::accept( AST *ast )
{ AST::accept(ast, this); }

void ASTVisitor::getPosition( unsigned offset,
                              unsigned *line,
                              unsigned *column,
                              const Sql::StringLiteral **fileName ) const
{ m_translationUnit->getPosition(offset, line, column, fileName); }

void ASTVisitor::getTokenPosition( unsigned index,
                                   unsigned *line,
                                   unsigned *column,
                                   const Sql::StringLiteral **fileName ) const
{ m_translationUnit->getTokenPosition(index, line, column, fileName); }

void ASTVisitor::getTokenStartPosition( unsigned index, unsigned *line, unsigned *column ) const
{ getPosition(tokenAt(index).begin(), line, column); }

void ASTVisitor::getTokenEndPosition( unsigned index, unsigned *line, unsigned *column ) const
{ getPosition(tokenAt(index).end(), line, column); }

}   // namespace Postgres
