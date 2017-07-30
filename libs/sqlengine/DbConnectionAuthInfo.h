#ifndef DBCONNECTIONAUTHINFO_H
#define DBCONNECTIONAUTHINFO_H

#include <QString>
#include <QStringList>

#include "sqlengine_global.h"

struct SQL_EXPORT DbConnectionAuthInfo {
    DbConnectionAuthInfo( const QString &dbType = "", const QStringList &pars = QStringList() )
        : databaseType(dbType)
        , description("")
        , parameters(pars)
    { }

    bool isEqual(const DbConnectionAuthInfo &other);

    QString     databaseType;
    QString     description;
    QStringList parameters;
};

#endif // DBCONNECTIONAUTHINFO_H
