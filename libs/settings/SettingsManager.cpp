#include <QSettings>

#include "SettingsManager.h"
#include "SettingsDialog.h"

SettingsManager *SettingsManager::m_instance(0);

////////////////////////////////////////////////////////////////////////////////////////////
/// QISettings
void ISettings::readSettings( QSettings &settings )
{
    if ( !valuesMap.size() )
        return;

    if ( groupName() == QLatin1String("") )
        return;

    settings.beginGroup(groupName());
    QMutableMapIterator <QString, QVariant> i(valuesMap);
    while ( i.hasNext() ) {
        i.next();
        valuesMap[i.key()] = settings.value(i.key(), i.value());
    }
    settings.endGroup();
}

void ISettings::writeSettings( QSettings &settings )
{
    if ( !valuesMap.size() )
        return;

    if ( groupName() == QLatin1String("") )
        return;

    settings.beginGroup(groupName());
    QMutableMapIterator <QString, QVariant> i(valuesMap);
    while ( i.hasNext() ) {
        i.next();
        settings.setValue(i.key(), i.value());
    }
    settings.endGroup();
}

void ISettings::applySettings( ISettingsTabPage *page )
{
    if ( page )
        page->fillSettings(valuesMap);

    doApplySettings();
}

////////////////////////////////////////////////////////////////////////////////////////////
/// QSettingsManager
SettingsManager::SettingsManager( QObject *parent )
    : QObject(parent)
{
    m_instance = this;
}

SettingsManager::~SettingsManager()
{
    qDeleteAll(settingsMap);
    settingsMap.clear();
}

SettingsManager* SettingsManager::instance()
{
    return m_instance;
}

void SettingsManager::registerSettingsClient(ISettingsClient *client)
{
    m_instance->m_clients.push_back(client);
}

bool SettingsManager::hasKey( const QString &settingsKey )
{
    return (settingsMap.find(settingsKey) != settingsMap.end());
}

void SettingsManager::addSettings( const QString &settingsKey, ISettings *settings )
{
    settingsMap[settingsKey] = settings;
}

void SettingsManager::readSettings( QSettings &settings )
{
    foreach(ISettings *iSettings, settingsMap ) {
        iSettings->readSettings(settings);
        iSettings->applySettings();
    }
}

void SettingsManager::writeSetings( QSettings &settings )
{
    foreach(ISettings *iSettings, settingsMap)
        iSettings->writeSettings(settings);
}

void SettingsManager::exec( QWidget *parent )
{
    SettingsDialog settingsDialog(&settingsMap, parent);
    if ( settingsDialog.exec() == QDialog::Accepted ) {
        foreach (ISettingsClient *client, m_clients)
            client->onSettingsChanged();
    }
}
