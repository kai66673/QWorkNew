#include "CppHelperItem.h"

namespace CppTools {

CppHelperItem::CppHelperItem(CppHelperType type,
                              const QIcon &icon,
                              const QString &text,
                              const QString &qualifiedName,
                              bool isSourceFile,
                              const QString &fileName,
                              const QString &functionReturnType , int matchingPosition)
    : m_type(type)
    , m_icon(icon)
    , m_text(text)
    , m_qualifiedName(qualifiedName)
    , m_isSourceFile(isSourceFile)
    , m_fileName(fileName)
    , m_functionReturnType(functionReturnType)
    , m_matchingPosition(matchingPosition)
{ }

CppHelperItem::CppHelperItem(const CppHelperItem &other)
    : m_type(other.m_type)
    , m_icon(other.m_icon)
    , m_text(other.m_text)
    , m_qualifiedName(other.m_qualifiedName)
    , m_isSourceFile(other.m_isSourceFile)
    , m_fileName(other.m_fileName)
    , m_functionReturnType(other.m_functionReturnType)
    , m_matchingPosition(other.m_matchingPosition)
{

}

CppHelperItem::CppHelperType CppHelperItem::fromIndexType( IndexItem::ItemType itemType )
{
    switch ( itemType ) {
        case IndexItem::Enum:         return Enum;
        case IndexItem::Class:        return Class;
        case IndexItem::Function:     return Function;
        case IndexItem::Declaration:  return Declaration;
    }

    return Macro;
}

}   // namespace CppTools
