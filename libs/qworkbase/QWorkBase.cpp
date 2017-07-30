#include <QDir>
#include <QPluginLoader>
#include <QApplication>
#include <QFileDialog>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QSplitter>
//#include <QAbstractFileEngine>
#include <QMessageBox>
#include <QComboBox>
#include <QToolBar>
#include <QKeyEvent>
#include <QSettings>
#include <QPointer>
#include <QTimer>
#include <QApplication>
#include <QDomDocument>
#include <QStatusBar>
#include <QPushButton>

#include "PlainTextEditor.h"
#include "texteditor.h"
#include "ifile.h"
#include "progressmanager_p.h"
#include "mimedatabase.h"
#include "texteditorsettings.h"

#include "SearchEngine.h"
#include "LogManager.h"
#include "PythonScriptManager.h"
#include "IPlugin.h"
#include "CommandManager.h"
#include "NavigationManager.h"
#include "SessionManager.h"

#include "QWorkBase.h"
#include "ListSelectorDialog.h"
#include "BookmarkPlugin.h"
#include "TextPositionStatusBarWidget.h"
#include "RecentFilesMenu.h"
#include "SessionsDialog.h"
#include "TasksStatusWidget.h"

#include "completionassistprovider.h"

#include "PythonScriptManager.h"
#include "DocumentManagerPythonModule.h"
#include "Core.h"
#include "PlainTextDocumentPlugin.h"
#include "projectmanager.h"
#include "theme/theme.h"
#include "historycompleter.h"
#include "CodeModel/CodeModelManager.h"
#include "cppmodelmanager.h"
#include "WizardManager.h"
#include "DockCornerSettingsDialog.h"
#include "ProjectManagerPythonModule.h"

static int s_splitterWatcherIndex = -1;
static int s_tabWidgetWatcherIndex = -1;

class QRecentFilesDelegate: public IRecentFilesDelegate
{
public:
    QRecentFilesDelegate( QWorkMainWindow *mainWindow_ )
        : mainWindow(mainWindow_)
    {}

    virtual void openFile( const QString &fileName ) const {
        DocumentManager::getInstance()->openDocument(fileName, 0, 0, false);
    }
    virtual QString menuName() const { return QObject::tr("Recent Files"); }
    virtual QString settingName() const  { return QStringLiteral("RecentFiles"); }

private:
    QWorkMainWindow *mainWindow;
};

//////////////////////////////////////////////////////////////////////////////////////
/// ContainerTabBar
ContainerTabBar::ContainerTabBar( QWidget *parent )
    : QTabBar(parent)
    , Utils::StyleManagedWidget(this, s_tabWidgetWatcherIndex)
    , m_isAlternativeDrag(false)
{ }

void ContainerTabBar::mousePressEvent( QMouseEvent *event )
{
    int i = tabAt(event->pos());
    m_isAlternativeDrag = false;
    m_dragStartPosition = QPoint();
    setMovable(true);

    if ( i != -1 && event->button() == Qt::LeftButton && (QApplication::keyboardModifiers() & Qt::ControlModifier) ) {
        m_isAlternativeDrag = true;
        m_dragStartPosition = event->pos();
        setMovable(false);
    }

    QTabBar::mousePressEvent(event);
    emit currentWidgetChanged(this);
}

void ContainerTabBar::mouseMoveEvent( QMouseEvent *event )
{
    if ( !m_isAlternativeDrag ) {
        QTabBar::mouseMoveEvent(event);
        return;
    }

    if ( (event->pos() - m_dragStartPosition).manhattanLength() < QApplication::startDragDistance() )
        return;

    QMimeData* m = new QMimeData;
    m->setData(Core::Storage::mainWindow()->appName() + "/DragType", QByteArray());
    m->setProperty("tab_index", tabAt(m_dragStartPosition));
    m->setProperty("tab_bar", QVariant::fromValue(this));

    QDrag *d = new QDrag(this);
    d->setMimeData(m);
    QIcon cursorIcon(":/images/move_tab.png");
    d->setDragCursor(cursorIcon.pixmap(QSize(24, 24)), Qt::MoveAction);
    d->exec(Qt::MoveAction);

    QTabBar::mouseMoveEvent(event);
}

//////////////////////////////////////////////////////////////////////////////////////
/// ContainerTabWidget
ContainerTabWidget::ContainerTabWidget(TabWidgetSplitterHolder *tabWidgetSplitterHolder)
    : QTabWidget(tabWidgetSplitterHolder)
    , m_prevTabCount(0)
    , m_tabBar(new ContainerTabBar)
    , m_tabWidgetSplitterHolder(tabWidgetSplitterHolder)
{
    setTabBar(m_tabBar);
    setStyleSheet("QTabWidget::pane { border-top: 1px solid #C2C7CB; }");
    setAcceptDrops(true);
    tabBar()->installEventFilter(this);

    QToolButton *contextMenuButton = new QToolButton();
    contextMenuButton->setIcon(QIcon(":/images/tab_ctx_menu.png"));
    setCornerWidget(contextMenuButton);
    contextMenuButton->setStyleSheet("QToolButton { border: 0px; border-radius: 0px; }"
                                     "QToolButton:hover { border: 0px; border-radius: 0px; background-color: #d3d3d3; }");
    connect(contextMenuButton, SIGNAL(clicked()), this, SLOT(showContextMenu()));

    createActions();
}

bool ContainerTabWidget::eventFilter( QObject *o, QEvent *ev )
{
    if ( ev->type() == QEvent::MouseButtonRelease ) {
        m_prevTabCount = tabBar()->count();
    }

    else if ( /*o == tabBar() &&*/ ev->type() == QEvent::MouseButtonDblClick ) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(ev);
        QTabBar *tb = tabBar();
        if ( m_prevTabCount == tb->count() ) {
            for (int i = 0; i < tb->count(); i++) {
                if ( tb->tabRect(i).contains(mouseEvent->pos()) ) {
                    DocumentManager::getInstance()->closeFile();
                    return true;
                }
            }
        }
    }

    return QTabWidget::eventFilter(o, ev);
}

void ContainerTabWidget::mouseDoubleClickEvent( QMouseEvent * )
{
    DocumentManager::getInstance()->newFile();
}

void ContainerTabWidget::dragEnterEvent( QDragEnterEvent *event )
{
    if ( event->mimeData()->hasFormat(Core::Storage::mainWindow()->appName() + "/DragType") ) {
        ContainerTabBar *tabBar_ = qvariant_cast<ContainerTabBar *>(event->mimeData()->property("tab_bar"));
        if ( tabBar() != tabBar_ )
            event->acceptProposedAction();
    }

    QTabWidget::dragEnterEvent(event);
}

void ContainerTabWidget::dropEvent( QDropEvent *event )
{
    if ( !event->mimeData()->hasUrls() ) {
        if ( event->mimeData()->hasFormat(Core::Storage::mainWindow()->appName() + "/DragType") ) {
            ContainerTabBar *tabBar_ = qvariant_cast<ContainerTabBar *>(event->mimeData()->property("tab_bar"));
            int index = event->mimeData()->property("tab_index").toInt();
            event->acceptProposedAction();
            emit tabMoveRequested(tabBar_, index, this);
        }
    }

    QTabWidget::dropEvent(event);
}

void ContainerTabWidget::showContextMenu()
{
    if ( QWidget *w = qobject_cast<QWidget *>(sender()) ) {
        updateActions();
        m_ctxMenu->exec(mapToGlobal(w->pos() + w->rect().bottomLeft()));
    }
}

void ContainerTabWidget::split()
{
    m_tabWidgetSplitterHolder->splitTabWidget(this);
}

void ContainerTabWidget::removeSplit()
{
    m_tabWidgetSplitterHolder->removeSplit(this);
}

void ContainerTabWidget::updateActions()
{
    m_actSplit->setEnabled(count() > 1);
    m_actRemoveSplit->setEnabled(m_tabWidgetSplitterHolder->canRemoveSplit());
}

void ContainerTabWidget::createActions()
{
    m_actSplit = new QAction(QIcon(":/images/tab_add_split.png"), tr("Split"), this);
    m_actSplit->setToolTip(tr("Move current document to new splitter pane"));
    m_actRemoveSplit = new QAction(QIcon(":/images/tab_del_split.png"), tr("Remove split"), this);
    m_actRemoveSplit->setToolTip(tr("Remove current splitter pane (no documents closed)"));

    m_ctxMenu = new QMenu(this);
    m_ctxMenu->addAction(m_actSplit);
    m_ctxMenu->addAction(m_actRemoveSplit);

    connect(m_actSplit, SIGNAL(triggered()), this, SLOT(split()));
    connect(m_actRemoveSplit, SIGNAL(triggered()), this, SLOT(removeSplit()));

    updateActions();
}

//////////////////////////////////////////////////////////////////////////////////////
/// SplitterTabWidget
TabWidgetSplitterHolder::TabWidgetSplitterHolder( TabDocumentContainer *container, QWidget *parent )
    : QSplitter(parent)
    , Utils::StyleManagedWidget(this, s_splitterWatcherIndex)
    , m_container(container)
{
    setChildrenCollapsible(false);
    setFrameShape(QFrame::NoFrame);
    setLineWidth(0);
    addTabWidget();
}

TabWidgetSplitterHolder::~TabWidgetSplitterHolder()
{
    qDeleteAll(m_removedTabWidgets);
}

ContainerTabWidget *TabWidgetSplitterHolder::mainTabWidget()
{ return m_tabWidgets[0]; }

ContainerTabWidget *TabWidgetSplitterHolder::split( QWidget *lastActiveWidget )
{
    if ( ContainerTabWidget *t = currentTabWidget(lastActiveWidget) ) {
        if ( t->count() > 1 ) {
            int tabIndex = t->indexOf(lastActiveWidget);
            QString title = t->tabText(tabIndex);
            QString toolTip = t->tabToolTip(tabIndex);
            QIcon icon = t->tabIcon(tabIndex);
            t->removeTab(tabIndex);

            ContainerTabWidget *tabWidget = addTabWidget();

            tabIndex = tabWidget->addTab(lastActiveWidget, icon, title);
            tabWidget->setTabToolTip(tabIndex, toolTip);

            recalcSizes();

            return tabWidget;
        }
    }

    return 0;
}

