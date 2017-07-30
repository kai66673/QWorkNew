#include <QPainter>
#include <QMenu>
#include <QTabBar>
#include <QLabel>
#include <QToolBar>
#include <QToolButton>
#include <QHBoxLayout>
#include <QDomDocument>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QApplication>

#include <QDebug>

#include "QwDockWidget.h"
#include "qtcfileutils.h"
#include "DesignerDockTitle.h"

static int s_QwTabBarWatcherIndex = -1;
static int s_QwTabTitleWatcherIndex = -1;
static int s_QwTabCornerToolBarWatcherIndex = -1;

static int registerQwTabBarWatcher()
{
    if ( s_QwTabBarWatcherIndex == -1 )
        s_QwTabBarWatcherIndex = Utils::StyleManager::registerQssFileWatcher(QLatin1String("QwTabBar"));
    return s_QwTabBarWatcherIndex;
}

static int registerQwTitleBarWatcher()
{
    if ( s_QwTabTitleWatcherIndex == -1 )
        s_QwTabTitleWatcherIndex = Utils::StyleManager::registerQssFileWatcher(QLatin1String("QwTitleBar"));
    return s_QwTabTitleWatcherIndex;
}

static int registerQwTabCornerToolBarWatcher()
{
    if ( s_QwTabCornerToolBarWatcherIndex == -1 )
        s_QwTabCornerToolBarWatcherIndex = Utils::StyleManager::registerQssFileWatcher(QLatin1String("QwTabCornerToolBar"));
    return s_QwTabCornerToolBarWatcherIndex;
}

QwTabBar::QwTabBar(QWidget *parent)
    : QTabBar(parent)
    , Utils::StyleManagedWidget(this, registerQwTabBarWatcher())
{ }

QwTabBarHolder::QwTabBarHolder(QMainWindow *mainWindow, QwTabCloseResolver *tabCloseResolver)
    : QWidget(0)
    , m_mainWindow(mainWindow)
    , m_tabCloseResolver(tabCloseResolver)
{
    QHBoxLayout *horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setSpacing(0);
    horizontalLayout->setContentsMargins(0, 0, 0, 0);

    QAction *actClear = new QAction(QIcon(":/images/sr_clear.png"), tr("Close All"), m_mainWindow);
    actClear->setEnabled(false);
    m_clearButton = new QToolButton(this);
    m_clearButton->setMaximumSize(QSize(20, 20));
    m_clearButton->setDefaultAction(actClear);
    m_clearButton->setAutoRaise(true);

    m_tabBar = new QwTabBar(this);
    m_tabBar->setTabsClosable(true);
    m_tabBar->setMaximumSize(QSize(16777215, 20));
    m_tabBar->setDrawBase(false);
    m_tabBar->setDocumentMode(true);

    connect(m_tabBar,       SIGNAL(tabCloseRequested(int)), this, SLOT(removeTab(int)));
    connect(m_clearButton,  SIGNAL(clicked()),              this, SLOT(removeAllTabs()));
    connect(m_tabBar,       SIGNAL(currentChanged(int)),    this, SIGNAL(currentChanged(int)));

    horizontalLayout->addWidget(m_clearButton);
    horizontalLayout->addWidget(m_tabBar, 1);
}

QwTabBarHolder::~QwTabBarHolder()
{ }

void QwTabBarHolder::addTab( const QIcon &icon, const QString &title, const QString &toolTip)
{
    m_tabBar->addTab(icon, title);
    int index = m_tabBar->count() - 1;
    m_tabBar->setTabToolTip(index, toolTip);
    m_tabBar->setCurrentIndex(index);
    m_clearButton->setEnabled(true);
}

void QwTabBarHolder::setCurrentIndex(int index)
{
    m_tabBar->setCurrentIndex(index);
}

void QwTabBarHolder::removeTab( int index )
{
    if ( m_tabCloseResolver && !m_tabCloseResolver->canCloseTab(index) )
        return;

    m_tabBar->removeTab(index);
    if ( !m_tabBar->count() )
        m_clearButton->setEnabled(false);
    emit tabRemoved(index);
}

void QwTabBarHolder::removeCurrentTab()
{
    int index = m_tabBar->currentIndex();
    if ( index != -1 )
        removeTab(index);
}

