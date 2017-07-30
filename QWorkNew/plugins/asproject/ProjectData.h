#ifndef QPROJECTDATA_H
#define QPROJECTDATA_H

#include <QObject>
#include <QList>
#include <QVariant>
#include <QAbstractItemModel>
#include <QSqlDatabase>
#include <QAction>

#include "ProjectBuildActionsModel.h"
#include "asproject_global.h"

class AsofProjectPlugin;
class ProjectModel;

class ASPROJECT_EXPORT INode
{
public:
    virtual QString name() const = 0;
    virtual QString description() const = 0;
    virtual QString icon() const = 0;

    virtual int nodeType() const { return -1; }
    virtual int id() const = 0;
    virtual QString defaultSourcePath() const = 0;

    virtual void setName( const QString &name_ ) = 0;
    virtual void setDescription( const QString &description_ ) = 0;
    virtual void setDefaultSourcePath( const QString &defaultSourcePath_ ) = 0;
};

class ASPROJECT_EXPORT ProjectNode : public INode
{
public:
    ProjectNode( const QString &projectName, const QString &projectDescription )
        : m_projectName(projectName)
        , m_projectDescription(projectDescription)
    {}

    virtual QString name() const { return m_projectName; }
    virtual QString description() const { return m_projectDescription; }
    virtual QString icon() const { return ":/images/prj_project.png"; }

    virtual int nodeType() const { return 0; }
    virtual int id() const { return 0; }
    virtual QString defaultSourcePath() const { return ""; }

    virtual void setName( const QString &name ) { m_projectName = name; }
    virtual void setDescription( const QString &description ) { m_projectDescription = description; }
    virtual void setDefaultSourcePath( const QString &defaultSourcePath_ ) { Q_UNUSED(defaultSourcePath_) }

private:
    QString m_projectName;
    QString m_projectDescription;
};

class ASPROJECT_EXPORT FolderNode : public INode
{
public:
    friend class ProjectModel;

    enum FolderType {
        NotAssigned,
        DefaultSource,
        DefaultHeader
    };

    FolderNode( int idfolder_, const QString &folderName_, const QString &folderDescription_, int idparentfolder_, const QString &defaultSourcePath_ )
        : idfolder(idfolder_)
        , folderName(folderName_)
        , folderDescription(folderDescription_)
        , idparentfolder(idparentfolder_)
        , defSourcePath(defaultSourcePath_)
    {}

    virtual QString name() const { return folderName; }
    virtual QString description() const { return folderDescription; }
    virtual QString icon() const { return ":/images/prj_folder.png"; }

    virtual int nodeType() const { return 1; }
    virtual int id() const { return idfolder; }
    virtual QString defaultSourcePath() const { return defSourcePath; }

    virtual void setName( const QString &name_ ) { folderName = name_; }
    virtual void setDescription( const QString &description_ ) { folderDescription = description_; }
    virtual void setDefaultSourcePath( const QString &defaultSourcePath_ ) { defSourcePath = defaultSourcePath_; }

private:
    int     idfolder;
    QString folderName;
    QString folderDescription;
    int     idparentfolder;
    QString defSourcePath;
};

class ASPROJECT_EXPORT FileNode : public INode
{
public:
    friend class ProjectModel;

    FileNode( int idfile_, const QString &fileName_, const QString &filePath_, int idparentfolder_ )
        : idfile(idfile_)
        , fileName(fileName_)
        , filePath(filePath_)
        , idparentfolder(idparentfolder_)
    {}

    virtual QString name() const { return fileName; }
    virtual QString description() const { return filePath; }
    virtual QString icon() const { return ":/images/prj_file.png"; }

    virtual int nodeType() const { return 2; }
    virtual int id() const { return idfile; }
    virtual QString defaultSourcePath() const { return ""; }

    virtual void setName( const QString &/*name_*/ ) {}
    virtual void setDescription( const QString &/*description_*/ ) {}
    virtual void setDefaultSourcePath( const QString &/*defaultSourcePath_*/ ) {}

private:
    int     idfile;
    QString fileName;
    QString filePath;
    int     idparentfolder;
};

class ASPROJECT_EXPORT ProjectTreeItem
{
public:
    friend class ProjectModel;

    ProjectTreeItem( INode *itemData_, ProjectTreeItem *parent = 0 )
        : itemData(itemData_)
        , parentItem(parent)
    {}

    virtual ~ProjectTreeItem();

    void appendChildFolder( ProjectTreeItem *child ) { childFolders.append(child); }
    void appendChildFile( ProjectTreeItem *child ) { childFiles.append(child); }
    void removeChildFile( int fileIndex) { delete childFiles[fileIndex]; childFiles.removeAt(fileIndex); }
    void removeChildFolder( int folderIndex ) { delete childFolders[folderIndex]; childFolders.removeAt(folderIndex); }

    ProjectTreeItem *child( int row );

    int childCount() const { return childFolders.size() + childFiles.size(); }
    int childFoldersCount() const { return childFolders.size(); }
    int childFilesCount() const { return childFiles.size(); }

    int columnCount() const { return 1; }

    QVariant data(int column) const;
    int row() const;

    ProjectTreeItem *parent() { return parentItem; }

    int nodeType() const { return itemData->nodeType(); }
    int id() const { return itemData->id(); }
    QString name() const { return itemData->name(); }
    QString description() const { return itemData->description(); }
    QString defaultSourcePath() const { return itemData->defaultSourcePath(); }
    void setDefaultSourcePath( const QString &path ) { itemData->setDefaultSourcePath(path); }

