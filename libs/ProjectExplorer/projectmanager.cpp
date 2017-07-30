#include "projectmanager.h"
#include "nodesvisitor.h"
#include "Core.h"
#include "RecentFilesMenu.h"
#include "LogManager.h"
#include "mimedatabase.h"
#include "toolchainoptionspage.h"
#include "toolchainmanager.h"
#include "settingsdialog.h"
#include "msvctoolchain.h"
#include "wincetoolchain.h"
#include "gcctoolchainfactories.h"
#include "customtoolchain.h"
#include "kitoptionspage.h"
#include "projectexplorersettingspage.h"
#include "kitinformation.h"
#include "devicesettingspage.h"
#include "idevice.h"
#include "desktopdevicefactory.h"
#include "desktopdevice.h"
#include "qtoptionspage.h"
#include "qtkitinformation.h"
#include "qmakeparser.h"
#include "profileevaluator.h"
#include "profilereader.h"
#include "desktopqtversionfactory.h"
#include "simulatorqtversionfactory.h"
#include "winceqtversionfactory.h"
#include "SelectProjectsDialog.h"
#include "buildmanager.h"
#include "appoutputpane.h"
#include "taskhub.h"
#include "target.h"
#include "buildsteplist.h"
#include "localapplicationruncontrol.h"
#include "showineditortaskhandler.h"
#include "projectwindow.h"
#include "projectpanelfactory.h"
#include "dependenciespanel.h"
#include "unconfiguredprojectpanel.h"
#include "targetsettingspanel.h"
#include "processstep.h"
#include "ConfigureProjectsDialog.h"
#include "uicodemodelsupport.h"
#include "sshconnection.h"
#include "selectablefilesmodel.h"
#include "removefiledialog.h"
#include "projecttreewidget.h"
#include "kitfeatureprovider.h"
#include "jsexpander.h"
#include "cpptoolsjsextension.h"
#include "codegensettingspage.h"
#include "codegenerator.h"
#include "cppfilesettingspage.h"
#include "iwizardfactory.h"
#include "projectfilewizardextension.h"
#include "IDocument.h"
#include "miniprojecttargetselector.h"

#include "jsonwizard/jsonwizardfactory.h"
#include "jsonwizard/jsonwizardgeneratorfactory.h"
#include "jsonwizard/jsonwizardpagefactory_p.h"

#include <algorithm.h>
#include <stringutils.h>
#include <parameteraction.h>

#include <QFileDialog>
#include <QFutureInterface>

namespace ProjectExplorer {

class ProjectRecentFilesDelagate: public IRecentFilesDelegate
{
public:
    ProjectRecentFilesDelagate( ProjectManager *projectManager )
        : m_projectManager(projectManager)
    {}

    virtual void openFile( const QString &fileName ) const { m_projectManager->openProjectFilePath(fileName); }
    virtual QString menuName() const { return QObject::tr("Recent Projects"); }
    virtual QString settingName() const  { return QStringLiteral("RecentProjects"); }

private:
    ProjectManager *m_projectManager;
};

class ProjectExplorerPluginPrivate : public QObject
{
public:
    ProjectExplorerPluginPrivate();

    void deploy(QList<ProjectExplorer::Project *>);
    int queue(QList<ProjectExplorer::Project *>, QList<Core::Id> stepIds);
    void updateContextMenuActions();
    void executeRunConfiguration(ProjectExplorer::RunConfiguration *runConfiguration, ProjectExplorer::RunMode runMode);
    QPair<bool, QString> buildSettingsEnabledForSession();
    QPair<bool, QString> buildSettingsEnabled(ProjectExplorer::Project *pro);

    void setCurrent(ProjectExplorer::Project *project, QString filePath, ProjectExplorer::Node *node);
    void startRunControl(ProjectExplorer::RunControl *runControl, ProjectExplorer::RunMode runMode);

    void updateActions();
    void updateContext();
    void updateDeployActions();
    void updateRunWithoutDeployMenu();

    QMenu *m_sessionContextMenu;
    QMenu *m_sessionMenu;
    QMenu *m_projectMenu;
    QMenu *m_subProjectMenu;
    QMenu *m_folderMenu;
    QMenu *m_fileMenu;
    QMenu *m_openWithMenu;
    QMenu *m_runContextMenu;

    QAction *m_buildSystemAction;
    QAction *m_buildSystemContextMenuAction;
    QMultiMap<int, QObject*> m_actionMap;
    QAction *m_sessionManagerAction;
    QAction *m_newAction;
    QAction *m_loadAction;
    Utils::ParameterAction *m_unloadAction;
    Utils::ParameterAction *m_unloadActionContextMenu;
    QAction *m_closeAllProjects;
    QAction *m_buildProjectOnlyAction;
    Utils::ParameterAction *m_buildAction;
    QAction *m_buildActionContextMenu;
    QAction *m_buildSessionAction;
    QAction *m_rebuildProjectOnlyAction;
    Utils::ParameterAction *m_rebuildAction;
    QAction *m_rebuildActionContextMenu;
    QAction *m_rebuildSessionAction;
    QAction *m_cleanProjectOnlyAction;
    QAction *m_deployProjectOnlyAction;
    Utils::ParameterAction *m_deployAction;
    QAction *m_deployActionContextMenu;
    QAction *m_deploySessionAction;
    Utils::ParameterAction *m_cleanAction;
    QAction *m_cleanActionContextMenu;
    QAction *m_cleanSessionAction;
    QAction *m_runAction;
    QAction *m_runActionContextMenu;
    QAction *m_runWithoutDeployAction;
    QAction *m_cancelBuildAction;
    QAction *m_addNewFileAction;
    QAction *m_addExistingFilesAction;
    QAction *m_addExistingDirectoryAction;
    QAction *m_addNewSubprojectAction;
    QAction *m_removeFileAction;
    QAction *m_removeProjectAction;
    QAction *m_deleteFileAction;
    QAction *m_renameFileAction;
    QAction *m_openFileAction;
    QAction *m_projectTreeCollapseAllAction;
    QAction *m_searchOnFileSystem;
    QAction *m_showInGraphicalShell;
    QAction *m_openTerminalHere;
    Utils::ParameterAction *m_setStartupProjectAction;
    QAction *m_projectSelectorAction;
    QAction *m_projectSelectorActionMenu;
    QAction *m_projectSelectorActionQuick;
    QAction *m_runSubProject;

    ProjectExplorer::Internal::ProjectWindow *m_proWindow;
    ConfigureProjectsDialog *m_configureProjectsFialog;
    QString m_sessionToRestoreAtStartup;

    ProjectExplorer::Project *m_currentProject;
//    Context m_lastProjectContext;
    ProjectExplorer::Node *m_currentNode;

    QStringList m_profileMimeTypes;
    ProjectExplorer::Internal::AppOutputPane *m_outputPane;

    QList<QPair<QString, QString> > m_recentProjects; // pair of filename, displayname
    static const int m_maxRecentProjects = 25;

    QString m_lastOpenDirectory;
    QPointer<ProjectExplorer::RunConfiguration> m_delayedRunConfiguration;
    QList<QPair<ProjectExplorer::RunConfiguration *, ProjectExplorer::RunMode>> m_delayedRunConfigurationForRun;
    bool m_shouldHaveRunConfiguration;
    ProjectExplorer::RunMode m_runMode;
    QString m_projectFilterString;
    ProjectExplorer::Internal::MiniProjectTargetSelector * m_targetSelector;
    ProjectExplorer::Internal::ProjectExplorerSettings m_projectExplorerSettings;

    ProjectExplorer::TaskHub *m_taskHub;
    ProjectExplorer::KitManager *m_kitManager;
    ProjectExplorer::ToolChainManager *m_toolChainManager;
    bool m_shuttingDown;
    bool m_ignoreDocumentManagerChangedFile;
    QStringList m_arguments;
//    QList<ProjectPanelFactory *> m_panelFactories;
};

ProjectManager *ProjectManager::m_instance = 0;
static ProjectExplorerPluginPrivate *dd = 0;

static ProjectExplorer::Target *activeTarget()
{
    ProjectExplorer::Project *project = ProjectManager::currentProject();
    return project ? project->activeTarget() : 0;
}

static ProjectExplorer::BuildConfiguration *activeBuildConfiguration()
{
    ProjectExplorer::Target *target = activeTarget();
    return target ? target->activeBuildConfiguration() : 0;
}

static ProjectExplorer::Kit *currentKit()
{
    ProjectExplorer::Target *target = activeTarget();
    return target ? target->kit() : 0;
}

ProjectExplorerPluginPrivate::ProjectExplorerPluginPrivate()
    : m_currentProject(0)
    , m_currentNode(0)
    , m_shouldHaveRunConfiguration(false)
    , m_runMode(ProjectExplorer::NoRunMode)
    , m_kitManager(0)
    , m_toolChainManager(0)
    , m_shuttingDown(false)
    , m_ignoreDocumentManagerChangedFile(false)
{ }

void ProjectExplorerPluginPrivate::updateActions()
{
    ProjectExplorer::Project *project = ProjectManager::startupProject();
    QString projectName = project ? project->displayName() : QString();
    QString projectNameContextMenu = m_currentProject ? m_currentProject->displayName() : QString();
    ProjectExplorer::IProjectProvider *provider = project ? project->projectProvider() : 0;
    ProjectExplorer::IProjectProvider *contextProvider = m_currentProject ? m_currentProject->projectProvider() : 0;

    QPair<bool, QString> buildSessionState = buildSettingsEnabledForSession();
    QPair<bool, QString> buildActionState = buildSettingsEnabled(project);

    if ( !provider ) {
        m_buildSystemAction->setVisible(false);
    } else {
        m_buildSystemAction->setVisible(true);
        m_buildSystemAction->setEnabled(buildActionState.first);
        m_buildSystemAction->setText(provider->buildSystemActionTitle());
    }

    if ( !contextProvider ) {
        m_buildSystemContextMenuAction->setVisible(false);
    } else {
        m_buildSystemContextMenuAction->setVisible(true);
        m_buildSystemContextMenuAction->setEnabled(buildActionState.first);
        m_buildSystemContextMenuAction->setText(contextProvider->buildSystemActionTitle());
    }

    // Normal actions
    m_buildAction->setParameter(projectName);
    m_rebuildAction->setParameter(projectName);
    m_cleanAction->setParameter(projectName);
    m_setStartupProjectAction->setParameter(projectNameContextMenu);

    m_buildAction->setEnabled(buildActionState.first);
    m_rebuildAction->setEnabled(buildActionState.first);
    m_cleanAction->setEnabled(buildActionState.first);

    m_buildAction->setToolTip(buildActionState.second);
    m_rebuildAction->setToolTip(buildActionState.second);
    m_cleanAction->setToolTip(buildActionState.second);

    // Session actions
    m_buildSessionAction->setEnabled(buildSessionState.first);
    m_rebuildSessionAction->setEnabled(buildSessionState.first);
    m_cleanSessionAction->setEnabled(buildSessionState.first);

    m_buildSessionAction->setToolTip(buildSessionState.second);
    m_rebuildSessionAction->setToolTip(buildSessionState.second);
    m_cleanSessionAction->setToolTip(buildSessionState.second);

    m_cancelBuildAction->setEnabled(ProjectExplorer::BuildManager::isBuilding());

    m_projectSelectorAction->setEnabled(ProjectManager::projects().size() > 0);
}

int ProjectExplorerPluginPrivate::queue(QList<ProjectExplorer::Project *> projects, QList<Core::Id> stepIds)
{
    if (!ProjectManager::instance()->saveModifiedFiles())
        return -1;

    QList<ProjectExplorer::BuildStepList *> stepLists;
    QStringList names;
    QStringList preambleMessage;

    foreach (ProjectExplorer::Project *pro, projects)
        if (pro && pro->needsConfiguration())
            preambleMessage.append(tr("The project %1 is not configured, skipping it.")
                                   .arg(pro->displayName()) + QLatin1Char('\n'));
    foreach (Core::Id id, stepIds) {
        foreach (ProjectExplorer::Project *pro, projects) {
            if (!pro || pro->needsConfiguration())
                continue;
            ProjectExplorer::BuildStepList *bsl = 0;
            if (id == ProjectExplorer::Constants::BUILDSTEPS_DEPLOY
                && pro->activeTarget()->activeDeployConfiguration())
                bsl = pro->activeTarget()->activeDeployConfiguration()->stepList();
            else if (pro->activeTarget()->activeBuildConfiguration())
                bsl = pro->activeTarget()->activeBuildConfiguration()->stepList(id);

            if (!bsl || bsl->isEmpty())
                continue;
            stepLists << bsl;
            names << ProjectManager::displayNameForStepId(id);
        }
    }

    if (stepLists.isEmpty())
        return 0;

    if (!ProjectExplorer::BuildManager::buildLists(stepLists, names, preambleMessage))
        return -1;
    return stepLists.count();
}

void ProjectExplorerPluginPrivate::updateContextMenuActions()
{
    m_addExistingFilesAction->setEnabled(false);
    m_addExistingDirectoryAction->setEnabled(false);
    m_addNewFileAction->setEnabled(false);
    m_addNewSubprojectAction->setEnabled(false);
    m_removeProjectAction->setEnabled(false);
    m_removeFileAction->setEnabled(false);
    m_deleteFileAction->setEnabled(false);
    m_renameFileAction->setEnabled(false);

    m_addExistingFilesAction->setVisible(true);
    m_addExistingDirectoryAction->setVisible(true);
    m_addNewFileAction->setVisible(true);
    m_addNewSubprojectAction->setVisible(true);
    m_removeProjectAction->setVisible(true);
    m_removeFileAction->setVisible(true);
    m_deleteFileAction->setVisible(true);
    m_runActionContextMenu->setVisible(false);

    m_openTerminalHere->setVisible(true);
    m_showInGraphicalShell->setVisible(true);
    m_searchOnFileSystem->setVisible(true);

    m_runContextMenu->clear();
    m_runContextMenu->menuAction()->setVisible(false);

    if (dd->m_currentNode && dd->m_currentNode->projectNode()) {
        QList<ProjectExplorer::ProjectAction> actions = dd->m_currentNode->supportedActions(dd->m_currentNode);

        if (ProjectExplorer::ProjectNode *pn = qobject_cast<ProjectExplorer::ProjectNode *>(dd->m_currentNode)) {
            if (pn == dd->m_currentProject->rootProjectNode()) {
                dd->m_runActionContextMenu->setVisible(true);
            } else {
                QList<ProjectExplorer::RunConfiguration *> runConfigs = pn->runConfigurations();
                if (runConfigs.count() == 1) {
                    dd->m_runActionContextMenu->setVisible(true);
                    dd->m_runActionContextMenu->setData(QVariant::fromValue(runConfigs.first()));
                } else if (runConfigs.count() > 1) {
                    m_runContextMenu->menuAction()->setVisible(true);
                    foreach (ProjectExplorer::RunConfiguration *rc, runConfigs) {
                        QAction *act = new QAction(dd->m_runContextMenu);
                        act->setData(QVariant::fromValue(rc));
                        act->setText(tr("Run %1").arg(rc->displayName()));
                        dd->m_runContextMenu->addAction(act);
                        connect(act, SIGNAL(triggered()),
                                ProjectManager::instance(), SLOT(runProjectContextMenu()));
                    }
                }
            }
        }
        if (qobject_cast<ProjectExplorer::FolderNode*>(dd->m_currentNode)) {
            // Also handles ProjectNode
            dd->m_addNewFileAction->setEnabled(actions.contains(ProjectExplorer::AddNewFile));
            dd->m_addNewSubprojectAction->setEnabled(dd->m_currentNode->nodeType() == ProjectExplorer::ProjectNodeType
                                                    && actions.contains(ProjectExplorer::AddSubProject));
            dd->m_removeProjectAction->setEnabled(dd->m_currentNode->nodeType() == ProjectExplorer::ProjectNodeType
                                                    && actions.contains(ProjectExplorer::RemoveSubProject));
            dd->m_addExistingFilesAction->setEnabled(actions.contains(ProjectExplorer::AddExistingFile));
            dd->m_addExistingDirectoryAction->setEnabled(actions.contains(ProjectExplorer::AddExistingDirectory));
            dd->m_renameFileAction->setEnabled(actions.contains(ProjectExplorer::Rename));
        } else if (qobject_cast<ProjectExplorer::FileNode*>(dd->m_currentNode)) {
            // Enable and show remove / delete in magic ways:
            // If both are disabled show Remove
            // If both are enabled show both (can't happen atm)
            // If only removeFile is enabled only show it
            // If only deleteFile is enable only show it
            bool enableRemove = actions.contains(ProjectExplorer::RemoveFile);
            dd->m_removeFileAction->setEnabled(enableRemove);
            bool enableDelete = actions.contains(ProjectExplorer::EraseFile);
            dd->m_deleteFileAction->setEnabled(enableDelete);
            dd->m_deleteFileAction->setVisible(enableDelete);

            dd->m_removeFileAction->setVisible(!enableDelete || enableRemove);
            dd->m_renameFileAction->setEnabled(actions.contains(ProjectExplorer::Rename));
        }

        if (actions.contains(ProjectExplorer::HidePathActions)) {
            dd->m_openTerminalHere->setVisible(false);
            dd->m_showInGraphicalShell->setVisible(false);
            dd->m_searchOnFileSystem->setVisible(false);
        }

        if (actions.contains(ProjectExplorer::HideFileActions)) {
            dd->m_deleteFileAction->setVisible(false);
            dd->m_removeFileAction->setVisible(false);
        }

        if (actions.contains(ProjectExplorer::HideFolderActions)) {
            dd->m_addNewFileAction->setVisible(false);
            dd->m_addNewSubprojectAction->setVisible(false);
            dd->m_removeProjectAction->setVisible(false);
            dd->m_addExistingFilesAction->setVisible(false);
            dd->m_addExistingDirectoryAction->setVisible(false);
        }
    }
}

static ProjectExplorer::IRunControlFactory *findRunControlFactory(ProjectExplorer::RunConfiguration *config, ProjectExplorer::RunMode mode)
{
    foreach ( ProjectExplorer::IRunControlFactory *factory, ProjectManager::runControlFactories() )
        if ( factory->canRun(config, mode) )
            return factory;

    return 0;
}

void ProjectExplorerPluginPrivate::executeRunConfiguration(ProjectExplorer::RunConfiguration *runConfiguration, ProjectExplorer::RunMode runMode)
{
    ProjectManager *instance = ProjectManager::instance();

    if (!runConfiguration->isConfigured()) {
        QString errorMessage;
        ProjectExplorer::RunConfiguration::ConfigurationState state = runConfiguration->ensureConfigured(&errorMessage);

        if (state == ProjectExplorer::RunConfiguration::UnConfigured) {
            instance->showRunErrorMessage(errorMessage);
            return;
        } else if (state == ProjectExplorer::RunConfiguration::Waiting) {
            QObject::connect(runConfiguration, SIGNAL(configurationFinished()),
                    instance, SLOT(runConfigurationConfigurationFinished()));
            m_delayedRunConfigurationForRun.append(qMakePair(runConfiguration, runMode));
            return;
        }
    }

    if (ProjectExplorer::IRunControlFactory *runControlFactory = findRunControlFactory(runConfiguration, runMode)) {
        emit instance->aboutToExecuteProject(runConfiguration->target()->project(), runMode);

        QString errorMessage;
        ProjectExplorer::RunControl *control = runControlFactory->create(runConfiguration, runMode, &errorMessage);
        if (!control) {
            instance->showRunErrorMessage(errorMessage);
            return;
        }
        startRunControl(control, runMode);
    }
}

static bool hasBuildSettings(ProjectExplorer::Project *pro)
{
    return Utils::anyOf(ProjectManager::instance()->projectOrder(pro), [](ProjectExplorer::Project *project) {
        return project
                && project->activeTarget()
                && project->activeTarget()->activeBuildConfiguration();
    });
}

QPair<bool, QString> ProjectExplorerPluginPrivate::buildSettingsEnabledForSession()
{
    QPair<bool, QString> result;
    result.first = true;
    if (ProjectManager::projects().isEmpty()) {
        result.first = false;
        result.second = tr("No project loaded");
    } else if (ProjectExplorer::BuildManager::isBuilding()) {
        result.first = false;
        result.second = tr("A build is in progress");
    } else if (!hasBuildSettings(0)) {
        result.first = false;
        result.second = tr("Project has no build settings");
    } else {
        foreach (ProjectExplorer::Project *project, ProjectManager::instance()->projectOrder()) {
            if (project
                    && project->activeTarget()
                    && project->activeTarget()->activeBuildConfiguration()
                    && !project->activeTarget()->activeBuildConfiguration()->isEnabled()) {
                result.first = false;
                result.second += tr("Building \"%1\" is disabled: %2")
                        .arg(project->displayName(),
                             project->activeTarget()->activeBuildConfiguration()->disabledReason());
                result.second += QLatin1Char('\n');
            }
        }
    }
    return result;
}

QPair<bool, QString> ProjectExplorerPluginPrivate::buildSettingsEnabled(ProjectExplorer::Project *pro)
{
    QPair<bool, QString> result;
    result.first = true;
    if (!pro) {
        result.first = false;
        result.second = tr("No project loaded.");
    } else if (ProjectExplorer::BuildManager::isBuilding(pro)) {
        result.first = false;
        result.second = tr("Currently building the active project.");
    } else if (pro->needsConfiguration()) {
        result.first = false;
        result.second = tr("The project %1 is not configured.").arg(pro->displayName());
    } else if (!hasBuildSettings(pro)) {
        result.first = false;
        result.second = tr("Project has no build settings.");
    } else {
        const QList<ProjectExplorer::Project *> & projects = ProjectManager::instance()->projectOrder(pro);
        foreach (ProjectExplorer::Project *project, projects) {
            if (project
                    && project->activeTarget()
                    && project->activeTarget()->activeBuildConfiguration()
                    && !project->activeTarget()->activeBuildConfiguration()->isEnabled()) {
                result.first = false;
                result.second += tr("Building \"%1\" is disabled: %2<br>")
                        .arg(project->displayName(),
                             project->activeTarget()->activeBuildConfiguration()->disabledReason());
            }
        }
    }
    return result;
}

void ProjectExplorerPluginPrivate::startRunControl(ProjectExplorer::RunControl *runControl, ProjectExplorer::RunMode runMode)
{
    ProjectManager *instance = ProjectManager::instance();

    m_outputPane->createNewOutputWindow(runControl);
    m_outputPane->flash(); // one flash for starting
    m_outputPane->showTabFor(runControl);
    bool popup = (runMode == ProjectExplorer::NormalRunMode && dd->m_projectExplorerSettings.showRunOutput)
            || ((runMode == ProjectExplorer::DebugRunMode || runMode == ProjectExplorer::DebugRunModeWithBreakOnMain)
                && m_projectExplorerSettings.showDebugOutput);
    m_outputPane->setBehaviorOnOutput(runControl, popup ?
                                                  ProjectExplorer::Internal::AppOutputPane::Popup :
                                                  ProjectExplorer::Internal::AppOutputPane::Flash);
    QObject::connect(runControl, SIGNAL(finished()), instance, SIGNAL(updateRunActions()));
    runControl->start();
    emit instance->updateRunActions();
}

ProjectManager::ProjectManager()
    : ISessionClient()
    , m_sessionNode(new ProjectExplorer::SessionNode(0))
    , m_startupProject(0)
    , m_taskHandler(new ProjectExplorer::Internal::ShowInEditorTaskHandler)
    , m_kitOptionsPage(0)
    , m_fileSettings(new CppTools::Internal::CppFileSettings)
    , m_appOutputDock(0)
    , m_issuesPane(0)
{
    m_instance = this;
    dd = new ProjectExplorerPluginPrivate;

    m_jsExpander = new Core::JsExpander();
    Core::JsExpander::registerQObjectForJs(QLatin1String("Cpp"), new CppTools::Internal::CppToolsJsExtension);
    Core::JsExpander::registerQObjectForJs(QLatin1String("QtSupport"), new QtSupport::CodeGenerator);

    createActions();

    IMainWindow *mainWindow = Core::Storage::mainWindow();

    m_settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                               mainWindow->orgName(),
                               mainWindow->appName(),
                               this);

