#include <QFileInfo>
#include <QSettings>

#include "RecentFilesMenu.h"

RecentFilesMenu::RecentFilesMenu( const QString &orgName, const QString &appName, IRecentFilesDelegate *delegete, int maxItems, QWidget *parent )
    : QMenu(delegete->menuName(), parent)
    , m_delegeate(delegete)
    , m_maxItems(maxItems)
    , m_orgName(orgName)
    , m_appName(appName)
{
    loadSettings();
}

RecentFilesMenu::~RecentFilesMenu()
{
    saveSettings();
    delete m_delegeate;
}

void RecentFilesMenu::appendItem( const QString &fileName )
{
    int foundIndex = -1;
    QFileInfo fi(fileName);
    QString filePath = fi.filePath();
    for (int i = 0; i < m_fileNames.size(); i++) {
#ifdef Q_OS_WIN
        if ( !m_fileNames[i].compare(filePath, Qt::CaseInsensitive) ) {
            foundIndex = i;
            break;
        }
#else
        if ( m_fileNames[i] == filePath ) {
            foundIndex = i;
            break;
        }
#endif
    }

    RecentFilesAction *newAction = new RecentFilesAction(filePath, this);
    connect(newAction, SIGNAL(triggered()), this, SLOT(openRecentFile()));

    if ( foundIndex == -1 ) {
        // New recent files Item
        if ( m_fileNames.size() == m_maxItems ) {
            m_fileNames.removeLast();
            removeAction(actions().last());
        }
        m_fileNames.prepend(filePath);
        if ( !actions().isEmpty() )
            insertAction(actions().first(), newAction);
        else
            addAction(newAction);
    }

    else {
        // Existing recent files Item
        m_fileNames.removeAt(foundIndex);
        removeAction(actions().at(foundIndex));
        m_fileNames.prepend(filePath);
        if ( !actions().isEmpty() )
            insertAction(actions().first(), newAction);
        else
            addAction(newAction);
    }
}

void RecentFilesMenu::updateItem( const QString &oldFileName, const QString &newFileName )
{
    int foundIndex = -1;
    QFileInfo fi(oldFileName);
    QString filePath = fi.filePath();
    for (int i = 0; i < m_fileNames.size(); i++) {
#ifdef Q_OS_WIN
        if ( !m_fileNames[i].compare(filePath, Qt::CaseInsensitive) ) {
            foundIndex = i;
            break;
        }
#else
        if ( m_fileNames[i] == filePath ) {
            foundIndex = i;
            break;
        }
#endif
    }

    if ( foundIndex == -1 )
        return;

    QFileInfo nfi(newFileName);
    filePath = nfi.filePath();
    m_fileNames[foundIndex] = filePath;
    actions().at(foundIndex)->setText(filePath);
}

void RecentFilesMenu::setEnabledAllItems( bool onoff )
{
    foreach ( QAction *act, actions() )
        act->setEnabled(onoff);
}

void RecentFilesMenu::openRecentFile()
{
    RecentFilesAction *action = qobject_cast<RecentFilesAction *>(sender());
    if ( action ) {
        QString filePath = action->filePath();
        m_delegeate->openFile(filePath);
        int selIndex = -1;
        for (int i = 0; i < actions().size(); i++) {
            if ( actions().at(i) == action ) {
                selIndex = i;
                break;
            }
        }
        if ( selIndex > 0 ) {
            m_fileNames.removeAt(selIndex);
            removeAction(action);
            m_fileNames.prepend(filePath);
            RecentFilesAction *newAction = new RecentFilesAction(filePath, this);
            connect(newAction, SIGNAL(triggered()), this, SLOT(openRecentFile()));
            insertAction(actions().first(), newAction);
        }
    }
}

void RecentFilesMenu::saveSettings()
{
    QSettings settings(m_orgName, m_appName);

    settings.beginWriteArray(m_delegeate->settingName(), m_fileNames.size());

    for (int i = 0; i < m_fileNames.size(); i++) {
        settings.setArrayIndex(i);
        settings.setValue("FileName", m_fileNames[i]);
    }

    settings.endArray();
}

void RecentFilesMenu::loadSettings()
{
    QSettings settings(m_orgName, m_appName);

    int filesCount = settings.beginReadArray(m_delegeate->settingName());
    if ( filesCount > m_maxItems )
        filesCount = m_maxItems;

    for (int i = 0; i < filesCount; i++) {
        settings.setArrayIndex(i);
        QString filePath = settings.value("FileName").toString();
        m_fileNames.append(filePath);
        RecentFilesAction *newAction = new RecentFilesAction(filePath, this);
        connect(newAction, SIGNAL(triggered()), this, SLOT(openRecentFile()));
        addAction(newAction);
    }

    settings.endArray();
}
