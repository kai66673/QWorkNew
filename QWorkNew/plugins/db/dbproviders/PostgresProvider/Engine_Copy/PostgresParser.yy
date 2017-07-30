%skeleton "lalr1.cc"
%require  "3.0"
%debug
%defines
%define api.namespace {Postgres}
%define parser_class_name {PostgresParser}

%code requires{
  #include "PostgresASTfwd.h"

   namespace Postgres {
      class PostgresDriver;
      class PostgresScanner;
   }
}

%parse-param { PostgresScanner  &scanner  }
%parse-param { PostgresDriver  &driver  }

%code {
   #include <iostream>
   #include <cstdlib>
   #include <fstream>

   /* include for all driver functions */
   #include "PostgresDriver.h"
   #include "PostgresAST.h"

#undef yylex
#define yylex scanner.yylex
}

/* token types */
%union {
   int location;
   AST *astNode;
   ValueExpressionListAST       *exprList;
   BaseSelectColumnListAST      *colList;
   TableReferenceListAST        *tableRefList;
   OrderingTermListAST          *orderTermList;
   ColumnIdentifierListAST      *colIdentList;
   CaseExpressionClauseListAST  *caseExprList;
   CaseConditionClauseListAST   *caseCondList;
   AssignmentExpressionListAST  *updateSetList;
   ColumnConstraintListAST      *colConstrList;
   NameListAST                  *nameList;
}

%token T_EOF    0     "end of file"

/* Common Tokens */
%token <location> T_IDENTIFIER   "identifier"
%token <location> T_DIGIT       "number constant"
%token <location> T_CHARSTR      "quoted string"
%token <location> T_STRSTR       "double quoted string"

/* Punctuation Tokens */
%token <location> T_AMPER "&"
%token <location> T_CARET "^"
%token <location> T_COLON ":"
%token <location> T_ASSIGN ":="
%token <location> T_COMMA ","
%token <location> T_SLASH "/"
%token <location> T_DOT "."
%token <location> T_DOTDOT ".."
%token <location> T_EQUAL "="
%token <location> T_EXCLAIM "!"
%token <location> T_EXCLAIM_EQUAL "!="
%token <location> T_GREATER ">"
%token <location> T_GREATER_EQUAL ">="
%token <location> T_LEFT_BRACE "{"
%token <location> T_LEFT_BRACKET "["
%token <location> T_LESS "<"
%token <location> T_LESS_EQUAL "<="
%token <location> T_LESS_GREATER "<>"
%token <location> T_LEFT_PAREN "("
%token <location> T_DASH "-"
%token <location> T_PERCENT "%"
%token <location> T_PIPE "|"
%token <location> T_PIPE_PIPE "||"
%token <location> T_PLUS "+"
%token <location> T_RIGHT_BRACE "}"
%token <location> T_RIGHT_BRACKET "]"
%token <location> T_RIGHT_PAREN ")"
%token <location> T_SEMICOLON ";"
%token <location> T_STAR "*"

