#include <QMenu>
#include <QContextMenuEvent>
#include <QAction>
#include <QModelIndex>
#include <QFileDialog>
#include <QStatusBar>

#include "Core.h"
#include "IDocument.h"
#include "ProjectTreeView.h"
#include "ProjectData.h"
#include "NewFolderDialog.h"

ProjectTreeView::ProjectTreeView( QWidget *parent )
    : QTreeView(parent)
{
    QMainWindow *mainWindow = Core::Storage::mainWindow()->widget();

    folderMenu  = new QMenu(mainWindow);
    fileMenu    = new QMenu(mainWindow);

    actAddFolder            = new QAction(tr("Add Folder"), mainWindow);
    actEditFolder           = new QAction(tr("Folder Properties"), mainWindow);
    actDelFolder            = new QAction(tr("Remove Folder"), mainWindow);
    actAddFile              = new QAction(tr("Add Files"), mainWindow);
    actDelFile              = new QAction(tr("Remove File"), mainWindow);
    actOpenFile             = new QAction(tr("Open File"), mainWindow);

    actProjectProperties            = new QAction(tr("Properties"), mainWindow);
    actProjectConfigBuildActions    = new QAction(tr("Configure Build Actions"), mainWindow);
    actProjectSynhronize            = new QAction(tr("Synhronize"), mainWindow);

    connect(actAddFolder,           SIGNAL(triggered()), this, SLOT(onAddFolder()));
    connect(actEditFolder,          SIGNAL(triggered()), this, SLOT(onEditFolder()));
    connect(actDelFolder,           SIGNAL(triggered()), this, SLOT(onDelFolder()));
    connect(actAddFile,             SIGNAL(triggered()), this, SLOT(onAddFile()));
    connect(actDelFile,             SIGNAL(triggered()), this, SLOT(onDelFile()));
    connect(actOpenFile,            SIGNAL(triggered()), this, SLOT(onOpenFile()));

    connect(actProjectProperties,           SIGNAL(triggered()), this, SLOT(onEditProjectProperties()));
    connect(actProjectConfigBuildActions,   SIGNAL(triggered()), this, SLOT(onConfigureProjectBuildActions()));
    connect(actProjectSynhronize,           SIGNAL(triggered()), this, SLOT(onProjectSynhronize()));

    folderMenu->addAction(actAddFolder);
    folderMenu->addAction(actAddFile);
    folderMenu->addSeparator();
    folderMenu->addAction(actEditFolder);
    folderMenu->addSeparator();
    folderMenu->addAction(actDelFolder);

    fileMenu->addAction(actOpenFile);
    fileMenu->addSeparator();
    fileMenu->addAction(actDelFile);
}

ProjectTreeItem *ProjectTreeView::addDocument( ProjectTreeItem *parent, const QString &filePath )
{
    ProjectModel *projectModel = qobject_cast<ProjectModel *>(model());
    if ( !projectModel )
        return 0;

    if ( !projectModel->isValid() )
        return 0;

    return projectModel->addDocument(parent, filePath);
}

void ProjectTreeView::contextMenuEvent( QContextMenuEvent *event )
{
    ProjectModel *projectModel = qobject_cast<ProjectModel *>(model());
    if ( !projectModel )
        return;

    if ( !projectModel->isValid() )
        return;

    QModelIndex index = selectionModel()->currentIndex();
    if ( !index.isValid() )
        return;

    switch ( projectModel->nodeType(index) ) {
        case 0: {
            QMenu prjMenu;
            prjMenu.addActions(projectModel->projectBuildActions());
            if ( prjMenu.actions().size() > 0 )
                prjMenu.addSeparator();
            prjMenu.addAction(actProjectConfigBuildActions);
            prjMenu.addSeparator();
            if ( projectModel->hasImportFile() ) {
                prjMenu.addAction(actProjectSynhronize);
                prjMenu.addSeparator();
            }
            prjMenu.addAction(actAddFolder);
            prjMenu.addAction(actAddFile);
            prjMenu.addSeparator();
            prjMenu.addAction(actProjectProperties);
            prjMenu.exec(event->globalPos());
            break;
        }
        case 1:
            folderMenu->exec(event->globalPos());
            break;
        case 2:
            fileMenu->exec(event->globalPos());
            break;
    }
}

void ProjectTreeView::mouseDoubleClickEvent( QMouseEvent *event )
{
    ProjectModel *projectModel = qobject_cast<ProjectModel *>(model());
    if ( !projectModel )
        return;

    if ( !projectModel->isValid() )
        return;

    QModelIndex index = selectionModel()->currentIndex();
    if ( !index.isValid() )
        return;

    if ( projectModel->nodeType(index) == 2 )
        DocumentManager::getInstance()->openDocument(projectModel->data(index, Qt::ToolTipRole).toString());
    else
        QTreeView::mouseDoubleClickEvent(event);
}

void ProjectTreeView::keyPressEvent( QKeyEvent *event )
{
    if ( event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return ) {
        ProjectModel *projectModel = qobject_cast<ProjectModel *>(model());
        if ( !projectModel )
            return;
        if ( !projectModel->isValid() )
            return;
        QModelIndex index = selectionModel()->currentIndex();
        if ( !index.isValid() )
            return;
        if ( projectModel->nodeType(index) == 2 ) {
            DocumentManager::getInstance()->openDocument(projectModel->data(index, Qt::ToolTipRole).toString());
            return;
        }
    }

    QTreeView::keyPressEvent(event);
}