    restorePersistentSettings();
    loadProjectProvidersPlugins();
}

ProjectManager::~ProjectManager()
{
    delete m_jsExpander;
    delete dd->m_targetSelector;

    ProjectExplorer::JsonWizardFactory::destroyAllFactories();

    dd->m_toolChainManager->saveToolChains();
    dd->m_kitManager->saveKits();

    delete m_sessionNode;
    delete m_recentProjectMenu;

    qDeleteAll(m_projectProviders);
    qDeleteAll(m_projects);
    qDeleteAll(m_buildStepFactories);
    qDeleteAll(m_buildConfigurationFactories);
    qDeleteAll(m_deviceFactories);
    qDeleteAll(m_deployConfigurationFactories);
    qDeleteAll(m_runControlFactories);
    qDeleteAll(m_runConfigurationFactories);
    qDeleteAll(m_toolChainFactories);
    qDeleteAll(m_optionPages);

    delete dd->m_taskHub;
    delete m_taskHandler;
#ifdef Q_OS_WIN
    delete m_winDebugInterface;
#endif
    delete dd->m_toolChainManager;
    delete dd->m_kitManager;
    delete m_deviceManager;
    delete m_qtVersionManager;
    delete m_uiCodeModelManager;

    delete dd->m_outputPane;

    delete dd;
}

static QString pathOrDirectoryFor(ProjectExplorer::Node *node, bool dir)
{
    QString path = node->path();
    QString location;
    ProjectExplorer::FolderNode *folder = qobject_cast<ProjectExplorer::FolderNode *>(node);
    if (node->nodeType() == ProjectExplorer::VirtualFolderNodeType && folder) {
        // Virtual Folder case
        // If there are files directly below or no subfolders, take the folder path
        if (!folder->fileNodes().isEmpty() || folder->subFolderNodes().isEmpty()) {
            location = path;
        } else {
            // Otherwise we figure out a commonPath from the subfolders
            QStringList list;
            foreach (ProjectExplorer::FolderNode *f, folder->subFolderNodes())
                list << f->path() + QLatin1Char('/');
            location = Utils::commonPath(list);
        }

        QFileInfo fi(location);
        while ((!fi.exists() || !fi.isDir())
               && !fi.isRoot())
            fi.setFile(fi.absolutePath());
        location = fi.absoluteFilePath();
    } else {
        QFileInfo fi(path);
        // remove any /suffixes, which e.g. ResourceNode uses
        // Note this should be removed again by making node->path() a true path again
        // That requires changes in both the VirtualFolderNode and ResourceNode
        while (!fi.exists() && !fi.isRoot())
            fi.setFile(fi.absolutePath());

        if (dir)
            location = fi.isDir() ? fi.absoluteFilePath() : fi.absolutePath();
        else
            location = fi.absoluteFilePath();
    }
    return location;
}

static QString pathFor(ProjectExplorer::Node *node)
{
    return pathOrDirectoryFor(node, false);
}

static QString directoryFor(ProjectExplorer::Node *node)
{
    return pathOrDirectoryFor(node, true);
}

ProjectManager *ProjectManager::instance()
{ return m_instance; }

void ProjectManager::unloadProject(ProjectExplorer::Project *project)
{
    if (ProjectExplorer::BuildManager::isBuilding(project)) {
        QMessageBox box;
        QPushButton *closeAnyway = box.addButton(tr("Cancel Build && Unload"), QMessageBox::AcceptRole);
        QPushButton *cancelClose = box.addButton(tr("Do Not Unload"), QMessageBox::RejectRole);
        box.setDefaultButton(cancelClose);
        box.setWindowTitle(tr("Unload Project %1?").arg(project->displayName()));
        box.setText(tr("The project %1 is currently being built.").arg(project->displayName()));
        box.setInformativeText(tr("Do you want to cancel the build process and unload the project anyway?"));
        box.exec();
        if (box.clickedButton() != closeAnyway)
            return;
        ProjectExplorer::BuildManager::cancel();
    }

    m_instance->closeProjects(QList<ProjectExplorer::Project *>() << project);
    m_instance->updateActions();
}

QMenu *ProjectManager::menu()
{
    QMenu *menu = new QMenu(tr("&Projects"));
    menu->addAction(m_cmdNewProject->action());
    menu->addAction(m_cmdOpenProject->action());
    menu->addSeparator();
    menu->addMenu(m_projectBuildMenu);
    menu->addSeparator();
    menu->addAction(m_cmdConfigureProjects->action());
    menu->addAction(m_cmdMiniProjectTargetSelector->action());
    menu->addSeparator();
    menu->addMenu(m_recentProjectMenu);
    menu->addSeparator();
    menu->addAction(m_cmdCloseProject->action());
    menu->addSeparator();
    menu->addAction(m_cmdBuildOptions->action());
    return menu;
}

