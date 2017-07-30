#ifndef SQLEDITORCONSTANTS_H
#define SQLEDITORCONSTANTS_H

namespace Sql {

namespace Constants {

    const char * const ACTION_ID = "SqlEditor.Action";
    const char * const MENU_ID = "SqlEditor.Menu";

    const char * const C_SQLEDITOR    = "SqlPlugin.SQL-Editor";
    const char * const SQLEDITOR_ID   = "SqlPlugin.SQL-Editor";
    const char * const SQL_MIMETYPE   = "text/sql";
    const char * const SQLEDITOR_DISPLAY_NAME = QT_TRANSLATE_NOOP("OpenWith::Editors", "SQL Editor");

    const char * const SQL_CATEGORY         = "SQL.SQL";
    const char * const SQL_TR_CATEGORY      = QT_TRANSLATE_NOOP("SQL", "SQL");
    const char * const SQL_CATEGORY_ICON    = ":/sqleditor/images/sql_settings.png";

    const char * const C_SQL_TABLE_FORMAT   = "table";
    const char * const C_SQL_ALIAS_FORMAT   = "alias";
    const char * const C_SQL_COLUMN_FORMAT  = "column";
    const char * const C_SQL_INDEX_FORMAT   = "index";
    const char * const C_SQL_TRIGGER_FORMAT = "trigger";
    const char * const C_SQL_BIND_FORMAT    = "bind";
    const char * const C_SQL_IDENT_FORMAT   = "identifier";
    const char * const C_SQL_LABEL_FORMAT   = "label";
    const char * const C_SQL_FUNC_FORMAT    = "function";
    const char * const C_SQL_CURSOR_FORMAT  = "cursor";
    const char * const C_SQL_MAP_FORMAT     = "map";
    const char * const C_SQL_AREA_FORMAT    = "area";
    const char * const C_SQL_MODULE_FORMAT  = "module";

} // namespace Constants
} // namespace Sql

#endif // SQLEDITORCONSTANTS_H

