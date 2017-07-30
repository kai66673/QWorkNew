#include "ISettingsFactory.h"

void ISettingsFactory::registerSettings() const
{
    QStringList settingsKeys = settingsKeyList();
    foreach( const QString &settingsKey, settingsKeys ) {
        SettingsManager *settingsManager = SettingsManager::instance();
        if ( !settingsManager->hasKey(settingsKey) ) {
            ISettings *settings = createSettings(settingsKey);
            if ( settings )
                settingsManager->addSettings(settingsKey, settings);
        }
    }
}