void QwTabBarHolder::removeAllTabs()
{
    int tabCount = m_tabBar->count();
    if ( !tabCount )
        return;

    for (int i = tabCount - 1; i >= 0; i--) {
        removeTab(i);
        if ( m_tabBar->count() != i )
            return;
    }

    m_clearButton->setEnabled(false);
    emit allTabsRemoved();
}

void QwTabBarHolder::nextTab()
{
    int count = m_tabBar->count();
    if ( count > 1 ) {
        int current = m_tabBar->currentIndex();
        if ( current >= count - 1 )
            current = 0;
        else
            current++;
        m_tabBar->setCurrentIndex(current);
    }
}

void QwTabBarHolder::prevTab()
{
    int count = m_tabBar->count();
    if ( count > 1 ) {
        int current = m_tabBar->currentIndex();
        if ( current == 0 )
            current = count - 1;
        else
            current--;
        m_tabBar->setCurrentIndex(current);
    }
}

QwTitleBar::QwTitleBar( QMainWindow *mainWindow , QwCustomDockWidget *dock, const QString &title,
                        const QList<QAction *> &actions,
                        QWidget *widget, bool innerMode )
    : QWidget(mainWindow)
    , Utils::StyleManagedWidget(this, registerQwTitleBarWatcher())
    , m_dock(dock)
    , m_dockTitle(title)
    , m_toggleMenu(0)
    , m_mainWindow(mainWindow)
    , m_appName(Utils::FileUtils::applicationName())
    , m_innerMode(innerMode)
    , m_permanentToolButtonsCount(0)
{
    if ( !m_innerMode )
        createToggleActions(actions);

    resize(160, 20);
    setMinimumSize(QSize(94, 20));
    setMaximumSize(QSize(16777215, 20));

    horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setSpacing(0);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalLayout->setContentsMargins(0, 0, 0, 0);

    titleLabel = new QLabel(this);
    titleLabel->setObjectName(QString::fromUtf8("titleLabel"));
    QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(titleLabel->sizePolicy().hasHeightForWidth());
    titleLabel->setSizePolicy(sizePolicy);
    titleLabel->setMinimumSize(QSize(40, 0));
    QFont font;
    font.setPointSize(8);
    font.setBold(true);
    font.setWeight(75);
    titleLabel->setFont(font);
    titleLabel->setFocusPolicy(Qt::NoFocus);
    titleLabel->setCursor(Qt::SizeAllCursor);

    horizontalLayout->addWidget(titleLabel);

    int i = 0;
    if ( !m_innerMode ) {
        foreach ( QAction *act, actions ) {
            QToolButton *tb = new QToolButton(this);
            tb->setDefaultAction(act);
            tb->setMaximumSize(QSize(act->menu() ? 32 : 20, 20));
            tb->setAutoRaise(true);
            horizontalLayout->addWidget(tb);
            if ( !m_toggleActions[i]->isChecked() )
                tb->hide();
            connect(m_toggleActions[i], SIGNAL(triggered(bool)), tb, SLOT(setVisible(bool)));
            i++;
        }
    } else {
        foreach ( QAction *act, actions ) {
            QToolButton *tb = new QToolButton(this);
            tb->setDefaultAction(act);
            tb->setMaximumSize(QSize(20, 20));
            tb->setAutoRaise(true);
            horizontalLayout->addWidget(tb);
            i++;
        }
    }

    if ( !widget )
        horizontalLayout->addWidget(new QStackedWidget(this), 1);
    else {
        widget->setMaximumHeight(20);
        horizontalLayout->addWidget(widget, 1);
    }


    actionsButton = new QToolButton(this);
    actionsButton->setObjectName(QString::fromUtf8("acionsButton"));
    actionsButton->setMaximumSize(QSize(20, 20));
    actionsButton->setFocusPolicy(Qt::NoFocus);
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/utils/images/dock_menu.png"), QSize(), QIcon::Normal, QIcon::Off);
    actionsButton->setIcon(icon);
    actionsButton->setPopupMode(QToolButton::InstantPopup);
    actionsButton->setAutoRaise(true);
    actionsButton->setArrowType(Qt::NoArrow);

    horizontalLayout->addWidget(actionsButton);

    restoreButton = new QToolButton(this);
    restoreButton->setObjectName(QString::fromUtf8("restoreButton"));
    restoreButton->setMaximumSize(QSize(20, 20));
    restoreButton->setFocusPolicy(Qt::NoFocus);
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/utils/images/dock_restore.png"), QSize(), QIcon::Normal, QIcon::Off);
    restoreButton->setIcon(icon1);
    restoreButton->setAutoRaise(true);
    restoreButton->setArrowType(Qt::NoArrow);

    horizontalLayout->addWidget(restoreButton);

    closeButton = new QToolButton(this);
    closeButton->setObjectName(QString::fromUtf8("closeButton"));
    closeButton->setMaximumSize(QSize(20, 20));
    closeButton->setFocusPolicy(Qt::NoFocus);
    QIcon icon2;
    icon2.addFile(QString::fromUtf8(":/utils/images/dock_close.png"), QSize(), QIcon::Normal, QIcon::Off);
    closeButton->setIcon(icon2);
    closeButton->setAutoRaise(true);
    closeButton->setArrowType(Qt::NoArrow);

    horizontalLayout->addWidget(closeButton);

    setWindowTitle(tr("Form"));
    titleLabel->setText(title);
    actionsButton->setToolTip(tr("Context Menu\n(with Ctrl modifier - configure Dock title bar)"));
    actionsButton->setText(tr(".."));
    restoreButton->setToolTip(tr("Toggle Floating"));
    restoreButton->setText(tr(".."));
    closeButton->setToolTip(tr("Close"));
    closeButton->setText(tr("..."));

    connect(actionsButton,   SIGNAL(clicked()), this, SLOT(showContextMenu()));
    connect(restoreButton,  SIGNAL(clicked()), m_dock, SLOT(toggleFloating()));
    connect(closeButton,    SIGNAL(clicked()), m_dock, SLOT(close()));

    actionsButton->hide();
}