void ProjectTreeView::onAddFolder()
{
    ProjectModel *projectModel = qobject_cast<ProjectModel *>(model());
    if ( !projectModel )
        return;

    QModelIndex index = selectionModel()->currentIndex();
    if ( !index.isValid() )
        return;

    NewFolderDialog folderDialog(Core::Storage::mainWindow()->widget());
    folderDialog.setFolderType(FolderNode::NotAssigned);

    if ( folderDialog.exec() != QDialog::Accepted )
        return;

    QString folderName          = folderDialog.folderName();
    QString folderDescription   = folderDialog.folderDescription();
    QString defaultSourcePath   = folderDialog.defaultSourcePath();
    FolderNode::FolderType folderType = folderDialog.folderType();

    projectModel->addFolder(folderName, folderDescription, defaultSourcePath, index, folderType);
}

void ProjectTreeView::onEditFolder()
{
    ProjectModel *projectModel = qobject_cast<ProjectModel *>(model());
    if ( !projectModel )
        return;

    QModelIndex index = selectionModel()->currentIndex();
    if ( !index.isValid() )
        return;

    ProjectTreeItem *folderItem = static_cast<ProjectTreeItem *>(index.internalPointer());
    if ( !folderItem )
        return;

    NewFolderDialog folderDialog(Core::Storage::mainWindow()->widget());

    folderDialog.setFolderName(folderItem->name());
    folderDialog.setFolderDescription(folderItem->description());
    folderDialog.setDefaultSourcePath(folderItem->defaultSourcePath());

    bool isSrcFolder = ( projectModel->defaultSourceIdFolder() == folderItem->id() );
    bool isHdrFolder = ( projectModel->defaultHeaderIdFolder() == folderItem->id() );

    FolderNode::FolderType folderType = FolderNode::NotAssigned;
    if ( isSrcFolder )
        folderType = FolderNode::DefaultSource;
    else if ( isHdrFolder )
        folderType = FolderNode::DefaultHeader;

    folderDialog.setFolderType(folderType);

    if ( folderDialog.exec() != QDialog::Accepted )
        return;

    QString folderName          = folderDialog.folderName();
    QString folderDescription   = folderDialog.folderDescription();
    QString defaultSourcePath   = folderDialog.defaultSourcePath();
    FolderNode::FolderType folderTypeNew = folderDialog.folderType();

    if ( folderTypeNew != folderType ) {
        if ( isSrcFolder ) {
            projectModel->setDefaultSourceIdFolder(0);
            if ( folderTypeNew == FolderNode::DefaultHeader )
                projectModel->setDefaultHeaderIdFolder(folderItem->id());
        } else if ( isHdrFolder ) {
            projectModel->setDefaultHeaderIdFolder(0);
            if ( folderTypeNew == FolderNode::DefaultSource )
                projectModel->setDefaultSourceIdFolder(folderItem->id());
        } else {
            if ( folderTypeNew == FolderNode::DefaultSource )
                projectModel->setDefaultSourceIdFolder(folderItem->id());
            else if ( folderTypeNew == FolderNode::DefaultHeader )
                projectModel->setDefaultHeaderIdFolder(folderItem->id());
        }
    }

    projectModel->editFolder(folderName, folderDescription, defaultSourcePath, index);
}

void ProjectTreeView::onDelFolder()
{
    ProjectModel *projectModel = qobject_cast<ProjectModel *>(model());
    if ( !projectModel )
        return;

    QModelIndex index = selectionModel()->currentIndex();
    if ( !index.isValid() )
        return;

    projectModel->removeFolder(index);
}

void ProjectTreeView::onAddFile()
{
    ProjectModel *projectModel = qobject_cast<ProjectModel *>(model());
    if ( !projectModel )
        return;

    QModelIndex index = selectionModel()->currentIndex();
    if ( !index.isValid() )
        return;

    ProjectTreeItem *item = static_cast<ProjectTreeItem *>(index.internalPointer());
    if ( !item )
        return;

    QString dirName = item->defaultSourcePath();

    QStringList files = QFileDialog::getOpenFileNames( Core::Storage::mainWindow()->widget(),
                                                       tr("Add Files"), dirName );
    projectModel->addFiles(files, index);
}

void ProjectTreeView::onDelFile()
{
    ProjectModel *projectModel = qobject_cast<ProjectModel *>(model());
    if ( !projectModel )
        return;

    QModelIndex index = selectionModel()->currentIndex();
    if ( !index.isValid() )
        return;

    projectModel->removeFile(index);
}

void ProjectTreeView::onOpenFile()
{
    ProjectModel *projectModel = qobject_cast<ProjectModel *>(model());
    if ( !projectModel )
        return;

    QModelIndex index = selectionModel()->currentIndex();
    if ( !index.isValid() )
        return;

    DocumentManager::getInstance()->openDocument(projectModel->data(index, Qt::ToolTipRole).toString());
}

void ProjectTreeView::onEditProjectProperties()
{
    emit(editProjectProperties());
}

void ProjectTreeView::onConfigureProjectBuildActions()
{
    ProjectModel *projectModel = qobject_cast<ProjectModel *>(model());
    if ( !projectModel )
        return;

    projectModel->configureProjectBuildActions();
}

void ProjectTreeView::onProjectSynhronize()
{
    ProjectModel *projectModel = qobject_cast<ProjectModel *>(model());
    if ( !projectModel )
        return;

    projectModel->synhronizeProject();
}


void ProjectTreeView::onBuildActionStarted()
{
    actProjectProperties->setEnabled(false);
    actProjectConfigBuildActions->setEnabled(false);
}

void ProjectTreeView::onBuildActionFinished()
{
    actProjectProperties->setEnabled(true);
    actProjectConfigBuildActions->setEnabled(true);
}
