#include <QDir>
#include <QDomDocument>
#include <QTextStream>
#include <QSettings>

#include "qtcassert.h"
#include "SessionManager.h"

SessionManager *SessionManager::m_instance = 0;

SessionManager::SessionManager( const QString &orgName, const QString &appName,
                                IMainWindow *mainWindow, QObject *parent )
    : QObject(parent)
    , m_orgName(orgName)
    , m_appName(appName)
    , m_mainWindow(mainWindow)
    , m_restoreLastSession(false)
    , m_currentSessionIndex(-1)
{
    m_instance = this;
    loadSessions();

    QSettings settings(m_orgName, m_appName);

    settings.beginGroup("SessionsManager");
    m_restoreLastSession = settings.value("restoreLastSession", false).toBool();
    if ( m_restoreLastSession )
        m_currentSessionIndex = settings.value("lastSessionIndex", 0).toInt();
    settings.endGroup();
}

SessionManager::~SessionManager()
{
    saveSessions();

    QSettings settings(m_orgName, m_appName);

    settings.beginGroup("SessionsManager");
    settings.setValue("restoreLastSession", m_restoreLastSession);
    settings.setValue("lastSessionIndex", m_currentSessionIndex);
    settings.endGroup();
}

SessionManager *SessionManager::getInstance()
{
    return m_instance;
}

void SessionManager::registerSessionClient( ISessionClient *client )
{
    m_clients.append(client);
}

void SessionManager::saveSessions()
{
    QString dirName = m_mainWindow->applicationDataLocation() + "/.sessions";
    QDir dir(dirName);

    if ( !dir.exists() )
        dir.mkpath(dirName);
    dirName += "/";

    QString fileName = dirName + "sessions.xml";

    QFile file(fileName);

    if ( !file.open(QFile::WriteOnly | QFile::Text) ) {
        m_mainWindow->showWarning( tr("Error On Save Sessions\nFile \"%1\" Cannot be Opened for Writing:\n%2.")
                          .arg(fileName).arg(file.errorString()) );
        return;
    }

    QDomDocument domDocument("sessions");

    domDocument.appendChild(domDocument.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"windows-1251\""));

    QDomElement root = domDocument.createElement("sessions");
    root.setAttribute("version", "1.0");
    domDocument.appendChild(root);

    for (int i = 1; i < m_sessions.size(); i++) {
        QDomElement itemElement = domDocument.createElement("item");
        itemElement.setAttribute("sessionName", m_sessions[i]);
        root.appendChild(itemElement);
    }

    const int IndentSize = 4;
    QTextStream out(&file);
    domDocument.save(out, IndentSize);

    file.close();
}

void SessionManager::loadSessions()
{
    m_sessions.clear();
    m_sessions.append(tr("default"));

    QString dirName = m_mainWindow->applicationDataLocation() + "/.sessions";
    QDir dir(dirName);

    if ( !dir.exists() )
        return;
    dirName += "/";

    QString fileName = dirName + "sessions.xml";

    QFile file(fileName);
    if ( !file.exists() )
        return;

    QDomDocument domDocument;

    QString errorStr;
    int errorLine;
    int errorColumn;

    // Загружаем всю структуру
    if (!domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn)) {
        m_mainWindow->showWarning( tr("Error On Load Sessions\nCommon Error in xml-file:\nLine %1, Column %2: %3")
                          .arg(errorLine).arg(errorColumn).arg(errorStr) );
        return;
    }

    // Первичная проверка (тип и версия документа)
    QDomElement root = domDocument.documentElement();
    if ( root.tagName() != "sessions" ) {
        m_mainWindow->showWarning( tr("Error On Load Sessions\nInvalid Format of Sessions xml-file") );
        return;
    }
    if ( !root.hasAttribute("version") ) {
        m_mainWindow->showWarning( tr("Error On Load Sessions\nInvalid Format of Sessions xml-file (version expected)") );
        return;
    }
    if ( root.attribute("version") != "1.0" ) {
        m_mainWindow->showWarning( tr("Error On Load Sessions\nInvalid Format of Sessions xml-file (wrong version)") );
        return;
    }

    QDomElement itemElement = root.firstChildElement();
    while ( !itemElement.isNull() ) {
        if ( itemElement.tagName() != "item" ) {
            m_mainWindow->showWarning( tr("Error On Load Sessions\nInvalid Format of Sessions xml-file (unknown tag <%1> at line %2)")
                              .arg(itemElement.tagName()).arg(itemElement.lineNumber()) );
            return;
        }
        m_sessions.append(itemElement.attribute("sessionName"));
        itemElement = itemElement.nextSiblingElement();
    }
}

void SessionManager::saveCurrentSession()
{
    if ( m_currentSessionIndex == -1 )
        return;

    QString dirName = m_mainWindow->applicationDataLocation() + "/.sessions/";
    dirName += m_sessions[m_currentSessionIndex];
    QDir dir(dirName);

    if ( !dir.exists() )
        dir.mkpath(dirName);

    for (int i = 0; i < m_clients.size(); i++)
        m_clients[i]->saveSession(dirName);
}

void SessionManager::switchToSession( int sessionIndex , bool save )
{
    if ( sessionIndex == -1 )
        sessionIndex = m_currentSessionIndex;

    if ( sessionIndex == -1 ) {
        m_currentSessionIndex = 0;
        return;
    }

    int i;

    for (i = 0; i < m_clients.size(); i++)
        if ( !m_clients[i]->sessionAboutToClose() )
            return;

    if ( save )
        saveCurrentSession();

    for (i = 0; i < m_clients.size(); i++)
        m_clients[i]->sessionClose();

    m_currentSessionIndex = sessionIndex;

    QTC_ASSERT( m_currentSessionIndex >= 0 && m_currentSessionIndex < m_sessions.size(), m_currentSessionIndex = 0 );

    QString dirName = m_mainWindow->applicationDataLocation() + "/.sessions/";
    dirName += m_sessions[m_currentSessionIndex];
    QDir dir(dirName);

    if ( !dir.exists() )
        dir.mkpath(dirName);

    for (i = 0; i < m_clients.size(); i++)
        m_clients[i]->loadSession(dirName);
}

static void deleteAllFiles( const QString &path )
{
    QDir oDir(path);
    QStringList files = oDir.entryList(QDir::Files);

    QStringList::Iterator itFile = files.begin();
    while ( itFile != files.end() ) {
        QFile oFile(path + "/" + *itFile);
        oFile.remove();
        ++itFile;
    }

    QStringList dirs = oDir.entryList(QDir::Dirs);
    QStringList::Iterator itDir = dirs.begin();
    while ( itDir != dirs.end() ) {
        if ( *itDir != "." && *itDir != ".." )
            deleteAllFiles(path + "/" + *itDir);
        ++itDir;
    }

    oDir.rmdir(path);
}

void SessionManager::deleteSession( int index )
{
    if ( index > 0 && m_currentSessionIndex != index ) {
        QString location = m_sessions[index];
        m_sessions.removeAt(index);
        if ( index < m_currentSessionIndex )
            m_currentSessionIndex--;

        QString dirName = m_mainWindow->applicationDataLocation() + "/.sessions/";
        dirName += location;
        deleteAllFiles(dirName);
    }
}
