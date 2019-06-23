#ifndef QWDOCKWIDGET_H
#define QWDOCKWIDGET_H

#include <QDockWidget>
#include <QList>
#include <QAction>
#include <QStackedWidget>
#include <QList>
#include <QAction>
#include <QTabBar>
#include <QMainWindow>
#include <QToolBar>

#include "StyleManager.h"
#include "utils_global.h"

QT_BEGIN_NAMESPACE
class QHBoxLayout;
class QLabel;
class QToolButton;
QT_END_NAMESPACE

class QwCustomDockWidget;
class QwTabBarDockWidget;

class QwTabCloseResolver
{
public:
    virtual ~QwTabCloseResolver() = default;
    virtual bool canCloseTab(int /*tabIndex*/) { return true; }
};

class QTCREATOR_UTILS_EXPORT QwTabBar: public QTabBar, public Utils::StyleManagedWidget
{
    Q_OBJECT

public:
    QwTabBar(QWidget *parent = nullptr);
};

class QwTabBarHolder: public QWidget
{
    Q_OBJECT
    friend class QwTabBarDockWidget;

public:
    QwTabBarHolder(QMainWindow *mainWindow, QwTabCloseResolver *tabCloseResolver = nullptr);
    virtual ~QwTabBarHolder();

    void addTab( const QIcon &icon, const QString &title, const QString &toolTip );

public slots:
    void setCurrentIndex(int index);

private slots:
    void removeTab( int index );
    void removeCurrentTab();
    void removeAllTabs();
    void nextTab();
    void prevTab();

signals:
    void tabRemoved( int index );
    void allTabsRemoved();
    void currentChanged( int index );

private:
    QMainWindow *m_mainWindow;
    QwTabBar *m_tabBar;
    QToolButton *m_clearButton;

    QwTabCloseResolver *m_tabCloseResolver;
};

class QwTitleBar: public QWidget, public Utils::StyleManagedWidget
{
    Q_OBJECT

public:
    explicit QwTitleBar(QMainWindow *mainWindow, QwCustomDockWidget *dock, const QString &title,
                        const QList<QAction *> &actions = QList<QAction *>(),
                        QWidget *widget = nullptr, bool innerMode = false);
    virtual ~QwTitleBar();

    void writeSettings();

    QToolButton *createPermanentToolButton(QAction *action);
    void setContextMenuButtonVisisble(bool visible);

public slots:
    void featuresChanged( QDockWidget::DockWidgetFeatures features );
    void showContextMenu();

private:
    QwCustomDockWidget *m_dock;

    QHBoxLayout *horizontalLayout;
    QLabel      *titleLabel;
    QToolButton *actionsButton;
    QToolButton *restoreButton;
    QToolButton *closeButton;

    void createToggleActions( const QList<QAction *> &actions );
    void readSettings( QMap <QString, bool> &actSettings );
    QString m_dockTitle;
    QList<QAction *> m_toggleActions;
    QMenu *m_toggleMenu;

    QMainWindow *m_mainWindow;
    QString m_appName;

    bool m_innerMode;

    int m_permanentToolButtonsCount;
};

class QTCREATOR_UTILS_EXPORT QwCustomDockWidget: public QDockWidget
{
    Q_OBJECT

public:
    enum TabifyHint {
        First,
        Second,
        Third
    };

    typedef QPair <Qt::DockWidgetArea, TabifyHint> DockAreaTabifyHint;

    explicit QwCustomDockWidget(QMainWindow *mainWindow, const QString &title, const QString &objName,
                                const QKeySequence &toggleShortcut, const QIcon &toggleIcon,
                                Qt::DockWidgetArea initialDockArea, TabifyHint tabifyHint,
                                const QList<QAction *> &actions = QList<QAction *>(),
                                QWidget *widget = nullptr, bool innerMode = false);
    virtual ~QwCustomDockWidget();

    void setContextMenu( QMenu *menu );
    void writeSettings();
    Qt::DockWidgetArea initialDockArea() const;
    TabifyHint initialTabifyHint() const;

    QToolButton *createPermanentToolButton(QAction *action);

    void setWidget(QWidget *widget);

protected:
    void paintEvent( QPaintEvent *event );
    QWidget *customWidget() { return m_customWidget; }

public slots:
    void toggleFloating();
    void showContextMenu(const QPoint &pos);
    void toggled(bool onoff);
    void showAndSetCurrentIfTabified(bool *indexChanged = nullptr);
    void popup();

protected:
    QMenu *m_contextMenu;
    QwTitleBar *m_titleBar;
    QWidget *m_customWidget;
    QMainWindow *m_mainWindow;

    Qt::DockWidgetArea m_initialDockArea;
    TabifyHint m_tabifyHint;
};

class QTCREATOR_UTILS_EXPORT QwTabBarDockWidget: public QwCustomDockWidget
{
    Q_OBJECT

public:
    QwTabBarDockWidget(QMainWindow *mainWindow, const QString &title, const QString &objName,
                       const QKeySequence &toggleShortcut, const QIcon &toggleIcon,
                       Qt::DockWidgetArea initialDockArea, TabifyHint tabifyHint,
                       const QList<QAction *> &actions = QList<QAction *>(),
                       QwTabCloseResolver *tabCloseRsolver = nullptr);

    void addTab( const QIcon &icon, const QString &title, const QString &toolTip, QWidget *widget );

    int currentIndex() const;
    QWidget *currentWidget() const;
    QWidget *tabWidget(int index) const;
    int indexOf(QWidget *widget) const;
    int count() const;

signals:
    void currentChanged(int index);
    void tabRemoved(int index);

public slots:
    void setCurrentIndex(int index);

    void removeCurrentTab();
    void removeAllTabs();
    void nextTab();
    void prevTab();

private slots:
    void handleTabRemoved( int index );
    void handleAllTabsRemoved();

private:
    void setWidget( QWidget *widget );

    QStackedWidget *m_stackedWidget;
    QwTabBarHolder *m_tabBar;
};

class QTCREATOR_UTILS_EXPORT QwTabCornerToolBar: public QToolBar, public Utils::StyleManagedWidget
{
    Q_OBJECT

public:
    QwTabCornerToolBar(QWidget *parent = nullptr);
};

#endif // QWDOCKWIDGET_H
