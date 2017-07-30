#ifndef SQL_CONSTANTS_H
#define SQL_CONSTANTS_H

namespace Database {

enum ExecuteType {
    AllDocument = 0,
    DocumentSelectedText
};

}   // namespace Database

namespace Sql {
namespace Constants {

const char * const C_SQL_EDITOR_SETTINGS_ID         = "002_SqlEditor";

//namespace DbRequestType {

//const unsigned C_SCHEMAS            = 0;
//const unsigned C_TABLES             = 1;
//const unsigned C_VIEWS              = 2;
//const unsigned C_TABLES_AND_VIEWS   = 3;
//const unsigned C_INDEXES            = 4;
//const unsigned C_TRIGGERS           = 5;
//const unsigned C_CONSTRAINTS        = 6;
//const unsigned C_TABLE_COLUMNS      = 7;
//const unsigned C_TABLE_INDEXES      = 8;
//const unsigned C_TABLE_TRIGGERS     = 9;
//const unsigned C_TABLE_CONSTRAINTS  = 10;
//const unsigned C_VIEW_COLUMNS       = 11;
//const unsigned C_VIEW_SOURCE        = 12;

//const unsigned C_INDEX_TABLE_NAME   = 0;
//const unsigned C_TRIGGER_TABLE_NAME = 1;
//const unsigned C_CONSTR_TABLE_NAME  = 2;

//}   // DbRequestType

namespace DbDatabaseObjectType {

const int C_UNDEFINED           = -1;
const int C_SCHEMA              = 0;
const int C_LAST_BASE_TYPE      = 0;

} // SbDatabaseObjectType

namespace DbSchemaObjectType {

const int C_UNDEFINED           = -1;
const int C_TABLE               = 0;
const int C_VIEW                = 1;
const int C_INDEX               = 2;
const int C_TRIGGER             = 3;
const int C_CONSTRAINT          = 4;
const int C_LAST_BASE_TYPE      = 4;

} // DbSchemaObjectType

namespace DbSchemaObjectSubtype {

const int C_TABLE               = 0;
const int C_COLUMN              = 1;
const int C_INDEX               = 2;
const int C_TRIGGER             = 3;
const int C_CONSTRAINT          = 4;

const int C_UNDEFINED           = -1;
const int C_TABLE_COLUMN        = 0;
const int C_TABLE_INDEX         = 1;
const int C_TABLE_TRIGGER       = 2;
const int C_TABLE_CONSTRAINT    = 3;

const int C_VIEW_COLUMN         = 0;

}   // DbSchemaObjectSubtype

namespace DbItemAction {

const int C_FIRST_ACTION        = 0;
const int C_DETAILS_ACTION      = 0;
const int C_COPY_NAME_ACTION    = 1;
const int C_QUERY_DATA_ACTION   = 2;
const int C_LAST_ACTION         = 2;

}   // DbItemAction

}   // Sql
}   // Constants

#endif // SQL_CONSTANTS_H