QwTitleBar::~QwTitleBar()
{
}

void QwTitleBar::createToggleActions( const QList<QAction *> &actions )
{
    qDeleteAll(m_toggleActions);
    m_toggleActions.clear();

    QMap<QString, bool> actSettings;
    readSettings(actSettings);

    m_toggleMenu = new QMenu(this);

    foreach ( QAction *act, actions ) {
        QAction *toggleAction = new QAction(act->icon(), act->text(), this);
        toggleAction->setCheckable(true);
        bool bChecked = false;
        if ( actSettings.find(act->text()) != actSettings.end() )
            bChecked = actSettings[act->text()];
        toggleAction->setChecked(bChecked);
        m_toggleActions.append(toggleAction);
        m_toggleMenu->addAction(toggleAction);
    }
}

void QwTitleBar::readSettings( QMap<QString, bool> &actSettings )
{
    QString fileName = Utils::FileUtils::applicationDataLocation();
    fileName += "/TitleBar." + m_dockTitle.replace(" ", "") + ".Actions.xml";

    QFile file(fileName);

    if ( !file.exists() )
        return;

    if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
        QMessageBox::warning(m_mainWindow, m_appName, tr("Error On Loading TitleBar Settings\nFile %1 Cannot be Opened for Read").arg(fileName) );
        return;
    }

    QDomDocument domDocument;
    QString errorStr;
    int errorLine;
    int errorColumn;

    if (!domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn)) {
        QMessageBox::warning( m_mainWindow, m_appName,
                              tr("Error On Loading TitleBar Settings\nCommon Error in xml-file %1:\nLine %2, Column %2: %3")
                              .arg(fileName).arg(errorLine).arg(errorColumn).arg(errorStr) );
        return;
    }

    QDomElement root = domDocument.documentElement();
    if ( root.tagName() != "TitleBarActions" ) {
        QMessageBox::warning( m_mainWindow, m_appName, tr("Error On Loading TitleBar Settings\nInvalid Format of xml-file %1").arg(fileName) );
        return;
    }
    if ( !root.hasAttribute("version") ) {
        QMessageBox::warning( m_mainWindow, m_appName, tr("Error On Loading TitleBar Settings\nInvalid Format of xml-file %1(version expected)") .arg(fileName) );
        return;
    }
    if ( root.attribute("version") != "1.0" ) {
        QMessageBox::warning( m_mainWindow, m_appName, tr("Error On Loading TitleBar Settings\nWrong Version of xml-file %1").arg(fileName) );
        return;
    }

    QDomElement itemElement = root.firstChildElement();
    while ( !itemElement.isNull() ) {
        if ( itemElement.tagName() != "item" ) {
            QMessageBox::warning( m_mainWindow, m_appName,
                                  tr("Error On Loading TitleBar Settings\nUnknown Tag <%1> in xml-file %2 at line %3)")
                                  .arg(itemElement.tagName()).arg(fileName).arg(itemElement.lineNumber()) );
            return;
        }
        bool bShown = (itemElement.attribute("shown", "0").toInt() ? true : false);
        actSettings[itemElement.attribute("name", "")] = bShown;

        itemElement = itemElement.nextSiblingElement();
    }
}

