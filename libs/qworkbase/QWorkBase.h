#ifndef QWORKBASE_H
#define QWORKBASE_H

#include "QWorkBase_global.h"

#include <QMainWindow>
#include <QtPlugin>
#include <QList>
#include <QObject>
#include <QVariant>
#include <QMessageBox>
#include <QMutex>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QCloseEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QSplitter>
#include <QTabBar>

#include "SettingsManager.h"
#include "CommandManager.h"
#include "ISessionClient.h"
#include "IMainWindow.h"
#include "CloseApplicationHandler.h"
#include "IDocument.h"
#include "ITextDocument.h"
#include "WizardManager.h"
#include "QwDockWidget.h"
#include "ModeManager.h"
#include "StyleManager.h"
#include "HighlighterFormatsManager.h"
#include "MainToolBar.h"
#include "SnippetsDockWidget.h"

QT_BEGIN_NAMESPACE
class QString;
class QAction;
class QTabWidget;
class QFile;
class QSplitter;
class QComboBox;
class QToolBar;
class QKeyEvent;
class QToolButton;
class QTableView;
class QFileSystemWatcher;
class QActionGroup;
class QSignalMapper;
QT_END_NAMESPACE

class PlainTextDocumentPlugin;
class QFormatScheme;
class QLanguageFactory;
class QCodeEdit;
class SettingsManager;
class IPlugin;
class IDocumentPlugin;
class SearchEngine;
class DocumentManager;
class BookmarkPlugin;
class NavigationManager;
class RecentFilesMenu;
class SessionManager;

namespace ProjectExplorer {
class ProjectManager;
}

namespace TextEditor {
    class PlainTextEditorWidget;
    class ITextEditor;
    class CompletionAssistProvider;
}

namespace CppEditor {
    namespace Internal {
        class CPPEditorWidget;
    }
}

namespace Core {
    class ProgressManager;
    class MimeDatabase;
    class IEditor;
    class CommandManager;
    namespace Internal {
        class ProgressManagerPrivate;
    }
    class LogManager;
}

namespace Python {
    class PythonScriptManager;
    class PythonScript;
}


namespace CppTools {
namespace Internal {
class CppModelManager;
}
}

class TabDocumentContainer;
class TabWidgetSplitterHolder;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defaults
class ContainerTabBar: public QTabBar, public Utils::StyleManagedWidget
{
    Q_OBJECT

public:
    ContainerTabBar( QWidget *parent = 0 );

signals:
    void currentWidgetChanged( ContainerTabBar *t );

protected:
    virtual void mousePressEvent( QMouseEvent* event );
    virtual void mouseMoveEvent( QMouseEvent* event );

private:
    bool m_isAlternativeDrag;
    QPoint m_dragStartPosition;
};

class ContainerTabWidget: public QTabWidget
{
    Q_OBJECT

public:
    ContainerTabWidget( TabWidgetSplitterHolder *tabWidgetSplitterHolder );
    bool eventFilter( QObject *o, QEvent *ev );

    ContainerTabBar *containerTabBar() { return m_tabBar; }

signals:
    void tabMoveRequested( ContainerTabBar *sourceTabBar, int index, ContainerTabWidget *targetTabWidget );

protected:
    virtual void mouseDoubleClickEvent( QMouseEvent * );
    virtual void dragEnterEvent( QDragEnterEvent *event );
    virtual void dropEvent( QDropEvent *event );

private slots:
    void showContextMenu();
    void split();
    void removeSplit();

private:
    int m_prevTabCount;
    ContainerTabBar *m_tabBar;
    TabWidgetSplitterHolder *m_tabWidgetSplitterHolder;

    void updateActions();
    void createActions();
    QMenu *m_ctxMenu;
    QAction *m_actSplit;
    QAction *m_actRemoveSplit;
};

class TabWidgetSplitterHolder: public QSplitter, public Utils::StyleManagedWidget
{
    Q_OBJECT

public:
    TabWidgetSplitterHolder( TabDocumentContainer *container, QWidget *parent = 0 );
    virtual ~TabWidgetSplitterHolder();

