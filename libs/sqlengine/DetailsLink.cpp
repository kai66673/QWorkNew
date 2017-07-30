#include "DetailsLink.h"

namespace Sql {

DetailsLink::DetailsLink( int begin, int end, Database::DbConnection *connection, Database::IDbCatalogItem *item )
    : Core::ILink(begin, end)
    , m_connection(connection)
    , m_item(item)
{
}

bool DetailsLink::open() const
{
    m_connection->requestDetails(m_item);
    return true;
}

}   // namespace Sql