void TabWidgetSplitterHolder::splitTabWidget(ContainerTabWidget *t)
{
    if ( t ) {
        int tabIndex = t->currentIndex();
        QWidget *w = t->widget(tabIndex);
        QString title = t->tabText(tabIndex);
        QString toolTip = t->tabToolTip(tabIndex);
        QIcon icon = t->tabIcon(tabIndex);
        t->removeTab(tabIndex);

        ContainerTabWidget *tabWidget = addTabWidget();

        tabIndex = tabWidget->addTab(w, icon, title);
        tabWidget->setTabToolTip(tabIndex, toolTip);

        recalcSizes();
    }
}

void TabWidgetSplitterHolder::toggleOrientation()
{
    Qt::Orientation newOrientation = (orientation() == Qt::Horizontal ? Qt::Vertical : Qt::Horizontal);
    setOrientation(newOrientation);
}

QList<QWidget *> TabWidgetSplitterHolder::widgetsOrdered() const
{
    QList<QWidget *> result;

    foreach ( ContainerTabWidget *t, m_tabWidgets ) {
        for (int i = 0; i < t->count(); i++)
            result << t->widget(i);
    }

    return result;
}

ContainerTabWidget *TabWidgetSplitterHolder::currentTabWidget( QWidget *lastActiveWidget )
{
    foreach ( ContainerTabWidget *t, m_tabWidgets ) {
        for (int i = 0; i < t->count(); i++)
            if ( t->widget(i) == lastActiveWidget )
                return t;
    }

    return m_tabWidgets[0];
}

ContainerTabWidget *TabWidgetSplitterHolder::tabForWidget( QWidget *w )
{
    foreach ( ContainerTabWidget *t, m_tabWidgets ) {
        for (int i = 0; i < t->count(); i++)
            if ( t->widget(i) == w )
                return t;
    }

    return 0;
}

void TabWidgetSplitterHolder::clearEmptyPane( ContainerTabWidget *t )
{
    if ( m_tabWidgets.size() > 1 ) {
        int tabIndex = m_tabWidgets.indexOf(t);
        m_tabWidgets.removeAt(tabIndex);
        t->setParent(0);
        // On Linux: SGEFAULT
        m_removedTabWidgets.append(t);
        // t->deleteLater();

        recalcSizes();
    }
}

void TabWidgetSplitterHolder::onRemoveSplit()
{
    if ( ContainerTabWidget *t = qobject_cast<ContainerTabWidget *>(sender()->parent()) )
        removeSplit(t);
}

ContainerTabWidget *TabWidgetSplitterHolder::addTabWidget()
{
    ContainerTabWidget *tabWidget = new ContainerTabWidget(this);
    tabWidget->setMovable(true);
    tabWidget->setTabsClosable(true);
    m_tabWidgets.append(tabWidget);
    addWidget(tabWidget);
    connect( tabWidget, SIGNAL(tabMoveRequested(ContainerTabBar*,int,ContainerTabWidget*)),
             m_container, SLOT(onTabMoveRequested(ContainerTabBar*,int,ContainerTabWidget*)) );
    connect( tabWidget->containerTabBar(), SIGNAL(currentWidgetChanged(ContainerTabBar*)),
             m_container, SLOT(onCurrentWidgetChanged(ContainerTabBar*)) );
    return tabWidget;
}

void TabWidgetSplitterHolder::recalcSizes()
{
    QList<int> s = sizes();
    if ( s.size() > 1 ) {
        int ms = width() / s.size();
        for (int i = 0; i < s.size() - 1; i++)
            s[i] = ms;
        s[s.size() - 1] = width() - ms * (s.size() - 1);
        setSizes(s);
    }
}

void TabWidgetSplitterHolder::removeSplit( ContainerTabWidget *t )
{
    struct TabInfo {
        TabInfo( QWidget *widget_,
                 const QString &title_,
                 const QString &toolTip_,
                 const QIcon &icon_ )
            : widget(widget_)
            , title(title_)
            , toolTip(toolTip_)
            , icon(icon_)
        { }

        QWidget *widget;
        QString title;
        QString toolTip;
        QIcon icon;
    };


    if ( m_tabWidgets.size() > 1 ) {
        QList<TabInfo *> tabsToMove;
        QWidget *currentWidget = t->widget(t->currentIndex());

        int tabIndex = m_tabWidgets.indexOf(t);

        for (int i = 0; i < t->count(); i++)
            tabsToMove.append( new TabInfo( t->widget(i),
                                            t->tabText(i),
                                            t->tabToolTip(i),
                                            t->tabIcon(i) ) );
        t->clear();
        clearEmptyPane(t);
        if ( tabIndex )
            tabIndex--;

        ContainerTabWidget *nt = m_tabWidgets[tabIndex];
        foreach( TabInfo *p, tabsToMove ) {
            int addIndex = nt->addTab(p->widget, p->icon, p->title);
            nt->setTabToolTip(addIndex, p->toolTip);
        }
        nt->setCurrentWidget(currentWidget);

        recalcSizes();

        qDeleteAll(tabsToMove);
    }
}

bool TabWidgetSplitterHolder::canRemoveSplit()
{
    return m_tabWidgets.size() > 1;
}

//////////////////////////////////////////////////////////////////////////////////////
/// MdiSubwindow
MdiSubwindow::MdiSubwindow( IDocument *document, QWidget *parent, Qt::WindowFlags flags )
    : QMdiSubWindow(parent, flags)
    , m_document(document)
{ }

void MdiSubwindow::closeEvent( QCloseEvent *ev )
{
    DocumentManager::getInstance()->closeDocument(m_document);
    ev->ignore();
    emit windowClosed();
}

void MdiSubwindow::showEvent( QShowEvent *showEvent )
{
    QMdiSubWindow::showEvent(showEvent);
    emit showStateChanged(this);
}

//////////////////////////////////////////////////////////////////////////////////////
/// MdiDocumentContainer
MdiDocumentContainer::MdiDocumentContainer( QObject *parent )
    : IDocumentContainer(QWorkMainWindow::getInstance(), parent)
    , m_area(new QMdiArea(QWorkMainWindow::getInstance()))
    , m_maximizedMode(false)
    , m_tileMode(false)
    , m_blockToggleSignal(false)
    , m_tiling(false)
    , m_cwsChanged(false)
    , m_addingSubwindow(false)
    , m_lbuttonPressed(false)
{
    connect(m_area, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(onSubWindowActivated(QMdiSubWindow*)));
    m_area->installEventFilter(this);
}

MdiDocumentContainer::~MdiDocumentContainer()
{
    saveSettings();
}

QWidget *MdiDocumentContainer::createWidget()
{
    return m_area;
}

void MdiDocumentContainer::addWidget( QWidget *widget )
{
    IDocument *document = m_documents[widget];
    MdiSubwindow *child = new MdiSubwindow(document, m_area, /*Qt::CustomizeWindowHint*/0);
    child->setWidget(widget);
    child->setWindowTitle(document->title());
    m_area->addSubWindow(child);
    if ( m_maximizedMode ) {
        m_addingSubwindow = true;
        child->showMaximized();
        m_addingSubwindow = false;
    }
    else {
        child->show();
        if ( m_tileMode )
            tileSubWindows();
    }
    m_area->setActiveSubWindow(child);
    connect(child, SIGNAL(showStateChanged(QMdiSubWindow*)), this, SLOT(onSubWindowShowStateChanged(QMdiSubWindow*)));
    connect(child, SIGNAL(windowClosed()), this, SLOT(onSubWindowClosed()));

    child->setMouseTracking(true);
    child->installEventFilter(this);
}

void MdiDocumentContainer::removeWidget( QWidget *widget )
{
    QList<QMdiSubWindow *> mdiSubwindows = m_area->subWindowList();
    foreach ( QMdiSubWindow *subwindow, mdiSubwindows )
        if ( subwindow->widget() == widget ) {
            m_area->removeSubWindow(subwindow);
            break;
        }

    onSubWindowClosed();
}

void MdiDocumentContainer::setCurrentWidget( QWidget *widget )
{
    QList<QMdiSubWindow *> mdiSubwindows = m_area->subWindowList();
    foreach ( QMdiSubWindow *subwindow, mdiSubwindows )
        if ( subwindow->widget() == widget ) {
            m_area->setActiveSubWindow(subwindow);
            break;
        }
}

void MdiDocumentContainer::documentChanged( IDocument *document )
{
    QList<QMdiSubWindow *> mdiSubwindows = m_area->subWindowList();
    foreach ( QMdiSubWindow *subwindow, mdiSubwindows )
        if ( subwindow->widget() == document->widget() ) {
            subwindow->setWindowTitle(document->title());
            break;
        }
}

void MdiDocumentContainer::updateDocumentIcon( IDocument *document )
{
    QList<QMdiSubWindow *> mdiSubwindows = m_area->subWindowList();
    foreach ( QMdiSubWindow *subwindow, mdiSubwindows )
        if ( subwindow->widget() == document->widget() ) {
            subwindow->setWindowIcon(QIcon(document->iconName(document->isModified())));
            break;
        }
}

QList<IDocument *> MdiDocumentContainer::documentsOrdered() const
{
    QList<IDocument *> documents;

    QList<QMdiSubWindow *> mdiSubwindows = m_area->subWindowList();
    foreach ( QMdiSubWindow *subwindow, mdiSubwindows )
        documents << m_documents[subwindow->widget()];

    return documents;
}

void MdiDocumentContainer::restoreState()
{
    restoreSettings();

    if ( m_maximizedMode ) {
        if ( QMdiSubWindow *child = m_area->activeSubWindow() )
            child->showMaximized();
    } else if ( m_tileMode ) {
        m_area->tileSubWindows();
    } else {
        m_area->cascadeSubWindows();
    }
}

