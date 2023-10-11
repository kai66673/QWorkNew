#ifndef ISESSIONCLIENT_H
#define ISESSIONCLIENT_H

#include "corelib_global.h"

class CORELIB_EXPORT ISessionClient
{
public:
    ISessionClient();

    virtual bool sessionAboutToClose() = 0;
    virtual void sessionClose() = 0;

    virtual void saveSession( const QString &location ) = 0;
    virtual void loadSession( const QString &location ) = 0;
};

#endif // ISESSIONCLIENT_H