void ProjectManager::initialize()
{
    qRegisterMetaType<ProjectExplorer::RunControl *>();

    Core::CommandManager::getInstance()->registerCommands( tr("ProjectCommands"),
                                                            QList<Core::Command *>() << m_cmdNewProject
                                                                                     << m_cmdOpenProject
                                                                                     << m_cmdConfigureProjects
                                                                                     << m_cmdMiniProjectTargetSelector
                                                                                     << m_cmdCloseProject
                                                                                     << m_cmdBuildOptions
                                                                                     << m_cmdCancelBuildAction );
    m_projectFilterString = "";
    if ( m_projectProviders.isEmpty() )
        return;

    QList<Core::MimeGlobPattern> allGlobPatterns;
    const QString filterSeparator = QLatin1String(";;");
    QStringList filterStrings;
    Core::MimeDatabase *mimeDb = Core::Storage::mainWindow()->mimeDatabase();

    foreach (ProjectExplorer::IProjectProvider *provider, m_projectProviders) {
        const QString mimeType = provider->mimeType();
        Core::MimeType mime = mimeDb->findByType(mimeType);
        allGlobPatterns.append(mime.globPatterns());
        filterStrings.append(mime.filterString());
    }

    if ( m_projectProviders.size() > 1 )
        filterStrings.prepend(Core::MimeType::formatFilterString(tr("All Projects"), allGlobPatterns));
    m_projectFilterString = filterStrings.join(filterSeparator);

    QMakeParser::initialize();
    ProFileEvaluator::initialize();
    new QtSupport::ProFileCacheManager(this);

    QString errorMessage;
    Core::Storage::mainWindow()->mimeDatabase()->addMimeTypes(QLatin1String(":/projectexplorer/qtsupport/QtSupport.mimetypes.xml"), &errorMessage);

    // ProjectPanelFactories
    auto dependenciesPanelFactory = new ProjectExplorer::ProjectPanelFactory;
    dependenciesPanelFactory->setPriority(50);
    QString displayName = QCoreApplication::translate("DependenciesPanelFactory", "Dependencies");
    dependenciesPanelFactory->setDisplayName(displayName);
    QIcon icon = QIcon(QLatin1String(":/projectexplorer/images/ProjectDependencies.png"));
    dependenciesPanelFactory->setSimpleCreateWidgetFunction<ProjectExplorer::Internal::DependenciesWidget>(icon);
    ProjectExplorer::ProjectPanelFactory::registerFactory(dependenciesPanelFactory);

    auto unconfiguredProjectPanel = new ProjectExplorer::ProjectPanelFactory;
    unconfiguredProjectPanel->setPriority(-10);
    unconfiguredProjectPanel->setDisplayName(tr("Configure Project"));
    unconfiguredProjectPanel->setSupportsFunction([](ProjectExplorer::Project *project){
        return project->targets().isEmpty() && !project->requiresTargetPanel();
    });
    icon = QIcon(QLatin1String(":/projectexplorer/images/unconfigured.png"));
    unconfiguredProjectPanel->setSimpleCreateWidgetFunction<ProjectExplorer::Internal::TargetSetupPageWrapper>(icon);
    ProjectExplorer::ProjectPanelFactory::registerFactory(unconfiguredProjectPanel);

    auto targetSettingsPanelFactory = new ProjectExplorer::ProjectPanelFactory;
    targetSettingsPanelFactory->setPriority(-10);
    displayName = QCoreApplication::translate("TargetSettingsPanelFactory", "Build & Run");
    targetSettingsPanelFactory->setDisplayName(displayName);
    targetSettingsPanelFactory->setSupportsFunction([](ProjectExplorer::Project *project) {
        return !project->targets().isEmpty()
                || project->requiresTargetPanel();
    });
    targetSettingsPanelFactory->setCreateWidgetFunction([](ProjectExplorer::Project *project) {
        return new ProjectExplorer::Internal::TargetSettingsPanelWidget(project);
    });
    ProjectExplorer::ProjectPanelFactory::registerFactory(targetSettingsPanelFactory);

    registerBuildStepFactory(new ProjectExplorer::Internal::ProcessStepFactory);

    m_optionPages << new ProjectExplorer::Internal::ProjectExplorerSettingsPage()
                  << new QtSupport::Internal::QtOptionsPage()
                  << new QtSupport::Internal::CodeGenSettingsPage()
                  << new CppTools::Internal::CppFileSettingsPage(m_fileSettings)
                  << new ProjectExplorer::Internal::DeviceSettingsPage();

    m_deviceManager = new ProjectExplorer::DeviceManager();
    dd->m_kitManager = new ProjectExplorer::KitManager();
    dd->m_toolChainManager = new ProjectExplorer::ToolChainManager();
    m_qtVersionManager = new QtSupport::QtVersionManager();
    m_uiCodeModelManager = new QtSupport::UiCodeModelManager();

    // Qt Version Factories
    QtSupport::QtVersionManager::registerQtVersionFactory(new QtSupport::Internal::DesktopQtVersionFactory);
    QtSupport::QtVersionManager::registerQtVersionFactory(new QtSupport::Internal::SimulatorQtVersionFactory);
    QtSupport::QtVersionManager::registerQtVersionFactory(new QtSupport::Internal::WinCeQtVersionFactory);

    // Add ToolChainFactories:
#ifdef Q_OS_WIN
    m_winDebugInterface = new ProjectExplorer::Internal::WinDebugInterface;
    m_toolChainFactories << new ProjectExplorer::Internal::MsvcToolChainFactory()
                         << new ProjectExplorer::Internal::WinCEToolChainFactory();
#else
    m_toolChainFactories << new ProjectExplorer::Internal::LinuxIccToolChainFactory;
#endif
#ifndef Q_OS_MAC
    m_toolChainFactories << new ProjectExplorer::Internal::MingwToolChainFactory; // Mingw offers cross-compiling to windows
#endif
    m_toolChainFactories << new ProjectExplorer::Internal::GccToolChainFactory()
                         << new ProjectExplorer::Internal::ClangToolChainFactory()
                         << new ProjectExplorer::Internal::CustomToolChainFactory();

    m_deviceFactories << new ProjectExplorer::Internal::DesktopDeviceFactory();
    m_runControlFactories << new ProjectExplorer::Internal::LocalApplicationRunControlFactory;
    m_deployConfigurationFactories << new ProjectExplorer::DeployConfigurationFactory;

    // Register KitInformation:
    ProjectExplorer::KitManager::registerKitInformation(new ProjectExplorer::DeviceTypeKitInformation);
    ProjectExplorer::KitManager::registerKitInformation(new ProjectExplorer::DeviceKitInformation);
    ProjectExplorer::KitManager::registerKitInformation(new ProjectExplorer::ToolChainKitInformation);
    ProjectExplorer::KitManager::registerKitInformation(new ProjectExplorer::SysRootKitInformation);
    ProjectExplorer::KitManager::registerKitInformation(new ProjectExplorer::EnvironmentKitInformation);
    ProjectExplorer::KitManager::registerKitInformation(new QtSupport::QtKitInformation);

    Core::WizardManager::registerFeatureProvider(new ProjectExplorer::Internal::KitFeatureProvider);

    m_kitOptionsPage = new ProjectExplorer::KitOptionsPage;
    m_optionPages << new ProjectExplorer::Internal::ToolChainOptionsPage()
                  << m_kitOptionsPage;

    QtSupport::QtVersionManager::initialized();

    ProjectExplorer::DeviceManager::instance()->addDevice(ProjectExplorer::IDevice::Ptr(new ProjectExplorer::DesktopDevice));
    ProjectExplorer::DeviceManager::instance()->load();
    ProjectExplorer::ToolChainManager::restoreToolChains();
    dd->m_kitManager->restoreKits();

    dd->m_configureProjectsFialog = new ConfigureProjectsDialog(Core::Storage::mainWindow()->widget());
    dd->m_proWindow = dd->m_configureProjectsFialog->projectWindow();

    m_customWizardMetaFactories << new ProjectExplorer::CustomWizardMetaFactory<ProjectExplorer::CustomProjectWizard>(Core::IWizardFactory::ProjectWizard)
                                << new ProjectExplorer::CustomWizardMetaFactory<ProjectExplorer::CustomWizard>(Core::IWizardFactory::FileWizard)
                                << new ProjectExplorer::CustomWizardMetaFactory<ProjectExplorer::CustomWizard>(Core::IWizardFactory::ClassWizard);

    ProjectExplorer::JsonWizardFactory::registerPageFactory(new ProjectExplorer::Internal::FieldPageFactory);
    ProjectExplorer::JsonWizardFactory::registerPageFactory(new ProjectExplorer::Internal::FilePageFactory);
    ProjectExplorer::JsonWizardFactory::registerPageFactory(new ProjectExplorer::Internal::KitsPageFactory);
    ProjectExplorer::JsonWizardFactory::registerPageFactory(new ProjectExplorer::Internal::ProjectPageFactory);
    ProjectExplorer::JsonWizardFactory::registerPageFactory(new ProjectExplorer::Internal::SummaryPageFactory);

    ProjectExplorer::JsonWizardFactory::registerGeneratorFactory(new ProjectExplorer::FileGeneratorFactory);
    Core::WizardManager::registerFileWizardExtensions(new ProjectExplorer::Internal::ProjectFileWizardExtension);

    fillBuildMenus();
    fillProjectTreeContextMenus();
    initProviders();

    dd->m_projectSelectorAction = m_cmdMiniProjectTargetSelector->action();
    QToolButton *miniSelectorToolButton = m_projectDock->createPermanentToolButton(m_cmdMiniProjectTargetSelector->action());
    dd->m_targetSelector = new ProjectExplorer::Internal::MiniProjectTargetSelector(miniSelectorToolButton);
    connect(m_cmdMiniProjectTargetSelector->action(), SIGNAL(triggered(bool)), dd->m_targetSelector, SLOT(toggleVisible()));

    Core::WizardManager::registerDelayedWizardsLoader(this);
}

static QwCustomDockWidget *createDockFromOutputPane(Core::IOutputPane *pane, IMainWindow *mainWindow, const QKeySequence &shortcut, const QIcon &icon)
{
    QList<QAction*> actions = pane->toolBarActions();
    QwCustomDockWidget *result = new QwCustomDockWidget( mainWindow->widget(), pane->displayName(), "OutputPane_" + pane->displayName(),
                                                         shortcut, icon,
                                                         Qt::BottomDockWidgetArea, QwCustomDockWidget::First,
                                                         actions );
    QMenu *contextMenu = new QMenu;
    contextMenu->addActions(actions);
    result->setContextMenu(contextMenu);
    result->setWidget(pane->outputWidget(result));

    QObject::connect(pane, SIGNAL(showPage(bool)), result, SLOT(popup()));

    return result;
}

static QwTabBarDockWidget *createDockFromAppOutputPane(ProjectExplorer::Internal::AppOutputPane *pane, IMainWindow *mainWindow, const QKeySequence &shortcut, const QIcon &icon)
{
    QList<QAction*> actions = pane->toolBarActions();
    QwTabBarDockWidget *result = new QwTabBarDockWidget( mainWindow->widget(), pane->displayName(), "OutputPane_" + pane->displayName(),
                                                         shortcut, icon,
                                                         Qt::BottomDockWidgetArea, QwCustomDockWidget::First,
                                                         actions,
                                                         pane );
    QMenu *contextMenu = new QMenu;
    contextMenu->addActions(actions);
    result->setContextMenu(contextMenu);
    pane->setDock(result);
//    result->setWidget(pane->outputWidget(result));

    QObject::connect(pane, SIGNAL(showPage(bool)), result, SLOT(popup()));

    return result;
}

QList<UiProviderInterface *> ProjectManager::createUiProviders(IMainWindow *mainWindow)
{
    m_projectDock = new QwCustomDockWidget( mainWindow->widget(), tr("Projects"), "ProjectsDockWidget",
                                            QKeySequence(tr("Ctrl+D,P")), QIcon(":/core/images/prj_dock.png"),
                                            Qt::LeftDockWidgetArea, QwCustomDockWidget::First,
                                            QList<QAction *>() << m_cmdNewProject->action()
                                                               << m_cmdOpenProject->action()
                                                               << m_cmdConfigureProjects->action()
                                                               << m_cmdCloseProject->action()
                                                               << m_cmdBuildOptions->action() );

    m_projectsTree = new ProjectExplorer::Internal::ProjectTreeWidget(mainWindow->widget());
    m_projectDock->setWidget(m_projectsTree);

    QMenu *contextMenu = new QMenu;
    contextMenu->addAction(m_cmdNewProject->action());
    contextMenu->addAction(m_cmdOpenProject->action());
    contextMenu->addSeparator();
    contextMenu->addAction(m_cmdConfigureProjects->action());
    contextMenu->addSeparator();
    contextMenu->addMenu(m_recentProjectMenu);
    contextMenu->addSeparator();
    contextMenu->addAction(m_cmdCloseProject->action());
    m_projectDock->setContextMenu(contextMenu);

    dd->m_taskHub = new ProjectExplorer::TaskHub();

    ProjectExplorer::BuildManager *buildManager = new ProjectExplorer::BuildManager(this, m_cmdCancelBuildAction->action());
    ProjectExplorer::BuildManager::extensionsInitialized();
    connect(buildManager, SIGNAL(buildStateChanged(ProjectExplorer::Project*)),
            this, SLOT(buildStateChanged(ProjectExplorer::Project*)));
    connect(buildManager, SIGNAL(buildQueueFinished(bool)),
            this, SLOT(buildQueueFinished(bool)), Qt::QueuedConnection);

    registerMacroExpanderVariables();

    m_compilerOutputDock = createDockFromOutputPane(buildManager->compileOutputWindow(), mainWindow, QKeySequence(tr("Ctrl+D,/")),QIcon(":/projectexplorer/images/prj_compileoutput.png"));
    m_issuesPane = buildManager->taskOutputWindow();
    QwCustomDockWidget *taskOutputDock = createDockFromOutputPane(m_issuesPane, mainWindow, QKeySequence(tr("Ctrl+D,\\")), QIcon(":/projectexplorer/images/prj_task.png"));

    QMainWindow *mw = mainWindow->widget();
    m_cmdIssuesClear    = new Core::Command( tr("BuildIssuesClear"),
                                             QIcon(":/projectexplorer/images/clean_small.png"),
                                             tr("Clear"),
                                             tr("Clear Build Issues"),
                                             QKeySequence(),
                                             mw );
    m_cmdIssuesNext     = new Core::Command( tr("BuildIssuesNext"),
                                             QIcon(":/projectexplorer/images/iss_next.png"),
                                             tr("Next"),
                                             tr("Go To Next Issue"),
                                             QKeySequence(tr("Alt+F6")),
                                             mw );
    m_cmdIssuesPrevious = new Core::Command( tr("BuildIssuesPrevious"),
                                             QIcon(":/projectexplorer/images/iss_prev.png"),
                                             tr("Previous"),
                                             tr("Go To Previous Issue"),
                                             QKeySequence(tr("Alt+Shift+F6")),
                                             mw );

    m_cmdIssuesNext->action()->setEnabled(false);
    m_cmdIssuesPrevious->action()->setEnabled(false);

    connect(m_cmdIssuesClear->action(), SIGNAL(triggered()), this, SLOT(issuesClear()));
    connect(m_cmdIssuesNext->action(), SIGNAL(triggered()), this, SLOT(issuesGoToNext()));
    connect(m_cmdIssuesPrevious->action(), SIGNAL(triggered()), this, SLOT(issuesGoToPrevious()));
    connect(m_issuesPane, SIGNAL(navigateStateUpdate()), this, SLOT(issuesPaneNavigateStateChanged()));

    taskOutputDock->createPermanentToolButton(m_cmdIssuesClear->action());
    taskOutputDock->createPermanentToolButton(m_cmdIssuesPrevious->action());
    taskOutputDock->createPermanentToolButton(m_cmdIssuesNext->action());

    Core::CommandManager::getInstance()->registerCommands( tr("ProjectCommands"),
                                                           QList<Core::Command *>() << m_cmdIssuesClear
                                                                                    << m_cmdIssuesNext
                                                                                    << m_cmdIssuesPrevious );

    dd->m_outputPane = new ProjectExplorer::Internal::AppOutputPane();
    m_appOutputDock = createDockFromAppOutputPane(dd->m_outputPane, mainWindow, QKeySequence(QObject::tr("Ctrl+D,O")), QIcon(":/projectexplorer/images/prj_appoutput.png"));

    return QList<UiProviderInterface *>() << new UiProviderInterface(m_projectDock)
                                          << new UiProviderInterface(m_appOutputDock)
                                          << new UiProviderInterface(m_compilerOutputDock)
                                          << new UiProviderInterface(taskOutputDock);
}

bool ProjectManager::sessionAboutToClose()
{
    if  ( ProjectExplorer::BuildManager::isBuilding() ) {
        if ( !Core::Storage::question(tr("Cancel Build Process?")) )
            return false;
        ProjectExplorer::BuildManager::cancel();
    }

    return true;
}

void ProjectManager::saveSession( const QString &location )
{
    QString fileName = location;
    fileName += "/projects.xml";

    QFile file(fileName);
    if ( !file.open(QFile::WriteOnly | QFile::Text) ) {
        Core::Storage::warning( tr("Error On Save Session Project \"%1\" Cannot be Opened for Writing:\n%2.")
                                .arg(fileName).arg(file.errorString()) );
        return;
    }

    QDomDocument domDocument("projects");

    domDocument.appendChild(domDocument.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"windows-1251\""));

    QDomElement root = domDocument.createElement("projects");
    root.setAttribute("version", "1.0");
    domDocument.appendChild(root);

    foreach ( ProjectExplorer::Project *pro, m_projects ) {
        pro->saveSettings();
        QDomElement projectElement = domDocument.createElement("project");
        projectElement.setAttribute("path", pro->projectFilePath().toString());
        projectElement.setAttribute("active", pro == m_startupProject ? 1 : 0);
        root.appendChild(projectElement);
    }

    QDomElement depsElement = domDocument.createElement("depencies");
    for ( QMap<QString, QStringList>::const_iterator it = m_depMap.constBegin();
          it != m_depMap.constEnd(); ++it ) {
        QDomElement slaveElement = domDocument.createElement("slave");
        slaveElement.setAttribute("path", it.key());
        const QStringList &deps = it.value();
        foreach ( const QString &depOn, deps) {
            QDomElement masterElement = domDocument.createElement("master");
            masterElement.setAttribute("path", depOn);
            slaveElement.appendChild(masterElement);
        }
        depsElement.appendChild(slaveElement);
    }
    root.appendChild(depsElement);

    const int IndentSize = 4;
    QTextStream out(&file);
    domDocument.save(out, IndentSize);

    file.close();
}