QList<QAction *> MdiDocumentContainer::createAdditionalWindowsActions()
{
    QList<QAction *> actions;

    QMainWindow *mainWindow = m_mainWindow->widget();

    m_cmdTileWindows    = new Core::Command( tr("TileWindows"),
                                             QIcon(":/images/win_tile.png"),
                                             tr("&Tile Windows"),
                                             tr("Tile Document Windows"),
                                             QKeySequence(tr("Ctrl+E,T")),
                                             mainWindow );

    m_cmdCascadeWindows = new Core::Command( tr("CascadeWindow"),
                                             QIcon(":/images/win_cascade.png"),
                                             tr("&Cascade Windows"),
                                             tr("Cascade Document Windows"),
                                             QKeySequence(tr("Ctrl+E,C")),
                                             mainWindow );

    m_cmdToggleMaxMode  = new Core::Command( tr("ToggleMaximezedWindows"),
                                             QIcon(":/images/win_maximize.png"),
                                             tr("&Maximize"),
                                             tr("Toggle Maximized Windows Mode"),
                                             QKeySequence(tr("Ctrl+E,M")),
                                             mainWindow );

    m_cmdRestoreAllMinimized = new Core::Command( tr("RestoreAllMinimized"),
                                                  tr("Restore All Minimized"),
                                                  tr("Restore All Minimized Windows"),
                                                  QKeySequence(tr("Ctrl+E,R")),
                                                  mainWindow );

    m_cmdToggleMaxMode->action()->setCheckable(true);
    m_cmdToggleMaxMode->action()->setChecked(m_maximizedMode);

    connect(m_cmdTileWindows->action(),     SIGNAL(triggered()),    this, SLOT(tileSubWindows()));
    connect(m_cmdCascadeWindows->action(),  SIGNAL(triggered()),    this, SLOT(cascadeSubWindows()));
    connect(m_cmdToggleMaxMode->action(),   SIGNAL(toggled(bool)),  this, SLOT(toggleMaximizedWindows(bool)));
    connect(m_cmdRestoreAllMinimized->action(), SIGNAL(triggered()), this, SLOT(restoreAllMinimized()));

    actions << m_cmdTileWindows->action()
            << m_cmdCascadeWindows->action()
            << m_cmdToggleMaxMode->action()
            << m_cmdRestoreAllMinimized->action();

    return actions;
}

bool MdiDocumentContainer::eventFilter( QObject *obj, QEvent *ev )
{
    if ( ev->type() == QEvent::MouseButtonPress ) {
        QMouseEvent *me = static_cast<QMouseEvent *>(ev);
        if ( me->button() == Qt::LeftButton )
            m_lbuttonPressed = true;
        return QObject::eventFilter(obj, ev);
    }

    if ( ev->type() == QEvent::MouseButtonRelease ) {
        QMouseEvent *me = static_cast<QMouseEvent *>(ev);
        if ( me->button() == Qt::LeftButton )
            m_lbuttonPressed = false;
        return QObject::eventFilter(obj, ev);
    }

    if ( m_area == obj && ev->type() == QEvent::Resize ) {
        m_cwsChanged = true;
        return QObject::eventFilter(obj, ev);
    }

    if ( m_tileMode && !m_tiling ) {
        if ( ev->type() == QEvent::MouseMove && m_lbuttonPressed )
            return true;
        if ( ev->type() == QEvent::Resize && !m_cwsChanged && m_lbuttonPressed ) {
            m_cwsChanged = false;
            return true;
        }
    }

    return QObject::eventFilter(obj, ev);
}

void MdiDocumentContainer::onSubWindowActivated( QMdiSubWindow *subwindow )
{
    IDocument *document = 0;
    if ( subwindow )
        document = m_documents[subwindow->widget()];
    emit currentDocumentChanged(document);
}

void MdiDocumentContainer::onSubWindowShowStateChanged( QMdiSubWindow *subwindow )
{
    if ( m_addingSubwindow )
        return;

    if ( subwindow ) {
        m_blockToggleSignal = true;
        m_cmdToggleMaxMode->action()->setChecked(subwindow->isMaximized());
        m_blockToggleSignal = false;

        if ( !subwindow->isMaximized() && m_tileMode )
            tileSubWindows();
    }
}

void MdiDocumentContainer::onSubWindowClosed()
{
    if ( m_maximizedMode ) {
        // Repair Maximized Mode State
        if ( QMdiSubWindow *subWindow = m_area->activeSubWindow() )
            subWindow->showMaximized();
    } else if ( m_tileMode )
        tileSubWindows();
}

void MdiDocumentContainer::tileSubWindows()
{
    m_tiling = true;

    m_blockToggleSignal = true;
    m_cmdToggleMaxMode->action()->setChecked(false);
    m_blockToggleSignal = false;
    m_tileMode = true;
    m_area->tileSubWindows();

    m_tiling = false;
}

void MdiDocumentContainer::cascadeSubWindows()
{
    m_blockToggleSignal = true;
    m_cmdToggleMaxMode->action()->setChecked(false);
    m_blockToggleSignal = false;
    m_tileMode = false;
    m_area->cascadeSubWindows();
}

void MdiDocumentContainer::toggleMaximizedWindows( bool onoff )
{
    m_maximizedMode = onoff;
    if ( m_blockToggleSignal )
        return;
    if ( QMdiSubWindow *subWindow = m_area->activeSubWindow() ) {
        if ( onoff )
            subWindow->showMaximized();
        else {
            subWindow->showNormal();
            if ( m_tileMode )
                tileSubWindows();
        }
    }
}

void MdiDocumentContainer::restoreAllMinimized()
{
    foreach ( QMdiSubWindow *subwindow, m_area->subWindowList() ) {
        qDebug() << "RRR" << subwindow->isMinimized() << subwindow->isShaded();
        if ( subwindow->isMinimized() )
            subwindow->showNormal();
    }
}

void MdiDocumentContainer::saveSettings()
{
    QWorkMainWindow *mainWindow = QWorkMainWindow::getInstance();
    QSettings settings(mainWindow->orgName(), mainWindow->appName());

    settings.beginGroup("MdiDocumentContainer_State");
    settings.setValue("maximized", m_maximizedMode);
    settings.setValue("tiled", m_tileMode);
    settings.endGroup();
}

void MdiDocumentContainer::restoreSettings()
{
    QWorkMainWindow *mainWindow = QWorkMainWindow::getInstance();
    QSettings settings(mainWindow->orgName(), mainWindow->appName());

    settings.beginGroup("MdiDocumentContainer_State");
    m_maximizedMode = settings.value("maximized", false).toBool();
    m_tileMode = settings.value("tiled", false).toBool();
    settings.endGroup();
}

//////////////////////////////////////////////////////////////////////////////////////
/// MdiArea...
Frame::Frame(QWidget *docWidget, MdiArea *area , MdiDocument *document )
   : QFrame(area), vbox(this), layout(this)
   , m_area(area)
   , m_document(document)
   , m_documentWidget(docWidget)
   , m_titleBar(document->document()->title(), this)
{
   m_mouse_down = false;
   setFrameShape(Panel);
   setLineWidth(2);
   // Make this a borderless window which can't
   // be resized or moved via the window system
//   setWindowFlags(Qt::FramelessWindowHint);
   this->setMouseTracking(true);
   docWidget->setMouseTracking(true);
   docWidget->setAutoFillBackground(true);

   vbox.addWidget(&m_titleBar);
   vbox.setMargin(0);
   vbox.setSpacing(0);

//   win.menuBar()->setFixedHeight(19);
//   layout.addWidget(win.menuBar());
   layout.addWidget(m_documentWidget);

   layout.setMargin(0);
   layout.setSpacing(0);
   vbox.addLayout(&layout);
}

TitleBar *Frame::titleBar() {
    return &m_titleBar;
}

QWidget *Frame::documentWidget() const
{
    return m_documentWidget;
}

void Frame::activate( bool notifyArea)
{
    if ( notifyArea )
        m_area->setActiveSubWindow(m_document);
    raise();
}

void Frame::mousePressEvent( QMouseEvent *e )
{
    m_old_pos = e->pos();
    m_mouse_down = e->button() == Qt::LeftButton;
}

void Frame::mouseMoveEvent( QMouseEvent *e )
{
   int x = e->x();
   int y = e->y();

   std::cout
         << "x = " << x << std::endl
         << "y = " << y << std::endl;

   if (m_mouse_down){
      int dx = x - m_old_pos.x();
      int dy = y - m_old_pos.y();
      QRect g = geometry();

      if (left)
         g.setLeft(g.left() + dx);

      if (right)
         g.setRight(g.right() + dx);

      if (bottom)
         g.setBottom(g.bottom() + dy);

      setGeometry(g);
      m_old_pos = QPoint(!left ? e->x() : m_old_pos.x(), e->y());

      return;
   }
   QRect r = rect();
   left = r.left() - x <= 4 && r.left() - x >= -1;
   right = x - r.right() <= 4 && x - r.right() >= -1;
   bottom = y - r.bottom() <= 4 && y - r.bottom() >= -1;
   bool hor = left | right;

   std::cout
         << "hor    = " << hor << std::endl
         << "bottom = " << bottom << std::endl
         << "left   = " << left << std::endl
         << "right  = " << right << std::endl << std::endl;

   if (hor && bottom) {
      if (left) {
         setCursor(Qt::SizeBDiagCursor);
      } else {
         setCursor(Qt::SizeFDiagCursor);
      }
   } else if (hor){
      setCursor(Qt::SizeHorCursor);
   }else if (bottom){
      setCursor(Qt::SizeVerCursor);
   }else{
      setCursor(Qt::ArrowCursor);
   }
}

void Frame::mouseReleaseEvent( QMouseEvent *e )
{
    Q_UNUSED(e)
    m_mouse_down = false;
}

void Frame::enterEvent( QEvent *event )
{
    Q_UNUSED(event)
    activate();
}

