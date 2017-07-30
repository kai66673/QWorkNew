#ifndef QASOFPROJECTPLUGIN_H
#define QASOFPROJECTPLUGIN_H

#include "IPlugin.h"
#include "CommandManager.h"
#include "SearchEngine.h"
#include "ISessionClient.h"
#include "CloseApplicationHandler.h"
#include "ProjectBuildActionsManager.h"
#include "AsofProjectImportManager.h"

#include <QObject>

QT_BEGIN_NAMESPACE
class QTreeView;
QT_END_NAMESPACE

class ProjectTreeView;
class AsofProjectPlugin;
class RecentFilesMenu;

namespace Python {
class AsofProjectPythonModule;
}

class AsofProjectUiProvider: public UiProviderInterface
{
public:
    AsofProjectUiProvider( IPlugin *plugin, QwCustomDockWidget *dock )
        : UiProviderInterface(plugin, dock)
    {}

    virtual int     pluginUiProviderIndex() const { return 0; }
    virtual QString dockWidgetName() const { return QObject::tr("AsofProjectDockWidget"); }
    virtual QString toolBarName() const { return ""; }
    virtual QString toolBarObjectName() const { return ""; }
    virtual QString menuBarName() const { return QObject::tr("&Asof Project"); }
    virtual QKeySequence toggleDockKeySequence() const { return QKeySequence(QObject::tr("Ctrl+D,Ctrl+A")); }
};

class AsofProjectBuildActionsUiProvider: public UiProviderInterface
{
public:
    AsofProjectBuildActionsUiProvider( IPlugin *plugin, QwCustomDockWidget *dock )
        : UiProviderInterface(plugin, dock)
    {}

    virtual int     pluginUiProviderIndex() const { return 1; }
    virtual QString dockWidgetName() const { return QObject::tr("AsofProjectBuildDockWidget"); }
    virtual QString toolBarName() const { return ""; }
    virtual QString toolBarObjectName() const { return ""; }
    virtual QString menuBarName() const { return ""; }
    virtual QKeySequence toggleDockKeySequence() const { return QKeySequence(QObject::tr("Ctrl+D,R")); }
};

class AsofProjectPlugin : public IPlugin, public IDocumentHelper, public ISessionClient, public ICloseApplicationClient
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)
#ifdef HAVE_QT5
    Q_PLUGIN_METADATA(IID "QWorkBase.IPlugin/1.0")
#endif

    friend class Python::AsofProjectPythonModule;

public:
    AsofProjectPlugin();
    virtual ~AsofProjectPlugin();

    virtual QString id() const { return "AsofProjectManager"; }
    virtual void populateMenu( int pluginUiIndex, QMenu *menu );
    virtual void populateToolBar( int pluginUiIndex, QToolBar *toolBar ) { Q_UNUSED(pluginUiIndex); Q_UNUSED(toolBar); }

    // QIDocumentHelper implementation
    virtual bool documentEventFilter( IDocument *document, QEvent *event );

    virtual QList<QByteArray> signalList() const {
        return QList<QByteArray>()
                << "projectOpened"
                << "projectClosed"
                << "projectChanged";
    }

    void emitProjectChanged( QVariantList &valuesList );

    bool isProjectOpened() const { return m_projectOpened; }
    void getProjectFiles( QStringList &fileNames ) const;

    virtual void initialize();

    virtual QList<UiProviderInterface *>createUiProviders( IMainWindow *mainWindow );

    void openProjectFilePath( QString filePath = "" );

    // ISessionClient implementation
    virtual bool sessionAboutToClose();
    virtual void sessionClose();
    virtual void saveSession( const QString &location );
    virtual void loadSession( const QString &location );

    // QIAppCloseReflectorClient implementation
    virtual bool applicationAboutToClose();
    virtual void applicationClose() {}
    virtual void destroy();

    inline ProjectBuildActionsManager *buildActionsManager() const { return m_buildActionsManager; }
    inline AsofProjectImportManager *projectImportManager() const { return m_importManager; }

    void expandProjectFirstLevel();

    void emitProjectSynhronized( const QString &projectName, const QStringList &includePaths,
                                 const QStringList &systemIncludePaths, const QStringList &defines,
                                 const QStringList &sourceFiles, const QString &precompiledHeader );

    QStringList projectFiles();
    ProjectTreeItem *projectFolderItem();
    ProjectTreeItem *defaultSourceFolderItem();
    ProjectTreeItem *defaultHeaderFolderItem();

public slots:
    void newProjectDb();
    void openProjectDb();
    void closeProjectDb();
    void onDocumentSaved( const QString &filePath );
    void onEditProjectProperties();

    void onBuildActionStarted();
    void onBuildActionFifnished();

signals:
    void projectOpened( QVariantList &valuesList );
    void projectClosed( QVariantList &valuesList );
    void projectChanged( QVariantList &valuesList );

protected:
    virtual QStringList settingsKeyList() const { return QStringList(); }
    virtual ISettings *createSettings( const QString &/*settingsKey*/ ) const { return NULL; }

private:
    void createActions();
    Core::Command *m_cmdNewProject;
    Core::Command *m_cmdOpenProject;
    Core::Command *m_cmdCloseProject;
    Core::Command *m_cmdToggleBuildIssues;
    Core::Command *m_cmdCancelBuild;
    Core::Command *m_cmdToggleWarnings;

    ProjectTreeView *projectTreeView;

    bool m_projectOpened;

    RecentFilesMenu *recentProjectMenu;

    AsofProjectImportManager *m_importManager;
    ProjectBuildActionsManager *m_buildActionsManager;
    QTabBar *m_buildTabBar;

    QwCustomDockWidget *m_projectDock;
};

class QAsProjectFilesFindScope : public BaseTextFindScope
{
public:
    QAsProjectFilesFindScope( AsofProjectPlugin *projectPlugin )
        : BaseTextFindScope()
        , m_projectPlugin(projectPlugin)
    {}

    virtual QString name() const { return QObject::tr("Asof Project Documents"); }
    virtual bool isSearchEnabled() const { return m_projectPlugin->isProjectOpened(); }
    virtual bool isSearchAndReplaceEnabled() const { return m_projectPlugin->isProjectOpened(); }
    virtual bool prepareToSearch() const { return true; }

protected:
    virtual Utils::FileIterator *files();

private:
    AsofProjectPlugin *m_projectPlugin;
};

#endif // QASOFPROJECTPLUGIN_H
