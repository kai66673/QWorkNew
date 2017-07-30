#include "CppLibrariesManager.h"

namespace CppTools {

CppLibraryItem::CppLibraryItem(const QString &name,
                               const QString &rootFolder,
                               const QString &preprocConfiguration)
    : m_name(name)
    , m_rootFolder(rootFolder)
    , m_preprocConfiguration(preprocConfiguration)
{
    /// TODO: create SQLITE db
    m_status = Ready;
}

CppLibrariesManager::CppLibrariesManager()
{

}

}   // namespace CppTools
