#include "CloseApplicationHandler.h"

CloseApplicationHandler *CloseApplicationHandler::m_instance = 0;

ICloseApplicationClient::ICloseApplicationClient()
{
    CloseApplicationHandler::instance()->registerCloseReflectorClient(this);
}

CloseApplicationHandler::CloseApplicationHandler()
{
    m_instance = this;
}

CloseApplicationHandler *CloseApplicationHandler::instance()
{
    return m_instance;
}

void CloseApplicationHandler::registerCloseReflectorClient( ICloseApplicationClient *client )
{
    m_clients.append(client);
}

bool CloseApplicationHandler::applicationAboutToClose()
{
    for (int i = 0; i < m_clients.size(); i++)
        if ( !m_clients[i]->applicationAboutToClose() )
            return false;

    return true;
}

void CloseApplicationHandler::applicationClose()
{
    for (int i = 0; i < m_clients.size(); i++)
        m_clients[i]->applicationClose();
}

void CloseApplicationHandler::destroyClients()
{
    for (int i = 0; i < m_clients.size(); i++)
        m_clients[i]->destroy();
}