TitleBar::TitleBar( const QString &title, QWidget *parent )
   : QWidget(parent),label(this),minimize(this),maximize(this)
   , close(this),space(this),hbox(this)
{
    // Don't let this widget inherit the parent's background color
    setAutoFillBackground(true);
    // Use a brush with a Highlight color role to render the background
    setBackgroundRole(QPalette::Highlight);
    space.setFixedWidth(5);
    // Use the style to set the button pixmaps
    QPixmap pix;
    pix.load(":/image/close.png");
    close.setIcon(pix);
    close.setIconSize(QSize(42, 17));
    maxPix.load(":/image/maximize.png");
    maximize.setIcon(maxPix);
    maximize.setIconSize(QSize(24, 17));
    pix.load(":/image/minimize.png");
    minimize.setIcon(pix);
    minimize.setIconSize(QSize(24, 17));
    restorePix.load(":/image/restore.png");
    minimize.setFixedSize(24, 17);
    close.setFixedSize(42, 17);
    maximize.setFixedSize(24, 17);
    label.setText(title);
    parent->setWindowTitle(title);
    hbox.addWidget(&label);
    hbox.addWidget(&minimize);
    hbox.addWidget(&maximize);
    hbox.addWidget(&space);
    hbox.addWidget(&close);
    hbox.setContentsMargins(2,2,2,2);
    hbox.insertStretch(1, 500);
    hbox.setSpacing(0);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    maxNormal = false;
//    connect(&close, SIGNAL( clicked() ), parent, SLOT(close() ) );
    connect(&minimize, SIGNAL( clicked() ), this, SLOT(showSmall() ) );
    connect(&maximize, SIGNAL( clicked() ), this, SLOT(showMaxRestore() ) );
}
void TitleBar::showSmall(){
    parentWidget()->showMinimized();
}
void TitleBar::showMaxRestore(){
    if (maxNormal){
        parentWidget()->showNormal();
        maxNormal = !maxNormal;
        maximize.setIcon(maxPix);
    }else{
        parentWidget()->showMaximized();
        maxNormal = !maxNormal;
        maximize.setIcon(restorePix);
    }
}
void TitleBar::mousePressEvent( QMouseEvent *me )
{
    clickPos = me->pos();
}
void TitleBar::mouseMoveEvent( QMouseEvent *me )
{
    if (maxNormal)
        return;
    parentWidget()->move(parentWidget()->mapToParent(mapToParent(me->pos() - clickPos)));
}

MdiDocument::MdiDocument( IDocument *document, MdiArea *area, Qt::WindowFlags flags )
    : QMdiSubWindow(area, flags)
    , m_document(document)
{
    m_frame = new Frame(document->widget(), area, this);
}

bool MdiDocument::isMyWidget( QWidget *widget_ ) const
{
    return m_frame->documentWidget() == widget_;
}

QWidget *MdiDocument::myWidget() const
{
    return m_frame->documentWidget();
}

IDocument *MdiDocument::document() const
{
    return m_document;
}

void MdiDocument::activate( bool notifyArea )
{
    m_frame->activate(notifyArea);
}

QWidget *MdiDocument::widget() const
{
    return QMdiSubWindow::widget();
}

MdiArea::MdiArea( Mode mode, QWidget *parent )
    : QMdiArea(parent)
    , m_mode(mode)
{

}

QList<IDocument *> MdiArea::documentList() const
{
    QList<IDocument *> result;
    foreach ( MdiDocument *child, m_documents )
        result << child->document();
    return result;
}

MdiDocument *MdiArea::documentForWidget( QWidget *widget ) const
{
    foreach ( MdiDocument *child, m_documents )
        if ( child->isMyWidget(widget) )
            return child;

    return 0;
}

QWidget *MdiArea::activeWidget() const
{
    if ( QMdiSubWindow *child = activeSubWindow() ) {
        foreach ( MdiDocument *mdiDoc, m_documents )
            if ( mdiDoc == child )
                return mdiDoc->myWidget();
    }

    return 0;
}

void MdiArea::addDocument( IDocument *document )
{
    if ( document ) {
        if ( document->widget() ) {
            MdiDocument *child = new MdiDocument(document, this, Qt::Window | Qt::FramelessWindowHint);
            m_documents.append(child);
            child->setWindowTitle(document->title());
            addSubWindow(child);
            child->show();
            setActiveSubWindow(child);
        }
    }
}

void MdiArea::removeWidget( QWidget *widget )
{
    for (int i = 0; i < m_documents.size(); i++)
        if ( m_documents[i]->isMyWidget(widget) ) {
            removeSubWindow(m_documents[i]);
            m_documents.removeAt(i);
            break;
        }
}

MdiAreaContainer::MdiAreaContainer( QObject *parent )
    : IDocumentContainer(QWorkMainWindow::getInstance(), parent)
    , m_area(new MdiArea(MdiArea::Default, QWorkMainWindow::getInstance()))
{
    connect(m_area, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(onSubWindowActivated(QMdiSubWindow*)));
}

QWidget *MdiAreaContainer::createWidget()
{
    return m_area;
}

void MdiAreaContainer::addWidget( QWidget *widget )
{
    m_area->addDocument(m_documents[widget]);
}

void MdiAreaContainer::removeWidget( QWidget *widget )
{
    m_area->removeWidget(widget);
}

void MdiAreaContainer::setCurrentWidget( QWidget *widget )
{
    if (  MdiDocument *child = m_area->documentForWidget(widget) )
        m_area->setActiveSubWindow(child);
}

void MdiAreaContainer::documentChanged( IDocument *document )
{
    if ( MdiDocument *child = m_area->documentForWidget(document->widget()) )
        child->setWindowTitle(document->title());   // TODO:
}

void MdiAreaContainer::updateDocumentIcon( IDocument *document )
{
    if ( MdiDocument *child = m_area->documentForWidget(document->widget()) )
        child->setWindowIcon(QIcon(document->iconName(document->isModified())));    // TODO
}

QList<IDocument *> MdiAreaContainer::documentsOrdered() const
{
    return m_area->documentList();
}

void MdiAreaContainer::onSubWindowActivated( QMdiSubWindow *subwindow )
{
    IDocument *document = 0;
    if ( MdiDocument *doc = qobject_cast<MdiDocument *>(subwindow) ) {
        document = doc->document();
        doc->activate();
    }

    emit currentDocumentChanged(document);
}

//////////////////////////////////////////////////////////////////////////////////////
/// TabDocumentContainer
TabDocumentContainer::TabDocumentContainer( QObject *parent )
    : IDocumentContainer(QWorkMainWindow::getInstance(), parent)
{
    QWorkMainWindow *mainWindow = QWorkMainWindow::getInstance();

    s_splitterWatcherIndex = Utils::StyleManager::registerQssFileWatcher(QLatin1String("TabWidgetSplitterHolder"));
    s_tabWidgetWatcherIndex = Utils::StyleManager::registerQssFileWatcher(QLatin1String("ContainerTabBar"));

    m_splitter = new TabWidgetSplitterHolder(this, mainWindow);
    ContainerTabWidget *t = m_splitter->mainTabWidget();
    connect(t, SIGNAL(currentChanged(int)), this, SLOT(onCurrentChanged(int)));
    connect(t, SIGNAL(tabCloseRequested(int)), this, SLOT(onDocumentCloseRequested(int)));
}

QWidget *TabDocumentContainer::createWidget()
{
    return m_splitter;
}

void TabDocumentContainer::addWidget( QWidget *widget )
{
    if ( QFrame *frame = dynamic_cast<QFrame *>(widget) ) {
        frame->setFrameShape(QFrame::NoFrame);
        frame->setLineWidth(0);
    }

    IDocument *document = m_documents[widget];
    ContainerTabWidget *t = m_splitter->currentTabWidget(lastActiveWidget());
    int tabIndex = t->addTab( widget, document->title());
    if ( widget != document->editorWidget() )
        widget->setFocusProxy(document->editorWidget());
    t->setTabToolTip(tabIndex, document->toolTip());
}

void TabDocumentContainer::removeWidget( QWidget *widget )
{
    if ( ContainerTabWidget *t = m_splitter->tabForWidget(widget) ) {
        t->removeTab(t->indexOf(widget));
        if ( !t->count() )
            m_splitter->clearEmptyPane(t);
    }
}

void TabDocumentContainer::setCurrentWidget( QWidget *widget )
{
    if ( ContainerTabWidget *t = m_splitter->tabForWidget(widget) )
        t->setCurrentWidget(widget);
}

void TabDocumentContainer::documentChanged( IDocument *document )
{
    QWidget *w = document->widget();
    if ( ContainerTabWidget *t = m_splitter->tabForWidget(w) ) {
        int tabIndex = t->indexOf(w);
        t->setTabText(tabIndex, document->title());
        t->setTabToolTip(tabIndex, document->toolTip());
    }
}

void TabDocumentContainer::updateDocumentIcon( IDocument *document )
{
    QWidget *w = document->widget();
    if ( ContainerTabWidget *t = m_splitter->tabForWidget(w) ) {
        int tabIndex = t->indexOf(w);
        t->setTabIcon(tabIndex, QIcon(document->iconName(document->isModified())));
    }
}

QList<IDocument *> TabDocumentContainer::documentsOrdered() const
{
    QList<IDocument *> documents;
    QList<QWidget *> widgets = m_splitter->widgetsOrdered();
    foreach ( QWidget *w, widgets )
        documents << m_documents[w];

    return documents;
}

void TabDocumentContainer::restoreState()
{ }

QList<QAction *> TabDocumentContainer::createAdditionalWindowsActions()
{
    QList<QAction *> actions;

    QMainWindow *mainWindow = m_mainWindow->widget();

    m_cmdSplit                  = new Core::Command( tr("Split"),
                                                     QIcon(":/images/split.png"),
                                                     tr("&Split"),
                                                     tr("Split Editor View"),
                                                     QKeySequence(tr("Ctrl+E,2")),
                                                     mainWindow );

    m_cmdSplitToggleOrientation = new Core::Command( tr("SplitToggleOrintation"),
                                                     QIcon(":/images/split_toggle_orient.png"),
                                                     tr("&Toggle Split Orientation"),
                                                     tr("Toggle Split Orientation"),
                                                     QKeySequence(tr("Ctrl+E,1")),
                                                     mainWindow );

    m_cmdRemoveSplit            = new Core::Command( tr("SplitRemove"),
                                                     QIcon(":/images/split_remove.png"),
                                                     tr("&Remove Split"),
                                                     tr("Remove Last Splitting"),
                                                     QKeySequence(tr("Ctrl+E,0")),
                                                     mainWindow );

    connect(m_cmdSplit->action(),                   SIGNAL(triggered()), this, SLOT(split()));
    connect(m_cmdSplitToggleOrientation->action(),  SIGNAL(triggered()), this, SLOT(toggleSplitOrientation()));
    connect(m_cmdRemoveSplit->action(),             SIGNAL(triggered()), this, SLOT(removeSplit()));

    actions << m_cmdSplit->action()
            << m_cmdSplitToggleOrientation->action()
            << m_cmdRemoveSplit->action();

    return actions;
}

