#ifndef CPPLIBRARIESMANAGER_H
#define CPPLIBRARIESMANAGER_H

#include <QPair>
#include <QString>

#include "CloseApplicationHandler.h"
#include "cpptools_global.h"

namespace CppTools {

class CPPTOOLS_EXPORT CppLibraryItem
{
public:
    enum Status {
        Ready,
        Canceled,
        Proceded
    };

    typedef QPair<Status, QString> LibraryFile;

    CppLibraryItem(const QString &name,
                   const QString &rootFolder,
                   const QString &preprocConfiguration);

private:
    Status m_status;
    QList<LibraryFile> m_files;
    QString m_name;
    QString m_rootFolder;
    QString m_preprocConfiguration;
};

class CPPTOOLS_EXPORT CppLibrariesManager : public ICloseApplicationClient
{
public:
    CppLibrariesManager();
};

}   // namespace CppTools

#endif // CPPLIBRARIESMANAGER_H
