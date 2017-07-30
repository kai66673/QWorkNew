#ifndef CPPHELPERITEM_H
#define CPPHELPERITEM_H

#include <QIcon>

#include "indexitem.h"

namespace CppTools {

class CppHelperItem
{
public:
    enum CppHelperType {
        Enum,
        Class,
        Function,
        Declaration,
        Macro
    };

    CppHelperItem( CppHelperType type,
                   const QIcon &icon,
                   const QString &text,
                   const QString &qualifiedName,
                   bool isSourceFile,
                   const QString &fileName,
                   const QString &functionReturnType,
                   int matchingPosition = 0 );

    CppHelperItem( const CppHelperItem &other );

    CppHelperType m_type;
    QIcon m_icon;
    QString m_text;
    QString m_qualifiedName;
    bool m_isSourceFile;
    QString m_fileName;
    QString m_functionReturnType;
    int m_matchingPosition;

    static CppHelperType fromIndexType( IndexItem::ItemType itemType );
};

}   // namespace CppTools

#endif // CPPHELPERITEM_H
