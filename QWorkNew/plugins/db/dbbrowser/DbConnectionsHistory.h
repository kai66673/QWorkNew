#ifndef DBCONNECTIONSHISTORY_H
#define DBCONNECTIONSHISTORY_H

#include <QStringList>

#include "DbConnectionAuthInfo.h"

class DbConnectionsHistory
{
public:
    DbConnectionsHistory();
    virtual ~DbConnectionsHistory();

    bool hasConnections() const { return (connections.size() > 0); }
    void addConnection( const DbConnectionAuthInfo &connection );

    bool selectOrEditConnection( DbConnectionAuthInfo &connection );

private:
    void load();
    void save();

    QList<DbConnectionAuthInfo> connections;
};

#endif // DBCONNECTIONSHISTORY_H