void QwTitleBar::writeSettings()
{
    if ( m_innerMode )
        return;

    QString fileName = Utils::FileUtils::applicationDataLocation();
    fileName += "/TitleBar." + m_dockTitle.replace(" ", "") + ".Actions.xml";

    QFile file(fileName);

    if ( !file.exists() ) {
        QFileInfo fi(fileName);
        if ( !fi.absoluteDir().exists() ) {
            QDir dir;
            dir.mkpath(fi.absolutePath());
        }
    }

    if ( !file.open(QFile::WriteOnly | QFile::Text) ) {
        QMessageBox::warning( m_mainWindow, m_appName,
                              tr("Error On Saving TitleBar Settings\nFile \"%1\" Cannot be Opened for Write:\n%2.")
                              .arg(fileName).arg(file.errorString()) );
        return;
    }

    QDomDocument domDocument("TitleBarActions");

    domDocument.appendChild(domDocument.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"windows-1251\""));

    QDomElement root = domDocument.createElement("TitleBarActions");
    root.setAttribute("version", "1.0");
    domDocument.appendChild(root);

    foreach ( QAction *act, m_toggleActions ) {
        QDomElement itemElement = domDocument.createElement("item");
        itemElement.setAttribute("name", act->text());
        itemElement.setAttribute("shown", act->isChecked());
        root.appendChild(itemElement);
    }

    const int IndentSize = 4;
    QTextStream out(&file);
    domDocument.save(out, IndentSize);
    file.close();
}

QToolButton *QwTitleBar::createPermanentToolButton(QAction *action)
{
    QToolButton *tb = new QToolButton(this);

    tb->setDefaultAction(action);
    tb->setMaximumSize(QSize(32, 20));
    tb->setAutoRaise(true);
    horizontalLayout->insertWidget(m_permanentToolButtonsCount + 1, tb);
    m_permanentToolButtonsCount++;

    return tb;
}

void QwTitleBar::setContextMenuButtonVisisble(bool visible)
{
    actionsButton->setVisible(visible);
}

void QwTitleBar::featuresChanged( QDockWidget::DockWidgetFeatures features )
{
    restoreButton->setVisible(features & QDockWidget::DockWidgetFloatable);
    closeButton->setVisible(features & QDockWidget::DockWidgetClosable);
}

void QwTitleBar::showContextMenu()
{
    if ( QWidget *w = qobject_cast<QWidget *>(sender()) ) {
        if ( QApplication::keyboardModifiers() & Qt::ControlModifier ) {
            if ( m_toggleMenu && m_toggleMenu->actions().size() > 0 )
                m_toggleMenu->exec(mapToGlobal(w->pos() + w->rect().bottomLeft()));
        } else {
            m_dock->showContextMenu(mapToGlobal(w->pos() + w->rect().bottomLeft()));
        }
    }
}

