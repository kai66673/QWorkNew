
#ifndef PROJECTEXPLORER_H
#define PROJECTEXPLORER_H

#include <QObject>
#include <QSettings>
#include <QSharedPointer>

#include "projectexplorer_export.h"

#include "IPlugin.h"
#include "ISessionClient.h"
#include "iprojectprovider.h"
#include "project.h"
#include "projectnodes.h"
#include "projecttreewidget.h"
#include "projectexplorersettings.h"
#include "buildstep.h"
#include "buildconfiguration.h"
#include "itaskhandler.h"
#include "idevicefactory.h"
#include "deployconfiguration.h"
#include "runconfiguration.h"
#include "toolchain.h"
#include "kitmanager.h"
#include "ioptionspage.h"
#include "devicemanager.h"
#include "qtversionmanager.h"
#include "windebuginterface.h"
#include "WizardManager.h"
#include "customwizard/customwizard.h"
#include "CloseApplicationHandler.h"
#include "ioutputpane.h"

class RecentFilesMenu;

namespace Core {
class JsExpander;
}

namespace QtSupport {
class UiCodeModelManager;
}

namespace CppTools {
namespace Internal {
struct CppFileSettings;
}
}

namespace Python {
class ProjectManagerPythonModule;
class Project;
}

namespace ProjectExplorer {

class ToolChainManager;
class KitOptionsPage;

class PROJECTEXPLORER_EXPORT ProjectManager: public IPlugin, public ISessionClient,
                                             public ICloseApplicationClient, public Core::IDelayedWizardsLoader
{
    Q_OBJECT
    friend class Python::ProjectManagerPythonModule;
    friend class Python::Project;

public:
    ProjectManager();
    virtual ~ProjectManager();
    static ProjectManager *instance();
    
    static void unloadProject(ProjectExplorer::Project *project);

    // IPlugin implementation
    virtual QString id() const { return QStringLiteral("ProjectManager"); }
    virtual QMenu *menu();

    virtual void initialize();
    virtual QList<UiProviderInterface *> createUiProviders( IMainWindow *mainWindow );

    // ISessionClient implementation
    virtual bool sessionAboutToClose();
    virtual void sessionClose() {}
    virtual void saveSession( const QString &location );
    virtual void loadSession( const QString &location );

    // ICloseApplicationClient
    virtual bool applicationAboutToClose();
    virtual void applicationClose();

    static void showOptionsDialog(const Core::Id &categoryId = Core::Id(),
                                  const Core::Id &pageId = Core::Id(),
                                  QWidget *parent = 0);
    static ProjectExplorer::Internal::ProjectExplorerSettings projectExplorerSettings();
    static void setProjectExplorerSettings(const ProjectExplorer::Internal::ProjectExplorerSettings &settings);
    static QString projectsDirectory();
    static void setProjectsDirectory(const QString &dir);
    static bool useProjectsDirectory();
    static void setUseProjectsDirectory(bool onoff);
    static QString buildDirectory();
    static void setBuildDirectory(const QString &dir);
    static ProjectExplorer::SessionNode *sessionNode();
    static ProjectExplorer::Project *startupProject();
    static void setStartupProject(ProjectExplorer::Project *startupProject);
    static QList<ProjectExplorer::Node *> nodesForFile(const QString &fileName, ProjectExplorer::Project *project = 0);
    static ProjectExplorer::Node *nodeForFile(const QString &fileName, ProjectExplorer::Project *project = 0);
    static ProjectExplorer::Project *projectForFile(const QString &fileName);
    static ProjectExplorer::Project *projectForNode(ProjectExplorer::Node *node);
    static QString directoryFor(ProjectExplorer::Node *node);
    static ProjectExplorer::Project *currentProject();
    static ProjectExplorer::Node *currentNode();
    static void setCurrentNode(ProjectExplorer::Node *node);
    static const QList<ProjectExplorer::Project *> &projects();
    static QString displayNameForStepId(Core::Id stepId);
    static bool saveModifiedFiles();
    static bool hasDependency(const ProjectExplorer::Project *project, const ProjectExplorer::Project *depProject);
    static bool addDependency(ProjectExplorer::Project *project, ProjectExplorer::Project *depProject);
    bool recursiveDependencyCheck(const QString &newDep, const QString &checkDep) const;
    static void removeDependency(ProjectExplorer::Project *project, ProjectExplorer::Project *depProject);

    void runProject(ProjectExplorer::Project *pro, ProjectExplorer::RunMode, const bool forceSkipDeploy = false);
    void runRunConfiguration(ProjectExplorer::RunConfiguration *rc, ProjectExplorer::RunMode runMode,
                             const bool forceSkipDeploy = false);

    static void showRunErrorMessage(const QString &errorMessage);
    QList<ProjectExplorer::Project *> projectOrder(ProjectExplorer::Project *project = 0);
    static void showContextMenu(QWidget *view, const QPoint &globalPos, ProjectExplorer::Node *node);

    static void renameFile(ProjectExplorer::Node *node, const QString &to);

    static QList<ProjectExplorer::IBuildStepFactory *> buildStepFactories();
    void registerBuildStepFactory(ProjectExplorer::IBuildStepFactory *factory);
    static QList<ProjectExplorer::IBuildConfigurationFactory *> buildConfigurationFactories();
    void registerBuildConfigurationFactory(ProjectExplorer::IBuildConfigurationFactory *factory);
    static QList<ProjectExplorer::IDeviceFactory *> deviceFactories();
    static QList<ProjectExplorer::DeployConfigurationFactory *> deployConfigurationFactories();
    static QList<ProjectExplorer::IRunControlFactory *> runControlFactories();
    static QList<ProjectExplorer::IRunConfigurationFactory *> runConfigurationFactories();
    void registerRunConfigurationFactory(ProjectExplorer::IRunConfigurationFactory *factory);
    static QList<ProjectExplorer::ToolChainFactory *> toolChainFactories();
    static QList<ProjectExplorer::ICustomWizardMetaFactory *> customWizardMetaFactories();
    static void registerCustomWizardMetaFactory(ProjectExplorer::ICustomWizardMetaFactory *factory);
    static ProjectExplorer::ITaskHandler *taskHandler();
    static QSettings *settings();
    static void saveSettings();
    static QList<Core::IOptionsPage *> optionPages();
    static void registerOptionPage(Core::IOptionsPage *page);
    static ProjectExplorer::KitOptionsPage *kitOptionsPage();
    static bool autoRunBuildSystemCommand();

    ProjectExplorer::Project *openProjectFilePath(const QString &fileName);
    static bool canRun(ProjectExplorer::Project *pro, ProjectExplorer::RunMode runMode, QString *whyNot = 0);

    static void addExistingFiles(ProjectExplorer::FolderNode *projectNode, const QStringList &filePaths);
    static void addExistingFiles(const QStringList &filePaths);

    static QList<ProjectExplorer::IProjectProvider *> projectProviders();
    QString licenseTemplate(const QString &file = QString(), const QString &className = QString());

    // IDelayedWizardsLoader implementation
    virtual void delayedWizardsLoad() const;

    static const QStringList &headerPrefixes();
    static const QStringList &sourcePrefixes();
    static const QStringList &headerSearchPaths();
    static const QStringList &sourceSearchPaths();

protected:
    // ISettingsFactory implementation
    virtual QStringList settingsKeyList() const;
    virtual ISettings *createSettings( const QString & ) const;

    void savePersistentSettings();
    void restorePersistentSettings();

public slots:
    void reportError(const QString &message);
    static void requestProjectModeUpdate(ProjectExplorer::Project *p);

private:
    void buildProjectNode(ProjectExplorer::ProjectNode *projectNode);
    void rebuildProjectNode(ProjectExplorer::ProjectNode *projectNode);
    void cleanProjectNode(ProjectExplorer::ProjectNode *projectNode);
    void runProjectNode(ProjectExplorer::ProjectNode *projectNode);
    void executeBuildSystemCommandForNode(ProjectExplorer::ProjectNode *projectNode);
    void closeProjectNode(ProjectExplorer::ProjectNode *projectNode);

private slots:
    void newProject();
    void openProject();
    void configureProjects();
    void closeProject();
    void buildOptions();
    void clearProjectFileCache();
    void setStartupProject();

    void buildStateChanged(ProjectExplorer::Project * pro);
    void buildQueueFinished(bool success);
    void buildSession();
    void buildProject();
    void rebuildSession();
    void rebuildProject();
    void cleanSession();
    void cleanProject();
    void cancelBuild();
    void runProject();
    void updateActions();

    void runConfigurationConfigurationFinished();
    void slotUpdateRunActions();
    void slotStartupProjectChanged();
    void activeTargetChanged();
    void activeRunConfigurationChanged();

    // project context menu
    void executeConextMenuBuildSystemCommand();
    void executeBuildSystemCommand();
    void buildProjectContextMenu();
    void rebuildProjectContextMenu();
    void cleanProjectContextMenu();
    void addExistingFiles();
    void addExistingDirectory();
    void addNewFile();
    void addNewSubproject();
    void removeProject();
    void removeFile();
    void deleteFile();
    void renameFile();
    void runProjectContextMenu();
    void openTerminalHere();
    void showInGraphicalShell();
    void searchOnFileSystem();
    void unloadProject();

    void issuesPaneNavigateStateChanged();
    void issuesClear();
    void issuesGoToNext();
    void issuesGoToPrevious();

signals:
    void currentProjectChanged(ProjectExplorer::Project *project);
    void currentNodeChanged(ProjectExplorer::Node *node, ProjectExplorer::Project *project);
    void saveSettingsRequested();
    void settingsChanged();
    void sessionLoaded();
    void aboutToShowContextMenu(ProjectExplorer::Project *project,
                                ProjectExplorer::Node *node);

    void projectAdded(ProjectExplorer::Project *project);
    void aboutToRemoveProject(ProjectExplorer::Project *project);
    void projectRemoved(ProjectExplorer::Project *project);
    void startupProjectChanged(ProjectExplorer::Project *project);
    void projectDisplayNameChanged(ProjectExplorer::Project *project);
    void aboutToExecuteProject(ProjectExplorer::Project *project, ProjectExplorer::RunMode runMode);
    void fileListChanged();
    void updateRunActions();
    void dependencyChanged(ProjectExplorer::Project *a, ProjectExplorer::Project *b);

private:
    static ProjectManager *m_instance;
    void loadProjectProvidersPlugins();
    void initProviders();
    void registerMacroExpanderVariables();

    bool projectContainsFile(ProjectExplorer::Project *p, const QString &fileName) const;
    void setCurrent(ProjectExplorer::Project *project, QString filePath, ProjectExplorer::Node *node);

    void addProject(ProjectExplorer::Project *pro);
    ProjectExplorer::Project *openProjectInternal(const QString &filePath);
    void closeProjects(const QList<ProjectExplorer::Project *> &remove);

    QStringList dependencies(const QString &proName) const;
    void dependencies(const QString &proName, QStringList &result) const;
    QStringList dependenciesOrder() const;

    QList<ProjectExplorer::IProjectProvider *> m_projectProviders;
    QString m_projectFilterString;
    QString m_projectsDirectory;
    bool m_useProjectsDirectory;
    QString m_buildDirectory;
    ProjectExplorer::SessionNode *m_sessionNode;
    ProjectExplorer::Project *m_startupProject;
    QList<ProjectExplorer::Project *> m_projects;
    mutable QHash<ProjectExplorer::Project *, QStringList> m_projectFileCache;
    QMap<QString, QStringList> m_depMap;

    QList<ProjectExplorer::IBuildStepFactory *> m_buildStepFactories;
    QList<ProjectExplorer::IBuildConfigurationFactory *> m_buildConfigurationFactories;
    QList<ProjectExplorer::IDeviceFactory *> m_deviceFactories;
    QList<ProjectExplorer::DeployConfigurationFactory *> m_deployConfigurationFactories;
    QList<ProjectExplorer::IRunControlFactory *> m_runControlFactories;
    QList<ProjectExplorer::IRunConfigurationFactory *> m_runConfigurationFactories;
    QList<ProjectExplorer::ToolChainFactory *> m_toolChainFactories;
    QList<ProjectExplorer::ICustomWizardMetaFactory *> m_customWizardMetaFactories;

    ProjectExplorer::Internal::ProjectTreeWidget *m_projectsTree;
    QwCustomDockWidget *m_projectDock;
    QwCustomDockWidget *m_compilerOutputDock;
    ProjectExplorer::ITaskHandler *m_taskHandler;
#ifdef Q_OS_WIN
    ProjectExplorer::Internal::WinDebugInterface *m_winDebugInterface;
#endif

    RecentFilesMenu *m_recentProjectMenu;

    void createActions();
    Core::Command *m_cmdNewProject;
    Core::Command *m_cmdOpenProject;
    Core::Command *m_cmdConfigureProjects;
    Core::Command *m_cmdCloseProject;
    Core::Command *m_cmdBuildOptions;
    Core::Command *m_cmdMiniProjectTargetSelector;

    QSettings *m_settings;
    QList<Core::IOptionsPage *> m_optionPages;
    ProjectExplorer::KitOptionsPage *m_kitOptionsPage;
    ProjectExplorer::DeviceManager *m_deviceManager;
    QtSupport::QtVersionManager *m_qtVersionManager;
    QtSupport::UiCodeModelManager *m_uiCodeModelManager;

    Core::Command *m_cmdCancelBuildAction;

    void fillProjectTreeContextMenus();
    void fillBuildMenus();
    QMenu *m_projectBuildMenu;

    Core::JsExpander *m_jsExpander;

    Utils::FileName licenseTemplatePath();
    QSharedPointer<CppTools::Internal::CppFileSettings> m_fileSettings;

    QwTabBarDockWidget *m_appOutputDock;

    Core::Command *m_cmdIssuesClear;
    Core::Command *m_cmdIssuesNext;
    Core::Command *m_cmdIssuesPrevious;
    Core::IOutputPane *m_issuesPane;
};

}   // namespace ProjectExplorer

#endif // PROJECTEXPLORER_H