/* Keywords */
%token <location> T_AS "as"
%token <location> T_AT "at"
%token <location> T_BY "by"
%token <location> T_DO "do"
%token <location> T_GO "go"
%token <location> T_IF "if"
%token <location> T_IN "in"
%token <location> T_IS "is"
%token <location> T_NO "no"
%token <location> T_OF "of"
%token <location> T_ON "on"
%token <location> T_OR "or"
%token <location> T_TO "to"
%token <location> T_ADD "add"
%token <location> T_ALL "all"
%token <location> T_AND "and"
%token <location> T_ANY "any"
%token <location> T_ARE "are"
%token <location> T_ASC "asc"
%token <location> T_DEC "dec"
%token <location> T_END "end"
%token <location> T_FOR "for"
%token <location> T_GET "get"
%token <location> T_KEY "key"
%token <location> T_MAP "map"
%token <location> T_NEW "new"
%token <location> T_NOT "not"
%token <location> T_OLD "old"
%token <location> T_OUT "out"
%token <location> T_REF "ref"
%token <location> T_ROW "row"
%token <location> T_SET "set"
%token <location> T_SQL "sql"
%token <location> T_BOTH "both"
%token <location> T_CALL "call"
%token <location> T_CASE "case"
%token <location> T_CUBE "cube"
%token <location> T_DATA "data"
%token <location> T_DESC "desc"
%token <location> T_DROP "drop"
%token <location> T_EACH "each"
%token <location> T_ELSE "else"
%token <location> T_EXEC "exec"
%token <location> T_EXIT "exit"
%token <location> T_FREE "free"
%token <location> T_FROM "from"
%token <location> T_FULL "full"
%token <location> T_GOTO "goto"
%token <location> T_HOLD "hold"
%token <location> T_INTO "into"
%token <location> T_JOIN "join"
%token <location> T_LEFT "left"
%token <location> T_LIKE "like"
%token <location> T_LOOP "loop"
%token <location> T_NEXT "next"
%token <location> T_NONE "none"
%token <location> T_NULL "null"
%token <location> T_ONLY "only"
%token <location> T_OPEN "open"
%token <location> T_OVER "over"
%token <location> T_READ "read"
%token <location> T_ROLE "role"
%token <location> T_ROWS "rows"
%token <location> T_SETS "sets"
%token <location> T_SOME "some"
%token <location> T_THEN "then"
%token <location> T_TRUE "true"
%token <location> T_UNDO "undo"
%token <location> T_USER "user"
%token <location> T_VIEW "view"
%token <location> T_WHEN "when"
%token <location> T_WITH "with"
%token <location> T_WORK "work"
%token <location> T_AFTER "after"
%token <location> T_ALTER "alter"
%token <location> T_ARRAY "array"
%token <location> T_BEGIN "begin"
%token <location> T_CHECK "check"
%token <location> T_CLOSE "close"
%token <location> T_CROSS "cross"
%token <location> T_CYCLE "cycle"
%token <location> T_DEPTH "depth"
%token <location> T_DEREF "deref"
%token <location> T_FALSE "false"
%token <location> T_FETCH "fetch"
%token <location> T_GRANT "grant"
%token <location> T_GROUP "group"
%token <location> T_INDEX "index"
%token <location> T_INNER "inner"
%token <location> T_INOUT "inout"
%token <location> T_INPUT "input"
%token <location> T_LARGE "large"
%token <location> T_LEAVE "leave"
%token <location> T_LEVEL "level"
%token <location> T_LIMIT "limit"
%token <location> T_LOCAL "local"
%token <location> T_MATCH "match"
%token <location> T_MINUS "minus"
%token <location> T_NAMES "names"
%token <location> T_ORDER "order"
%token <location> T_OUTER "outer"
%token <location> T_PRIOR "prior"
%token <location> T_RANGE "range"
%token <location> T_READS "reads"
%token <location> T_RIGHT "right"
%token <location> T_SCOPE "scope"
%token <location> T_SPACE "space"
%token <location> T_START "start"
%token <location> T_STATE "state"
%token <location> T_TABLE "table"
%token <location> T_TREAT "treat"
%token <location> T_UNDER "under"
%token <location> T_UNION "union"
%token <location> T_UNTIL "until"
%token <location> T_USAGE "usage"
%token <location> T_USING "using"
%token <location> T_VALUE "value"
%token <location> T_WHERE "where"
%token <location> T_WHILE "while"
%token <location> T_WRITE "write"
%token <location> T_ACTION "action"
%token <location> T_ATOMIC "atomic"
%token <location> T_BEFORE "before"
%token <location> T_COLUMN "column"
%token <location> T_COMMIT "commit"
%token <location> T_CREATE "create"
%token <location> T_CURSOR "cursor"
%token <location> T_DELETE "delete"
%token <location> T_DOMAIN "domain"
%token <location> T_ELSEIF "elseif"
%token <location> T_ENABLE "enable"
%token <location> T_EQUALS "equals"
%token <location> T_ESCAPE "escape"
%token <location> T_EXCEPT "except"
%token <location> T_EXISTS "exists"
%token <location> T_FILTER "filter"
%token <location> T_GLOBAL "global"
%token <location> T_HAVING "having"
%token <location> T_INSERT "insert"
%token <location> T_METHOD "method"
%token <location> T_MODULE "module"
%token <location> T_NOWAIT "nowait"
%token <location> T_NUMBER "number"
%token <location> T_OPTION "option"
%token <location> T_OUTPUT "output"
%token <location> T_RENAME "rename"
%token <location> T_REPEAT "repeat"
%token <location> T_RESULT "result"
%token <location> T_RETURN "return"
%token <location> T_REVOKE "revoke"
%token <location> T_ROLLUP "rollup"
%token <location> T_SCHEMA "schema"
%token <location> T_SCROLL "scroll"
%token <location> T_SEARCH "search"
%token <location> T_SELECT "select"
%token <location> T_SIGNAL "signal"
%token <location> T_SIMPLE "simple"
%token <location> T_STATIC "static"
%token <location> T_SYSTEM "system"
%token <location> T_UNIQUE "unique"
%token <location> T_UNNEST "unnest"
%token <location> T_UPDATE "update"
%token <location> T_VALUES "values"
%token <location> T_WINDOW "window"
%token <location> T_WITHIN "within"
%token <location> T_BETWEEN "between"
%token <location> T_BREADTH "breadth"
%token <location> T_CASCADE "cascade"
%token <location> T_CATALOG "catalog"
%token <location> T_COLLATE "collate"
%token <location> T_COMMENT "comment"
%token <location> T_CONNECT "connect"
%token <location> T_CURRENT "current"
%token <location> T_DECLARE "declare"
%token <location> T_DEFAULT "default"
%token <location> T_DISABLE "disable"
%token <location> T_DYNAMIC "dynamic"
%token <location> T_EXECUTE "execute"
%token <location> T_FOREIGN "foreign"
%token <location> T_GENERAL "general"
%token <location> T_HANDLER "handler"
%token <location> T_ITERATE "iterate"
%token <location> T_LATERAL "lateral"
%token <location> T_LEADING "leading"
%token <location> T_LOCATOR "locator"
%token <location> T_NATURAL "natural"
%token <location> T_PARTIAL "partial"
%token <location> T_PREPARE "prepare"
%token <location> T_PRIMARY "primary"
%token <location> T_RELEASE "release"
%token <location> T_REPLACE "replace"
%token <location> T_RETURNS "returns"
%token <location> T_ROUTINE "routine"
%token <location> T_SECTION "section"
%token <location> T_SESSION "session"
%token <location> T_SIMILAR "similar"
%token <location> T_TRIGGER "trigger"
%token <location> T_UNKNOWN "unknown"
%token <location> T_WITHOUT "without"
%token <location> T_ABSOLUTE "absolute"
%token <location> T_ALLOCATE "allocate"
%token <location> T_CASCADED "cascaded"
%token <location> T_CONTINUE "continue"
%token <location> T_DEFERRED "deferred"
%token <location> T_DESCRIBE "describe"
%token <location> T_DISTINCT "distinct"
%token <location> T_EXTERNAL "external"
%token <location> T_FUNCTION "function"
%token <location> T_GROUPING "grouping"
%token <location> T_IDENTITY "identity"
%token <location> T_INTERVAL "interval"
%token <location> T_LANGUAGE "language"
%token <location> T_MODIFIES "modifies"
%token <location> T_OVERLAPS "overlaps"
%token <location> T_PRESERVE "preserve"
%token <location> T_RELATIVE "relative"
%token <location> T_RESIGNAL "resignal"
%token <location> T_RESTRICT "restrict"
%token <location> T_ROLLBACK "rollback"
%token <location> T_SPECIFIC "specific"
%token <location> T_SQLSTATE "sqlstate"
%token <location> T_TRAILING "trailing"
%token <location> T_TRUNCATE "truncate"
%token <location> T_WHENEVER "whenever"
%token <location> T_ASSERTION "assertion"
%token <location> T_COLLATION "collation"
%token <location> T_CONDITION "condition"
%token <location> T_EXCEPTION "exception"
%token <location> T_IMMEDIATE "immediate"
%token <location> T_INDICATOR "indicator"
%token <location> T_INITIALLY "initially"
%token <location> T_INTERSECT "intersect"
%token <location> T_ISOLATION "isolation"
%token <location> T_LOCALTIME "localtime"
%token <location> T_PARAMETER "parameter"
%token <location> T_PARTITION "partition"
%token <location> T_PRECISION "precision"
%token <location> T_PROCEDURE "procedure"
%token <location> T_RECURSIVE "recursive"
%token <location> T_SAVEPOINT "savepoint"
%token <location> T_SENSITIVE "sensitive"
%token <location> T_STATEMENT "statement"
%token <location> T_SYMMETRIC "symmetric"
%token <location> T_TEMPORARY "temporary"
%token <location> T_ASENSITIVE "asensitive"
%token <location> T_ASYMMETRIC "asymmetric"
%token <location> T_CONNECTION "connection"
%token <location> T_CONSTRAINT "constraint"
%token <location> T_DEALLOCATE "deallocate"
%token <location> T_DEFERRABLE "deferrable"
%token <location> T_DESCRIPTOR "descriptor"
%token <location> T_DISCONNECT "disconnect"
%token <location> T_ORDINALITY "ordinality"
%token <location> T_PRIVILEGES "privileges"
%token <location> T_REFERENCES "references"
%token <location> T_SQLWARNING "sqlwarning"
%token <location> T_CONSTRAINTS "constraints"
%token <location> T_CONSTRUCTOR "constructor"
%token <location> T_DIAGNOSTICS "diagnostics"
%token <location> T_INSENSITIVE "insensitive"
%token <location> T_REFERENCING "referencing"
%token <location> T_SYSTEM_USER "system_user"
%token <location> T_TRANSACTION "transaction"
%token <location> T_TRANSLATION "translation"
%token <location> T_CURRENT_DATE "current_date"
%token <location> T_CURRENT_PATH "current_path"
%token <location> T_CURRENT_ROLE "current_role"
%token <location> T_CURRENT_TIME "current_time"
%token <location> T_CURRENT_USER "current_user"
%token <location> T_SESSION_USER "session_user"
%token <location> T_SPECIFICTYPE "specifictype"
%token <location> T_SQLEXCEPTION "sqlexception"
%token <location> T_AUTHORIZATION "authorization"
%token <location> T_CORRESPONDING "corresponding"
%token <location> T_DETERMINISTIC "deterministic"
%token <location> T_TIMEZONE_HOUR "timezone_hour"
%token <location> T_LOCALTIMESTAMP "localtimestamp"
%token <location> T_TIMEZONE_MINUTE "timezone_minute"
%token <location> T_CURRENT_TIMESTAMP "current_timestamp"
/* Builtin Functions */
%token <location> T_DAY "day"
%token <location> T_MAX "max"
%token <location> T_MIN "min"
%token <location> T_PAD "pad"
%token <location> T_SUM "sum"
%token <location> T_CAST "cast"
%token <location> T_HOUR "hour"
%token <location> T_LAST "last"
%token <location> T_SIZE "size"
%token <location> T_COUNT "count"
%token <location> T_FIRST "first"
%token <location> T_FOUND "found"
%token <location> T_MONTH "month"
%token <location> T_MINUTE "minute"
%token <location> T_SECOND "second"
%token <location> T_SUBSTRING "substring"
/* Builtin Types */
%token <location> T_BIGINT "bigint"
%token <location> T_BIGSERIAL "bigserial"
%token <location> T_BIT "bit"
%token <location> T_VARBIT "varbit"
%token <location> T_BOX "box"
%token <location> T_BYTEA "bytea"
%token <location> T_CIDR "cidr"
%token <location> T_CIRCLE "circle"
%token <location> T_INET "inet"
%token <location> T_JSON "json"
%token <location> T_LINE "line"
%token <location> T_LSEG "lseg"
%token <location> T_MACADDR "macaddr"
%token <location> T_MONEY "money"
%token <location> T_PATH "path"
%token <location> T_POINT "point"
%token <location> T_POLYGON "polygon"
%token <location> T_REAL "real"
%token <location> T_SMALLSERIAL "smallserial"
%token <location> T_SERIAL "serial"
%token <location> T_TEXT "text"
%token <location> T_TIME "time"
%token <location> T_TIMESTAMP "timestamp"
%token <location> T_TSQUERY "tsquery"
%token <location> T_TSVECTOR "tsvector"
%token <location> T_TXID_SNAPSHOT "txid_snapshot"
%token <location> T_UUID "uuid"
%token <location> T_XML "xml"

%token <location> T_INT2 "int2"
%token <location> T_INT4 "int4"
%token <location> T_INT8 "int8"
%token <location> T_SERIAL2 "serial2"
%token <location> T_SERIAL4 "serial4"
%token <location> T_SERIAL8 "serial8"
%token <location> T_BOOL "bool"
%token <location> T_FLOAT8 "float8"
%token <location> T_FLOAT4 "float4"
%token <location> T_TIMETZ "timetz"
%token <location> T_TIMESTAMPTZ "timestamptz"

%token <location> T_INT "int"
%token <location> T_CHAR "char"
%token <location> T_DATE "date"
%token <location> T_YEAR "year"
%token <location> T_ZONE "zone"
%token <location> T_FLOAT "float"
%token <location> T_DOUBLE "double"
%token <location> T_OBJECT "object"
%token <location> T_BOOLEAN "boolean"
%token <location> T_DECIMAL "decimal"
%token <location> T_INTEGER "integer"
%token <location> T_NUMERIC "numeric"
%token <location> T_VARCHAR "varchar"
%token <location> T_VARYING "varying"
%token <location> T_SMALLINT "smallint"
%token <location> T_CHARACTER "character"

%type <astNode>       statement top_select_statement sel_statement commit_statement rollback_statement empty_aka_optional
                      value_expression binary_expression unary_expression primary_expression in_expression
                      value_expression_set type_spec func_ident table_identifier column_identifier
                      subquery_table_reference conditional_expression conditional_term conditional_primary
                      between_expression null_comparison_expression exists_expression select_core_item
                      select_columns_item join_spec join_condition where_clause group_clause set_clause
                      table_reference joined_table direct_table_reference order_clause ordered_expression
                      update_clause case_expression case_condition case_expression_item case_condition_item
                      delete_statement update_statement update_set_clause update_set_item referrences_clause
                      references_match_clause references_action_clause insert_statement insert_columns_clause
                      insert_values_clause drop_statement index_identifier trigger_identifier truncate_statement
                      comment_statement column_constraint_item deferrable_cluase constraint_decl_optional
                      alter_table_statement if_exists_clause alter_table_action trigger_wildcard column_definition
                      collate_clause

