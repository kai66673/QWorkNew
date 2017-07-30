#ifndef VIRTUALFUNCTIONHELPER_H
#define VIRTUALFUNCTIONHELPER_H

#include <QObject>

#include "symbolfinder.h"
#include "Symbols.h"
#include "TypeOfExpression.h"

#include "cpptools_global.h"

namespace CppTools {
class SymbolFinder;
}

namespace CPlusPlus {

class CPPTOOLS_EXPORT VirtualFunctionHelper {
public:
    VirtualFunctionHelper(const TypeOfExpression &typeOfExpression,
                          Scope *scope,
                          const Document::Ptr &document,
                          const Snapshot &snapshot,
                          CppTools::SymbolFinder *symbolFinder);

    bool canLookupVirtualFunctionOverrides(Function *function);

    /// Returns != 0 if canLookupVirtualFunctionOverrides() succeeded.
    Class *staticClassOfFunctionCallExpression() const
    { return m_staticClassOfFunctionCallExpression; }

private:
    VirtualFunctionHelper();
    Q_DISABLE_COPY(VirtualFunctionHelper)

    Class *staticClassOfFunctionCallExpression_internal() const;

private:
    // Provided
    const Document::Ptr m_expressionDocument;
    Scope *m_scope;
    const Document::Ptr &m_document;
    const Snapshot &m_snapshot;
    CppTools::SymbolFinder *m_finder;

    // Determined
    ExpressionAST *m_baseExpressionAST;
    Function *m_function;
    int m_accessTokenKind;
    Class *m_staticClassOfFunctionCallExpression; // Output
};

}   // namespace CPlusPlus

#endif // VIRTUALFUNCTIONHELPER_H
