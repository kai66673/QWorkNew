#include "Core.h"
#include "ProjectManagerPythonModule.h"

namespace Python {

Project::Project(Core::PythonQtStorageDestroyDelegate *delegate, ProjectExplorer::ProjectNode *projectNode)
    : Core::PythonQtObject(delegate, projectNode)
    , m_projectNode(projectNode)
{ }

Project::~Project()
{ }

QStringList Project::files()
{
    QStringList result;
    if ( isValid() )
        fillFiles(m_projectNode, result);

    return result;
}

QList<Project *> Project::subprojects()
{
    QList<Project *> result;
    if ( isValid() )
        foreach ( ProjectExplorer::ProjectNode *projectNode, m_projectNode->subProjectNodes() )
            result << m_pythonSubProjectsStorage.findOrCreatePythonQtObject(projectNode);

    return result;
}

void Project::build()
{
    if ( isValid() )
        ProjectExplorer::ProjectManager::instance()->buildProjectNode(m_projectNode);
}

void Project::rebuild()
{
    if ( isValid() )
        ProjectExplorer::ProjectManager::instance()->rebuildProjectNode(m_projectNode);
}

void Project::clean()
{
    if ( isValid() )
        ProjectExplorer::ProjectManager::instance()->cleanProjectNode(m_projectNode);
}

void Project::run()
{
    if ( isValid() )
        ProjectExplorer::ProjectManager::instance()->runProjectNode(m_projectNode);
}

void Project::executeBuildSystemCommand()
{
    if ( isValid() )
        ProjectExplorer::ProjectManager::instance()->executeBuildSystemCommandForNode(m_projectNode);
}

void Project::close()
{
    if ( isValid() )
        ProjectExplorer::ProjectManager::instance()->closeProjectNode(m_projectNode);
}

void Project::fillFiles(ProjectExplorer::FolderNode *folderNode, QStringList &files)
{
    foreach ( ProjectExplorer::FileNode *fileNode, folderNode->fileNodes() )
        files << fileNode->path();
    foreach ( ProjectExplorer::FolderNode *subFolder, folderNode->subFolderNodes() )
        fillFiles(subFolder, files);
}

ProjectManagerPythonModule::ProjectManagerPythonModule()
    : IPythonModule("QwProjectManager", "projectManager", "Project Manager public methods")
{
    m_functions << new PythonModuleFunction("projects", tr("Returns list of Opened Projects"))
                << new PythonModuleFunction("currentProject", tr("Returns Active (Current) Project"))
                << new PythonModuleFunction("openProject", tr("Open Existing Project"))
                << new PythonModuleFunction("newProject", tr("Create New Project"))
                << new PythonModuleFunction("buildAll", tr("Build All Opened Projects"))
                << new PythonModuleFunction("rebuildAll", tr("Rebuild All Opened Projects"))
                << new PythonModuleFunction("cleanAll", tr("Clean All Opened Projects"))
                << new PythonModuleFunction("closeAll", tr("Close All Opened Projects"));
}

ProjectManagerPythonModule::~ProjectManagerPythonModule()
{
    qDeleteAll(m_functions);
}

QList<PythonModuleFunction *> ProjectManagerPythonModule::functions() const
{
    return m_functions;
}

QString ProjectManagerPythonModule::sourceCodeForAssist() const
{
    return QString("class Project():\n"
                   "    def files(self):\n"
                   "        return ['a', 'b', 'c']\n"
                   "    def subprojects(self):\n"
                   "        return [Project(), Project(), Project()]\n"
                   "class QwProjectManager():\n"
                   "    def projects(self):\n"
                   "        return [Document(), Document()]\n"
                   "    def currentProject(self):\n"
                   "        return Document()\n"
                   "    def openProject(self):\n"
                   "        return Document()\n"
                   "    def newProject(self):\n"
                   "        return Document()\n"
                   "    def buildAll(self):\n"
                   "        return None\n"
                   "    def rebuildAll(self):\n"
                   "        return None\n"
                   "    def cleanAll(self):\n"
                   "        return None\n"
                   "    def closeAll(self):\n"
                   "        return None\n");
}

QList<Project *> ProjectManagerPythonModule::projects()
{
    QList<Project *> result;
    foreach ( ProjectExplorer::Project *project, ProjectExplorer::ProjectManager::projects() )
        result << m_pythonProjectsStorage.findOrCreatePythonQtObject(project->rootProjectNode());

    return result;
}

Project *ProjectManagerPythonModule::currentProject()
{
    if ( ProjectExplorer::Project *project =  ProjectExplorer::ProjectManager::currentProject() )
        return m_pythonProjectsStorage.findOrCreatePythonQtObject(project->rootProjectNode());

    Core::Storage::mainWindow()->pythonReport(tr("No Projects Opened"));
    return 0;
}

Project *ProjectManagerPythonModule::openProject()
{
    ProjectExplorer::ProjectManager *projectManager = ProjectExplorer::ProjectManager::instance();
    ProjectExplorer::Project *currentProjectBefore = projectManager->currentProject();
    projectManager->openProject();
    ProjectExplorer::Project *currentProjectAfter = projectManager->currentProject();
    if ( currentProjectAfter && currentProjectAfter != currentProjectBefore )
        return m_pythonProjectsStorage.findOrCreatePythonQtObject(currentProjectAfter->rootProjectNode());
    return 0;
}

Project *ProjectManagerPythonModule::newProject()
{
    ProjectExplorer::ProjectManager *projectManager = ProjectExplorer::ProjectManager::instance();
    ProjectExplorer::Project *currentProjectBefore = projectManager->currentProject();
    projectManager->newProject();
    ProjectExplorer::Project *currentProjectAfter = projectManager->currentProject();
    if ( currentProjectAfter && currentProjectAfter != currentProjectBefore )
        return m_pythonProjectsStorage.findOrCreatePythonQtObject(currentProjectAfter->rootProjectNode());
    return 0;
}

void ProjectManagerPythonModule::buildAll()
{
    ProjectExplorer::ProjectManager::instance()->buildSession();
}

void ProjectManagerPythonModule::rebuildAll()
{
    ProjectExplorer::ProjectManager::instance()->rebuildSession();
}

void ProjectManagerPythonModule::cleanAll()
{
    ProjectExplorer::ProjectManager::instance()->cleanSession();
}

void ProjectManagerPythonModule::closeAll()
{
    ProjectExplorer::ProjectManager *projectManager = ProjectExplorer::ProjectManager::instance();
    projectManager->closeProjects(projectManager->projects());
}

} // namespace Python
