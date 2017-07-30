#include "VirtualFunctionHelper.h"
#include "qtcassert.h"
#include "TranslationUnit.h"
#include "AST.h"
#include "functionutils.h"
#include "ResolveExpression.h"

namespace CPlusPlus {

VirtualFunctionHelper::VirtualFunctionHelper(const TypeOfExpression &typeOfExpression,
                                             Scope *scope,
                                             const Document::Ptr &document,
                                             const Snapshot &snapshot,
                                             CppTools::SymbolFinder *finder)
    : m_expressionDocument(typeOfExpression.context().expressionDocument())
    , m_scope(scope)
    , m_document(document)
    , m_snapshot(snapshot)
    , m_finder(finder)
    , m_baseExpressionAST(0)
    , m_function(0)
    , m_accessTokenKind(0)
    , m_staticClassOfFunctionCallExpression(0)
{
    if (ExpressionAST *expressionAST = typeOfExpression.expressionAST()) {
        if (CallAST *callAST = expressionAST->asCall()) {
            if (ExpressionAST *baseExpressionAST = callAST->base_expression)
                m_baseExpressionAST = baseExpressionAST;
        }
    }
}

bool VirtualFunctionHelper::canLookupVirtualFunctionOverrides(Function *function)
{
    m_function = function;
    if (!m_function || !m_baseExpressionAST || !m_expressionDocument || !m_document || !m_scope
            || m_scope->isClass() || m_scope->isFunction() || m_snapshot.isEmpty()) {
        return false;
    }

    bool result = false;

    if (IdExpressionAST *idExpressionAST = m_baseExpressionAST->asIdExpression()) {
        NameAST *name = idExpressionAST->name;
        const bool nameIsQualified = name && name->asQualifiedName();
        result = !nameIsQualified && CppTools::FunctionUtils::isVirtualFunction(
                    function, LookupContext(m_document, m_snapshot));
    } else if (MemberAccessAST *memberAccessAST = m_baseExpressionAST->asMemberAccess()) {
        NameAST *name = memberAccessAST->member_name;
        const bool nameIsQualified = name && name->asQualifiedName();
        if (!nameIsQualified && CppTools::FunctionUtils::isVirtualFunction(
                    function, LookupContext(m_document, m_snapshot))) {
            TranslationUnit *unit = m_expressionDocument->translationUnit();
            QTC_ASSERT(unit, return false);
            m_accessTokenKind = unit->tokenKind(memberAccessAST->access_token);

            if (m_accessTokenKind == T_ARROW) {
                result = true;
            } else if (m_accessTokenKind == T_DOT) {
                TypeOfExpression typeOfExpression;
                typeOfExpression.init(m_document, m_snapshot);
//                typeOfExpression.setExpandTemplates(true);
                const QList<LookupItem> items = typeOfExpression.reference(
                            memberAccessAST->base_expression, m_document, m_scope);
                if (!items.isEmpty()) {
                    const LookupItem item = items.first();
                    if (Symbol *declaration = item.declaration())
                        result = declaration->type()->isReferenceType();
                }
            }
        }
    }

    if (!result)
        return false;
    return (m_staticClassOfFunctionCallExpression = staticClassOfFunctionCallExpression_internal());
}

/// For "f()" in "class C { void g() { f(); };" return class C.
/// For "c->f()" in "{ C *c; c->f(); }" return class C.
Class *VirtualFunctionHelper::staticClassOfFunctionCallExpression_internal() const
{
    if (!m_finder)
        return 0;

    Class *result = 0;

    if (m_baseExpressionAST->asIdExpression()) {
        for (Scope *s = m_scope; s ; s = s->enclosingScope()) {
            if (Function *function = s->asFunction()) {
                result = m_finder->findMatchingClassDeclaration(function, m_snapshot);
                break;
            }
        }
    } else if (MemberAccessAST *memberAccessAST = m_baseExpressionAST->asMemberAccess()) {
        QTC_ASSERT(m_accessTokenKind == T_ARROW || m_accessTokenKind == T_DOT, return result);
        TypeOfExpression typeOfExpression;
        typeOfExpression.init(m_document, m_snapshot);
//        typeOfExpression.setExpandTemplates(true);
        const QList<LookupItem> items = typeOfExpression(memberAccessAST->base_expression,
                                                         m_expressionDocument, m_scope);
        ResolveExpression resolveExpression(typeOfExpression.context());
        ClassOrNamespace *binding = resolveExpression.baseExpression(items, m_accessTokenKind);
        if (binding) {
            /*if (Class *klass = binding->rootClass()) {
                result = klass;
            } else*/ {
                const QList<Symbol *> symbols = binding->symbols();
                if (!symbols.isEmpty()) {
                    Symbol * const first = symbols.first();
                    if (first->isForwardClassDeclaration())
                        result = m_finder->findMatchingClassDeclaration(first, m_snapshot);
                }
            }
        }
    }

    return result;
}

}   // namespace CppTools