%type <exprList>      comma_expression_list
%type <colList>       select_columns
%type <tableRefList>  table_reference_list
%type <orderTermList> comma_ordered_expression_list
%type <colIdentList>  comma_column_identifier_list
%type <caseExprList>  case_expression_list
%type <caseCondList>  case_condition_list
%type <updateSetList> update_set_list
%type <colConstrList> column_constraint_list
%type <nameList>      comma_name_list

%type <location>      semicolon_or_eof binary_op unary_op builtin_function_name count_spec char_type_name
                      simple_type_name not_optional delete_or_update numeric_or_decimal
                      binary_cond_op compare_op select_spec order_definition nowait_clause only_optional
                      restrict_or_cascade_optional column_optional enable_or_disable

%start start

%%

start
    : statement_list T_EOF
    ;

semicolon_or_eof
    : T_SEMICOLON { $$ = $1; }
    | T_EOF       { $$ = 0; }
    ;

statement_list
    : statement
    | statement_list statement
    ;

statement
    : top_select_statement    { driver.addStatement( $1->asStatement() ); }
    | delete_statement        { driver.addStatement( $1->asStatement() ); }
    | update_statement        { driver.addStatement( $1->asStatement() ); }
    | insert_statement        { driver.addStatement( $1->asStatement() ); }
    | drop_statement          { driver.addStatement( $1->asStatement() ); }
    | truncate_statement      { driver.addStatement( $1->asStatement() ); }
    | alter_table_statement   { driver.addStatement( $1->asStatement() ); }
    | comment_statement       { driver.addStatement( $1->asStatement() ); }
    | commit_statement        { driver.addStatement( $1->asStatement() ); }
    | rollback_statement      { driver.addStatement( $1->asStatement() ); }
    /* And Many Other Statements... */
    /* ............................ */
    /* And Many Other Statements... */
    /* Unknown Statement (aka Error) */
    | unknown_statement       { driver.addUnknownStatement(); }
    ;

if_exists_clause
    : empty_aka_optional
    | T_IF T_EXISTS
      { $$ = driver.makeAstNode<IfExistsAST>( $1, $2 ); }
    ;

only_optional
    : /* empty */ { $$ = 0; }
    | T_ONLY
    ;

restrict_or_cascade_optional
    : /* empty */ { $$ = 0; }
    | T_RESTRICT
    | T_CASCADE
    ;

column_optional
    : /* empty */ { $$ = 0; }
    | T_COLUMN
    ;

enable_or_disable
    : T_ENABLE
    | T_DISABLE
    ;

trigger_wildcard
    : empty_aka_optional
    | T_ALL         { $$ = driver.makeAstNode<AllOrUserTriggersAST>( $1 ); }
    | T_USER        { $$ = driver.makeAstNode<AllOrUserTriggersAST>( $1 ); }
    | T_IDENTIFIER  { $$ = driver.makeAstNode<ConcreteTriggerAST>( driver.createName(NameAST::Trigger, $1) ); }
    ;

collate_clause
    : empty_aka_optional
    | T_COLLATE T_STRSTR
      {
        $$ = driver.makeAstNode<CollateClauseAST>( $1,
                                                   driver.makeAstNode<LiteralExpressionAST>($2) );
      }
    ;

not_optional
    : /* empty */ { $$ = 0; }
    | T_NOT
    ;

deferrable_cluase
    : empty_aka_optional
    | not_optional T_DEFERRABLE T_INITIALLY T_DEFERRED   { $$ = driver.makeAstNode<DeferrableClauseAST>( $1, $2, $3, $4 ); }
    | not_optional T_DEFERRABLE T_INITIALLY T_IMMEDIATE  { $$ = driver.makeAstNode<DeferrableClauseAST>( $1, $2, $3, $4 ); }
    | not_optional T_DEFERRABLE                          { $$ = driver.makeAstNode<DeferrableClauseAST>( $1, $2,  0,  0 ); }
    | T_INITIALLY T_DEFERRED                             { $$ = driver.makeAstNode<DeferrableClauseAST>(  0,  0, $1, $2 ); }
    | T_INITIALLY T_IMMEDIATE                            { $$ = driver.makeAstNode<DeferrableClauseAST>(  0,  0, $1, $2 ); }
    ;

constraint_decl_optional
    : empty_aka_optional
    | T_CONSTRAINT T_IDENTIFIER
      {
        $$ = driver.makeAstNode<ConstraintDeclarationAST>( $1,
                                                           driver.createName(NameAST::Constraint, $2) );
      }
    ;

column_constraint_item
    : constraint_decl_optional T_PRIMARY T_KEY deferrable_cluase
      {
        $$  = driver.makeAstNode<PkColumnConstraintAST>( $1->asConstraintDeclaration(),
                                                         $2,
                                                         $3,
                                                         $4->asDeferrableClause() );
      }
    | constraint_decl_optional not_optional T_NULL deferrable_cluase
      {
        $$ = driver.makeAstNode<NotNullColumnConstraintAST>( $1->asConstraintDeclaration(),
                                                             $2,
                                                             $3,
                                                             $4->asDeferrableClause() );
      }
    | constraint_decl_optional T_UNIQUE deferrable_cluase
      {
        $$ = driver.makeAstNode<UniqueColumnConstraintAST>( $1->asConstraintDeclaration(),
                                                            $2,
                                                            $3->asDeferrableClause() );
      }
    | constraint_decl_optional T_CHECK T_LEFT_PAREN conditional_expression T_RIGHT_PAREN deferrable_cluase
      {
        $$ = driver.makeAstNode<CheckColumnConstraintAST>( $1->asConstraintDeclaration(),
                                                           $2,
                                                           $3,
                                                           $4->asConditionExpression(),
                                                           $5,
                                                           $6->asDeferrableClause());
      }
    | constraint_decl_optional referrences_clause deferrable_cluase
      {
        $$ = driver.makeAstNode<ReferencesColumnConstraintAST>( $1->asConstraintDeclaration(),
                                                                $2->asFkReferencesClause(),
                                                                $3->asDeferrableClause() );
      }
    ;

delete_or_update
    : T_DELETE
    | T_UPDATE
    ;

references_action_clause
    : empty_aka_optional
    | T_ON delete_or_update T_NO  T_ACTION  { $$ = driver.makeAstNode<FkReferencesActionAST>( $1, $2, $3, $4 ); }
    | T_ON delete_or_update T_SET T_NULL    { $$ = driver.makeAstNode<FkReferencesActionAST>( $1, $2, $3, $4 ); }
    | T_ON delete_or_update T_SET T_DEFAULT { $$ = driver.makeAstNode<FkReferencesActionAST>( $1, $2, $3, $4 ); }
    | T_ON delete_or_update T_RESTRICT      { $$ = driver.makeAstNode<FkReferencesActionAST>( $1, $2, $3,  0 ); }
    | T_ON delete_or_update T_CASCADE       { $$ = driver.makeAstNode<FkReferencesActionAST>( $1, $2, $3,  0 ); }
    ;

references_match_clause
    : empty_aka_optional
    | T_MATCH T_FULL    { $$ = driver.makeAstNode<FkReferencesMatchAST>( $1, $2); }
    | T_MATCH T_PARTIAL { $$ = driver.makeAstNode<FkReferencesMatchAST>( $1, $2); }
    | T_MATCH T_SIMPLE  { $$ = driver.makeAstNode<FkReferencesMatchAST>( $1, $2); }
    ;

referrences_clause
    : T_REFERENCES table_identifier T_LEFT_PAREN comma_name_list T_RIGHT_PAREN references_match_clause references_action_clause
      {
        $$ = driver.makeAstNode<FkReferencesClauseAST>( $1,
                                                        $2->asTableIdentifier(),
                                                        $3,
                                                        $4,
                                                        $5,
                                                        $6->asFkReferencesMatch(),
                                                        $7->asFkReferencesAction() );
      }
    | T_REFERENCES table_identifier
      {
        $$ = driver.makeAstNode<FkReferencesClauseAST>( $1,
                                                        $2->asTableIdentifier(),
                                                        0,
                                                        (NameListAST *) 0,
                                                        0,
                                                        (FkReferencesMatchAST *) 0,
                                                        (FkReferencesActionAST *) 0 );
      }
    ;

comma_name_list
    : comma_name_list T_COMMA T_IDENTIFIER
      {
        $$ = $1;
        if ( $3 )
          $$ = driver.makeAstNode<NameListAST>( $1,
                                                driver.createName(NameAST::Column, $3) );
      }
    | T_IDENTIFIER
      {
        $$ = 0;
        if ( $1 )
          $$ = driver.makeAstNode<NameListAST>( driver.createName(NameAST::Column, $1) );
      }
    ;

