#ifndef ASOFPROJECTPYTHONMODULE_H
#define ASOFPROJECTPYTHONMODULE_H

#include "IPythonModule.h"
#include "AsofProjectPlugin.h"

namespace Python {

class IProjectItem
{
public:
    IProjectItem( ProjectTreeItem *item );
    virtual ~IProjectItem();

    void invalidate();

protected:
    bool isValid() const;
    ProjectTreeItem *m_item;
};

class AsofProjectPythonModule;
class ProjectFile;
class ProjectFolder;

class ProjectFolder: public QObject, public IProjectItem
{
    Q_OBJECT
    friend class AsofProjectPythonModule;

public:
    ProjectFolder( AsofProjectPythonModule *module, ProjectTreeItem *item );
    virtual ~ProjectFolder();

public slots:
    QString folderName() const;
    QString defaultPath() const;
    int filesCount() const;
    int foldersCount() const;
    ProjectFile *fileAt( int index );
    ProjectFolder *folderAt( int index );
    ProjectFile *addDocument( const QString &filePath );

private:
    bool checkValid() const;
    ProjectTreeItem *folderTtem();
    AsofProjectPythonModule *m_module;
};

class ProjectFile: public QObject, public IProjectItem
{
    Q_OBJECT

public:
    ProjectFile( AsofProjectPythonModule *module, ProjectTreeItem *item );
    virtual ~ProjectFile();

public slots:
    QString fileName() const;
    QString filePath() const;

private:
    bool checkValid() const;
    AsofProjectPythonModule *m_module;
};

class AsofProjectPythonModule : public IPythonModule
{
    Q_OBJECT

public:
    AsofProjectPythonModule( AsofProjectPlugin *plugin );
    virtual ~AsofProjectPythonModule();

    virtual QList<PythonModuleFunction *> functions() const;
    virtual QString sourceCodeForAssist() const;

    void onProjectItemDeleted( ProjectTreeItem *item );
    ProjectFolder *findOrInsertFolder( ProjectTreeItem *item );
    ProjectFile *findOrInsertFile( ProjectTreeItem *item );
    ProjectFile *addDocument( ProjectFolder *folder, const QString &filePath );

public slots:
    bool isProjectOpened();
    QStringList projectFiles();
    ProjectFolder *rootFolder();
    ProjectFolder *defaultSourceFolder();
    ProjectFolder *defaultHeaderFolder();

private:
    QList<PythonModuleFunction *> m_functions;
    AsofProjectPlugin *m_plugin;

    QMap<ProjectTreeItem *, IProjectItem *> m_pythonItems;
    QList<IProjectItem *> m_pythonItemsClosed;
};

}   // namespace Python

#endif // ASOFPROJECTPYTHONMODULE_H
