#include <QIcon>
#include <QFile>
#include <QSqlQuery>
#include <QSqlError>
#include <QFileInfo>
#include <QDate>
#include <QTime>
#include <QMessageBox>
#include <QStatusBar>

#include <QDebug>

#include "Core.h"
#include "AsofProjectPlugin.h"
#include "ProjectData.h"
#include "ConfigureProjectBuildActionsDialog.h"
#include "AsofProjectPythonModule.h"

extern Python::AsofProjectPythonModule *projectPythonModule;

ProjectTreeItem::~ProjectTreeItem()
{
    projectPythonModule->onProjectItemDeleted(this);

    delete itemData;
    qDeleteAll(childFolders);
    qDeleteAll(childFiles);
}

ProjectTreeItem *ProjectTreeItem::child( int row )
{
    if ( row < 0 )
        return 0;

    if ( row < childFolders.size() )
        return childFolders[row];

    if ( row < childFolders.size() + childFiles.size() )
        return childFiles[row - childFolders.size()];

    return 0;
}

QVariant ProjectTreeItem::data(int column) const
{
    switch ( column ) {
    case 0: return itemData->name();
    case 1: return itemData->description();
    case 2: return itemData->icon();
    }

    return QVariant();
}

int ProjectTreeItem::row() const
{
    if ( !parentItem )
        return 0;

    int index = parentItem->childFolders.indexOf(const_cast<ProjectTreeItem *>(this));
    if ( index == -1 )
        index = parentItem->childFiles.indexOf(const_cast<ProjectTreeItem *>(this));

    return index;
}

ProjectModel::ProjectModel( AsofProjectPlugin *projectPlugin, QObject *parent )
    : QAbstractItemModel(parent)
    , m_rootItem(0)
    , valid(false)
    , errorText("")
    , m_filePath("")
    , m_importFilePath("")
    , m_srcIdFolder(0)
    , m_hdrIdFolder(0)
    , m_projectPlugin(projectPlugin)
    , m_projectBuildActionsModel(new ProjectBuildActionsModel(projectPlugin->buildActionsManager()))
{
}

void ProjectModel::initEmpty()
{
    if ( m_rootItem )
        delete m_rootItem;

    m_rootItem = new ProjectTreeItem(new FolderNode(0, "", "", 0, ""));
}

ProjectModel::~ProjectModel()
{
    if ( m_rootItem ) delete m_rootItem;
    delete m_projectBuildActionsModel;
}

QVariant ProjectModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    ProjectTreeItem *item = static_cast<ProjectTreeItem *>(index.internalPointer());

    switch ( role ) {
    case Qt::DisplayRole: return item->data(0);
    case Qt::ToolTipRole: return item->data(1);
    case Qt::DecorationRole: return QIcon(item->data(2).toString());
    }

    return QVariant();
}

Qt::ItemFlags ProjectModel::flags( const QModelIndex &index ) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant ProjectModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
        return m_rootItem->data(section);

    return QVariant();
}