column_constraint_list
    : column_constraint_list column_constraint_item
      {
        $$ = $1;
        if ( $2 )
          $$ = driver.makeAstNode<ColumnConstraintListAST>( $1, $2->asColumnConstraint() );
      } /*
    | column_constraint_item
      {
        $$ = 0;
        if ( $1 )
          $$ = driver.makeAstNode<ColumnConstraintListAST>( $1->asColumnConstraint() );
      } */
    | /* empty */ { $$ = 0; }
    ;

column_definition
    : T_IDENTIFIER type_spec collate_clause T_DEFAULT value_expression column_constraint_list
      {
        $$ = driver.makeAstNode<ColumnDefinitionAST>( driver.createName(NameAST::Column, $1),
                                                      $2->asBaseType(),
                                                      $3->asCollateClause(),
                                                      $4,
                                                      $5->asValueExpression(),
                                                      $6 );
      }
    | T_IDENTIFIER type_spec collate_clause column_constraint_list
      {
        $$ = driver.makeAstNode<ColumnDefinitionAST>( driver.createName(NameAST::Column, $1),
                                                      $2->asBaseType(),
                                                      $3->asCollateClause(),
                                                      0,
                                                      (ConstantExpressionAST *) 0,
                                                      $4 );
      }
    | T_IDENTIFIER
      {
        $$ = driver.makeAstNode<ColumnDefinitionAST>( driver.createName(NameAST::Column, $1),
                                                      (BaseTypeAST *) 0,
                                                      (CollateClauseAST *) 0,
                                                      0,
                                                      (ConstantExpressionAST *) 0,
                                                      (ColumnConstraintListAST *) 0 );
      }
    ;

alter_table_action
    : empty_aka_optional
    | T_ADD column_optional column_definition
      {
        $$ = driver.makeAstNode<AlterTableAddColumnActionAST>( $1,
                                                               $2,
                                                               $3->asColumnDefinition() );
      }
    | T_RENAME T_TO T_IDENTIFIER
      {
        $$ = driver.makeAstNode<AlterTableRenameActionAST>( $1,
                                                            $2,
                                                            driver.createName(NameAST::Table, $3) );
      }
    | T_RENAME column_optional T_IDENTIFIER T_TO T_IDENTIFIER
      {
        $$ = driver.makeAstNode<AlterTableRenameColumnActionAST>( $1,
                                                                  $2,
                                                                  driver.createName(NameAST::Column, $3),
                                                                  $4,
                                                                  driver.createName(NameAST::Column, $5) );
      }
    | T_RENAME column_optional
      {
        $$ = driver.makeAstNode<AlterTableRenameColumnActionAST>( $1,
                                                                  $2,
                                                                  (NameAST *) 0,
                                                                  0,
                                                                  (NameAST *) 0 );
      }
    | T_RENAME T_CONSTRAINT T_IDENTIFIER T_TO T_IDENTIFIER
      {
        $$ = driver.makeAstNode<AlterTableRenameConstraintActionAST>( $1,
                                                                      $2,
                                                                      driver.createName(NameAST::Constraint, $3),
                                                                      $4,
                                                                      driver.createName(NameAST::Constraint, $5) );
      }
    | T_RENAME T_CONSTRAINT
      {
        $$ = driver.makeAstNode<AlterTableRenameConstraintActionAST>( $1,
                                                                      $2,
                                                                      (NameAST *) 0,
                                                                      0,
                                                                      (NameAST *) 0 );
      }
    | enable_or_disable T_TRIGGER trigger_wildcard
      {
        $$ = driver.makeAstNode<AlterTableAbleTriggerActionAST>( $1,
                                                                 $2,
                                                                 $3->asTriggerWildcard() );
      }
    | T_DROP column_optional if_exists_clause T_IDENTIFIER restrict_or_cascade_optional
      {
        $$ = driver.makeAstNode<AlterTableDropColumnActionAST>( $1,
                                                                $2,
                                                                $3->asIfExists(),
                                                                driver.createName(NameAST::Column, $4),
                                                                $5 );
      }
    | T_DROP column_optional if_exists_clause
      {
        $$ = driver.makeAstNode<AlterTableDropColumnActionAST>( $1,
                                                                $2,
                                                                $3->asIfExists(),
                                                                (NameAST *) 0,
                                                                0 );
      }
    | T_DROP T_CONSTRAINT if_exists_clause T_IDENTIFIER restrict_or_cascade_optional
      {
        $$ = driver.makeAstNode<AlterTableDropConstraintActionAST>( $1,
                                                                    $2,
                                                                    $3->asIfExists(),
                                                                    driver.createName(NameAST::Constraint, $4),
                                                                    $5 );
      }
    | T_DROP T_CONSTRAINT if_exists_clause
      {
        $$ = driver.makeAstNode<AlterTableDropConstraintActionAST>( $1,
                                                                    $2,
                                                                    $3->asIfExists(),
                                                                    (NameAST *) 0,
                                                                    0 );
      }
    ;

alter_table_statement
    : T_ALTER T_TABLE if_exists_clause only_optional table_identifier alter_table_action semicolon_or_eof
      {
        $$ = driver.makeAstNode<AlterTableStatementAST>( $1,
                                                         $2,
                                                         $3->asIfExists(),
                                                         $4,
                                                         $5->asTableIdentifier(),
                                                         $6->asAlterTableAction() );
      }
    ;

comment_statement
    : T_COMMENT T_ON T_TABLE table_identifier T_IS T_CHARSTR semicolon_or_eof
      {
        $$ = driver.makeAstNode<CommentOnTableStatementAST>( $1,
                                                             $2,
                                                             $3,
                                                             $4->asTableIdentifier(),
                                                             $5,
                                                             driver.makeAstNode<LiteralExpressionAST>($6) );
      }
    | T_COMMENT T_ON T_TABLE table_identifier semicolon_or_eof
      {
        $$ = driver.makeAstNode<CommentOnTableStatementAST>( $1,
                                                             $2,
                                                             $3,
                                                             $4->asTableIdentifier(),
                                                             0,
                                                             (LiteralExpressionAST *) 0 );
      }
    | T_COMMENT T_ON T_COLUMN column_identifier T_IS T_CHARSTR semicolon_or_eof
      {
        $$ = driver.makeAstNode<CommentOnColumnStatementAST>( $1,
                                                              $2,
                                                              $3,
                                                              $4->asColumnIdentifier(),
                                                              $5,
                                                              driver.makeAstNode<LiteralExpressionAST>($6) );
      }
    | T_COMMENT T_ON T_COLUMN column_identifier semicolon_or_eof
      {
        $$ = driver.makeAstNode<CommentOnColumnStatementAST>( $1,
                                                              $2,
                                                              $3,
                                                              $4->asColumnIdentifier(),
                                                              0,
                                                              (LiteralExpressionAST *) 0 );
      }
    ;

truncate_statement
    : T_TRUNCATE T_TABLE table_identifier semicolon_or_eof
      { $$ = driver.makeAstNode<TruncateStatementAST>( $1, $2, $3->asTableIdentifier() ); }
    ;

drop_statement
    : T_DROP T_TABLE table_identifier semicolon_or_eof
      { $$ = driver.makeAstNode<DropTableStatementAST>( $1, $2, $3->asTableIdentifier() ); }
    | T_DROP T_VIEW table_identifier semicolon_or_eof
      { $$ = driver.makeAstNode<DropViewStatementAST>( $1, $2, $3->asTableIdentifier() ); }
    | T_DROP T_INDEX index_identifier semicolon_or_eof
      { $$ = driver.makeAstNode<DropIndexStatementAST>( $1, $2, $3->asIndexIdentifier() ); }
    | T_DROP T_TRIGGER trigger_identifier semicolon_or_eof
      { $$ = driver.makeAstNode<DropTriggerStatementAST>( $1, $2, $3->asTriggerIdentifier() ); }
    ;

insert_columns_clause
    : empty_aka_optional
    | T_LEFT_PAREN comma_column_identifier_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<InsertColumnsClauseAST>( $1, $2, $3 ); }
    ;

insert_values_clause
    : T_VALUES T_LEFT_PAREN comma_expression_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<InsertClauseAsValuesAST>( $1, $2, $3, $4 ); }
    | sel_statement
      { $$ = driver.makeAstNode<InsertClauseAsSelectAST>( $1->asSelectStatement() ); }
    ;

insert_statement
    : T_INSERT T_INTO direct_table_reference insert_columns_clause insert_values_clause semicolon_or_eof
      {
        $$ = driver.makeAstNode<InsertStatementAST>( $1,
                                                     $2,
                                                     $3->asDirectSingleSource(),
                                                     $4->asInsertColumnsClause(),
                                                     $5->asInsertClause() );
      }
    ;

update_set_item
    : column_identifier T_EQUAL value_expression
      {
        $$ = driver.makeAstNode<AssignmentExpressionAST>( $1->asColumnIdentifier(),
                                                          $2,
                                                          $3->asValueExpression() );
      }
    | column_identifier T_EQUAL
      {
        $$ = driver.makeAstNode<AssignmentExpressionAST>( $1->asColumnIdentifier(),
                                                          $2,
                                                          (ValueExpressionAST *) 0 );
        driver.error($2, "Assignment Value Expected");
      }
    | column_identifier
      {
        $$ = driver.makeAstNode<AssignmentExpressionAST>( $1->asColumnIdentifier(),
                                                          0,
                                                          (ValueExpressionAST *) 0 );
        driver.error($1, "Assignment Value Expected");
      }
    ;

