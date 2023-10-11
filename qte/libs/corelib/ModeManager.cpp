#include "QwDockWidget.h"
#include "ModeManager.h"
#include "Core.h"

namespace Core {

GuiModeClient::GuiModeClient()
{
    ModeManager::registerGuiClient(this);
}

GuiModeClient::~GuiModeClient()
{ }

DockModeClient::DockModeClient(QDockWidget *dockWidget)
    : GuiModeClient()
    , m_dockWidget(dockWidget)
{ }

DockModeClient::~DockModeClient()
{ }

void DockModeClient::setClientEnabled(bool enabled)
{
    enabled ? m_dockWidget->show() : m_dockWidget->hide();
}

void DockModeClient::setToggleActionEnabled(bool enabled)
{
    m_dockWidget->toggleViewAction()->setEnabled(enabled);
}

ToolBarModeClient::ToolBarModeClient(MainToolBar *toolBar)
    : GuiModeClient()
    , m_toolBar(toolBar)
{ }

ToolBarModeClient::~ToolBarModeClient()
{ }

void ToolBarModeClient::setClientEnabled(bool enabled)
{
    enabled ? m_toolBar->show() : m_toolBar->hide();
}

void ToolBarModeClient::setToggleActionEnabled(bool enabled)
{
    m_toolBar->toggleViewAction()->setEnabled(enabled);
}

EditModeDockGuiClient::EditModeDockGuiClient(QDockWidget *dock)
    : DockModeClient(dock)
{ }

EditModeDockGuiClient::~EditModeDockGuiClient()
{ }

IMode *EditModeDockGuiClient::preferredMode(ScreenMode screenMode) const
{ return screenMode == Core::FullScreen ? 0 : Core::ModeManager::editMode(); }

bool EditModeDockGuiClient::isEnabledForMode(IMode *) const
{ return true; }

EditModeToolBarGuiClient::EditModeToolBarGuiClient(MainToolBar *toolBar)
    : ToolBarModeClient(toolBar)
{ }

EditModeToolBarGuiClient::~EditModeToolBarGuiClient()
{ }

IMode *EditModeToolBarGuiClient::preferredMode(ScreenMode /*screenMode*/) const
{ return Core::ModeManager::editMode(); }

bool EditModeToolBarGuiClient::isEnabledForMode(IMode *) const
{ return true; }

ModeManager *ModeManager::m_instance = 0;

ModeManager::ModeManager(QObject *parent)
    : QObject(parent)
    , m_screenMode(Normal)
    , m_activeMode(0)
    , m_editMode(new EditMode)
    , m_isMaximazed(false)
{
    m_instance = this;
    registerModeInternal(m_editMode);
}

ModeManager::~ModeManager()
{
    qDeleteAll(m_modes);
    foreach( GuiModeClient *mc, m_guiClients )
        if ( !mc->isManaged() )
            delete mc;
}

void ModeManager::registerModeInternal(IMode *mode)
{
    m_modes.append(new ModeState(mode));
}

ModeManager *ModeManager::instance()
{
    return m_instance;
}

void ModeManager::setScreenMode(ScreenMode screenMode)
{
    m_instance->setScreenModeInternal(screenMode);
}

void ModeManager::registerMode(IMode *mode)
{
    m_instance->registerModeInternal(mode);
}

void ModeManager::switchToMode(IMode *mode)
{
    m_instance->switchToModeInternal(mode);
}

void ModeManager::writeSettings(QSettings &settings)
{
    m_instance->writeSettingsInternal(settings);
}

void ModeManager::readSettings(QSettings &settings)
{
    m_instance->readSettingsInternal(settings);
}

IMode *ModeManager::editMode()
{
    return m_instance->m_editMode;
}

IMode *ModeManager::activeMode()
{
    return m_instance->m_activeMode;
}

void ModeManager::registerGuiClient(GuiModeClient *client)
{
    m_instance->m_guiClients.append(client);
}

void ModeManager::setScreenModeInternal(ScreenMode screenMode)
{
    if ( m_screenMode == screenMode )
        return;

    ScreenMode oldScreenMode = m_screenMode;
    m_screenMode = screenMode;

    QMainWindow *mainWindow = Core::Storage::mainWindow()->widget();

    if ( ModeState *ms = modeStateForMode(m_activeMode) ) {
        ms->states[oldScreenMode] = mainWindow->saveState();

        if ( m_screenMode == FullScreen ) {
            m_isMaximazed = mainWindow->isMaximized();
            mainWindow->showFullScreen();
        } else {
            m_isMaximazed ? mainWindow->showMaximized() : mainWindow->showNormal();
        }

        if ( !ms->states[screenMode].isEmpty() ) {
            mainWindow->restoreState(ms->states[screenMode]);
        } else {
            restorePreferredStateForMode(m_activeMode);
            ms->states[screenMode] = mainWindow->saveState();
        }
    }
}

void ModeManager::switchToModeInternal(IMode *mode)
{
    if ( !mode )
        mode = m_editMode;

    if ( mode == m_activeMode )
        return;

    QMainWindow *mainWindow = Core::Storage::mainWindow()->widget();

    if (  m_activeMode ) {
        if ( ModeState *ms = modeStateForMode(m_activeMode) ) {
            ms->states[m_screenMode] = mainWindow->saveState();
        }
    }


    if ( ModeState *ms = modeStateForMode(mode) ) {
        if ( !ms->states[m_screenMode].isEmpty() ) {
            mainWindow->restoreState(ms->states[m_screenMode]);
        } else {
            restorePreferredStateForMode(mode);
            ms->states[m_screenMode] = mainWindow->saveState();
        }
    }

    refreshToggleActionsForMode(mode);
    m_activeMode = mode;
}

void ModeManager::writeSettingsInternal(QSettings &settings)
{
    QMainWindow *mainWindow = Core::Storage::mainWindow()->widget();

    foreach ( ModeState *ms, m_modes ) {
        for (int i = 0; i < ScreenModeCount; i++) {
            if ( ms->mode == m_activeMode && i == m_screenMode) {
                ms->states[i] = mainWindow->saveState();
            }
            settings.setValue(QString("Mode.%1.State.%2").arg(i).arg(ms->mode->id().toString()), ms->states[i]);
        }
    }
}

void ModeManager::readSettingsInternal(QSettings &settings)
{
    foreach ( ModeState *ms, m_modes ) {
        for (int i = 0; i < ScreenModeCount; i++) {
            ms->states[i] = settings.value(QString("Mode.%1.State.%2").arg(i).arg(ms->mode->id().toString()), QByteArray()).toByteArray();
        }
    }
}

ModeManager::ModeState *ModeManager::modeStateForMode(IMode *mode)
{
    foreach ( ModeState *ms, m_modes )
        if ( ms->mode == mode )
            return ms;
    return 0;
}

void ModeManager::restorePreferredStateForMode(IMode *mode)
{
    foreach ( GuiModeClient *gc, m_guiClients )
        gc->setClientEnabled(gc->preferredMode(m_screenMode) == mode);
}

void ModeManager::refreshToggleActionsForMode(IMode *mode)
{
    foreach ( GuiModeClient *gc, m_guiClients )
        gc->setToggleActionEnabled(gc->isEnabledForMode(mode));
}

DockWidgetHandler::DockWidgetHandler(QwCustomDockWidget *dock, DockModeClient *guiClient)
    : m_dock(dock)
    , m_guiClient(guiClient)
{ }

ToolBarHandler::ToolBarHandler(MainToolBar *toolBar, ToolBarModeClient *guiClient)
    : m_toolBar(toolBar)
    , m_guiClient(guiClient)
{ }

}   // namespace Core
