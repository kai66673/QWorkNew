#ifndef ISETTINGSCLIENT_H
#define ISETTINGSCLIENT_H

#include "settings_global.h"

class SETTINGSSHARED_EXPORT ISettingsClient
{
public:
    ISettingsClient();
    virtual void onSettingsChanged() = 0;
};

#endif // ISETTINGSCLIENT_H
