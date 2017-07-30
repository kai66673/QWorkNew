#include "Core.h"
#include "AsofProjectPythonModule.h"
#include "ProjectTreeView.h"

namespace Python {

IProjectItem::IProjectItem( ProjectTreeItem *item )
    : m_item(item)
{

}

IProjectItem::~IProjectItem()
{ }

void IProjectItem::invalidate()
{
    m_item = 0;
}

bool IProjectItem::isValid() const
{
    return ( m_item != 0 );
}

ProjectFolder::ProjectFolder( AsofProjectPythonModule *module, ProjectTreeItem *item )
    : QObject()
    , IProjectItem(item)
    , m_module(module)
{
}

ProjectFolder::~ProjectFolder()
{ }

QString ProjectFolder::folderName() const
{
    if ( !checkValid() )
        return "";
    return m_item->name();
}

QString ProjectFolder::defaultPath() const
{
    if ( !checkValid() )
        return "";
    return m_item->defaultSourcePath();
}

int ProjectFolder::filesCount() const
{
    if ( !checkValid() )
        return 0;
    return m_item->childFilesCount();
}

int ProjectFolder::foldersCount() const
{
    if ( !checkValid() )
        return 0;
    return m_item->childFoldersCount();
}

ProjectFile *ProjectFolder::fileAt( int index )
{
    if ( !checkValid() )
        return 0;

    if ( index < 0 || index >= m_item->childFilesCount() ) {
        Core::Storage::warning(tr("File Index Out of Range"));
        return 0;
    }

    return m_module->findOrInsertFile(m_item->child(m_item->childFoldersCount() + index));
}

ProjectFolder *ProjectFolder::folderAt( int index )
{
    if ( !checkValid() )
        return 0;

    if ( index < 0 || index >= m_item->childFoldersCount() ) {
        Core::Storage::warning(tr("Folder Index Out of Range"));
        return 0;
    }

    return m_module->findOrInsertFolder(m_item->child(index));
}

ProjectFile *ProjectFolder::addDocument( const QString &filePath )
{
    if ( !checkValid() )
        return 0;

    QFile addFile(filePath);
    if ( !addFile.exists() ) {
        Core::Storage::warning(tr("File \"%1\" Not Exists!").arg(filePath));
        return 0;
    }

    return m_module->addDocument(this, filePath);
}

bool ProjectFolder::checkValid() const
{
    if ( !isValid() ) {
        Core::Storage::warning(tr("Project Folder No Longer Valid"));
        return false;
    }
    return true;
}

ProjectTreeItem *ProjectFolder::folderTtem()
{
    return m_item;
}

ProjectFile::ProjectFile(AsofProjectPythonModule *module, ProjectTreeItem *item)
    : QObject()
    , IProjectItem(item)
    , m_module(module)
{
}

ProjectFile::~ProjectFile()
{ }

QString ProjectFile::fileName() const
{
    if ( !checkValid() )
        return "";
    return m_item->name();
}

QString ProjectFile::filePath() const
{
    if ( !checkValid() )
        return "";
    return m_item->description();
}

bool ProjectFile::checkValid() const
{
    if ( !isValid() ) {
        Core::Storage::warning(tr("Project File No Longer Valid"));
        return false;
    }
    return true;
}

AsofProjectPythonModule::AsofProjectPythonModule( AsofProjectPlugin *plugin )
    : IPythonModule("QwProjectManager", "projectManager", "Project Manager Public Methods")
    , m_plugin(plugin)
{
    m_functions << new PythonModuleFunction("isProjectOpened", "Returns True If Project Opened")
                << new PythonModuleFunction("projectFiles", "Returns All Project Files")
                << new PythonModuleFunction("rootFolder", "Returns Project Root Folder")
                << new PythonModuleFunction("defaultSourceFolder", "Returns Project Default Sources Folder")
                << new PythonModuleFunction("defaultHeaderFolder", "Returns Project Default Headers Folder");
}

AsofProjectPythonModule::~AsofProjectPythonModule()
{
    qDeleteAll(m_functions);
    qDeleteAll(m_pythonItems.values());
    qDeleteAll(m_pythonItemsClosed);
}

QList<PythonModuleFunction *> AsofProjectPythonModule::functions() const
{
    return m_functions;
}

QString AsofProjectPythonModule::sourceCodeForAssist() const
{
    return  "class ProjectFolder():\n"
            "    def folderName(self):\n"
            "        return ''\n"
            "    def defaultPath(self):\n"
            "        return ''\n"
            "    def filesCount(self):\n"
            "        return 1\n"
            "    def foldersCount(self):\n"
            "        return 1\n"
            "    def fileAt(self, index):\n"
            "        return ProjectFile()\n"
            "    def folderAt(self, index):\n"
            "        return ProjectFolder()\n"
            "    def addDocument(self, filePath):\n"
            "        return ProjectFile()\n"
            "class ProjectFile():\n"
            "    def fileName(self):\n"
            "        return ''\n"
            "    def filePath(self):\n"
            "        return ''\n"
            "class QwProjectManager():\n"
            "    def isProjectOpened(self):\n"
            "        return True\n"
            "    def projectFiles(self):\n"
            "        return [ProjectFile(), ProjectFile()]\n"
            "    def rootFolder(self):\n"
            "        return ProjectFolder()\n"
            "    def defaultSourceFolder(self):\n"
            "        return ProjectFolder()\n"
            "    def defaultHeaderFolder(self):\n"
            "        return ProjectFolder()\n";
}

void AsofProjectPythonModule::onProjectItemDeleted( ProjectTreeItem *item )
{
    if ( m_pythonItems.find(item) == m_pythonItems.end() )
        return;

    IProjectItem *pyItem = m_pythonItems[item];
    pyItem->invalidate();
    m_pythonItems.remove(item);
    m_pythonItemsClosed << pyItem;
}

bool AsofProjectPythonModule::isProjectOpened()
{
    return m_plugin->isProjectOpened();
}

QStringList AsofProjectPythonModule::projectFiles()
{
    if ( !m_plugin->isProjectOpened() ) {
        Core::Storage::mainWindow()->pythonReport(tr("No Project Opened!"));
        return QStringList();
    }

    return m_plugin->projectFiles();
}

ProjectFolder *AsofProjectPythonModule::rootFolder()
{
    if ( !m_plugin->isProjectOpened() ) {
        Core::Storage::mainWindow()->pythonReport(tr("No Project Opened!"));
        return 0;
    }

    ProjectTreeItem *rootItem = m_plugin->projectFolderItem();
    if ( !rootItem )
        return 0;
    return findOrInsertFolder(rootItem);
}

ProjectFolder *AsofProjectPythonModule::defaultSourceFolder()
{
    if ( !m_plugin->isProjectOpened() ) {
        Core::Storage::mainWindow()->pythonReport(tr("No Project Opened!"));
        return 0;
    }

    ProjectTreeItem *srcItem = m_plugin->defaultSourceFolderItem();
    if ( !srcItem )
        return 0;
    return findOrInsertFolder(srcItem);
}

ProjectFolder *AsofProjectPythonModule::defaultHeaderFolder()
{
    if ( !m_plugin->isProjectOpened() ) {
        Core::Storage::mainWindow()->pythonReport(tr("No Project Opened!"));
        return 0;
    }

    ProjectTreeItem *hdrItem = m_plugin->defaultHeaderFolderItem();
    if ( !hdrItem )
        return 0;
    return findOrInsertFolder(hdrItem);
}

ProjectFolder *AsofProjectPythonModule::findOrInsertFolder( ProjectTreeItem *item )
{
    if ( m_pythonItems.find(item) != m_pythonItems.end() )
        return static_cast<ProjectFolder *>(m_pythonItems[item]);

    ProjectFolder *pyFolder = new ProjectFolder(this, item);
    m_pythonItems[item] = pyFolder;
    return pyFolder;
}

ProjectFile *AsofProjectPythonModule::findOrInsertFile( ProjectTreeItem *item )
{
    if ( m_pythonItems.find(item) != m_pythonItems.end() )
        return static_cast<ProjectFile *>(m_pythonItems[item]);

    ProjectFile *pyFile = new ProjectFile(this, item);
    m_pythonItems[item] = pyFile;
    return pyFile;
}

ProjectFile *AsofProjectPythonModule::addDocument( ProjectFolder *folder, const QString &filePath )
{
    if ( folder && folder->checkValid() ) {
        if ( ProjectTreeItem *folderItem = folder->folderTtem() )
            if ( ProjectTreeItem *addedItem = m_plugin->projectTreeView->addDocument(folderItem, filePath) )
                return findOrInsertFile(addedItem);
    }

    return 0;
}

}   // namespace Python
