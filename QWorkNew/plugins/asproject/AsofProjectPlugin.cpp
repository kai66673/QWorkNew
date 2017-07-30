#include <QSqlDatabase>
#include <QDomDocument>
#include <QSqlQuery>
#include <QHBoxLayout>
#include <QApplication>
#include <QFileDialog>

#include "Core.h"
#include "IDocument.h"
#include "IPythonModule.h"
#include "ProjectTreeView.h"
#include "AsofProjectPlugin.h"
#include "ProjectData.h"
#include "NewProjectDialog.h"
#include "RecentFilesMenu.h"
#include "ProjectTypeDialog.h"

#include "AsofProjectPythonModule.h"

Python::AsofProjectPythonModule *projectPythonModule;

class AsofProjectRecentFilesDelagate: public IRecentFilesDelegate
{
public:
    AsofProjectRecentFilesDelagate( AsofProjectPlugin *plugin )
        : m_plugin(plugin)
    {}

    virtual void openFile( const QString &fileName ) const { m_plugin->openProjectFilePath(fileName); }
    virtual QString menuName() const { return QObject::tr("Recent Projects"); }
    virtual QString settingName() const  { return "RecentAsofProjects"; }

private:
    AsofProjectPlugin *m_plugin;
};

AsofProjectPlugin::AsofProjectPlugin()
    : IDocumentHelper()
    , ISessionClient()
    , ICloseApplicationClient()
    , m_projectOpened(false)
    , m_importManager(new AsofProjectImportManager)
{
    QSqlDatabase::addDatabase("QSQLITE", "ASPDB");
    createActions();

    BaseSearchModePlugin *baseSearchMode = SearchEngine::getInstance()->getBaseSearchMode();
    if ( baseSearchMode )
        baseSearchMode->registerFindScope(new QAsProjectFilesFindScope(this));

    projectPythonModule = new Python::AsofProjectPythonModule(this);
    Core::Storage::mainWindow()->registerModule(projectPythonModule, true, QStringList() << "ProjectFolder" << "ProjectFile");
}

AsofProjectPlugin::~AsofProjectPlugin()
{
}

void AsofProjectPlugin::destroy()
{
    delete recentProjectMenu;
    delete m_importManager;
}

void AsofProjectPlugin::createActions()
{
    QMainWindow *mainWindow = Core::Storage::mainWindow()->widget();

    m_cmdNewProject     = new Core::Command( tr("New"),
                                             QIcon(":/images/prj_new.png"),
                                             tr("New Project"),
                                             tr("Create New Project"),
                                             QKeySequence(),
                                             mainWindow );

    m_cmdOpenProject    = new Core::Command( tr("Open"),
                                             QIcon(":/images/prj_open.png"),
                                             tr("Open Project"),
                                             tr("Open Existing Project"),
                                             QKeySequence(),
                                             mainWindow );

    m_cmdCloseProject   = new Core::Command( tr("Close"),
                                             QIcon(":/images/prj_close.png"),
                                             tr("Close Project"),
                                             tr("Close Current Project"),
                                             QKeySequence(),
                                             mainWindow );

    m_cmdToggleBuildIssues = new Core::Command( tr("ToggleBuildIssues"),
                                                tr("Toggle Build/Issues Mode"),
                                                tr("Toggle between Build and Issues Modes"),
                                                QKeySequence(),
                                                mainWindow );

    m_cmdCancelBuild    = new Core::Command( tr("CancelBuild"),
                                             QIcon(":/images/build_cancel.png"),
                                             tr("Cancel Build Action"),
                                             tr("Cancel Current Build Action"),
                                             QKeySequence(),
                                             mainWindow );

    m_cmdToggleWarnings = new Core::Command( tr("ToggleShowBuildWarnings"),
                                             QIcon(":/images/build_warnings.png"),
                                             tr("Hide Warnings"),
                                             tr("Hide Warnings"),
                                             QKeySequence(),
                                             mainWindow );

    m_cmdCancelBuild->action()->setEnabled(false);
    m_cmdToggleWarnings->action()->setCheckable(true);

    connect(m_cmdNewProject->action(),      SIGNAL(triggered()), this, SLOT(newProjectDb()));
    connect(m_cmdOpenProject->action(),     SIGNAL(triggered()), this, SLOT(openProjectDb()));
    connect(m_cmdCloseProject->action(),    SIGNAL(triggered()), this, SLOT(closeProjectDb()));

    recentProjectMenu = new RecentFilesMenu( Core::Storage::mainWindow()->orgName(),
                                             Core::Storage::mainWindow()->appName(),
                                             new AsofProjectRecentFilesDelagate(this) );
}

