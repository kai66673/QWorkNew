#ifndef MODEMANAGER_H
#define MODEMANAGER_H

#include <QObject>
#include <QSettings>
#include <QDockWidget>

#include "id.h"
#include "corelib_global.h"
#include "coreconstants.h"
#include "MainToolBar.h"

class QwCustomDockWidget;

namespace Core {

enum ScreenMode {
    Normal = 0,
    FullScreen,
    ScreenModeCount
};

class CORELIB_EXPORT IMode
{
public:
    virtual ~IMode() {}
    virtual Id id() const = 0;
};

class CORELIB_EXPORT GuiModeClient
{
public:
    GuiModeClient();
    virtual ~GuiModeClient();

    virtual IMode *preferredMode(ScreenMode screenMode) const = 0;
    virtual bool isEnabledForMode(IMode *mode) const = 0;
    virtual void setClientEnabled(bool enabled) = 0;
    virtual void setToggleActionEnabled(bool enabled) = 0;
    virtual bool isManaged() const { return false; }
};

class CORELIB_EXPORT DockModeClient: public GuiModeClient
{
public:
    DockModeClient(QDockWidget *dockWidget);
    virtual ~DockModeClient();

    virtual void setClientEnabled(bool enabled);
    virtual void setToggleActionEnabled(bool enabled);

private:
    QDockWidget *m_dockWidget;
};

class CORELIB_EXPORT ToolBarModeClient: public GuiModeClient
{
public:
    ToolBarModeClient(MainToolBar *toolBar);
    virtual ~ToolBarModeClient();

    virtual void setClientEnabled(bool enabled);
    virtual void setToggleActionEnabled(bool enabled);

private:
    MainToolBar *m_toolBar;
};

class CORELIB_EXPORT EditModeDockGuiClient: public DockModeClient
{
public:
    EditModeDockGuiClient(QDockWidget *dock);
    virtual ~EditModeDockGuiClient();

    virtual Core::IMode *preferredMode(Core::ScreenMode screenMode) const;
    virtual bool isEnabledForMode(Core::IMode * /*mode*/) const;
};

class CORELIB_EXPORT EditModeToolBarGuiClient: public ToolBarModeClient
{
public:
    EditModeToolBarGuiClient(MainToolBar *toolBar);
    virtual ~EditModeToolBarGuiClient();

    virtual Core::IMode *preferredMode(Core::ScreenMode) const;
    virtual bool isEnabledForMode(Core::IMode * /*mode*/) const;
};

class CORELIB_EXPORT EditMode: public IMode
{
public:
    virtual Id id() const { return Core::Constants::MODE_EDIT; }
};

class CORELIB_EXPORT ModeManager: public QObject
{
    Q_OBJECT

public:
    ModeManager(QObject *parent = 0);
    virtual ~ModeManager();

    static ModeManager *instance();
    static void setScreenMode(ScreenMode screenMode);
    static void registerMode(IMode *mode);
    static void switchToMode(IMode *mode);
    static void writeSettings(QSettings &settings);
    static void readSettings(QSettings &settings);
    static IMode *editMode();
    static IMode *activeMode();
    static void registerGuiClient(GuiModeClient *client);

    static ModeManager *m_instance;

private:
    struct ModeState {
        ModeState(IMode *mode_)
            : mode(mode_)
        { }

        ~ModeState() {
            delete mode;
        }

        IMode *mode;
        QByteArray states[ScreenModeCount];
    };

    void setScreenModeInternal(ScreenMode screenMode);

    void registerModeInternal(IMode *mode);
    void switchToModeInternal(IMode *mode);
    void writeSettingsInternal(QSettings &settings);
    void readSettingsInternal(QSettings &settings);

    ModeState *modeStateForMode(IMode *mode);
    void restorePreferredStateForMode(IMode *mode);
    void refreshToggleActionsForMode(IMode *mode);

    ScreenMode m_screenMode;
    IMode *m_activeMode;
    IMode *m_editMode;
    QList<ModeState *> m_modes;
    QList<GuiModeClient *> m_guiClients;
    bool m_isMaximazed;
};

class CORELIB_EXPORT DockWidgetHandler
{
public:
    DockWidgetHandler(QwCustomDockWidget *dock, DockModeClient *guiClient);

    QwCustomDockWidget *m_dock;
    GuiModeClient *m_guiClient;
};

class CORELIB_EXPORT ToolBarHandler
{
public:
    ToolBarHandler(MainToolBar *toolBar, ToolBarModeClient *guiClient);

    MainToolBar *m_toolBar;
    GuiModeClient *m_guiClient;
};

}   // namespace Core

#endif // MODEMANAGER_H
