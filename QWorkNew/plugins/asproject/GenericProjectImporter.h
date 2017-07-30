#ifndef GENERICPROJECTIMPORTER_H
#define GENERICPROJECTIMPORTER_H

#include "IASofProjectImporter.h"
#include "ProjectData.h"

class GenericProjectImporter: public IAsofProjectImporter
{
public:
    virtual QString description() const;
    virtual QString sourceFilesDescription() const;
    virtual QString sourceFilesExtension() const;
    virtual bool projectUpdateSupported() const { return false; }
    virtual bool getImportSourceFilePath( QString &sourceFilePath ) const;
    virtual bool executeImport( QString sourceFilePath,
                                QString &dbFilePath,
                                QStringList &includePaths,
                                QStringList &defines,
                                QString &precompiledHeader,
                                ProjectTreeItem *projectItem ) const;

private:
    void readDirSources( const QString &path, ProjectTreeItem *parentItem, int &idfile , int &idfolder ) const;
};

#endif // GENERICPROJECTIMPORTER_H
