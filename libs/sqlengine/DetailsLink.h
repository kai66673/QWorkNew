#ifndef DETAILSLINK_H
#define DETAILSLINK_H

#include "ILink.h"
#include "DbConnection.h"

#include "sqlengine_global.h"

namespace Sql {

class SQL_EXPORT DetailsLink : public Core::ILink
{
public:
    DetailsLink( int begin, int end, Database::DbConnection *connection, Database::IDbCatalogItem *item );

    virtual bool open() const;

private:
    Database::DbConnection *m_connection;
    Database::IDbCatalogItem *m_item;
};

}   // namespace Sql

#endif // DETAILSLINK_H