void ProjectManager::loadSession(const QString &location)
{
    QString fileName = location;
    fileName += "/projects.xml";

    QFile file(fileName);
    if ( !file.exists() )
        return;

    QDomDocument domDocument;

    QString errorStr;
    int errorLine;
    int errorColumn;

    // Load all
    if (!domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn)) {
        Core::Storage::warning( tr("Error On Load Session Projects\nCommon Error in xml-file:\nLine %1, Column %2: %3")
                                .arg(errorLine).arg(errorColumn).arg(errorStr) );
        return;
    }

    // Primary check (Document Type and Version)
    QDomElement root = domDocument.documentElement();
    if ( root.tagName() != "projects" ) {
        Core::Storage::warning( tr("Error On Load Session Projects\nInvalid Format of Projects xml-file") );
        return;
    }
    if ( !root.hasAttribute("version") ) {
        Core::Storage::warning( tr("Error On Load Session Projects\nInvalid Format of Projects xml-file (version expected)") );
        return;
    }
    if ( root.attribute("version") != "1.0" ) {
        Core::Storage::warning( tr("Error On Load Session Projects\nInvalid Format of Projects xml-file (wrong version)") );
        return;
    }

    QDomElement itemElement = root.firstChildElement();
    while ( !itemElement.isNull() ) {
        if ( itemElement.tagName() == QLatin1String("project") ) {
            bool isActive = itemElement.attribute("active", "0").toInt();
            QString filePath = itemElement.attribute("path", "");
            ProjectExplorer::Project *pro = openProjectInternal(filePath);
            if ( pro && isActive )
                setStartupProject(pro);
        } else if ( itemElement.tagName() == QLatin1String("depencies") ) {
            m_depMap.clear();
            QDomElement depsElement = itemElement.firstChildElement("slave");
            while ( !depsElement.isNull() ) {
                QString slave = depsElement.attribute("path", "");
                QStringList masters;
                QDomElement masterElement = depsElement.firstChildElement("master");
                while ( !masterElement.isNull() ) {
                    masters << masterElement.attribute("path", "");
                    masterElement = masterElement.nextSiblingElement("master");
                }
                if ( !masters.isEmpty() )
                    m_depMap[slave] = masters;
                depsElement = depsElement.nextSiblingElement("slave");
            }
        } else {
            Core::Storage::warning( tr("Error On Load Session Projects\nInvalid Format of Projects xml-file (unknown tag <%1> at line %2)")
                                    .arg(itemElement.tagName()).arg(itemElement.lineNumber()) );
            return;
        }
        QString fileName = itemElement.attribute("projectPath");
        bool isOpened = itemElement.attribute("opened").toInt();
        if ( isOpened )
            openProjectFilePath(fileName);
        itemElement = itemElement.nextSiblingElement();
    }

    file.close();

    updateActions();
    emit sessionLoaded();
}

bool ProjectManager::applicationAboutToClose()
{
    if  ( ProjectExplorer::BuildManager::isBuilding() ) {
        if ( !Core::Storage::question(tr("Cancel Build Process?")) )
            return false;
        ProjectExplorer::BuildManager::cancel();
    }

    m_instance->savePersistentSettings();
    m_instance->m_settings->sync();

    m_appOutputDock->removeAllTabs();
    return (m_appOutputDock->count() == 0);
}

void ProjectManager::applicationClose()
{
    dd->m_proWindow->aboutToShutdown();
}

void ProjectManager::showOptionsDialog(const Core::Id &categoryId, const Core::Id &pageId, QWidget *parent)
{
    if ( !parent )
        parent = Core::Storage::mainWindow()->widget();
    Core::Internal::SettingsDialog::getSettingsDialog(parent, categoryId, pageId)->execDialog();

}

ProjectExplorer::Internal::ProjectExplorerSettings ProjectManager::projectExplorerSettings()
{ return dd->m_projectExplorerSettings; }

void ProjectManager::setProjectExplorerSettings(const ProjectExplorer::Internal::ProjectExplorerSettings &settings)
{ dd->m_projectExplorerSettings = settings; }

QString ProjectManager::projectsDirectory()
{ return m_instance->m_projectsDirectory; }

void ProjectManager::setProjectsDirectory(const QString &dir)
{ m_instance->m_projectsDirectory = dir; }

bool ProjectManager::useProjectsDirectory()
{ return m_instance->m_useProjectsDirectory; }

void ProjectManager::setUseProjectsDirectory(bool onoff)
{ m_instance->m_useProjectsDirectory = onoff; }

QString ProjectManager::buildDirectory()
{ return m_instance->m_buildDirectory; }

void ProjectManager::setBuildDirectory(const QString &dir)
{ m_instance->m_buildDirectory = dir; }

QStringList ProjectManager::settingsKeyList() const
{ return QStringList(); }

ISettings *ProjectManager::createSettings( const QString & ) const
{ return 0; }

void ProjectManager::reportError(const QString &message)
{
    m_compilerOutputDock->showAndSetCurrentIfTabified();
    ProjectExplorer::BuildManager::reportError(message);
}

void ProjectManager::requestProjectModeUpdate(ProjectExplorer::Project *p)
{
    dd->m_proWindow->projectUpdated(p);
}

void ProjectManager::buildProjectNode(ProjectNode *projectNode)
{
    dd->queue(projectOrder(projectForNode(projectNode)), QList<Core::Id>() << Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD));
}

void ProjectManager::rebuildProjectNode(ProjectNode *projectNode)
{
    dd->queue(projectOrder(projectForNode(projectNode)),
              QList<Core::Id>() << Core::Id(ProjectExplorer::Constants::BUILDSTEPS_CLEAN)
              << Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD));
}

void ProjectManager::cleanProjectNode(ProjectNode *projectNode)
{
    dd->queue(projectOrder(projectForNode(projectNode)), QList<Core::Id>() << Core::Id(ProjectExplorer::Constants::BUILDSTEPS_CLEAN));
}

void ProjectManager::runProjectNode(ProjectNode *projectNode)
{
    runProject(projectForNode(projectNode), ProjectExplorer::NormalRunMode);
}

void ProjectManager::executeBuildSystemCommandForNode(ProjectNode *projectNode)
{
    if ( ProjectExplorer::Project *project = projectForNode(projectNode) ) {
        if ( ProjectExplorer::IProjectProvider *provider = project->projectProvider() ) {
            provider->executeBuildSystemCommand(project, 0);
        }
    }
}

void ProjectManager::closeProjectNode(ProjectNode *projectNode)
{
    QList<ProjectExplorer::Project *> projectsToClose;
    projectsToClose << projectForNode(projectNode);
    closeProjects(projectsToClose);
}

void ProjectManager::newProject()
{
    Core::WizardManager::showNewItemDialog(tr("New Project", "Title of dialog"),
                                           QList<Core::IWizardFactory::WizardKind>() << Core::IWizardFactory::ProjectWizard);
    updateActions();
}

ProjectExplorer::Project *ProjectManager::openProjectInternal(const QString &filePath)
{
    foreach (ProjectExplorer::Project *pi, ProjectManager::projects()) {
        if (filePath == pi->projectFilePath().toString()) {
            QString errorMessage = tr("Failed opening project \"%1\": Project already open.").arg(QDir::toNativeSeparators(filePath));
            Core::Storage::warning(errorMessage);
            Core::LogManager::addMessage(Core::LogMessage(errorMessage, tr("Project")));
            /// TODO:! SessionManager::reportProjectLoadingProgress();
            return 0;
        }
    }

    Core::MimeDatabase *mimeDb = Core::Storage::mainWindow()->mimeDatabase();
    const Core::MimeType mt = mimeDb->findByFile(QFileInfo(filePath));
    if (mt.isNull()) {
        Core::LogManager::addMessage(Core::LogMessage(tr("Failed opening project \"%1\": Unknown project type").arg(QDir::toNativeSeparators(filePath)),
                                                      tr("Project")));
        return 0;
    }

    foreach (ProjectExplorer::IProjectProvider *provider, m_projectProviders) {
        if ( provider->mimeType() == mt.type() ) {
            QString tmp;
            ProjectExplorer::Project *pro = provider->openProject(filePath, &tmp);
            if ( pro ) {
                if ( !pro->restoreSettings() ) {
                    Core::LogManager::addMessage(Core::LogMessage(tr("Failed opening project \"%1\": Settings could not be restored.").arg(QDir::toNativeSeparators(filePath)),
                                                                  tr("Project")));
                    delete pro;
                    return 0;
                }
                connect(pro, SIGNAL(fileListChanged()), this, SIGNAL(fileListChanged()));
                addProject(pro);
                pro->setProjectProvider(provider);
                m_recentProjectMenu->appendItem(filePath);
            } else
                Core::LogManager::addMessage(Core::LogMessage(tr("Failed opening project \"%1\": %2.").arg(QDir::toNativeSeparators(filePath)).arg(tmp),
                                                              tr("Project")));
            return pro;
        }
    }

    Core::LogManager::addMessage(Core::LogMessage(tr("Failed opening project \"%1\": No Project Provider can open project type \"%2\".")
                                                  .arg(QDir::toNativeSeparators(filePath)) .arg((mt.type())),
                                                  tr("Project")));
    return 0;
}

void ProjectManager::closeProjects(const QList<ProjectExplorer::Project *> &remove)
{
    foreach (ProjectExplorer::Project *pro, remove)
        emit aboutToRemoveProject(pro);

    // Refresh dependencies
    QMap<QString, QStringList> resMap;
    QSet<QString> projectFiles;
    foreach (ProjectExplorer::Project *pro, projects()) {
        if (!remove.contains(pro))
            projectFiles.insert(pro->projectFilePath().toString());
    }

    QSet<QString>::const_iterator i = projectFiles.begin();
    while (i != projectFiles.end()) {
        QStringList dependencies;
        foreach (const QString &dependency, m_depMap.value(*i)) {
            if (projectFiles.contains(dependency))
                dependencies << dependency;
        }
        if (!dependencies.isEmpty())
            resMap.insert(*i, dependencies);
        ++i;
    }

    m_depMap = resMap;

    // Delete projects
    foreach (ProjectExplorer::Project *pro, remove) {
        pro->saveSettings();
        m_projects.removeOne(pro);

        if (pro == m_startupProject)
            setStartupProject(0);

        disconnect(pro, SIGNAL(fileListChanged()), this, SLOT(clearProjectFileCache()));
        m_projectFileCache.remove(pro);

        m_sessionNode->removeProjectNodes(QList<ProjectExplorer::ProjectNode *>() << pro->rootProjectNode());
        disconnect(pro, SIGNAL(buildConfigurationEnabledChanged()),
                   this, SLOT(updateActions()));
        emit projectRemoved(pro);
        delete pro->rootProjectNode();  /// TODO: ?
        delete pro;
    }

    if (startupProject() == 0)
        if (!m_projects.isEmpty())
            setStartupProject(m_projects.first());
}

QList<ProjectExplorer::Project *> ProjectManager::projectOrder(ProjectExplorer::Project *project)
{
    QList<ProjectExplorer::Project *> result;

    QStringList pros;
    if (project)
        pros = dependencies(project->projectFilePath().toString());
    else
        pros = dependenciesOrder();

    foreach (const QString &proFile, pros) {
        foreach (ProjectExplorer::Project *pro, projects()) {
            if (pro->projectFilePath().toString() == proFile) {
                result << pro;
                break;
            }
        }
    }

    return result;
}

void ProjectManager::showContextMenu(QWidget *view, const QPoint &globalPos, ProjectExplorer::Node *node)
{
    QMenu *contextMenu = 0;

    if (!node)
        node = sessionNode();


    ProjectExplorer::Project *project = ProjectManager::projectForNode(node);

    if (node->nodeType() != ProjectExplorer::SessionNodeType) {
        setCurrentNode(node);

        emit m_instance->aboutToShowContextMenu(project, node);
        switch (node->nodeType()) {
        case ProjectExplorer::ProjectNodeType:
            if (node->parentFolderNode() == ProjectManager::sessionNode())
                contextMenu = dd->m_projectMenu;
            else
                contextMenu = dd->m_subProjectMenu;
            break;
        case ProjectExplorer::VirtualFolderNodeType:
        case ProjectExplorer::FolderNodeType:
            contextMenu = dd->m_folderMenu;
            break;
        case ProjectExplorer::FileNodeType:
            /// TODO: ?
            // m_instance->populateOpenWithMenu();
            contextMenu = dd->m_fileMenu;
            break;
        default:
            qWarning("ProjectExplorerPlugin::showContextMenu - Missing handler for node type");
        }
    } else { // session item
        emit m_instance->aboutToShowContextMenu(0, node);

        contextMenu = dd->m_sessionContextMenu;
    }

    dd->updateContextMenuActions();
    dd->m_projectTreeCollapseAllAction->disconnect(SIGNAL(triggered()));
    connect(dd->m_projectTreeCollapseAllAction, SIGNAL(triggered()), view, SLOT(collapseAll()));
    if (project && contextMenu && contextMenu->actions().count() > 0) {
        QList<QAction *> additionalActions;
        if ( ProjectExplorer::IProjectProvider *provider = project->projectProvider() )
            additionalActions = provider->additionalContextMenuActions(node);
        QAction *separatorAction = 0;
        if ( !additionalActions.isEmpty() ) {
            separatorAction = contextMenu->addSeparator();
            contextMenu->addActions(additionalActions);
        }
        contextMenu->exec(globalPos);
        if ( !additionalActions.isEmpty() ) {
            contextMenu->removeAction(separatorAction);
            foreach (QAction *action, additionalActions)
                contextMenu->removeAction(action);
        }
    }
}

void ProjectManager::renameFile(ProjectExplorer::Node *node, const QString &to)
{
    QString orgFilePath = QFileInfo(node->path()).absoluteFilePath();
    QString dir = QFileInfo(orgFilePath).absolutePath();
    QString newFilePath = dir + QLatin1Char('/') + to;

    if (Utils::FileUtils::renameFile(orgFilePath, newFilePath)) {
        // Tell the project plugin about rename
        ProjectExplorer::FolderNode *folderNode = node->parentFolderNode();
        QString projectDisplayName = folderNode->projectNode()->displayName();
        if (!folderNode->renameFile(orgFilePath, newFilePath)) {
            QMessageBox::warning(Core::Storage::mainWindow()->widget(),
                                 tr("Project Editing Failed"),
                                 tr("The file %1 was renamed to %2, but the project file %3 could not be automatically changed.")
                                 .arg(orgFilePath)
                                 .arg(newFilePath)
                                 .arg(projectDisplayName));
        } else {
            m_instance->setCurrent(projectForFile(newFilePath), newFilePath, 0);
        }
    }
}

QStringList ProjectManager::dependencies(const QString &proName) const
{
    QStringList result;
    dependencies(proName, result);
    return result;
}

void ProjectManager::dependencies(const QString &proName, QStringList &result) const
{
    QStringList depends = m_depMap.value(proName);

    foreach (const QString &dep, depends)
        dependencies(dep, result);

    if (!result.contains(proName))
        result.append(proName);
}

QStringList ProjectManager::dependenciesOrder() const
{
    QList<QPair<QString, QStringList> > unordered;
    QStringList ordered;

    // copy the map to a temporary list
    foreach (ProjectExplorer::Project *pro, m_projects) {
        const QString &proName = pro->projectFilePath().toString();
        unordered << QPair<QString, QStringList>(proName, m_depMap.value(proName));
    }

    while (!unordered.isEmpty()) {
        for (int i=(unordered.count()-1); i>=0; --i) {
            if (unordered.at(i).second.isEmpty()) {
                ordered << unordered.at(i).first;
                unordered.removeAt(i);
            }
        }

        // remove the handled projects from the dependency lists
        // of the remaining unordered projects
        for (int i = 0; i < unordered.count(); ++i) {
            foreach (const QString &pro, ordered) {
                QStringList depList = unordered.at(i).second;
                depList.removeAll(pro);
                unordered[i].second = depList;
            }
        }
    }

    return ordered;
}

void ProjectManager::openProject()
{
    if ( m_projectProviders.isEmpty() ) {
        Core::Storage::warning(tr("Project Providers not Found"));
        return;
    }

    QStringList files = QFileDialog::getOpenFileNames( Core::Storage::mainWindow()->widget(),
                                                       tr("Open Project"),
                                                       QString(),
                                                       m_projectFilterString );
    if ( files.isEmpty() )
        return;

    QList<ProjectExplorer::Project *> openedPro;

    foreach ( const QString &fileName, files ) {
        QFileInfo fi = QFileInfo(fileName);
        QString filePath = fileName;
        if (fi.exists()) // canonicalFilePath will be empty otherwise!
            filePath = fi.canonicalFilePath();

        if ( ProjectExplorer::Project *pro = openProjectInternal(filePath) ) {
            // Make sure we always have a current project / node
            if (!dd->m_currentProject && openedPro.isEmpty())
                setCurrentNode(pro->rootProjectNode());
            openedPro += pro;
            m_recentProjectMenu->appendItem(filePath);
        }
    }

    if ( !openedPro.isEmpty() )
        setStartupProject(openedPro.first());

    updateActions();
}