void AsofProjectPlugin::populateMenu( int pluginUiIndex, QMenu *menu )
{
    if ( pluginUiIndex == 0 ) {
        menu->addAction(m_cmdNewProject->action());
        menu->addAction(m_cmdOpenProject->action());
        menu->addSeparator();
        menu->addMenu(recentProjectMenu);
        menu->addSeparator();
        menu->addAction(m_cmdCloseProject->action());
    }
}

bool AsofProjectPlugin::documentEventFilter( IDocument * /*document*/, QEvent *event )
{
    if ( event->type() == QEvent::KeyPress ) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if ( (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
             && QApplication::keyboardModifiers () & Qt::ControlModifier ) {
//            QString tagText = document->currentWord();
//            projectTreeView->searchTagName(tagText);
            return true;
        }
    }
    return false;
}

void AsofProjectPlugin::emitProjectSynhronized( const QString &projectName, const QStringList &includePaths,
                                                 const QStringList &systemIncludePaths, const QStringList &defines,
                                                 const QStringList &sourceFiles, const QString &precompiledHeader )
{
    QVariantList valuesList;
    emit(projectClosed(valuesList));
    valuesList << projectName << includePaths << systemIncludePaths << defines << sourceFiles << precompiledHeader;
    emit(projectOpened(valuesList));
}

QStringList AsofProjectPlugin::projectFiles()
{
    if ( !m_projectOpened )
        return QStringList();

    ProjectModel *model = qobject_cast<ProjectModel *>(projectTreeView->model());
    if ( !model )
        return QStringList();

    return model->projectFiles();
}

ProjectTreeItem *AsofProjectPlugin::projectFolderItem()
{
    if ( !m_projectOpened )
        return 0;

    ProjectModel *model = qobject_cast<ProjectModel *>(projectTreeView->model());
    if ( !model )
        return 0;

    return model->projectFolderItem();
}

ProjectTreeItem *AsofProjectPlugin::defaultSourceFolderItem()
{
    if ( !m_projectOpened )
        return 0;

    ProjectModel *model = qobject_cast<ProjectModel *>(projectTreeView->model());
    if ( !model )
        return 0;

    return model->defaultSourceFolderItem();
}

ProjectTreeItem *AsofProjectPlugin::defaultHeaderFolderItem()
{
    if ( !m_projectOpened )
        return 0;

    ProjectModel *model = qobject_cast<ProjectModel *>(projectTreeView->model());
    if ( !model )
        return 0;

    return model->defaultHeaderFolderItem();

}