update_set_list
    : update_set_list T_COMMA update_set_item
      {
        $$ = $1;
        if ( $3 )
          $$ = driver.makeAstNode<AssignmentExpressionListAST>( $1, $3->asAssignmentExpression() );
      }
    | update_set_item
      {
        $$ = 0;
        if ( $1 )
          $$ = driver.makeAstNode<AssignmentExpressionListAST>( $1->asAssignmentExpression() );
      }
    ;

update_set_clause
    : T_SET update_set_list
      { $$ = driver.makeAstNode<UpdateSetClauseAST>( $1, $2 ); }
    ;

update_statement
    : T_UPDATE direct_table_reference update_set_clause where_clause semicolon_or_eof
      {
        $$ = driver.makeAstNode<UpdateStatementAST>( $1,
                                                     $2->asDirectSingleSource(),
                                                     $3->asUpdateSetClause(),
                                                     $4->asWhereClause() );
      }
    | T_UPDATE direct_table_reference
      {
        $$ = driver.makeAstNode<UpdateStatementAST>( $1,
                                                     $2->asDirectSingleSource(),
                                                     (UpdateSetClauseAST *) 0,
                                                     (WhereClauseAST *) 0 );
      }
    ;

delete_statement
    : T_DELETE T_FROM direct_table_reference where_clause semicolon_or_eof
      {
        $$ = driver.makeAstNode<DeleteStatementAST>( $1,
                                                     $2,
                                                     $3->asDirectSingleSource(),
                                                     $4->asWhereClause() );
      }
    ;

top_select_statement
    : sel_statement semicolon_or_eof
    ;

select_spec
    :             { $$ = 0; }
    | T_ALL       { $$ = $1; }
    | T_DISTINCT  { $$ = $1; }
    ;

select_core_item
    : T_SELECT select_spec select_columns T_FROM table_reference_list where_clause group_clause set_clause
      {
        $$ = driver.makeAstNode<SelectCoreAST>( $1,                         /* SELECT token */
                                                $2,                         /* ALL|DISTINCT token */
                                                $3,                         /* select columns */
                                                $4,                         /* FROM token */
                                                $5,                         /* select source */
                                                $6->asWhereClause(),        /* WHERE ... */
                                                $7->asGroupByBlock(),       /* GROUP BY ... */
                                                $8->asSetClause()           /* UNION | INTERSECT | MINUS <select_core> */ );
      }
    ;

sel_statement
    : select_core_item order_clause update_clause
      {
        $$ = driver.makeAstNode<SelectStatementAST>( $1->asSelectCore(),
                                                     $2->asOrderByBlock(),
                                                     $3->asForUpdateClause() );
      }
    ;

order_definition
    : /* empty */     { $$ = 0; }
    | T_DESC
    | T_ASC
    ;

ordered_expression
    : value_expression order_definition
      { $$ = driver.makeAstNode<OrderingTermAST>( $1->asValueExpression(), $2 ); }
    ;

comma_ordered_expression_list
    : comma_ordered_expression_list T_COMMA ordered_expression
      {
        $$ = $1;
        if ( $3 )
          $$ = driver.makeAstNode<OrderingTermListAST>( $1, $3->asOrderingTerm() );
      }
    | ordered_expression
      {
        $$ = 0;
        if ( $1 )
          $$ = driver.makeAstNode<OrderingTermListAST>( $1->asOrderingTerm() );
      }
    ;

order_clause
    : empty_aka_optional
    | T_ORDER T_BY comma_ordered_expression_list
      { $$ = driver.makeAstNode<OrderByBlockAST>( $1, $2, $3); }
    ;

comma_column_identifier_list
    : comma_column_identifier_list T_COMMA column_identifier
      {
        $$ = $1;
        if ( $3 )
          $$ = driver.makeAstNode<ColumnIdentifierListAST>( $1, $3->asColumnIdentifier() );
      }
    | column_identifier
      {
        $$ = 0;
        if ( $1 )
          $$ = driver.makeAstNode<ColumnIdentifierListAST>( $1->asColumnIdentifier() );
      }
    ;

nowait_clause
    : /* empty */ { $$ = 0; }
    | T_NOWAIT
    ;

update_clause
    : empty_aka_optional
    | T_FOR T_UPDATE T_OF comma_column_identifier_list nowait_clause
      { $$ = driver.makeAstNode<ForUpdateClauseAST>( $1, $2, $3, $4, $5 ); }
    ;

group_clause
    : empty_aka_optional
    | T_GROUP T_BY comma_expression_list T_HAVING conditional_expression
      {
        $$ = driver.makeAstNode<GroupByBlockAST>( $1,
                                                  $2,
                                                  $3,
                                                  $4,
                                                  $5->asConditionExpression() );
      }
    | T_GROUP T_BY comma_expression_list
      {
        $$ = driver.makeAstNode<GroupByBlockAST>( $1,
                                                  $2,
                                                  $3,
                                                  0,
                                                  (ConditionExpressionAST *) 0 );
      }
    ;

set_clause
    : empty_aka_optional
    | T_UNION T_ALL select_core_item
      {
        $$ = driver.makeAstNode<SetClauseAST>( driver.makeAstNode<UnionSetSpecAST>($1, $2),
                                               $3->asSelectCore() );
      }
    | T_UNION T_DISTINCT select_core_item
      {
        $$ = driver.makeAstNode<SetClauseAST>( driver.makeAstNode<UnionSetSpecAST>($1, $2),
                                               $3->asSelectCore() );
      }
    | T_UNION select_core_item
      {
        $$ = driver.makeAstNode<SetClauseAST>( driver.makeAstNode<UnionSetSpecAST>($1, 0),
                                               $2->asSelectCore() );
      }
    | T_INTERSECT select_core_item
      {
        $$ = driver.makeAstNode<SetClauseAST>( driver.makeAstNode<IntersectSetSpecAST>($1),
                                               $2->asSelectCore() );
      }
    | T_MINUS select_core_item
      {
        $$ = driver.makeAstNode<SetClauseAST>( driver.makeAstNode<MinusSetSpecAST>($1),
                                               $2->asSelectCore() );
      }
    ;

where_clause
    : empty_aka_optional
    | T_WHERE conditional_expression
      {
        $$ = driver.makeAstNode<WhereClauseAST>( $1,
                                                 $2->asConditionExpression() );
      }
    ;

join_spec
    : T_JOIN                  { $$ = driver.makeAstNode<JoinSpecAST>( 0,  0, $1); }
    | T_INNER T_JOIN          { $$ = driver.makeAstNode<JoinSpecAST>( 0, $1, $2); }
    | T_OUTER T_JOIN          { $$ = driver.makeAstNode<JoinSpecAST>( 0, $1, $2); }
    | T_LEFT T_JOIN           { $$ = driver.makeAstNode<JoinSpecAST>( 0, $1, $2); }
    | T_RIGHT T_JOIN          { $$ = driver.makeAstNode<JoinSpecAST>( 0, $1, $2); }
    | T_FULL T_JOIN           { $$ = driver.makeAstNode<JoinSpecAST>( 0, $1, $2); }
    | T_CROSS T_JOIN          { $$ = driver.makeAstNode<JoinSpecAST>( 0, $1, $2); }
    | T_INNER T_LEFT T_JOIN   { $$ = driver.makeAstNode<JoinSpecAST>($1, $2, $3); }
    | T_INNER T_RIGHT T_JOIN  { $$ = driver.makeAstNode<JoinSpecAST>($1, $2, $3); }
    | T_OUTER T_LEFT T_JOIN   { $$ = driver.makeAstNode<JoinSpecAST>($1, $2, $3); }
    | T_OUTER T_RIGHT T_JOIN  { $$ = driver.makeAstNode<JoinSpecAST>($1, $2, $3); }
    ;

join_condition
    : empty_aka_optional
    | T_ON conditional_expression
      {
        $$ = driver.makeAstNode<JoinConditionAST>( $1,
                                                   $2->asConditionExpression() );
      }
    ;

joined_table
    : T_LEFT_PAREN joined_table T_RIGHT_PAREN
      {
        $$ = driver.makeAstNode<CompoundJoinedTableAST>( $1,
                                                         $2->asJoinedTable(),
                                                         $3 );
      }
    | table_reference join_spec direct_table_reference join_condition
      {
        $$ = driver.makeAstNode<JoinJoinedTableAST>( $1->asTableReference(),
                                                     $2->asJoinSpec(),
                                                     $3->asTableReference(),
                                                     $4->asJoinCondition() );
      }
    ;

direct_table_reference
    : table_identifier T_AS T_IDENTIFIER
      {
        $$ = driver.makeAstNode<DirectSingleSourceAST>( $1->asTableIdentifier(),
                                                        $2,
                                                        driver.createName(NameAST::TableAlias, $3) );
      }
    | table_identifier T_IDENTIFIER
      {
        $$ = driver.makeAstNode<DirectSingleSourceAST>( $1->asTableIdentifier(),
                                                        0,
                                                        driver.createName(NameAST::TableAlias, $2) );
      }
    | table_identifier
      {
        $$ = driver.makeAstNode<DirectSingleSourceAST>( $1->asTableIdentifier(),
                                                        0,
                                                        (NameAST *) 0 );
      }
    ;

