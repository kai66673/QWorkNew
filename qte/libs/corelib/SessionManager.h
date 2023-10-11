#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QObject>
#include <QList>
#include <QStringList>

#include "corelib_global.h"
#include "ISessionClient.h"
#include "IMainWindow.h"
#include "SessionsDialog.h"

class CORELIB_EXPORT SessionManager : public QObject
{
    Q_OBJECT

public:
    explicit SessionManager( const QString &orgName, const QString &appName,
                             IMainWindow *mainWindow, QObject *parent = 0 );
    virtual ~SessionManager();

    static SessionManager *getInstance();

    void registerSessionClient( ISessionClient *client );

public slots:
    void saveSessions();
    void loadSessions();

    void saveCurrentSession();
    void switchToSession( int sessionIndex = -1, bool save = false );
    
private:
    QList <ISessionClient *> m_clients;
    QString m_orgName;
    QString m_appName;
    IMainWindow *m_mainWindow;

    bool m_restoreLastSession;
    int m_currentSessionIndex;
    QStringList m_sessions;

    static SessionManager *m_instance;

    void deleteSession( int index );

    friend class SessionsDialog;
};

#endif // SESSIONMANAGER_H