void AsofProjectPlugin::newProjectDb()
{
    if ( IAsofProjectImporter *importer = m_importManager->selectImporter() ) {
        QString importFileExtension = importer->sourceFilesExtension();
        QString filePath = "";
        QString importFilePath = "";
        QString projectName = "";
        QString projectDescription = "";
        if ( !importFileExtension.isEmpty() ) {
            importFilePath = QFileDialog::getOpenFileName( Core::Storage::mainWindow()->widget(),
                                                           tr("Open Import File"),
                                                           "",
                                                           tr("%1 (*.%2)").arg(importer->sourceFilesDescription()).arg(importFileExtension) );
            if ( importFilePath.isEmpty() )
                return;
            QFileInfo importFileInfo(importFilePath);
            projectName = importFileInfo.completeBaseName();
            filePath = importFileInfo.canonicalPath() + "/" + projectName + ".aspdb";
            projectDescription = tr("Imported From ") + importFileInfo.fileName();
        }

        QStringList includePaths;
        QStringList systemIncludePaths;
        QStringList defines;
        QString precompiledHeader = "";

        ProjectModel *model = new ProjectModel(this);
        ProjectTreeItem *projectItem = model->initRootItem(projectName, projectDescription);

        if ( !importer->executeImport(importFilePath, filePath, includePaths, defines, precompiledHeader, projectItem) ) {
            delete model;
            return;
        }

        NewEditProjectDialog newProjectDialog( projectName,
                                               projectDescription,
                                               filePath,
                                               includePaths,
                                               systemIncludePaths,
                                               defines,
                                               precompiledHeader,
                                               Core::Storage::mainWindow()->widget() );
        if ( newProjectDialog.exec() != QDialog::Accepted ) {
            delete model;
            return;
        }

        projectName = newProjectDialog.projectName();
        projectDescription = newProjectDialog.projectDescription();
        filePath = newProjectDialog.projectLocation();

        includePaths = newProjectDialog.includePaths();
        systemIncludePaths = newProjectDialog.systemIncludePaths();
        defines = newProjectDialog.defines();

        precompiledHeader = newProjectDialog.precompiledHeader();

        closeProjectDb();

        QStringList sourceFiles;
        if ( !model->createProjectDb( projectName, projectDescription, filePath, importFilePath,
                                      includePaths, systemIncludePaths, defines, precompiledHeader, sourceFiles ) ) {
            Core::Storage::warning(tr("Create Project DB Failed:\n%1").arg(model->lastError()));
            delete model;
            return;
        }

        model->updateProjectItem(projectName, projectDescription);
        projectTreeView->setModel(model);
        projectTreeView->setExpanded(model->index(0, 0), true);

        QVariantList valuesList;
        valuesList << projectName << includePaths << systemIncludePaths << defines << sourceFiles << precompiledHeader;
        emit(projectOpened(valuesList));
        m_buildActionsManager->refreshEnvironment();
        m_projectOpened = true;

        recentProjectMenu->appendItem(filePath);

        m_projectDock->showAndSetCurrentIfTabified();
    }
}

void AsofProjectPlugin::expandProjectFirstLevel()
{
    projectTreeView->setExpanded(projectTreeView->model()->index(0, 0), true);
}

void AsofProjectPlugin::openProjectDb()
{
    openProjectFilePath();
}

void AsofProjectPlugin::openProjectFilePath( QString filePath )
{
    bool appentToRecentProject = false;
    if ( filePath.isEmpty() ) {
        filePath = QFileDialog::getOpenFileName( Core::Storage::mainWindow()->widget(),
                                                 tr("Open Project Database"),
                                                 QString(),
                                                 tr("ASOF Project Database (*.aspdb)") );
        appentToRecentProject = true;
    }

    if ( filePath.isEmpty() )
        return;

    QString projectName;
    ProjectModel *model = new ProjectModel(this);
    QStringList includePaths;
    QStringList systemIncludePaths;
    QStringList defines;
    QStringList sourceFiles;
    QString precompiledHeader;

    closeProjectDb();

    if ( !model->openProjectDb(filePath, projectName, includePaths, systemIncludePaths, defines, sourceFiles, precompiledHeader) ) {
        Core::Storage::warning(tr("Open Project DB Failed:\n%1").arg(model->lastError()));
        delete model;
        return;
    }

    projectTreeView->setModel(model);
    projectTreeView->setExpanded(model->index(0, 0), true);

    QVariantList valuesList;
    valuesList << projectName << includePaths << systemIncludePaths << defines << sourceFiles << precompiledHeader;
    emit(projectOpened(valuesList));
    m_buildActionsManager->refreshEnvironment();
    m_projectOpened = true;

    if ( appentToRecentProject )
        recentProjectMenu->appendItem(filePath);

    m_projectDock->showAndSetCurrentIfTabified();
}

