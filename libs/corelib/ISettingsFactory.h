#ifndef ISETTINGSFACTORY_H
#define ISETTINGSFACTORY_H

#include <QStringList>

#include "corelib_global.h"
#include "SettingsManager.h"

class CORELIB_EXPORT ISettingsFactory
{
protected:
    virtual QStringList settingsKeyList() const = 0;
    virtual ISettings *createSettings( const QString &settingsKey ) const = 0;

public:
    void registerSettings() const;
};

#endif // ISETTINGSFACTORY_H
