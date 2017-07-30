#ifndef PROPOSENAMERESOLVER_H
#define PROPOSENAMERESOLVER_H

#include <QStringList>

#include "ASTfwd.h"
#include "cpptools_global.h"
#include "CppDocument.h"

namespace CPlusPlus {

class CPPTOOLS_EXPORT ProposeNameResolver
{
public:
    ProposeNameResolver(const QString &typeText);
    virtual ~ProposeNameResolver();

    QStringList operator ()(bool inClass, bool *isTypedef);
    CPlusPlus::Document::Ptr document();

private:
    QString m_typeText;
    CPlusPlus::Document::Ptr m_document;
};

}   // namespace CPlusPlus

#endif // PROPOSENAMERESOLVER_H
