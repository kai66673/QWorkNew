#ifndef CLOSEAPPLICATIONHANDLER_H
#define CLOSEAPPLICATIONHANDLER_H

#include <QList>

#include "corelib_global.h"

class CORELIB_EXPORT ICloseApplicationClient
{
public:
    ICloseApplicationClient();

    virtual bool applicationAboutToClose() = 0;
    virtual void applicationClose() = 0;
    virtual void destroy() {}
};

class CORELIB_EXPORT CloseApplicationHandler
{
public:
    CloseApplicationHandler();

    static CloseApplicationHandler *instance();

    void registerCloseReflectorClient( ICloseApplicationClient *client );

    bool applicationAboutToClose();
    void applicationClose();
    void destroyClients();

private:
    QList <ICloseApplicationClient *> m_clients;
    static CloseApplicationHandler *m_instance;
};

#endif // CLOSEAPPLICATIONHANDLER_H
