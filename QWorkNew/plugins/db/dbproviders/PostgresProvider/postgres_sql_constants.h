#ifndef POSTGRES_SQL_CONSTANTS_H
#define POSTGRES_SQL_CONSTANTS_H

#include "sql_constants.h"

namespace Sql {

namespace Constants {

namespace DbDatabaseObjectType {

const int C_TABLESPACE          = C_LAST_BASE_TYPE + 1;
const int C_GROUP_ROLE          = C_LAST_BASE_TYPE + 2;
const int C_USER                = C_LAST_BASE_TYPE + 3;

} // DbDatabaseObjectType

namespace DbSchemaObjectType {

const int C_FUNCTION            = C_LAST_BASE_TYPE + 1;
const int C_TRIGGER_FUNCTION    = C_LAST_BASE_TYPE + 2;
const int C_SEQUENCE            = C_LAST_BASE_TYPE + 3;
const int C_TYPE                = C_LAST_BASE_TYPE + 4;

} // DbSchemaObjectType

} // Constants

} // Sql

#endif // POSTGRES_SQL_CONSTANTS_H
