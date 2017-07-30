#ifndef __DSPIMPORTERPLUGIN_H

#define __DSPIMPORTERPLUGIN_H

#include <QObject>

#include "IASofProjectImporter.h"
#include "ProjectData.h"

class DspImporter: public QObject, public IAsofProjectImporter
{
    Q_OBJECT
    Q_INTERFACES(IAsofProjectImporter)
#ifdef HAVE_QT5
    Q_PLUGIN_METADATA(IID "QWorkBase.ASProject.IAsofProjectImporterz/1.0")
#endif

public:
    DspImporter( QObject *parent = 0 );

    virtual QString description() const;
    virtual QString sourceFilesDescription() const;
    virtual QString sourceFilesExtension() const;
    virtual bool projectUpdateSupported() const { return true; }
    virtual bool getImportSourceFilePath( QString &sourceFilePath ) const;
    virtual bool executeImport( QString sourceFilePath,
                                QString &dbFilePath,
                                QStringList &includePaths,
                                QStringList &defines,
                                QString &precompiledHeader,
                                ProjectTreeItem *projectItem ) const;

private:
    bool getDspCfgName( const QString &filePath, QString &cfgName ) const;
    bool readDspCfgProperties( const QString &path, const QString &filePath,
                               const QString &cfgName, QStringList &includes,
                               QStringList &defines, QString &precompiledHeader ) const;
    bool readDspSourceStructure( const QString &path, const QString &dspFilePath, ProjectTreeItem *projectItem ) const;
};

#endif // __DSPIMPORTERPLUGIN_H