void ProjectManager::configureProjects()
{
    dd->m_configureProjectsFialog->exec();
    updateActions();
}

void ProjectManager::closeProject()
{
    if ( m_projects.isEmpty() )
        return;

    QList<ProjectExplorer::Project *> projectsToClose;
    if ( m_projects.size() == 1 )
        projectsToClose << m_projects[0];
    else {
        SelectProjectsDialog projectsDialog(m_projects, m_startupProject, Core::Storage::mainWindow()->widget());
        if ( projectsDialog.exec() == QDialog::Accepted )
            projectsToClose = projectsDialog.selectedProjects();
    }

    if ( !projectsToClose.isEmpty() ) {
        closeProjects(projectsToClose);
        updateActions();
    }
}

void ProjectManager::buildOptions()
{
    Core::Internal::SettingsDialog::getSettingsDialog( Core::Storage::mainWindow()->widget(),
                                                       Core::Id(), Core::Id())->execDialog();
}

void ProjectManager::clearProjectFileCache()
{
    ProjectExplorer::Project *pro = qobject_cast<ProjectExplorer::Project*>(m_instance->sender());
    if (pro)
        m_projectFileCache.remove(pro);
    else
        m_projectFileCache.clear();
}

void ProjectManager::setStartupProject()
{
    setStartupProject(dd->m_currentProject);
}

void ProjectManager::buildStateChanged(ProjectExplorer::Project *pro)
{
    Q_UNUSED(pro);
    updateActions();
}

void ProjectManager::buildQueueFinished(bool success)
{
    updateActions();

    bool ignoreErrors = true;
    if (!dd->m_delayedRunConfiguration.isNull() && success && ProjectExplorer::BuildManager::getErrorTaskCount() > 0) {
        ignoreErrors = QMessageBox::question(Core::Storage::mainWindow()->widget(),
                                             tr("Ignore All Errors?"),
                                             tr("Found some build errors in current task.\n"
                                                "Do you want to ignore them?"),
                                             QMessageBox::Yes | QMessageBox::No,
                                             QMessageBox::No) == QMessageBox::Yes;
    }
    if (dd->m_delayedRunConfiguration.isNull() && dd->m_shouldHaveRunConfiguration) {
        QMessageBox::warning(Core::Storage::mainWindow()->widget(),
                             tr("Run Configuration Removed"),
                             tr("The configuration that was supposed to run is no longer "
                                "available."), QMessageBox::Ok);
    }

    if (success && ignoreErrors && !dd->m_delayedRunConfiguration.isNull()) {
        dd->executeRunConfiguration(dd->m_delayedRunConfiguration.data(), dd->m_runMode);
    } else {
        if (ProjectExplorer::BuildManager::tasksAvailable())
            ProjectExplorer::BuildManager::showTaskWindow();
    }
    dd->m_delayedRunConfiguration = 0;
    dd->m_shouldHaveRunConfiguration = false;
    dd->m_runMode = ProjectExplorer::NoRunMode;
}

void ProjectManager::buildSession()
{
    dd->queue(projectOrder(), QList<Core::Id>() << Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD));
}

void ProjectManager::buildProject()
{
    dd->queue(projectOrder(startupProject()), QList<Core::Id>() << Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD));
}

void ProjectManager::rebuildSession()
{
    dd->queue(projectOrder(), QList<Core::Id>() << Core::Id(ProjectExplorer::Constants::BUILDSTEPS_CLEAN)
                                                << Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD));
}

void ProjectManager::rebuildProject()
{
    dd->queue(projectOrder(startupProject()), QList<Core::Id>() << Core::Id(ProjectExplorer::Constants::BUILDSTEPS_CLEAN)
              << Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD));
}

void ProjectManager::cleanSession()
{
    dd->queue(projectOrder(), QList<Core::Id>() << Core::Id(ProjectExplorer::Constants::BUILDSTEPS_CLEAN));
}

void ProjectManager::cleanProject()
{
    dd->queue(projectOrder(startupProject()), QList<Core::Id>() << Core::Id(ProjectExplorer::Constants::BUILDSTEPS_CLEAN));
}

void ProjectManager::cancelBuild()
{
    if (ProjectExplorer::BuildManager::isBuilding())
        ProjectExplorer::BuildManager::cancel();
}

void ProjectManager::runProject()
{
    runProject(startupProject(), ProjectExplorer::NormalRunMode);
}

void ProjectManager::updateActions()
{
    dd->updateActions();
    emit updateRunActions();
}

void ProjectManager::savePersistentSettings()
{
    QSettings *s = settings();
    s->setValue(QLatin1String("ProjectExplorer/Settings/BuildDirectoryTemplate"), m_buildDirectory);
    s->setValue(QLatin1String("ProjectExplorer/Settings/UseProjectsDirectory"), m_useProjectsDirectory);
    s->setValue(QLatin1String("ProjectExplorer/Settings/ProjectsDirectory"), m_projectsDirectory);
    s->setValue(QLatin1String("ProjectExplorer/Settings/BuildBeforeDeploy"), dd->m_projectExplorerSettings.buildBeforeDeploy);
    s->setValue(QLatin1String("ProjectExplorer/Settings/DeployBeforeRun"), dd->m_projectExplorerSettings.deployBeforeRun);
    s->setValue(QLatin1String("ProjectExplorer/Settings/SaveBeforeBuild"), dd->m_projectExplorerSettings.saveBeforeBuild);
    s->setValue(QLatin1String("ProjectExplorer/Settings/ShowCompilerOutput"), dd->m_projectExplorerSettings.showCompilerOutput);
    s->setValue(QLatin1String("ProjectExplorer/Settings/ShowRunOutput"), dd->m_projectExplorerSettings.showRunOutput);
    s->setValue(QLatin1String("ProjectExplorer/Settings/ShowDebugOutput"), dd->m_projectExplorerSettings.showDebugOutput);
    s->setValue(QLatin1String("ProjectExplorer/Settings/CleanOldAppOutput"), dd->m_projectExplorerSettings.cleanOldAppOutput);
    s->setValue(QLatin1String("ProjectExplorer/Settings/MergeStdErrAndStdOut"), dd->m_projectExplorerSettings.mergeStdErrAndStdOut);
    s->setValue(QLatin1String("ProjectExplorer/Settings/WrapAppOutput"), dd->m_projectExplorerSettings.wrapAppOutput);
    s->setValue(QLatin1String("ProjectExplorer/Settings/UseJom"), dd->m_projectExplorerSettings.useJom);
    s->setValue(QLatin1String("ProjectExplorer/Settings/AutoRestoreLastSession"), dd->m_projectExplorerSettings.autorestoreLastSession);
    s->setValue(QLatin1String("ProjectExplorer/Settings/PromptToStopRunControl"), dd->m_projectExplorerSettings.prompToStopRunControl);
    s->setValue(QLatin1String("ProjectExplorer/Settings/MaxAppOutputLines"), dd->m_projectExplorerSettings.maxAppOutputLines);
    // s->setValue(QLatin1String("ProjectExplorer/Settings/AutoRunBuildSystemCommand"), dd->m_projectExplorerSettings.autoRunBuildSystemCommand);
    s->setValue(QLatin1String("ProjectExplorer/Settings/EnvironmentId"), dd->m_projectExplorerSettings.environmentId.toByteArray());
    m_fileSettings->toSettings(s);
}

void ProjectManager::restorePersistentSettings()
{
    QSettings *s = settings();
    m_buildDirectory = s->value(QLatin1String("ProjectExplorer/Settings/BuildDirectoryTemplate"),
                                QLatin1String("../build-%{CurrentProject:Name}-%{CurrentKit:FileSystemName}-%{CurrentBuild:Name}")).toString();
    m_useProjectsDirectory = s->value(QLatin1String("ProjectExplorer/Settings/UseProjectsDirectory"), false).toBool();
    m_projectsDirectory = s->value(QLatin1String("ProjectExplorer/Settings/ProjectsDirectory"), QLatin1String("")).toString();
    dd->m_projectExplorerSettings.buildBeforeDeploy =
            s->value(QLatin1String("ProjectExplorer/Settings/BuildBeforeDeploy"), true).toBool();
    dd->m_projectExplorerSettings.deployBeforeRun =
            s->value(QLatin1String("ProjectExplorer/Settings/DeployBeforeRun"), true).toBool();
    dd->m_projectExplorerSettings.saveBeforeBuild =
            s->value(QLatin1String("ProjectExplorer/Settings/SaveBeforeBuild"), false).toBool();
    dd->m_projectExplorerSettings.showCompilerOutput =
            s->value(QLatin1String("ProjectExplorer/Settings/ShowCompilerOutput"), false).toBool();
    dd->m_projectExplorerSettings.showRunOutput =
            s->value(QLatin1String("ProjectExplorer/Settings/ShowRunOutput"), true).toBool();
    dd->m_projectExplorerSettings.showDebugOutput =
            s->value(QLatin1String("ProjectExplorer/Settings/ShowDebugOutput"), false).toBool();
    dd->m_projectExplorerSettings.cleanOldAppOutput =
            s->value(QLatin1String("ProjectExplorer/Settings/CleanOldAppOutput"), false).toBool();
    dd->m_projectExplorerSettings.mergeStdErrAndStdOut =
            s->value(QLatin1String("ProjectExplorer/Settings/MergeStdErrAndStdOut"), false).toBool();
    dd->m_projectExplorerSettings.wrapAppOutput =
            s->value(QLatin1String("ProjectExplorer/Settings/WrapAppOutput"), true).toBool();
    dd->m_projectExplorerSettings.useJom =
            s->value(QLatin1String("ProjectExplorer/Settings/UseJom"), true).toBool();
    dd->m_projectExplorerSettings.autorestoreLastSession =
            s->value(QLatin1String("ProjectExplorer/Settings/AutoRestoreLastSession"), false).toBool();
    dd->m_projectExplorerSettings.prompToStopRunControl =
            s->value(QLatin1String("ProjectExplorer/Settings/PromptToStopRunControl"), false).toBool();
    dd->m_projectExplorerSettings.maxAppOutputLines =
            s->value(QLatin1String("ProjectExplorer/Settings/MaxAppOutputLines"), 100000).toInt();
    // dd->m_projectExplorerSettings.autoRunBuildSystemCommand =
    //         s->value(QLatin1String("ProjectExplorer/Settings/AutoRunBuildSystemCommand"), true).toBool();
    dd->m_projectExplorerSettings.environmentId =
            QUuid(s->value(QLatin1String("ProjectExplorer/Settings/EnvironmentId")).toByteArray());
    if (dd->m_projectExplorerSettings.environmentId.isNull())
        dd->m_projectExplorerSettings.environmentId = QUuid::createUuid();
    m_fileSettings->fromSettings(s);
}

void ProjectManager::runConfigurationConfigurationFinished()
{
    ProjectExplorer::RunConfiguration *rc = qobject_cast<ProjectExplorer::RunConfiguration *>(sender());
    ProjectExplorer::RunMode runMode = ProjectExplorer::NoRunMode;
    for (int i = 0; i < dd->m_delayedRunConfigurationForRun.size(); ++i) {
        if (dd->m_delayedRunConfigurationForRun.at(i).first == rc) {
            runMode = dd->m_delayedRunConfigurationForRun.at(i).second;
            dd->m_delayedRunConfigurationForRun.removeAt(i);
            break;
        }
    }
    if (runMode != ProjectExplorer::NoRunMode && rc->isConfigured())
        dd->executeRunConfiguration(rc, runMode);
}

void ProjectManager::slotUpdateRunActions()
{
    ProjectExplorer::Project *project = ProjectManager::startupProject();
    QString whyNot;
    const bool state = canRun(project, ProjectExplorer::NormalRunMode, &whyNot);
    dd->m_runAction->setEnabled(state);
    dd->m_runAction->setToolTip(whyNot);
}

void ProjectManager::slotStartupProjectChanged()
{
    static QPointer<ProjectExplorer::Project> previousStartupProject = 0;
    ProjectExplorer::Project *project = startupProject();
    if (project == previousStartupProject)
        return;

    if (previousStartupProject) {
        disconnect(previousStartupProject, SIGNAL(activeTargetChanged(ProjectExplorer::Target*)),
                   this, SLOT(activeTargetChanged()));
    }

    previousStartupProject = project;

    if (project) {
        connect(project, SIGNAL(activeTargetChanged(ProjectExplorer::Target*)),
                this, SLOT(activeTargetChanged()));
    }

    activeTargetChanged();
    updateActions();
}

void ProjectManager::activeTargetChanged()
{
    static QPointer<ProjectExplorer::Target> previousTarget = 0;
    ProjectExplorer::Target *target = 0;
    ProjectExplorer::Project *startupProject_ = startupProject();
    if (startupProject_)
        target = startupProject_->activeTarget();
    if (target == previousTarget)
        return;

    if (previousTarget) {
        disconnect(previousTarget, SIGNAL(activeRunConfigurationChanged(ProjectExplorer::RunConfiguration*)),
                   this, SLOT(activeRunConfigurationChanged()));
    }
    previousTarget = target;
    if (target) {
        connect(target, SIGNAL(activeRunConfigurationChanged(ProjectExplorer::RunConfiguration*)),
                this, SLOT(activeRunConfigurationChanged()));
    }

    activeRunConfigurationChanged();
}

void ProjectManager::activeRunConfigurationChanged()
{
    static QPointer<ProjectExplorer::RunConfiguration> previousRunConfiguration = 0;
    ProjectExplorer::RunConfiguration *rc = 0;
    ProjectExplorer::Project *startupProject_ = startupProject();
    if (startupProject_ && startupProject_->activeTarget())
        rc = startupProject_->activeTarget()->activeRunConfiguration();
    if (rc == previousRunConfiguration)
        return;
    if (previousRunConfiguration) {
        disconnect(previousRunConfiguration, SIGNAL(requestRunActionsUpdate()),
                   this, SIGNAL(updateRunActions()));
    }
    previousRunConfiguration = rc;
    if (rc) {
        connect(rc, SIGNAL(requestRunActionsUpdate()),
                this, SIGNAL(updateRunActions()));
    }
    emit updateRunActions();
}

void ProjectManager::executeConextMenuBuildSystemCommand()
{
    if ( ProjectExplorer::Project *project = dd->m_currentProject ) {
        if ( ProjectExplorer::IProjectProvider *provider = project->projectProvider() ) {
            provider->executeBuildSystemCommand(project, dd->m_currentNode);
        }
    }
}

void ProjectManager::executeBuildSystemCommand()
{
    if ( ProjectExplorer::Project *project = ProjectManager::startupProject() ) {
        if ( ProjectExplorer::IProjectProvider *provider = project->projectProvider() ) {
            provider->executeBuildSystemCommand(project, 0);
        }
    }
}

void ProjectManager::buildProjectContextMenu()
{
    if ( ProjectExplorer::IProjectProvider *provider = dd->m_currentProject->projectProvider() )
        if ( provider->executeBuildCommand(dd->m_currentProject, dd->m_currentNode,
                                           QList<Core::Id>() << Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD)) )
            return;
    dd->queue(QList<ProjectExplorer::Project *>() <<  dd->m_currentProject,
              QList<Core::Id>() << Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD));
}

void ProjectManager::rebuildProjectContextMenu()
{
    if ( ProjectExplorer::IProjectProvider *provider = dd->m_currentProject->projectProvider() )
        if ( provider->executeBuildCommand(dd->m_currentProject, dd->m_currentNode,
                                           QList<Core::Id>() << Core::Id(ProjectExplorer::Constants::BUILDSTEPS_CLEAN)
                                                             << Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD)) )
            return;
    dd->queue(QList<ProjectExplorer::Project *>() <<  dd->m_currentProject,
              QList<Core::Id>() << Core::Id(ProjectExplorer::Constants::BUILDSTEPS_CLEAN) << Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD));
}

