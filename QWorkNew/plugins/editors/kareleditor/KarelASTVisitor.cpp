#include "KarelASTVisitor.h"
#include "KarelAST.h"

namespace Karel {

ASTVisitor::ASTVisitor(TranslationUnit *unit)
    : _translationUnit(unit)
{ }

ASTVisitor::~ASTVisitor()
{ }

TranslationUnit *ASTVisitor::translationUnit() const
{ return _translationUnit; }

void ASTVisitor::setTranslationUnit(TranslationUnit *translationUnit)
{ _translationUnit = translationUnit; }

Control *ASTVisitor::control() const
{ return _translationUnit ? _translationUnit->control() : 0; }

unsigned ASTVisitor::tokenCount() const
{ return _translationUnit->tokenCount(); }

const Token &ASTVisitor::tokenAt(unsigned index) const
{ return _translationUnit->tokenAt(index); }

int ASTVisitor::tokenKind(unsigned index) const
{ return _translationUnit->tokenKind(index); }

const char *ASTVisitor::spell(unsigned index) const
{ return _translationUnit->spell(index); }

const Identifier *ASTVisitor::identifier(unsigned index) const
{ return _translationUnit->identifier(index); }

const Literal *ASTVisitor::literal(unsigned index) const
{ return _translationUnit->literal(index); }

const NumericLiteral *ASTVisitor::numericLiteral(unsigned index) const
{ return _translationUnit->numericLiteral(index); }

const StringLiteral *ASTVisitor::stringLiteral(unsigned index) const
{ return _translationUnit->stringLiteral(index); }

void ASTVisitor::getPosition(unsigned offset,
                             unsigned *line,
                             unsigned *column,
                             const StringLiteral **fileName) const
{ _translationUnit->getPosition(offset, line, column, fileName); }

void ASTVisitor::getTokenPosition(unsigned index,
                                  unsigned *line,
                                  unsigned *column,
                                  const StringLiteral **fileName) const
{ _translationUnit->getTokenPosition(index, line, column, fileName); }

void ASTVisitor::getTokenStartPosition(unsigned index, unsigned *line, unsigned *column) const
{ getPosition(tokenAt(index).begin(), line, column); }

void ASTVisitor::getTokenEndPosition(unsigned index, unsigned *line, unsigned *column) const
{ getPosition(tokenAt(index).end(), line, column); }

void ASTVisitor::accept(AST *ast)
{ AST::accept(ast, this); }

}   // namespace Karel