bool AsofProjectPlugin::sessionAboutToClose()
{
    m_buildActionsManager->cancelBuildAction();
    return true;
}

void AsofProjectPlugin::sessionClose()
{
    closeProjectDb();
}

void AsofProjectPlugin::saveSession( const QString &location )
{
    QString fileName = location;
    fileName += "/asproject.xml";

    QFile file(fileName);
    if ( !file.open(QFile::WriteOnly | QFile::Text) ) {
        Core::Storage::warning( tr("Error On Save Session Project \"%1\" Cannot be Opened for Writing:\n%2.")
                                .arg(fileName).arg(file.errorString()) );
        return;
    }

    QDomDocument domDocument("asproject");

    domDocument.appendChild(domDocument.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"windows-1251\""));

    QDomElement root = domDocument.createElement("asproject");
    root.setAttribute("version", "1.0");
    domDocument.appendChild(root);

    QDomElement itemElement = domDocument.createElement("item");
    if ( m_projectOpened ) {
        itemElement.setAttribute("opened", "1");
        ProjectModel *model = qobject_cast<ProjectModel *>(projectTreeView->model());
        itemElement.setAttribute("projectPath", model->projectFilePath());
    }
    else {
        itemElement.setAttribute("opened", "0");
        itemElement.setAttribute("projectPath", "");
    }
    root.appendChild(itemElement);

    const int IndentSize = 4;
    QTextStream out(&file);
    domDocument.save(out, IndentSize);

    file.close();
}

void AsofProjectPlugin::loadSession( const QString &location )
{
    QString fileName = location;
    fileName += "/asproject.xml";

    QFile file(fileName);
    if ( !file.exists() )
        return;

    QDomDocument domDocument;

    QString errorStr;
    int errorLine;
    int errorColumn;

    // Загружаем всю структуру
    if (!domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn)) {
        Core::Storage::warning( tr("Error On Load Session Documents\nCommon Error in xml-file:\nLine %1, Column %2: %3")
                                .arg(errorLine).arg(errorColumn).arg(errorStr) );
        return;
    }

    // Первичная проверка (тип и версия документа)
    QDomElement root = domDocument.documentElement();
    if ( root.tagName() != "asproject" ) {
        Core::Storage::warning( tr("Error On Load Session Project\nInvalid Format of Sessions xml-file") );
        return;
    }
    if ( !root.hasAttribute("version") ) {
        Core::Storage::warning( tr("Error On Load Session Project\nInvalid Format of Sessions xml-file (version expected)") );
        return;
    }
    if ( root.attribute("version") != "1.0" ) {
        Core::Storage::warning( tr("Error On Load Session Project\nInvalid Format of Sessions xml-file (wrong version)") );
        return;
    }

    QDomElement itemElement = root.firstChildElement();
    if ( !itemElement.isNull() ) {
        if ( itemElement.tagName() != "item" ) {
            Core::Storage::warning( tr("Error On Load Session Documents\nInvalid Format of Sessions xml-file (unknown tag <%1> at line %2)")
                                    .arg(itemElement.tagName()).arg(itemElement.lineNumber()) );
            return;
        }
        QString fileName = itemElement.attribute("projectPath");
        bool isOpened = itemElement.attribute("opened").toInt();
        if ( isOpened )
            openProjectFilePath(fileName);
    }

    file.close();
}

bool AsofProjectPlugin::applicationAboutToClose()
{
    m_buildActionsManager->cancelBuildAction();
    return true;
}

void AsofProjectPlugin::closeProjectDb()
{
    ProjectModel *model = qobject_cast<ProjectModel *>(projectTreeView->model());
    if ( model ) {
        model->closeProjectDb();
        ProjectModel *newModel = new ProjectModel(this);
        newModel->initEmpty();
        projectTreeView->setModel(newModel);
        projectTreeView->setExpanded(newModel->index(0, 0), true);
        delete model;

        QVariantList valuesList;
        emit(projectClosed(valuesList));
        m_buildActionsManager->clearEnvironment();
        m_projectOpened = false;
    }
}

