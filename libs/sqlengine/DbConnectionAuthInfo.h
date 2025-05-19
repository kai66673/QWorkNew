#ifndef DBCONNECTIONAUTHINFO_H
#define DBCONNECTIONAUTHINFO_H

#include <QString>
#include <QStringList>

#include "sqlengine_global.h"

struct SQL_EXPORT DbConnectionAuthInfo {
    DbConnectionAuthInfo(const QString &dbType = "", const QStringList &pars = QStringList(), const QString &connTitile_ = "")
        : databaseType(dbType)
        , description("")
        , parameters(pars)
        , connTitle(connTitile_)
    { }

    bool isEqual(const DbConnectionAuthInfo &other);

    QString     databaseType;
    QString     description;
    QStringList parameters;
    QString     connTitle;
};

#endif // DBCONNECTIONAUTHINFO_H