void TabDocumentContainer::onCurrentChanged( int index )
{
    IDocument *document = 0;
    ContainerTabWidget *t = qobject_cast<ContainerTabWidget *>(sender());

    if ( index != -1 && t )
        document = m_documents[t->widget(index)];

    emit currentDocumentChanged(document);
}

void TabDocumentContainer::onDocumentCloseRequested( int index )
{
    ContainerTabWidget *t = qobject_cast<ContainerTabWidget *>(sender());

    if ( index != -1 && t )
        emit closeDocumentRequested(m_documents[t->widget(index)]);
}

void TabDocumentContainer::onTabMoveRequested( ContainerTabBar *sourceTabBar, int index, ContainerTabWidget *targetTabWidget )
{
    ContainerTabWidget *t = qobject_cast<ContainerTabWidget *>(sourceTabBar->parent());

    QWidget *w = t->widget(index);
    QString title = sourceTabBar->tabText(index);
    QString toolTip = sourceTabBar->tabToolTip(index);
    QIcon icon = sourceTabBar->tabIcon(index);

    t->removeTab(index);
    int addedIndex = targetTabWidget->addTab(w, icon, title);
    targetTabWidget->setTabToolTip(addedIndex, toolTip);
    targetTabWidget->setCurrentIndex(addedIndex);

    if ( !t->count() )
        m_splitter->clearEmptyPane(t);
}

void TabDocumentContainer::onCurrentWidgetChanged( ContainerTabBar *t )
{
    ContainerTabWidget *tw = qobject_cast<ContainerTabWidget *>(t->parent());
    IDocument *document = m_documents[tw->currentWidget()];
    emit currentDocumentChanged(document);
    document->refreshDocumentCursor();
}

void TabDocumentContainer::split()
{
    if ( QWidget *lastActiveWidget_ = lastActiveWidget() ) {
        if ( ContainerTabWidget *t = m_splitter->split(lastActiveWidget_) ) {
            connect(t, SIGNAL(currentChanged(int)), this, SLOT(onCurrentChanged(int)));
            connect(t, SIGNAL(tabCloseRequested(int)), this, SLOT(onDocumentCloseRequested(int)));
            DocumentManager::getInstance()->activateDocument(m_documents[lastActiveWidget_]);
        }
    }
}

void TabDocumentContainer::toggleSplitOrientation()
{
    m_splitter->toggleOrientation();
}

void TabDocumentContainer::removeSplit()
{
    if ( ContainerTabWidget *t = m_splitter->currentTabWidget(lastActiveWidget()) )
        m_splitter->removeSplit(t);
}

QWidget *TabDocumentContainer::lastActiveWidget() const
{
    if ( m_docHistory.isEmpty() )
        return 0;

    return m_docHistory.first()->widget();
}

//////////////////////////////////////////////////////////////////////////////////////
/// DocumentFactory
void DocumentFactory::loadDocumentPlugins()
{
    QDir pluginsDir;
    pluginsDir = QDir(qApp->applicationDirPath());
    pluginsDir.cd("editors");

    Core::LogManager::addMessage( Core::LogMessage( QObject::tr("Editors-Plugins Loading Started..."),
                                                    QObject::tr("Plugins"), Core::LogMessage::Information,
                                                    QColor(255, 255, 192) ) );

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if ( plugin ) {
            IDocumentPlugin *documentPlugin = qobject_cast<IDocumentPlugin *>(plugin);
            if ( documentPlugin ) {
                documentPlugin->registerSettings();
                m_documentPlugins.append(documentPlugin);
                Core::LogManager::addMessage( Core::LogMessage( QObject::tr("  Plugin %1 Loaded").arg(fileName),
                                                                QObject::tr("Plugins"), Core::LogMessage::Information,
                                                                QColor(255, 255, 192) ) );
            }
        }
    }

    PlainTextDocumentPlugin *plainDocumentPlugin = new PlainTextDocumentPlugin();
    plainDocumentPlugin->registerSettings();
    m_documentPlugins.append(plainDocumentPlugin);
    Core::LogManager::addMessage( Core::LogMessage( QObject::tr("  Plugin [plainDocumentPlugin] Loaded"),
                                                    QObject::tr("Plugins"), Core::LogMessage::Information,
                                                    QColor(255, 255, 192) ) );

    Core::LogManager::addMessage( Core::LogMessage( QObject::tr("Plugins Loaded"),
                                                    QObject::tr("Plugins"), Core::LogMessage::Information,
                                                    QColor(255, 255, 192) ) );
}

IDocument *DocumentFactory::newDocument( QString &extension )
{
    if ( m_documentPlugins.size() == 1 ) {
        extension = m_documentPlugins[0]->defaultExtension();
        return m_documentPlugins[0]->doCreateDocument();
    }

    extension = "";

    QList <IDocumentPlugin *> availablePlugins;
    foreach ( IDocumentPlugin *documentPlugin, m_documentPlugins )
        if ( documentPlugin->isAvailableInNewDocumentDialog() )
            availablePlugins << documentPlugin;

    QStringList descriptions;
    foreach ( IDocumentPlugin *documentPlugin, availablePlugins )
        descriptions << documentPlugin->description();

    ListSelectorDialog selectorDialog(QObject::tr("New Document"), descriptions, QWorkMainWindow::getInstance());
    if ( selectorDialog.exec() != QDialog::Accepted )
        return 0;

    int pluginIndex = selectorDialog.getCurrentIndex();
    if ( pluginIndex == -1 )
        return 0;
    else if ( pluginIndex == -2 ) {
        Core::WizardManager::showNewItemDialog(QObject::tr("Create new..."));
        return 0;
    }

    extension = availablePlugins[pluginIndex]->defaultExtension();
    return availablePlugins[pluginIndex]->doCreateDocument();
}

//////////////////////////////////////////////////////////////////////////////////////
/// QWorkMainWindow
QWorkMainWindow *QWorkMainWindow::m_mainWindow(0);

void QWorkMainWindow::parseArguments( bool showErrorMessage )
{
    QStringList args = QApplication::arguments();
    QString errorMessage = "";

    args.removeFirst();

    foreach ( const QString &arg, args ) {
        if ( arg.toUpper() == QLatin1String("-M") || arg.toUpper() == QLatin1String("--MDI") )
            m_centralWidgetType = Mdi;
        else
            errorMessage += tr("Invalid Argument '%1' of Command Line\n").arg(arg);
    }

    if ( showErrorMessage && !errorMessage.isEmpty() ) {
        errorMessage += "\nUSAGE: QWorkIde [-m|--mdi]";
        warning(errorMessage);
    }
}