    ContainerTabWidget *mainTabWidget();

    ContainerTabWidget *split( QWidget *lastActiveWidget );
    void splitTabWidget(ContainerTabWidget *t);
    void toggleOrientation();
    QList<QWidget *> widgetsOrdered() const;
    ContainerTabWidget *currentTabWidget( QWidget *lastActiveWidget );
    ContainerTabWidget *tabForWidget( QWidget *w );

    void clearEmptyPane( ContainerTabWidget *t );
    void removeSplit( ContainerTabWidget *t );
    bool canRemoveSplit();

private slots:
    void onRemoveSplit();

private:
    ContainerTabWidget *addTabWidget();
    void recalcSizes();

    QList<ContainerTabWidget *> m_tabWidgets;
    TabDocumentContainer *m_container;
    QList<ContainerTabWidget *> m_removedTabWidgets;
};

class MdiSubwindow: public QMdiSubWindow
{
    Q_OBJECT

public:
    MdiSubwindow( IDocument *document, QWidget *parent = 0, Qt::WindowFlags flags = 0 );

protected:
    virtual void closeEvent( QCloseEvent *ev );
    virtual void showEvent( QShowEvent *showEvent );

signals:
    void showStateChanged( QMdiSubWindow *subwindow );
    void windowClosed();

private:
    IDocument *m_document;
};

class QWORKBASESHARED_EXPORT MdiDocumentContainer: public IDocumentContainer
{
    Q_OBJECT

public:
    MdiDocumentContainer( QObject *parent = 0 );
    virtual ~MdiDocumentContainer();

    virtual QWidget *createWidget();
    virtual void addWidget( QWidget *widget );
    virtual void removeWidget( QWidget *widget );
    virtual void setCurrentWidget( QWidget *widget );
    virtual void documentChanged( IDocument *document );
    virtual void updateDocumentIcon( IDocument *document );
    virtual QList<IDocument *> documentsOrdered() const;

    virtual void restoreState();

    virtual QList<QAction *> createAdditionalWindowsActions();

protected:
    bool eventFilter( QObject *obj, QEvent *ev );

private slots:
    void onSubWindowActivated( QMdiSubWindow *subwindow );
    void onSubWindowShowStateChanged( QMdiSubWindow *subwindow );
    void onSubWindowClosed();
    void tileSubWindows();
    void cascadeSubWindows();
    void toggleMaximizedWindows( bool onoff );
    void restoreAllMinimized();

private:
    void saveSettings();
    void restoreSettings();

    QMdiArea *m_area;
    bool m_maximizedMode;
    bool m_tileMode;
    bool m_blockToggleSignal;
    bool m_tiling;
    bool m_cwsChanged;
    bool m_addingSubwindow;

    Core::Command *m_cmdTileWindows;
    Core::Command *m_cmdCascadeWindows;
    Core::Command *m_cmdToggleMaxMode;
    Core::Command *m_cmdRestoreAllMinimized;

    bool m_lbuttonPressed;
};

class TitleBar : public QWidget
{
    Q_OBJECT
public:
    TitleBar( const QString &title, QWidget *parent );
    QLabel label;

public slots:
    void showSmall();
    void showMaxRestore();

protected:
    void mousePressEvent( QMouseEvent *me );
    void mouseMoveEvent( QMouseEvent *me );

private:
    QPushButton minimize;
    QPushButton maximize;
    QPushButton close;
    QLabel space;
    QHBoxLayout hbox;

    QPixmap restorePix, maxPix;
    bool maxNormal;
    QPoint clickPos;
};

class MdiArea;
class MdiDocument;

class Frame : public QFrame
{
public:
    Frame( QWidget *docWidget, MdiArea *area, MdiDocument *document );
    TitleBar *titleBar();