    ProjectTreeItem *folder( int folderIndex ) const { return childFolders[folderIndex]; }
    ProjectTreeItem *file( int fileIndex ) const { return childFiles[fileIndex]; }

protected:
    INode *itemData;
    ProjectTreeItem *parentItem;
    QList <ProjectTreeItem *> childFolders;
    QList <ProjectTreeItem*> childFiles;
};

class ASPROJECT_EXPORT ProjectModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    ProjectModel( AsofProjectPlugin *projectPlugin, QObject *parent = 0 );
    ~ProjectModel();

    void initEmpty();

    bool isValid() const { return valid; }
    QString lastError() const { return errorText; }

    bool createProjectDb( const QString &projectName, const QString &projectDescription, const QString &filePath, const QString &importFilePath,
                          const QStringList &includePaths, const QStringList &systemIncludePaths, const QStringList &defines,
                          const QString &precompiledHeader, QStringList &sourceFiles );
    bool openProjectDb( const QString &filePath, QString &projectName,
                        QStringList &includePaths, QStringList &systemIncludePaths, QStringList &defines,
                        QStringList &sources,
                        QString &precompiledHeader );
    void closeProjectDb();
    bool refreshFilesStructureFromDb( QStringList &sources );
    bool refreshDbFromFilesStructure(QStringList &sourceFiles , ProjectTreeItem *prjItem = 0 );

    void getProjectProperties( QString &projectName, QString &projectDescription, QString &filePath,
                               QStringList &includePaths, QStringList &systemIncludePaths, QStringList &defines,
                               QString &precompiledHeader );
    bool changeProjectSettings( const QString &projectName, const QString &projectDescription,
                                const QStringList &includePaths, const QStringList &systemIncludePaths, const QStringList &defines, QStringList &sources,
                                const QString &precompiledHeader );

    int nodeType( const QModelIndex &index ) const;

    void addFolder( const QString &folderName, const QString &folderDescription, const QString &defaultSourcePath,
                    const QModelIndex &parent, FolderNode::FolderType folderType );
    void editFolder(const QString &folderName, const QString &folderDescription, const QString &defaultSourcePath, const QModelIndex &index );
    void addFiles( const QStringList &files, const QModelIndex &parent );
    ProjectTreeItem *addDocument( ProjectTreeItem *parent, const QString &filePath );
    void removeFile( const QModelIndex &index );
    void removeFolder( const QModelIndex &index );

    QVariant data( const QModelIndex &index, int role ) const;
    Qt::ItemFlags flags( const QModelIndex &index ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
    QModelIndex parent( const QModelIndex &index ) const;
    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const;

    void getProjectFiles( QStringList &fileNames ) const;

    QString projectFilePath() const { return m_filePath; }

    void configureProjectBuildActions();
    QList<QAction *> projectBuildActions() const { return m_projectBuildActionsModel->projectBuildActions(); }

    ProjectTreeItem *initRootItem( const QString &projectName, const QString &projectDescription );
    void updateProjectItem( const QString &projectName, const QString &projectDescription );
    bool hasImportFile() const;

    void synhronizeProject();

    QStringList projectFiles();
    ProjectTreeItem *projectFolderItem();
    ProjectTreeItem *defaultSourceFolderItem();
    ProjectTreeItem *defaultHeaderFolderItem();

    int defaultSourceIdFolder() const { return m_srcIdFolder; }
    int defaultHeaderIdFolder() const { return m_hdrIdFolder; }
    void setDefaultSourceIdFolder( int id );
    void setDefaultHeaderIdFolder( int id );

private:
    ProjectTreeItem *m_rootItem;
    bool valid;
    QString errorText;
    QString m_filePath;
    QString m_importFilePath;
    int m_srcIdFolder;
    int m_hdrIdFolder;

    AsofProjectPlugin *m_projectPlugin;

    ProjectBuildActionsModel *m_projectBuildActionsModel;

    void dbRemoveFolder( ProjectTreeItem *folderItem, QStringList &removedFiles );
    bool updateDbVersionTo1( QSqlDatabase *db );
    bool updateDbVersionTo2( QSqlDatabase *db );
    bool updateDbVersionTo3( QSqlDatabase *db );
    bool updateDbVersionTo4( QSqlDatabase *db );
    bool checkAndUpdateDbVersion( QSqlDatabase *db );

    bool refreshDbFromFilesStructureInternal( QSqlDatabase *db, ProjectTreeItem *item, int idparentfolder, QStringList &sourceFiles,
                                              const QString &folderPrefix, const QMap<QString, QString> &folderToDefaultPathMap = QMap<QString, QString>() );
    void saveFoldersDefaultPaths( ProjectTreeItem *parentFolder, const QString &folderPrefix, QMap<QString, QString> &folderToDefaultPathMap );
    bool recreateProjectDb( ProjectTreeItem *projectItem, const QStringList &newIncludePaths, const QStringList &newDefines, const QString &newPrecompiledHeader,
                            QStringList &systemIncludePaths, QStringList &sources );

    void fillProjectFilesForItem( QStringList &files, ProjectTreeItem *item );
    ProjectTreeItem *findFolderById( int folderId, ProjectTreeItem *item );
};

#endif // QPROJECTDATA_H