QwCustomDockWidget::QwCustomDockWidget( QMainWindow *mainWindow , const QString &title, const QString &objName,
                                        const QKeySequence &toggleShortcut, const QIcon &toggleIcon,
                                        Qt::DockWidgetArea initialDockArea, TabifyHint tabifyHint,
                                        const QList<QAction *> &actions,
                                        QWidget *widget , bool innerMode )
    : QDockWidget(title, mainWindow)
    , m_contextMenu(0)
    , m_titleBar(new QwTitleBar(mainWindow, this, title, actions, widget, innerMode))
    , m_customWidget(widget)
    , m_mainWindow(mainWindow)
    , m_initialDockArea(initialDockArea)
    , m_tabifyHint(tabifyHint)
{
    setTitleBarWidget(m_titleBar);
    setObjectName(objName);

    QAction *toggleAction = toggleViewAction();
    toggleAction->setIcon(toggleIcon);
    QString toolTipText = toggleAction->text();
    toolTipText.prepend(tr("Toggle "));
    toolTipText.append(tr(" Dock"));
    toggleAction->setStatusTip(toolTipText);

    if ( toggleShortcut.isEmpty() ) {
        toggleAction->setToolTip(QString("<b>%1</b>").arg(toolTipText));
    }
    else {
        toggleAction->setToolTip( QString("<b>%1</b> <span style=\"color: gray; font-size: small\">%2</span>")
                                  .arg(toolTipText)
                                  .arg(toggleShortcut.toString(QKeySequence::NativeText)) );
        toggleAction->setShortcut(toggleShortcut);
    }

    connect(this, SIGNAL(featuresChanged(QDockWidget::DockWidgetFeatures)), m_titleBar, SLOT(featuresChanged(QDockWidget::DockWidgetFeatures)));
    connect(toggleViewAction(), SIGNAL(toggled(bool)), this, SLOT(toggled(bool)));
}

QwCustomDockWidget::~QwCustomDockWidget()
{
    if ( m_contextMenu )
        if ( !m_contextMenu->parent() )
            delete m_contextMenu;
}

void QwCustomDockWidget::setContextMenu( QMenu *menu )
{
    if ( m_contextMenu )
        return;
    m_contextMenu = menu;
    m_titleBar->setContextMenuButtonVisisble(menu);
}

void QwCustomDockWidget::writeSettings()
{
    m_titleBar->writeSettings();
}

Qt::DockWidgetArea QwCustomDockWidget::initialDockArea() const
{
    return m_initialDockArea;
}

QwCustomDockWidget::TabifyHint QwCustomDockWidget::initialTabifyHint() const
{
    return m_tabifyHint;
}

QToolButton *QwCustomDockWidget::createPermanentToolButton(QAction *action)
{
    return m_titleBar->createPermanentToolButton(action);
}

void QwCustomDockWidget::setWidget(QWidget *widget)
{
    QDockWidget::setWidget(widget);
    if ( QFrame *frame = dynamic_cast<QFrame *>(widget) ) {
        frame->setFrameShape(QFrame::NoFrame);
        frame->setLineWidth(0);
    }
}

void QwCustomDockWidget::paintEvent( QPaintEvent *event )
{
    QDockWidget::paintEvent(event);

    if ( isFloating() ) {
        // Draw border in Floating mode
        QPainter painter(this);
        QRect rect = this->rect();
        rect.adjust(0, 0, -1, -1);
        painter.drawRect(rect);
    }
}

void QwCustomDockWidget::toggleFloating()
{
    if ( features() & QDockWidget::DockWidgetFloatable )
        setFloating(!isFloating());
}

void QwCustomDockWidget::showContextMenu(const QPoint &pos)
{
    if ( m_contextMenu )
        m_contextMenu->exec(pos);
}

void QwCustomDockWidget::toggled(bool onoff)
{
    if ( onoff )
        showAndSetCurrentIfTabified();
}

void QwCustomDockWidget::showAndSetCurrentIfTabified( bool *indexChanged )
{
    show();

    if ( indexChanged )
        *indexChanged = false;

    QList<QDockWidget *> tabifiedDocks = m_mainWindow->tabifiedDockWidgets(this);
    if ( tabifiedDocks.size() > 0 ) {
        QList<QTabBar *> tabBars = m_mainWindow->findChildren<QTabBar *>();
        bool bFound = false;
        foreach ( QTabBar *tabBar, tabBars ) {
            if ( tabBar->isVisible() ) {
                int currentIndex = tabBar->currentIndex();
                for (int i = 0; i < tabBar->count(); i++) {
                    quintptr wId = qvariant_cast<quintptr>(tabBar->tabData(i));
                    QwCustomDockWidget *widget = reinterpret_cast<QwCustomDockWidget *>(wId);
                    if ( widget == this ) {
                        if ( i != currentIndex && indexChanged )
                            *indexChanged = true;
                        tabBar->setCurrentIndex(i);
                        bFound = true;
                        break;
                    }
                }
                if ( bFound )
                    break;
            }
        }
    }
}

