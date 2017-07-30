#ifndef PROJECTMANAGERPYTHONMODULE_H
#define PROJECTMANAGERPYTHONMODULE_H

#include <QObject>

#include "IPythonModule.h"
#include "PythonQtObjectStorage.h"
#include "projectmanager.h"
#include "projectexplorer_export.h"

namespace Python {

class ProjectManagerPythonModule;

class Project: public Core::PythonQtObject
{
    Q_OBJECT

    friend class ProjectManagerPythonModule;

public:
    Project(Core::PythonQtStorageDestroyDelegate *delegate, ProjectExplorer::ProjectNode *projectNode );
    virtual ~Project();

public slots:
    QStringList files();
    QList<Project *> subprojects();

    void build();
    void rebuild();
    void clean();
    void run();
    void executeBuildSystemCommand();
    void close();

private:
    void fillFiles(ProjectExplorer::FolderNode *folderNode, QStringList &files);
    ProjectExplorer::ProjectNode *m_projectNode;
    Core::PythonQtObjectStorage<ProjectExplorer::ProjectNode, Project> m_pythonSubProjectsStorage;
};

class PROJECTEXPLORER_EXPORT ProjectManagerPythonModule : public IPythonModule
{
    Q_OBJECT

public:
    ProjectManagerPythonModule();
    virtual ~ProjectManagerPythonModule();

    virtual QList<PythonModuleFunction *> functions() const;
    virtual QString sourceCodeForAssist() const;

public slots:
    QList<Project *> projects();
    Project *currentProject();
    Project *openProject();
    Project *newProject();
    void buildAll();
    void rebuildAll();
    void cleanAll();
    void closeAll();

private:
    QList<PythonModuleFunction *> m_functions;
    Core::PythonQtObjectStorage<ProjectExplorer::ProjectNode, Project> m_pythonProjectsStorage;
};

} // namespace Python

#endif // PROJECTMANAGERPYTHONMODULE_H