QWorkMainWindow::QWorkMainWindow( const QString &orgName_, const QString &appName_, QWidget *parent )
    : QMainWindow(parent)
    , m_orgName(orgName_)
    , m_appName(appName_)
    , m_isStatusBarHidden(false)
    , m_centralWidgetType(Tabbed)
    , m_modeManager(new Core::ModeManager)
{
    qApp->setStyleSheet("QMainWindow::separator { background: gray; width: 1px; height: 1px; }"
                       "QMainWindow::separator:hover { background: black; }");

    m_mainWindow = this;
    Core::Storage::registerMainWindow(this);

    m_wizardManager = new Core::WizardManager;

    parseArguments();

    Utils::Theme *theme = new Utils::Theme(qApp);
    Utils::setCreatorTheme(theme);

    m_sessionManager = new SessionManager(m_orgName, m_appName, this);
    m_closeReflector = new CloseApplicationHandler;

    new TextEditor::TextEditorSettings(this);

    m_mimeDatabase = new Core::MimeDatabase;
    QString errorMessage;
    m_mimeDatabase->addMimeTypes(QLatin1String(":/TextEditor.mimetypes.xml"), &errorMessage);

    m_settingsManager = new SettingsManager();
    m_commandManager  = new Core::CommandManager(this);
    m_navigationManager = new NavigationManager();

    m_styleManager = new Utils::StyleManager(this);
    m_progressManager = new Core::Internal::ProgressManagerPrivate();
    m_progressManager->init();

    createNavigateActions();

    m_logManager = new Core::LogManager(this);
    m_highlighterFormatsManager = new HighlighterFormatsManager;

    m_scriptsMenu = new QMenu(tr("S&cripts"));
    m_pythonScriptingManager = new Python::PythonScriptManager(m_scriptsMenu);

    IDocumentContainer *container;
    switch ( m_centralWidgetType ) {
        case Mdi:
             container = new MdiDocumentContainer();
//            container = new MdiAreaContainer();
            break;
        default:
            container = new TabDocumentContainer();
    }

    m_documentManager = new DocumentManager(this, new DocumentFactory, container);
    new CodeModelManager(this);
    /// TODO: load plugins
    m_cppModelManager = new CppTools::Internal::CppModelManager(this);
    m_documentManager->loadDocumentPlugins();

    Python::DocumentManagerPythonModule *module = new Python::DocumentManagerPythonModule();
    Python::PythonScriptManager::getInstance()->registerModule(module, true, QStringList() << "Document");
    connect(m_documentManager, SIGNAL(currentDocumentChanged(IDocument*)), m_pythonScriptingManager, SLOT(currentDocumentChanged(IDocument*)));

    m_navigationManager->init();

    connect(m_documentManager, SIGNAL(addToRecentFiles(QString)), this, SLOT(addToRecentFiles(QString)));
    connect(m_documentManager, SIGNAL(updateRecentFile(QString,QString)), this, SLOT(updateRecentFile(QString,QString)));

    TextPositionStatusBarWidget *statusWidget = new TextPositionStatusBarWidget(this);
    connect(m_documentManager, SIGNAL(currentDocumentChanged(IDocument*)),           statusWidget, SLOT(onCurrentDocumentChanged(IDocument*)));
    connect(m_documentManager, SIGNAL(currentDocumentPositionChanged(IDocument*)),   statusWidget, SLOT(onCurrentDocumentChanged(IDocument*)));

    TasksStatusWidget *tasksStatusWidget = new TasksStatusWidget(this);
    connect(m_progressManager, SIGNAL(tasksStatusChanged(Core::TasksStatus)), tasksStatusWidget, SLOT(onTasksStatusChanged(Core::TasksStatus)));
    QwCustomDockWidget *tasksDock = new QwCustomDockWidget( this, tr("Tasks"), "Core.TasksDockWidget",
                                                            QKeySequence(), QIcon(),
                                                            Qt::LeftDockWidgetArea, QwCustomDockWidget::Third);
    tasksDock->setWidget(m_progressManager->progressView());
    addDockWidgetHandler(new Core::DockWidgetHandler(tasksDock, new Core::EditModeDockGuiClient(tasksDock)));

    statusBar()->addPermanentWidget(tasksStatusWidget);
    statusBar()->addPermanentWidget(statusWidget);

    createActions();
    createMenus();
    createToolBars();

    connect(m_documentManager, SIGNAL(currentDocumentChanged(IDocument*)),           m_documentManager, SLOT(updateEditMenu()));
    connect(m_documentManager, SIGNAL(currentDocumentPositionChanged(IDocument*)),   m_documentManager, SLOT(updateEditMenu()));

    m_searchEngine = new SearchEngine(m_searchMenu);

    m_searchMenu->addSeparator();
    m_windowsMenu = m_searchMenu->addMenu(tr("&Windows"));
    m_windowsMenu->addActions(m_documentManager->createWindowsActions());
    m_searchMenu->addSeparator();
    m_searchMenu->addAction(m_cmdNavigateBack->action());
    m_searchMenu->addAction(m_cmdNavigateForward->action());
    connect(m_cmdNavigateForward->action(), SIGNAL(triggered()), this, SLOT(onNavigateForward()));
    connect(m_cmdNavigateBack->action(),    SIGNAL(triggered()), this, SLOT(onNavigateBack()));

    setCentralWidget(m_documentManager->centralWidget());

    statusBar()->showMessage(tr("Ready"));
    viewMenu->addSeparator();
    viewMenu->addAction(m_cmdToggleStatusBar->action());
    viewMenu->addAction(m_cmdToggleFullScreenMode->action());
    m_viewToolBar->addSeparator();
    m_viewToolBar->addAction(m_cmdToggleStatusBar->action());

    loadPlugins();
    populateUiFromPlugins();
    initializePlugins();
    initializeDocumentPlugins();

    initializeSnippets();

    m_pythonScriptingManager->registerModule( new Python::PluginScriptModule(&m_plugins) );

    Python::ProjectManagerPythonModule *projectManagerModule = new Python::ProjectManagerPythonModule();
    Python::PythonScriptManager::getInstance()->registerModule(projectManagerModule, true, QStringList() << "Project");

    readSettings();

    tasksStatusWidget->initStatus(tasksDock);

    m_modeManager->switchToMode(0);
    m_sessionManager->switchToSession();

    Core::LogManager::getInstance()->setAutoResizeRowHeight(true);
    addToggleAction(m_logManager->toggleAction());
    addToggleAction(m_pythonScriptingManager->toggleAction());

    container->restoreState();

    m_pythonScriptingManager->initScritingRopeProject();

    setAcceptDrops(true);
}

void QWorkMainWindow::initializePlugins()
{
    foreach ( IPlugin *plugin, m_plugins )
        plugin->initialize();
}

void QWorkMainWindow::initializeDocumentPlugins()
{
    m_documentManager->initializeDocumentPlugins();
}

QWorkMainWindow::~QWorkMainWindow()
{
    delete m_wizardManager;
    delete m_projectManager;
    delete m_bookmarkManager;

    delete m_documentManager;
    delete m_settingsManager;
    delete m_commandManager;
    delete m_navigationManager;
    delete m_logManager;
    delete m_styleManager;
    delete m_pythonScriptingManager;
    delete m_searchEngine;

    m_progressManager->cancelAllRunningTasks();
    delete m_progressManager;

    delete m_mimeDatabase;

    m_plugins.clear();

    qDeleteAll(m_completionAssistProviders);

    delete m_closeReflector;
    delete m_sessionManager;

    qDeleteAll(m_dockHandlers);
    delete m_modeManager;
    delete m_highlighterFormatsManager;
}

void QWorkMainWindow::addDockWidgetHandler(Core::DockWidgetHandler *dockHandler )
{
    QwCustomDockWidget::DockAreaTabifyHint dockPlaceHint =
            qMakePair(dockHandler->m_dock->initialDockArea(),
                      dockHandler->m_dock->initialTabifyHint());
    if ( m_dockHintMap.find(dockPlaceHint) == m_dockHintMap.end() )
        QMainWindow::addDockWidget(dockHandler->m_dock->initialDockArea(), dockHandler->m_dock);
    else
        tabifyDockWidget(m_dockHintMap[dockPlaceHint], dockHandler->m_dock);
    m_dockHintMap[dockPlaceHint] = dockHandler->m_dock;
    m_dockHandlers.append(dockHandler);
}

QString QWorkMainWindow::applicationDataLocation() const
{
#ifndef HAVE_QT5
    QString result = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
#else
    QString result = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#endif
    result += "/" + m_appName;
    return result;
}

QWorkMainWindow* QWorkMainWindow::getInstance()
{
    return m_mainWindow;
}

Core::ProgressManager *QWorkMainWindow::progressManager() const
{
    return m_progressManager;
}

Core::MimeDatabase *QWorkMainWindow::mimeDatabase() const
{
    return m_mimeDatabase;
}

void QWorkMainWindow::loadPlugins()
{
    m_plugins.clear();

    Core::LogManager::addMessage(Core::LogMessage(tr("Plugins Loading Started..."), tr("Plugins"), Core::LogMessage::Information, QColor(255, 255, 192)));
    Core::LogManager::addMessage(Core::LogMessage(tr("  Project Explorer Plugin Loading Started..."), tr("Plugins"), Core::LogMessage::Information, QColor(255, 255, 192)));
    m_projectManager = new ProjectExplorer::ProjectManager();
    m_plugins.append(m_projectManager);
    Core::LogManager::addMessage(Core::LogMessage(tr("  Project Explorer Plugin Loaded"), tr("Plugins"), Core::LogMessage::Information, QColor(255, 255, 192)));

    Utils::HistoryCompleter::setSettings(ProjectExplorer::ProjectManager::settings());

    QDir pluginsDir;
    pluginsDir = QDir(qApp->applicationDirPath());
    pluginsDir.cd("plugins");

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if ( plugin ) {
            IPlugin *iPlugin = qobject_cast<IPlugin *>(plugin);
            if ( iPlugin ) {
                iPlugin->registerSettings();
                m_plugins.append(iPlugin);
                Core::LogManager::addMessage( Core::LogMessage( tr("  Plugin %1 Loaded").arg(fileName),
                                                                tr("Plugins"), Core::LogMessage::Information,
                                                                QColor(255, 255, 192) ) );
            }
        }
    }

    foreach ( IPlugin *iPlug, m_plugins ) {
        iPlug->registerSignals();
        iPlug->registerSlots();
    }

    m_bookmarkManager = new BookmarkPlugin();
    m_bookmarkManager->registerSettings();
    m_plugins.append(m_bookmarkManager);
    m_bookmarkManager->registerSignals();
    m_bookmarkManager->registerSlots();

    Core::LogManager::addMessage(Core::LogMessage(tr("Plugins Loaded"), tr("Plugins"), Core::LogMessage::Information, QColor(255, 255, 192)));
}

void QWorkMainWindow::createNavigateActions()
{
    m_cmdNavigateForward    = new Core::Command( tr("GoForward"),
                                                 QIcon(":/images/sr_next.png"),
                                                 tr("Go Forward"),
                                                 tr("Goto Forward"),
                                                 QKeySequence(tr("Alt+Right")),
                                                 this );

    m_cmdNavigateBack       = new Core::Command( tr("GoBack"),
                                                 QIcon(":/images/sr_prev.png"),
                                                 tr("Go Back"),
                                                 tr("Goto Back"),
                                                 QKeySequence(tr("Alt+Left")),
                                                 this );

    m_cmdNavigateForward->action()->setEnabled(false);
    m_cmdNavigateBack->action()->setEnabled(false);
}

