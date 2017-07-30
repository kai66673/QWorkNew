#ifndef ORACLE_SQL_CONSTANTS_H
#define ORACLE_SQL_CONSTANTS_H

#include "sql_constants.h"

namespace Sql {

namespace Constants {

namespace DbSchemaObjectType {

const int C_PACKAGE         = C_LAST_BASE_TYPE + 1;
const int C_PACKAGE_BODY    = C_LAST_BASE_TYPE + 2;
const int C_FUNCTION        = C_LAST_BASE_TYPE + 3;
const int C_PROCEDURE       = C_LAST_BASE_TYPE + 4;

} // DbSchemaObjectType

namespace DbSchemaObjectSubtype {

const int C_PACKAGE_FUNCTION    = 0;
const int C_PACKAGE_PROCEDURE   = 1;

}   // DbSchemaObjectSubtype

} // Constants

} // Sql

#endif // ORACLE_SQL_CONSTANTS_H