    QWidget *documentWidget() const;
    void activate( bool notifyArea = true );

protected:
    void mousePressEvent( QMouseEvent *e );
    void mouseMoveEvent( QMouseEvent *e );
    void mouseReleaseEvent( QMouseEvent *e );
    void enterEvent( QEvent *event );

private:
    QVBoxLayout vbox, layout;
    QPoint m_old_pos;
    bool m_mouse_down;
    bool left, right, bottom;

    MdiArea *m_area;
    MdiDocument *m_document;

    QWidget *m_documentWidget;

public:
    TitleBar m_titleBar;
};

class MdiDocument: public QMdiSubWindow
{
    Q_OBJECT

public:
    MdiDocument( IDocument *document, MdiArea *area, Qt::WindowFlags flags = 0 );

    bool isMyWidget( QWidget *widget_ ) const;
    QWidget *myWidget() const;

    IDocument *document() const;
    void activate( bool notifyArea = false );

private:
    IDocument *m_document;
    Frame *m_frame;

    // Hide public methods
    QWidget *widget() const;
};

class MdiArea: public QMdiArea
{
    Q_OBJECT

public:
    enum Mode {
        Default = 0,
        Maximized,
        Tiled,
        Cascade
    };

    MdiArea( Mode mode = Default, QWidget *parent = 0 );

    QList<IDocument *> documentList() const;

    MdiDocument *documentForWidget( QWidget *widget ) const;
    QWidget *activeWidget() const;

    void addDocument( IDocument *document );
    void removeWidget( QWidget *widget );

private:
    Mode m_mode;
    QList<MdiDocument *> m_documents;
};

class QWORKBASESHARED_EXPORT MdiAreaContainer: public IDocumentContainer
{
    Q_OBJECT

public:
    MdiAreaContainer( QObject *parent = 0 );

    virtual QWidget *createWidget();
    virtual void addWidget( QWidget *widget );
    virtual void removeWidget( QWidget *widget );
    virtual void setCurrentWidget( QWidget *widget );
    virtual void documentChanged( IDocument *document );
    virtual void updateDocumentIcon( IDocument *document );
    virtual QList<IDocument *> documentsOrdered() const;

private slots:
    void onSubWindowActivated( QMdiSubWindow *subwindow );

private:
    MdiArea *m_area;
};

class QWORKBASESHARED_EXPORT TabDocumentContainer: public IDocumentContainer
{
    Q_OBJECT

public:
    TabDocumentContainer( QObject *parent = 0 );

    virtual QWidget *createWidget();
    virtual void addWidget( QWidget *widget );
    virtual void removeWidget( QWidget *widget );
    virtual void setCurrentWidget( QWidget *widget );
    virtual void documentChanged( IDocument *document );
    virtual void updateDocumentIcon( IDocument *document );
    virtual QList <IDocument *> documentsOrdered() const;

    virtual void restoreState();

    virtual QList<QAction *> createAdditionalWindowsActions();

public slots:
    void onCurrentChanged( int index );
    void onDocumentCloseRequested( int index );
    void onTabMoveRequested( ContainerTabBar *sourceTabBar, int index, ContainerTabWidget *targetTabWidget );
    void onCurrentWidgetChanged( ContainerTabBar *t );

    void split();
    void toggleSplitOrientation();
    void removeSplit();

private:
    QWidget *lastActiveWidget() const;

    TabWidgetSplitterHolder *m_splitter;

    Core::Command *m_cmdSplit;
    Core::Command *m_cmdSplitToggleOrientation;
    Core::Command *m_cmdRemoveSplit;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Base Classes
class DocumentFactory: public IDocumentFactory
{
public:
    IDocument *newDocument( QString &extension );
    void loadDocumentPlugins();
};

class QWORKBASESHARED_EXPORT QWorkMainWindow : public QMainWindow, public IMainWindow
{
    Q_OBJECT
    friend class Python::PythonScriptManager;

private:
    void addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockwidget);
    void addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockwidget,
                       Qt::Orientation orientation);

    void parseArguments( bool showErrorMessage = true );