void QWorkMainWindow::createActions()
{
    m_cmdNewFile            = new Core::Command( tr("New"),
                                                 QIcon(":/images/file_new.png"),
                                                 tr("&New"),
                                                 tr("Create a new File"),
                                                 QKeySequence(tr("Ctrl+N")),
                                                 this );

    m_cmdNewFromWizard      = new Core::Command( tr("NewFromWizard"),
                                                 QIcon(":/images/file_new.png"),
                                                 tr("New from Wizard"),
                                                 tr("Create a new Document/Project/Class..."),
                                                 QKeySequence(tr("Ctrl+Alt+N")),
                                                 this );

    m_cmdOpenFile           = new Core::Command( tr("Open"),
                                                 QIcon(":/images/file_open.png"),
                                                 tr("&Open"),
                                                 tr("Open File"),
                                                 QKeySequence(tr("Ctrl+O")),
                                                 this );

    m_cmdSaveFile           = new Core::Command( tr("Save"),
                                                 QIcon(":/images/file_save.png"),
                                                 tr("&Save"),
                                                 tr("Save File"),
                                                 QKeySequence(tr("Ctrl+S")),
                                                 this );

    m_cmdSaveAsFile         = new Core::Command( tr("SaveAs"),
                                                 QIcon(":/images/file_saveas.png"),
                                                 tr("Save &As"),
                                                 tr("Save File As ..."),
                                                 QKeySequence(tr("Ctrl+Alt+S")),
                                                 this );

    m_cmdSaveAllFile        = new Core::Command( tr("SaveAll"),
                                                 QIcon(":/images/file_saveall.png"),
                                                 tr("Save A&ll"),
                                                 tr("Save All Opened Files"),
                                                 QKeySequence(tr("Ctrl+Shift+S")),
                                                 this );

    m_cmdSessions           = new Core::Command( tr("Sessions"),
                                                 tr("Sessions"),
                                                 tr("Session Management"),
                                                 QKeySequence(),
                                                 this );

    m_cmdCloseFile          = new Core::Command( tr("Close"),
                                                 QIcon(":/images/file_close.png"),
                                                 tr("Close"),
                                                 tr("Close File"),
                                                 QKeySequence(tr("Ctrl+W")),
                                                 this );

    m_cmdCloseAllFiles      = new Core::Command( tr("CloseAll"),
                                                 tr("Close All"),
                                                 tr("Close All Files"),
                                                 QKeySequence(tr("Ctrl+Shift+W")),
                                                 this );

    m_cmdExit               = new Core::Command( tr("Exit"),
                                                 QIcon(":/images/file_exit.png"),
                                                 tr("E&xit"),
                                                 tr("Close Application"),
                                                 QKeySequence(tr("Ctrl+Q")),
                                                 this );

    m_cmdToggleStatusBar    = new Core::Command( tr("ToggleStatusBar"),
                                                 QIcon(":/images/view_statusbar.png"),
                                                 tr("Toggle Status Bar"),
                                                 tr("Show/Hide Status Bar"),
                                                 QKeySequence(tr("Ctrl+F12")),
                                                 this );

    m_cmdToggleFullScreenMode = new Core::Command( tr("ToggleFullScreenMode"),
                                                   QIcon(":/images/full_screen.png"),
                                                   tr("Full Screen"),
                                                   tr("Toggle Full Screen Mode"),
                                                   QKeySequence("F12"),
                                                   this );

    m_commandManager->registerCommands( tr("DocumentCommands"),
                                      QList<Core::Command *>() << m_cmdNewFile
                                                               << m_cmdNewFromWizard
                                                               << m_cmdOpenFile
                                                               << m_cmdSaveFile
                                                               << m_cmdSaveAsFile
                                                               << m_cmdSaveAllFile
                                                               << m_cmdCloseFile
                                                               << m_cmdCloseAllFiles );

    m_commandManager->registerCommands( tr("CommonCommands"),
                                      QList<Core::Command *>() << m_cmdExit
                                                               << m_cmdSessions
                                                               << m_cmdToggleStatusBar
                                                               << m_cmdToggleFullScreenMode );

    /// Actions for Prefs Menu
    m_cmdSettings           = new Core::Command( tr("Settings"),
                                                 QIcon(":/images/pref_settings.png"),
                                                 tr("&Settings"),
                                                 tr("Change Application Settings"),
                                                 QKeySequence(tr("Ctrl+P")),
                                                 this );

    m_cmdShortcuts          = new Core::Command( tr("KeyboardShortcuts"),
                                                 tr("Keyboard Shortcuts"),
                                                 tr("Keyboard Shortcuts Modification"),
                                                 QKeySequence(),
                                                 this );

    m_cmdDockCornerSettings = new Core::Command( tr("DockCornerSettings"),
                                                 QIcon(":/images/prefs_dock.png"),
                                                 tr("Dock Corners"),
                                                 tr("Dock Corners Settings"),
                                                 QKeySequence(),
                                                 this );

    m_commandManager->registerCommands( tr("SettingCommands"),
                                        QList<Core::Command *>() << m_cmdSettings
                                                                 << m_cmdShortcuts
                                                                 << m_cmdDockCornerSettings);

    m_cmdToggleStatusBar->action()->setCheckable(true);
    m_cmdToggleStatusBar->action()->setChecked(true);

    m_cmdToggleFullScreenMode->action()->setCheckable(true);
    m_cmdToggleFullScreenMode->action()->setChecked(false);
    connect(m_cmdToggleFullScreenMode->action(), SIGNAL(toggled(bool)), this, SLOT(onToggleFullScreenMode(bool)));

    connect(m_cmdNewFile->action(),            SIGNAL(triggered()), m_documentManager, SLOT(newFile()));
    connect(m_cmdNewFromWizard->action(),      SIGNAL(triggered()), this,              SLOT(newFromWizard()));
    connect(m_cmdOpenFile->action(),           SIGNAL(triggered()), m_documentManager, SLOT(openFile()));
    connect(m_cmdSaveFile->action(),           SIGNAL(triggered()), m_documentManager, SLOT(saveFile()));
    connect(m_cmdSaveAsFile->action(),         SIGNAL(triggered()), m_documentManager, SLOT(saveAsFile()));
    connect(m_cmdSaveAllFile->action(),        SIGNAL(triggered()), m_documentManager, SLOT(saveAllFiles()));
    connect(m_cmdSessions->action(),           SIGNAL(triggered()), this,              SLOT(onSessionControl()));
    connect(m_cmdCloseFile->action(),          SIGNAL(triggered()), m_documentManager, SLOT(closeFile()));
    connect(m_cmdCloseAllFiles->action(),      SIGNAL(triggered()), m_documentManager, SLOT(closeAllFiles()));

    connect(m_cmdExit->action(),               SIGNAL(triggered()), this, SLOT(close()));
    connect(m_cmdSettings->action(),           SIGNAL(triggered()), this, SLOT(onSettings()));
    connect(m_cmdToggleStatusBar->action(),    SIGNAL(triggered(bool)), this, SLOT(onStatusBarShow(bool)));

    connect(m_cmdShortcuts->action(),          SIGNAL(triggered()), m_commandManager, SLOT(onShortcutsModifyRequest()));
    connect(m_cmdDockCornerSettings->action(), SIGNAL(triggered()), this, SLOT(onDockCornerSettings()));
}

void QWorkMainWindow::createMenus()
{
    m_recentFilesMenu = new RecentFilesMenu(m_orgName, m_appName, new QRecentFilesDelegate(this), 20);

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(m_cmdNewFile->action());
    fileMenu->addAction(m_cmdNewFromWizard->action());
    fileMenu->addAction(m_cmdOpenFile->action());
    fileMenu->addSeparator();
    fileMenu->addMenu(m_recentFilesMenu);
    fileMenu->addSeparator();
    fileMenu->addAction(m_cmdSaveFile->action());
    fileMenu->addAction(m_cmdSaveAsFile->action());
    fileMenu->addAction(m_cmdSaveAllFile->action());
    fileMenu->addSeparator();
    fileMenu->addAction(m_cmdSessions->action());
    fileMenu->addSeparator();
    fileMenu->addAction(m_cmdCloseFile->action());
    fileMenu->addAction(m_cmdCloseAllFiles->action());
    fileMenu->addSeparator();
    fileMenu->addAction(m_cmdExit->action());

    m_searchMenu = menuBar()->addMenu(tr("&Search/Navigate"));

    m_editMenu = menuBar()->addMenu(tr("&Edit"));

    viewMenu = menuBar()->addMenu(tr("&View"));

    menuBar()->addMenu(m_scriptsMenu);

    prefsMenu = menuBar()->addMenu(tr("&Preferences"));
    prefsMenu->addAction(m_cmdSettings->action());
    prefsMenu->addAction(m_cmdShortcuts->action());
    prefsMenu->addSeparator();
    prefsMenu->addAction(m_cmdDockCornerSettings->action());
}

void QWorkMainWindow::createToolBars()
{
    m_fileToolBar = new Core::MainToolBar(tr("File"), this);
    addToolBar(m_fileToolBar);
    m_fileToolBar->setObjectName("fileToolBar");

    m_fileToolBar->addAction(m_cmdNewFile->action());
    m_fileToolBar->addAction(m_cmdOpenFile->action());
    m_fileToolBar->addAction(m_cmdSaveFile->action());
    m_fileToolBar->addAction(m_cmdSaveAsFile->action());
    m_fileToolBar->addSeparator();
    m_fileToolBar->addAction(m_cmdCloseFile->action());

    m_viewToolBar = new Core::MainToolBar(tr("View"), this);
    addToolBar(m_viewToolBar);
    m_viewToolBar->setObjectName("viewToolBar");
}

void QWorkMainWindow::populateUiFromPlugins()
{
    QList<UiProviderInterface *>uiProviders;

    foreach ( IPlugin *plugin, m_plugins )
        uiProviders << plugin->createUiProviders(this);

    uiProviders << m_documentManager->createUiProviders(this);

    foreach ( IPlugin *plugin, m_plugins )
        if ( QMenu *menu = plugin->menu() )
            menuBar()->addMenu(menu);

    foreach (UiProviderInterface *uiProvider, uiProviders) {
        if ( Core::ToolBarHandler *toolBarHandler = uiProvider->toolBarHandler() )
            if ( Core::MainToolBar *toolBar = toolBarHandler->m_toolBar )
                addToolBar(toolBar);

        Core::DockWidgetHandler *dockHandler = uiProvider->dockWidgetHandler();
        QwCustomDockWidget *dock = dockHandler ? dockHandler->m_dock : 0;
        if ( dock ) {
            addDockWidgetHandler(dockHandler);
            addToggleAction(dock->toggleViewAction());
        }
    }
    qDeleteAll(uiProviders);
}

void QWorkMainWindow::addToggleAction( QAction *action )
{
    viewMenu->addAction(action);
    m_viewToolBar->addAction(action);
}

void QWorkMainWindow::onSettings()
{
    m_settingsManager->exec(this);
}

void QWorkMainWindow::newFromWizard()
{
    Core::WizardManager::showNewItemDialog(tr("Create new..."));
}

void QWorkMainWindow::showStatusMessage( const QString &message )
{
    if ( m_isStatusBarHidden )
        statusBar()->show();

    statusBar()->showMessage(message);
}

void QWorkMainWindow::hideStatusMessage( const QString &message )
{
    statusBar()->showMessage(message);

    if ( m_isStatusBarHidden )
        statusBar()->hide();
}

