#include "GrammarBind.h"
#include "AST.h"
#include "Literals.h"

using namespace CPlusPlus;

GrammarBind::GrammarBind( TranslationUnit *unit )
    : ASTVisitor(unit)
    , _grammarScope(0)
    , _typeName(0)
    , _usageType(GrammarSymbol::UnknownType)
{
}

void GrammarBind::operator()( GrammarAST *ast, GrammarScope *scope )
{
    if ( !ast || !scope )
        return;

    _grammarScope = scope;
    accept(ast);
}

bool GrammarBind::visit( SimpleDeclarationAST * )
{
    return false;
}

bool GrammarBind::visit( GrammarTokenDeclarationAST * )
{
    _typeName = 0;
    _usageType = GrammarSymbol::TokenDeclarationType;
    return true;
}

bool GrammarBind::visit( GrammarRuleTypeDeclarationAST * )
{
    _typeName = 0;
    _usageType = GrammarSymbol::RuleTypeDeclarationType;
    return true;
}

bool GrammarBind::visit( GrammarPrecedenceDeclarationAST * )
{
    _typeName = 0;
    _usageType = GrammarSymbol::TokenPrecDeclarationType;
    return true;
}

bool GrammarBind::visit( GrammarRuleDefinitionAST * )
{
    _typeName = 0;
    _usageType = GrammarSymbol::RuleDefinitionType;
    return true;
}

bool GrammarBind::visit( GrammarRuleTermAtomAST * )
{
    _usageType = GrammarSymbol::RuleItemDefinitionType;
    return true;
}

bool GrammarBind::visit( GrammarRuleCharacterAtomAST * )
{
    return false;
}

bool GrammarBind::visit( GrammarRulePrecidentAtomAST * )
{
    _usageType = GrammarSymbol::RulePrecidentType;
    return true;
}

bool GrammarBind::visit( GrammarRuleCodeInjectionAtomAST * )
{
    return false;
}

bool GrammarBind::visit( GrammarSemanticTypeNameAST *ast )
{
    ast->name = identifier(ast->identifier_token);
    _typeName = ast->name;
    _grammarScope->semanticTypesScope()->addMember(control()->newEnum(ast->identifier_token, _typeName));
    return false;
}

bool GrammarBind::visit( SimpleNameAST *ast )
{
    ast->name = identifier(ast->identifier_token);
    GrammarSymbol *st = control()->newGrammarSymbol( ast->name,
                                                     ast->identifier_token,
                                                     _typeName,
                                                     _usageType );
    _grammarScope->addMember(st);
    return false;
}