public:
    enum CentralWidgetType {
        Tabbed,
        Mdi
    };

    QWorkMainWindow( const QString &orgName_, const QString &appName_, QWidget *parent = 0 );
    virtual ~QWorkMainWindow();

    QString getOrgName() const { return m_orgName; }
    QString getAppName() const { return m_appName; }
    QString applicationDataLocation() const;

    void addToggleAction( QAction *action );
    void showStatusMessage( const QString &message );
    void hideStatusMessage( const QString &message = tr("Ready"));

    static QWorkMainWindow *getInstance();

    QMenu *getSearchMenu() { return m_searchMenu; }
    QMenu *windowsMenu() { return m_windowsMenu; }

    BookmarkPlugin *getBookmarkManager() const { return m_bookmarkManager; }

    void addCompletionAssistProvider(TextEditor::CompletionAssistProvider *provider) { m_completionAssistProviders << provider; }
    QList<TextEditor::CompletionAssistProvider *> getCompletionAssistProviders() const { return m_completionAssistProviders; }

    QList<QAction *> navigateActions() { return QList<QAction *>() << m_cmdNavigateBack->action() << m_cmdNavigateForward->action(); }
    void setNavigationHistoryBlocked( bool onoff = true );
    void cutForwardNavigationHistory();
    void handleNavigationStateChanged( bool canBack, bool canForward );

    static QMessageBox::StandardButton information( const QString& text,
                                                    QMessageBox::StandardButtons buttons = QMessageBox::StandardButtons(QMessageBox::Ok),
                                                    QMessageBox::StandardButton defaultButton = QMessageBox::Ok);
    static QMessageBox::StandardButton warning( const QString& text,
                                                QMessageBox::StandardButtons buttons = QMessageBox::StandardButtons(QMessageBox::Ok),
                                                QMessageBox::StandardButton defaultButton = QMessageBox::Ok );

    QMenu *editMenu() { return m_editMenu; }

    // IMainWindow implementation
    virtual QMainWindow *widget();
    virtual QString orgName() const;
    virtual QString appName() const;
    virtual void addDockWidgetHandler( Core::DockWidgetHandler *dockHandler );
    virtual Core::MimeDatabase *mimeDatabase() const;

    virtual QMessageBox::StandardButton showInformation( const QString& text,
                                                         QMessageBox::StandardButtons buttons = QMessageBox::StandardButtons(QMessageBox::Ok),
                                                         QMessageBox::StandardButton defaultButton = QMessageBox::Ok );

    virtual QMessageBox::StandardButton showWarning( const QString& text,
                                                     QMessageBox::StandardButtons buttons = QMessageBox::StandardButtons(QMessageBox::Ok),
                                                     QMessageBox::StandardButton defaultButton = QMessageBox::Ok );

    virtual bool question(const QString &msg);

    virtual Core::FutureProgress *addTask( const QFuture<void> &future, const QString &title, const QString &type,
                                           bool signToAppStatusBar = true, bool isSyncTask = false );
    Core::ProgressManager *progressManager() const;
    void addCurrentPositionToNavigationHistory(Core::IEditor *editor = 0, const QByteArray &saveState = QByteArray());

    virtual bool registerModule( Python::IPythonModule *module, bool deleteOnFail = true, const QStringList &classNames = QStringList() );
    virtual bool registerBuiltinScript( const QString &scriptId, const QString &scriptSource );
    virtual void pythonReport( const QString &message, IMainWindow::ReportType reportType = IMainWindow::Error );
    virtual void executeBuiltinScript( const QString &scriptId );
    virtual QStringList pythonCodeAssist( const QString &source, int position );
    virtual void handleScriptChanged( const QString &scriptName, const QString &source );
    virtual QMap<QString, Python::IPythonModule *> *pythonModules();