void AsofProjectPlugin::onDocumentSaved( const QString &/*filePath*/ )
{
}

void AsofProjectPlugin::onEditProjectProperties()
{
    ProjectModel *model = qobject_cast<ProjectModel *>(projectTreeView->model());

    QString projectName;
    QString projectDescription;
    QString projectLocation;
    QStringList includes;
    QStringList systemIncludes;
    QStringList defines;
    QStringList sources;
    QString precompiledHeader;

    model->getProjectProperties(projectName, projectDescription, projectLocation, includes, systemIncludes, defines, precompiledHeader);

    NewEditProjectDialog editProjectDialog( projectName, projectDescription, projectLocation,
                                            includes, systemIncludes, defines, precompiledHeader,
                                            Core::Storage::mainWindow()->widget() );

    if ( editProjectDialog.exec() == QDialog::Accepted ) {
        projectName             = editProjectDialog.projectName();
        projectDescription      = editProjectDialog.projectDescription();
        includes                = editProjectDialog.includePaths();
        systemIncludes          = editProjectDialog.systemIncludePaths();
        defines                 = editProjectDialog.defines();
        precompiledHeader       = editProjectDialog.precompiledHeader();
        if ( model->changeProjectSettings(projectName, projectDescription, includes, systemIncludes, defines, sources, precompiledHeader) ) {
            QVariantList valuesList;
            emit(projectClosed(valuesList));
            valuesList << projectName << includes << systemIncludes << defines << sources << precompiledHeader;
            emit(projectOpened(valuesList));
        }
    }
}

void AsofProjectPlugin::onBuildActionStarted()
{
    m_cmdNewProject->action()->setEnabled(false);
    m_cmdOpenProject->action()->setEnabled(false);
    m_cmdCloseProject->action()->setEnabled(false);
    m_cmdCancelBuild->action()->setEnabled(true);
    recentProjectMenu->setEnabledAllItems(false);
}

void AsofProjectPlugin::onBuildActionFifnished()
{
    m_cmdNewProject->action()->setEnabled(true);
    m_cmdOpenProject->action()->setEnabled(true);
    m_cmdCloseProject->action()->setEnabled(true);
    m_cmdCancelBuild->action()->setEnabled(false);
    recentProjectMenu->setEnabledAllItems(true);
}

void AsofProjectPlugin::emitProjectChanged( QVariantList &valuesList )
{
    emit(projectChanged(valuesList));
}

void AsofProjectPlugin::getProjectFiles( QStringList &fileNames ) const
{
    ProjectModel *model = qobject_cast<ProjectModel *>(projectTreeView->model());
    if ( !model )
        return;

    model->getProjectFiles(fileNames);
}

void AsofProjectPlugin::initialize()
{
    m_importManager->loadImporterPlugins();
    Core::CommandManager::getInstance()->registerCommands( tr("AsofProjectCommands"),
                                                            QList<Core::Command *>() << m_cmdNewProject
                                                                                     << m_cmdOpenProject
                                                                                     << m_cmdCloseProject
                                                                                     << m_cmdToggleBuildIssues
                                                                                     << m_cmdCancelBuild
                                                                                     << m_cmdToggleWarnings );
}

