#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QtGui>
#include <QtPlugin>
#include <QObject>

QT_BEGIN_NAMESPACE
class QDockWidget;
class QMenu;
class QStringList;
QT_END_NAMESPACE

#include "CommandManager.h"
#include "QwDockWidget.h"
#include "ISettingsFactory.h"
#include "IMainWindow.h"
#include "ModeManager.h"
#include "IGuiFactory.h"
#include "MainToolBar.h"

class ISettingsFactory;
class UiProviderInterface;

class IPlugin : public QObject, public ISettingsFactory, public IGuiFactory
{
public:
    virtual QString id() const = 0;
    virtual QMenu *menu() { return 0; }

    virtual QList<QByteArray> signalList() const { return QList<QByteArray>(); }
    void registerSignals () {
        QList<QByteArray> sList = signalList();
        Core::CommandManager *commandManager = Core::CommandManager::getInstance();
        foreach( QByteArray signal, sList )
            commandManager->registerSignal(this, signal);
    }

    virtual QList<QByteArray> slotList() const { return QList<QByteArray>(); }
    void registerSlots() {
        QList<QByteArray> sList = slotList();
        Core::CommandManager *commandManager = Core::CommandManager::getInstance();
        foreach ( QByteArray slot, sList )
            commandManager->registerSlot(this, slot);
    }

    virtual void initialize() = 0;
};

class UiProviderInterface
{
public:
    UiProviderInterface( QwCustomDockWidget *dock = 0, Core::MainToolBar *toolBar = 0 )
        : m_dock(dock)
        , m_toolBar(toolBar)
    { }

    virtual ~UiProviderInterface()
    { }

    virtual Core::DockWidgetHandler *dockWidgetHandler() {
        return m_dock ? new Core::DockWidgetHandler(m_dock, new Core::EditModeDockGuiClient(m_dock)) : 0;
    }

    virtual Core::ToolBarHandler *toolBarHandler() {
        return m_toolBar ? new Core::ToolBarHandler(m_toolBar, new Core::EditModeToolBarGuiClient(m_toolBar)) : 0;
    }

protected:
    QwCustomDockWidget *m_dock;
    Core::MainToolBar *m_toolBar;
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IPlugin, "QWorkBase.IPlugin/1.0")
QT_END_NAMESPACE

#endif // IPLUGIN_H