subquery_table_reference
    : T_LEFT_PAREN sel_statement T_RIGHT_PAREN T_AS T_IDENTIFIER
      {
        $$ = driver.makeAstNode<SubQuerySingleSourceAST>( $1,
                                                          $2->asSelectStatement(),
                                                          $3,
                                                          $4,
                                                          driver.createName(NameAST::TableAlias, $5) );
      }
    | T_LEFT_PAREN sel_statement T_RIGHT_PAREN T_IDENTIFIER
      {
        $$ = driver.makeAstNode<SubQuerySingleSourceAST>( $1,
                                                          $2->asSelectStatement(),
                                                          $3,
                                                          0,
                                                          driver.createName(NameAST::TableAlias, $4) );
      }
    | T_LEFT_PAREN sel_statement T_RIGHT_PAREN
      {
        $$ = driver.makeAstNode<SubQuerySingleSourceAST>( $1,
                                                          $2->asSelectStatement(),
                                                          $3,
                                                          0,
                                                          (NameAST *) 0 );
      }
    ;

table_reference
    : direct_table_reference
    | subquery_table_reference
    | joined_table
    ;

table_reference_list
    : table_reference_list T_COMMA table_reference
      {
        $$ = $1;
        if ( $3 )
          $$ = driver.makeAstNode<TableReferenceListAST>( $1, $3->asTableReference() );
      }
    | table_reference
      {
        $$ = 0;
        if ( $1 )
          $$ = driver.makeAstNode<TableReferenceListAST>( $1->asTableReference() );
      }
    ;

select_columns_item
    : T_STAR
      { $$ = driver.makeAstNode<StarSelectColumnAST>( $1 ); }
    | T_IDENTIFIER T_DOT T_IDENTIFIER T_DOT T_STAR
      {
        TableIdentifierAST *tabIdent = driver.makeAstNode<TableIdentifierAST>( driver.createName(NameAST::Schema, $1),
                                                                               $2,
                                                                               driver.createName(NameAST::Table, $3) );
        $$ = driver.makeAstNode<TableSelectColumnAST>( tabIdent, $4, $5 );
      }
    | T_IDENTIFIER T_DOT T_STAR
      {
        TableIdentifierAST *tabIdent = driver.makeAstNode<TableIdentifierAST>( (NameAST *) 0,
                                                                               0,
                                                                               driver.createName(NameAST::Table, $1) );
        $$ = driver.makeAstNode<TableSelectColumnAST>( tabIdent, $2, $3 );
      }
    | value_expression T_AS T_IDENTIFIER
      {
        $$ = driver.makeAstNode<ExpressionSelectColumnAST>( $1->asValueExpression(),
                                                            $2,
                                                            driver.createName(NameAST::ColumnAlias, $3) );
      }
    | value_expression T_IDENTIFIER
      {
        $$ = driver.makeAstNode<ExpressionSelectColumnAST>( $1->asValueExpression(),
                                                            0,
                                                            driver.createName(NameAST::ColumnAlias, $2) );
      }
    | value_expression
      {
        $$ = driver.makeAstNode<ExpressionSelectColumnAST>( $1->asValueExpression(),
                                                            0,
                                                            (NameAST *) 0 );
      }
    ;

select_columns
    : select_columns T_COMMA select_columns_item
      {
        $$ = $1;
        if ( $3 )
          $$ = driver.makeAstNode<BaseSelectColumnListAST>( $1, $3->asBaseSelectColumn() );
      }
    | select_columns_item
      {
        $$ = 0;
        if ( $1 )
          $$ = driver.makeAstNode<BaseSelectColumnListAST>( $1->asBaseSelectColumn() );
      }
    ;

commit_statement
    : T_COMMIT semicolon_or_eof   { $$ = driver.makeAstNode<CommitStatementAST>($1); }
    ;

rollback_statement
    : T_ROLLBACK semicolon_or_eof { $$ = driver.makeAstNode<RollbackStatementAST>($1); }
    ;

value_expression
    : binary_expression
    ;

binary_op
    : T_PLUS
    | T_DASH
    | T_PIPE_PIPE
    | T_STAR
    | T_SLASH
    ;

binary_expression
    : binary_expression binary_op unary_expression
      {
        $$ = driver.makeAstNode<BinaryValueExpressionAST>( $1->asValueExpression(),
                                                           $2,
                                                           $3->asValueExpression() );
      }
    | unary_expression
    ;

unary_op
    : T_PLUS
    | T_DASH
    ;

unary_expression
    : unary_op primary_expression { $$ = driver.makeAstNode<UnaryValueExpressionAST>( $1, $2->asValueExpression() ); }
    | primary_expression
    ;

builtin_function_name
    : T_DAY
    | T_MAX
    | T_MIN
    | T_PAD
    | T_SUM
    | T_HOUR
    | T_LAST
    | T_SIZE
    | T_FIRST
    | T_FOUND
    | T_MONTH
    | T_MINUTE
    | T_SECOND
    ;

count_spec
    :             { $$ = 0; }
    | T_ALL
    | T_DISTINCT
    ;

char_type_name
    : T_BIT
    | T_BIT T_VARYING       { $$ = $2; }
    | T_VARBIT
    | T_CHAR
    | T_CHARACTER
    | T_CHARACTER T_VARYING { $$ = $2; }
    | T_VARCHAR
    ;

simple_type_name
    : T_BIGINT
    | T_BIGSERIAL
    | T_BOX
    | T_BYTEA
    | T_CIDR
    | T_CIRCLE
    | T_INET
    | T_JSON
    | T_LINE
    | T_LSEG
    | T_MACADDR
    | T_MONEY
    | T_PATH
    | T_POINT
    | T_POLYGON
    | T_REAL
    | T_SMALLSERIAL
    | T_SERIAL
    | T_TEXT
    | T_TIME
    | T_TIMESTAMP
    | T_TSQUERY
    | T_TSVECTOR
    | T_TXID_SNAPSHOT
    | T_UUID
    | T_XML
    | T_INT
    | T_DATE
    | T_YEAR
    | T_ZONE
    | T_FLOAT
    | T_DOUBLE T_PRECISION  { $$ = $2; }
    | T_DOUBLE
    | T_BOOLEAN
    | T_INTEGER
    | T_NUMBER
    | T_SMALLINT
    | T_BOOL
    | T_INT2
    | T_INT4
    | T_INT8
    | T_SERIAL2
    | T_SERIAL4
    | T_SERIAL8
    | T_FLOAT4
    | T_FLOAT8
    | T_TIMETZ
    | T_TIMESTAMPTZ
    ;

numeric_or_decimal
    : T_NUMERIC
    | T_DECIMAL
    ;

type_spec
    : char_type_name
      { $$ = driver.makeAstNode<CharacterTypeAST>($1, 0, 0, 0); }
    | char_type_name T_LEFT_PAREN T_RIGHT_PAREN
      { $$ = driver.makeAstNode<CharacterTypeAST>($1, $2, 0, $3); }
    | char_type_name T_LEFT_PAREN T_DIGIT T_RIGHT_PAREN
      { $$ = driver.makeAstNode<CharacterTypeAST>($1, $2, $3, $4); }
    | numeric_or_decimal
      { $$ = driver.makeAstNode<NumberTypeAST>($1, 0, 0, 0, 0, 0); }
    | numeric_or_decimal T_LEFT_PAREN T_DIGIT T_RIGHT_PAREN
      { $$ = driver.makeAstNode<NumberTypeAST>($1, $2, $3, 0, 0, $4); }
    | numeric_or_decimal T_LEFT_PAREN T_DIGIT T_COMMA T_DIGIT T_RIGHT_PAREN
      { $$ = driver.makeAstNode<NumberTypeAST>($1, $2, $3, $4, $5, $6); }
    | simple_type_name
      { $$ = driver.makeAstNode<SimpleTypeAST>($1); }
    | T_IDENTIFIER
      { $$ = driver.makeAstNode<UserDefinedTypeAST>( driver.createName(NameAST::Type, $1) ); }
    ;

