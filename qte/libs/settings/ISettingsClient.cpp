#include "ISettingsClient.h"
#include "SettingsManager.h"

ISettingsClient::ISettingsClient()
{
    SettingsManager::registerSettingsClient(this);
}

