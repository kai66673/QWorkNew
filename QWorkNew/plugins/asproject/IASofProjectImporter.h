#ifndef __IASOF_PROJECT_IMPORTER

#define __IASOF_PROJECT_IMPORTER

#include <QStringList>
#include <QtPlugin>

#include "asproject_global.h"
#include "ProjectData.h"

class ASPROJECT_EXPORT IAsofProjectImporter
{
public:
    virtual QString description() const = 0;
    virtual QString sourceFilesDescription() const = 0;
    virtual QString sourceFilesExtension() const = 0;
    virtual bool projectUpdateSupported() const = 0;
    virtual bool getImportSourceFilePath( QString &sourceFilePath ) const = 0;
    virtual bool executeImport( QString sourceFilePath,
                                QString &dbFilePath,
                                QStringList &includePaths,
                                QStringList &defines,
                                QString &precompiledHeader,
                                ProjectTreeItem *projectItem ) const = 0;
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IAsofProjectImporter, "QWorkBase.ASProject.IAsofProjectImporterz/1.0")
QT_END_NAMESPACE

#endif // __IASOF_PROJECT_IMPORTER
