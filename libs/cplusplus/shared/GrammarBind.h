#ifndef GRAMMARBIND_H
#define GRAMMARBIND_H

#include "ASTVisitor.h"
#include "Names.h"
#include "Symbols.h"

namespace CPlusPlus {

class GrammarBind: protected ASTVisitor
{
public:
    GrammarBind( TranslationUnit *unit );

    void operator()( GrammarAST *ast, GrammarScope *scope );

protected:
    // First Level
    bool visit( SimpleDeclarationAST * );
    bool visit( GrammarTokenDeclarationAST * );
    bool visit( GrammarRuleTypeDeclarationAST * );
    bool visit( GrammarPrecedenceDeclarationAST * );
    bool visit( GrammarRuleDefinitionAST * );

    // Second Level
    bool visit( GrammarRuleTermAtomAST * );
    bool visit( GrammarRuleCharacterAtomAST * );
    bool visit( GrammarRulePrecidentAtomAST * );
    bool visit( GrammarRuleCodeInjectionAtomAST * );

    // Names Level
    bool visit( GrammarSemanticTypeNameAST *ast );
    bool visit( SimpleNameAST *ast );

private:
    GrammarScope *_grammarScope;
    const Name *_typeName;
    GrammarSymbol::UsageType _usageType;
};

}   // namespace CPlusPlus

#endif // GRAMMARBIND_H