void ProjectManager::cleanProjectContextMenu()
{
    if ( ProjectExplorer::IProjectProvider *provider = dd->m_currentProject->projectProvider() )
        if ( provider->executeBuildCommand(dd->m_currentProject, dd->m_currentNode,
                                           QList<Core::Id>() << Core::Id(ProjectExplorer::Constants::BUILDSTEPS_CLEAN)) )
            return;
    dd->queue(QList<ProjectExplorer::Project *>() <<  dd->m_currentProject,
              QList<Core::Id>() << Core::Id(ProjectExplorer::Constants::BUILDSTEPS_CLEAN));
}

void ProjectManager::addExistingFiles()
{
    QTC_ASSERT(dd->m_currentNode, return);

    QStringList fileNames = QFileDialog::getOpenFileNames(Core::Storage::mainWindow()->widget(),
        tr("Add Existing Files"), directoryFor(dd->m_currentNode));
    if (fileNames.isEmpty())
        return;
    addExistingFiles(fileNames);
}

void ProjectManager::addExistingDirectory()
{
    QTC_ASSERT(dd->m_currentNode, return);

    ProjectExplorer::SelectableFilesDialogAddDirectory dialog(directoryFor(dd->m_currentNode), QStringList(), Core::Storage::mainWindow()->widget());

    if (dialog.exec() == QDialog::Accepted)
        addExistingFiles(dialog.selectedFiles());
}

void ProjectManager::addNewFile()
{
    QTC_ASSERT(dd->m_currentNode, return);
    QString location = directoryFor(dd->m_currentNode);

    QVariantMap map;
    map.insert(QLatin1String(ProjectExplorer::Constants::PREFERRED_PROJECT_NODE),
               QVariant::fromValue(dd->m_currentNode));
    if (dd->m_currentProject) {
        QList<Core::Id> profileIds = Utils::transform(dd->m_currentProject->targets(), &ProjectExplorer::Target::id);
        map.insert(QLatin1String(ProjectExplorer::Constants::PROJECT_KIT_IDS), QVariant::fromValue(profileIds));
    }
    Core::WizardManager::showNewItemDialog(tr("New File", "Title of dialog"),
                                           QList<Core::IWizardFactory::WizardKind>() << Core::IWizardFactory::FileWizard
                                                                                     << Core::IWizardFactory::ClassWizard,
                                           location, map);
}

void ProjectManager::addNewSubproject()
{
    QTC_ASSERT(dd->m_currentNode, return);
    QString location = directoryFor(dd->m_currentNode);

    if (dd->m_currentNode->nodeType() == ProjectExplorer::ProjectNodeType &&
            dd->m_currentNode->supportedActions(dd->m_currentNode).contains(ProjectExplorer::AddSubProject)) {
        QVariantMap map;
        map.insert(QLatin1String(ProjectExplorer::Constants::PREFERRED_PROJECT_NODE), QVariant::fromValue(dd->m_currentNode));
        if (dd->m_currentProject) {
            QList<Core::Id> profileIds = Utils::transform(dd->m_currentProject->targets(), &ProjectExplorer::Target::id);
            map.insert(QLatin1String(ProjectExplorer::Constants::PROJECT_KIT_IDS), QVariant::fromValue(profileIds));
        }

        Core::WizardManager::showNewItemDialog(tr("New Subproject", "Title of dialog"),
                                               QList<Core::IWizardFactory::WizardKind>() << Core::IWizardFactory::ProjectWizard,
                                               location, map);
    }
}

void ProjectManager::removeProject()
{
    ProjectExplorer::ProjectNode *subProjectNode = qobject_cast<ProjectExplorer::ProjectNode*>(dd->m_currentNode->projectNode());
    ProjectExplorer::ProjectNode *projectNode = qobject_cast<ProjectExplorer::ProjectNode *>(subProjectNode->parentFolderNode());
    if (projectNode) {
        Core::RemoveFileDialog removeFileDialog(subProjectNode->path(), Core::Storage::mainWindow()->widget());
        removeFileDialog.setDeleteFileVisible(false);
        if (removeFileDialog.exec() == QDialog::Accepted)
            projectNode->removeSubProjects(QStringList() << subProjectNode->path());
    }
}

void ProjectManager::removeFile()
{
    QTC_ASSERT(dd->m_currentNode && dd->m_currentNode->nodeType() == ProjectExplorer::FileNodeType, return);

    ProjectExplorer::FileNode *fileNode = qobject_cast<ProjectExplorer::FileNode*>(dd->m_currentNode);

    QString filePath = dd->m_currentNode->path();
    Core::RemoveFileDialog removeFileDialog(filePath, Core::Storage::mainWindow()->widget());

    if (removeFileDialog.exec() == QDialog::Accepted) {
        const bool deleteFile = removeFileDialog.isDeleteFileChecked();

        // remove from project
        ProjectExplorer::FolderNode *folderNode = fileNode->parentFolderNode();
        Q_ASSERT(folderNode);

        if (!folderNode->removeFiles(QStringList(filePath))) {
            Core::Storage::warning(tr("Removing File Failed.\n") +
                                   tr("Could not remove file %1 from project %2.").arg(filePath).arg(folderNode->projectNode()->displayName()));
            return;
        }

        Utils::FileUtils::removeFile(filePath, deleteFile, Core::Storage::mainWindow()->widget());
    }
}

void ProjectManager::deleteFile()
{
    QTC_ASSERT(dd->m_currentNode && dd->m_currentNode->nodeType() == ProjectExplorer::FileNodeType, return);

    ProjectExplorer::FileNode *fileNode = qobject_cast<ProjectExplorer::FileNode*>(dd->m_currentNode);

    QString filePath = dd->m_currentNode->path();
    QMessageBox::StandardButton button =
            QMessageBox::question(Core::Storage::mainWindow()->widget(),
                                  tr("Delete File"),
                                  tr("Delete %1 from file system?").arg(filePath),
                                  QMessageBox::Yes | QMessageBox::No);
    if (button != QMessageBox::Yes)
        return;

    ProjectExplorer::FolderNode *folderNode = fileNode->parentFolderNode();
    QTC_ASSERT(folderNode, return);

    folderNode->deleteFiles(QStringList(filePath));

    QFile file(filePath);
    if (file.exists()) {
        if (!file.remove())
            QMessageBox::warning(Core::Storage::mainWindow()->widget(),
                                 tr("Deleting File Failed"),
                                 tr("Could not delete file %1.").arg(filePath));
    }
}

void ProjectManager::renameFile()
{
    QWidget *focusWidget = QApplication::focusWidget();
    while (focusWidget) {
        ProjectExplorer::Internal::ProjectTreeWidget *treeWidget = qobject_cast<ProjectExplorer::Internal::ProjectTreeWidget*>(focusWidget);
        if (treeWidget) {
            treeWidget->editCurrentItem();
            return;
        }
        focusWidget = focusWidget->parentWidget();
    }
}

void ProjectManager::runProjectContextMenu()
{
    ProjectExplorer::ProjectNode *projectNode = qobject_cast<ProjectExplorer::ProjectNode*>(dd->m_currentNode);
    if (projectNode == dd->m_currentProject->rootProjectNode() || !projectNode) {
        runProject(dd->m_currentProject, ProjectExplorer::NormalRunMode);
    } else {
        QAction *act = qobject_cast<QAction *>(sender());
        if (!act)
            return;
        ProjectExplorer::RunConfiguration *rc = act->data().value<ProjectExplorer::RunConfiguration *>();
        if (!rc)
            return;
        runRunConfiguration(rc, ProjectExplorer::NormalRunMode);
    }
}

void ProjectManager::openTerminalHere()
{
    Utils::FileUtils::openTerminal(directoryFor(dd->m_currentNode), settings());
}

void ProjectManager::showInGraphicalShell()
{
    QTC_ASSERT(dd->m_currentNode, return);
    Utils::FileUtils::showInGraphicalShell(Core::Storage::mainWindow()->widget(), pathFor(dd->m_currentNode), settings());
}

void ProjectManager::searchOnFileSystem()
{
    /// TODO: !
}

void ProjectManager::unloadProject()
{
    unloadProject(dd->m_currentProject);
}

void ProjectManager::issuesPaneNavigateStateChanged()
{
    m_cmdIssuesNext->action()->setEnabled(m_issuesPane->canNext());
    m_cmdIssuesPrevious->action()->setEnabled(m_issuesPane->canPrevious());
}

void ProjectManager::issuesClear()
{
    m_issuesPane->clearContents();
}

void ProjectManager::issuesGoToNext()
{
    m_issuesPane->goToNext();
}

void ProjectManager::issuesGoToPrevious()
{
    m_issuesPane->goToPrev();
}

void ProjectManager::loadProjectProvidersPlugins()
{
    Core::LogManager::addMessage(Core::LogMessage(tr("    Project Provider Plugins Loading Started..."), tr("Plugins"), Core::LogMessage::Information, QColor(255, 255, 192)));

    m_projectProviders.clear();

    QDir pluginsDir;
    pluginsDir = QDir(qApp->applicationDirPath());
    pluginsDir.cd("projectproviders");

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        if ( QObject *plugin = loader.instance() )
            if ( ProjectExplorer::IProjectProvider *iProjectProvider = qobject_cast<ProjectExplorer::IProjectProvider *>(plugin) ) {
                m_projectProviders.append(iProjectProvider);
                Core::LogManager::addMessage(Core::LogMessage(tr("      Project Provider %1").arg(fileName), tr("Plugins"), Core::LogMessage::Information, QColor(255, 255, 192)));
            }
    }

    Core::LogManager::addMessage(Core::LogMessage(tr("    Project Provider Plugins Loaded"), tr("Plugins"), Core::LogMessage::Information, QColor(255, 255, 192)));
}

void ProjectManager::initProviders()
{
    foreach( ProjectExplorer::IProjectProvider *provider, m_projectProviders )
        provider->initialize(this);
}

void ProjectManager::delayedWizardsLoad() const
{
    QFutureInterface<void> future;
    future.setProgressRange(0, 0);
    future.reportStarted();
    Core::Storage::mainWindow()->addTask(future.future(), tr("Loading Wizards"), "ProjectManager.Load.Wizards", true, true);

    foreach (Core::IWizardFactory *cpw, ProjectExplorer::CustomWizard::createWizards())
        Core::WizardManager::registerWizardFactory(cpw);
    foreach (Core::IWizardFactory *cpw, ProjectExplorer::JsonWizardFactory::createWizardFactories())
        Core::WizardManager::registerWizardFactory(cpw);

    future.reportFinished();
}

const QStringList &ProjectManager::headerPrefixes()
{
    return m_instance->m_fileSettings->headerPrefixes;
}

const QStringList &ProjectManager::sourcePrefixes()
{
    return m_instance->m_fileSettings->sourcePrefixes;
}

const QStringList &ProjectManager::headerSearchPaths()
{
    return m_instance->m_fileSettings->headerSearchPaths;
}

const QStringList &ProjectManager::sourceSearchPaths()
{
    return m_instance->m_fileSettings->sourceSearchPaths;
}

void ProjectManager::registerMacroExpanderVariables()
{
    Utils::MacroExpander *expander = Utils::globalMacroExpander();
    expander->registerFileVariables(ProjectExplorer::Constants::VAR_CURRENTPROJECT_PREFIX,
        tr("Current project's main file"),
        [this]() -> QString {
            QString projectFilePath;
            if (ProjectExplorer::Project *project = ProjectManager::currentProject())
                projectFilePath = project->projectFilePath().toString();
            return projectFilePath;
        });

    expander->registerVariable(ProjectExplorer::Constants::VAR_CURRENTPROJECT_BUILDPATH,
        tr("Full build path of the current project's active build configuration."),
        []() -> QString {
            ProjectExplorer::BuildConfiguration *bc = activeBuildConfiguration();
            return bc ? bc->buildDirectory().toUserOutput() : QString();
        });

    expander->registerVariable(ProjectExplorer::Constants::VAR_CURRENTPROJECT_NAME,
        tr("The name of the current project."),
        [this]() -> QString {
            ProjectExplorer::Project *project = ProjectManager::currentProject();
            return project ? project->displayName() : QString();
        });

    expander->registerVariable(ProjectExplorer::Constants::VAR_CURRENTKIT_NAME,
        tr("The name of the currently active kit."),
        []() -> QString {
            ProjectExplorer::Kit *kit = currentKit();
            return kit ? kit->displayName() : QString();
        });

    expander->registerVariable(ProjectExplorer::Constants::VAR_CURRENTKIT_FILESYSTEMNAME,
        tr("The name of the currently active kit in a filesystem friendly version."),
        []() -> QString {
            ProjectExplorer::Kit *kit = currentKit();
            return kit ? kit->fileSystemFriendlyName() : QString();
        });

    expander->registerVariable(ProjectExplorer::Constants::VAR_CURRENTKIT_ID,
        tr("The id of the currently active kit."),
        []() -> QString {
            ProjectExplorer::Kit *kit = currentKit();
            return kit ? kit->id().toString() : QString();
        });

    expander->registerVariable(ProjectExplorer::Constants::VAR_CURRENTDEVICE_HOSTADDRESS,
        tr("The host address of the device in the currently active kit."),
        []() -> QString {
            ProjectExplorer::Kit *kit = currentKit();
            const ProjectExplorer::IDevice::ConstPtr device = ProjectExplorer::DeviceKitInformation::device(kit);
            return device ? device->sshParameters().host : QString();
        });

    expander->registerVariable(ProjectExplorer::Constants::VAR_CURRENTDEVICE_SSHPORT,
        tr("The SSH port of the device in the currently active kit."),
        []() -> QString {
            ProjectExplorer::Kit *kit = currentKit();
            const ProjectExplorer::IDevice::ConstPtr device = ProjectExplorer::DeviceKitInformation::device(kit);
            return device ? QString::number(device->sshParameters().port) : QString();
        });

    expander->registerVariable(ProjectExplorer::Constants::VAR_CURRENTDEVICE_USERNAME,
        tr("The username with which to log into the device in the currently active kit."),
        []() -> QString {
            ProjectExplorer::Kit *kit = currentKit();
            const ProjectExplorer::IDevice::ConstPtr device = ProjectExplorer::DeviceKitInformation::device(kit);
            return device ? device->sshParameters().userName : QString();
        });


    expander->registerVariable(ProjectExplorer::Constants::VAR_CURRENTDEVICE_PRIVATEKEYFILE,
        tr("The private key file with which to authenticate when logging into the device "
           "in the currently active kit."),
        []() -> QString {
            ProjectExplorer::Kit *kit = currentKit();
            const ProjectExplorer::IDevice::ConstPtr device = ProjectExplorer::DeviceKitInformation::device(kit);
            return device ? device->sshParameters().privateKeyFile : QString();
        });

    expander->registerVariable(ProjectExplorer::Constants::VAR_CURRENTBUILD_NAME,
        tr("The currently active build configuration's name."),
        [&]() -> QString {
            ProjectExplorer::BuildConfiguration *bc = activeBuildConfiguration();
            return bc ? bc->displayName() : QString();
        });


    expander->registerVariable(ProjectExplorer::Constants::VAR_CURRENTBUILD_TYPE,
        tr("The currently active build configuration's type."),
        [&]() -> QString {
            if (ProjectExplorer::BuildConfiguration *bc = activeBuildConfiguration()) {
                ProjectExplorer::BuildConfiguration::BuildType type = bc->buildType();
                if (type == ProjectExplorer::BuildConfiguration::Debug)
                    return tr("debug");
                if (type == ProjectExplorer::BuildConfiguration::Release)
                    return tr("release");
            }
            return tr("unknown");
        });

    expander->registerVariable("Cpp:LicenseTemplate",
                               tr("The license template."),
                               [this]() { return licenseTemplate(); });
    expander->registerFileVariables("Cpp:LicenseTemplatePath",
                                    tr("The configured path to the license template"),
                                    [this]() { return licenseTemplatePath().toString(); });

//    QString fileDescription = tr("File where current session is saved.");
//    auto fileHandler = [] { return SessionManager::sessionNameToFileName(SessionManager::activeSession()).toString(); };
//    expander->registerFileVariables("Session", fileDescription, fileHandler);
//    expander->registerFileVariables("CurrentSession", fileDescription, fileHandler, false);
}

