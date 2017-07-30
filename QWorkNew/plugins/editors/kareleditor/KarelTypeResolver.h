#ifndef TYPERESOLVER_H
#define TYPERESOLVER_H

#include <QTextBlock>
#include <QStack>

#include "KarelType.h"
#include "KarelKeywords.h"
#include "KarelSimpleLexer.h"
#include "Control.h"

namespace Karel {

class TypeResolver
{
public:
    enum ResolveStatus {
        Ready = 0,
        Finished,
        Failed
    };

    enum AccessKind {
        VariableIdentifier = 0,
        Brackets,
        Dot
    };

    struct AccessItem {
        AccessItem(AccessKind kind_ = Dot, const QString &id_ = QString())
            : kind(kind_)
            , id(id_)
        { }

        AccessKind kind;
        QString id;
    };

    TypeResolver(QTextDocument *document, Scope *scope)
        : m_document(document)
        , m_scope(scope)
        , m_bracketDepth(0)
    { }

    const Type *operator () (const QString &text, const QTextBlock &block);

private:
    ResolveStatus resolveText(const QString &text);
    const Type *resolveType();

    QTextDocument *m_document;
    Scope *m_scope;
    int m_bracketDepth;
    QStack<AccessItem> m_accessItems;
};

}   // namespace Karel

#endif // TYPERESOLVER_H