column_identifier
    : T_IDENTIFIER T_DOT T_IDENTIFIER T_DOT T_IDENTIFIER
      {
        $$ = driver.makeAstNode<ColumnIdentifierAST>( driver.createName(NameAST::Schema, $1),
                                                      $2,
                                                      driver.createName(NameAST::Table, $3),
                                                      $4,
                                                      driver.createName(NameAST::Column, $5) );
      }
    | T_IDENTIFIER T_DOT T_IDENTIFIER T_DOT   /* +3 shift/reduce conflicts */
      {
        $$ = driver.makeAstNode<ColumnIdentifierAST>( driver.createName(NameAST::Schema, $1),
                                                      $2,
                                                      driver.createName(NameAST::Table, $3),
                                                      $4,
                                                      (NameAST *) 0 );
        driver.error($4, "Table Identifier Expected");
      }
    | T_IDENTIFIER T_DOT T_IDENTIFIER
      {
        $$ = driver.makeAstNode<ColumnIdentifierAST>( (NameAST *) 0,
                                                      0,
                                                      driver.createName(NameAST::Table, $1),
                                                      $2,
                                                      driver.createName(NameAST::Column, $3) );
      }
    | T_IDENTIFIER T_DOT                      /* +3 shift/reduce conflicts */
      {
        $$ = driver.makeAstNode<ColumnIdentifierAST>( (NameAST *) 0,
                                                      0,
                                                      driver.createName(NameAST::Table, $1),
                                                      $2,
                                                      (NameAST *) 0 );
        driver.error($2, "Table Identifier Expected");
      }
    | T_IDENTIFIER
      {
        $$ = driver.makeAstNode<ColumnIdentifierAST>( (NameAST *) 0,
                                                      0,
                                                      (NameAST *) 0,
                                                      0,
                                                      driver.createName(NameAST::Column, $1) );
      }
    ;

case_expression_item
    : T_WHEN value_expression T_THEN value_expression
      {
        $$ = driver.makeAstNode<CaseExpressionClauseAST>( $1,
                                                          $2->asValueExpression(),
                                                          $3,
                                                          $4->asValueExpression() );
      }
    ;

case_expression_list
    : case_expression_list case_expression_item
      {
        $$ = $1;
        if ( $2 )
          $$ = driver.makeAstNode<CaseExpressionClauseListAST>( $1, $2->asCaseExpressionClause() );
      }
    | case_expression_item
      {
        $$ = 0;
        if ( $1 )
          $$ = driver.makeAstNode<CaseExpressionClauseListAST>( $1->asCaseExpressionClause() );
      }
    ;

case_expression
    : T_CASE value_expression case_expression_list T_ELSE value_expression T_END
      {
        $$ = driver.makeAstNode<CaseExpressionAST>( $1,
                                                    $2->asValueExpression(),
                                                    $3,
                                                    $4,
                                                    $5->asValueExpression(),
                                                    $6 );
      }
    | T_CASE value_expression case_expression_list T_END
      {
        $$ = driver.makeAstNode<CaseExpressionAST>( $1,
                                                    $2->asValueExpression(),
                                                    $3,
                                                    0,
                                                    (ValueExpressionAST *) 0,
                                                    $4 );
      }
    ;

case_condition_item
    : T_WHEN conditional_expression T_THEN value_expression
      {
        $$ = driver.makeAstNode<CaseConditionClauseAST>( $1,
                                                         $2->asConditionExpression(),
                                                         $3,
                                                         $4->asValueExpression() );
      }
    ;

case_condition_list
    : case_condition_list case_condition_item
      {
        $$ = $1;
        if ( $2 )
          $$ = driver.makeAstNode<CaseConditionClauseListAST>( $1, $2->asCaseConditionClause() );
      }
    | case_condition_item
      {
        $$ = 0;
        if ( $1 )
          $$ = driver.makeAstNode<CaseConditionClauseListAST>( $1->asCaseConditionClause() );
      }
    ;

case_condition
    : T_CASE case_condition_list T_ELSE value_expression T_END
      {
        $$ = driver.makeAstNode<CaseConditionAST>( $1,
                                                   $2,
                                                   $3,
                                                   $4->asValueExpression(),
                                                   $5 );
      }
    | T_CASE case_condition_list T_END
      {
        $$ = driver.makeAstNode<CaseConditionAST>( $1,
                                                   $2,
                                                   0,
                                                   (ValueExpressionAST *) 0,
                                                   $3 );
      }
    ;

primary_expression
    : T_LEFT_PAREN value_expression T_RIGHT_PAREN
      {
        $$ = driver.makeAstNode<CompoundValueExpressionAST>( $1,
                                                             $2->asValueExpression(),
                                                             $3 );
      }
    | T_LEFT_PAREN sel_statement T_RIGHT_PAREN
      {
        $$ = driver.makeAstNode<SelectCompoundValueExpressionAST>( $1,
                                                                   $2->asSelectStatement(),
                                                                   $3 );
      }
    | builtin_function_name T_LEFT_PAREN comma_expression_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<BuiltinFunctionCallAST>($1, $2, $3, $4); }
    | T_COUNT T_LEFT_PAREN T_STAR T_RIGHT_PAREN
      { $$ = driver.makeAstNode<CountStarCallAST>($1, $2, $3, $4); }
    | T_COUNT T_LEFT_PAREN count_spec value_expression T_RIGHT_PAREN
      { $$ = driver.makeAstNode<CountExprCallAST>($1, $2, $3, $4->asValueExpression(), $5); }
    | T_CAST T_LEFT_PAREN value_expression T_AS type_spec T_RIGHT_PAREN
      {
        $$ = driver.makeAstNode<CastFunctionAST>( $1,
                                                  $2,
                                                  $3->asValueExpression(),
                                                  $4,
                                                  $5->asBaseType(),
                                                  $6 );
      }
    | T_CAST T_LEFT_PAREN value_expression T_RIGHT_PAREN
      {
        $$ = driver.makeAstNode<CastFunctionAST>( $1,
                                                  $2,
                                                  $3->asValueExpression(),
                                                  0,
                                                  (BaseTypeAST *) 0,
                                                  $4 );
      }
    | T_SUBSTRING T_LEFT_PAREN value_expression T_FROM value_expression T_FOR value_expression T_RIGHT_PAREN
      {
        $$ = driver.makeAstNode<SubstringFunctionAST>( $1,
                                                       $2,
                                                       $3->asValueExpression(),
                                                       $4,
                                                       $5->asValueExpression(),
                                                       $6,
                                                       $7->asValueExpression(),
                                                       $8 );
      }
    | T_SUBSTRING T_LEFT_PAREN value_expression T_FROM value_expression T_RIGHT_PAREN
      {
        $$ = driver.makeAstNode<SubstringFunctionAST>( $1,
                                                       $2,
                                                       $3->asValueExpression(),
                                                       $4,
                                                       $5->asValueExpression(),
                                                       0,
                                                       (ValueExpressionAST *) 0,
                                                       $6 );
      }
    | T_SUBSTRING T_LEFT_PAREN value_expression T_RIGHT_PAREN
      {
        $$ = driver.makeAstNode<SubstringFunctionAST>( $1,
                                                       $2,
                                                       $3->asValueExpression(),
                                                       0,
                                                       (ValueExpressionAST *) 0,
                                                       0,
                                                       (ValueExpressionAST *) 0,
                                                       $4 );
      }
    | case_expression
    | case_condition
    | func_ident T_LEFT_PAREN comma_expression_list T_RIGHT_PAREN
      {
        $$ = driver.makeAstNode<FunctionCallAST>( $1->asFunctionIdentifier(),
                                                  $2,
                                                  $3,
                                                  $4 );
      }
    | column_identifier
    | T_DIGIT    { $$ = driver.makeAstNode<NumericExpressionAST>($1); }
    | T_CHARSTR  { $$ = driver.makeAstNode<LiteralExpressionAST>($1); }
    | T_NULL     { $$ = driver.makeAstNode<NullValueExpressionAST>($1); }
    | T_COLON T_IDENTIFIER
      {
        $$ = driver.makeAstNode<ParameterExpressionAST>( $1,
                                                         driver.createName(NameAST::Binding, $2) );
      }
    | T_COLON
      {
        $$ = driver.makeAstNode<ParameterExpressionAST>( $1,
                                                         (NameAST *) 0 );
        driver.error($1, "Parameter Identifier Expected");
      }
    ;

unknown_statement
    : error semicolon_or_eof
    ;

comma_expression_list
    : comma_expression_list T_COMMA value_expression
      {
        $$ = $1;
        if ( $3 )
          $$ = driver.makeAstNode< List<ValueExpressionAST *> >( $1, $3->asValueExpression() );
      }
    | value_expression
      {
        $$ = 0;
        if ( $1 )
          $$ = driver.makeAstNode< List<ValueExpressionAST *> >( $1->asValueExpression() );
      }
    ;

table_identifier
    : T_IDENTIFIER T_DOT T_IDENTIFIER
      {
        $$ = driver.makeAstNode<TableIdentifierAST>( driver.createName(NameAST::Schema, $1),
                                                     $2,
                                                     driver.createName(NameAST::Table, $3) );
      }
    | T_IDENTIFIER T_DOT    /* +1 shift/reduce conflict */
      {
        $$ = driver.makeAstNode<TableIdentifierAST>( driver.createName(NameAST::Schema, $1),
                                                     $2,
                                                     (NameAST *) 0 );
        driver.error($2, "Table Identifier Expected");
      }
    | T_IDENTIFIER
      {
        $$ = driver.makeAstNode<TableIdentifierAST>( (NameAST *) 0,
                                                     0,
                                                     driver.createName(NameAST::Table, $1) );
      }
    ;