bool ProjectManager::projectContainsFile(ProjectExplorer::Project *p, const QString &fileName) const
{
    if (!m_projectFileCache.contains(p))
        m_projectFileCache.insert(p, p->files(ProjectExplorer::Project::AllFiles));

    return m_projectFileCache.value(p).contains(fileName);
}

void ProjectManager::setCurrent(ProjectExplorer::Project *project, QString filePath, ProjectExplorer::Node *node)
{
    if (node)
        filePath = pathFor(node);
    else
        node = m_projectsTree->nodeForFile(filePath, project);

    bool projectChanged = false;
    if (dd->m_currentProject != project)
        projectChanged = true;
    dd->m_currentProject = project;

    if (projectChanged || dd->m_currentNode != node) {
        dd->m_currentNode = node;
        emit currentNodeChanged(dd->m_currentNode, project);
        dd->updateContextMenuActions();
    }
    if (projectChanged) {
        emit currentProjectChanged(project);
        updateActions();
    }
}

void ProjectManager::addProject(ProjectExplorer::Project *pro)
{
    if ( !m_projects.contains(pro) ) {
        m_projects.append(pro);
        m_sessionNode->addProjectNodes(QList<ProjectExplorer::ProjectNode *>() << pro->rootProjectNode());
        emit projectAdded(pro);
        connect(pro, SIGNAL(buildConfigurationEnabledChanged()),
                this, SLOT(updateActions()));
    }
}

void ProjectManager::createActions()
{
    QMainWindow *mainWindow = Core::Storage::mainWindow()->widget();

    m_projectBuildMenu = new QMenu(tr("&Build"), mainWindow);

    m_cmdNewProject         = new Core::Command( tr("New"),
                                                 QIcon(":/core/images/prj_new.png"),
                                                 tr("New Project"),
                                                 tr("Create New Project"),
                                                 QKeySequence(),
                                                 mainWindow );

    m_cmdOpenProject        = new Core::Command( tr("Open"),
                                                 QIcon(":/core/images/prj_open.png"),
                                                 tr("Open Project"),
                                                 tr("Open Existing Project"),
                                                 QKeySequence(),
                                                 mainWindow );

    m_cmdConfigureProjects  = new Core::Command( tr("Configure"),
                                                 QIcon(":core/images/prj_configure.png"),
                                                 tr("Configure Projects"),
                                                 tr("Configure Projects"),
                                                 QKeySequence(),
                                                 mainWindow );

    m_cmdCloseProject       = new Core::Command( tr("Close"),
                                                 QIcon(":/core/images/prj_close.png"),
                                                 tr("Close Project"),
                                                 tr("Close Current Project"),
                                                 QKeySequence(),
                                                 mainWindow );

    m_cmdBuildOptions       = new Core::Command( tr("Options"),
                                                 QIcon(":/core/images/prj_options.png"),
                                                 tr("Options"),
                                                 tr("Edit Run/Build Options"),
                                                 QKeySequence(),
                                                 mainWindow );

    m_cmdCancelBuildAction  = new Core::Command( tr("Cancel Build"),
                                                 QIcon(":/projectexplorer/images/build_cancel.png"),
                                                 tr("Cancel Build"),
                                                 tr("Cancel Build"),
                                                 QKeySequence(),
                                                 mainWindow );

    m_cmdMiniProjectTargetSelector = new Core::Command( tr("MiniProjectTargetSelector"),
                                                        QIcon(),
                                                        tr("Activate Project/Target"),
                                                        tr("Activate Project/Target"),
                                                        QKeySequence(tr("Ctrl+T")),
                                                        mainWindow );

    connect(m_cmdNewProject->action(),          SIGNAL(triggered()), this, SLOT(newProject()));
    connect(m_cmdOpenProject->action(),         SIGNAL(triggered()), this, SLOT(openProject()));
    connect(m_cmdConfigureProjects->action(),   SIGNAL(triggered()), this, SLOT(configureProjects()));
    connect(m_cmdCloseProject->action(),        SIGNAL(triggered()), this, SLOT(closeProject()));
    connect(m_cmdBuildOptions->action(),        SIGNAL(triggered()), this, SLOT(buildOptions()));
    connect(m_cmdCancelBuildAction->action(),   SIGNAL(triggered()), this, SLOT(cancelBuild()));

    m_cmdMiniProjectTargetSelector->action()->setCheckable(true);
    m_cmdMiniProjectTargetSelector->action()->setEnabled(false);

    m_recentProjectMenu = new RecentFilesMenu( Core::Storage::mainWindow()->orgName(),
                                               Core::Storage::mainWindow()->appName(),
                                               new ProjectRecentFilesDelagate(this) );
}

void ProjectManager::fillProjectTreeContextMenus()
{
    QWidget *mainWindow = Core::Storage::mainWindow()->widget();

    dd->m_projectMenu = new QMenu(mainWindow);
    dd->m_subProjectMenu = new QMenu(mainWindow);
    dd->m_folderMenu = new QMenu(mainWindow);
    dd->m_fileMenu = new QMenu(mainWindow);
    dd->m_runContextMenu = new QMenu(tr("Run"), mainWindow);

    dd->m_setStartupProjectAction = new Utils::ParameterAction(tr("Set as Active Project"),
                                                               tr("Set \"%1\" as Active Project"),
                                                               Utils::ParameterAction::AlwaysEnabled, this);
    dd->m_buildActionContextMenu = new QAction(tr("Build"), mainWindow);
    dd->m_rebuildActionContextMenu = new QAction(tr("Rebuild"), mainWindow);
    dd->m_cleanActionContextMenu = new QAction(tr("Clean"), mainWindow);
    dd->m_addExistingFilesAction = new QAction(tr("Add Existing Files..."), mainWindow);
    dd->m_addExistingDirectoryAction = new QAction(tr("Add Existing Directory..."), mainWindow);
    dd->m_addNewFileAction = new QAction(tr("Add New..."), mainWindow);
    dd->m_addNewSubprojectAction = new QAction(tr("New Subproject..."), mainWindow);
    dd->m_removeProjectAction = new QAction(tr("Remove Project..."), mainWindow);
    dd->m_removeFileAction = new QAction(tr("Remove File..."), mainWindow);
    dd->m_deleteFileAction = new QAction(tr("Delete File..."), mainWindow);
    dd->m_renameFileAction = new QAction(tr("Rename..."), mainWindow);
    dd->m_runActionContextMenu = new QAction(QIcon(QLatin1String(ProjectExplorer::Constants::ICON_RUN)),
                                             tr("Run"), mainWindow);
    dd->m_openTerminalHere = new QAction(Utils::FileUtils::msgTerminalAction(), mainWindow);
    dd->m_showInGraphicalShell = new QAction(Utils::FileUtils::msgGraphicalShellAction(), mainWindow);
    dd->m_searchOnFileSystem = new QAction(Utils::FileUtils::msgFindInDirectory(), mainWindow);
    dd->m_unloadActionContextMenu = new Utils::ParameterAction(tr("Close Project"), tr("Close Project \"%1\""),
                                                               Utils::ParameterAction::EnabledWithParameter, mainWindow);

    dd->m_projectMenu->addAction(dd->m_setStartupProjectAction);
    dd->m_projectMenu->addSeparator();
    dd->m_projectMenu->addAction(dd->m_buildActionContextMenu);
    dd->m_projectMenu->addAction(dd->m_runActionContextMenu);
    dd->m_projectMenu->addMenu(dd->m_runContextMenu);
    dd->m_projectMenu->addAction(dd->m_buildSystemContextMenuAction);
    dd->m_projectMenu->addSeparator();
    dd->m_projectMenu->addAction(dd->m_rebuildActionContextMenu);
    dd->m_projectMenu->addAction(dd->m_cleanActionContextMenu);
    dd->m_projectMenu->addSeparator();
    dd->m_projectMenu->addAction(dd->m_addExistingFilesAction);
    dd->m_projectMenu->addAction(dd->m_addExistingDirectoryAction);
    dd->m_projectMenu->addAction(dd->m_addNewFileAction);
    dd->m_projectMenu->addAction(dd->m_addNewSubprojectAction);
    dd->m_projectMenu->addSeparator();
    dd->m_projectMenu->addAction(dd->m_searchOnFileSystem);
    dd->m_projectMenu->addSeparator();
    dd->m_projectMenu->addAction(dd->m_unloadActionContextMenu);

    dd->m_subProjectMenu->addAction(dd->m_buildActionContextMenu);
    dd->m_subProjectMenu->addAction(dd->m_runActionContextMenu);
    dd->m_subProjectMenu->addAction(dd->m_buildSystemContextMenuAction);
    dd->m_subProjectMenu->addSeparator();
    dd->m_subProjectMenu->addAction(dd->m_rebuildActionContextMenu);
    dd->m_subProjectMenu->addAction(dd->m_cleanActionContextMenu);
    dd->m_subProjectMenu->addSeparator();
    dd->m_subProjectMenu->addAction(dd->m_addExistingFilesAction);
    dd->m_subProjectMenu->addAction(dd->m_addExistingDirectoryAction);
    dd->m_subProjectMenu->addAction(dd->m_addNewFileAction);
    dd->m_subProjectMenu->addAction(dd->m_addNewSubprojectAction);
    dd->m_subProjectMenu->addSeparator();
    dd->m_subProjectMenu->addAction(dd->m_removeProjectAction);
    dd->m_subProjectMenu->addAction(dd->m_searchOnFileSystem);

    dd->m_folderMenu->addAction(dd->m_addExistingFilesAction);
    dd->m_folderMenu->addAction(dd->m_addExistingDirectoryAction);
    dd->m_folderMenu->addAction(dd->m_addNewFileAction);
    dd->m_projectMenu->addSeparator();
    dd->m_folderMenu->addAction(dd->m_openTerminalHere);
    dd->m_folderMenu->addAction(dd->m_showInGraphicalShell);
    dd->m_folderMenu->addAction(dd->m_searchOnFileSystem);

    dd->m_fileMenu->addAction(dd->m_removeFileAction);
    dd->m_fileMenu->addAction(dd->m_deleteFileAction);
    dd->m_fileMenu->addAction(dd->m_renameFileAction);
    dd->m_projectMenu->addSeparator();
    dd->m_fileMenu->addAction(dd->m_openTerminalHere);
    dd->m_fileMenu->addAction(dd->m_showInGraphicalShell);
    dd->m_fileMenu->addAction(dd->m_searchOnFileSystem);

    connect(dd->m_setStartupProjectAction, SIGNAL(triggered()), this, SLOT(setStartupProject()));
    connect(dd->m_buildActionContextMenu, SIGNAL(triggered()), this, SLOT(buildProjectContextMenu()));
    connect(dd->m_addExistingFilesAction, SIGNAL(triggered()), this, SLOT(addExistingFiles()));
    connect(dd->m_addExistingDirectoryAction, SIGNAL(triggered()), this, SLOT(addExistingDirectory()));
    connect(dd->m_addNewFileAction, SIGNAL(triggered()), this, SLOT(addNewFile()));
    connect(dd->m_addNewSubprojectAction, SIGNAL(triggered()), this, SLOT(addNewSubproject()));
    connect(dd->m_removeProjectAction, SIGNAL(triggered()), this, SLOT(removeProject()));
    connect(dd->m_removeFileAction, SIGNAL(triggered()), this, SLOT(removeFile()));
    connect(dd->m_deleteFileAction, SIGNAL(triggered()), this, SLOT(deleteFile()));
    connect(dd->m_renameFileAction, SIGNAL(triggered()), this, SLOT(renameFile()));
    connect(dd->m_runActionContextMenu, SIGNAL(triggered()), this, SLOT(runProjectContextMenu()));
    connect(dd->m_rebuildActionContextMenu, SIGNAL(triggered()), this, SLOT(rebuildProjectContextMenu()));
    connect(dd->m_cleanActionContextMenu, SIGNAL(triggered()), this, SLOT(cleanProjectContextMenu()));
    connect(dd->m_openTerminalHere, SIGNAL(triggered()), this, SLOT(openTerminalHere()));
    connect(dd->m_showInGraphicalShell, SIGNAL(triggered()), this, SLOT(showInGraphicalShell()));
    connect(dd->m_searchOnFileSystem, SIGNAL(triggered()), this, SLOT(searchOnFileSystem()));
    connect(dd->m_unloadActionContextMenu, SIGNAL(triggered()), this, SLOT(unloadProject()));
}

void ProjectManager::fillBuildMenus()
{
    QWidget *mainWindow = Core::Storage::mainWindow()->widget();

    dd->m_buildSystemAction = new QAction(tr("...make"), mainWindow);
    connect(dd->m_buildSystemAction, SIGNAL(triggered()), this, SLOT(executeBuildSystemCommand()));

    dd->m_buildSystemContextMenuAction = new QAction(tr("...make"), mainWindow);
    connect(dd->m_buildSystemContextMenuAction, SIGNAL(triggered()), this, SLOT(executeConextMenuBuildSystemCommand()));

    dd->m_buildAction = new Utils::ParameterAction(tr("Build Project"), tr("Build Project \"%1\""), Utils::ParameterAction::AlwaysEnabled, mainWindow);
    dd->m_buildAction->setIcon(QIcon(ProjectExplorer::Constants::ICON_BUILD_SMALL));
    connect(dd->m_buildAction, SIGNAL(triggered()), this, SLOT(buildProject()));

    dd->m_buildSessionAction = new QAction(QIcon(ProjectExplorer::Constants::ICON_BUILD_SMALL), tr("Build All"), mainWindow);
    connect(dd->m_buildSessionAction, SIGNAL(triggered()), this, SLOT(buildSession()));

    dd->m_rebuildSessionAction = new QAction(QIcon(ProjectExplorer::Constants::ICON_REBUILD_SMALL), tr("Rebuild All"), this);
    connect(dd->m_rebuildSessionAction, SIGNAL(triggered()), this, SLOT(rebuildSession()));

    dd->m_rebuildAction = new Utils::ParameterAction(tr("Rebuild Project"), tr("Rebuild Project \"%1\""), Utils::ParameterAction::AlwaysEnabled, mainWindow);
    connect(dd->m_rebuildAction, SIGNAL(triggered()), this, SLOT(rebuildProject()));

    dd->m_cleanSessionAction = new QAction(QIcon(ProjectExplorer::Constants::ICON_CLEAN_SMALL), tr("Clean All"), mainWindow);
    connect(dd->m_cleanSessionAction, SIGNAL(triggered()), this, SLOT(cleanSession()));

    dd->m_cleanAction = new Utils::ParameterAction(tr("Clean Project"), tr("Clean Project \"%1\""), Utils::ParameterAction::AlwaysEnabled, mainWindow);
    connect(dd->m_cleanAction, SIGNAL(triggered()), this, SLOT(cleanProject()));

    dd->m_runAction = new QAction(QIcon(ProjectExplorer::Constants::ICON_RUN_SMALL), tr("Run"), mainWindow);
    connect(dd->m_runAction, SIGNAL(triggered()), this, SLOT(runProject()));
    dd->m_runAction->setEnabled(false);

    dd->m_cancelBuildAction = m_cmdCancelBuildAction->action();

    dd->m_projectTreeCollapseAllAction = new QAction(tr("Collapse All"), mainWindow);

    m_projectBuildMenu->addAction(dd->m_buildSessionAction);
    m_projectBuildMenu->addAction(dd->m_buildAction);
    m_projectBuildMenu->addAction(dd->m_buildSystemAction);
    m_projectBuildMenu->addSeparator();
    m_projectBuildMenu->addAction(dd->m_rebuildSessionAction);
    m_projectBuildMenu->addAction(dd->m_rebuildAction);
    m_projectBuildMenu->addSeparator();
    m_projectBuildMenu->addAction(dd->m_cleanSessionAction);
    m_projectBuildMenu->addAction(dd->m_cleanAction);
    m_projectBuildMenu->addSeparator();
    m_projectBuildMenu->addAction(m_cmdCancelBuildAction->action());
    m_projectBuildMenu->addSeparator();
    m_projectBuildMenu->addAction(dd->m_runAction);

    Core::Command *cmdBuildSession          = new Core::Command( dd->m_buildSessionAction,
                                                                 tr("BuildAll"),
                                                                 QKeySequence(tr("Ctrl+Shift+B")),
                                                                 mainWindow );

    Core::Command *cmdBuild                 = new Core::Command( dd->m_buildAction,
                                                                 tr("Build"),
                                                                 QKeySequence(tr("Ctrl+B")),
                                                                 mainWindow );

    Core::Command *cmdRebuildSession        = new Core::Command( dd->m_rebuildSessionAction,
                                                                 tr("RebuildAll"),
                                                                 QKeySequence(),
                                                                 mainWindow );

    Core::Command *cmdRebuild               = new Core::Command( dd->m_rebuildAction,
                                                                 tr("Rebuild"),
                                                                 QKeySequence(),
                                                                 mainWindow );

    Core::Command *cmdCleanSession          = new Core::Command( dd->m_cleanSessionAction,
                                                                 tr("CleanAll"),
                                                                 QKeySequence(),
                                                                 mainWindow );

    Core::Command *cmdClean                 = new Core::Command( dd->m_cleanAction,
                                                                 tr("Clean"),
                                                                 QKeySequence(),
                                                                 mainWindow );

    Core::Command *cmdRun                   = new Core::Command( dd->m_runAction,
                                                                 tr("Run"),
                                                                 QKeySequence(tr("Ctrl+R")),
                                                                 mainWindow );

    connect(this, SIGNAL(startupProjectChanged(ProjectExplorer::Project*)), this, SLOT(slotStartupProjectChanged()));
    connect(this, SIGNAL(updateRunActions()), this, SLOT(slotUpdateRunActions()));

    Core::CommandManager::getInstance()->registerCommands( tr("ProjectCommands"),
                                                            QList<Core::Command *>() << cmdBuildSession
                                                                                     << cmdBuild
                                                                                     << cmdRebuildSession
                                                                                     << cmdRebuild
                                                                                     << cmdCleanSession
                                                                                     << cmdClean
                                                           << cmdRun );
}

