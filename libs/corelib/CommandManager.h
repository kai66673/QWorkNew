#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QAction>
#include <QKeySequence>

#include "corelib_global.h"
#include "IMainWindow.h"

class IDocument;

class CORELIB_EXPORT IDocumentHelper
{
public:
    IDocumentHelper();
    virtual bool documentEventFilter( IDocument *document, QEvent *event ) = 0;
};

namespace Core {

class CORELIB_EXPORT Command: public QObject
{
public:
    Command( const QString &commandId,
             const QString &title,
             const QString &toolTipText,
             const QKeySequence &defaultShortcut,
             QObject *parent );

    Command( const QString &commandId,
             const QIcon &icon,
             const QString &title,
             const QString &toolTipText,
             const QKeySequence &defaultShortcut,
             QObject *parent );

    Command( QAction *action,
             const QString &commandId,
             const QIcon &icon,
             const QString &title,
             const QString &toolTipText,
             const QKeySequence &defaultShortcut,
             QObject *parent );

    Command( QAction *action,
             const QString &commandId,
             const QKeySequence &defaultShortcut,
             QObject *parent );

    void updateShortcut( const QKeySequence &shortcut );

    inline QAction *action() { return m_action; }
    inline QString Id() const { return m_commandId; }
    inline QString defaultShortcut() const { return m_defaultShortcut.toString(QKeySequence::NativeText); }
    inline QString currentShortcut() const { return m_currentShortcut.toString(QKeySequence::NativeText); }

private:
    void initCommand();

    QString         m_commandId;
    QString         m_toolTipText;
    QKeySequence    m_defaultShortcut;
    QKeySequence    m_currentShortcut;
    QAction        *m_action;
};

struct CORELIB_EXPORT CommandGroup {
    QString groupId;
    QList<Command *> commands;
};


class CORELIB_EXPORT CommandManager : public QObject
{
    Q_OBJECT

public:
    CommandManager( IMainWindow *mainWindow, QObject *parent = 0 );
    ~CommandManager();

    static CommandManager *getInstance();

    typedef QMap<QString, QList<Command *> > CommandsMap;

    void registerDocumentHelper( IDocumentHelper *documentHelper );
    void registerSignal( QObject *sender, const QByteArray &signal );
    void connectSlotToSignal( QObject *receiver, const QByteArray &slot, const QByteArray &signal );
    void registerSlot( QObject *receiver, const QByteArray &slot );
    void connectSignalToSlot( QObject *sender, const QByteArray &signal, const QByteArray &slot );

    void registerCommands( const QString &groupId, const QList<Command *> &commands );
    void saveCommands();

public slots:
    void onShortcutsModifyRequest();

protected:
    bool eventFilter( QObject *sender, QEvent *event );

private:
    IMainWindow *m_mainWindow;

    QList <IDocumentHelper *> documentHelpers;
    QMap <QByteArray, QObject *> signalsMap;
    QMap <QByteArray, QObject *> slotsMap;

    static CommandManager *commandManager;

    CommandsMap commandsMap;
};

} // namespace Core

#endif // COMMANDMANAGER_H