QModelIndex ProjectModel::index( int row, int column, const QModelIndex &parent ) const
{
    if ( !hasIndex(row, column, parent) )
        return QModelIndex();

    ProjectTreeItem *parentItem;

    if ( !parent.isValid() )
        parentItem = m_rootItem;
    else
        parentItem = static_cast<ProjectTreeItem *>(parent.internalPointer());

    ProjectTreeItem *childItem = parentItem->child(row);
    if ( childItem )
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex ProjectModel::parent( const QModelIndex &index ) const
{
    if ( !index.isValid() )
        return QModelIndex();

    ProjectTreeItem *childItem = static_cast<ProjectTreeItem *>(index.internalPointer());
    ProjectTreeItem *parentItem = childItem->parent();

    if ( parentItem == m_rootItem )
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int ProjectModel::rowCount( const QModelIndex &parent ) const
{
    if (parent.column() > 0)
        return 0;

    ProjectTreeItem *parentItem;
    if ( !parent.isValid() )
        parentItem = m_rootItem;
    else
        parentItem = static_cast<ProjectTreeItem *>(parent.internalPointer());

    return parentItem->childCount();
}

int ProjectModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

bool ProjectModel::createProjectDb( const QString &projectName, const QString &projectDescription, const QString &filePath, const QString &importFilePath,
                                     const QStringList &includePaths, const QStringList &systemIncludePaths, const QStringList &defines,
                                     const QString &precompiledHeader, QStringList &sourceFiles )
{
    if ( valid ) {
        errorText = tr("Model Cannot be Reused");
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database("ASPDB");
    if ( db.isOpen() ) {
        db.close();
    }

    QFile file(filePath);
    if ( file.exists() ) {
        if ( !file.remove() ) {
            errorText = tr("File \"%1\" Cannot be recreated").arg(filePath);
            return false;
        }
    }

    db.setDatabaseName(filePath);
    if ( !db.open() ) {
        errorText = tr("Unable to establish a database connection");
        return false;
    }

    /// —оздание новой базы проекта
    bool queryResult;
    QString queryString;
    QSqlQuery query(db);

    const int createCommandCount = 8;
    const QString createCommands[createCommandCount][2] = {
        {"create table project (name string, description string, prj_version integer, imp_file_path string, imp_add_info string, src_idfolder integer, hdr_idfolder integer)", "project"},
        {"create table folder (idfolder integer primary key, name string, description string, idparentfolder integer, defsrcpath string)", "folder"},
        {"create table file (idfile integer primary key, name string, path string, idparentfolder integer)", "file"},
        {"create table includes (idinclude integer primary key, issystem integer, path text)", "includes"},
        {"create table defines (iddefine integer primary key, definename text)", "defines"},
        {"create table precompiledheader (header string)"},
        {"create table buildaction (idbuildaction integer primary key, action_name string, exe_file_path string, work_dir string, prior_action integer, exe_args string)", "buildaction"},
        {"create table buildenv (envkey string, envvalue string)", "buildenv"}
    };

    int i;
    for (i = 0; i < createCommandCount; i++) {
        queryResult = query.exec(createCommands[i][0]);
        if ( !queryResult ) {
            errorText = tr("Unable to create table \'%1\': %2").arg(createCommands[i][1]).arg(query.lastError().text());
            return false;
        }
    }

    queryString = "insert into project values (\'" + projectName + "\', \'" + projectDescription + "\', 3, \'" + importFilePath + "\', \'\', 0, 0)";
    queryResult = query.exec(queryString);
    if ( !queryResult ) {
        errorText = tr("Unable to insert into table \'project\': %1").arg(query.lastError().text());
        return false;
    }

    int idinclude = 1;
    for (i = 0;i < includePaths.size(); i++) {
        queryString = "insert into includes values(" + QString::number(idinclude) + ", 0, \'" + includePaths[i] + "\')";
        queryResult = query.exec(queryString);
        if ( !queryResult ) {
            errorText = tr("Unable to insert into table \'includes\': %1").arg(query.lastError().text());
            return false;
        }
        idinclude++;
    }
    for (i = 0; i < systemIncludePaths.size(); i++) {
        queryString = "insert into includes values(" + QString::number(idinclude) + ", 1, \'" + systemIncludePaths[i] + "\')";
        queryResult = query.exec(queryString);
        if ( !queryResult ) {
            errorText = tr("Unable to insert into table \'includes\': %1").arg(query.lastError().text());
            return false;
        }
        idinclude++;
    }

    int iddefine = 1;
    for (i = 0; i < defines.size(); i++) {
        queryString = "insert into defines values (" + QString::number(iddefine) + ", \'" + defines[i] + "\')";
        queryResult = query.exec(queryString);
        if ( !queryResult ) {
            errorText = tr("Unable to inserty into table \'defines\': %1").arg(query.lastError().text());
            return false;
        }
        iddefine++;
    }

    queryString = "insert into precompiledheader values(:pch)";
    query.prepare(queryString);
    query.bindValue(":pch", precompiledHeader);
    queryResult = query.exec();
    if ( !queryResult ) {
        errorText = tr("Unable to insert into table \'precompiledheader\': %1").arg(query.lastError().text());
        return false;
    }

    if ( !refreshDbFromFilesStructure(sourceFiles) ) {
        errorText = tr("Error on refresh Db Files Structure");
        return false;
    }

//    rootItem = new QProjectTreeItem(new QFolderNode(0, projectName, projectDescription, 0));
//    QProjectTreeItem *projectItem = new QProjectTreeItem(new QProjectNode(projectName, projectDescription), rootItem);
//    rootItem->appendChildFolder(projectItem);

    m_filePath = filePath;
    m_importFilePath = importFilePath;

    return (valid = true);
}

ProjectTreeItem *ProjectModel::initRootItem( const QString &projectName, const QString &projectDescription )
{
    m_rootItem = new ProjectTreeItem(new FolderNode(0, projectName, projectDescription, 0, ""));
    ProjectTreeItem *projectItem = new ProjectTreeItem(new ProjectNode(projectName, projectDescription), m_rootItem);
    m_rootItem->appendChildFolder(projectItem);
    return projectItem;
}

void ProjectModel::updateProjectItem( const QString &projectName, const QString &projectDescription )
{
    if ( m_rootItem ) {
        if ( ProjectTreeItem *projectItem = m_rootItem->child(0) ) {
            if ( ProjectNode *node = dynamic_cast<ProjectNode *>(projectItem->itemData) ) {
                node->setName(projectName);
                node->setDescription(projectDescription);
            }
        }
    }
}

bool ProjectModel::hasImportFile() const
{
    return !m_importFilePath.isEmpty();
}

void ProjectModel::synhronizeProject()
{
    if ( m_importFilePath.isEmpty() ) {
        Core::Storage::warning(tr("No Import Project-Source File Path Specified!"));
        return;
    }

    QFileInfo fi(m_importFilePath);
    if ( !fi.exists() ) {
        Core::Storage::warning(tr("File \"%1\" Not Found!").arg(m_importFilePath));
        return;
    }

    IAsofProjectImporter *importer = m_projectPlugin->projectImportManager()->findImporterByExtension(fi.completeSuffix());
    if ( !importer ) {
        Core::Storage::warning(tr("Project-Importer for Extension \"%1\" Not Found!").arg(fi.completeSuffix()));
        return;
    }

    QStringList newIncludePaths;
    QStringList newDefines;
    QString newPrecompiledHeader;

    ProjectNode *node = dynamic_cast<ProjectNode *>(m_rootItem->child(0)->itemData);
    QString projectName = node->name();
    QString projectDescription = node->description();
    ProjectTreeItem *projectItem = new ProjectTreeItem(new ProjectNode(projectName, projectDescription), m_rootItem);

    if ( !importer->executeImport(m_importFilePath, m_filePath, newIncludePaths, newDefines, newPrecompiledHeader, projectItem) ) {
        Core::Storage::warning(tr("Import From File \"%1\" Failed!").arg(m_importFilePath));
        delete projectItem;
        return;
    }

    QStringList sources;
    QStringList systemIncludePaths;

    if ( !recreateProjectDb(projectItem, newIncludePaths, newDefines, newPrecompiledHeader, systemIncludePaths, sources) ) {
        Core::Storage::warning(tr("Recreate Project Database Failed!"));
        delete projectItem;
        return;
    }

    beginResetModel();

    m_rootItem->removeChildFolder(0);
    m_rootItem->appendChildFolder(projectItem);

    endResetModel();

    m_projectPlugin->expandProjectFirstLevel();

    m_projectPlugin->emitProjectSynhronized(projectName, newIncludePaths, systemIncludePaths, newDefines, sources, newPrecompiledHeader);
}

QStringList ProjectModel::projectFiles()
{
    QStringList files;
    fillProjectFilesForItem(files, m_rootItem);
    return files;
}

ProjectTreeItem *ProjectModel::projectFolderItem()
{
    if ( !m_rootItem )
        return 0;
    if ( !m_rootItem->childCount() != 1 )
        return 0;
    return m_rootItem->child(0);
}

ProjectTreeItem *ProjectModel::defaultSourceFolderItem()
{
    if ( !m_srcIdFolder )
        return 0;

    return findFolderById(m_srcIdFolder, m_rootItem);
}

ProjectTreeItem *ProjectModel::defaultHeaderFolderItem()
{
    if ( !m_hdrIdFolder )
        return 0;

    return findFolderById(m_hdrIdFolder, m_rootItem);
}

void ProjectModel::setDefaultSourceIdFolder( int id )
{
    QSqlDatabase db = QSqlDatabase::database("ASPDB");
    QString queryString = QString("update project set src_idfolder = %1").arg(id);
    QSqlQuery query(db);

    if ( query.exec(queryString) )
        m_srcIdFolder = id;
}

void ProjectModel::setDefaultHeaderIdFolder( int id )
{
    QSqlDatabase db = QSqlDatabase::database("ASPDB");
    QString queryString = QString("update project set hdr_idfolder = %1").arg(id);
    QSqlQuery query(db);

    if ( query.exec(queryString) )
        m_hdrIdFolder = id;
}

bool ProjectModel::updateDbVersionTo1( QSqlDatabase *db )
{
    QString queryString;
    QSqlQuery query(*db);
    bool queryResult;

    // Table buildaction
    queryString = "select * from buildaction";
    queryResult = query.exec(queryString);
    query.clear();
    if ( !queryResult ) {   // Table not Exists
        queryString = "create table buildaction (idbuildaction integer primary key, action_name string, exe_file_path string, work_dir string, prior_action integer, exe_args string)";
        queryResult = query.exec(queryString);
    }
    else {                  // table exists - check prior_action
        queryString = "alter table buildaction add column prior_action integer default 0";
        query.exec(queryString);
        query.clear();      //              - check exe_args
        queryString = "alter table buildaction add column exe_args string default \'\'";
        query.exec(queryString);
    }
    query.clear();

    // Table buildenv
    queryString = "select * buildenv";
    queryResult = query.exec(queryString);
    query.clear();
    if ( !queryResult ) {   // Table not Exists
        queryString = "create table buildenv (envkey string, envvalue string)";
        query.exec(queryString);
        query.clear();
    }

    queryString = "update project set prj_version = 1";
    queryResult = query.exec(queryString);
    if ( !queryResult ) {
        errorText = tr("Update Project Version To 1 Failed");
        return false;
    }

    return true;
}

bool ProjectModel::updateDbVersionTo2( QSqlDatabase *db )
{
    QString queryString;
    QSqlQuery query(*db);
    bool queryResult;

    queryString = "alter table project add column imp_file_path string default \'\'";
    query.exec(queryString);
    query.clear();
    queryString = "alter table project add column imp_add_info string default \'\'";
    query.exec(queryString);
    query.clear();

    queryString = "update project set prj_version = 2";
    queryResult = query.exec(queryString);
    if ( !queryResult ) {
        errorText = tr("Update Project Version To 2 Failed");
        return false;
    }

    return true;
}

bool ProjectModel::updateDbVersionTo3( QSqlDatabase *db )
{
    QString queryString;
    QSqlQuery query(*db);
    bool queryResult;

    queryString = "alter table folder add column defsrcpath string default \'\'";
    query.exec(queryString);
    query.clear();

    queryString = "update project set prj_version = 3";
    queryResult = query.exec(queryString);
    if ( !queryResult ) {
        errorText = tr("Update Project Version To 3 Failed");
        return false;
    }

    return true;
}

bool ProjectModel::updateDbVersionTo4( QSqlDatabase *db )
{
    QString queryString;
    QSqlQuery query(*db);
    bool queryResult;

    queryString = "alter table project add column src_idfolder integer default 0";
    queryResult = query.exec(queryString);
    query.clear();

    queryString = "alter table project add column hdr_idfolder integer default 0";
    queryResult = query.exec(queryString);
    query.clear();

    queryString = "update project set prj_version = 4";
    queryResult = query.exec(queryString);
    if ( !queryResult ) {
        errorText = tr("Update Project Version To 3 Failed");
        return false;
    }

    return true;
}

bool ProjectModel::checkAndUpdateDbVersion( QSqlDatabase *db )
{
    QString queryString;
    QSqlQuery query(*db);
    bool queryResult;

    int dbVersion = 0;

    queryString = "select prj_version from project";
    queryResult = query.exec(queryString);
    if ( !queryResult ) {
        query.clear();
        queryString = "alter table project add column prj_version integer default 0";
        queryResult = query.exec(queryString);
        if ( !queryResult ) {
            errorText = tr("Project Updating Error: Add column \"prj_version\" into table \"project\" Failed.");
            return false;
        }
    }
    else {
        if ( !query.next() ) {
            errorText = tr("Not Found Project Information (empty table \"project\")");
            return false;
        }
        bool isDigit;
        dbVersion = query.value(0).toInt(&isDigit);
        if ( !isDigit ) {
            errorText = tr("Incorrect Project Version (not Number)");
            return false;
        }
    }

    if ( dbVersion == 0 ) {
        if ( !updateDbVersionTo1(db) )
            return false;
        dbVersion++;
    }

    if ( dbVersion == 1 ) {
        if ( !updateDbVersionTo2(db) )
            return false;
        dbVersion++;
    }
    if ( dbVersion == 2 ) {
        if ( !updateDbVersionTo3(db) )
            return false;
        dbVersion++;
    }
    if ( dbVersion == 3 ) {
        if ( !updateDbVersionTo4(db) )
            return false;
        dbVersion++;
    }
    /* Common Update Scheme
    if ( dbVersion == 4 ) {
        if ( !updateDbVersionTo5(db) )
            return false;
        dbVersion++;
    } */

    if ( dbVersion != 4 /* Last Db Version */ ) {
        errorText = tr("Unsupported Project Version");
        return false;
    }

    return true;
}

bool ProjectModel::openProjectDb( const QString &filePath, QString &projectName,
                                   QStringList &includePaths, QStringList &systemIncludePaths,
                                   QStringList &defines, QStringList &sources,
                                   QString &precompiledHeader )
{
    if ( valid ) {
        errorText = tr("Model Cannot be Reused");
        return false;
    }

    QSqlDatabase db = QSqlDatabase::database("ASPDB");
    if ( db.isOpen() ) {
        db.close();
    }

    QFile file(filePath);
    if ( !file.exists() ) {
        errorText = tr("File \'%1\' not exists").arg(filePath);
        return false;
    }

    db.setDatabaseName(filePath);
    if ( !db.open() ) {
        errorText = tr("Unable to establish a database connection");
        return false;
    }

    if ( !checkAndUpdateDbVersion(&db) )
        return false;

    QList <FolderNode *> foldersList;
    QList <FileNode *> filesList;
    QList <FolderNode *> delFoldersList;
    QList <FileNode *> delFilesList;

    QString queryString;
    bool queryResult;
    QSqlQuery query(db);

    projectName = tr("<UNKNOWN>");
    QString projectDescription = tr("<UNKNOWN>");
    m_importFilePath = "";
    queryString = "select name, description, imp_file_path, src_idfolder, hdr_idfolder from project";
    query.exec(queryString);
    if ( query.next() ) {
        projectName = query.value(0).toString();
        projectDescription = query.value(1).toString();
        m_importFilePath = query.value(2).toString();
        m_srcIdFolder = query.value(3).toInt();
        m_hdrIdFolder = query.value(4).toInt();
    }

    includePaths.clear();
    systemIncludePaths.clear();
    queryString = "select issystem, path from includes";
    query.exec(queryString);
    while ( query.next() ) {
        if ( query.value(0).toInt() )
            systemIncludePaths << query.value(1).toString();
        else
            includePaths << query.value(1).toString();
    }

    defines.clear();
    queryString = "select definename from defines";
    query.exec(queryString);
    while ( query.next() ) {
        defines << query.value(0).toString();
    }

    queryString = "select header from precompiledheader";
    queryResult = query.exec(queryString);
    if ( queryResult ) {
        if ( query.next() )
            precompiledHeader = query.value(0).toString();
    }
    else {
        queryString = "create table precompiledheader (header string)";
        query.exec(queryString);
        precompiledHeader = "";
    }

    queryString = "select idfolder, name, description, idparentfolder, defsrcpath from folder order by idparentfolder, name";
    query.exec(queryString);
    while ( query.next() ) {
        int idfolder = query.value(0).toInt();
        foldersList.append( new FolderNode( idfolder,
                                             query.value(1).toString(),
                                             query.value(2).toString(),
                                             query.value(3).toInt(),
                                             query.value(4).toString() ) );
    }

    queryString = "select idfile, name, path, idparentfolder from file order by idparentfolder, name";
    query.exec(queryString);
    while ( query.next() ) {
        int idfile = query.value(0).toInt();
        filesList.append(new FileNode(idfile, query.value(1).toString(), query.value(2).toString(), query.value(3).toInt()));
    }

    QMap <int, ProjectTreeItem *> foldersMap;

    if ( m_rootItem )
        delete m_rootItem;

    m_rootItem = new ProjectTreeItem( new FolderNode(0, projectName, projectDescription, 0, "") );
    ProjectTreeItem *projectItem = new ProjectTreeItem(new ProjectNode(projectName, projectDescription), m_rootItem);
    foldersMap[0] = projectItem;
    m_rootItem->appendChildFolder(projectItem);

    foreach ( FolderNode *folder, foldersList ) {
        int idfolder = folder->idfolder;
        int idparentfolder = folder->idparentfolder;
        if ( foldersMap.contains(idparentfolder) ) {
            foldersMap[idfolder] = new ProjectTreeItem(folder, foldersMap[idparentfolder]);
            foldersMap[idparentfolder]->appendChildFolder(foldersMap[idfolder]);
        }
        else
            delFoldersList.append(folder);
    }

    foreach ( FileNode *file, filesList ) {
        int idparentfolder = file->idparentfolder;
        if ( foldersMap.contains(idparentfolder) ) {
            foldersMap[idparentfolder]->appendChildFile(new ProjectTreeItem(file, foldersMap[idparentfolder]));
            sources.append(file->description());
        }
        else
            delFilesList.append(file);
    }

    // ќчистка вис€чих записей
    foreach ( FolderNode *delFolder, delFoldersList ) {
        queryString = "delete from folder where idfolder = " + QString::number(delFolder->idfolder);
        query.exec(queryString);
    }
    foreach ( FileNode *delFile, delFilesList ) {
        queryString = "delete from file where idfile = " + QString::number(delFile->idfile);
        query.exec(queryString);
    }
    qDeleteAll(delFoldersList);
    qDeleteAll(delFilesList);

    m_filePath = filePath;

    m_projectBuildActionsModel->readFromDb();

    return (valid = true);
}

void ProjectModel::closeProjectDb()
{
    QSqlDatabase db = QSqlDatabase::database("ASPDB");
    if ( db.isOpen() ) {
        db.close();
    }
    delete m_projectBuildActionsModel;
    m_projectBuildActionsModel = new ProjectBuildActionsModel(m_projectPlugin->buildActionsManager());
    valid = false;
}

bool ProjectModel::refreshFilesStructureFromDb( QStringList &sources )
{
    ProjectTreeItem *projectItem = m_rootItem->child(0);
    QSqlDatabase db = QSqlDatabase::database("ASPDB");
    QList <FolderNode *> foldersList;
    QList <FileNode *> filesList;
    QList <FolderNode *> delFoldersList;
    QList <FileNode *> delFilesList;

    QString queryString;
    QSqlQuery query(db);

    queryString = "select idfolder, name, description, idparentfolder, defsrcpath from folder order by idparentfolder, name";
    query.exec(queryString);
    while ( query.next() ) {
        foldersList.append( new FolderNode( query.value(0).toInt(),
                                             query.value(1).toString(),
                                             query.value(2).toString(),
                                             query.value(3).toInt(),
                                             query.value(4).toString() ) );
    }

    queryString = "select idfile, name, path, idparentfolder from file order by idparentfolder, name";
    query.exec(queryString);
    while ( query.next() ) {
        int idfile = query.value(0).toInt();
        filesList.append(new FileNode(idfile, query.value(1).toString(), query.value(2).toString(), query.value(3).toInt()));
    }

    QMap <int, ProjectTreeItem *> foldersMap;

    foldersMap[0] = projectItem;

    foreach ( FolderNode *folder, foldersList ) {
        int idfolder = folder->idfolder;
        int idparentfolder = folder->idparentfolder;
        if ( foldersMap.contains(idparentfolder) ) {
            foldersMap[idfolder] = new ProjectTreeItem(folder, foldersMap[idparentfolder]);
            foldersMap[idparentfolder]->appendChildFolder(foldersMap[idfolder]);
        }
        else
            delFoldersList.append(folder);
    }

    foreach ( FileNode *file, filesList ) {
        int idparentfolder = file->idparentfolder;
        if ( foldersMap.contains(idparentfolder) ) {
            foldersMap[idparentfolder]->appendChildFile(new ProjectTreeItem(file, foldersMap[idparentfolder]));
            sources.append(file->description());
        }
        else
            delFilesList.append(file);
    }

    // ќчистка вис€чих записей
    foreach ( FolderNode *delFolder, delFoldersList ) {
        queryString = "delete from folder where idfolder = " + QString::number(delFolder->idfolder);
        query.exec(queryString);
    }
    foreach ( FileNode *delFile, delFilesList ) {
        queryString = "delete from file where idfile = " + QString::number(delFile->idfile);
        query.exec(queryString);
    }
    qDeleteAll(delFoldersList);
    qDeleteAll(delFilesList);

    return (valid = true);
}

bool ProjectModel::refreshDbFromFilesStructureInternal( QSqlDatabase *db, ProjectTreeItem *item, int idparentfolder, QStringList &sourceFiles,
                                                         const QString &folderPrefix, const QMap<QString, QString> &folderToDefaultPathMap )
{
    QSqlQuery query(*db);
    QString queryString;
    QString fPrefix;

    switch ( item->nodeType() ) {
    case 1: // Folder
        queryString = "insert into folder values (:idfolder, :name, :description, :idparentfolder, :defSourcePath)";
        query.prepare(queryString);
        query.bindValue(":idfolder", item->id());
        query.bindValue(":name", item->name());
        query.bindValue(":description", item->description());
        query.bindValue(":idparentfolder", idparentfolder);
        fPrefix = folderPrefix + "/" + item->name();
        if ( folderToDefaultPathMap.find(fPrefix) != folderToDefaultPathMap.end() )
            item->setDefaultSourcePath(folderToDefaultPathMap[fPrefix]);
        query.bindValue(":defSourcePath", item->defaultSourcePath());
        query.exec();
        query.clear();
        for (int i = 0; i < item->childCount(); i++)
            if ( !refreshDbFromFilesStructureInternal(db, item->child(i), item->id(), sourceFiles, fPrefix, folderToDefaultPathMap) )
                return false;
        break;
    case 2: // File
        queryString = "insert into file values (:idfile, :name, :path, :idparentfolder)";
        query.prepare(queryString);
        query.bindValue(":idfile", item->id());
        query.bindValue(":name", item->name());
        query.bindValue(":path", item->description());
        query.bindValue(":idparentfolder", idparentfolder);
        query.exec();
        query.clear();
        sourceFiles << item->description();
        break;
    }

    return true;
}

void ProjectModel::saveFoldersDefaultPaths( ProjectTreeItem *parentFolder, const QString &folderPrefix, QMap<QString, QString> &folderToDefaultPathMap )
{
    QString key = folderPrefix + "/" + parentFolder->name();

    folderToDefaultPathMap[key] = parentFolder->defaultSourcePath();

    for (int i = 0; i < parentFolder->childFoldersCount(); i++)
        saveFoldersDefaultPaths(parentFolder->folder(i), key, folderToDefaultPathMap);
}

bool ProjectModel::recreateProjectDb( ProjectTreeItem *projectItem, const QStringList &newIncludePaths, const QStringList &newDefines, const QString &newPrecompiledHeader,
                                       QStringList &systemIncludePaths, QStringList &sources )
{
    QSqlDatabase db = QSqlDatabase::database("ASPDB");
    QString queryString;
    QSqlQuery query(db);
    bool queryResult;

    QMap<QString, QString> folderToDefaultPathMap;

    /* Saving systemIncludePaths */
    systemIncludePaths.clear();
    queryString = "select issystem, path from includes";
    queryResult = query.exec(queryString);
    if ( !queryResult ) {
        db.rollback();
        return false;
    }
    while ( query.next() ) {
        if ( query.value(0).toInt() )
            systemIncludePaths << query.value(1).toString();
    }

    /* Save Default Folders Paths */
    saveFoldersDefaultPaths(m_rootItem->child(0), "", folderToDefaultPathMap);

    db.transaction();

    /* INCLUDES */
    queryString = "delete from includes";
    queryResult = query.exec(queryString);
    if ( !queryResult ) {
        db.rollback();
        return false;
    }
    queryString = "insert into includes (issystem, path) values (0, :inc)";
    foreach ( QString inc, newIncludePaths ) {
        query.prepare(queryString);
        query.bindValue(":inc", inc);
        queryResult = query.exec();
        if ( !queryResult ) {
            db.rollback();
            return false;
        }
    }
    queryString = "insert into includes (issystem, path) values (1, :sysInc)";
    foreach ( QString sysInc, systemIncludePaths ) {
        query.prepare(queryString);
        query.bindValue(":sysInc", sysInc);
        queryResult = query.exec();
        if ( !queryResult ) {
            db.rollback();
            return false;
        }
    }

    /* DEFINES */
    queryString = "delete from defines";
    queryResult = query.exec(queryString);
    if ( !queryResult ) {
        db.rollback();
        return false;
    }
    queryString = "insert into defines (definename) values (:def)";
    foreach ( QString def, newDefines ) {
        query.prepare(queryString);
        query.bindValue(":def", def);
        queryResult = query.exec();
        if ( !queryResult ) {
            db.rollback();
            return false;
        }
    }

    /* PRECOMPILEDHEADER */
    queryString = "delete from precompiledheader";
    queryResult = query.exec(queryString);
    if ( !queryResult ) {
        db.rollback();
        return false;
    }
    queryString = "insert into precompiledheader values (:pch)";
    query.prepare(queryString);
    query.bindValue(":pch", newPrecompiledHeader);
    queryResult = query.exec();
    if ( !queryResult ) {
        db.rollback();
        return false;
    }

    /* FILES/FOLDERS */
    queryString = "delete from folder";
    queryResult = query.exec(queryString);
    if ( !queryResult ) {
        db.rollback();
        return false;
    }
    queryString = "delete from file";
    queryResult = query.exec(queryString);
    if ( !queryResult ) {
        db.rollback();
        return false;
    }
    for (int i = 0; i < projectItem->childCount(); i++) {
        if ( !refreshDbFromFilesStructureInternal(&db, projectItem->child(i), 0, sources, "/" + projectItem->name(), folderToDefaultPathMap) ) {
            db.rollback();
            return false;
        }
    }

    /// TODO: save default source/header folders
    setDefaultHeaderIdFolder(0);
    setDefaultSourceIdFolder(0);

    db.commit();
    return true;
}

void ProjectModel::fillProjectFilesForItem( QStringList &files, ProjectTreeItem *item )
{
    if ( item->nodeType() == 2 ) {
        files << item->data(1).toString();
        return;
    }

    for (int i = 0; i < item->childCount(); i++)
        fillProjectFilesForItem(files, item->child(i));
}

ProjectTreeItem *ProjectModel::findFolderById( int folderId , ProjectTreeItem *item )
{
    if ( !item )
        return 0;
    if ( item->id() == folderId )
        return item;
    for (int i = 0; i < item->childFoldersCount(); i++)
        if ( ProjectTreeItem *childFolder = findFolderById(folderId, item->child(i)) )
            return childFolder;
    return 0;
}

bool ProjectModel::refreshDbFromFilesStructure( QStringList &sourceFiles, ProjectTreeItem *prjItem )
{
    ProjectTreeItem *projectItem = prjItem;
    if ( !projectItem )
        projectItem = m_rootItem->child(0);

    QSqlDatabase db = QSqlDatabase::database("ASPDB");

    QString queryString;
    QSqlQuery query(db);

    queryString = "delete from folder";
    query.exec(queryString);
    query.clear();

    queryString = "delete from file";
    query.exec(queryString);
    query.clear();

    db.transaction();

    for(int i = 0; i < projectItem->childCount(); i++)
        if ( !refreshDbFromFilesStructureInternal(&db, projectItem->child(i), 0, sourceFiles, "") ) {
            db.rollback();
            return false;
        }

    db.commit();

    return true;
}

int ProjectModel::nodeType( const QModelIndex &index ) const
{
    if ( !index.isValid() )
        return -1;

    ProjectTreeItem *item = static_cast<ProjectTreeItem *>(index.internalPointer());
    if ( !item )
        return -1;

    return item->nodeType();
}

void ProjectModel::addFolder( const QString &folderName, const QString &folderDescription, const QString &defaultSourcePath,
                               const QModelIndex &parent, FolderNode::FolderType folderType )
{
    if ( !valid )
        return;

    qDebug() << "QProjectModel::addFolder 01";

    QSqlDatabase db = QSqlDatabase::database("ASPDB");
    if ( !db.isOpen() )
        return;

    if ( !parent.isValid() )
        return;

    ProjectTreeItem *parentItem = static_cast<ProjectTreeItem *>(parent.internalPointer());
    if ( !parentItem )
        return;

    int idfolder = 0;
    int idparentfolder = parentItem->id();
    QString queryString;
    QSqlQuery query(db);

    queryString = "select max(idfolder) from folder";
    query.exec(queryString);
    if ( query.next() )
        idfolder = query.value(0).toInt();
    idfolder++;

    queryString = "insert into folder values(:idfolder, :name, :description, :idparentfolder, :defsrcpath)";
    query.prepare(queryString);
    query.bindValue(":idfolder", idfolder);
    query.bindValue(":name", folderName);
    query.bindValue(":description", folderDescription);
    query.bindValue(":idparentfolder", idparentfolder);
    query.bindValue(":defsrcpath", defaultSourcePath);

    if ( !query.exec() )
        return;

    int pos = parentItem->childFoldersCount();
    beginInsertRows(parent, pos, pos + 1);
    ProjectTreeItem *folderItem = new ProjectTreeItem(new FolderNode(idfolder, folderName, folderDescription, idparentfolder, defaultSourcePath), parentItem);
    parentItem->appendChildFolder(folderItem);
    endInsertRows();

    if ( folderType == FolderNode::DefaultSource ) {
        setDefaultSourceIdFolder(idfolder);
    } else if ( folderType == FolderNode::DefaultHeader ) {
        setDefaultHeaderIdFolder(idfolder);
    }
}

void ProjectModel::editFolder( const QString &folderName, const QString &folderDescription, const QString &defaultSourcePath, const QModelIndex &index )
{
    if ( !valid )
        return;

    QSqlDatabase db = QSqlDatabase::database("ASPDB");
    if ( !db.isOpen() )
        return;

    if ( !index.isValid() )
        return;

    ProjectTreeItem *folderItem = static_cast<ProjectTreeItem *>(index.internalPointer());
    if ( !folderItem )
        return;

    int idfolder = folderItem->id();
    QString queryString;
    QSqlQuery query(db);
    queryString = "update folder set name = :folderName, description = :folderDescription, defsrcpath = :defaultSourcePath where idfolder = " + QString::number(idfolder);
    query.prepare(queryString);
    query.bindValue(":folderName", folderName);
    query.bindValue(":folderDescription", folderDescription);
    query.bindValue(":defaultSourcePath", defaultSourcePath);
    if ( query.exec() ) {
        folderItem->itemData->setName(folderName);
        folderItem->itemData->setDescription(folderDescription);
        folderItem->itemData->setDefaultSourcePath(defaultSourcePath);
        emit dataChanged(index, index);
    }
    else {
        Core::Storage::warning(tr("Update Folder Properties Failed:\n%1").arg(query.lastError().text()));
    }
}

void ProjectModel::addFiles( const QStringList &files, const QModelIndex &parent )
{
    int filesCount = files.size();
    if ( !filesCount )
        return;

    if ( !valid )
        return;

    QSqlDatabase db = QSqlDatabase::database("ASPDB");
    if ( !db.isOpen() )
        return;

    if ( !parent.isValid() )
        return;

    ProjectTreeItem *parentItem = static_cast<ProjectTreeItem *>(parent.internalPointer());
    if ( !parentItem )
        return;

    int idfile = 0;
    int idparentfolder = parentItem->id();
    QString queryString;
    QSqlQuery query(db);

    db.transaction();

    queryString = "select max(idfile) from file";
    query.exec(queryString);
    if ( query.next() )
        idfile = query.value(0).toInt();
    idfile++;

    QStringList newFiles;
    foreach ( QString f, files ) {
        queryString = "select * from file where upper(path) = :path";
        query.prepare(queryString);
        query.bindValue(":path", f.toUpper());
        query.exec();
        if ( !query.next() )
            newFiles << f;
    }
    filesCount = newFiles.size();

    if ( !filesCount )
        return;

    int pos = parentItem->childFilesCount();
    beginInsertRows(parent, pos, pos + filesCount);
    foreach ( QString filePath, newFiles ) {
        QFileInfo fi(filePath);

        queryString = "insert into file values(:idfile, :name, :path, :idparentfolder)";
        query.prepare(queryString);
        query.bindValue(":idfile", idfile);
        query.bindValue(":name", fi.fileName());
        query.bindValue(":path", fi.filePath());
        query.bindValue(":idparentfolder", idparentfolder);
        query.exec();

        ProjectTreeItem *fileItem = new ProjectTreeItem(new FileNode(idfile, fi.fileName(), fi.filePath(), idparentfolder), parentItem);
        parentItem->appendChildFile(fileItem);

        idfile++;
    }
    endInsertRows();

    db.commit();

    QVariantList valuesList;
    valuesList << 1; // Add Files
    valuesList << newFiles;
    m_projectPlugin->emitProjectChanged(valuesList);
}

ProjectTreeItem *ProjectModel::addDocument( ProjectTreeItem *parent, const QString &filePath )
{
    if ( !parent )
        return 0;

    if ( !valid )
        return 0;

    QSqlDatabase db = QSqlDatabase::database("ASPDB");
    if ( !db.isOpen() )
        return 0;

    int idfile = 0;
    int idparentfolder = parent->id();

    QString queryString;
    QSqlQuery query(db);


    queryString = "select max(idfile) from file";
    query.exec(queryString);
    if ( query.next() )
        idfile = query.value(0).toInt();
    idfile++;

    QFileInfo fi(filePath);

    int pos = parent->childFilesCount();
    beginInsertRows(createIndex(parent->parent()->childFolders.indexOf(parent), 0, parent), pos, pos + 1);

    queryString = "insert into file values(:idfile, :name, :path, :idparentfolder)";
    query.prepare(queryString);
    query.bindValue(":idfile", idfile);
    query.bindValue(":name", fi.fileName());
    query.bindValue(":path", fi.filePath());
    query.bindValue(":idparentfolder", idparentfolder);
    query.exec();

    ProjectTreeItem *fileItem = new ProjectTreeItem(new FileNode(idfile, fi.fileName(), fi.filePath(), idparentfolder), parent);
    parent->appendChildFile(fileItem);
    endInsertRows();

    return fileItem;
}

void ProjectModel::removeFile( const QModelIndex &index )
{
    if ( !valid )
        return;

    QSqlDatabase db = QSqlDatabase::database("ASPDB");
    if ( !db.isOpen() )
        return;

    if ( !index.isValid() || !index.parent().isValid() )
        return;

    ProjectTreeItem *fileItem = static_cast<ProjectTreeItem *>(index.internalPointer());
    if ( !fileItem )
        return;

    QStringList removeFiles;
    removeFiles << fileItem->description();

    int idfile = fileItem->id();

    QString queryString;
    QSqlQuery query(db);

    queryString = "delete from file where idfile = :id";
    query.prepare(queryString);
    query.bindValue(":id", idfile);
    query.exec();

    int pos = fileItem->row();
    int foldersCount = fileItem->parent()->childFoldersCount();

    beginRemoveRows(index.parent(), pos, pos + 1);
    fileItem->parent()->removeChildFile(pos - foldersCount);
    endRemoveRows();

    QVariantList valuesList;
    valuesList << 2;        // RemoveFile
    valuesList << removeFiles;
    m_projectPlugin->emitProjectChanged(valuesList);
}

void ProjectModel::dbRemoveFolder( ProjectTreeItem *folderItem, QStringList &removedFiles )
{
    for ( int i = 0; i < folderItem->childFoldersCount(); i++ )
        dbRemoveFolder(folderItem->folder(i), removedFiles);

    QSqlDatabase db = QSqlDatabase::database("ASPDB");
    if ( !db.isOpen() )
        return;

    QString queryString;
    QSqlQuery query(db);
    int idfolder = folderItem->id();

    queryString = "delete from folder where idparentfolder = :id";
    query.prepare(queryString);
    query.bindValue(":id", idfolder);
    query.exec();

    queryString = "select path from file where idparentfolder = :id";
    query.prepare(queryString);
    query.bindValue(":id", idfolder);
    query.exec();
    while ( query.next() )
        removedFiles << query.value(0).toString();
    query.clear();

    queryString = "delete from file where idparentfolder = :id";
    query.prepare(queryString);
    query.bindValue(":id", idfolder);
    query.exec();

    queryString = "delete from tag where idparentfolder = :id";
    query.prepare(queryString);
    query.bindValue(":id", idfolder);
    query.exec();

    queryString = "delete from filetag where idparentfolder = :id";
    query.prepare(queryString);
    query.bindValue(":id", idfolder);
    query.exec();

    queryString = "delete from folder where idfolder = :id";
    query.prepare(queryString);
    query.bindValue(":id", idfolder);
    query.exec();
}

void ProjectModel::removeFolder( const QModelIndex &index )
{
    if ( !valid )
        return;

    if ( !index.isValid() || !index.parent().isValid() )
        return;

    ProjectTreeItem *folderItem = static_cast<ProjectTreeItem *>(index.internalPointer());
    if ( !folderItem )
        return;

    if ( folderItem->id() == m_srcIdFolder )
        setDefaultSourceIdFolder(0);
    else if ( folderItem->id() == m_hdrIdFolder )
        setDefaultHeaderIdFolder(0);

    QStringList removedFiles;
    dbRemoveFolder(folderItem, removedFiles);

    int pos = folderItem->row();

    beginRemoveRows(index.parent(), pos, pos + 1);
    folderItem->parent()->removeChildFolder(pos);
    endRemoveRows();

    QVariantList valuesList;
    valuesList << 2;        // Remove Files
    valuesList << removedFiles;
    m_projectPlugin->emitProjectChanged(valuesList);
}

void ProjectModel::getProjectProperties( QString &projectName, QString &projectDescription, QString &filePath,
                                          QStringList &includePaths, QStringList &systemIncludePaths, QStringList &defines,
                                          QString &precompiledHeader )
{
    QSqlDatabase db = QSqlDatabase::database("ASPDB");
    if ( !db.isOpen() )
        return;

    filePath = m_filePath;

    QString queryString;
    QSqlQuery query(db);

    queryString = "select name, description from project";
    query.exec(queryString);
    if ( query.next() ) {
        projectName = query.value(0).toString();
        projectDescription = query.value(1).toString();
    }

    includePaths.clear();
    systemIncludePaths.clear();
    queryString = "select issystem, path from includes";
    query.exec(queryString);
    while ( query.next() ) {
        if ( query.value(0).toInt() )
            systemIncludePaths << query.value(1).toString();
        else
            includePaths << query.value(1).toString();
    }

    defines.clear();
    queryString = "select definename from defines";
    query.exec(queryString);
    while ( query.next() ) {
        defines << query.value(0).toString();
    }

    precompiledHeader = "";
    queryString = "select header from precompiledheader";
    query.exec(queryString);
    if ( query.next() )
        precompiledHeader = query.value(0).toString();
}

bool ProjectModel::changeProjectSettings( const QString &projectName, const QString &projectDescription,
                                           const QStringList &includePaths, const QStringList &systemIncludePaths, const QStringList &defines, QStringList &sources,
                                           const QString &precompiledHeader )
{
    QSqlDatabase db = QSqlDatabase::database("ASPDB");
    if ( !db.isOpen() )
        return false;

    QString queryString;
    QSqlQuery query(db);
    bool queryResult;

    db.transaction();

    queryString = "update project set name = :projectName, description = :projectDescription";
    query.prepare(queryString);
    query.bindValue(":projectName", projectName);
    query.bindValue("::projectDescription", projectDescription);
    queryResult = query.exec();
    if ( !queryResult ) {
        db.rollback();
        return false;
    }

    queryString = "delete from includes";
    queryResult = query.exec(queryString);
    if ( !queryResult ) {
        db.rollback();
        return false;
    }
    queryString = "insert into includes (issystem, path) values (0, :inc)";
    foreach ( QString inc, includePaths ) {
        query.prepare(queryString);
        query.bindValue(":inc", inc);
        queryResult = query.exec();
        if ( !queryResult ) {
            db.rollback();
            return false;
        }
    }
    queryString = "insert into includes (issystem, path) values (1, :sysInc)";
    foreach ( QString sysInc, systemIncludePaths ) {
        query.prepare(queryString);
        query.bindValue(":sysInc", sysInc);
        queryResult = query.exec();
        if ( !queryResult ) {
            db.rollback();
            return false;
        }
    }

    queryString = "delete from defines";
    queryResult = query.exec(queryString);
    if ( !queryResult ) {
        db.rollback();
        return false;
    }
    queryString = "insert into defines (definename) values (:def)";
    foreach ( QString def, defines ) {
        query.prepare(queryString);
        query.bindValue(":def", def);
        queryResult = query.exec();
        if ( !queryResult ) {
            db.rollback();
            return false;
        }
    }

    queryString = "delete from precompiledheader";
    queryResult = query.exec(queryString);
    if ( !queryResult ) {
        db.rollback();
        return false;
    }
    queryString = "insert into precompiledheader values (:pch)";
    query.prepare(queryString);
    query.bindValue(":pch", precompiledHeader);
    queryResult = query.exec();
    if ( !queryResult ) {
        db.rollback();
        return false;
    }

    queryString = "select path from file";
    query.exec(queryString);
    while ( query.next() )
        sources.append(query.value(0).toString());

    db.commit();

    return true;
}

void ProjectModel::getProjectFiles( QStringList &fileNames ) const
{
    QSqlDatabase db = QSqlDatabase::database("ASPDB");
    if ( !db.isOpen() )
        return;

    QString queryString;
    QSqlQuery query(db);

    queryString = "select path from file";
    query.exec(queryString);
    while ( query.next() )
        fileNames << query.value(0).toString();
}

void ProjectModel::configureProjectBuildActions()
{
    ConfigureProjectBuildActionsDialog confProjectBuildActionsDialog(m_projectBuildActionsModel, Core::Storage::mainWindow()->widget());
    confProjectBuildActionsDialog.exec();
}
