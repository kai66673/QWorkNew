#include "ISessionClient.h"
#include "SessionManager.h"

ISessionClient::ISessionClient()
{
    SessionManager::getInstance()->registerSessionClient(this);
}

