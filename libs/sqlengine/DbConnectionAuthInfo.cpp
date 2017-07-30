#include "DbConnectionAuthInfo.h"

bool DbConnectionAuthInfo::isEqual( const DbConnectionAuthInfo &other )
{
    if ( databaseType != other.databaseType )
        return false;

    if ( parameters.size() != other.parameters.size() )
        return false;

    for (int i = 0; i < parameters.size(); i++) {
        if ( parameters[i] != other.parameters[i] )
            return false;
    }

    return true;
}
