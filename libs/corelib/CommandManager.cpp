#include <QEvent>
#include <QKeyEvent>
#include <QSettings>

#include "CommandManager.h"
#include "CommandShortcutsDialog.h"
#include "IDocument.h"

namespace Core {

CommandManager *CommandManager::commandManager(0);

CommandManager::CommandManager(IMainWindow *mainWindow, QObject *parent )
    : QObject(parent)
    , m_mainWindow(mainWindow)
{
    commandManager = this;
}

CommandManager::~CommandManager()
{
    documentHelpers.clear();
    signalsMap.clear();
    slotsMap.clear();
}

CommandManager *CommandManager::getInstance()
{
    return commandManager;
}

void CommandManager::registerDocumentHelper( IDocumentHelper *documentHelper )
{
    documentHelpers.append(documentHelper);
}

void CommandManager::registerSignal( QObject *sender, const QByteArray &signal )
{
    if ( signalsMap.find(signal) != signalsMap.end() )
        return;

    signalsMap[signal] = sender;
}

void CommandManager::connectSlotToSignal( QObject *receiver, const QByteArray &slot, const QByteArray &signal )
{
    if ( signalsMap.find(signal) == signalsMap.end() )
        return;

    QObject *sender = signalsMap[signal];

    QByteArray realSignal = "2" + signal + "(QVariantList&)";
    QByteArray realSlot   = "1" + slot   + "(QVariantList&)";

    connect(sender, realSignal, receiver, realSlot);
}

void CommandManager::registerSlot( QObject *receiver, const QByteArray &slot )
{
    if ( slotsMap.find(slot) != slotsMap.end() )
        return;

    slotsMap[slot] = receiver;
}

void CommandManager::connectSignalToSlot( QObject *sender, const QByteArray &signal, const QByteArray &slot )
{
    if ( slotsMap.find(slot) == slotsMap.end() )
        return;

    QObject *receiver = slotsMap[slot];

    QByteArray realSignal = "2" + signal + "(QVariantList&)";
    QByteArray realSlot   = "1" + slot   + "(QVariantList&)";

    connect(sender, realSignal, receiver, realSlot);
}

void CommandManager::registerCommands( const QString &groupId, const QList<Command *> &commands )
{
    QSettings settings(m_mainWindow->orgName(), m_mainWindow->appName());
    settings.beginGroup("Shortcut");
    settings.beginGroup(groupId);

    foreach ( Command *command, commands ) {
        QString commandId = command->Id();
        QString defaultShortcut = command->defaultShortcut();
        QString currentShortcut = settings.value(commandId, defaultShortcut).toString();
        command->updateShortcut(currentShortcut);
    }

    settings.endGroup();
    settings.endGroup();

    commandsMap[groupId] << commands;
}

void CommandManager::saveCommands()
{
    QSettings settings(m_mainWindow->orgName(), m_mainWindow->appName());
    settings.beginGroup("Shortcut");
    QMutableMapIterator <QString, QList<Command *> > i(commandsMap);
    while ( i.hasNext() ) {
        i.next();
        settings.beginGroup(i.key());
        foreach ( Command *command, i.value() )
            settings.setValue(command->Id(), command->currentShortcut());
        settings.endGroup();
    }
    settings.endGroup();
}

void CommandManager::onShortcutsModifyRequest()
{
    CommandShortcutsDialog shortcutsDialog(&commandsMap, m_mainWindow->widget());
    shortcutsDialog.exec();
}

bool CommandManager::eventFilter( QObject *sender, QEvent *event )
{
    IDocument *document = DocumentManager::getInstance()->currentDocument();

    if ( document ) {
        foreach(IDocumentHelper *helper, documentHelpers)
            if ( helper->documentEventFilter(document, event) )
                return true;
    }

    return QObject::eventFilter(sender, event);
}

Command::Command( const QString &commandId,
                  const QString &title,
                  const QString &toolTipText,
                  const QKeySequence &defaultShortcut,
                  QObject *parent )
    : QObject(parent)
    , m_commandId(commandId)
    , m_toolTipText(toolTipText)
    , m_defaultShortcut(defaultShortcut)
    , m_currentShortcut(defaultShortcut)
    , m_action(new QAction(title, parent))
{
    initCommand();
}

Command::Command( const QString &commandId,
                  const QIcon &icon,
                  const QString &title,
                  const QString &toolTipText,
                  const QKeySequence &defaultShortcut,
                  QObject *parent )
    : QObject(parent)
    , m_commandId(commandId)
    , m_toolTipText(toolTipText)
    , m_defaultShortcut(defaultShortcut)
    , m_currentShortcut(defaultShortcut)
    , m_action(new QAction(icon, title, parent))
{
    initCommand();
}

Command::Command ( QAction *action,
                   const QString &commandId,
                   const QIcon &icon,
                   const QString &title,
                   const QString &toolTipText,
                   const QKeySequence &defaultShortcut,
                   QObject *parent )
    : QObject(parent)
    , m_commandId(commandId)
    , m_toolTipText(toolTipText)
    , m_defaultShortcut(defaultShortcut)
    , m_currentShortcut(defaultShortcut)
    , m_action(action)
{
    m_action->setIcon(icon);
    m_action->setText(title);
    initCommand();
}

Command::Command( QAction *action,
                  const QString &commandId,
                  const QKeySequence &defaultShortcut,
                  QObject *parent )
    : QObject(parent)
    , m_commandId(commandId)
    , m_toolTipText(action->toolTip())
    , m_defaultShortcut(defaultShortcut)
    , m_currentShortcut(defaultShortcut)
    , m_action(action)
{
    initCommand();
}

void Command::updateShortcut( const QKeySequence &shortcut )
{
    m_currentShortcut = shortcut;
    initCommand();
}

void Command::initCommand()
{
    m_action->setStatusTip(m_toolTipText);

    if ( m_currentShortcut.isEmpty() ) {
        m_action->setToolTip(QString("<b>%1</b>").arg(m_toolTipText));
        return;
    }

    m_action->setToolTip( QString("<b>%1</b> <span style=\"color: gray; font-size: small\">%2</span>")
                          .arg(m_toolTipText)
                          .arg(m_currentShortcut.toString(QKeySequence::NativeText)) );
    m_action->setShortcut(m_currentShortcut);
}

} // namespace Core


IDocumentHelper::IDocumentHelper()
{
    Core::CommandManager::getInstance()->registerDocumentHelper(this);
}