protected:
    virtual void closeEvent( QCloseEvent *event );
    virtual void changeEvent( QEvent *event );
    virtual void dragEnterEvent( QDragEnterEvent *event );
    virtual void dropEvent( QDropEvent *event );

private slots:
    void onSettings();
    void newFromWizard();
    void onStatusBarShow( bool show );
    void onToggleFullScreenMode( bool onoff );
    void switchMode(IDocument *document);

    void onNavigateForward();
    void onNavigateBack();

    void addToRecentFiles( const QString &filePath );
    void updateRecentFile( const QString &oldFileName, const QString &newFileName );

    void onSessionControl();
    void onDockCornerSettings();

    void openDelayedFiles();

signals:
    void windowActivated();

private:
    static QWorkMainWindow *m_mainWindow;

private:
    SessionManager *m_sessionManager;
    CloseApplicationHandler *m_closeReflector;
    Core::WizardManager *m_wizardManager;

    SettingsManager *m_settingsManager;
    DocumentManager *m_documentManager;
    Core::CommandManager  *m_commandManager;

    Core::Internal::ProgressManagerPrivate *m_progressManager;
    Core::MimeDatabase *m_mimeDatabase;

    void createActions();
    Core::Command *m_cmdNewFile;
    Core::Command *m_cmdNewFromWizard;
    Core::Command *m_cmdOpenFile;
    Core::Command *m_cmdSaveFile;
    Core::Command *m_cmdSaveAsFile;
    Core::Command *m_cmdSaveAllFile;
    Core::Command *m_cmdSessions;
    Core::Command *m_cmdCloseFile;
    Core::Command *m_cmdCloseAllFiles;
    Core::Command *m_cmdExit;
    Core::Command *m_cmdToggleStatusBar;
    Core::Command *m_cmdToggleFullScreenMode;

    Core::Command *m_cmdSettings;
    Core::Command *m_cmdShortcuts;
    Core::Command *m_cmdDockCornerSettings;

    void createNavigateActions();
    Core::Command *m_cmdNavigateForward;
    Core::Command *m_cmdNavigateBack;

    void createMenus();
    QMenu *fileMenu;
    QMenu *m_searchMenu;
    QMenu *m_windowsMenu;
    QMenu *m_editMenu;
    QMenu *viewMenu;
    QMenu *prefsMenu;

    QMenu *m_scriptsMenu;

    void createToolBars();
    Core::MainToolBar *m_fileToolBar;
    Core::MainToolBar *m_viewToolBar;

    void writeSettings();
    void readSettings();

    void populateUiFromPlugins();

    QString m_orgName;
    QString m_appName;

    void loadPlugins();
    QList <IPlugin *> m_plugins;
    BookmarkPlugin *m_bookmarkManager;
    ProjectExplorer::ProjectManager *m_projectManager;

    void initializePlugins();
    void initializeDocumentPlugins();

    Core::LogManager *m_logManager;
    Utils::StyleManager *m_styleManager;
    Python::PythonScriptManager *m_pythonScriptingManager;
    SearchEngine *m_searchEngine;

    bool m_isStatusBarHidden;

    NavigationManager *m_navigationManager;

    QList<TextEditor::CompletionAssistProvider *> m_completionAssistProviders;

    RecentFilesMenu *m_recentFilesMenu;

    QStringList m_filesToOpenDelayed;

    QList<Core::DockWidgetHandler *> m_dockHandlers;
    QList<QToolBar *> m_toolBars;

    CentralWidgetType m_centralWidgetType;
    CppTools::Internal::CppModelManager *m_cppModelManager;

    QMap<QwCustomDockWidget::DockAreaTabifyHint, QwCustomDockWidget *> m_dockHintMap;

    Core::ModeManager *m_modeManager;
    HighlighterFormatsManager *m_highlighterFormatsManager;

    void initializeSnippets();
    SnippetsDockWidget *m_snippetsDock;
    Core::Command *m_cmdAddSnippet;
    Core::Command *m_cmdDeleteSnippet;
};

#endif // QWORKBASE_H