QList<UiProviderInterface *> AsofProjectPlugin::createUiProviders( IMainWindow *mainWindow )
{
    m_projectDock = new QwCustomDockWidget( mainWindow->widget(),
                                            tr("Asof Project"),
                                            Qt::LeftDockWidgetArea, QwCustomDockWidget::First,
                                            QList<QAction *>() << m_cmdNewProject->action()
                                                               << m_cmdOpenProject->action()
                                                               << m_cmdCloseProject->action() );
    m_projectDock->toggleViewAction()->setIcon(QIcon(":/images/prj_browser.png"));
    projectTreeView = new ProjectTreeView(mainWindow->widget());
    projectTreeView->setHeaderHidden(true);
    projectTreeView->setRootIsDecorated(true);
    m_projectDock->setWidget(projectTreeView);

    connect(DocumentManager::getInstance(), SIGNAL(documentSaved(QString)), this, SLOT(onDocumentSaved(QString)));
    connect(projectTreeView, SIGNAL(editProjectProperties()), this, SLOT(onEditProjectProperties()));

    QMenu *contextMenu = new QMenu;
    contextMenu->addAction(m_cmdNewProject->action());
    contextMenu->addAction(m_cmdOpenProject->action());
    contextMenu->addSeparator();
    contextMenu->addMenu(recentProjectMenu);
    contextMenu->addSeparator();
    contextMenu->addAction(m_cmdCloseProject->action());
    m_projectDock->setContextMenu(contextMenu);

    m_buildTabBar = new QTabBar;
    m_buildTabBar->setDrawBase(false);
    m_buildTabBar->addTab(QIcon(":/images/build_console.png"), tr("Console"));
    m_buildTabBar->addTab(QIcon(":/images/build_issues.png"), tr("Issues"));
    QwCustomDockWidget *buildDock = new QwCustomDockWidget( mainWindow->widget(),
                                                            tr("Project Build"),
                                                            Qt::BottomDockWidgetArea, QwCustomDockWidget::First,
                                                            QList<QAction *>() << m_cmdCancelBuild->action() << m_cmdToggleWarnings->action(),
                                                            m_buildTabBar );

    m_buildActionsManager = new ProjectBuildActionsManager(m_buildTabBar, mainWindow->widget());
    buildDock->setWidget(m_buildActionsManager);
    buildDock->toggleViewAction()->setIcon(QIcon(":/images/prj_run.png"));

    connect(m_buildTabBar, SIGNAL(currentChanged(int)), m_buildActionsManager, SLOT(setCurrentIndex(int)));
    m_buildTabBar->setCurrentIndex(0);

    connect(m_buildActionsManager, SIGNAL(buildActionStarted()), this, SLOT(onBuildActionStarted()));
    connect(m_buildActionsManager, SIGNAL(buildActionFinished()), this, SLOT(onBuildActionFifnished()));
    connect(m_buildActionsManager, SIGNAL(buildActionStarted()), projectTreeView, SLOT(onBuildActionStarted()));
    connect(m_buildActionsManager, SIGNAL(buildActionFinished()), projectTreeView, SLOT(onBuildActionFinished()));

    connect(m_cmdToggleBuildIssues->action(), SIGNAL(triggered()), m_buildActionsManager, SLOT(toggleMode()));
    connect(m_buildActionsManager, SIGNAL(buildActionStarted()), buildDock, SLOT(showAndSetCurrentIfTabified()));
    connect(m_buildActionsManager, SIGNAL(buildActionStartFailed()), buildDock, SLOT(showAndSetCurrentIfTabified()));
    connect(m_cmdCancelBuild->action(), SIGNAL(triggered()), m_buildActionsManager, SLOT(cancelBuildAction()));
    connect(m_cmdToggleWarnings->action(), SIGNAL(toggled(bool)), m_buildActionsManager, SLOT(toggleHideWarnings(bool)));

    QMenu *buildMenu = new QMenu;
    buildMenu->addAction(m_cmdToggleBuildIssues->action());
    buildMenu->addAction(m_cmdToggleWarnings->action());
    buildMenu->addSeparator();
    buildMenu->addAction(m_cmdCancelBuild->action());
    buildDock->setContextMenu(buildMenu);

    QList<UiProviderInterface *> providers;
    providers << new AsofProjectUiProvider(this, m_projectDock)
              << new AsofProjectBuildActionsUiProvider(this, buildDock);

    return providers;
}

Utils::FileIterator * QAsProjectFilesFindScope::files()
{
    QStringList fileNames;
    m_projectPlugin->getProjectFiles(fileNames);

    return new Utils::FileIterator( fileNames, QList<QTextCodec *>() << QTextCodec::codecForName("Windows-1251") );
}

#ifndef HAVE_QT5
Q_EXPORT_PLUGIN2(AsofProjectPlugin, AsofProjectPlugin)
#endif