void QwCustomDockWidget::popup()
{
    showAndSetCurrentIfTabified();
}

QwTabBarDockWidget::QwTabBarDockWidget( QMainWindow *mainWindow, const QString &title, const QString &objName,
                                        const QKeySequence &toggleShortcut, const QIcon &toggleIcon,
                                        Qt::DockWidgetArea initialDockArea, TabifyHint tabifyHint,
                                        const QList<QAction *> &actions,
                                        QwTabCloseResolver *tabCloseRsolver )
    : QwCustomDockWidget( mainWindow, title, objName, toggleShortcut, toggleIcon,
                          initialDockArea, tabifyHint, actions,
                          new QwTabBarHolder(mainWindow, tabCloseRsolver) )
{
    m_tabBar = qobject_cast<QwTabBarHolder *>(customWidget());
    m_stackedWidget = new QStackedWidget(this);
    setWidget(m_stackedWidget);

    connect(m_tabBar,   SIGNAL(currentChanged(int)),    m_stackedWidget,    SLOT(setCurrentIndex(int)));
    connect(m_tabBar,   SIGNAL(tabRemoved(int)),        this,               SLOT(handleTabRemoved(int)));
    connect(m_tabBar,   SIGNAL(allTabsRemoved()),       this,               SLOT(handleAllTabsRemoved()));

    connect(m_tabBar,   SIGNAL(currentChanged(int)),    this,               SIGNAL(currentChanged(int)));
    connect(m_tabBar,   SIGNAL(tabRemoved(int)),        this,               SIGNAL(tabRemoved(int)));
}

void QwTabBarDockWidget::addTab( const QIcon &icon, const QString &title, const QString &toolTip, QWidget *widget )
{
    m_stackedWidget->addWidget(widget);
    if ( QFrame *frame = dynamic_cast<QFrame *>(widget) ) {
        frame->setFrameShape(QFrame::NoFrame);
        frame->setLineWidth(0);
    }
    m_tabBar->addTab(icon, title, toolTip);
}

int QwTabBarDockWidget::currentIndex() const
{
    return m_stackedWidget->currentIndex();
}

QWidget *QwTabBarDockWidget::currentWidget() const
{
    return m_stackedWidget->currentWidget();
}

QWidget *QwTabBarDockWidget::tabWidget(int index) const
{
    return m_stackedWidget->widget(index);
}

int QwTabBarDockWidget::indexOf(QWidget *widget) const
{
    return m_stackedWidget->indexOf(widget);
}

int QwTabBarDockWidget::count() const
{
    return m_stackedWidget->count();
}

void QwTabBarDockWidget::setCurrentIndex(int index)
{
    m_tabBar->setCurrentIndex(index);
}

void QwTabBarDockWidget::removeCurrentTab()
{
    m_tabBar->removeCurrentTab();
}

void QwTabBarDockWidget::removeAllTabs()
{
    m_tabBar->removeAllTabs();
}

void QwTabBarDockWidget::nextTab()
{
    m_tabBar->nextTab();
}

void QwTabBarDockWidget::prevTab()
{
    m_tabBar->prevTab();
}

void QwTabBarDockWidget::handleTabRemoved( int index )
{
    if ( index >= 0 && index < m_stackedWidget->count() ) {
        QWidget *delWidget = m_stackedWidget->widget(index);
        m_stackedWidget->removeWidget(delWidget);
        delWidget->deleteLater();
    }
}

void QwTabBarDockWidget::handleAllTabsRemoved()
{
    while ( m_stackedWidget->count() ) {
        QWidget *delWidget = m_stackedWidget->widget(0);
        m_stackedWidget->removeWidget(delWidget);
        delWidget->deleteLater();
    }
}

void QwTabBarDockWidget::setWidget( QWidget *widget )
{
    QDockWidget::setWidget(widget);
    if ( QFrame *frame = dynamic_cast<QFrame *>(widget) ) {
        frame->setFrameShape(QFrame::NoFrame);
        frame->setLineWidth(0);
    }
}

QwTabCornerToolBar::QwTabCornerToolBar(QWidget *parent)
    : QToolBar(parent)
    , Utils::StyleManagedWidget(this, registerQwTabCornerToolBarWatcher())
{ }