void QWorkMainWindow::onStatusBarShow( bool show )
{
    show ? statusBar()->show() : statusBar()->hide();
    m_isStatusBarHidden = !show;
}

void QWorkMainWindow::onToggleFullScreenMode( bool onoff )
{
    Core::ModeManager::setScreenMode(onoff ? Core::FullScreen : Core::Normal);
}

void QWorkMainWindow::switchMode(IDocument *document)
{
    Core::ModeManager::switchToMode(document ? document->documentPlugin()->mode() : 0);
}

void QWorkMainWindow::onNavigateBack()
{
    m_navigationManager->goBackInNavigationHistory();
}

void QWorkMainWindow::addToRecentFiles( const QString &filePath )
{
    m_recentFilesMenu->appendItem(filePath);
}

void QWorkMainWindow::updateRecentFile( const QString &oldFileName, const QString &newFileName )
{
    m_recentFilesMenu->updateItem(oldFileName, newFileName);
}

void QWorkMainWindow::onSessionControl()
{
    SessionsDialog sessionDialog(this);
    sessionDialog.exec();
}

void QWorkMainWindow::onDockCornerSettings()
{
    DockCornerSettingsDialog dockSettingsDialog(this);
    dockSettingsDialog.exec();
}

void QWorkMainWindow::onNavigateForward()
{
    m_navigationManager->goForwardInNavigationHistory();
}

void QWorkMainWindow::closeEvent( QCloseEvent *event )
{
    if ( !m_closeReflector->applicationAboutToClose() ) {
        event->ignore();
        return;
    }

    disconnect(m_documentManager, SIGNAL(currentDocumentChanged(IDocument*)), this, SLOT(switchMode(IDocument*)));
    Core::ModeManager::setScreenMode(Core::Normal);

    m_sessionManager->saveCurrentSession();
    m_closeReflector->applicationClose();

    m_snippetsDock->synchronize();

    writeSettings();

    m_closeReflector->destroyClients();

    delete m_recentFilesMenu;

    event->accept();
}

void QWorkMainWindow::changeEvent( QEvent *event )
{
    QMainWindow::changeEvent(event);
    if ( event->type() == QEvent::ActivationChange ) {
        if ( isActiveWindow() ) {
            emit windowActivated();
        }
    }
}

static bool isDesktopFileManagerDrop(const QMimeData *d, QStringList *files = 0)
{
    if ( files )
        files->clear();
    // Extract dropped files from Mime data.
    if ( !d->hasUrls() )
        return false;
    const QList<QUrl> urls = d->urls();
    if ( urls.empty() )
        return false;
    // Try to find local files
    bool hasFiles = false;
    const QList<QUrl>::const_iterator cend = urls.constEnd();
    for (QList<QUrl>::const_iterator it = urls.constBegin(); it != cend; ++it) {
        const QString fileName = it->toLocalFile();
        if ( !fileName.isEmpty() ) {
            hasFiles = true;
            if ( files ) {
                files->push_back(fileName);
            } else {
                break; // No result list, sufficient for checking
            }
        }
    }
    return hasFiles;
}

void QWorkMainWindow::dragEnterEvent( QDragEnterEvent *event )
{
    if ( isDesktopFileManagerDrop(event->mimeData()) && m_filesToOpenDelayed.isEmpty() ) {
        event->accept();
    } else {
        event->ignore();
    }
}

void QWorkMainWindow::dropEvent( QDropEvent *event )
{
    QStringList files;
    if ( isDesktopFileManagerDrop(event->mimeData(), &files) ) {
        event->accept();
        m_filesToOpenDelayed.append(files);
        QTimer::singleShot(50, this, SLOT(openDelayedFiles()));
    } else {
        event->ignore();
    }
}

void QWorkMainWindow::openDelayedFiles()
{
    if ( m_filesToOpenDelayed.isEmpty() )
        return;
    activateWindow();
    raise();
    DocumentManager *documentManager = DocumentManager::getInstance();
    QString errorMessage;
    foreach( const QString &fileName, m_filesToOpenDelayed ) {
        if ( !documentManager->openDocument(fileName, 0, &errorMessage) ) {
            Core::LogManager::addMessage(Core::LogMessage(QString("File \"%1\" cannot be loaded: %2").arg(fileName).arg(errorMessage)));
        }

    }
    m_filesToOpenDelayed.clear();
}

void QWorkMainWindow::writeSettings()
{
    if ( m_cmdToggleFullScreenMode->action()->isChecked() )
        onToggleFullScreenMode(false);

    QSettings settings(m_orgName, m_appName);

    settings.beginGroup("MainWindow");
    settings.setValue("geometry", saveGeometry());
    Core::ModeManager::writeSettings(settings);
    settings.setValue("statusBarHidden", m_isStatusBarHidden);
    settings.endGroup();

    m_settingsManager->writeSetings(settings);

    foreach ( Core::DockWidgetHandler *dockHandler, m_dockHandlers )
        dockHandler->m_dock->writeSettings();
}

void QWorkMainWindow::readSettings()
{
    QSettings settings(m_orgName, m_appName);

    settings.beginGroup("MainWindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    Core::ModeManager::readSettings(settings);
    m_isStatusBarHidden = settings.value("statusBarHidden").toBool();
    settings.endGroup();

    if ( m_isStatusBarHidden ) {
        statusBar()->hide();
        m_cmdToggleStatusBar->action()->setChecked(false);
    }

    m_settingsManager->readSettings(settings);
    m_highlighterFormatsManager->restore();
}

QMessageBox::StandardButton QWorkMainWindow::information( const QString &text, QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton )
{
    return QMessageBox::information(QWorkMainWindow::getInstance(), QWorkMainWindow::getInstance()->getAppName(), text, buttons, defaultButton);
}

QMessageBox::StandardButton QWorkMainWindow::warning( const QString &text, QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton )
{
    return QMessageBox::warning(QWorkMainWindow::getInstance(), QWorkMainWindow::getInstance()->getAppName(), text, buttons, defaultButton);
}

QMainWindow *QWorkMainWindow::widget()
{
    return this;
}

QString QWorkMainWindow::orgName() const
{
    return m_orgName;
}

QString QWorkMainWindow::appName() const
{
    return m_appName;
}

QMessageBox::StandardButton QWorkMainWindow::showInformation( const QString &text,
                                                              QMessageBox::StandardButtons buttons,
                                                              QMessageBox::StandardButton defaultButton )
{
    return QMessageBox::information(this, m_appName, text, buttons, defaultButton);
}

QMessageBox::StandardButton QWorkMainWindow::showWarning( const QString &text,
                                                          QMessageBox::StandardButtons buttons,
                                                          QMessageBox::StandardButton defaultButton )
{
    return QMessageBox::warning(this, m_appName, text, buttons, defaultButton);
}

bool QWorkMainWindow::question(const QString &msg)
{
    return (QMessageBox::Yes == QMessageBox::question(this, m_appName, msg));
}

Core::FutureProgress *QWorkMainWindow::addTask( const QFuture<void> &future, const QString &title, const QString &type,
                                                bool signToAppStatusBar, bool isSyncTask )
{
    return m_progressManager->addTask( future, title, type, 0, signToAppStatusBar, isSyncTask );
}

void QWorkMainWindow::addCurrentPositionToNavigationHistory( Core::IEditor *editor, const QByteArray &saveState )
{
    m_navigationManager->addCurrentPositionToNavigationHistory(editor, saveState);
}

bool QWorkMainWindow::registerModule( Python::IPythonModule *module, bool deleteOnFail, const QStringList &classNames )
{
    return m_pythonScriptingManager->registerModule(module, deleteOnFail, classNames);
}

bool QWorkMainWindow::registerBuiltinScript( const QString &scriptId, const QString &scriptSource )
{
    return m_pythonScriptingManager->registerBuiltinScript(scriptId, scriptSource);
}

void QWorkMainWindow::pythonReport( const QString &message, IMainWindow::ReportType reportType )
{
    m_pythonScriptingManager->pythonReport(message, reportType);
}

void QWorkMainWindow::executeBuiltinScript( const QString &scriptId )
{
    m_pythonScriptingManager->executeBuiltinScript(scriptId);
}

QStringList QWorkMainWindow::pythonCodeAssist( const QString &source, int position )
{
    return m_pythonScriptingManager->pythonCodeAssist(source, position);
}

void QWorkMainWindow::handleScriptChanged( const QString &scriptName, const QString &source )
{
    m_pythonScriptingManager->handleScriptChanged(scriptName, source);
}

QMap<QString, Python::IPythonModule *> *QWorkMainWindow::pythonModules()
{
    return m_pythonScriptingManager->pythonModules();
}

void QWorkMainWindow::setNavigationHistoryBlocked( bool onoff )
{
    m_navigationManager->setNavigationHistoryBlocked(onoff);
}

void QWorkMainWindow::cutForwardNavigationHistory()
{
    m_navigationManager->cutForwardNavigationHistory();
}

void QWorkMainWindow::handleNavigationStateChanged( bool canBack, bool canForward )
{
    m_cmdNavigateBack->action()->setEnabled(canBack);
    m_cmdNavigateForward->action()->setEnabled(canForward);
}

void QWorkMainWindow::initializeSnippets()
{
    m_cmdAddSnippet     = new Core::Command(tr("SnippetAdd"),
                                            QIcon(":/images/alias_add.png"),
                                            tr("Add snippet"),
                                            tr("Add snippet"),
                                            QKeySequence(),
                                            this);

    m_cmdDeleteSnippet  = new Core::Command(tr("SnippetDelete"),
                                            QIcon(":/images/alias_delete.png"),
                                            tr("Delete snippet"),
                                            tr("Delete snippet"),
                                            QKeySequence(),
                                            this);

    m_snippetsDock = new SnippetsDockWidget(this, new QComboBox(), m_cmdAddSnippet->action(), m_cmdDeleteSnippet->action());

    addDockWidgetHandler(new Core::DockWidgetHandler(m_snippetsDock,
                                                     new Core::EditModeDockGuiClient(m_snippetsDock)));

}