index_identifier
    : T_IDENTIFIER T_DOT T_IDENTIFIER
      {
        $$ = driver.makeAstNode<IndexIdentifierAST>( driver.createName(NameAST::Schema, $1),
                                                     $2,
                                                     driver.createName(NameAST::Index, $3) );
      }
    | T_IDENTIFIER T_DOT
      {
        $$ = driver.makeAstNode<IndexIdentifierAST>( driver.createName(NameAST::Schema, $1),
                                                     $2,
                                                     (NameAST *) 0 );
        driver.error($2, "Index Identifier Expected");
      }
    | T_IDENTIFIER
      {
        $$ = driver.makeAstNode<IndexIdentifierAST>( (NameAST *) 0,
                                                     0,
                                                     driver.createName(NameAST::Index, $1) );
      }
    ;

trigger_identifier
    : T_IDENTIFIER T_DOT T_IDENTIFIER
      {
        $$ = driver.makeAstNode<TriggerIdentifierAST>( driver.createName(NameAST::Schema, $1),
                                                       $2,
                                                       driver.createName(NameAST::Trigger, $3) );
      }
    | T_IDENTIFIER T_DOT
      {
        $$ = driver.makeAstNode<TriggerIdentifierAST>( driver.createName(NameAST::Schema, $1),
                                                       $2,
                                                       (NameAST *) 0 );
        driver.error($2, "Trigger Identifier Expected");
      }
    | T_IDENTIFIER
      {
        $$ = driver.makeAstNode<TriggerIdentifierAST>( (NameAST *) 0,
                                                       0,
                                                       driver.createName(NameAST::Trigger, $1) );
      }
    ;

func_ident
    : T_IDENTIFIER T_DOT T_IDENTIFIER T_DOT T_IDENTIFIER
    {
      $$ = driver.makeAstNode<FunctionIdentifierAST>( driver.createName(NameAST::Schema, $1),
                                                      $2,
                                                      driver.createName(NameAST::Package, $3),
                                                      $4,
                                                      driver.createName(NameAST::Routine, $3) );
    }
    | T_IDENTIFIER T_DOT T_IDENTIFIER T_DOT
      {
        $$ = driver.makeAstNode<FunctionIdentifierAST>( driver.createName(NameAST::Schema, $1),
                                                        $2,
                                                        driver.createName(NameAST::Package, $3),
                                                        $4,
                                                        (NameAST *) 0 );
        driver.error($4, "Identifier Expected");
      }
    | T_IDENTIFIER T_DOT T_IDENTIFIER
      {
        $$ = driver.makeAstNode<FunctionIdentifierAST>( (NameAST *) 0,
                                                        0,
                                                        driver.createName(NameAST::Package, $1),
                                                        $2,
                                                        driver.createName(NameAST::Routine, $3) );
      }
    | T_IDENTIFIER T_DOT
      {
        $$ = driver.makeAstNode<FunctionIdentifierAST>( (NameAST *) 0,
                                                        0,
                                                        driver.createName(NameAST::Package, $1),
                                                        $2,
                                                        (NameAST *) 0 );
        driver.error($2, "Identifier Expected");
      }
    | T_IDENTIFIER
      {
        $$ = driver.makeAstNode<FunctionIdentifierAST>( (NameAST *) 0,
                                                        0,
                                                        (NameAST *) 0,
                                                        0,
                                                        driver.createName(NameAST::Routine, $1) );
      }
    ;

binary_cond_op
    : T_AND
    | T_OR
    ;

conditional_expression
    : conditional_term
    | conditional_expression binary_cond_op conditional_term
      {
        $$ = driver.makeAstNode<BinaryConditionExpressionAST>( $1->asConditionExpression(),
                                                               $2,
                                                               $3->asConditionExpression() );
      }
    ;

conditional_term
    : conditional_primary
    | T_NOT conditional_primary
      {
        $$ = driver.makeAstNode<NotConditionExpressionAST>( $1,
                                                            $2->asConditionExpression() );
      }
    ;

compare_op
    : T_EQUAL
    | T_LESS
    | T_GREATER
    | T_EXCLAIM_EQUAL
    | T_LESS_GREATER
    | T_LESS_EQUAL
    | T_GREATER_EQUAL
    ;

between_expression
    : value_expression T_NOT T_BETWEEN value_expression T_AND value_expression
      {
        $$ = driver.makeAstNode<BetweenConditionExpressionAST>( $1->asValueExpression(),
                                                                $2,
                                                                $3,
                                                                $4->asValueExpression(),
                                                                $5,
                                                                $6->asValueExpression() );
      }
    | value_expression T_NOT T_BETWEEN value_expression T_AND
      {
        $$ = driver.makeAstNode<BetweenConditionExpressionAST>( $1->asValueExpression(),
                                                                $2,
                                                                $3,
                                                                $4->asValueExpression(),
                                                                $5,
                                                                (ValueExpressionAST *) 0 );
      }
    | value_expression       T_BETWEEN value_expression T_AND value_expression
      {
        $$ = driver.makeAstNode<BetweenConditionExpressionAST>( $1->asValueExpression(),
                                                                0,
                                                                $2,
                                                                $3->asValueExpression(),
                                                                $4,
                                                                $5->asValueExpression() );
      }
    | value_expression       T_BETWEEN value_expression T_AND
      {
        $$ = driver.makeAstNode<BetweenConditionExpressionAST>( $1->asValueExpression(),
                                                                0,
                                                                $2,
                                                                $3->asValueExpression(),
                                                                $4,
                                                                (ValueExpressionAST *) 0 );
      }
    ;

value_expression_set
    : T_LEFT_PAREN sel_statement T_RIGHT_PAREN
      { $$ = driver.makeAstNode<SubQueryExpressionAST>($1, $2->asSelectStatement(), $3); }
    | T_LEFT_PAREN comma_expression_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<ValueListExpressionAST>($1, $2, $3); }
    /*| value_expression*/
    ;

in_expression
    : value_expression T_NOT T_IN value_expression_set
      {
        $$ = driver.makeAstNode<InConditionExpressionAST>( $1->asValueExpression(),
                                                           $2,
                                                           $3,
                                                           $4->asCompoundRValueExpression() );
      }
    | value_expression       T_IN value_expression_set
      {
        $$ = driver.makeAstNode<InConditionExpressionAST>( $1->asValueExpression(),
                                                           0,
                                                           $2,
                                                           $3->asCompoundRValueExpression() );
      }
    ;

exists_expression
    : T_EXISTS T_LEFT_PAREN sel_statement T_RIGHT_PAREN
      {
        $$ = driver.makeAstNode<ExistsConditionExpressionAST>( $1,
                                                               driver.makeAstNode<SubQueryExpressionAST>( $2,
                                                                                                          $3->asSelectStatement(),
                                                                                                          $4 ) );
      }
    ;

null_comparison_expression
    : value_expression T_IS T_NOT T_NULL
      {
        $$ = driver.makeAstNode<IsNullConditionExpressionAST>( $1->asValueExpression(),
                                                               $2,
                                                               $3,
                                                               $4 );
      }
    | value_expression T_IS T_NOT
      {
        $$ = driver.makeAstNode<IsNullConditionExpressionAST>( $1->asValueExpression(),
                                                               $2,
                                                               $3,
                                                               0 );
      }
    | value_expression T_IS       T_NULL
      {
        $$ = driver.makeAstNode<IsNullConditionExpressionAST>( $1->asValueExpression(),
                                                               $2,
                                                               0,
                                                               $3 );
      }
    | value_expression T_IS
      {
        $$ = driver.makeAstNode<IsNullConditionExpressionAST>( $1->asValueExpression(),
                                                               $2,
                                                               0,
                                                               0 );
      }
    ;

conditional_primary
    : T_LEFT_PAREN conditional_expression T_RIGHT_PAREN
      {
        $$ = driver.makeAstNode<CompoundConditionExpressionAST>( $1,
                                                                 $2->asConditionExpression(),
                                                                 $3 );
      }
    | value_expression compare_op value_expression
      {
        $$ = driver.makeAstNode<CompareConditionExpressionAST>( $1->asValueExpression(),
                                                                $2,
                                                                $3->asValueExpression() );
      }
    | value_expression T_NOT T_LIKE value_expression
      {
        $$ = driver.makeAstNode<LikeConditionExpressionAST>( $1->asValueExpression(),
                                                             $2,
                                                             $3,
                                                             $4->asValueExpression() );
      }
    | value_expression T_LIKE value_expression
      {
        $$ = driver.makeAstNode<LikeConditionExpressionAST>( $1->asValueExpression(),
                                                             0,
                                                             $2,
                                                             $3->asValueExpression() );
      }
    | between_expression
    | in_expression
    | exists_expression
    | null_comparison_expression
    | T_TRUE            { $$ = driver.makeAstNode<TrueConditionExpressionAST>($1); }
    | T_FALSE           { $$ = driver.makeAstNode<FalseConditionExpressionAST>($1); }
    | value_expression  /* +1 shift/reduce conflict */
      {
        $$ = driver.makeAstNode<ConditionAsValueExpressionAST>( $1->asValueExpression() );
        driver.error($1, "Invalid Conditional Expression");
      }
    ;

empty_aka_optional
    : /* empty */ { $$ = driver.makeAstNode<EmptyAST>(); }
    ;

%%


void Postgres::PostgresParser::error( const std::string &err_message )
{
   std::cerr << "Error: " << err_message << "\n";
}
