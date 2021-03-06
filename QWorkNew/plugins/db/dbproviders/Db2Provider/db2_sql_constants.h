#ifndef DB2_SQL_CONSTANTS_H
#define DB2_SQL_CONSTANTS_H

#include "sql_constants.h"

namespace Sql {

namespace Constants {

namespace DbSchemaObjectType {

const int C_MODULE         	= C_LAST_BASE_TYPE + 1;
const int C_FUNCTION        = C_LAST_BASE_TYPE + 2;
const int C_PROCEDURE       = C_LAST_BASE_TYPE + 3;

} // DbSchemaObjectType

namespace DbSchemaObjectSubtype {

const int C_MODULE_FUNCTION     = 0;
const int C_MODULE_PROCEDURE    = 1;

} // DbSchemaObjectSubtype

} // Constants

} // Sql

#endif // DB2_SQL_CONSTANTS_H