Utils::FileName ProjectManager::licenseTemplatePath()
{ return Utils::FileName::fromString(m_fileSettings->licenseTemplatePath); }

QString ProjectManager::licenseTemplate(const QString &file, const QString &className)
{ return m_fileSettings->licenseTemplate(file, className); }

ProjectExplorer::Project *ProjectManager::currentProject()
{ return dd->m_currentProject; }

ProjectExplorer::Node *ProjectManager::currentNode()
{ return dd->m_currentNode; }

void ProjectManager::setCurrentNode(ProjectExplorer::Node *node)
{ m_instance->setCurrent(projectForNode(node), QString(), node); }

const QList<ProjectExplorer::Project *> &ProjectManager::projects()
{ return m_instance->m_projects; }

QString ProjectManager::displayNameForStepId(Core::Id stepId)
{
    if (stepId == ProjectExplorer::Constants::BUILDSTEPS_CLEAN)
        return tr("Clean");
    if (stepId == ProjectExplorer::Constants::BUILDSTEPS_BUILD)
        return tr("Build", "Build step");
    if (stepId == ProjectExplorer::Constants::BUILDSTEPS_DEPLOY)
        return tr("Deploy");
    return tr("Build", "Build step");
}

bool ProjectManager::saveModifiedFiles()
{
    if ( dd->m_projectExplorerSettings.saveBeforeBuild )
        return DocumentManager::getInstance()->silentSaveAllDocuments();

    return DocumentManager::getInstance()->tryConfirmSaveAllDocuments(true);
}

bool ProjectManager::hasDependency(const ProjectExplorer::Project *project, const ProjectExplorer::Project *depProject)
{
    const QString proName = project->projectFilePath().toString();
    const QString depName = depProject->projectFilePath().toString();

    const QStringList proDeps = m_instance->m_depMap.value(proName);
    return proDeps.contains(depName);
}

bool ProjectManager::addDependency(ProjectExplorer::Project *project, ProjectExplorer::Project *depProject)
{
    const QString proName = project->projectFilePath().toString();
    const QString depName = depProject->projectFilePath().toString();

    // check if this dependency is valid
    if (!m_instance->recursiveDependencyCheck(proName, depName))
        return false;

    QStringList proDeps = m_instance->m_depMap.value(proName);
    if (!proDeps.contains(depName)) {
        proDeps.append(depName);
        m_instance->m_depMap[proName] = proDeps;
    }
    emit m_instance->dependencyChanged(project, depProject);

    return true;
}

bool ProjectManager::recursiveDependencyCheck(const QString &newDep, const QString &checkDep) const
{
    if (newDep == checkDep)
        return false;

    foreach (const QString &dependency, m_depMap.value(checkDep))
        if (!recursiveDependencyCheck(newDep, dependency))
            return false;

    return true;
}

void ProjectManager::removeDependency(ProjectExplorer::Project *project, ProjectExplorer::Project *depProject)
{
    const QString proName = project->projectFilePath().toString();
    const QString depName = depProject->projectFilePath().toString();

    QStringList proDeps = m_instance->m_depMap.value(proName);
    proDeps.removeAll(depName);
    if (proDeps.isEmpty())
        m_instance->m_depMap.remove(proName);
    else
        m_instance->m_depMap[proName] = proDeps;
    emit m_instance->dependencyChanged(project, depProject);
}

void ProjectManager::runProject(ProjectExplorer::Project *pro, ProjectExplorer::RunMode mode, const bool forceSkipDeploy)
{
    if (!pro)
        return;

    if (ProjectExplorer::Target *target = pro->activeTarget())
        if (ProjectExplorer::RunConfiguration *rc = target->activeRunConfiguration())
            runRunConfiguration(rc, mode, forceSkipDeploy);
}

void ProjectManager::runRunConfiguration(ProjectExplorer::RunConfiguration *rc, ProjectExplorer::RunMode runMode, const bool forceSkipDeploy)
{
    if (!rc->isEnabled())
        return;

    QList<Core::Id> stepIds;
    if (!forceSkipDeploy && dd->m_projectExplorerSettings.deployBeforeRun) {
        if (dd->m_projectExplorerSettings.buildBeforeDeploy)
            stepIds << Core::Id(ProjectExplorer::Constants::BUILDSTEPS_BUILD);
        stepIds << Core::Id(ProjectExplorer::Constants::BUILDSTEPS_DEPLOY);
    }

    ProjectExplorer::Project *pro = rc->target()->project();
    int queueCount = dd->queue(projectOrder(pro), stepIds);

    if (queueCount < 0) // something went wrong
        return;

    if (queueCount > 0) {
        // delay running till after our queued steps were processed
        dd->m_runMode = runMode;
        dd->m_delayedRunConfiguration = rc;
        dd->m_shouldHaveRunConfiguration = true;
    } else {
        dd->executeRunConfiguration(rc, runMode);
    }
    emit updateRunActions();
}

void ProjectManager::showRunErrorMessage(const QString &errorMessage)
{
    QString errorPrefix = errorMessage.isNull() ? tr("Unknown error") : tr("Could Not Run");
    Core::Storage::information(errorPrefix + "\n" + errorMessage);
}

QList<ProjectExplorer::IBuildStepFactory *> ProjectManager::buildStepFactories()
{ return m_instance->m_buildStepFactories; }

void ProjectManager::registerBuildStepFactory(ProjectExplorer::IBuildStepFactory *factory)
{ m_buildStepFactories << factory; }

QList<ProjectExplorer::IBuildConfigurationFactory *> ProjectManager::buildConfigurationFactories()
{ return m_instance->m_buildConfigurationFactories; }

void ProjectManager::registerBuildConfigurationFactory(ProjectExplorer::IBuildConfigurationFactory *factory)
{ m_buildConfigurationFactories << factory; }

QList<ProjectExplorer::IDeviceFactory *> ProjectManager::deviceFactories()
{ return m_instance->m_deviceFactories; }

QList<ProjectExplorer::DeployConfigurationFactory *> ProjectManager::deployConfigurationFactories()
{ return m_instance->m_deployConfigurationFactories; }

QList<ProjectExplorer::IRunControlFactory *> ProjectManager::runControlFactories()
{ return m_instance->m_runControlFactories; }

QList<ProjectExplorer::IRunConfigurationFactory *> ProjectManager::runConfigurationFactories()
{ return m_instance->m_runConfigurationFactories; }

void ProjectManager::registerRunConfigurationFactory(ProjectExplorer::IRunConfigurationFactory *factory)
{ m_runConfigurationFactories << factory; }

QList<ProjectExplorer::ToolChainFactory *> ProjectManager::toolChainFactories()
{ return m_instance->m_toolChainFactories; }

QList<ProjectExplorer::ICustomWizardMetaFactory *> ProjectManager::customWizardMetaFactories()
{ return m_instance->m_customWizardMetaFactories; }

void ProjectManager::registerCustomWizardMetaFactory(ProjectExplorer::ICustomWizardMetaFactory *factory)
{ m_instance->m_customWizardMetaFactories << factory; }

ProjectExplorer::ITaskHandler *ProjectManager::taskHandler()
{ return m_instance->m_taskHandler; }

QSettings *ProjectManager::settings()
{ return m_instance->m_settings; }

void ProjectManager::saveSettings()
{
    m_instance->savePersistentSettings();
    emit m_instance->saveSettingsRequested();
    m_instance->m_settings->sync();
}

QList<Core::IOptionsPage *> ProjectManager::optionPages()
{ return m_instance->m_optionPages; }

void ProjectManager::registerOptionPage(Core::IOptionsPage *page)
{ m_instance->m_optionPages << page; }

ProjectExplorer::KitOptionsPage *ProjectManager::kitOptionsPage()
{
    return m_instance->m_kitOptionsPage;
}

bool ProjectManager::autoRunBuildSystemCommand()
{ return dd->m_projectExplorerSettings.autoRunBuildSystemCommand; }

ProjectExplorer::Project *ProjectManager::openProjectFilePath(const QString &fileName)
{
    QFileInfo fi = QFileInfo(fileName);
    QString filePath = fileName;
    if (fi.exists()) // canonicalFilePath will be empty otherwise!
        filePath = fi.canonicalFilePath();

    ProjectExplorer::Project *pro = openProjectInternal(filePath);
    if ( pro ) {
        // Make sure we always have a current project / node
        if (!dd->m_currentProject)
            setCurrentNode(pro->rootProjectNode());
        m_recentProjectMenu->appendItem(filePath);
        setStartupProject(pro);
    }

    updateActions();

    return pro;
}

bool ProjectManager::canRun(ProjectExplorer::Project *project, ProjectExplorer::RunMode runMode, QString *whyNot)
{
    if (!project) {
        if (whyNot)
            *whyNot = tr("No active project.");
        return false;
    }

    if (project->needsConfiguration()) {
        if (whyNot)
            *whyNot = tr("The project \"%1\" is not configured.").arg(project->displayName());
        return false;
    }

    ProjectExplorer::Target *target = project->activeTarget();
    if (!target) {
        if (whyNot)
            *whyNot = tr("The project \"%1\" has no active kit.").arg(project->displayName());
        return false;
    }

    ProjectExplorer::RunConfiguration *activeRC = target->activeRunConfiguration();
    if (!activeRC) {
        if (whyNot)
            *whyNot = tr("The kit \"%1\" for the project \"%2\" has no active run configuration.")
                .arg(target->displayName(), project->displayName());
        return false;
    }

    if (!activeRC->isEnabled()) {
        if (whyNot)
            *whyNot = activeRC->disabledReason();
        return false;
    }


    if (dd->m_projectExplorerSettings.buildBeforeDeploy
            && dd->m_projectExplorerSettings.deployBeforeRun
            && hasBuildSettings(project)) {
        QPair<bool, QString> buildState = dd->buildSettingsEnabled(project);
        if (!buildState.first) {
            if (whyNot)
                *whyNot = buildState.second;
            return false;
        }
    }

    // shouldn't actually be shown to the user...
    if (!findRunControlFactory(activeRC, runMode)) {
        if (whyNot)
            *whyNot = tr("Cannot run \"%1\".").arg(activeRC->displayName());
        return false;
    }

    if (ProjectExplorer::BuildManager::isBuilding()) {
        if (whyNot)
            *whyNot = tr("A build is still in progress.");
        return false;
    }

    return true;
}

void ProjectManager::addExistingFiles(ProjectExplorer::FolderNode *folderNode, const QStringList &filePaths)
{
    if (!folderNode) // can happen when project is not yet parsed
        return;

    QStringList fileNames = filePaths;
    QStringList notAdded;
    folderNode->addFiles(fileNames, &notAdded);

    if (!notAdded.isEmpty()) {
        QString message = tr("Could not add following files to project %1:").arg(folderNode->projectNode()->displayName());
        message += QLatin1Char('\n');
        QString files = notAdded.join(QLatin1Char('\n'));
        Core::Storage::warning(tr("Adding Files to Project Failed:\n") + message + files);
        foreach (const QString &file, notAdded)
            fileNames.removeOne(file);
    }
}

void ProjectManager::addExistingFiles(const QStringList &filePaths)
{
    ProjectExplorer::FolderNode *folderNode = qobject_cast<ProjectExplorer::FolderNode *>(dd->m_currentNode);
    addExistingFiles(folderNode, filePaths);
}

QList<ProjectExplorer::IProjectProvider *> ProjectManager::projectProviders()
{ return m_instance->m_projectProviders; }

ProjectExplorer::SessionNode *ProjectManager::sessionNode()
{ return m_instance->m_sessionNode; }

ProjectExplorer::Project *ProjectManager::startupProject()
{ return m_instance->m_startupProject; }

void ProjectManager::setStartupProject(ProjectExplorer::Project *startupProject)
{
    if ( m_instance->m_startupProject == startupProject )
        return;

    if ( startupProject )
        Q_ASSERT(m_instance->m_projects.contains(startupProject));

    m_instance->m_startupProject = startupProject;
    emit m_instance->startupProjectChanged(startupProject);

    m_instance->updateActions();
}

QList<ProjectExplorer::Node *> ProjectManager::nodesForFile(const QString &fileName, ProjectExplorer::Project *project)
{
    if (!project)
        project = projectForFile(fileName);

    if (project) {
        ProjectExplorer::FindNodesForFileVisitor findNodes(fileName);
        project->rootProjectNode()->accept(&findNodes);
        return findNodes.nodes();
    }

    return QList<ProjectExplorer::Node *>();
}

ProjectExplorer::Node *ProjectManager::nodeForFile(const QString &fileName, ProjectExplorer::Project *project)
{
    ProjectExplorer::Node *node = 0;
    foreach (ProjectExplorer::Node *n, nodesForFile(fileName, project)) {
        // prefer file nodes
        if (!node || (node->nodeType() != ProjectExplorer::FileNodeType && n->nodeType() == ProjectExplorer::FileNodeType))
            node = n;
    }
    return node;
}

ProjectExplorer::Project *ProjectManager::projectForFile(const QString &fileName)
{
    const QList<ProjectExplorer::Project *> &projectList = projects();

    // Check current project first
    ProjectExplorer::Project *currProject = currentProject();
    if (currProject && m_instance->projectContainsFile(currProject, fileName))
        return currProject;

    foreach (ProjectExplorer::Project *p, projectList)
        if (p != currProject && m_instance->projectContainsFile(p, fileName))
            return p;

    return 0;
}

ProjectExplorer::Project *ProjectManager::projectForNode(ProjectExplorer::Node *node)
{
    if (!node)
        return 0;

    ProjectExplorer::FolderNode *rootProjectNode = qobject_cast<ProjectExplorer::FolderNode*>(node);
    if (!rootProjectNode)
        rootProjectNode = node->parentFolderNode();

    while (rootProjectNode && rootProjectNode->parentFolderNode() != m_instance->m_sessionNode)
        rootProjectNode = rootProjectNode->parentFolderNode();

    Q_ASSERT(rootProjectNode);

    return Utils::findOrDefault(m_instance->m_projects, Utils::equal(&ProjectExplorer::Project::rootProjectNode, rootProjectNode));
}

QString ProjectManager::directoryFor(ProjectExplorer::Node *node)
{
    return pathOrDirectoryFor(node, true);
}

}   // namespace ProjectExplorer
