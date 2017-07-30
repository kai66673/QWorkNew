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
   #include <QString>
   #include <QDebug>

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
   OptRoleListAST               *optRoleList;
   RoleIdListAST                *roleIdList;
   VarValueListAST              *varValueList;
   TransactionModeListAST       *transModeList;
   NameListAST                  *nameList;
   AlterTableCmdListAST         *alterCmdList;
   ArrBoundListAST              *arrBoundsList;
   GenericOptionListAST         *optionList;
   ExpressionListAST            *exprList;
   TypeListAST                  *typeList;
   IndirectionListAST           *indirectList;
   OrderExprListAST             *orderList;
   ColumnExpressionListAST      *colExprList;
   CommonTableExprListAST       *tabExprList;
   LockingExprListAST           *lockSpecList;
   TableIdentifierListAST       *tableIdentList;
   IndexIdentifierListAST       *indexIdentList;
   SequenceIdentifierListAST    *seqIdentList;
   SelectTargetListAST          *selTargetList;
   TableRefListAST              *tableRefList;
   FunctionArgExprListAST       *funcArgList;
   XmlAttributeListAST          *xmlAttrList;
   TableFuncElementListAST      *funcElemList;
   WindowDefinitionListAST      *winDefList;
   CaseWhenClauseListAST        *caseWhenList;
   ArrayExprListAST             *arrExprList;
   ColumnConstraintListAST      *colConstrList;
   DefinitionElemListAST        *defList;
   RelOptionListAST             *relOptList;
   ConstraintAttributeListAST   *constrAttrList;
   ExclusionConstraintListAST   *exclConstrList;
   AlterGenericOptionListAST    *altGenOptList;
   ColumnRefListAST             *columnRefList;
   SetClauseListAST             *updSetList;
   EventTriggerWhenListAST      *evtTriggerList;
   EventTriggerValueListAST     *evtTrgValueList;
   AlterDbOptionListAST         *alterDbOptList;
   DefACLOptionListAST          *defACLOptList;
   GranteeListAST               *granteeList;
   CommonPrivilegeListAST       *privsList;
   UpdateExtensionCmdListAST    *updExtList;
   FunctionArgListAST           *fncArgList;
   FdwOptionListAST             *fdwOptList;
   AlterFunctionOptionListAST   *altFuncOptList;
   SequenceOptionListAST        *seqOptList;
   AlterTypeCommandListAST      *altTypeCmdList;
   AnyIdentifierListAST         *anyIdentList;
   ConstraintIdentifierListAST  *constrIdentList;
   CopyOptionListAST            *copyOptList;
   GenericCopyOptionListAST     *genCopyOptList;
   GenericCopyOptionCmpArgItemListAST  *genCopyArgList;
   CreateExtOptionListAST       *createExtOptList;
   TableElementListAST          *tableElemList;
   TableLikeOptionListAST       *tabLikeOptList;
   CreateFuncOptionListAST      *creFuncOptList;
   FuncArgWithDefaultListAST    *funcArgDefList;
   TableFuncColumnListAST       *tabFuncColList;
   OpClassListAST               *opClassList;
   OpClassDropListAST           *opClassDropList;
   StatementListAST             *stmtList;
   TypedTableElementListAST     *typElemList;
   TriggerEventListAST          *trgEventList;
   TriggerFuncArgListAST        *trgFuncArgList;
   CreateDbOptionListAST        *creDbOptList;
   EnumValueListAST             *enumValList;
   AggrDefinitionListAST        *aggrDefList;
   DoStmtItemListAST            *doStmtItemList;
   ExplainOptionListAST         *explOptList;
   FunctionWithArgTypesListAST  *funcArgTypeList;
   NumericListAST               *numList;
   IndexElementListAST          *indElemList;
   RelationExprListAST          *relExprList;
   VacuumOptionListAST          *vacuumOptList;
}

%token T_EOF    0     "end of file"

/*!TODO! Composite Tokens */
%token <location> T_NULLS_FIRST
%token <location> T_NULLS_LAST
%token <location> T_WITH_TIME
/*!TODO! Parameter As $nnn */
%token <location> T_PARAM

/* Common Tokens */
%token <location> T_IDENTIFIER   "identifier"
%token <location> T_DIGIT       "number constant"
%token <location> T_CHARSTR      "quoted string"
%token <location> T_STRSTR       "double quoted string"

/* Punctuation Tokens */
%token <location> T_AMPER "&"
                  T_CARET "^"
                  T_COLON ":"
                  T_TYPECAST "::"
                  T_ASSIGN ":="
                  T_COMMA ","
                  T_SLASH "/"
                  T_DOT "."
                  T_DOTDOT ".."
                  T_EQUAL "="
                  T_EXCLAIM "!"
                  T_EXCLAIM_EQUAL "!="
                  T_GREATER ">"
                  T_GREATER_EQUAL ">="
                  T_LEFT_BRACE "{"
                  T_LEFT_BRACKET "["
                  T_LESS "<"
                  T_LESS_EQUAL "<="
                  T_LESS_GREATER "<>"
                  T_LEFT_PAREN "("
                  T_DASH "-"
                  T_PERCENT "%"
                  T_PIPE "|"
                  T_PIPE_PIPE "||"
                  T_PLUS "+"
                  T_RIGHT_BRACE "}"
                  T_RIGHT_BRACKET "]"
                  T_RIGHT_PAREN ")"
                  T_SEMICOLON ";"
                  T_STAR "*"
                  T_TILDA "~"

%token <location> T_ABORT T_ABSOLUTE T_ACCESS T_ACTION T_ADD T_ADMIN T_AFTER
                  T_AGGREGATE T_ALL T_ALSO T_ALTER T_ALWAYS T_ANALYSE T_ANALYZE T_AND T_ANY T_ARRAY T_AS T_ASC
                  T_ASSERTION T_ASSIGNMENT T_ASYMMETRIC T_AT T_ATTRIBUTE T_AUTHORIZATION

                  T_BACKWARD T_BEFORE T_BEGIN T_BETWEEN T_BIGINT T_BIGSERIAL T_BINARY T_BIT
                  T_BOOL T_BOOLEAN T_BOTH T_BOX T_BY T_BYTEA

                  T_CACHE T_CALLED T_CASCADE T_CASCADED T_CASE T_CAST T_CATALOG T_CHAIN T_CHAR
                  T_CHARACTER T_CHARACTERISTICS T_CHECK T_CHECKPOINT T_CIDR T_CIRCLE T_CLASS T_CLOSE
                  T_CLUSTER T_COALESCE T_COLLATE T_COLLATION T_COLUMN T_COMMENT T_COMMENTS T_COMMIT
                  T_COMMITTED T_CONCURRENTLY T_CONFIGURATION T_CONNECTION T_CONSTRAINT T_CONSTRAINTS
                  T_CONTENT T_CONTINUE T_CONVERSION T_COPY T_COST T_CREATE T_COUNT
                  T_CROSS T_CSV T_CURRENT
                  T_CURRENT_CATALOG T_CURRENT_DATE T_CURRENT_ROLE T_CURRENT_SCHEMA
                  T_CURRENT_TIME T_CURRENT_TIMESTAMP T_CURRENT_USER T_CURSOR T_CYCLE

                  T_DATA T_DATABASE T_DATE T_DAY T_DEALLOCATE T_DEC T_DECIMAL T_DECLARE T_DEFAULT T_DEFAULTS
                  T_DEFERRABLE T_DEFERRED T_DEFINER T_DELETE T_DELIMITER T_DELIMITERS T_DESC
                  T_DICTIONARY T_DISABLE T_DISCARD T_DISTINCT T_DO T_DOCUMENT T_DOMAIN T_DOUBLE T_DROP

                  T_EACH T_ELSE T_ENABLE T_ENCODING T_ENCRYPTED T_END T_ENUM T_ESCAPE T_EVENT T_EXCEPT
                  T_EXCLUDE T_EXCLUDING T_EXCLUSIVE T_EXECUTE T_EXISTS T_EXPLAIN
                  T_EXTENSION T_EXTERNAL T_EXTRACT

                  T_FALSE T_FAMILY T_FETCH T_FIRST T_FLOAT T_FLOAT4 T_FLOAT8 T_FOLLOWING T_FOR T_FORCE T_FOREIGN T_FORWARD
                  T_FREEZE T_FROM T_FULL T_FUNCTION T_FUNCTIONS

                  T_GLOBAL T_GRANT T_GRANTED T_GREATEST T_GROUP

                  T_HANDLER T_HAVING T_HEADER T_HOLD T_HOUR

                  T_IDENTITY T_IF T_ILIKE T_IMMEDIATE T_IMMUTABLE T_IMPLICIT T_IN
                  T_INCLUDING T_INCREMENT T_INDEX T_INDEXES T_INET T_INHERIT T_INHERITS T_INITIALLY T_INLINE
                  T_INNER T_INOUT T_INPUT T_INSENSITIVE T_INSERT T_INSTEAD T_INT T_INT2 T_INT4 T_INT8 T_INTEGER
                  T_INTERSECT T_INTERVAL T_INTO T_INVOKER T_IS T_ISNULL T_ISOLATION

                  T_JOIN T_JSON

                  T_KEY

                  T_LABEL T_LANGUAGE T_LARGE T_LAST T_LATERAL T_LC_COLLATE T_LC_CTYPE
                  T_LEADING T_LEAKPROOF T_LEAST T_LEFT T_LEVEL T_LIKE T_LINE T_LIMIT T_LISTEN T_LOAD T_LOCAL
                  T_LOCALTIME T_LOCALTIMESTAMP T_LOCATION T_LOCK T_LSEG

                  T_MACADDR T_MAPPING T_MATCH T_MATERIALIZED T_MAX T_MAXVALUE T_MIN T_MINUTE T_MINVALUE T_MODE T_MONEY T_MONTH T_MOVE

                  T_NAME T_NAMES T_NATIONAL T_NATURAL T_NCHAR T_NEXT T_NO T_NONE
                  T_NOT T_NOTHING T_NOTIFY T_NOTNULL T_NOWAIT T_NULL T_NULLIF
                  T_NULLS T_NUMERIC

                  T_OBJECT T_OF T_OFF T_OFFSET T_OIDS T_ON T_ONLY T_OPERATOR T_OPTION T_OPTIONS T_OR
                  T_ORDER T_OUT T_OUTER T_OVER T_OVERLAPS T_OVERLAY T_OWNED T_OWNER

                  T_PARSER T_PARTIAL T_PARTITION T_PASSING T_PASSWORD T_PATH T_PLACING T_PLANS T_POINT T_POLYGON T_POSITION
                  T_PRECEDING T_PRECISION T_PRESERVE T_PREPARE T_PREPARED T_PRIMARY
                  T_PRIOR T_PRIVILEGES T_PROCEDURAL T_PROCEDURE T_PROGRAM

                  T_QUOTE

                  T_RANGE T_READ T_REAL T_REASSIGN T_RECHECK T_RECURSIVE T_REF T_REFERENCES T_REFRESH T_REINDEX
                  T_RELATIVE T_RELEASE T_RENAME T_REPEATABLE T_REPLACE T_REPLICA
                  T_RESET T_RESTART T_RESTRICT T_RETURNING T_RETURNS T_REVOKE T_RIGHT T_ROLE T_ROLLBACK
                  T_ROW T_ROWS T_RULE

                  T_SAVEPOINT T_SCHEMA T_SCROLL T_SEARCH T_SECOND T_SECURITY T_SELECT T_SEQUENCE T_SEQUENCES
                  T_SERIAL T_SERIAL2 T_SERIAL4 T_SERIAL8 T_SERIALIZABLE T_SERVER T_SESSION T_SESSION_USER T_SET T_SETOF T_SHARE
                  T_SHOW T_SIMILAR T_SIMPLE T_SMALLINT T_SMALLSERIAL T_SNAPSHOT T_SOME T_STABLE T_STANDALONE T_START
                  T_STATEMENT T_STATISTICS T_STDIN T_STDOUT T_STORAGE T_STRICT T_STRIP T_SUBSTRING T_SUM
                  T_SYMMETRIC T_SYSID T_SYSTEM

                  T_TABLE T_TABLES T_TABLESPACE T_TEMP T_TEMPLATE T_TEMPORARY T_TEXT T_THEN T_TIME T_TIMESTAMP T_TIMESTAMPTZ
                  T_TIMETZ T_TO T_TRAILING T_TRANSACTION T_TREAT T_TRIGGER T_TRIM T_TRUE
                  T_TRUNCATE T_TRUSTED T_TSQUERY T_TSVECTOR T_TYPE T_TYPES T_TXID_SNAPSHOT

                  T_UNBOUNDED T_UNCOMMITTED T_UNENCRYPTED T_UNION T_UNIQUE T_UNKNOWN T_UNLISTEN T_UNLOGGED
                  T_UNTIL T_UPDATE T_USER T_USING T_UUID

                  T_VACUUM T_VALID T_VALIDATE T_VALIDATOR T_VALUE T_VALUES T_VARBIT T_VARCHAR T_VARIADIC T_VARYING
                  T_VERBOSE T_VERSION T_VIEW T_VOLATILE

                  T_WHEN T_WHERE T_WHITESPACE T_WINDOW T_WITH T_WITHOUT T_WORK T_WRAPPER T_WRITE

                  T_XML T_XMLATTRIBUTES T_XMLCONCAT T_XMLELEMENT T_XMLEXISTS T_XMLFOREST T_XMLPARSE
                  T_XMLPI T_XMLROOT T_XMLSERIALIZE

                  T_YEAR T_YES

                  T_ZONE

%type <astNode>       empty_aka_optional statement_inner
                      CreateRoleStmt CreateOptRoleElem RoleId AlterOptRoleElem CreateUserStmt
                      AlterRoleStmt AlterRoleSetStmt opt_in_database database_name SetResetClause
                      set_rest VariableResetStmt set_rest_more zone_value var_value any_name column_identifier
                      opt_interval transaction_mode_item iso_level interval_second opt_float trigger_identifier
                      AlterUserStmt AlterUserSetStmt AlterGroupStmt CreateGroupStmt
                      DiscardStmt AlterStmt relation_expr alter_relation_expr table_identifier table_identifier_we index_identifier sequence_identifier
                      alter_table_cmd columnDef Typename SimpleTypename GenericType opt_type_modifiers Numeric
                      Bit BitWithLength BitWithoutLength Character CharacterWithLength CharacterWithoutLength
                      character ConstDatetime opt_charset opt_timezone generic_option_elem create_generic_options
                      SelectStmt select_with_parens select_no_parens select_clause simple_select with_clause
                      sort_clause opt_sort_clause for_locking_clause opt_for_locking_clause select_limit opt_select_limit
                      select_limit_value select_offset_value select_offset_value2 opt_select_fetch_first_value
                      values_clause ctext_row ctext_expr common_table_expr PreparableStmt for_locking_item
                      sortby subquery_Op row columnref indirection_el opt_name_list limit_clause offset_clause
                      for_locking_strength locked_rels_list opt_distinct into_clause OptTempTableName target_el
                      from_clause table_ref opt_collate opt_collate_clause alias_clause opt_alias_clause
                      func_expr func_name over_clause func_arg_expr extract_list overlay_list position_list
                      substr_list trim_list func_table func_alias_clause TableConstraint ConstraintElem ConstraintAttributeElem
                      substr_from substr_for xml_attribute_el joined_table join_type join_qual where_clause group_clause having_clause
                      TableFuncElement window_specification opt_partition_clause opt_frame_clause frame_extent frame_bound
                      xml_attributes xmlexists_argument xml_whitespace_option xml_root_version opt_xml_root_standalone window_clause window_definition
                      a_expr b_expr c_expr in_expr any_operator qual_all_Op qual_Op AexprConst ConstTypename
                      case_expr case_arg case_default when_clause array_expr ColConstraint ConstraintAttr
                      ColConstraintElem opt_definition OptConsTableSpace definition def_elem def_arg key_action
                      func_type opt_no_inherit opt_column_list key_match key_actions key_update key_delete ExistingIndex
                      alter_column_default reloptions reloption_elem opt_set_data alter_using alter_generic_options alter_generic_option_elem
                      access_method_clause ExclusionConstraintElem ExclusionWhereClause index_elem opt_class
                      DeleteStmt relation_expr_opt_alias using_clause where_or_current_clause returning_clause opt_with_clause
                      InsertStmt insert_rest insert_column_item UpdateStmt set_clause single_set_clause multiple_set_clause set_target
                      AlterEventTrigStmt CreateEventTrigStmt enable_trigger event_trigger_when_item
                      AlterDatabaseStmt AlterDatabaseSetStmt alterdb_opt_item AlterDefaultPrivilegesStmt
                      DefACLOption DefACLAction privileges opt_grant_grant_option grantee privilege oper_argtypes
                      AlterDomainStmt CreateDomainStmt AlterEnumStmt opt_if_not_exists function_with_argtypes func_arg
                      AlterExtensionStmt AlterExtensionContentsStmt alter_extension_opt_item aggr_args func_args
                      AlterFdwStmt fdw_option handler_name AlterForeignServerStmt foreign_server_version auth_ident
                      AlterForeignTableStmt AlterFunctionStmt common_func_opt_item FunctionSetResetClause SeqOptElem
                      AlterObjectSchemaStmt SetSchemaClause AlterOwnerStmt OwnerToClause AlterSeqStmt CreateSeqStmt OptTemp
                      AlterCompositeTypeStmt alter_type_cmd AlterTSConfigurationStmt AlterTSDictionaryStmt AlterUserMappingStmt
                      AnalyzeStmt CheckPointStmt ClosePortalStmt ClusterStmt cluster_index_specification CommentStmt
                      CommentTextClause CommentObject CommentOnName ConstraintsSetStmt constraint_identifier constraints_set_list opt_oids
                      CopyStmt copy_options copy_opt_item copy_generic_opt_elem copy_generic_opt_arg copy_generic_opt_arg_list_item copy_delimiter
                      CreateTableAsStmt create_as_target opt_with_data OptWith OnCommitOption OptTableSpace CreateAssertStmt
                      CreateCastStmt opt_if_exists cast_context CreateConversionStmt CreateExtensionStmt create_extension_opt_item
                      CreateFdwStmt CreateForeignServerStmt opt_foreign_server_version opt_type CreateForeignTableStmt
                      TableLikeClause TableLikeOption TableElement CreateFunctionStmt opt_or_replace func_args_with_defaults
                      func_return createfunc_opt_item func_as func_arg_with_default table_func_column CreateMatViewStmt
                      opt_reloptions create_mv_target CreateOpClassStmt CreateOpFamilyStmt AlterOpFamilyStmt
                      opt_opfamily opclass_item opclass_purpose opclass_drop schema_stmt
                      CreatePLangStmt opt_inline_handler opt_validator validator_clause CreateSchemaStmt
                      CreateSchemaClause CreateTableStmt CreateTableSpaceStmt CreateTableClause CreateTableOptions OptInherit
                      OptTypedTableElementList TypedTableElement columnOptions OptTableSpaceOwner CreateTrigStmt
                      TriggerActionTime TriggerForSpec TriggerWhen TriggerOneEvent TriggerFuncArg OptConstrFromTable
                      CreateUserMappingStmt CreateDbStmt createdb_opt_item DeallocateStmt DeclareCursorStmt opt_hold
                      DefineStmt old_aggr_definition old_aggr_elem DoStmt dostmt_opt_item
                      ReassignOwnedStmt RuleStmt RuleActionList RuleActionStmt RuleActionStmtOrEmpty DropStmt drop_type
                      ExecuteStmt execute_param_clause ExplainStmt RefreshMatViewStmt ExplainableStmt explain_option_elem
                      FetchStmt fetch_args GrantStmt RevokeStmt privilege_target GrantRoleStmt RevokeRoleStmt
                      opt_grant_admin_option opt_granted_by IndexStmt ListenStmt NotifyStmt UnlistenStmt notify_payload
                      LoadStmt LockStmt opt_lock lock_type PrepareStmt prep_type_clause ReindexStmt
                      RenameStmt SecLabelStmt opt_provider security_label_type TransactionStmt
                      TruncateStmt opt_restart_seqs VacuumStmt vacuum_option_elem VariableSetStmt VariableShowStmt ViewStmt opt_check_option

%type <location>      semicolon_or_eof opt_with Iconst Sconst SignedIconst ColId type_function_name
                      NonReservedWord unreserved_keyword col_name_keyword type_func_name_keyword param_name
                      reserved_keyword ColLabel attr_name name first_or_next row_or_rows opt_table join_outer
                      /*builtin_func_name builtin_type_name*/
                      ConstInterval NonReservedWord_or_Sconst NumericOnly document_or_content access_method
                      opt_boolean_or_string opt_encoding add_drop opt_column opt_drop_behavior opt_varying
                      generic_option_name generic_option_arg opt_asymmetric opt_asc_desc opt_all opt_nowait
                      all_Op Op MathOp sub_type opt_existing_window_name extract_arg columnElem index_name
                      opt_nulls_order cursor_name opt_equal defacl_privilege_target opt_as opt_procedural arg_class
                      opt_restrict opt_by opt_verbose analyze_keyword comment_text constraints_set_mode
                      opt_program opt_binary opt_using copy_file_name copy_from opt_default OptNoLog opt_recheck
                      opt_trusted OptSchemaName TriggerForOptEach TriggerForType opt_instead event
                      explain_option_arg explain_option_name from_in opt_from_in opt_unique opt_concurrently
                      opt_index_name file_name opt_force security_label opt_transaction opt_freeze opt_full
                      drop comment
                      target_schema target_table target_table_list target_sequence target_sequence_list target_view target_view_list target_index target_index_list target_trigger target_column
                      CH_on_before_table_identifier


%type <optRoleList>     OptRoleList AlterOptRoleList
%type <roleIdList>      role_list
%type <varValueList>    var_list
%type <transModeList>   transaction_mode_list transaction_mode_list_or_empty
%type <nameList>        var_name attrs name_list columnList schema_name_list
%type <alterCmdList>    alter_table_cmds
%type <arrBoundsList>   opt_array_bounds
%type <optionList>      generic_option_list
%type <exprList>        expr_list
%type <typeList>        type_list
%type <indirectList>    indirection opt_indirection
%type <orderList>       sortby_list
%type <colExprList>     ctext_expr_list
%type <tabExprList>     cte_list
%type <lockSpecList>    for_locking_items
%type <tableIdentList>  table_identifier_list
%type <indexIdentList>  index_identifier_list
%type <seqIdentList>    sequence_identifier_list
%type <selTargetList>   target_list
%type <tableRefList>    from_list
%type <funcArgList>     func_arg_list
%type <xmlAttrList>     xml_attribute_list
%type <funcElemList>    TableFuncElementList OptTableFuncElementList
%type <winDefList>      window_definition_list
%type <caseWhenList>    when_clause_list
%type <arrExprList>     array_expr_list
%type <colConstrList>   ColQualList
%type <defList>         def_list
%type <relOptList>      reloption_list
%type <constrAttrList>  ConstraintAttributeSpec
%type <exclConstrList>  ExclusionConstraintList
%type <altGenOptList>   alter_generic_option_list
%type <columnRefList>   insert_column_list set_target_list
%type <updSetList>      set_clause_list
%type <evtTriggerList>  event_trigger_when_list
%type <evtTrgValueList> event_trigger_value_list
%type <alterDbOptList>  alterdb_opt_list
%type <defACLOptList>   DefACLOptionList
%type <granteeList>     grantee_list
%type <privsList>       privilege_list
%type <updExtList>      alter_extension_opt_list
%type <fncArgList>      func_args_list
%type <fdwOptList>      fdw_options opt_fdw_options
%type <altFuncOptList>  alterfunc_opt_list
%type <seqOptList>      OptSeqOptList SeqOptList
%type <altTypeCmdList>  alter_type_cmds
%type <anyIdentList>    any_name_list
%type <constrIdentList> constraint_identifier_list
%type <copyOptList>     copy_opt_list
%type <genCopyOptList>  copy_generic_opt_list
%type <genCopyArgList>  copy_generic_opt_arg_list
%type <createExtOptList>create_extension_opt_list
%type <tableElemList>   OptTableElementList TableElementList
%type <tabLikeOptList>  TableLikeOptionList
%type <creFuncOptList>  createfunc_opt_list
%type <funcArgDefList>  func_args_with_defaults_list
%type <tabFuncColList>  table_func_column_list
%type <opClassList>     opclass_item_list
%type <opClassDropList> opclass_drop_list
%type <stmtList>        OptSchemaEltList RuleActionMulti
%type <typElemList>     TypedTableElementList
%type <trgEventList>    TriggerEvents
%type <trgFuncArgList>  TriggerFuncArgs
%type <creDbOptList>    createdb_opt_list
%type <enumValList>     enum_val_list opt_enum_val_list
%type <aggrDefList>     old_aggr_list
%type <doStmtItemList>  dostmt_opt_list
%type <explOptList>     explain_option_list
%type <funcArgTypeList> function_with_argtypes_list
%type <numList>         NumericOnly_list
%type <indElemList>     index_params
%type <relExprList>     relation_expr_list
%type <vacuumOptList>   vacuum_option_list

/* Precedence: lowest to highest */
%nonassoc T_SET
%left     T_UNION T_EXCEPT
%left     T_INTERSECT
%left     T_OR
%left     T_AND
%right    T_NOT
%right    T_EQUAL T_LESS_GREATER T_EXCLAIM T_EXCLAIM_EQUAL
%nonassoc T_LESS T_LESS_EQUAL T_GREATER T_GREATER_EQUAL
%nonassoc T_LIKE T_ILIKE T_SIMILAR
%nonassoc T_ESCAPE
%nonassoc T_OVERLAPS
%nonassoc T_BETWEEN
%nonassoc T_IN
%left     POSTFIXOP               /* dummy for postfix Op rules */
%nonassoc T_UNBOUNDED
%nonassoc T_IDENTIFIER T_NULL T_PARTITION T_RANGE T_ROWS T_PRECEDING T_FOLLOWING
%left     Op T_OPERATOR
%nonassoc T_NOTNULL
%nonassoc T_ISNULL
%nonassoc T_IS
%left     T_PLUS T_DASH T_PIPE_PIPE
%left     T_STAR T_SLASH T_PERCENT
%left     T_CARET
/* Unary Operators */
%left     T_AT
%left     T_COLLATE
%right    UMINUS                  /* dummy for unary '+' and '-' */
%left     T_LEFT_BRACKET T_RIGHT_BRACKET
%left     T_LEFT_PAREN T_RIGHT_PAREN
%left     T_TYPECAST
%left     T_DOT
%left     T_JOIN T_CROSS T_LEFT T_FULL T_RIGHT T_INNER T_NATURAL
/* kluge to keep xml_whitespace_option from causing shift/reduce conflicts */
%right    T_PRESERVE T_STRIP

%start start

%%

start:
      statement_list T_EOF
    ;

semicolon_or_eof:
      T_SEMICOLON { $$ = $1; }
    | T_EOF       { $$ = 0; }
    ;

statement_list:
      statement
    | statement_list statement
    ;

statement:
      statement_inner semicolon_or_eof  { driver.addStatement( $1->asStatement() ); }
    | error semicolon_or_eof            { driver.addUnknownStatement(); }
    ;

empty_aka_optional:
      /* empty */ { $$ = driver.makeAstNode<EmptyAST>(); }
    ;

statement_inner:
      AlterEventTrigStmt
    | AlterDatabaseStmt
    | AlterDatabaseSetStmt
    | AlterDefaultPrivilegesStmt
    | AlterDomainStmt
    | AlterEnumStmt
    | AlterExtensionStmt
    | AlterExtensionContentsStmt
    | AlterFdwStmt
    | AlterForeignServerStmt
    | AlterForeignTableStmt
    | AlterFunctionStmt
    | AlterGroupStmt
    | AlterObjectSchemaStmt
    | AlterOpFamilyStmt
    | AlterOwnerStmt
    | AlterRoleStmt
    | AlterRoleSetStmt
    | AlterSeqStmt
    | AlterStmt
    | AlterTSConfigurationStmt
    | AlterTSDictionaryStmt
    | AlterCompositeTypeStmt
    | AlterUserMappingStmt
    | AlterUserStmt
    | AlterUserSetStmt
    | AnalyzeStmt
    | CheckPointStmt
    | ClosePortalStmt
    | ClusterStmt
    | CommentStmt
    | ConstraintsSetStmt
    | CopyStmt
    | CreateAssertStmt
    | CreateCastStmt
    | CreateConversionStmt
    | CreateDbStmt
    | CreateDomainStmt
    | CreateExtensionStmt
    | CreateFdwStmt
    | CreateForeignServerStmt
    | CreateForeignTableStmt
    | CreateFunctionStmt
    | CreateGroupStmt
    | CreateMatViewStmt
    | CreateOpClassStmt
    | CreateOpFamilyStmt
    | CreatePLangStmt
    | CreateSchemaStmt
    | CreateRoleStmt
    | CreateSeqStmt
    | CreateTableStmt
    | CreateTableAsStmt
    | CreateTableSpaceStmt
    | CreateTrigStmt
    | CreateEventTrigStmt
    | CreateUserStmt
    | CreateUserMappingStmt
    | DeallocateStmt
    | DeclareCursorStmt
    | DefineStmt
    | DeleteStmt
    | DiscardStmt
    | DoStmt
    | DropStmt
    | ExecuteStmt
    | ExplainStmt
    | FetchStmt
    | GrantStmt
    | GrantRoleStmt
    | IndexStmt
    | InsertStmt
    | ListenStmt
    | LoadStmt
    | LockStmt
    | NotifyStmt
    | PrepareStmt
    | ReassignOwnedStmt
    | RefreshMatViewStmt
    | ReindexStmt
    | RenameStmt
    | RevokeStmt
    | RevokeRoleStmt
    | RuleStmt
    | SecLabelStmt
    | SelectStmt
    | TransactionStmt
    | TruncateStmt
    | UnlistenStmt
    | UpdateStmt
    | VacuumStmt
    | VariableResetStmt
    | VariableSetStmt
    | VariableShowStmt
    | ViewStmt
    /* Unknown Statement (aka Error) */
    ;


/*****************************************************************************
 *    Statemant <TARGET>
 *****************************************************************************/

 target_schema:
       T_SCHEMA     { driver.currentStatementInfo()->target = StatementAST::Info::Schema; }
     ;

target_table:
      T_TABLE       { driver.appendObjectIdentifierHelper(StatementAST::CompletionHelper::TableIdentifier, $1 + 1); }
    ;

target_table_list:
      T_TABLE       { driver.currentStatementInfo()->target = StatementAST::Info::Table; }
    ;

target_column:
      T_COLUMN      { driver.currentStatementInfo()->target = StatementAST::Info::Column; }
    ;

target_view:
      T_VIEW        { driver.appendObjectIdentifierHelper(StatementAST::CompletionHelper::ViewIdentifier, $1 + 1); }
    ;

target_view_list:
      T_VIEW        { driver.currentStatementInfo()->target = StatementAST::Info::View; }
    ;

target_index:
      T_INDEX       { driver.appendObjectIdentifierHelper(StatementAST::CompletionHelper::IndexIdentifier, $1 + 1); }
    ;

target_index_list:
      T_INDEX       { driver.currentStatementInfo()->target = StatementAST::Info::Index; }
    ;

target_trigger:
      T_TRIGGER     { driver.currentStatementInfo()->target = StatementAST::Info::Trigger; }
    ;

target_sequence:
       T_SEQUENCE    { driver.appendObjectIdentifierHelper(StatementAST::CompletionHelper::SequenceIdentifier, $1 + 1); }
     ;

target_sequence_list:
       T_SEQUENCE    { driver.currentStatementInfo()->target = StatementAST::Info::Sequence; }
     ;

 /*****************************************************************************
 *    QUERY:
 *        CREATE [ OR REPLACE ] [ TEMP ] VIEW <viewname> '('target-list ')'
 *          AS <query> [ WITH [ CASCADED | LOCAL ] CHECK OPTION ]
 *****************************************************************************/

ViewStmt:
      T_CREATE                OptTemp             T_VIEW table_identifier opt_column_list                       opt_reloptions T_AS SelectStmt opt_check_option
      {
        $$ = driver.makeAstNode<CreateViewStmtAST>( $1,
                                                    0, 0,
                                                    $2->asTempClause(),
                                                    0,
                                                    $3,
                                                    $4->asTableIdentifier(),
                                                    $5->asRefColumns(),
                                                    $6->asWithReloptions(),
                                                    $7,
                                                    $8->asSelectStmt(),
                                                    $9->asWithCheckOptionClause() );
      }
    | T_CREATE T_OR T_REPLACE OptTemp             T_VIEW table_identifier opt_column_list                       opt_reloptions T_AS SelectStmt opt_check_option
      {
        $$ = driver.makeAstNode<CreateViewStmtAST>( $1,
                                                    $2, $3,
                                                    $4->asTempClause(),
                                                    0,
                                                    $5,
                                                    $6->asTableIdentifier(),
                                                    $7->asRefColumns(),
                                                    $8->asWithReloptions(),
                                                    $9,
                                                    $10->asSelectStmt(),
                                                    $11->asWithCheckOptionClause() );
      }
    | T_CREATE                OptTemp T_RECURSIVE T_VIEW table_identifier T_LEFT_PAREN columnList T_RIGHT_PAREN opt_reloptions T_AS SelectStmt
      {
        $$ = driver.makeAstNode<CreateViewStmtAST>( $1,
                                                    0, 0,
                                                    $2->asTempClause(),
                                                    $3,
                                                    $4,
                                                    $5->asTableIdentifier(),
                                                    driver.makeAstNode<RefColumnsAST>( $6, $7, $8 ),
                                                    $9->asWithReloptions(),
                                                    $10,
                                                    $11->asSelectStmt(),
                                                    (WithCheckOptionClauseAST *) 0 );
      }
    | T_CREATE T_OR T_REPLACE OptTemp T_RECURSIVE T_VIEW table_identifier T_LEFT_PAREN columnList T_RIGHT_PAREN opt_reloptions T_AS SelectStmt
      {
        $$ = driver.makeAstNode<CreateViewStmtAST>( $1,
                                                    $2, $3,
                                                    $4->asTempClause(),
                                                    $5,
                                                    $6,
                                                    $7->asTableIdentifier(),
                                                    driver.makeAstNode<RefColumnsAST>( $8, $9, $10 ),
                                                    $11->asWithReloptions(),
                                                    $12,
                                                    $13->asSelectStmt(),
                                                    (WithCheckOptionClauseAST *) 0 );
      }
    ;

opt_check_option:
      T_WITH            T_CHECK T_OPTION
      { $$ = driver.makeAstNode<WithCheckOptionClauseAST>( $1,  0, $2, $3 ); }
    | T_WITH T_CASCADED T_CHECK T_OPTION
      { $$ = driver.makeAstNode<WithCheckOptionClauseAST>( $1, $2, $3, $4 ); }
    | T_WITH T_LOCAL    T_CHECK T_OPTION
      { $$ = driver.makeAstNode<WithCheckOptionClauseAST>( $1, $2, $3, $4 ); }
    | empty_aka_optional
    ;

/*****************************************************************************
 * Set PG internal variable
 *    SET name TO 'var_value'
 *    SHOW ...
 *****************************************************************************/

VariableSetStmt:
      T_SET set_rest
      { $$ = driver.makeAstNode<VariableSetStmtAST>( $1,  0, $2->asSetRest() ); }
    | T_SET T_LOCAL set_rest
      { $$ = driver.makeAstNode<VariableSetStmtAST>( $1, $2, $3->asSetRest() ); }
    | T_SET T_SESSION set_rest
      { $$ = driver.makeAstNode<VariableSetStmtAST>( $1, $2, $3->asSetRest() ); }
    ;

VariableShowStmt:
      T_SHOW var_name
      { $$ = driver.makeAstNode<VarShowStmtAST>( $1, $2 ); }
    | T_SHOW T_TIME T_ZONE
      { $$ = driver.makeAstNode<TimeZoneShowStmtAST>( $1, $2, $3 ); }
    | T_SHOW T_TRANSACTION T_ISOLATION T_LEVEL
      { $$ = driver.makeAstNode<IsoLevelShowStmtAST>( $1, $2, $3, $4 ); }
    | T_SHOW T_SESSION T_AUTHORIZATION
      { $$ = driver.makeAstNode<SessionAuthShowStmtAST>( $1, $2, $3 ); }
    | T_SHOW T_ALL
      { $$ = driver.makeAstNode<AllShowStmtAST>( $1, $2 ); }
    ;

/*****************************************************************************
 *    QUERY: VACUUM
 *****************************************************************************/

VacuumStmt:
      T_VACUUM opt_full opt_freeze opt_verbose
      { $$ = driver.makeAstNode<VacuumTableStmtAST>( $1, $2, $3, $4, (TableIdentifierAST *) 0 ); }
    | T_VACUUM opt_full opt_freeze opt_verbose table_identifier
      { $$ = driver.makeAstNode<VacuumTableStmtAST>( $1, $2, $3, $4, $5->asTableIdentifier() ); }
    | T_VACUUM opt_full opt_freeze opt_verbose AnalyzeStmt
      { $$ = driver.makeAstNode<VacuumAnalyzeStmtAST>( $1, $2, $3, $4, $5->asAnalyzeStmt() ); }
    | T_VACUUM T_LEFT_PAREN vacuum_option_list T_RIGHT_PAREN
      {
        $$ = driver.makeAstNode<CommonVacuumStmtAST>( $1, $2, $3, $4,
                                                      (TableIdentifierAST *) 0, (CompoundColumnNamesAST *) 0 );
      }
    | T_VACUUM T_LEFT_PAREN vacuum_option_list T_RIGHT_PAREN table_identifier opt_name_list
      {
        $$ = driver.makeAstNode<CommonVacuumStmtAST>( $1, $2, $3, $4,
                                                      $5->asTableIdentifier(), $6->asCompoundColumnNames() );
      }
    ;

vacuum_option_list:
      vacuum_option_elem
      { $$ = driver.makeAstNode<VacuumOptionListAST>( $1->asVacuumOption() ); }
    | vacuum_option_list T_COMMA vacuum_option_elem
      { $$ = driver.makeAstNode<VacuumOptionListAST>( $1, $3->asVacuumOption() ); }
    ;

vacuum_option_elem:
      analyze_keyword
      { $$ = driver.makeAstNode<VacuumOptionAST>( $1 ); }
    | T_VERBOSE
      { $$ = driver.makeAstNode<VacuumOptionAST>( $1 ); }
    | T_FREEZE
      { $$ = driver.makeAstNode<VacuumOptionAST>( $1 ); }
    | T_FULL
      { $$ = driver.makeAstNode<VacuumOptionAST>( $1 ); }
    ;

opt_freeze:
      T_FREEZE
    | /*EMPTY*/ { $$ = 0; }
    ;

opt_full:
    T_FULL
    | /*EMPTY*/ { $$ = 0; }
    ;

/*****************************************************************************
 *    QUERY:
 *        truncate table relname1, relname2, ...
 *****************************************************************************/

TruncateStmt:
      T_TRUNCATE opt_table relation_expr_list opt_restart_seqs opt_drop_behavior
      { $$ = driver.makeAstNode<TruncateStmtAST>( $1, $2, $3, $4->asTruncateRestartClause(), $5 ); }
    ;

opt_restart_seqs:
      T_CONTINUE T_IDENTITY
      { $$ = driver.makeAstNode<TruncateRestartClauseAST>( $1, $2 ); }
    | T_RESTART T_IDENTITY
      { $$ = driver.makeAstNode<TruncateRestartClauseAST>( $1, $2 ); }
    | empty_aka_optional
    ;

/*****************************************************************************
 *    Transactions:
 *        BEGIN / COMMIT / ROLLBACK
 *        (also older versions END / ABORT)
 *****************************************************************************/

TransactionStmt:
      T_BEGIN opt_transaction transaction_mode_list_or_empty
      { $$ = driver.makeAstNode<BeginTransactionStmtAST>( $1, $2, $3 ); }
    | T_START T_TRANSACTION transaction_mode_list_or_empty
      { $$ = driver.makeAstNode<BeginTransactionStmtAST>( $1, $2, $3 ); }
    | T_SAVEPOINT ColId
      { $$ = driver.makeAstNode<SavepointTransactionStmtAST>( $1, $2 ); }
    | T_RELEASE T_SAVEPOINT ColId
      { $$ = driver.makeAstNode<ReleaseTransactionStmtAST>( $1, $2, $3 ); }
    | T_RELEASE ColId
      { $$ = driver.makeAstNode<ReleaseTransactionStmtAST>( $1,  0, $2 ); }
    | T_COMMIT opt_transaction
      { $$ = driver.makeAstNode<CommitTransactionStmtAST>( $1, $2 ); }
    | T_END opt_transaction
      { $$ = driver.makeAstNode<CommitTransactionStmtAST>( $1, $2 ); }
    | T_ABORT opt_transaction
      { $$ = driver.makeAstNode<RollbackTransactionStmtAST>( $1, $2,  0,  0,  0 ); }
    | T_ROLLBACK opt_transaction
      { $$ = driver.makeAstNode<RollbackTransactionStmtAST>( $1, $2,  0,  0,  0 ); }
    | T_ROLLBACK opt_transaction T_TO T_SAVEPOINT ColId
      { $$ = driver.makeAstNode<RollbackTransactionStmtAST>( $1, $2, $3, $4, $5 ); }
    | T_ROLLBACK opt_transaction T_TO ColId
      { $$ = driver.makeAstNode<RollbackTransactionStmtAST>( $1, $2, $3,  0, $4 ); }
    | T_PREPARE T_TRANSACTION Sconst
      { $$ = driver.makeAstNode<PrepareTransactionStmtAST>( $1, $2, $3 ); }
    | T_COMMIT T_PREPARED Sconst
      { $$ = driver.makeAstNode<CommitPrepTransactionStmtAST>( $1, $2, $3 ); }
    | T_ROLLBACK T_PREPARED Sconst
      { $$ = driver.makeAstNode<RollPrepTransactionStmtAST>( $1, $2, $3 ); }
    ;

opt_transaction:
      T_WORK
    | T_TRANSACTION
    | /*EMPTY*/ { $$ = 0; }
    ;

transaction_mode_list_or_empty:
      transaction_mode_list
    | /* EMPTY */ { $$ = 0; }
    ;

/*****************************************************************************
 *  SECURITY LABEL [FOR <provider>] ON <object> IS <label>
 *    As with COMMENT ON, <object> can refer to various types of database
 *    objects (e.g. TABLE, COLUMN, etc.).
 *****************************************************************************/

SecLabelStmt:
      T_SECURITY T_LABEL opt_provider T_ON security_label_type any_name T_IS security_label
      {
        $$ = driver.makeAstNode<CommonSecLabelStmtAST>( $1, $2,
                                                        $3->asSecurityProvider(),
                                                        $4,
                                                        $5->asCommonSecurityType(),
                                                        $6->asAnyIdentifier(),
                                                        $7,
                                                        $8 );
      }
    | T_SECURITY T_LABEL opt_provider T_ON T_COLUMN column_identifier T_IS security_label
      {
        $$ = driver.makeAstNode<ColSecLabelStmtAST>( $1, $2,
                                                     $3->asSecurityProvider(),
                                                     $4,
                                                     $5,
                                                     $6->asColumnIdentifier(),
                                                     $7,
                                                     $8 );
      }
    | T_SECURITY T_LABEL opt_provider T_ON T_SCHEMA name T_IS security_label
      {
        $$ = driver.makeAstNode<SchemaSecLabelStmtAST>( $1, $2,
                                                        $3->asSecurityProvider(),
                                                        $4,
                                                        $5,
                                                        driver.createName(NameAST::Schema, $6),
                                                        $7,
                                                        $8 );
      }
    | T_SECURITY T_LABEL opt_provider T_ON T_ROLE RoleId T_IS security_label
      {
        $$ = driver.makeAstNode<RoleSecLabelStmtAST>( $1, $2,
                                                      $3->asSecurityProvider(),
                                                      $4,
                                                      $5,
                                                      $6->asRoleId(),
                                                      $7,
                                                      $8 );
      }
    | T_SECURITY T_LABEL opt_provider T_ON T_SEQUENCE sequence_identifier T_IS security_label
      {
        $$ = driver.makeAstNode<SequenceSecLabelStmtAST>( $1, $2,
                                                          $3->asSecurityProvider(),
                                                          $4,
                                                          $5,
                                                          $6->asSequenceIdentifier(),
                                                          $7,
                                                          $8 );
      }
    | T_SECURITY T_LABEL opt_provider T_ON T_FOREIGN      T_TABLE table_identifier T_IS security_label
      {
        $$ = driver.makeAstNode<TableSecLabelStmtAST>( $1, $2,
                                                       $3->asSecurityProvider(),
                                                       $4,
                                                       $5,
                                                       $6,
                                                       $7->asTableIdentifier(),
                                                       $8,
                                                       $9 );
      }
    | T_SECURITY T_LABEL opt_provider T_ON                T_TABLE table_identifier T_IS security_label
      {
        $$ = driver.makeAstNode<TableSecLabelStmtAST>( $1, $2,
                                                       $3->asSecurityProvider(),
                                                       $4,
                                                       0,
                                                       $5,
                                                       $6->asTableIdentifier(),
                                                       $7,
                                                       $8 );
      }
    | T_SECURITY T_LABEL opt_provider T_ON T_MATERIALIZED T_VIEW  table_identifier T_IS security_label
      {
        $$ = driver.makeAstNode<TableSecLabelStmtAST>( $1, $2,
                                                       $3->asSecurityProvider(),
                                                       $4,
                                                       $5,
                                                       $6,
                                                       $7->asTableIdentifier(),
                                                       $8,
                                                       $9 );
      }
    | T_SECURITY T_LABEL opt_provider T_ON                T_VIEW  table_identifier T_IS security_label
      {
        $$ = driver.makeAstNode<TableSecLabelStmtAST>( $1, $2,
                                                       $3->asSecurityProvider(),
                                                       $4,
                                                       0,
                                                       $5,
                                                       $6->asTableIdentifier(),
                                                       $7,
                                                       $8 );
      }
    | T_SECURITY T_LABEL opt_provider T_ON T_AGGREGATE func_name aggr_args T_IS security_label
      {
        $$ = driver.makeAstNode<AggrSecLabelStmtAST>( $1, $2,
                                                      $3->asSecurityProvider(),
                                                      $4,
                                                      $5,
                                                      $6->asFunctionRef(),
                                                      $7->asAggrArguments(),
                                                      $8,
                                                      $9 );
      }
    | T_SECURITY T_LABEL opt_provider T_ON T_FUNCTION func_name func_args T_IS security_label
      {
        $$ = driver.makeAstNode<FuncSecLabelStmtAST>( $1, $2,
                                                      $3->asSecurityProvider(),
                                                      $4,
                                                      $5,
                                                      $6->asFunctionRef(),
                                                      $7->asFunctionArgTypes(),
                                                      $8,
                                                      $9 );
      }
    | T_SECURITY T_LABEL opt_provider T_ON T_LARGE T_OBJECT NumericOnly T_IS security_label
      {
        $$ = driver.makeAstNode<LargeObjSecLabelStmtAST>( $1, $2,
                                                          $3->asSecurityProvider(),
                                                          $4,
                                                          $5,
                                                          $6,
                                                          $7,
                                                          $8,
                                                          $9 );
      }
    | T_SECURITY T_LABEL opt_provider T_ON opt_procedural T_LANGUAGE any_name T_IS security_label
      {
        $$ = driver.makeAstNode<LangSecLabelStmtAST>( $1, $2,
                                                      $3->asSecurityProvider(),
                                                      $4,
                                                      $5,
                                                      $6,
                                                      $7->asAnyIdentifier(),
                                                      $8,
                                                      $9 );
      }
    ;

opt_provider:
      T_FOR NonReservedWord_or_Sconst
      { $$ = driver.makeAstNode<SecurityProviderAST>( $1, $2 ); }
    | empty_aka_optional
    ;

security_label_type:
      T_DATABASE
      { $$ = driver.makeAstNode<CommonSecurityTypeAST>( $1,  0 ); }
    | T_EVENT T_TRIGGER
      { $$ = driver.makeAstNode<CommonSecurityTypeAST>( $1,$2 ); }
    | T_DOMAIN
      { $$ = driver.makeAstNode<CommonSecurityTypeAST>( $1,  0 ); }
    | T_TABLESPACE
      { $$ = driver.makeAstNode<CommonSecurityTypeAST>( $1,  0 ); }
    | T_TYPE
      { $$ = driver.makeAstNode<CommonSecurityTypeAST>( $1,  0 ); }
    ;

security_label:
      Sconst
    | T_NULL
    ;

/*****************************************************************************
 * ALTER THING name RENAME TO newname
 *****************************************************************************/

RenameStmt:
      T_ALTER T_AGGREGATE func_name aggr_args T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameAggrStmtAST>( $1, $2, $3->asFunctionRef(), $4->asAggrArguments(), $5, $6, driver.createName(NameAST::Unknown, $7) ); }
    | T_ALTER T_COLLATION any_name T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameCollationStmtAST>( $1, $2, $3->asAnyIdentifier(), $4, $5, driver.createName(NameAST::Unknown, $6) ); }
    | T_ALTER T_CONVERSION any_name T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameConversionStmtAST>( $1, $2, $3->asAnyIdentifier(), $4, $5, driver.createName(NameAST::Unknown, $6) ); }
    | T_ALTER T_DATABASE database_name T_RENAME T_TO database_name
      { $$ = driver.makeAstNode<RenameDatabaseStmtAST>( $1, $2, $3->asDatabaseName(), $4, $5, $6->asDatabaseName() ); }
    | T_ALTER T_DOMAIN any_name T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameDomainStmtAST>( $1, $2, $3->asAnyIdentifier(), $4, $5, driver.createName(NameAST::Unknown, $6) ); }
    | T_ALTER T_DOMAIN any_name T_RENAME T_CONSTRAINT name T_TO name
      { $$ = driver.makeAstNode<RenameDomainConstrStmtAST>( $1, $2, $3->asAnyIdentifier(), $4, $5, driver.createName(NameAST::Constraint, $6), $7, driver.createName(NameAST::Constraint, $8) ); }
    | T_ALTER T_FOREIGN T_DATA T_WRAPPER name T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameFdwStmtAST>( $1, $2, $3, $4, driver.createName(NameAST::Unknown, $5), $6, $7, driver.createName(NameAST::Unknown, $8) ); }
    | T_ALTER T_FUNCTION function_with_argtypes T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameFuncStmtAST>( $1, $2, $3->asFunctionWithArgTypes(), $4, $5, driver.createName(NameAST::Routine, $6) ); }
    | T_ALTER T_GROUP RoleId T_RENAME T_TO RoleId
      { $$ = driver.makeAstNode<RenameGroupRoleStmtAST>( $1, $2, $3->asRoleId(), $4, $5, $6->asRoleId() ); }
    | T_ALTER opt_procedural T_LANGUAGE name T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameLangStmtAST>( $1, $2, $3, driver.createName(NameAST::Unknown, $4), $5, $6, driver.createName(NameAST::Unknown, $7) ); }
    | T_ALTER T_OPERATOR T_CLASS any_name T_USING access_method T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameOpUsingStmtAST>( $1, $2, $3, $4->asAnyIdentifier(), $5, $6, $7, $8, driver.createName(NameAST::Unknown, $9) ); }
    | T_ALTER T_OPERATOR T_FAMILY any_name T_USING access_method T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameOpUsingStmtAST>( $1, $2, $3, $4->asAnyIdentifier(), $5, $6, $7, $8, driver.createName(NameAST::Unknown, $9) ); }
    | T_ALTER T_SCHEMA name T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameSchemaStmtAST>( $1, $2, driver.createName(NameAST::Schema, $3), $4, $5, driver.createName(NameAST::Schema, $6) ); }
    | T_ALTER T_SERVER name T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameServerStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, $5, driver.createName(NameAST::Unknown, $6) ); }
    | T_ALTER T_TABLE alter_relation_expr T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameTableStmtAST>( $1,  0, $2,  0,  0, $3->asRelationExpr(), $4, $5, driver.createName(NameAST::Table, $6) ); }
    | T_ALTER T_TABLE T_IF T_EXISTS alter_relation_expr T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameTableStmtAST>( $1,  0, $2, $3, $4, $5->asRelationExpr(), $6, $7, driver.createName(NameAST::Table, $8) ); }
    | T_ALTER T_SEQUENCE sequence_identifier T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameSequenceStmtAST>( $1, $2,  0,  0, $3->asSequenceIdentifier(), $4, $5, driver.createName(NameAST::Sequence, $6) ); }
    | T_ALTER T_SEQUENCE T_IF T_EXISTS sequence_identifier T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameSequenceStmtAST>( $1, $2, $3, $4, $5->asSequenceIdentifier(), $6, $7, driver.createName(NameAST::Sequence, $8) ); }
    | T_ALTER T_VIEW table_identifier T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameViewStmtAST>( $1,  0, $2,  0,  0, $3->asTableIdentifier(), $4, $5, driver.createName(NameAST::Table, $6) ); }
    | T_ALTER T_VIEW T_IF T_EXISTS table_identifier T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameViewStmtAST>( $1,  0, $2, $3, $4, $5->asTableIdentifier(), $6, $7, driver.createName(NameAST::Table, $8) ); }
    | T_ALTER T_MATERIALIZED T_VIEW table_identifier T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameViewStmtAST>( $1, $2, $3,  0,  0, $4->asTableIdentifier(), $5, $6, driver.createName(NameAST::Table, $7) ); }
    | T_ALTER T_MATERIALIZED T_VIEW T_IF T_EXISTS table_identifier T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameViewStmtAST>( $1, $2, $3, $4, $5, $6->asTableIdentifier(), $7, $8, driver.createName(NameAST::Table, $9) ); }
    | T_ALTER T_INDEX index_identifier T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameIndexStmtAST>( $1, $2,  0,  0, $3->asIndexIdentifier(), $4, $5, driver.createName(NameAST::Table, $6) ); }
    | T_ALTER T_INDEX T_IF T_EXISTS index_identifier T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameIndexStmtAST>( $1, $2, $3, $4, $5->asIndexIdentifier(), $6, $7, driver.createName(NameAST::Table, $8) ); }
    | T_ALTER T_FOREIGN T_TABLE relation_expr T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameTableStmtAST>( $1, $2, $3,  0,  0, $4->asRelationExpr(), $5, $6, driver.createName(NameAST::Table, $7) ); }
    | T_ALTER T_FOREIGN T_TABLE T_IF T_EXISTS relation_expr T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameTableStmtAST>( $1, $2, $3, $4, $5, $6->asRelationExpr(), $7, $8, driver.createName(NameAST::Table, $9) ); }
    | T_ALTER T_TABLE alter_relation_expr T_RENAME opt_column name T_TO name
      { $$ = driver.makeAstNode<RenameTableColStmtAST>( $1,  0, $2,  0,  0, $3->asRelationExpr(), $4, $5, driver.createName(NameAST::Column, $6), $7, driver.createName(NameAST::Column, $8) ); }
    | T_ALTER T_TABLE T_IF T_EXISTS alter_relation_expr T_RENAME opt_column name T_TO name
      { $$ = driver.makeAstNode<RenameTableColStmtAST>( $1,  0, $2, $3, $4, $5->asRelationExpr(), $6, $7, driver.createName(NameAST::Column, $8), $9, driver.createName(NameAST::Column, $10) ); }
    | T_ALTER T_MATERIALIZED T_VIEW table_identifier T_RENAME opt_column name T_TO name
      { $$ = driver.makeAstNode<RenameMatViewColStmtAST>( $1, $2, $3,  0,  0, $4->asTableIdentifier(), $5, $6, driver.createName(NameAST::Column, $7), $8, driver.createName(NameAST::Column, $9) ); }
    | T_ALTER T_MATERIALIZED T_VIEW T_IF T_EXISTS table_identifier T_RENAME opt_column name T_TO name
      { $$ = driver.makeAstNode<RenameMatViewColStmtAST>( $1, $2, $3, $4, $5, $6->asTableIdentifier(), $7, $8, driver.createName(NameAST::Column, $9), $10, driver.createName(NameAST::Column, $11) ); }
    | T_ALTER T_TABLE alter_relation_expr T_RENAME T_CONSTRAINT name T_TO name
      { $$ = driver.makeAstNode<RenameTableConstrStmtAST>( $1, $2, $3->asRelationExpr(), $4, $5, driver.createName(NameAST::Constraint, $6), $7, driver.createName(NameAST::Constraint, $8) ); }
    | T_ALTER T_FOREIGN T_TABLE relation_expr T_RENAME opt_column name T_TO name
      { $$ = driver.makeAstNode<RenameTableColStmtAST>( $1, $2, $3,  0,  0, $4->asRelationExpr(), $5, $6, driver.createName(NameAST::Column, $7), $8, driver.createName(NameAST::Column, $9) ); }
    | T_ALTER T_FOREIGN T_TABLE T_IF T_EXISTS relation_expr T_RENAME opt_column name T_TO name
      { $$ = driver.makeAstNode<RenameTableColStmtAST>( $1, $2, $3, $4, $5, $6->asRelationExpr(), $7, $8, driver.createName(NameAST::Column, $9), $10, driver.createName(NameAST::Column, $11) ); }
    | T_ALTER T_RULE name T_ON table_identifier T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameRuleStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, $5->asTableIdentifier(), $6, $7, driver.createName(NameAST::Unknown, $8) ); }
    | T_ALTER T_TRIGGER name T_ON table_identifier T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameTriggerStmtAST>( $1, $2, driver.createName(NameAST::Trigger, $3), $4, $5->asTableIdentifier(), $6, $7, driver.createName(NameAST::Trigger, $8) ); }
    | T_ALTER T_EVENT T_TRIGGER name T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameEvtTrgStmtAST>( $1, $2, $3, driver.createName(NameAST::Trigger, $4), $5, $6, driver.createName(NameAST::Trigger, $7) ); }
    | T_ALTER T_ROLE RoleId T_RENAME T_TO RoleId
      { $$ = driver.makeAstNode<RenameRoleStmtAST>( $1, $2, $3->asRoleId(), $4, $5, $6->asRoleId() ); }
    | T_ALTER T_USER RoleId T_RENAME T_TO RoleId
      { $$ = driver.makeAstNode<RenameRoleStmtAST>( $1, $2, $3->asRoleId(), $4, $5, $6->asRoleId() ); }
    | T_ALTER T_TABLESPACE name T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameTablespaceStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, $5, driver.createName(NameAST::Unknown, $6) ); }
    | T_ALTER T_TABLESPACE name T_SET reloptions
      { $$ = driver.makeAstNode<ChgReloptsTablespaceStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, $5->asRelOptions() ); }
    | T_ALTER T_TABLESPACE name T_RESET reloptions
      { $$ = driver.makeAstNode<ChgReloptsTablespaceStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, $5->asRelOptions() ); }
    | T_ALTER T_TEXT T_SEARCH T_PARSER any_name T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameSearchStmtAST>( $1, $2, $3, $4, $5->asAnyIdentifier(), $6, $7, driver.createName(NameAST::Unknown, $8) ); }
    | T_ALTER T_TEXT T_SEARCH T_DICTIONARY any_name T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameSearchStmtAST>( $1, $2, $3, $4, $5->asAnyIdentifier(), $6, $7, driver.createName(NameAST::Unknown, $8) ); }
    | T_ALTER T_TEXT T_SEARCH T_TEMPLATE any_name T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameSearchStmtAST>( $1, $2, $3, $4, $5->asAnyIdentifier(), $6, $7, driver.createName(NameAST::Unknown, $8) ); }
    | T_ALTER T_TEXT T_SEARCH T_CONFIGURATION any_name T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameSearchStmtAST>( $1, $2, $3, $4, $5->asAnyIdentifier(), $6, $7, driver.createName(NameAST::Unknown, $8) ); }
    | T_ALTER T_TYPE any_name T_RENAME T_TO name
      { $$ = driver.makeAstNode<RenameTypeStmtAST>( $1, $2, $3->asAnyIdentifier(), $4, $5, driver.createName(NameAST::Unknown, $6) ); }
    | T_ALTER T_TYPE any_name T_RENAME T_ATTRIBUTE name T_TO name opt_drop_behavior
      { $$ = driver.makeAstNode<RenameTypeAttrStmtAST>( $1, $2, $3->asAnyIdentifier(), $4, $5, driver.createName(NameAST::Unknown, $6), $7, driver.createName(NameAST::Unknown, $8) ); }
    ;

/*****************************************************************************
 *    QUERY:
 *        REINDEX type <name> [FORCE]
 *****************************************************************************/

ReindexStmt:
      T_REINDEX T_TABLE table_identifier opt_force
      { $$ = driver.makeAstNode<ReindexTableStmtAST>( $1, $2, $3->asTableIdentifier(), $4 ); }
    | T_REINDEX T_INDEX index_identifier opt_force
      { $$ = driver.makeAstNode<ReindexIndexStmtAST>( $1, $2, $3->asIndexIdentifier(), $4 ); }
    | T_REINDEX T_SYSTEM name opt_force
      { $$ = driver.makeAstNode<ReindexSystemStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4 ); }
    | T_REINDEX T_DATABASE name opt_force
      { $$ = driver.makeAstNode<ReindexDatabaseStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4 ); }
    ;

opt_force:
      T_FORCE
    | /* EMPTY */ { $$ = 0; }
    ;

/*****************************************************************************
 *    QUERY:
 *        PREPARE <plan_name> [(args, ...)] AS <query>
 *****************************************************************************/

PrepareStmt:
      T_PREPARE name prep_type_clause T_AS PreparableStmt
      {
        $$ = driver.makeAstNode<PrepareStmtAST>( $1,
                                                 driver.createName(NameAST::Unknown, $2),
                                                 $3->asPrepareTypeClause(),
                                                 $4,
                                                 $5->asStatement() );
      }
    ;

prep_type_clause:
      T_LEFT_PAREN type_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<PrepareTypeClauseAST>( $1, $2, $3 ); }
    | empty_aka_optional
    ;

/*****************************************************************************
 *    QUERY: LOCK TABLE
 *****************************************************************************/

LockStmt:
      T_LOCK opt_table relation_expr_list opt_lock opt_nowait
      { $$ = driver.makeAstNode<LockStmtAST>( $1, $2, $3, $4->asLockModeClause(), $5 ); }
    ;

opt_lock:
      T_IN lock_type T_MODE
      { $$ = driver.makeAstNode<LockModeClauseAST>( $1, $2->asLockType(), $3 ); }
    | empty_aka_optional
    ;

lock_type:
      T_ACCESS T_SHARE
      { $$ = driver.makeAstNode<LockTypeAST>( $1, $2,  0 ); }
    | T_ROW T_SHARE
      { $$ = driver.makeAstNode<LockTypeAST>( $1, $2,  0 ); }
    | T_ROW T_EXCLUSIVE
      { $$ = driver.makeAstNode<LockTypeAST>( $1, $2,  0 ); }
    | T_SHARE T_UPDATE T_EXCLUSIVE
      { $$ = driver.makeAstNode<LockTypeAST>( $1, $2, $3 ); }
    | T_SHARE
      { $$ = driver.makeAstNode<LockTypeAST>( $1,  0,  0 ); }
    | T_SHARE T_ROW T_EXCLUSIVE
      { $$ = driver.makeAstNode<LockTypeAST>( $1, $2, $3 ); }
    | T_EXCLUSIVE
      { $$ = driver.makeAstNode<LockTypeAST>( $1,  0,  0 ); }
    | T_ACCESS T_EXCLUSIVE
      { $$ = driver.makeAstNode<LockTypeAST>( $1, $2,  0 ); }
    ;

relation_expr_list:
      relation_expr
      { $$ = driver.makeAstNode<RelationExprListAST>( $1->asRelationExpr() ); }
    | relation_expr_list T_COMMA relation_expr
      { $$ = driver.makeAstNode<RelationExprListAST>( $1, $3->asRelationExpr() ); }
    ;

/*****************************************************************************
 *    QUERY: LOAD "filename"
 *****************************************************************************/

LoadStmt:
      T_LOAD file_name
      { $$ = driver.makeAstNode<LoadStmtAST>( $1, $2 ); }
    ;

file_name: Sconst ;

/*****************************************************************************
 *    QUERY:
 *        NOTIFY <identifier> can appear both in rule bodies and
 *        as a query-level command
 *****************************************************************************/

NotifyStmt:
      T_NOTIFY ColId notify_payload
      { $$ = driver.makeAstNode<NotifyStmtAST>( $1, $2, $3->asNotifyPayload() ); }
    ;

notify_payload:
      T_COMMA Sconst
      { $$ = driver.makeAstNode<NotifyPayloadAST>( $1, $2 ); }
    | empty_aka_optional
    ;

ListenStmt:
      T_LISTEN ColId
      { $$ = driver.makeAstNode<ListenStmtAST>( $1, $2 ); }
    ;

UnlistenStmt:
      T_UNLISTEN ColId
      { $$ = driver.makeAstNode<UnlistenStmtAST>( $1, $2 ); }
    | T_UNLISTEN T_STAR
      { $$ = driver.makeAstNode<UnlistenStmtAST>( $1, $2 ); }
    ;

/*****************************************************************************
 *    QUERY: CREATE INDEX
 *****************************************************************************/

IndexStmt:
      T_CREATE opt_unique T_INDEX opt_concurrently opt_index_name T_ON table_identifier
      {
        StatementAST::Info *info = driver.currentStatementInfo();
        info->type = StatementAST::Info::Create;
        info->target = StatementAST::Info::Index;
        driver.appendObjectIdentifierHelper(StatementAST::CompletionHelper::TableIdentifier, $6 + 1);
        $$ = driver.makeAstNode<CreateIndexStatementAST>( $1, $2, $3, $4,
                                                          driver.createName(NameAST::Index, $5),
                                                          $6,
                                                          $7->asTableIdentifier(),
                                                          (AccessMethodClauseAST *) 0,
                                                          0, (IndexElementListAST *) 0, 0,
                                                          (WithReloptionsAST *) 0,
                                                          (TableSpaceClauseAST *) 0,
                                                          (WhereClauseAST *) 0 );
      }
    | T_CREATE opt_unique T_INDEX opt_concurrently opt_index_name T_ON table_identifier
      access_method_clause T_LEFT_PAREN index_params T_RIGHT_PAREN opt_reloptions OptTableSpace where_clause
      {
        StatementAST::Info *info = driver.currentStatementInfo();
        info->type = StatementAST::Info::Create;
        info->target = StatementAST::Info::Index;
        driver.appendObjectIdentifierHelper(StatementAST::CompletionHelper::TableIdentifier, $6 + 1);
        TableIdentifierAST *tableIdent = $7->asTableIdentifier();
        if ( tableIdent )
          driver.appendColumnNameHelper($9, $11, tableIdent->schemaName, tableIdent->tableName);
        $$ = driver.makeAstNode<CreateIndexStatementAST>( $1, $2, $3, $4,
                                                          driver.createName(NameAST::Index, $5),
                                                          $6,
                                                          tableIdent,
                                                          $8->asAccessMethodClause(),
                                                          $9, $10, $11,
                                                          $12->asWithReloptions(),
                                                          $13->asTableSpaceClause(),
                                                          $14->asWhereClause() );
      }
    ;

opt_unique:
      T_UNIQUE
    | /*EMPTY*/ { $$ = 0; }
  ;

opt_concurrently:
      T_CONCURRENTLY
    | /*EMPTY*/ { $$ = 0; }
    ;

opt_index_name:
      index_name
    | /*EMPTY*/ { $$ = 0; }
    ;

index_params:
      index_elem
      { $$ = driver.makeAstNode<IndexElementListAST>( $1->asIndexElement() ); }
    | index_params T_COMMA index_elem
      { $$ = driver.makeAstNode<IndexElementListAST>( $1, $3->asIndexElement() ); }
    ;

/*****************************************************************************
 * GRANT and REVOKE ROLE statements
 *****************************************************************************/

GrantRoleStmt:
      T_GRANT privilege_list T_TO role_list opt_grant_admin_option opt_granted_by
      {
        $$ = driver.makeAstNode<GrantRoleStmtAST>( $1, $2, $3, $4,
                                                   $5->asWithAdminOption(),
                                                   $6->asGrantedByClause() );
      }
    ;

RevokeRoleStmt:
      T_REVOKE                        privilege_list T_FROM role_list opt_granted_by opt_drop_behavior
      {
        $$ = driver.makeAstNode<RevokeRoleStmtAST>( $1,
                                                    0, 0, 0,
                                                    $2, $3, $4,
                                                    $5->asGrantedByClause(),
                                                    $6 );
      }
    | T_REVOKE T_ADMIN T_OPTION T_FOR privilege_list T_FROM role_list opt_granted_by opt_drop_behavior
      {
        $$ = driver.makeAstNode<RevokeRoleStmtAST>( $1,
                                                    $2, $3, $4,
                                                    $5, $6, $7,
                                                    $8->asGrantedByClause(),
                                                    $9 );
      }
    ;

opt_grant_admin_option:
      T_WITH T_ADMIN T_OPTION
      { $$ = driver.makeAstNode<WithAdminOptionAST>( $1, $2, $3 ); }
    | empty_aka_optional
    ;

opt_granted_by:
      T_GRANTED T_BY RoleId
      { $$ = driver.makeAstNode<GrantedByClauseAST>( $1, $2, $3->asRoleId() ); }
    | empty_aka_optional
    ;

/*****************************************************************************
 * GRANT and REVOKE statements
 *****************************************************************************/

GrantStmt:
      T_GRANT privileges T_ON privilege_target T_TO grantee_list opt_grant_grant_option
      {
        $$ = driver.makeAstNode<GrantStmtAST>( $1,
                                               $2->asPrivileges(),
                                               $3,
                                               $4->asPrivilegeTarget(),
                                               $5,
                                               $6,
                                               $7->asWithGrantOption() );
      }
    ;

RevokeStmt:
      T_REVOKE                        privileges T_ON privilege_target T_FROM grantee_list opt_drop_behavior
      {
        $$ = driver.makeAstNode<RevokeStmtAST>( $1,
                                                0, 0, 0,
                                                $2->asPrivileges(),
                                                $3,
                                                $4->asPrivilegeTarget(),
                                                $5,
                                                $6,
                                                $7 );
      }
    | T_REVOKE T_GRANT T_OPTION T_FOR privileges T_ON privilege_target T_FROM grantee_list opt_drop_behavior
      {
        $$ = driver.makeAstNode<RevokeStmtAST>( $1,
                                                $2, $3, $4,
                                                $5->asPrivileges(),
                                                $6,
                                                $7->asPrivilegeTarget(),
                                                $8,
                                                $9,
                                                $10 );
      }
    ;

privilege_target:
      table_identifier_list
      { $$ = driver.makeAstNode<TablePrivilegeTargetAST>(  0, $1 ); }
    | T_TABLE table_identifier_list
      { $$ = driver.makeAstNode<TablePrivilegeTargetAST>( $1, $2 ); }
    | T_SEQUENCE sequence_identifier_list
      { $$ = driver.makeAstNode<SequencePrivilegeTargetAST>( $1, $2 ); }
    | T_FOREIGN T_DATA T_WRAPPER name_list
      { $$ = driver.makeAstNode<FdwPrivilegeTargetAST>( $1, $2, $3, $4 ); }
    | T_FOREIGN T_SERVER name_list
      { $$ = driver.makeAstNode<ForeignSrvPrivilegeTargetAST>( $1, $2, $3 ); }
    | T_FUNCTION function_with_argtypes_list
      { $$ = driver.makeAstNode<FuncPrivilegeTargetAST>( $1, $2 ); }
    | T_DATABASE name_list
      { $$ = driver.makeAstNode<DbPrivilegeTargetAST>( $1, $2 ); }
    | T_DOMAIN any_name_list
      { $$ = driver.makeAstNode<DomainPrivilegeTargetAST>( $1, $2 ); }
    | T_LANGUAGE name_list
      { $$ = driver.makeAstNode<LangPrivilegeTargetAST>( $1, $2 ); }
    | T_LARGE T_OBJECT NumericOnly_list
      { $$ = driver.makeAstNode<LargeObjPrivilegeTargetAST>( $1, $2, $3 ); }
    | T_SCHEMA schema_name_list
      { $$ = driver.makeAstNode<SchemaPrivilegeTargetAST>( $1, $2 ); }
    | T_TABLESPACE name_list
      { $$ = driver.makeAstNode<TblspacePrivilegeTargetAST>( $1, $2 ); }
    | T_TYPE any_name_list
      { $$ = driver.makeAstNode<TypePrivilegeTargetAST>( $1, $2 ); }
    | T_ALL T_TABLES T_IN T_SCHEMA schema_name_list
      { $$ = driver.makeAstNode<AllInSchPrivilegeTargetAST>( $1, $2, $3, $4, $5 ); }
    | T_ALL T_SEQUENCES T_IN T_SCHEMA schema_name_list
      { $$ = driver.makeAstNode<AllInSchPrivilegeTargetAST>( $1, $2, $3, $4, $5 ); }
    | T_ALL T_FUNCTIONS T_IN T_SCHEMA schema_name_list
      { $$ = driver.makeAstNode<AllInSchPrivilegeTargetAST>( $1, $2, $3, $4, $5 ); }
    ;

NumericOnly_list:
      NumericOnly
      { $$ = driver.makeAstNode<NumericListAST>( driver.makeAstNode<NumericAST>($1) ); }
    | NumericOnly_list T_COMMA NumericOnly
      { $$ = driver.makeAstNode<NumericListAST>( $1, driver.makeAstNode<NumericAST>($3) ); }
    ;

function_with_argtypes_list:
      function_with_argtypes
      { $$ = driver.makeAstNode<FunctionWithArgTypesListAST>( $1->asFunctionWithArgTypes() ); }
    | function_with_argtypes_list T_COMMA function_with_argtypes
      { $$ = driver.makeAstNode<FunctionWithArgTypesListAST>( $1, $3->asFunctionWithArgTypes() ); }
    ;

/*****************************************************************************
 *    QUERY: fetch/move
 *****************************************************************************/

FetchStmt:
      T_FETCH fetch_args
      { $$ = driver.makeAstNode<FetchStmtAST>( $1, $2->asFetchClause() ); }
    | T_MOVE fetch_args
      { $$ = driver.makeAstNode<FetchStmtAST>( $1, $2->asFetchClause() ); }
    ;

fetch_args:
      cursor_name
      { $$ = driver.makeAstNode<FetchClauseAST>(  0,  0,  0, driver.createName(NameAST::Unknown, $1) ); }
    | from_in cursor_name
      { $$ = driver.makeAstNode<FetchClauseAST>(  0,  0, $1, driver.createName(NameAST::Unknown, $2) ); }
    | T_NEXT opt_from_in cursor_name
      { $$ = driver.makeAstNode<FetchClauseAST>( $1,  0, $2, driver.createName(NameAST::Unknown, $3) ); }
    | T_PRIOR opt_from_in cursor_name
      { $$ = driver.makeAstNode<FetchClauseAST>( $1,  0, $2, driver.createName(NameAST::Unknown, $3) ); }
    | T_FIRST opt_from_in cursor_name
      { $$ = driver.makeAstNode<FetchClauseAST>( $1,  0, $2, driver.createName(NameAST::Unknown, $3) ); }
    | T_LAST opt_from_in cursor_name
      { $$ = driver.makeAstNode<FetchClauseAST>( $1,  0, $2, driver.createName(NameAST::Unknown, $3) ); }
    | T_ABSOLUTE SignedIconst opt_from_in cursor_name
      { $$ = driver.makeAstNode<FetchClauseAST>( $1, $2, $3, driver.createName(NameAST::Unknown, $4) ); }
    | T_RELATIVE SignedIconst opt_from_in cursor_name
      { $$ = driver.makeAstNode<FetchClauseAST>( $1, $2, $3, driver.createName(NameAST::Unknown, $4) ); }
    | SignedIconst opt_from_in cursor_name
      { $$ = driver.makeAstNode<FetchClauseAST>(  0, $1, $2, driver.createName(NameAST::Unknown, $3) ); }
    | T_ALL opt_from_in cursor_name
      { $$ = driver.makeAstNode<FetchClauseAST>(  0, $1, $2, driver.createName(NameAST::Unknown, $3) ); }
    | T_FORWARD opt_from_in cursor_name
      { $$ = driver.makeAstNode<FetchClauseAST>( $1,  0, $2, driver.createName(NameAST::Unknown, $3) ); }
    | T_FORWARD SignedIconst opt_from_in cursor_name
      { $$ = driver.makeAstNode<FetchClauseAST>( $1, $2, $3, driver.createName(NameAST::Unknown, $4) ); }
    | T_FORWARD T_ALL opt_from_in cursor_name
      { $$ = driver.makeAstNode<FetchClauseAST>( $1, $2, $3, driver.createName(NameAST::Unknown, $4) ); }
    | T_BACKWARD opt_from_in cursor_name
      { $$ = driver.makeAstNode<FetchClauseAST>( $1,  0, $3, driver.createName(NameAST::Unknown, $3) ); }
    | T_BACKWARD SignedIconst opt_from_in cursor_name
      { $$ = driver.makeAstNode<FetchClauseAST>( $1, $2, $3, driver.createName(NameAST::Unknown, $4) ); }
    | T_BACKWARD T_ALL opt_from_in cursor_name
      { $$ = driver.makeAstNode<FetchClauseAST>( $1, $2, $3, driver.createName(NameAST::Unknown, $4) ); }
    ;

from_in:
      T_FROM
    | T_IN
    ;

opt_from_in:
      from_in
    | /* EMPTY */ { $$ = 0; }
    ;

/*****************************************************************************
 *    QUERY :
 *        REFRESH MATERIALIZED VIEW qualified_name
 *****************************************************************************/

RefreshMatViewStmt:
      T_REFRESH T_MATERIALIZED T_VIEW table_identifier opt_with_data
      {
        $$ = driver.makeAstNode<RefreshMatViewStmtAST>( $1, $2, $3,
                                                        $4->asTableIdentifier(),
                                                        $5->asWithDataClause() );
      }
    ;

/*****************************************************************************
 *    QUERY:
 *        EXPLAIN [ANALYZE] [VERBOSE] query
 *        EXPLAIN ( options ) query
 *****************************************************************************/

ExplainStmt:
      T_EXPLAIN                             ExplainableStmt
      { $$ = driver.makeAstNode<CommonExplainStmtAST>( $1,  0,  0, $2->asStatement() ); }
    | T_EXPLAIN analyze_keyword opt_verbose ExplainableStmt
      { $$ = driver.makeAstNode<CommonExplainStmtAST>( $1, $2, $3, $4->asStatement() ); }
    | T_EXPLAIN                 T_VERBOSE   ExplainableStmt
      { $$ = driver.makeAstNode<CommonExplainStmtAST>( $1,  0, $2, $3->asStatement() ); }
    | T_EXPLAIN T_LEFT_PAREN explain_option_list T_RIGHT_PAREN ExplainableStmt
      { $$ = driver.makeAstNode<OptExplainStmtAST>( $1, $2, $3, $4, $5->asStatement() ); }
    ;

ExplainableStmt:
      SelectStmt
    | InsertStmt
    | UpdateStmt
    | DeleteStmt
    | DeclareCursorStmt
    | CreateTableAsStmt
    | CreateMatViewStmt
    | RefreshMatViewStmt
    | ExecuteStmt
    ;

explain_option_list:
      explain_option_elem
      { $$ = driver.makeAstNode<ExplainOptionListAST>( $1->asExplainOption() ); }
    | explain_option_list T_COMMA explain_option_elem
      { $$ = driver.makeAstNode<ExplainOptionListAST>( $1, $3->asExplainOption() ); }
    ;

explain_option_elem:
      explain_option_name explain_option_arg
      { $$ = driver.makeAstNode<ExplainOptionAST>( $1, $2 ); }
    ;

explain_option_name:
      NonReservedWord
    | analyze_keyword
    ;

explain_option_arg:
      opt_boolean_or_string
    | NumericOnly
    | /* EMPTY */ { $$ = 0; }
    ;

/*****************************************************************************
 *  EXECUTE <plan_name> [(params, ...)]
 *  CREATE TABLE <name> AS EXECUTE <plan_name> [(params, ...)]
 *****************************************************************************/

ExecuteStmt:
      T_EXECUTE name execute_param_clause
      {
        $$ = driver.makeAstNode<ExecuteStmtAST>( $1,
                                                 driver.createName(NameAST::Unknown, $2),
                                                 $3->asExecuteParamClause() );
      }
    | T_CREATE OptTemp T_TABLE create_as_target T_AS
      T_EXECUTE name execute_param_clause opt_with_data
      {
        ExecuteStmtAST *execStmt = driver.makeAstNode<ExecuteStmtAST>( $6,
                                                                       driver.createName(NameAST::Unknown, $7),
                                                                       $8->asExecuteParamClause() );
        $$ = driver.makeAstNode<CreateTableAsExecStmtAST>( $1,
                                                           $2->asTempClause(),
                                                           $3,
                                                           $4->asCreateAsTarget(),
                                                           $5,
                                                           execStmt,
                                                           $9->asWithDataClause() );
      }
    ;

execute_param_clause:
      T_LEFT_PAREN expr_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<ExecuteParamClauseAST>( $1, $2, $3 ); }
    | empty_aka_optional
    ;

/*****************************************************************************
 *    QUERY:
 *        DROP itemtype [ IF EXISTS ] itemname [, itemname ...]
 *           [ RESTRICT | CASCADE ]
 *****************************************************************************/

drop:
      T_DROP  { driver.currentStatementInfo()->type = StatementAST::Info::Drop; }
    ;

CH_on_before_table_identifier:
      T_ON  { driver.appendObjectIdentifierHelper(StatementAST::CompletionHelper::TableIdentifier, $1 + 1); $$ = $1; }
    ;

DropStmt:
      drop drop_type T_IF T_EXISTS any_name_list opt_drop_behavior
      {
        int behavior = $6;
        CommonDropStmtAST *ast = driver.makeAstNode<CommonDropStmtAST>( $1, $2->asCommonDropType(), $3, $4, $5, behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop drop_type               any_name_list opt_drop_behavior
      {
        int behavior = $4;
        CommonDropStmtAST *ast = driver.makeAstNode<CommonDropStmtAST>( $1, $2->asCommonDropType(),  0,  0, $3, behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop T_ASSERTION name opt_drop_behavior
      {
        int behavior = $4;
        DropAssertStmtAST *ast = driver.makeAstNode<DropAssertStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop opt_procedural T_LANGUAGE               NonReservedWord_or_Sconst opt_drop_behavior
      {
        int behavior = $5;
        DropPLangStmtAST *ast = driver.makeAstNode<DropPLangStmtAST>( $1, $2, $3, 0, 0, $4, behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop opt_procedural T_LANGUAGE T_IF T_EXISTS NonReservedWord_or_Sconst opt_drop_behavior
      {
        int behavior = $7;
        DropPLangStmtAST *ast = driver.makeAstNode<DropPLangStmtAST>( $1, $2, $3, $4, $5, $6, behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop T_OPERATOR T_FAMILY               any_name T_USING access_method opt_drop_behavior
      {
        int behavior = $7;
        DropOpFamilyStmtAST *ast = driver.makeAstNode<DropOpFamilyStmtAST>( $1, $2, $3, 0, 0, $4->asAnyIdentifier(), $5, $6, behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop T_OPERATOR T_FAMILY T_IF T_EXISTS any_name T_USING access_method opt_drop_behavior
      {
        int behavior = $9;
        DropOpFamilyStmtAST *ast = driver.makeAstNode<DropOpFamilyStmtAST>( $1, $2, $3, $4, $5, $6->asAnyIdentifier(), $7, $8, behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop T_OPERATOR T_CLASS               any_name T_USING access_method opt_drop_behavior
      {
        int behavior = $7;
        DropOpClassStmtAST *ast = driver.makeAstNode<DropOpClassStmtAST>( $1, $2, $3, 0, 0, $4->asAnyIdentifier(), $5, $6, behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop T_OPERATOR T_CLASS T_IF T_EXISTS any_name T_USING access_method opt_drop_behavior
      {
        int behavior = $9;
        DropOpClassStmtAST *ast = driver.makeAstNode<DropOpClassStmtAST>( $1, $2, $3, $4, $5, $6->asAnyIdentifier(), $7, $8, behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop T_OPERATOR               any_operator oper_argtypes opt_drop_behavior
      {
        int behavior = $5;
        DropOperStmtAST *ast = driver.makeAstNode<DropOperStmtAST>( $1, $2, 0, 0, $3->asOperand(), $4->asOperArgTypes(), behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop T_OPERATOR T_IF T_EXISTS any_operator oper_argtypes opt_drop_behavior
      {
        int behavior = $7;
        DropOperStmtAST *ast = driver.makeAstNode<DropOperStmtAST>( $1, $2, $3, $4, $5->asOperand(), $6->asOperArgTypes(), behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop T_AGGREGATE               func_name aggr_args opt_drop_behavior
      {
        int behavior = $5;
        DropAggrStmtAST *ast = driver.makeAstNode<DropAggrStmtAST>( $1, $2, 0, 0, $3->asFunctionRef(), $4->asAggrArguments(), behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop T_AGGREGATE T_IF T_EXISTS func_name aggr_args opt_drop_behavior
      {
        int behavior = $7;
        DropAggrStmtAST *ast = driver.makeAstNode<DropAggrStmtAST>( $1, $2, $3, $4, $5->asFunctionRef(), $6->asAggrArguments(), behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop T_FUNCTION               func_name func_args opt_drop_behavior
      {
        int behavior = $5;
        DropFuncStmtAST *ast = driver.makeAstNode<DropFuncStmtAST>( $1, $2, 0, 0, $3->asFunctionRef(), $4->asFunctionArgTypes(), behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop T_FUNCTION T_IF T_EXISTS func_name func_args opt_drop_behavior
      {
        int behavior = $7;
        DropFuncStmtAST *ast = driver.makeAstNode<DropFuncStmtAST>( $1, $2, $3, $4, $5->asFunctionRef(), $6->asFunctionArgTypes(), behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop T_CAST opt_if_exists T_LEFT_PAREN Typename T_AS Typename T_RIGHT_PAREN opt_drop_behavior
      {
        int behavior = $9;
        DropCastStmtAST *ast = driver.makeAstNode<DropCastStmtAST>( $1, $2, $3->asIfExists(), $4, $5->asType(), $6, $7->asType(), $8, behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop T_OWNED T_BY role_list opt_drop_behavior
      {
        int behavior = $5;
        DropOwnedStmtAST *ast = driver.makeAstNode<DropOwnedStmtAST>( $1, $2, $3, $4, behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop T_SERVER               name opt_drop_behavior
      {
        int behavior = $4;
        DropForeignServerStmtAST *ast = driver.makeAstNode<DropForeignServerStmtAST>( $1, $2, 0, 0, driver.createName(NameAST::Unknown, $3), behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop T_SERVER T_IF T_EXISTS name opt_drop_behavior
      {
        int behavior = $6;
        DropForeignServerStmtAST *ast = driver.makeAstNode<DropForeignServerStmtAST>( $1, $2, $3, $4, driver.createName(NameAST::Unknown, $5), behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop T_RULE               name CH_on_before_table_identifier table_identifier opt_drop_behavior
      {
        int behavior = $6;
        DropRuleStmtAST *ast = driver.makeAstNode<DropRuleStmtAST>( $1, $2, 0, 0, driver.createName(NameAST::Unknown, $3), $4, $5->asTableIdentifier(), behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop T_RULE T_IF T_EXISTS name CH_on_before_table_identifier table_identifier opt_drop_behavior
      {
        int behavior = $8;
        DropRuleStmtAST *ast = driver.makeAstNode<DropRuleStmtAST>( $1, $2, $3, $4, driver.createName(NameAST::Unknown, $5), $6, $7->asTableIdentifier(), behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop T_FOREIGN T_DATA T_WRAPPER               name opt_drop_behavior
      {
        int behavior = $6;
        DropFdwStmtAST *ast = driver.makeAstNode<DropFdwStmtAST>( $1, $2, $3, $4, 0, 0, driver.createName(NameAST::Unknown, $5), behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop T_FOREIGN T_DATA T_WRAPPER T_IF T_EXISTS name opt_drop_behavior
      {
        int behavior = $8;
        DropFdwStmtAST *ast = driver.makeAstNode<DropFdwStmtAST>( $1, $2, $3, $4, $5, $6, driver.createName(NameAST::Unknown, $7), behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop target_schema T_IF T_EXISTS schema_name_list opt_drop_behavior
      {
        int behavior = $6;
        DropSchemaStmtAST *ast = driver.makeAstNode<DropSchemaStmtAST>( $1, $2, $3, $4, $5, behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop target_schema               schema_name_list opt_drop_behavior
      {
        int behavior = $4;
        DropSchemaStmtAST *ast = driver.makeAstNode<DropSchemaStmtAST>( $1, $2,  0,  0, $3, behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop target_sequence_list T_IF T_EXISTS sequence_identifier_list opt_drop_behavior
      {
        int behavior = $6;
        DropSequenceStmtAST *ast = driver.makeAstNode<DropSequenceStmtAST>( $1, $2, $3, $4, $5, behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop target_sequence_list               sequence_identifier_list opt_drop_behavior
      {
        int behavior = $4;
        DropSequenceStmtAST *ast = driver.makeAstNode<DropSequenceStmtAST>( $1, $2,  0,  0, $3, behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop                target_table_list T_IF T_EXISTS table_identifier_list opt_drop_behavior
      {
        int behavior = $6;
        DropTableStmtAST *ast = driver.makeAstNode<DropTableStmtAST>( $1,  0, $2, $3, $4, $5, behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop                target_table_list               table_identifier_list opt_drop_behavior
      {
        int behavior = $4;
        DropTableStmtAST *ast = driver.makeAstNode<DropTableStmtAST>( $1,  0, $2,  0,  0, $3, behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop T_FOREIGN      target_table_list T_IF T_EXISTS table_identifier_list opt_drop_behavior
      {
        int behavior = $7;
        DropTableStmtAST *ast = driver.makeAstNode<DropTableStmtAST>( $1, $2, $3, $4, $5, $6, behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop T_FOREIGN      target_table_list               table_identifier_list opt_drop_behavior
      {
        int behavior = $5;
        DropTableStmtAST *ast = driver.makeAstNode<DropTableStmtAST>( $1, $2, $3,  0,  0, $4, behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop                target_view_list  T_IF T_EXISTS table_identifier_list opt_drop_behavior
      {
        int behavior = $6;
        DropViewStmtAST *ast = driver.makeAstNode<DropViewStmtAST>( $1,  0, $2, $3, $4, $5, behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop                target_view_list                table_identifier_list opt_drop_behavior
      {
        int behavior = $4;
        DropViewStmtAST *ast = driver.makeAstNode<DropViewStmtAST>( $1,  0, $2,  0,  0, $3, behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop T_MATERIALIZED target_view_list  T_IF T_EXISTS table_identifier_list opt_drop_behavior
      {
        int behavior = $7;
        DropViewStmtAST *ast = driver.makeAstNode<DropViewStmtAST>( $1, $2, $3, $4, $5, $6, behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop T_MATERIALIZED target_view_list                table_identifier_list opt_drop_behavior
      {
        int behavior = $5;
        DropViewStmtAST *ast = driver.makeAstNode<DropViewStmtAST>( $1, $2, $3,  0,  0, $4, behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop target_index_list                T_IF T_EXISTS index_identifier_list opt_drop_behavior
      {
        int behavior = $6;
        DropIndexStmtAST *ast = driver.makeAstNode<DropIndexStmtAST>( $1, $2,  0, $3, $4, $5, behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop target_index_list                              index_identifier_list opt_drop_behavior
      {
        int behavior = $4;
        DropIndexStmtAST *ast = driver.makeAstNode<DropIndexStmtAST>( $1, $2,  0,  0,  0, $3, behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop target_index_list T_CONCURRENTLY T_IF T_EXISTS index_identifier_list opt_drop_behavior
      {
        int behavior = $7;
        DropIndexStmtAST *ast = driver.makeAstNode<DropIndexStmtAST>( $1, $2, $3, $4, $5, $6, behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop target_index_list T_CONCURRENTLY               index_identifier_list opt_drop_behavior
      {
        int behavior = $5;
        DropIndexStmtAST *ast = driver.makeAstNode<DropIndexStmtAST>( $1, $2, $3,  0,  0, $4, behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop target_trigger               name
      {
        $$ = driver.makeAstNode<DropTrigStmtAST>( $1, $2,
                                                  0, 0,
                                                  driver.createName(NameAST::Trigger, $3),
                                                  0,
                                                  (TableIdentifierAST *) 0,
                                                  0 );
      }
    | drop target_trigger               name T_ON table_identifier opt_drop_behavior
      {
        int behavior = $6;
        DropTrigStmtAST *ast = driver.makeAstNode<DropTrigStmtAST>( $1, $2, 0, 0, driver.createName(NameAST::Trigger, $3), $4, $5->asTableIdentifier(), behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop target_trigger T_IF T_EXISTS name T_ON table_identifier opt_drop_behavior
      {
        int behavior = $8;
        DropTrigStmtAST *ast = driver.makeAstNode<DropTrigStmtAST>( $1, $2, $3, $4, driver.createName(NameAST::Trigger, $5), $6, $7->asTableIdentifier(), behavior );
        if ( !behavior )
          driver.appendKeywordsHelper(ast->lastToken(), "RESTRICT;CASCADE");
        $$ = ast;
      }
    | drop target_trigger T_IF T_EXISTS name
      {
        $$ = driver.makeAstNode<DropTrigStmtAST>( $1, $2,
                                                  $3, $4,
                                                  driver.createName(NameAST::Trigger, $5),
                                                  0,
                                                  (TableIdentifierAST *) 0,
                                                  0 );
      }
    | drop T_DATABASE database_name
      { $$ = driver.makeAstNode<DropDbStmtAST>( $1, $2,  0,  0, $3->asDatabaseName() ); }
    | drop T_DATABASE T_IF T_EXISTS database_name
      { $$ = driver.makeAstNode<DropDbStmtAST>( $1, $2, $3, $4, $5->asDatabaseName() ); }
    | drop T_USER T_MAPPING               T_FOR auth_ident T_SERVER name
      { $$ = driver.makeAstNode<DropUserMappingStmtAST>( $1, $2, $3, 0, 0, $4, $5->asAuthIdentifier(), $6, driver.createName(NameAST::Unknown, $7) ); }
    | drop T_USER T_MAPPING T_IF T_EXISTS T_FOR auth_ident T_SERVER name
      { $$ = driver.makeAstNode<DropUserMappingStmtAST>( $1, $2, $3, $4, $5, $6, $7->asAuthIdentifier(), $8, driver.createName(NameAST::Unknown, $9) ); }
    | drop T_ROLE role_list
      { $$ = driver.makeAstNode<DropRoleStmtAST>( $1, $2,  0,  0, $3 ); }
    | drop T_ROLE T_IF T_EXISTS role_list
      { $$ = driver.makeAstNode<DropRoleStmtAST>( $1, $2, $3, $4, $5 ); }
    | drop T_USER role_list
      { $$ = driver.makeAstNode<DropRoleStmtAST>( $1, $2,  0,  0, $3 ); }
    | drop T_USER T_IF T_EXISTS role_list
      { $$ = driver.makeAstNode<DropRoleStmtAST>( $1, $2, $3, $4, $5 ); }
    | drop T_TABLESPACE name
      { $$ = driver.makeAstNode<DropTableSpaceStmtAST>( $1, $2,  0,  0, driver.createName(NameAST::Unknown, $3) ); }
    | drop T_TABLESPACE T_IF T_EXISTS name
      { $$ = driver.makeAstNode<DropTableSpaceStmtAST>( $1, $2, $3, $4, driver.createName(NameAST::Unknown, $5) ); }
    | drop T_GROUP               role_list
      { $$ = driver.makeAstNode<DropGroupStmtAST>( $1, $2,  0,  0, $3 ); }
    | drop T_GROUP T_IF T_EXISTS role_list
      { $$ = driver.makeAstNode<DropGroupStmtAST>( $1, $2, $3, $4, $5 ); }
    ;


drop_type:
      T_EVENT T_TRIGGER
      { $$ = driver.makeAstNode<CommonDropTypeAST>( $1, $2,  0 ); }
    | T_TYPE
      { $$ = driver.makeAstNode<CommonDropTypeAST>( $1,  0,  0 ); }
    | T_DOMAIN
      { $$ = driver.makeAstNode<CommonDropTypeAST>( $1,  0,  0 ); }
    | T_COLLATION
      { $$ = driver.makeAstNode<CommonDropTypeAST>( $1,  0,  0 ); }
    | T_CONVERSION
      { $$ = driver.makeAstNode<CommonDropTypeAST>( $1,  0,  0 ); }
    | T_EXTENSION
      { $$ = driver.makeAstNode<CommonDropTypeAST>( $1,  0,  0 ); }
    | T_TEXT T_SEARCH T_PARSER
      { $$ = driver.makeAstNode<CommonDropTypeAST>( $1, $2, $3 ); }
    | T_TEXT T_SEARCH T_DICTIONARY
      { $$ = driver.makeAstNode<CommonDropTypeAST>( $1, $2, $3 ); }
    | T_TEXT T_SEARCH T_TEMPLATE
      { $$ = driver.makeAstNode<CommonDropTypeAST>( $1, $2, $3 ); }
    | T_TEXT T_SEARCH T_CONFIGURATION
      { $$ = driver.makeAstNode<CommonDropTypeAST>( $1, $2, $3 ); }
    ;

/*****************************************************************************
 *    QUERY:  Define Rewrite Rule
 *****************************************************************************/

RuleStmt:
      T_CREATE opt_or_replace T_RULE name T_AS T_ON event
      T_TO table_identifier where_clause T_DO opt_instead RuleActionList
      {
        $$ = driver.makeAstNode<RuleStmtAST>( $1,
                                              $2->asOrReplaceClause(),
                                              $3,
                                              driver.createName(NameAST::Unknown, $4),
                                              $5, $6, $7, $8,
                                              $9->asTableIdentifier(),
                                              $10->asWhereClause(),
                                              $11, $12,
                                              $13->asRuleActions() );
      }
    ;

RuleActionList:
      T_NOTHING
      { $$ = driver.makeAstNode<NothingRuleActionsAST>( $1 ); }
    | RuleActionStmt
      { $$ = driver.makeAstNode<SingleRuleActionsAST>( $1->asStatement() ); }
    | T_LEFT_PAREN RuleActionMulti T_RIGHT_PAREN
      { $$ = driver.makeAstNode<MultiRuleActionsAST>( $1, $2, $3 ); }
    ;

RuleActionMulti:
      RuleActionMulti T_SEMICOLON RuleActionStmtOrEmpty
      { $$ = driver.makeAstNode<StatementListAST>( $1, $3->asStatement() ); }
    | RuleActionStmtOrEmpty
      { $$ = driver.makeAstNode<StatementListAST>( $1->asStatement() ); }
    ;

RuleActionStmt:
      SelectStmt
    | InsertStmt
    | UpdateStmt
    | DeleteStmt
    | NotifyStmt
    ;

RuleActionStmtOrEmpty:
      RuleActionStmt
    | empty_aka_optional
    ;

event:
      T_SELECT
    | T_UPDATE
    | T_DELETE
    | T_INSERT
    ;

opt_instead:
      T_INSTEAD
    | T_ALSO
    | /*EMPTY*/ { $$ = 0; }
    ;

/*****************************************************************************
 *    QUERY:
 *        REASSIGN OWNED BY username [, username ...] TO username
 *****************************************************************************/

ReassignOwnedStmt:
      T_REASSIGN T_OWNED T_BY role_list T_TO name
      { $$ = driver.makeAstNode<ReassignOwnedStmtAST>( $1, $2, $3, $4, $5, driver.createName(NameAST::Role, $6) ); }
    ;

/*****************************************************************************
 *    DO <anonymous code block> [ LANGUAGE language ]
 *****************************************************************************/

DoStmt:
      T_DO dostmt_opt_list
      { $$ = driver.makeAstNode<DoStmtAST>( $1, $2 ); }
    ;

dostmt_opt_list:
      dostmt_opt_item
      { $$ = driver.makeAstNode<DoStmtItemListAST>( $1->asDoStmtItem() ); }
    | dostmt_opt_list dostmt_opt_item
      { $$ = driver.makeAstNode<DoStmtItemListAST>( $1, $2->asDoStmtItem() ); }
    ;

dostmt_opt_item:
      Sconst
      { $$ = driver.makeAstNode<DoStmtCmdItemAST>( $1 ); }
    | T_LANGUAGE NonReservedWord_or_Sconst
      { $$ = driver.makeAstNode<DoStmtLangItemAST>( $1, $2 ); }
    ;

/*****************************************************************************
 *    QUERY :
 *        define (aggregate,operator,type)
 *****************************************************************************/

DefineStmt:
      T_CREATE T_AGGREGATE func_name aggr_args definition
      {
        $$ = driver.makeAstNode<CreateAggregateStmtAST>( $1, $2,
                                                         $3->asFunctionRef(),
                                                         $4->asAggrArguments(),
                                                         $5->asDefinition() );
      }
    | T_CREATE T_AGGREGATE func_name old_aggr_definition
      {
        $$ = driver.makeAstNode<CreateOldAggregateStmtAST>( $1, $2,
                                                            $3->asFunctionRef(),
                                                            $4->asAggregateDefinition() );
      }
    | T_CREATE T_OPERATOR any_operator definition
      {
        $$ = driver.makeAstNode<CreateOpStmtAST>( $1, $2,
                                                  $3->asOperand(),
                                                  $4->asDefinition() );
      }
    | T_CREATE T_TYPE any_name definition
      {
        $$ = driver.makeAstNode<CreateTypeAsDefStmtAST>( $1, $2,
                                                         $3->asAnyIdentifier(),
                                                         $4->asDefinition() );
      }
    | T_CREATE T_TYPE any_name
      {
        $$ = driver.makeAstNode<CreateTypeAsDefStmtAST>( $1, $2,
                                                         $3->asAnyIdentifier(),
                                                         (DefinitionAST *) 0 );
      }
    | T_CREATE T_TYPE any_name T_AS T_LEFT_PAREN OptTableFuncElementList T_RIGHT_PAREN
      {
        $$ = driver.makeAstNode<CreateTypeAsRecordStmtAST>( $1, $2,
                                                            $3->asAnyIdentifier(),
                                                            $4, $5, $6, $7 );
      }
    | T_CREATE T_TYPE any_name T_AS T_ENUM T_LEFT_PAREN opt_enum_val_list T_RIGHT_PAREN
      {
        $$ = driver.makeAstNode<CreateTypeAsEnumStmtAST>( $1, $2,
                                                          $3->asAnyIdentifier(),
                                                          $4, $5, $6, $7, $8 );
      }
    | T_CREATE T_TYPE any_name T_AS T_RANGE definition
      {
        $$ = driver.makeAstNode<CreateTypeAsRangeStmtAST>( $1, $2,
                                                           $3->asAnyIdentifier(),
                                                           $4, $5,
                                                           $6->asDefinition() );
      }
    | T_CREATE T_TEXT T_SEARCH T_PARSER any_name definition
      {
        $$ = driver.makeAstNode<CreateSearchObjStmtAST>( $1, $2, $3, $4,
                                                         $5->asAnyIdentifier(),
                                                         $6->asDefinition() );
      }
    | T_CREATE T_TEXT T_SEARCH T_DICTIONARY any_name definition
      {
        $$ = driver.makeAstNode<CreateSearchObjStmtAST>( $1, $2, $3, $4,
                                                         $5->asAnyIdentifier(),
                                                         $6->asDefinition() );
      }
    | T_CREATE T_TEXT T_SEARCH T_TEMPLATE any_name definition
      {
        $$ = driver.makeAstNode<CreateSearchObjStmtAST>( $1, $2, $3, $4,
                                                         $5->asAnyIdentifier(),
                                                         $6->asDefinition() );
      }
    | T_CREATE T_TEXT T_SEARCH T_CONFIGURATION any_name definition
      {
        $$ = driver.makeAstNode<CreateSearchObjStmtAST>( $1, $2, $3, $4,
                                                         $5->asAnyIdentifier(),
                                                         $6->asDefinition() );
      }
    | T_CREATE T_COLLATION any_name definition
      {
        $$ = driver.makeAstNode<CreateCollationDefStmtAST>( $1, $2,
                                                            $3->asAnyIdentifier(),
                                                            $4->asDefinition() );
      }
    | T_CREATE T_COLLATION any_name T_FROM any_name
      {
        $$ = driver.makeAstNode<CreateCollationFromStmtAST>( $1, $2,
                                                             $3->asAnyIdentifier(),
                                                             $4,
                                                             $5->asAnyIdentifier() );
      }
    ;

OptTableFuncElementList:
      TableFuncElementList
    | /*EMPTY*/ { $$ = 0; }
    ;

opt_enum_val_list:
      enum_val_list
    | /*EMPTY*/ { $$ = 0; }
    ;

enum_val_list:
      Sconst
      { $$ = driver.makeAstNode<EnumValueListAST>( driver.makeAstNode<EnumValueAST>( $1 ) ); }
    | enum_val_list T_COMMA Sconst
      { $$ = driver.makeAstNode<EnumValueListAST>( $1, driver.makeAstNode<EnumValueAST>( $3 ) ); }
    ;

old_aggr_definition:
      T_LEFT_PAREN old_aggr_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<AggregateDefinitionAST>( $1, $2, $3 ); }
    ;

old_aggr_list:
      old_aggr_elem
      { $$ = driver.makeAstNode<AggrDefinitionListAST>( $1->asAggrDefinition() ); }
    | old_aggr_list T_COMMA old_aggr_elem
      { $$ = driver.makeAstNode<AggrDefinitionListAST>( $1, $3->asAggrDefinition() ); }
    ;

old_aggr_elem:
      T_IDENTIFIER T_EQUAL def_arg
      { $$ = driver.makeAstNode<AggrDefinitionAST>( $1, $2, $3->asDefinitionArgument() ); }
    ;

/*****************************************************************************
 *    QUERY:  CURSOR STATEMENTS
 *****************************************************************************/

DeclareCursorStmt:
      T_DECLARE cursor_name cursor_options T_CURSOR opt_hold T_FOR SelectStmt
      {
        $$ = driver.makeAstNode<DeclareCursorStmtAST>( $1,
                                                       driver.createName(NameAST::Unknown, $2),
                                                       $4,
                                                       $5->asCursorHoldOption(),
                                                       $6,
                                                       $7->asSelectStmt() );
      }
    ;

cursor_options: /*EMPTY*/
    | cursor_options T_NO T_SCROLL
    | cursor_options T_SCROLL
    | cursor_options T_BINARY
    | cursor_options T_INSENSITIVE
    ;

opt_hold:
      T_WITH T_HOLD
      { $$ = driver.makeAstNode<CursorHoldOptionAST>( $1, $2 ); }
    | T_WITHOUT T_HOLD
      { $$ = driver.makeAstNode<CursorHoldOptionAST>( $1, $2 ); }
    | empty_aka_optional
    ;

/*****************************************************************************
 *    QUERY:
 *        DEALLOCATE [PREPARE] <plan_name>
 *****************************************************************************/

DeallocateStmt:
      T_DEALLOCATE           name
      { $$ = driver.makeAstNode<DeallocateStmtAST>( $1,  0, $2 ); }
    | T_DEALLOCATE T_PREPARE name
      { $$ = driver.makeAstNode<DeallocateStmtAST>( $1, $2, $3 ); }
    | T_DEALLOCATE           T_ALL
      { $$ = driver.makeAstNode<DeallocateStmtAST>( $1,  0, $2 ); }
    | T_DEALLOCATE T_PREPARE T_ALL
      { $$ = driver.makeAstNode<DeallocateStmtAST>( $1, $2, $3 ); }
    ;

/*****************************************************************************
 *    CREATE DATABASE
 *****************************************************************************/

CreateDbStmt:
      T_CREATE T_DATABASE database_name opt_with createdb_opt_list
      { $$ = driver.makeAstNode<CreateDbStmtAST>( $1, $2, $3->asDatabaseName(), $4, $5 ); }
    ;

createdb_opt_list:
      createdb_opt_list createdb_opt_item
      { $$ = driver.makeAstNode<CreateDbOptionListAST>( $1, $2->asCreateDbOption() ); }
    | /* EMPTY */ { $$ = 0; }
    ;

createdb_opt_item:
      T_TABLESPACE opt_equal name
      { $$ = driver.makeAstNode<NamedCreateDbOptionAST>( $1, $2, driver.createName(NameAST::Unknown, $3) ); }
    | T_TABLESPACE opt_equal T_DEFAULT
      { $$ = driver.makeAstNode<ValDefCreateDbOptionAST>( $1,  0, $2, $3 ); }
    | T_LOCATION opt_equal Sconst
      { $$ = driver.makeAstNode<ValDefCreateDbOptionAST>( $1,  0, $2, $3 ); }
    | T_LOCATION opt_equal T_DEFAULT
      { $$ = driver.makeAstNode<ValDefCreateDbOptionAST>( $1,  0, $2, $3 ); }
    | T_TEMPLATE opt_equal name
      { $$ = driver.makeAstNode<NamedCreateDbOptionAST>( $1, $2, driver.createName(NameAST::Unknown, $3) ); }
    | T_TEMPLATE opt_equal T_DEFAULT
      { $$ = driver.makeAstNode<ValDefCreateDbOptionAST>( $1,  0, $2, $3 ); }
    | T_ENCODING opt_equal Sconst
      { $$ = driver.makeAstNode<ValDefCreateDbOptionAST>( $1,  0, $2, $3 ); }
    | T_ENCODING opt_equal Iconst
      { $$ = driver.makeAstNode<ValDefCreateDbOptionAST>( $1,  0, $2, $3 ); }
    | T_ENCODING opt_equal T_DEFAULT
      { $$ = driver.makeAstNode<ValDefCreateDbOptionAST>( $1,  0, $2, $3 ); }
    | T_LC_COLLATE opt_equal Sconst
      { $$ = driver.makeAstNode<ValDefCreateDbOptionAST>( $1,  0, $2, $3 ); }
    | T_LC_COLLATE opt_equal T_DEFAULT
      { $$ = driver.makeAstNode<ValDefCreateDbOptionAST>( $1,  0, $2, $3 ); }
    | T_LC_CTYPE opt_equal Sconst
      { $$ = driver.makeAstNode<ValDefCreateDbOptionAST>( $1,  0, $2, $3 ); }
    | T_LC_CTYPE opt_equal T_DEFAULT
      { $$ = driver.makeAstNode<ValDefCreateDbOptionAST>( $1,  0, $2, $3 ); }
    | T_CONNECTION T_LIMIT opt_equal SignedIconst
      { $$ = driver.makeAstNode<ValDefCreateDbOptionAST>( $1, $2, $3, $4 ); }
    | T_OWNER opt_equal name
      { $$ = driver.makeAstNode<NamedCreateDbOptionAST>( $1, $2, driver.createName(NameAST::Role, $3) ); }
    | T_OWNER opt_equal T_DEFAULT
      { $$ = driver.makeAstNode<ValDefCreateDbOptionAST>( $1,  0, $2, $3 ); }
    ;

/*****************************************************************************
 *    QUERY:
 *        CREATE USER MAPPING FOR auth_ident SERVER name [OPTIONS]
 *        ALTER USER MAPPING FOR auth_ident SERVER name OPTIONS
 *****************************************************************************/

CreateUserMappingStmt:
      T_CREATE T_USER T_MAPPING T_FOR auth_ident T_SERVER name create_generic_options
      {
        $$ = driver.makeAstNode<CreateUserMappingStmtAST>( $1,
                                                           $2,
                                                           $3,
                                                           $4,
                                                           $5->asAuthIdentifier(),
                                                           $6,
                                                           driver.createName(NameAST::Unknown, $7),
                                                           $8->asCreateGeneticOptions() );
      }
    ;

AlterUserMappingStmt:
      T_ALTER T_USER T_MAPPING T_FOR auth_ident T_SERVER name alter_generic_options
      {
        $$ = driver.makeAstNode<AlterUserMappingStmtAST>( $1,
                                                          $2,
                                                          $3,
                                                          $4,
                                                          $5->asAuthIdentifier(),
                                                          $6,
                                                          driver.createName(NameAST::Unknown, $7),
                                                          $8->asAlterTableGenericCmd() );
      }
    ;

auth_ident:
      T_CURRENT_USER
      { $$ = driver.makeAstNode<UserAuthIdentifierAST>( $1 ); }
    | T_USER
      { $$ = driver.makeAstNode<UserAuthIdentifierAST>( $1 ); }
    | RoleId
      { $$ = driver.makeAstNode<RoleAuthIdentifierAST>( $1->asRoleId() ); }
    ;

/*****************************************************************************
 *    QUERIES :
 *        CREATE TRIGGER ...
 *        DROP TRIGGER ...
 *****************************************************************************/

CreateTrigStmt:
      T_CREATE T_TRIGGER name TriggerActionTime TriggerEvents T_ON
      table_identifier TriggerForSpec TriggerWhen
      T_EXECUTE T_PROCEDURE func_name T_LEFT_PAREN TriggerFuncArgs T_RIGHT_PAREN
      {
        TriggerExecuteClauseAST *execAst =
             driver.makeAstNode<TriggerExecuteClauseAST>( $10, $11,
                                                          $12->asFunctionRef(),
                                                          $13, $14, $15 );
        $$ = driver.makeAstNode<CreateTrigStmtAST>( $1, $2,
                                                    driver.createName(NameAST::Trigger, $3),
                                                    $4->asTriggerActionTime(),
                                                    $5,
                                                    $6,
                                                    $7->asTableIdentifier(),
                                                    $8->asTriggerForSpec(),
                                                    $9->asTriggerWhen(),
                                                    execAst );
      }
    | T_CREATE T_CONSTRAINT T_TRIGGER name T_AFTER TriggerEvents T_ON
      table_identifier OptConstrFromTable ConstraintAttributeSpec
      T_FOR T_EACH T_ROW TriggerWhen
      T_EXECUTE T_PROCEDURE func_name T_LEFT_PAREN TriggerFuncArgs T_RIGHT_PAREN
      {
        TriggerExecuteClauseAST *execAst =
             driver.makeAstNode<TriggerExecuteClauseAST>( $15, $16,
                                                          $17->asFunctionRef(),
                                                          $18, $19, $20 );
        $$ = driver.makeAstNode<CreateConstrTrigStmtAST>( $1, $2, $3,
                                                          driver.createName(NameAST::Trigger, $4),
                                                          $5, $6, $7,
                                                          $8->asTableIdentifier(),
                                                          $9->asConstrFromTable(),
                                                          $10,
                                                          driver.makeAstNode<TriggerForSpecAST>( $11, $12, $13 ),
                                                          $14->asTriggerWhen(),
                                                          execAst );
      }
    ;

TriggerActionTime:
      T_BEFORE
      { $$ = driver.makeAstNode<TriggerActionTimeAST>( $1,  0 ); }
    | T_AFTER
      { $$ = driver.makeAstNode<TriggerActionTimeAST>( $1,  0 ); }
    | T_INSTEAD T_OF
      { $$ = driver.makeAstNode<TriggerActionTimeAST>( $1, $2 ); }
    ;

TriggerEvents:
      TriggerOneEvent
      { $$ = driver.makeAstNode<TriggerEventListAST>( $1->asTriggerEvent() ); }
    | TriggerEvents T_OR TriggerOneEvent
      { $$ = driver.makeAstNode<TriggerEventListAST>( $1, $3->asTriggerEvent() ); }
    ;

TriggerOneEvent:
      T_INSERT
      { $$ = driver.makeAstNode<TriggerEventAST>( $1, 0, (NameListAST *) 0 ); }
    | T_DELETE
      { $$ = driver.makeAstNode<TriggerEventAST>( $1, 0, (NameListAST *) 0 ); }
    | T_UPDATE
      { $$ = driver.makeAstNode<TriggerEventAST>( $1, 0, (NameListAST *) 0 ); }
    | T_UPDATE T_OF columnList
      { $$ = driver.makeAstNode<TriggerEventAST>( $1, $2, $3 ); }
    | T_TRUNCATE
      { $$ = driver.makeAstNode<TriggerEventAST>( $1, 0, (NameListAST *) 0 ); }
    ;

TriggerForSpec:
      T_FOR TriggerForOptEach TriggerForType
      { $$ = driver.makeAstNode<TriggerForSpecAST>( $1, $2, $3 ); }
    | empty_aka_optional
    ;

TriggerForOptEach:
    T_EACH
    | /*EMPTY*/ { $$ = 0; }
    ;

TriggerForType:
      T_ROW
    | T_STATEMENT
    ;

TriggerWhen:
      T_WHEN T_LEFT_PAREN a_expr T_RIGHT_PAREN
      { $$ = driver.makeAstNode<TriggerWhenAST>( $1, $2, $3->asExpression(), $4 ); }
    | empty_aka_optional
    ;

TriggerFuncArgs:
      TriggerFuncArg
      { $$ = driver.makeAstNode<TriggerFuncArgListAST>( $1->asTriggerFuncArg() ); }
    | TriggerFuncArgs T_COMMA TriggerFuncArg
      { $$ = driver.makeAstNode<TriggerFuncArgListAST>( $1, $3->asTriggerFuncArg() ); }
    | /*EMPTY*/ { $$ = 0; }
    ;

TriggerFuncArg:
      Iconst
      { $$ = driver.makeAstNode<TriggerFuncArgAST>( $1 ); }
    | Sconst
      { $$ = driver.makeAstNode<TriggerFuncArgAST>( $1 ); }
    | ColLabel
      { $$ = driver.makeAstNode<TriggerFuncArgAST>( $1 ); }
    ;

OptConstrFromTable:
      T_FROM table_identifier
      { $$ = driver.makeAstNode<ConstrFromTableAST>( $1, $2->asTableIdentifier() ); }
    | empty_aka_optional
    ;

/*****************************************************************************
 *    QUERY:
 *        CREATE TABLESPACE tablespace LOCATION '/path/to/tablespace/'
 *****************************************************************************/

CreateTableSpaceStmt:
      T_CREATE T_TABLESPACE name OptTableSpaceOwner T_LOCATION Sconst
      {
        $$ = driver.makeAstNode<CreateTableSpaceStmtAST>( $1,
                                                          $2,
                                                          driver.createName(NameAST::Unknown, $3),
                                                          $4->asTableSpaceOwnerClause(),
                                                          $5, $6 );
      }
    ;

OptTableSpaceOwner:
      T_OWNER name
      { $$ = driver.makeAstNode<TableSpaceOwnerClauseAST>( $1, driver.createName(NameAST::Role, $2) ); }
    | empty_aka_optional
    ;

/*****************************************************************************
 *    QUERY :
 *        CREATE TABLE relname
 *****************************************************************************/

CreateTableStmt:
      T_CREATE OptTemp T_TABLE                     table_identifier CreateTableClause CreateTableOptions
      {
        StatementAST::Info *info = driver.currentStatementInfo();
        info->type = StatementAST::Info::Create;
        info->target = StatementAST::Info::Table;
        TableIdentifierAST *tableIdent = $4->asTableIdentifier();
        if ( tableIdent ) {
          info->schemaName = tableIdent->schemaName;
          info->objectName = tableIdent->tableName;
        }
        $$ = driver.makeAstNode<CreateTableStmtAST>( $1, $2->asTempClause(), $3, 0, 0, 0, tableIdent, $5->asCreateTableClause(), $6->asCreateTableOptions() );
      }
    | T_CREATE OptTemp T_TABLE T_IF T_NOT T_EXISTS table_identifier CreateTableClause CreateTableOptions
      {
        StatementAST::Info *info = driver.currentStatementInfo();
        info->type = StatementAST::Info::Create;
        info->target = StatementAST::Info::Table;
        TableIdentifierAST *tableIdent = $7->asTableIdentifier();
        if ( tableIdent ) {
          info->schemaName = tableIdent->schemaName;
          info->objectName = tableIdent->tableName;
        }
        $$ = driver.makeAstNode<CreateTableStmtAST>( $1, $2->asTempClause(), $3, $4, $5, $6, tableIdent, $8->asCreateTableClause(), $9->asCreateTableOptions() );
      }
    ;

CreateTableClause:
      T_LEFT_PAREN OptTableElementList T_RIGHT_PAREN OptInherit
      { $$ = driver.makeAstNode<CreateTableDirecClauseAST>( $1, $2, $3, $4->asTableInheritClause() ); }
    | T_OF any_name OptTypedTableElementList
      { $$ = driver.makeAstNode<CreateTableOfClauseAST>( $1, $2->asAnyIdentifier(), $3->asTypedTableElements() ); }
    ;

CreateTableOptions:
      OptWith OnCommitOption OptTableSpace
      {
        $$ = driver.makeAstNode<CreateTableOptionsAST>( $1->asCreateWithOptions(),
                                                        $2->asOnCommitOption(),
                                                        $3->asTableSpaceClause() );
      }
    ;

OptInherit:
      T_INHERITS T_LEFT_PAREN table_identifier_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<TableInheritClauseAST>( $1, $2, $3, $4 ); }
    | empty_aka_optional
    ;

OptTypedTableElementList:
      T_LEFT_PAREN TypedTableElementList T_RIGHT_PAREN
      { $$ = driver.makeAstNode<TypedTableElementsAST>( $1, $2, $3 ); }
    | empty_aka_optional
    ;

TypedTableElementList:
      TypedTableElement
      { $$ = driver.makeAstNode<TypedTableElementListAST>( $1->asTypedTableElement() ); }
    | TypedTableElementList T_COMMA TypedTableElement
      { $$ = driver.makeAstNode<TypedTableElementListAST>( $1, $3->asTypedTableElement() ); }
    ;

TypedTableElement:
      columnOptions
    | TableConstraint
      { $$ = driver.makeAstNode<TypedTableConstrElementAST>( $1->asTableConstraint() ); }
    ;

columnOptions:
      ColId T_WITH T_OPTIONS ColQualList
      { $$ = driver.makeAstNode<TypedTableColElementAST>( driver.createName(NameAST::Column, $1), $2, $3, $4 ); }
    ;

/*****************************************************************************
 * Manipulate a schema
 *****************************************************************************/

CreateSchemaStmt:
      T_CREATE T_SCHEMA                     CreateSchemaClause
      { $$ = driver.makeAstNode<CreateSchemaStmtAST>( $1, $2,  0,  0,  0, $3->asCreateSchemaClause() ); }
    | T_CREATE T_SCHEMA T_IF T_NOT T_EXISTS CreateSchemaClause
      { $$ = driver.makeAstNode<CreateSchemaStmtAST>( $1, $2, $3, $4, $5, $6->asCreateSchemaClause() ); }
    ;

CreateSchemaClause:
      OptSchemaName T_AUTHORIZATION RoleId OptSchemaEltList
      {
        $$ = driver.makeAstNode<CreateSchemaClauseAST>( driver.createName(NameAST::Schema, $1),
                                                        $2,
                                                        $3->asRoleId(),
                                                        $4 );
      }
    | ColId                                OptSchemaEltList
      {
        $$ = driver.makeAstNode<CreateSchemaClauseAST>( driver.createName(NameAST::Schema, $1),
                                                        0,
                                                        (RoleIdAST *) 0,
                                                        $2 );
      }
    ;

OptSchemaName:
      ColId
    | /* EMPTY */ { $$ = 0; }
    ;

OptSchemaEltList:
      OptSchemaEltList schema_stmt
      { $$ = driver.makeAstNode<StatementListAST>( $1, $2->asStatement() ); }
    | /* EMPTY */ { $$ = 0; }
    ;

schema_stmt:
      CreateTableStmt
    | IndexStmt
    | CreateSeqStmt
    | CreateTrigStmt
    | GrantStmt
    | ViewStmt
    ;

/*****************************************************************************
 *    QUERIES :
 *        CREATE [OR REPLACE] [TRUSTED] [PROCEDURAL] LANGUAGE ...
 *****************************************************************************/

CreatePLangStmt:
      T_CREATE opt_or_replace opt_trusted opt_procedural T_LANGUAGE NonReservedWord_or_Sconst
      {
        $$ = driver.makeAstNode<CreatePLangStmtAST>( $1,
                                                     $2->asOrReplaceClause(),
                                                     $3, $4, $5, $6,
                                                     0,
                                                     (HandlerIdentifierAST *) 0,
                                                     (InlineHandlerClauseAST *) 0,
                                                     (ValidatorHandlerClauseAST *) 0 );
      }
    | T_CREATE opt_or_replace opt_trusted opt_procedural T_LANGUAGE NonReservedWord_or_Sconst T_HANDLER handler_name opt_inline_handler opt_validator
      {
        $$ = driver.makeAstNode<CreatePLangStmtAST>( $1,
                                                     $2->asOrReplaceClause(),
                                                     $3, $4, $5, $6,
                                                     $7,
                                                     $8->asHandlerIdentifier(),
                                                     $9->asInlineHandlerClause(),
                                                     $10->asValidatorHandlerClause() );
      }
    ;

opt_trusted:
      T_TRUSTED
    | /*EMPTY*/ { $$ = 0; }
    ;

opt_inline_handler:
      T_INLINE handler_name
      { $$ = driver.makeAstNode<InlineHandlerClauseAST>( $1, $2->asHandlerIdentifier() ); }
    | empty_aka_optional
    ;

validator_clause:
      T_VALIDATOR handler_name
      { $$ = driver.makeAstNode<ValidatorHandlerClauseAST>( $1, $2->asHandlerIdentifier() ); }
    | T_NO T_VALIDATOR
      { $$ = driver.makeAstNode<ValidatorHandlerClauseAST>( $2, (HandlerIdentifierAST *) 0 ); }
    ;

opt_validator:
      validator_clause
    | empty_aka_optional
    ;

/*****************************************************************************
 *    QUERIES :
 *        CREATE OPERATOR CLASS ...
 *        CREATE OPERATOR FAMILY ...
 *        ALTER OPERATOR FAMILY ...
 *****************************************************************************/

CreateOpClassStmt:
      T_CREATE T_OPERATOR T_CLASS any_name opt_default T_FOR T_TYPE Typename
      T_USING access_method opt_opfamily T_AS opclass_item_list
      {
        $$ = driver.makeAstNode<CreateOpClassStmtAST>( $1, $2, $3,
                                                       $4->asAnyIdentifier(),
                                                       $5, $6, $7,
                                                       $8->asType(),
                                                       $9, $10,
                                                       $11->asOpFamily(),
                                                       $12,
                                                       $13 );
      }
    ;

opclass_item_list:
      opclass_item
      { $$ = driver.makeAstNode<OpClassListAST>( $1->asOpClass() ); }
    | opclass_item_list T_COMMA opclass_item
      { $$ = driver.makeAstNode<OpClassListAST>( $1, $3->asOpClass() ); }
    ;

opclass_item:
      T_OPERATOR Iconst any_operator               opclass_purpose opt_recheck
      { $$ = driver.makeAstNode<OperatorOpClassAST>( $1, $2, $3->asOperand(), (OperArgTypesAST *)0, $4->asOpClassPurpose(), $5 ); }
    | T_OPERATOR Iconst any_operator oper_argtypes opclass_purpose opt_recheck
      { $$ = driver.makeAstNode<OperatorOpClassAST>( $1, $2, $3->asOperand(), $4->asOperArgTypes(), $5->asOpClassPurpose(), $6 ); }
    | T_FUNCTION Iconst                                      func_name func_args
      { $$ = driver.makeAstNode<FunctionOpClassAST>( $1, $2, 0, (TypeListAST *)0, 0, $3->asFunctionRef(), $4->asFunctionArgTypes() ); }
    | T_FUNCTION Iconst T_LEFT_PAREN type_list T_RIGHT_PAREN func_name func_args
      { $$ = driver.makeAstNode<FunctionOpClassAST>( $1, $2, $3, $4, $5, $6->asFunctionRef(), $7->asFunctionArgTypes() ); }
    | T_STORAGE Typename
      { $$ = driver.makeAstNode<StorageOpClassAST>( $1, $2->asType() ); }
    ;

opt_opfamily:
      T_FAMILY any_name
      { $$ = driver.makeAstNode<OpFamilyAST>( $1, $2->asAnyIdentifier() ); }
    | empty_aka_optional
    ;

opclass_purpose:
      T_FOR T_SEARCH
      { $$ = driver.makeAstNode<OpClassSearchPurposeAST>( $1, $2 ); }
    | T_FOR T_ORDER T_BY any_name
      { $$ = driver.makeAstNode<OpClassOrderPurposeAST>( $1, $2, $3, $4->asAnyIdentifier() ); }
    | empty_aka_optional
    ;

opt_recheck:
      T_RECHECK
    | /*EMPTY*/ { $$ = 0; }
    ;


CreateOpFamilyStmt:
      T_CREATE T_OPERATOR T_FAMILY any_name T_USING access_method
      {
        $$ = driver.makeAstNode<CreateOpFamilyStmtAST>( $1, $2, $3,
                                                        $4->asAnyIdentifier(),
                                                        $5,
                                                        $6 );
      }
    ;

AlterOpFamilyStmt:
      T_ALTER T_OPERATOR T_FAMILY any_name T_USING access_method T_ADD  opclass_item_list
      {
        $$ = driver.makeAstNode<AlterOpFamilyAddStmtAST>( $1, $2, $3,
                                                          $4->asAnyIdentifier(),
                                                          $5, $6, $7, $8 );
      }
    | T_ALTER T_OPERATOR T_FAMILY any_name T_USING access_method T_DROP opclass_drop_list
      {
        $$ = driver.makeAstNode<AlterOpFamilyDropStmtAST>( $1, $2, $3,
                                                           $4->asAnyIdentifier(),
                                                           $5, $6, $7, $8 );
      }
    ;

opclass_drop_list:
      opclass_drop
      { $$ = driver.makeAstNode<OpClassDropListAST>( $1->asOpClassDrop() ); }
    | opclass_drop_list T_COMMA opclass_drop
      { $$ = driver.makeAstNode<OpClassDropListAST>( $1, $3->asOpClassDrop() ); }
    ;

opclass_drop:
      T_OPERATOR Iconst T_LEFT_PAREN type_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<OpClassDropAST>( $1, $2, $3, $4, $5 ); }
    | T_FUNCTION Iconst T_LEFT_PAREN type_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<OpClassDropAST>( $1, $2, $3, $4, $5 ); }
    ;

/*****************************************************************************
 *    QUERY :
 *        CREATE MATERIALIZED VIEW relname AS SelectStmt
 *****************************************************************************/

CreateMatViewStmt:
      T_CREATE OptNoLog T_MATERIALIZED T_VIEW create_mv_target T_AS SelectStmt opt_with_data
      {
        $$ = driver.makeAstNode<CreateMatViewStmtAST>( $1, $2,
                                                       $3, $4,
                                                       $5->asCreateMvTarget(),
                                                       $6,
                                                       $7->asSelectStmt(),
                                                       $8->asWithDataClause() );
      }
    ;

create_mv_target:
      table_identifier opt_column_list opt_reloptions OptTableSpace
      {
        $$ = driver.makeAstNode<CreateMvTargetAST>( $1->asTableIdentifier(),
                                                    $2->asRefColumns(),
                                                    $3->asWithReloptions(),
                                                    $4->asTableSpaceClause() );
      }
    ;

OptNoLog:
      T_UNLOGGED
    | /*EMPTY*/ { $$ = 0; }
    ;

opt_reloptions:
      T_WITH reloptions
      { $$ = driver.makeAstNode<WithReloptionsAST>( $1, $2->asRelOptions() ); }
    | empty_aka_optional
    ;

/*****************************************************************************
 *    QUERY:
 *        create [or replace] function <fname>
 *                      [(<type-1> { , <type-n>})]
 *                      returns <type-r>
 *                      as <filename or code in language as appropriate>
 *                      language <lang> [with parameters]
 *****************************************************************************/

CreateFunctionStmt:
      T_CREATE opt_or_replace T_FUNCTION func_name func_args_with_defaults
      T_RETURNS func_return                                               createfunc_opt_list opt_definition
      {
        BaseFuncReturnAST *ret = driver.makeAstNode<FuncTypeFuncReturnAST>( $7->asFunctionType() );
        $$ = driver.makeAstNode<CreateFunctionStmtAST>( $1,
                                                        $2->asOrReplaceClause(),
                                                        $3,
                                                        $4->asFunctionRef(),
                                                        $5->asFuncArgsWithDefaults(),
                                                        $6,
                                                        ret,
                                                        $8,
                                                        $9->asConstraintDefinitions() );
      }
    | T_CREATE opt_or_replace T_FUNCTION func_name func_args_with_defaults
      T_RETURNS T_TABLE T_LEFT_PAREN table_func_column_list T_RIGHT_PAREN createfunc_opt_list opt_definition
      {
        BaseFuncReturnAST *ret = driver.makeAstNode<TableFuncReturnAST>( $7, $8, $9, $10 );
        $$ = driver.makeAstNode<CreateFunctionStmtAST>( $1,
                                                        $2->asOrReplaceClause(),
                                                        $3,
                                                        $4->asFunctionRef(),
                                                        $5->asFuncArgsWithDefaults(),
                                                        $6,
                                                        ret,
                                                        $11,
                                                        $12->asConstraintDefinitions() );
      }
    | T_CREATE opt_or_replace T_FUNCTION func_name func_args_with_defaults
                                                                          createfunc_opt_list opt_definition
      {
        $$ = driver.makeAstNode<CreateFunctionStmtAST>( $1,
                                                        $2->asOrReplaceClause(),
                                                        $3,
                                                        $4->asFunctionRef(),
                                                        $5->asFuncArgsWithDefaults(),
                                                        0,
                                                        (BaseFuncReturnAST *) 0,
                                                        $6,
                                                        $7->asConstraintDefinitions() );
      }
    ;

opt_or_replace:
      T_OR T_REPLACE
      { $$ = driver.makeAstNode<OrReplaceClauseAST>( $1,$2 ); }
    | empty_aka_optional
    ;

createfunc_opt_list:
      createfunc_opt_item
      { $$ = driver.makeAstNode<CreateFuncOptionListAST>( $1->asCreateFuncOption() ); }
    | createfunc_opt_list createfunc_opt_item
      { $$ = driver.makeAstNode<CreateFuncOptionListAST>( $1, $2->asCreateFuncOption() ); }
    ;

createfunc_opt_item:
      T_AS func_as
      { $$ = driver.makeAstNode<CreateFuncAsOptionAST>( $1, $2->asFunctionAsClause() ); }
    | T_LANGUAGE NonReservedWord_or_Sconst
      { $$ = driver.makeAstNode<CreateFuncLangOptionAST>( $1, $2 ); }
    | T_WINDOW
      { $$ = driver.makeAstNode<CreateFuncWindowOptionAST>( $1 ); }
    | common_func_opt_item
      { $$ = driver.makeAstNode<CreateFuncCommonOptionAST>( $1->asCommonAlterFunctionOpt() ); }
    ;

func_as:
      Sconst
      { $$ = driver.makeAstNode<FunctionAsClauseAST>( $1,  0,  0 ); }
    | Sconst T_COMMA Sconst
      { $$ = driver.makeAstNode<FunctionAsClauseAST>( $1, $2, $3 ); }
    ;

func_args_with_defaults:
      T_LEFT_PAREN func_args_with_defaults_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<FuncArgsWithDefaultsAST>( $1, $2, $3 ); }
    | T_LEFT_PAREN T_RIGHT_PAREN
      { $$ = driver.makeAstNode<FuncArgsWithDefaultsAST>( $1, (FuncArgWithDefaultListAST *) 0, $2 ); }
    ;

func_args_with_defaults_list:
      func_arg_with_default
      { $$ = driver.makeAstNode<FuncArgWithDefaultListAST>( $1->asFuncArgWithDefault() ); }
    | func_args_with_defaults_list T_COMMA func_arg_with_default
      { $$ = driver.makeAstNode<FuncArgWithDefaultListAST>( $1, $3->asFuncArgWithDefault() ); }
    ;

func_arg_with_default:
      func_arg
      { $$ = driver.makeAstNode<FuncArgWithDefaultAST>( $1->asFunctionArg(),  0, (ExpressionAST *) 0 ); }
    | func_arg T_DEFAULT a_expr
      { $$ = driver.makeAstNode<FuncArgWithDefaultAST>( $1->asFunctionArg(), $2, $3->asExpression() ); }
    | func_arg T_EQUAL a_expr
      { $$ = driver.makeAstNode<FuncArgWithDefaultAST>( $1->asFunctionArg(), $2, $3->asExpression() ); }
    ;

func_return:
      func_type
    ;

table_func_column:
      param_name func_type
      {
        $$ = driver.makeAstNode<TableFuncColumnAST>( driver.createName(NameAST::Unknown, $1),
                                                     $2->asFunctionType() );
      }
    ;

table_func_column_list:
      table_func_column
      { $$ = driver.makeAstNode<TableFuncColumnListAST>( $1->asTableFuncColumn() ); }
    | table_func_column_list T_COMMA table_func_column
      { $$ = driver.makeAstNode<TableFuncColumnListAST>( $1, $3->asTableFuncColumn() ); }
    ;

/*****************************************************************************
 *    QUERY:
 *       CREATE FOREIGN TABLE relname (...) SERVER name (...)
 *****************************************************************************/

CreateForeignTableStmt:
      T_CREATE T_FOREIGN T_TABLE table_identifier T_LEFT_PAREN OptTableElementList T_RIGHT_PAREN T_SERVER name create_generic_options
      {
        $$ = driver.makeAstNode<CreateForeignTableStmtAST>( $1, $2, $3,
                                                            0, 0, 0,
                                                            $4->asTableIdentifier(),
                                                            $5,
                                                            $6,
                                                            $7,
                                                            $8,
                                                            driver.createName(NameAST::Unknown, $9),
                                                            $10->asCreateGeneticOptions() );
      }
    | T_CREATE T_FOREIGN T_TABLE T_IF T_NOT T_EXISTS table_identifier T_LEFT_PAREN OptTableElementList T_RIGHT_PAREN T_SERVER name create_generic_options
      {
        $$ = driver.makeAstNode<CreateForeignTableStmtAST>( $1, $2, $3,
                                                            $4, $5, $6,
                                                            $7->asTableIdentifier(),
                                                            $8,
                                                            $9,
                                                            $10,
                                                            $11,
                                                            driver.createName(NameAST::Unknown, $12),
                                                            $13->asCreateGeneticOptions() );
      }
    ;

OptTableElementList:
      TableElementList
    | /*EMPTY*/ { $$ = 0; }
    ;

TableElementList:
      TableElement
      { $$ = driver.makeAstNode<TableElementListAST>( $1->asTableElement() ); }
    | TableElementList T_COMMA TableElement
      { $$ = driver.makeAstNode<TableElementListAST>( $1, $3->asTableElement() ); }
    ;

TableElement:
      columnDef
      { $$ = driver.makeAstNode<ColumnDefTableElementAST>( $1->asColumnDef() ); }
    | TableLikeClause
      { $$ = driver.makeAstNode<LikeTableElementAST>( $1->asTableLikeClause() ); }
    | TableConstraint
      { $$ = driver.makeAstNode<ConstraintTableElementAST>( $1->asTableConstraint() ); }
    ;

TableLikeClause:
      T_LIKE table_identifier TableLikeOptionList
      { $$ = driver.makeAstNode<TableLikeClauseAST>( $1, $2->asTableIdentifier(), $3 ); }
    ;

TableLikeOptionList:
      TableLikeOptionList T_INCLUDING TableLikeOption
      { $$ = driver.makeAstNode<TableLikeOptionListAST>( $1, $3->asTableLikeOption() ); }
    | TableLikeOptionList T_EXCLUDING TableLikeOption
      { $$ = driver.makeAstNode<TableLikeOptionListAST>( $1, $3->asTableLikeOption() ); }
    | /* EMPTY */ { $$ = 0; }
    ;

TableLikeOption:
      T_DEFAULTS
      { $$ = driver.makeAstNode<TableLikeOptionAST>( $1 ); }
    | T_CONSTRAINTS
      { $$ = driver.makeAstNode<TableLikeOptionAST>( $1 ); }
    | T_INDEXES
      { $$ = driver.makeAstNode<TableLikeOptionAST>( $1 ); }
    | T_STORAGE
      { $$ = driver.makeAstNode<TableLikeOptionAST>( $1 ); }
    | T_COMMENTS
      { $$ = driver.makeAstNode<TableLikeOptionAST>( $1 ); }
    | T_ALL
      { $$ = driver.makeAstNode<TableLikeOptionAST>( $1 ); }
    ;

/*****************************************************************************
 *    QUERY:
 *       CREATE SERVER name [TYPE] [VERSION] [OPTIONS]
 *****************************************************************************/

CreateForeignServerStmt:
      T_CREATE T_SERVER name opt_type opt_foreign_server_version T_FOREIGN T_DATA T_WRAPPER name create_generic_options
      {
        $$ = driver.makeAstNode<CreateForeignServerStmtAST>( $1,
                                                             $2,
                                                             driver.createName(NameAST::Unknown, $3),
                                                             $4->asServerTypeClause(),
                                                             $5->asForeignServerVersion(),
                                                             $6, $7, $8,
                                                             driver.createName(NameAST::Unknown, $9),
                                                             $10->asCreateGeneticOptions() );
      }
    ;

opt_type:
      T_TYPE Sconst
      { $$ = driver.makeAstNode<ServerTypeClauseAST>( $1, $2 ); }
    | empty_aka_optional
    ;

opt_foreign_server_version:
      foreign_server_version
    | empty_aka_optional
    ;

/*****************************************************************************
 *    QUERY:
 *        CREATE FOREIGN DATA WRAPPER name options
 *****************************************************************************/

CreateFdwStmt:
      T_CREATE T_FOREIGN T_DATA T_WRAPPER name opt_fdw_options create_generic_options
      {
        $$ = driver.makeAstNode<CreateFdwStmtAST>( $1, $2, $3, $4,
                                                   driver.createName(NameAST::Unknown, $5),
                                                   $6,
                                                   $7->asCreateGeneticOptions() );
      }
    ;


/*****************************************************************************
 *    QUERY:
 *       CREATE EXTENSION extension
 *         [ WITH ] [ SCHEMA schema ] [ VERSION version ] [ FROM oldversion ]
 *****************************************************************************/

CreateExtensionStmt:
      T_CREATE T_EXTENSION                     name opt_with create_extension_opt_list
      {
        $$ = driver.makeAstNode<CreateExtensionStmtAST>( $1,
                                                         $2,
                                                         0, 0, 0,
                                                         driver.createName(NameAST::Unknown, $3),
                                                         $4,
                                                         $5 );
      }
    | T_CREATE T_EXTENSION T_IF T_NOT T_EXISTS name opt_with create_extension_opt_list
      {
        $$ = driver.makeAstNode<CreateExtensionStmtAST>( $1,
                                                         $2,
                                                         $3, $4, $5,
                                                         driver.createName(NameAST::Unknown, $6),
                                                         $7,
                                                         $8 );
      }
    ;

create_extension_opt_list:
      create_extension_opt_list create_extension_opt_item
      { $$ = driver.makeAstNode<CreateExtOptionListAST>( $1, $2->asCreateExtOption() ); }
    | /* EMPTY */ { $$ = 0; }
    ;

create_extension_opt_item:
      T_SCHEMA name
      { $$ = driver.makeAstNode<CreateExtSchemaOptionAST>( $1, driver.createName(NameAST::Schema, $2) ); }
    | T_VERSION NonReservedWord_or_Sconst
      { $$ = driver.makeAstNode<CreateExtVerFromOptionAST>( $1, $2 ); }
    | T_FROM NonReservedWord_or_Sconst
      { $$ = driver.makeAstNode<CreateExtVerFromOptionAST>( $1, $2 ); }
    ;

/*****************************************************************************
 *    CREATE [DEFAULT] CONVERSION <conversion_name>
 *      FOR <encoding_name> TO <encoding_name> FROM <func_name>
 *****************************************************************************/

CreateConversionStmt:
      T_CREATE opt_default T_CONVERSION any_name T_FOR Sconst T_TO Sconst T_FROM any_name
      {
        $$ = driver.makeAstNode<CreateConversionStmtAST>( $1,
                                                          $2,
                                                          $3,
                                                          $4->asAnyIdentifier(),
                                                          $5,
                                                          $6,
                                                          $7,
                                                          $8,
                                                          $9,
                                                          $10->asAnyIdentifier() );
      }
    ;

opt_default:
      T_DEFAULT
    | /*EMPTY*/ { $$ = 0; }
    ;

/*****************************************************************************
 *    CREATE CAST / DROP CAST
 *****************************************************************************/

CreateCastStmt:
      T_CREATE T_CAST T_LEFT_PAREN Typename T_AS Typename T_RIGHT_PAREN T_WITH T_FUNCTION function_with_argtypes cast_context
      {
        $$ = driver.makeAstNode<CreateCastStmtAST>( $1,
                                                    $2,
                                                    $3,
                                                    $4->asType(),
                                                    $5,
                                                    $6->asType(),
                                                    $7,
                                                    $8,
                                                    $9,
                                                    $10->asFunctionWithArgTypes(),
                                                    $11->asCastContext() );
      }
    | T_CREATE T_CAST T_LEFT_PAREN Typename T_AS Typename T_RIGHT_PAREN T_WITHOUT T_FUNCTION cast_context
      {
        $$ = driver.makeAstNode<CreateCastStmtAST>( $1,
                                                    $2,
                                                    $3,
                                                    $4->asType(),
                                                    $5,
                                                    $6->asType(),
                                                    $7,
                                                    $8,
                                                    $9,
                                                    (FunctionWithArgTypesAST *) 0,
                                                    $10->asCastContext() );
      }
    | T_CREATE T_CAST T_LEFT_PAREN Typename T_AS Typename T_RIGHT_PAREN T_WITH T_INOUT cast_context
      {
        $$ = driver.makeAstNode<CreateCastStmtAST>( $1,
                                                    $2,
                                                    $3,
                                                    $4->asType(),
                                                    $5,
                                                    $6->asType(),
                                                    $7,
                                                    $8,
                                                    $9,
                                                    (FunctionWithArgTypesAST *) 0,
                                                    $10->asCastContext() );
      }
    ;

cast_context:
      T_AS T_IMPLICIT
      { $$ = driver.makeAstNode<CastContextAST>( $1, $2 ); }
    | T_AS T_ASSIGNMENT
      { $$ = driver.makeAstNode<CastContextAST>( $1, $2 ); }
    | empty_aka_optional
    ;

opt_if_exists:
      T_IF T_EXISTS
      { $$ = driver.makeAstNode<IfExistsAST>( $1, $2 ); }
    | empty_aka_optional
    ;

/*****************************************************************************
 *    QUERIES :
 *        CREATE ASSERTION ...
 *****************************************************************************/

CreateAssertStmt:
      T_CREATE T_ASSERTION name T_CHECK T_LEFT_PAREN a_expr T_RIGHT_PAREN ConstraintAttributeSpec
      {
        $$ = driver.makeAstNode<CreateAssertStmtAST>( $1,
                                                      $2,
                                                      driver.createName(NameAST::Unknown, $3),
                                                      $4,
                                                      $5,
                                                      $6->asExpression(),
                                                      $7, $8 );
      }
    ;

/*****************************************************************************
 *    QUERY :
 *        CREATE TABLE relname AS SelectStmt [ WITH [NO] DATA ]
 *****************************************************************************/

CreateTableAsStmt:
      T_CREATE OptTemp T_TABLE create_as_target T_AS SelectStmt opt_with_data
      {
        $$ = driver.makeAstNode<CreateTableAsStmtAST>( $1,
                                                       $2->asTempClause(),
                                                       $3,
                                                       $4->asCreateAsTarget(),
                                                       $5,
                                                       $6->asSelectStmt(),
                                                       $7->asWithDataClause() );
      }
    ;

create_as_target:
      table_identifier opt_column_list OptWith OnCommitOption OptTableSpace
      {
        $$ = driver.makeAstNode<CreateAsTargetAST>( $1->asTableIdentifier(),
                                                    $2->asRefColumns(),
                                                    $3->asCreateWithOptions(),
                                                    $4->asOnCommitOption(),
                                                    $5->asTableSpaceClause() );
      }
    ;

opt_with_data:
      T_WITH T_DATA
      { $$ = driver.makeAstNode<WithDataClauseAST>( $1,  0, $2 ); }
    | T_WITH T_NO T_DATA
      { $$ = driver.makeAstNode<WithDataClauseAST>( $1, $2, $3 ); }
    | empty_aka_optional
    ;

OnCommitOption:
      T_ON T_COMMIT T_DROP
      { $$ = driver.makeAstNode<OnCommitOptionAST>( $1, $2, $3,  0 ); }
    | T_ON T_COMMIT T_DELETE T_ROWS
      { $$ = driver.makeAstNode<OnCommitOptionAST>( $1, $2, $3, $4 ); }
    | T_ON T_COMMIT T_PRESERVE T_ROWS
      { $$ = driver.makeAstNode<OnCommitOptionAST>( $1, $2, $3, $4 ); }
    | empty_aka_optional
    ;

OptTableSpace:
      T_TABLESPACE name
      { $$ = driver.makeAstNode<TableSpaceClauseAST>( $1, driver.createName(NameAST::Unknown, $2) ); }
    | empty_aka_optional
    ;


OptWith:
      T_WITH reloptions
      { $$ = driver.makeAstNode<CreateWithRelOptionsAST>( $1, $2->asRelOptions() ); }
    | T_WITH T_OIDS
      { $$ = driver.makeAstNode<CreateWithOidsOptionsAST>( $1, $2 ); }
    | T_WITHOUT T_OIDS
      { $$ = driver.makeAstNode<CreateWithOidsOptionsAST>( $1, $2 ); }
    | empty_aka_optional
    ;

/*****************************************************************************
 *    QUERY :
 *        COPY relname [(columnList)] FROM/TO file [WITH] [(options)]
 *        COPY ( SELECT ... ) TO file [WITH] [(options)]
 *****************************************************************************/

CopyStmt:
      T_COPY opt_binary table_identifier opt_column_list opt_oids copy_from opt_program copy_file_name copy_delimiter opt_with copy_options
      {
        $$ = driver.makeAstNode<TableCopyStmtAST>( $1,
                                                   $2,
                                                   $3->asTableIdentifier(),
                                                   $4->asRefColumns(),
                                                   $5->asWithOidsClause(),
                                                   $6,
                                                   $7,
                                                   $8,
                                                   $9->asCopyDelimiter(),
                                                   $10,
                                                   $11->asCopyOptions() );
      }
    | T_COPY select_with_parens T_TO opt_program copy_file_name opt_with copy_options
      {
        $$ = driver.makeAstNode<SelectCopyStmtAST>( $1,
                                                    $2->asSelectWithParens(),
                                                    $3,
                                                    $4,
                                                    $5,
                                                    $6,
                                                    $7->asCopyOptions() );
      }
    ;

copy_from:
      T_FROM
    | T_TO
    ;

opt_program:
      T_PROGRAM
    | /* EMPTY */ { $$ = 0; }
    ;

copy_file_name:
      Sconst
    | T_STDIN
    | T_STDOUT
    ;

copy_options:
      copy_opt_list
      { $$ = driver.makeAstNode<OldCopyOptionsAST>( $1 ); }
    | T_LEFT_PAREN copy_generic_opt_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<NewCopyOptionsAST>( $1, $2, $3 ); }
    ;

copy_opt_list:
      copy_opt_list copy_opt_item
      { $$ = driver.makeAstNode<CopyOptionListAST>( $1, $2->asCopyOption() ); }
    | /* EMPTY */ { $$ = 0; }
    ;

copy_opt_item:
      T_BINARY
      { $$ = driver.makeAstNode<CopyOptionAST>( $1,  0,  0, (NameListAST *) 0 ); }
    | T_OIDS
      { $$ = driver.makeAstNode<CopyOptionAST>( $1,  0,  0, (NameListAST *) 0 ); }
    | T_FREEZE
      { $$ = driver.makeAstNode<CopyOptionAST>( $1,  0,  0, (NameListAST *) 0 ); }
    | T_DELIMITER opt_as Sconst
      { $$ = driver.makeAstNode<CopyOptionAST>( $1, $2, $3, (NameListAST *) 0 ); }
    | T_NULL opt_as Sconst
      { $$ = driver.makeAstNode<CopyOptionAST>( $1, $2, $3, (NameListAST *) 0 ); }
    | T_CSV
      { $$ = driver.makeAstNode<CopyOptionAST>( $1,  0,  0, (NameListAST *) 0 ); }
    | T_HEADER
      { $$ = driver.makeAstNode<CopyOptionAST>( $1,  0,  0, (NameListAST *) 0 ); }
    | T_QUOTE opt_as Sconst
      { $$ = driver.makeAstNode<CopyOptionAST>( $1, $2, $3, (NameListAST *) 0 ); }
    | T_ESCAPE opt_as Sconst
      { $$ = driver.makeAstNode<CopyOptionAST>( $1, $2, $3, (NameListAST *) 0 ); }
    | T_FORCE T_QUOTE columnList
      { $$ = driver.makeAstNode<CopyOptionAST>( $1, $2,  0, $3 ); }
    | T_FORCE T_QUOTE T_STAR
      { $$ = driver.makeAstNode<CopyOptionAST>( $1, $2, $3, (NameListAST *) 0 ); }
    | T_FORCE T_NOT T_NULL columnList
      { $$ = driver.makeAstNode<CopyOptionAST>( $1, $2, $3, $4 ); }
    | T_ENCODING Sconst
      { $$ = driver.makeAstNode<CopyOptionAST>( $1, $2,  0, (NameListAST *) 0 ); }
    ;

opt_binary:
      T_BINARY
    | /*EMPTY*/ { $$ = 0; }
    ;

opt_oids:
      T_WITH T_OIDS
      { $$ = driver.makeAstNode<WithOidsClauseAST>( $1, $2 ); }
    | empty_aka_optional
    ;

copy_delimiter:
      opt_using T_DELIMITERS Sconst
      { $$ = driver.makeAstNode<CopyDelimiterAST>( $1, $2, $3 ); }
    | empty_aka_optional
    ;

opt_using:
      T_USING
    | /*EMPTY*/ { $$ = 0; }
    ;

copy_generic_opt_list:
      copy_generic_opt_elem
      { $$ = driver.makeAstNode<GenericCopyOptionListAST>( $1->asGenericCopyOption() ); }
    | copy_generic_opt_list T_COMMA copy_generic_opt_elem
      { $$ = driver.makeAstNode<GenericCopyOptionListAST>( $1, $3->asGenericCopyOption() ); }
    ;

copy_generic_opt_elem:
      ColLabel copy_generic_opt_arg
      { $$ = driver.makeAstNode<GenericCopyOptionAST>( $1, $2->asGenericCopyOptionArg() ); }
    ;

copy_generic_opt_arg:
      opt_boolean_or_string
      { $$ = driver.makeAstNode<GenericCopyOptionValArgAST>( $1 ); }
    | NumericOnly
      { $$ = driver.makeAstNode<GenericCopyOptionValArgAST>( $1 ); }
    | T_STAR
      { $$ = driver.makeAstNode<GenericCopyOptionValArgAST>( $1 ); }
    | T_LEFT_PAREN copy_generic_opt_arg_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<GenericCopyOptionCmpArgAST>( $1, $2, $3 ); }
    | empty_aka_optional
    ;

copy_generic_opt_arg_list:
      copy_generic_opt_arg_list_item
      { $$ = driver.makeAstNode<GenericCopyOptionCmpArgItemListAST>( $1->asGenericCopyOptionCmpArgItem() ); }
    | copy_generic_opt_arg_list T_COMMA copy_generic_opt_arg_list_item
      { $$ = driver.makeAstNode<GenericCopyOptionCmpArgItemListAST>( $1, $3->asGenericCopyOptionCmpArgItem() ); }
    ;

copy_generic_opt_arg_list_item:
      opt_boolean_or_string
      { $$ = driver.makeAstNode<GenericCopyOptionCmpArgItemAST>( $1 ); }
    ;

/*****************************************************************************
 *  SET CONSTRAINTS ...
 *****************************************************************************/

ConstraintsSetStmt:
      T_SET T_CONSTRAINTS constraints_set_list constraints_set_mode
      { $$ = driver.makeAstNode<ConstraintsSetStmtAST>( $1, $2, $3->asConstraintsSetList(), $4 ); }
    ;

constraints_set_list:
      T_ALL
      { $$ = driver.makeAstNode<AllConstraintsSetListAST>( $1 ); }
    | constraint_identifier_list
      { $$ = driver.makeAstNode<CommonConstraintsSetListAST>( $1 ); }
    ;

constraint_identifier_list:
      constraint_identifier
      { $$ = driver.makeAstNode<ConstraintIdentifierListAST>( $1->asConstraintIdentifier() ); }
    | constraint_identifier_list T_COMMA constraint_identifier
      { $$ = driver.makeAstNode<ConstraintIdentifierListAST>( $1, $3->asConstraintIdentifier() ); }
    ;

constraints_set_mode:
      T_DEFERRED
    | T_IMMEDIATE
    ;

/*****************************************************************************
 *  The COMMENT ON ...
 *****************************************************************************/

comment:
      T_COMMENT { driver.currentStatementInfo()->type = StatementAST::Info::Comment; }
    ;

CommentStmt:
      comment T_ON CommentObject CommentTextClause
      { $$ = driver.makeAstNode<CommonCommentStmtAST>( $1, $2, $3->asCommentObject(), $4->asCommentTextClause() ); }
    | comment T_ON T_AGGREGATE func_name aggr_args CommentTextClause
      {
        $$ = driver.makeAstNode<AggrCommentStmtAST>( $1, $2,
                                                     $3,
                                                     $4->asFunctionRef(),
                                                     $5->asAggrArguments(),
                                                     $6->asCommentTextClause() );
      }
    | comment T_ON T_FUNCTION func_name func_args CommentTextClause
      {
        $$ = driver.makeAstNode<FunctionCommentStmtAST>( $1, $2,
                                                         $3,
                                                         $4->asFunctionRef(),
                                                         $5->asFunctionArgTypes(),
                                                         $6->asCommentTextClause() );
      }
    | comment T_ON T_OPERATOR any_operator oper_argtypes CommentTextClause
      {
        $$ = driver.makeAstNode<OpCommentStmtAST>( $1, $2,
                                                   $3,
                                                   $4->asOperand(),
                                                   $5->asOperArgTypes(),
                                                   $6->asCommentTextClause() );
      }
    | comment T_ON T_CONSTRAINT name CommentOnName CommentTextClause
      {
        $$ = driver.makeAstNode<ConstraintCommentStmtAST>( $1, $2,
                                                           $3,
                                                           driver.createName(NameAST::Constraint, $4),
                                                           $5->asCommentOnName(),
                                                           $6->asCommentTextClause() );
      }
    | comment T_ON T_RULE name CommentOnName CommentTextClause
      {
        $$ = driver.makeAstNode<RuleCommentStmtAST>( $1, $2,
                                                     $3,
                                                     driver.createName(NameAST::Unknown, $4),
                                                     $5->asCommentOnName(),
                                                     $6->asCommentTextClause() );
      }
    | comment T_ON T_RULE name CommentTextClause
      {
        $$ = driver.makeAstNode<RuleCommentStmtAST>( $1, $2,
                                                     $3,
                                                     driver.createName(NameAST::Unknown, $4),
                                                     (CommentOnNameAST *) 0,
                                                     $5->asCommentTextClause() );
      }
    | comment T_ON T_TRIGGER name CommentOnName CommentTextClause
      {
        $$ = driver.makeAstNode<TriggerCommentStmtAST>( $1, $2,
                                                        $3,
                                                        driver.createName(NameAST::Trigger, $4),
                                                        $5->asCommentOnName(),
                                                        $6->asCommentTextClause() );
      }
    | comment T_ON T_OPERATOR T_CLASS any_name T_USING access_method CommentTextClause
      {
        $$ = driver.makeAstNode<OpUsingCommentStmtAST>( $1, $2,
                                                        $3,
                                                        $4,
                                                        $5->asAnyIdentifier(),
                                                        $6,
                                                        $7,
                                                        $8->asCommentTextClause() );
      }
    | comment T_ON T_OPERATOR T_FAMILY any_name T_USING access_method CommentTextClause
      {
        $$ = driver.makeAstNode<OpUsingCommentStmtAST>( $1, $2,
                                                        $3,
                                                        $4,
                                                        $5->asAnyIdentifier(),
                                                        $6,
                                                        $7,
                                                        $8->asCommentTextClause() );
      }
    | comment T_ON T_LARGE T_OBJECT NumericOnly CommentTextClause
      {
        $$ = driver.makeAstNode<LargeObjCommentStmtAST>( $1, $2,
                                                         $3,
                                                         $4,
                                                         $5,
                                                         $6->asCommentTextClause() );
      }
    | comment T_ON T_CAST T_LEFT_PAREN Typename T_AS Typename T_RIGHT_PAREN CommentTextClause
      {
        $$ = driver.makeAstNode<CastCommentStmtAST>( $1, $2,
                                                     $3,
                                                     $4,
                                                     $5->asType(),
                                                     $6,
                                                     $7->asType(),
                                                     $8,
                                                     $9->asCommentTextClause() );
      }
    | comment T_ON opt_procedural T_LANGUAGE any_name CommentTextClause
      {
        $$ = driver.makeAstNode<LangCommentStmtAST>( $1, $2,
                                                     $3,
                                                     $4,
                                                     $5->asAnyIdentifier(),
                                                     $6->asCommentTextClause() );
      }
    ;

CommentOnName:
      CH_on_before_table_identifier table_identifier_we
      { $$ = driver.makeAstNode<CommentOnNameAST>( $1, $2->asTableIdentifier() ); }
    ;

CommentObject:
      target_column column_identifier
      { $$ = driver.makeAstNode<ColumnCommentObjectAST>( $1, $2->asColumnIdentifier() ); }
    | T_DATABASE name
      { $$ = driver.makeAstNode<NamedCommentObjectAST>( $1, driver.createName(NameAST::Unknown, $2) ); }
    | target_schema name
      { $$ = driver.makeAstNode<SchemaCommentObjectAST>( $1, driver.createName(NameAST::Schema, $2) ); }
    | target_index index_identifier
      { $$ = driver.makeAstNode<IndexCommentObjectAST>( $1, $2->asIndexIdentifier() ); }
    | target_sequence sequence_identifier
      { $$ = driver.makeAstNode<SequenceCommentObjectAST>( $1, $2->asSequenceIdentifier() ); }
    | target_table table_identifier
      { $$ = driver.makeAstNode<TableCommentObjectAST>(  0, $1, $2->asTableIdentifier() ); }
    | T_DOMAIN any_name
      { $$ = driver.makeAstNode<AnyCommentObjectAST>( $1, $2->asAnyIdentifier() ); }
    | T_TYPE any_name
      { $$ = driver.makeAstNode<AnyCommentObjectAST>( $1, $2->asAnyIdentifier() ); }
    | target_view table_identifier
      { $$ = driver.makeAstNode<TableCommentObjectAST>(  0, $1, $2->asTableIdentifier() ); }
    | T_MATERIALIZED target_view table_identifier
      { $$ = driver.makeAstNode<TableCommentObjectAST>( $1, $2, $3->asTableIdentifier() ); }
    | T_COLLATION any_name
      { $$ = driver.makeAstNode<AnyCommentObjectAST>( $1, $2->asAnyIdentifier() ); }
    | T_CONVERSION any_name
      { $$ = driver.makeAstNode<AnyCommentObjectAST>( $1, $2->asAnyIdentifier() ); }
    | T_TABLESPACE any_name
      { $$ = driver.makeAstNode<AnyCommentObjectAST>( $1, $2->asAnyIdentifier() ); }
    | T_EXTENSION any_name
      { $$ = driver.makeAstNode<AnyCommentObjectAST>( $1, $2->asAnyIdentifier() ); }
    | T_ROLE RoleId
      { $$ = driver.makeAstNode<RoleCommentObjectAST>( $1, $2->asRoleId() ); }
    | T_FOREIGN T_TABLE table_identifier
      { $$ = driver.makeAstNode<TableCommentObjectAST>( $1, $2, $3->asTableIdentifier() ); }
    | T_SERVER name
      { $$ = driver.makeAstNode<NamedCommentObjectAST>( $1, driver.createName(NameAST::Unknown, $2) ); }
    | T_FOREIGN T_DATA T_WRAPPER any_name
      { $$ = driver.makeAstNode<FdwCommentObjectAST>( $1, $2, $3, $4->asAnyIdentifier() ); }
    | T_EVENT T_TRIGGER trigger_identifier
      { $$ = driver.makeAstNode<EvtTriggerCommentObjectAST>( $1, $2, $3->asTriggerIdentifier() ); }
    | T_TEXT T_SEARCH T_CONFIGURATION any_name
      { $$ = driver.makeAstNode<SearchCommentObjectAST>( $1, $2, $3, $4->asAnyIdentifier() ); }
    | T_TEXT T_SEARCH T_DICTIONARY any_name
      { $$ = driver.makeAstNode<SearchCommentObjectAST>( $1, $2, $3, $4->asAnyIdentifier() ); }
    | T_TEXT T_SEARCH T_PARSER any_name
      { $$ = driver.makeAstNode<SearchCommentObjectAST>( $1, $2, $3, $4->asAnyIdentifier() ); }
    | T_TEXT T_SEARCH T_TEMPLATE any_name
      { $$ = driver.makeAstNode<SearchCommentObjectAST>( $1, $2, $3, $4->asAnyIdentifier() ); }
    ;

CommentTextClause:
      T_IS comment_text
      { $$ = driver.makeAstNode<CommentTextClauseAST>( $1, $2 ); }
    | empty_aka_optional
    ;

comment_text:
      Sconst
    | T_NULL
    ;

/*****************************************************************************
 *		QUERY:
 *				CLUSTER [VERBOSE] <qualified_name> [ USING <index_name> ]
 *				CLUSTER [VERBOSE]
 *				CLUSTER [VERBOSE] <index_name> ON <qualified_name> (for pre-8.3)
 *****************************************************************************/

ClusterStmt:
      T_CLUSTER opt_verbose table_identifier cluster_index_specification
      { $$ = driver.makeAstNode<ClusterUsingStmtAST>( $1, $2, $3->asTableIdentifier(), $4->asUsingIndexClause() ); }
    | T_CLUSTER opt_verbose
      { $$ = driver.makeAstNode<ClusterUsingStmtAST>( $1, $2, (TableIdentifierAST *) 0, (UsingIndexClauseAST *) 0 ); }
    | T_CLUSTER opt_verbose index_name T_ON table_identifier
      { $$ = driver.makeAstNode<ClusterOnStmtAST>( $1, $2, driver.createName(NameAST::Index, $3), $4, $5->asTableIdentifier() ); }
    ;

cluster_index_specification:
      T_USING index_name
      { $$ = driver.makeAstNode<UsingIndexClauseAST>( $1, driver.createName(NameAST::Index, $2) ); }
    | empty_aka_optional
    ;

/*****************************************************************************
 *    QUERY : CLOSE cursor
 *****************************************************************************/

ClosePortalStmt:
      T_CLOSE cursor_name
      { $$ = driver.makeAstNode<CloseCursorStmtAST>( $1, driver.createName(NameAST::Unknown, $2) ); }
    | T_CLOSE T_ALL
      { $$ = driver.makeAstNode<CloseAllStmtAST>( $1, $2 ); }
    ;

/*****************************************************************************
 *    QUERY: CHECKPOINT
 *****************************************************************************/

CheckPointStmt:
      T_CHECKPOINT
      { $$ = driver.makeAstNode<CheckPointStmtAST>( $1 ); }
    ;

/*****************************************************************************
 *    QUERY: ANALYZE
 *****************************************************************************/

AnalyzeStmt:
      analyze_keyword opt_verbose
      { $$ = driver.makeAstNode<AnalyzeStmtAST>( $1, $2, (TableIdentifierAST *) 0, (CompoundColumnNamesAST *) 0 ); }
    | analyze_keyword opt_verbose table_identifier opt_name_list
      { $$ = driver.makeAstNode<AnalyzeStmtAST>( $1, $2, $3->asTableIdentifier(), $4->asCompoundColumnNames() ); }
    ;

 analyze_keyword:
      T_ANALYZE
    | T_ANALYSE /* British */
    ;

 opt_verbose:
      T_VERBOSE
    | /*EMPTY*/ { $$ = 0; }
    ;

/*****************************************************************************
 * Manipulate a text search dictionary or configuration
 *****************************************************************************/

AlterTSDictionaryStmt:
      T_ALTER T_TEXT T_SEARCH T_DICTIONARY any_name definition
      {
        $$ = driver.makeAstNode<AlterTSDictionaryStmtAST>( $1,
                                                           $2,
                                                           $3,
                                                           $4,
                                                           $5->asAnyIdentifier(),
                                                           $6->asDefinition() );
      }
    ;

AlterTSConfigurationStmt:
      T_ALTER T_TEXT T_SEARCH T_CONFIGURATION any_name T_ADD   T_MAPPING T_FOR name_list                    T_WITH any_name_list
      {
        AlterTSConfCmdAST *cmd = driver.makeAstNode<AlterTSConfAddAlterCmdAST>( $6, $7, $8, $9,  0, (AnyIdentifierAST *) 0, $10, $11 );
        $$ = driver.makeAstNode<AlterTSConfigurationStmtAST>( $1, $2, $3, $4, $5->asAnyIdentifier(), cmd );
      }
    | T_ALTER T_TEXT T_SEARCH T_CONFIGURATION any_name T_ALTER T_MAPPING T_FOR name_list                    T_WITH any_name_list
      {
        AlterTSConfCmdAST *cmd = driver.makeAstNode<AlterTSConfAddAlterCmdAST>( $6, $7, $8, $9,  0, (AnyIdentifierAST *) 0, $10, $11 );
        $$ = driver.makeAstNode<AlterTSConfigurationStmtAST>( $1, $2, $3, $4, $5->asAnyIdentifier(), cmd );
      }
    | T_ALTER T_TEXT T_SEARCH T_CONFIGURATION any_name T_ALTER T_MAPPING                 T_REPLACE any_name T_WITH any_name
      {
        AlterTSConfCmdAST *cmd = driver.makeAstNode<AlterTSConfAddAlterCmdAST>( $6, $7,  0, (NameListAST *) 0, $8, $9->asAnyIdentifier(), $10, driver.makeAstNode<AnyIdentifierListAST>($11->asAnyIdentifier()) );
        $$ = driver.makeAstNode<AlterTSConfigurationStmtAST>( $1, $2, $3, $4, $5->asAnyIdentifier(), cmd );
      }
    | T_ALTER T_TEXT T_SEARCH T_CONFIGURATION any_name T_ALTER T_MAPPING T_FOR name_list T_REPLACE any_name T_WITH any_name
      {
        AlterTSConfCmdAST *cmd = driver.makeAstNode<AlterTSConfAddAlterCmdAST>( $6, $7, $8, $9, $10, $11->asAnyIdentifier(), $12, driver.makeAstNode<AnyIdentifierListAST>($13->asAnyIdentifier()) );
        $$ = driver.makeAstNode<AlterTSConfigurationStmtAST>( $1, $2, $3, $4, $5->asAnyIdentifier(), cmd );
      }
    | T_ALTER T_TEXT T_SEARCH T_CONFIGURATION any_name T_DROP  T_MAPPING               T_FOR name_list
      {
        AlterTSConfCmdAST *cmd = driver.makeAstNode<AlterTSConfDropCmdAST>( $6, $7,  0,  0, $8, $9 );
        $$ = driver.makeAstNode<AlterTSConfigurationStmtAST>( $1, $2, $3, $4, $5->asAnyIdentifier(), cmd );
      }
    | T_ALTER T_TEXT T_SEARCH T_CONFIGURATION any_name T_DROP  T_MAPPING T_IF T_EXISTS T_FOR name_list
      {
        AlterTSConfCmdAST *cmd = driver.makeAstNode<AlterTSConfDropCmdAST>( $6, $7, $8, $9, $10, $11 );
        $$ = driver.makeAstNode<AlterTSConfigurationStmtAST>( $1, $2, $3, $4, $5->asAnyIdentifier(), cmd );
      }
    ;

any_name_list:
      any_name
      { $$ = driver.makeAstNode<AnyIdentifierListAST>( $1->asAnyIdentifier() ); }
    | any_name_list T_COMMA any_name
      { $$ = driver.makeAstNode<AnyIdentifierListAST>( $1, $3->asAnyIdentifier() ); }
    ;

/*****************************************************************************
 *    ALTER TYPE
 *****************************************************************************/

AlterCompositeTypeStmt:
      T_ALTER T_TYPE any_name alter_type_cmds
      {
        $$ = driver.makeAstNode<AlterCompositeTypeStmtAST>( $1,
                                                            $2,
                                                            $3->asAnyIdentifier(),
                                                            $4 );
      }
    ;

alter_type_cmds:
      alter_type_cmd
      { $$ = driver.makeAstNode<AlterTypeCommandListAST>( $1->asAlterTypeCommand() ); }
    | alter_type_cmds T_COMMA alter_type_cmd
      { $$ = driver.makeAstNode<AlterTypeCommandListAST>( $1, $3->asAlterTypeCommand() ); }
    ;

alter_type_cmd:
      T_ADD T_ATTRIBUTE TableFuncElement opt_drop_behavior
      {
        $$ = driver.makeAstNode<AlterTypeAddCommandAST>( $1,
                                                         $2,
                                                         $3->asTableFuncElement(),
                                                         $4 );
      }
    | T_DROP T_ATTRIBUTE T_IF T_EXISTS ColId opt_drop_behavior
      {
        $$ = driver.makeAstNode<AlterTypeDropCommandAST>( $1,
                                                          $2,
                                                          $3, $4,
                                                          driver.createName(NameAST::Column, $5),
                                                          $6 );
      }
    | T_DROP T_ATTRIBUTE               ColId opt_drop_behavior
      {
        $$ = driver.makeAstNode<AlterTypeDropCommandAST>( $1,
                                                          $2,
                                                          0, 0,
                                                          driver.createName(NameAST::Column, $3),
                                                          $4 );
      }
    | T_ALTER T_ATTRIBUTE ColId opt_set_data T_TYPE Typename opt_collate_clause opt_drop_behavior
      {
        $$ = driver.makeAstNode<AlterTypeAlterCommandAST>( $1,
                                                           $2,
                                                           driver.createName(NameAST::Column, $3),
                                                           $4->asSetData(),
                                                           $5,
                                                           $6->asType(),
                                                           $7->asCollateClause(),
                                                           $8 );
      }
    ;

/*****************************************************************************
 *    QUERY :
 *      CREATE SEQUENCE seqname
 *      ALTER SEQUENCE seqname
 *****************************************************************************/

CreateSeqStmt:
      T_CREATE OptTemp T_SEQUENCE sequence_identifier OptSeqOptList
      {
        $$ = driver.makeAstNode<CreateSeqStmtAST>( $1,
                                                   $2->asTempClause(),
                                                   $3,
                                                   $4->asSequenceIdentifier(),
                                                   $5 );
      }
    ;

AlterSeqStmt:
      T_ALTER T_SEQUENCE sequence_identifier SeqOptList
      {
        $$ = driver.makeAstNode<AlterSeqStmtAST>( $1,
                                                  $2,
                                                  0, 0,
                                                  $3->asSequenceIdentifier(),
                                                  $4 );
      }
    | T_ALTER T_SEQUENCE T_IF T_EXISTS sequence_identifier SeqOptList
      {
        $$ = driver.makeAstNode<AlterSeqStmtAST>( $1,
                                                  $2,
                                                  $3, $4,
                                                  $5->asSequenceIdentifier(),
                                                  $6 );
      }
    ;

OptTemp:
      T_TEMPORARY
      { $$ = driver.makeAstNode<TempClauseAST>( $1,  0 ); }
    | T_TEMP
      { $$ = driver.makeAstNode<TempClauseAST>( $1,  0 ); }
    | T_LOCAL T_TEMPORARY
      { $$ = driver.makeAstNode<TempClauseAST>( $1, $2 ); }
    | T_LOCAL T_TEMP
      { $$ = driver.makeAstNode<TempClauseAST>( $1, $2 ); }
    | T_GLOBAL T_TEMPORARY
      { $$ = driver.makeAstNode<TempClauseAST>( $1, $2 ); }
    | T_GLOBAL T_TEMP
      { $$ = driver.makeAstNode<TempClauseAST>( $1, $2 ); }
    | T_UNLOGGED
      { $$ = driver.makeAstNode<TempClauseAST>( $1,  0 ); }
    | empty_aka_optional
    ;

OptSeqOptList:
      SeqOptList
    | /*EMPTY*/ { $$ = 0; }
    ;

SeqOptList:
      SeqOptElem
      { $$ = driver.makeAstNode<SequenceOptionListAST>( $1->asSequenceOption() ); }
    | SeqOptList SeqOptElem
      { $$ = driver.makeAstNode<SequenceOptionListAST>( $1, $2->asSequenceOption() ); }
    ;

SeqOptElem:
      T_CACHE NumericOnly
      { $$ = driver.makeAstNode<SimpleSequenceOptionAST>( $1, $2,  0 ); }
    | T_CYCLE
      { $$ = driver.makeAstNode<SimpleSequenceOptionAST>( $1,  0,  0 ); }
    | T_NO T_CYCLE
      { $$ = driver.makeAstNode<SimpleSequenceOptionAST>( $1, $2,  0 ); }
    | T_INCREMENT opt_by NumericOnly
      { $$ = driver.makeAstNode<SimpleSequenceOptionAST>( $1, $2, $3 ); }
    | T_MAXVALUE NumericOnly
      { $$ = driver.makeAstNode<SimpleSequenceOptionAST>( $1, $2,  0 ); }
    | T_MINVALUE NumericOnly
      { $$ = driver.makeAstNode<SimpleSequenceOptionAST>( $1, $2,  0 ); }
    | T_NO T_MAXVALUE
      { $$ = driver.makeAstNode<SimpleSequenceOptionAST>( $1, $2,  0 ); }
    | T_NO T_MINVALUE
      { $$ = driver.makeAstNode<SimpleSequenceOptionAST>( $1, $2,  0 ); }
    | T_OWNED T_BY RoleId
      { $$ = driver.makeAstNode<OwnedSequenceOptionAST>( $1, $2, $3->asRoleId() ); }
    | T_START opt_with NumericOnly
      { $$ = driver.makeAstNode<SimpleSequenceOptionAST>( $1, $2, $3 ); }
    | T_RESTART
      { $$ = driver.makeAstNode<SimpleSequenceOptionAST>( $1,  0,  0 ); }
    | T_RESTART opt_with NumericOnly
      { $$ = driver.makeAstNode<SimpleSequenceOptionAST>( $1, $2, $3 ); }
    ;

opt_by:
      T_BY
    | /* EMPTY */ { $$ = 0; }
    ;

/*****************************************************************************
 * ALTER THING name OWNER TO newname
 *****************************************************************************/

AlterOwnerStmt:
      T_ALTER T_AGGREGATE func_name aggr_args OwnerToClause
      {
        $$ = driver.makeAstNode<AlterAggrOwnerStmtAST>( $1,
                                                        $2,
                                                        $3->asFunctionRef(),
                                                        $4->asAggrArguments(),
                                                        $5->asOwnerToClause() );
      }
    | T_ALTER T_COLLATION any_name OwnerToClause
      {
        $$ = driver.makeAstNode<AlterCollationOwnerStmtAST>( $1,
                                                             $2,
                                                             $3->asAnyIdentifier(),
                                                             $4->asOwnerToClause() );
      }
    | T_ALTER T_CONVERSION any_name OwnerToClause
      {
        $$ = driver.makeAstNode<AlterConvOwnerStmtAST>( $1,
                                                        $2,
                                                        $3->asAnyIdentifier(),
                                                        $4->asOwnerToClause() );
      }
    | T_ALTER T_DATABASE database_name OwnerToClause
      {
        $$ = driver.makeAstNode<AlterDbOwnerStmtAST>( $1,
                                                      $2,
                                                      $3->asDatabaseName(),
                                                      $4->asOwnerToClause() );
      }
    | T_ALTER T_DOMAIN any_name OwnerToClause
      {
        $$ = driver.makeAstNode<AlterDomainOwnerStmtAST>( $1,
                                                          $2,
                                                          $3->asAnyIdentifier(),
                                                          $4->asOwnerToClause() );
      }
    | T_ALTER T_FUNCTION function_with_argtypes OwnerToClause
      {
        $$ = driver.makeAstNode<AlterFunctionOwnerStmtAST>( $1,
                                                            $2,
                                                            $3->asFunctionWithArgTypes(),
                                                            $4->asOwnerToClause() );
      }
    | T_ALTER opt_procedural T_LANGUAGE name OwnerToClause
      {
        $$ = driver.makeAstNode<AlterLangOwnerStmtAST>( $1,
                                                        $2,
                                                        $3,
                                                        driver.createName(NameAST::Unknown, $4),
                                                        $5->asOwnerToClause() );
      }
    | T_ALTER T_LARGE T_OBJECT NumericOnly OwnerToClause
      {
        $$ = driver.makeAstNode<AlterLargeObjOwnerStmtAST>( $1,
                                                            $2,
                                                            $3,
                                                            $4,
                                                            $5->asOwnerToClause() );
      }
    | T_ALTER T_OPERATOR any_operator oper_argtypes OwnerToClause
      {
        $$ = driver.makeAstNode<AlterOpOwnerStmtAST>( $1,
                                                      $2,
                                                      $3->asOperand(),
                                                      $4->asOperArgTypes(),
                                                      $5->asOwnerToClause() );
      }
    | T_ALTER T_OPERATOR T_CLASS any_name T_USING access_method OwnerToClause
      {
        $$ = driver.makeAstNode<AlterOpUsingOwnerStmtAST>( $1,
                                                           $2,
                                                           $3,
                                                           $4->asAnyIdentifier(),
                                                           $5,
                                                           $6,
                                                           $7->asOwnerToClause() );
      }
    | T_ALTER T_OPERATOR T_FAMILY any_name T_USING access_method OwnerToClause
      {
        $$ = driver.makeAstNode<AlterOpUsingOwnerStmtAST>( $1,
                                                           $2,
                                                           $3,
                                                           $4->asAnyIdentifier(),
                                                           $5,
                                                           $6,
                                                           $7->asOwnerToClause() );
      }
    | T_ALTER T_SCHEMA name OwnerToClause
      {
        $$ = driver.makeAstNode<AlterSchemaOwnerStmtAST>( $1,
                                                          $2,
                                                          driver.createName(NameAST::Schema, $3),
                                                          $4->asOwnerToClause() );
      }
    | T_ALTER T_TYPE any_name OwnerToClause
      {
        $$ = driver.makeAstNode<AlterTypeOwnerStmtAST>( $1,
                                                        $2,
                                                        $3->asAnyIdentifier(),
                                                        $4->asOwnerToClause() );
      }
    | T_ALTER T_TABLESPACE name OwnerToClause
      {
        $$ = driver.makeAstNode<AlterTablespaceOwnerStmtAST>( $1,
                                                              $2,
                                                              driver.createName(NameAST::Unknown, $3),
                                                              $4->asOwnerToClause() );
      }
    | T_ALTER T_TEXT T_SEARCH T_DICTIONARY any_name OwnerToClause
      {
        $$ = driver.makeAstNode<AlterSearchOwnerStmtAST>( $1,
                                                          $2,
                                                          $3,
                                                          $4,
                                                          $5->asAnyIdentifier(),
                                                          $6->asOwnerToClause() );
      }
    | T_ALTER T_TEXT T_SEARCH T_CONFIGURATION any_name OwnerToClause
      {
        $$ = driver.makeAstNode<AlterSearchOwnerStmtAST>( $1,
                                                          $2,
                                                          $3,
                                                          $4,
                                                          $5->asAnyIdentifier(),
                                                          $6->asOwnerToClause() );
      }
    | T_ALTER T_FOREIGN T_DATA T_WRAPPER name OwnerToClause
      {
        $$ = driver.makeAstNode<AlterFdwOwnerStmtAST>( $1,
                                                       $2,
                                                       $3,
                                                       $4,
                                                       driver.createName(NameAST::Unknown, $5),
                                                       $6->asOwnerToClause() );
      }
    | T_ALTER T_SERVER name OwnerToClause
      {
        $$ = driver.makeAstNode<AlterServerOwnerStmtAST>( $1,
                                                          $2,
                                                          driver.createName(NameAST::Unknown, $3),
                                                          $4->asOwnerToClause() );
      }
    | T_ALTER T_EVENT T_TRIGGER name OwnerToClause
      {
        $$ = driver.makeAstNode<AlterEvtTriggerOwnerStmtAST>( $1,
                                                              $2,
                                                              $3,
                                                              driver.createName(NameAST::Trigger, $4),
                                                              $5->asOwnerToClause() );
      }
    ;

OwnerToClause:
      T_OWNER T_TO RoleId
      { $$ = driver.makeAstNode<OwnerToClauseAST>( $1, $2, $3->asRoleId() ); }
    ;

/*****************************************************************************
 * ALTER THING name SET SCHEMA name
 *****************************************************************************/

AlterObjectSchemaStmt:
      T_ALTER T_AGGREGATE func_name aggr_args SetSchemaClause
      {
        $$ = driver.makeAstNode<AlterAggrFuncSchemaStmtAST>( $1,
                                                             $2,
                                                             $3->asFunctionRef(),
                                                             $4->asAggrArguments(),
                                                             $5->asSetSchemaClause() );
      }
    | T_ALTER T_COLLATION any_name SetSchemaClause
      {
        $$ = driver.makeAstNode<AlterCollationSchemaStmtAST>( $1,
                                                              $2,
                                                              $3->asAnyIdentifier(),
                                                              $4->asSetSchemaClause() );
      }
    | T_ALTER T_CONVERSION any_name SetSchemaClause
      {
        $$ = driver.makeAstNode<AlterConvSchemaStmtAST>( $1,
                                                         $2,
                                                         $3->asAnyIdentifier(),
                                                         $4->asSetSchemaClause() );
      }
    | T_ALTER T_DOMAIN any_name SetSchemaClause
      {
        $$ = driver.makeAstNode<AlterDomainSchemaStmtAST>( $1,
                                                           $2,
                                                           $3->asAnyIdentifier(),
                                                           $4->asSetSchemaClause() );
      }
    | T_ALTER T_EXTENSION any_name SetSchemaClause
      {
        $$ = driver.makeAstNode<AlterExtSchemaStmtAST>( $1,
                                                        $2,
                                                        $3->asAnyIdentifier(),
                                                        $4->asSetSchemaClause() );
      }
    | T_ALTER T_FUNCTION function_with_argtypes SetSchemaClause
      {
        $$ = driver.makeAstNode<AlterFunctionSchemaStmtAST>( $1,
                                                             $2,
                                                             $3->asFunctionWithArgTypes(),
                                                             $4->asSetSchemaClause() );
      }
    | T_ALTER T_OPERATOR any_operator oper_argtypes SetSchemaClause
      {
        $$ = driver.makeAstNode<AlterOpSchemaStmtAST>( $1,
                                                       $2,
                                                       $3->asOperand(),
                                                       $4->asOperArgTypes(),
                                                       $5->asSetSchemaClause() );
      }
    | T_ALTER T_OPERATOR T_CLASS any_name T_USING access_method SetSchemaClause
      {
        $$ = driver.makeAstNode<AlterOpUsingSchemaStmtAST>( $1,
                                                            $2,
                                                            $3,
                                                            $4->asAnyIdentifier(),
                                                            $5,
                                                            $6,
                                                            $7->asSetSchemaClause() );
      }
    | T_ALTER T_OPERATOR T_FAMILY any_name T_USING access_method SetSchemaClause
      {
        $$ = driver.makeAstNode<AlterOpUsingSchemaStmtAST>( $1,
                                                            $2,
                                                            $3,
                                                            $4->asAnyIdentifier(),
                                                            $5,
                                                            $6,
                                                            $7->asSetSchemaClause() );
      }
    | T_ALTER T_TABLE alter_relation_expr SetSchemaClause
      {
        $$ = driver.makeAstNode<AlterTableSchemaStmtAST>( $1, 0,
                                                          $2,
                                                          0, 0,
                                                          $3->asRelationExpr(),
                                                          $4->asSetSchemaClause() );
      }
    | T_ALTER T_TABLE T_IF T_EXISTS alter_relation_expr SetSchemaClause
      {
        $$ = driver.makeAstNode<AlterTableSchemaStmtAST>( $1, 0,
                                                          $2,
                                                          $3, $4,
                                                          $5->asRelationExpr(),
                                                          $6->asSetSchemaClause() );
      }
    | T_ALTER T_TEXT T_SEARCH T_PARSER any_name SetSchemaClause
      {
        $$ = driver.makeAstNode<AlterSearchSchemaStmtAST>( $1, $2, $3, $4,
                                                           $5->asAnyIdentifier(),
                                                           $6->asSetSchemaClause() );
      }
    | T_ALTER T_TEXT T_SEARCH T_DICTIONARY any_name SetSchemaClause
      {
        $$ = driver.makeAstNode<AlterSearchSchemaStmtAST>( $1, $2, $3, $4,
                                                           $5->asAnyIdentifier(),
                                                           $6->asSetSchemaClause() );
      }
    | T_ALTER T_TEXT T_SEARCH T_TEMPLATE any_name SetSchemaClause
      {
        $$ = driver.makeAstNode<AlterSearchSchemaStmtAST>( $1, $2, $3, $4,
                                                           $5->asAnyIdentifier(),
                                                           $6->asSetSchemaClause() );
      }
    | T_ALTER T_TEXT T_SEARCH T_CONFIGURATION any_name SetSchemaClause
      {
        $$ = driver.makeAstNode<AlterSearchSchemaStmtAST>( $1, $2, $3, $4,
                                                           $5->asAnyIdentifier(),
                                                           $6->asSetSchemaClause() );
      }
    | T_ALTER T_SEQUENCE sequence_identifier SetSchemaClause
      {
        $$ = driver.makeAstNode<AlterSeqSchemaStmtAST>( $1,
                                                        $2,
                                                        0, 0,
                                                        $3->asSequenceIdentifier(),
                                                        $4->asSetSchemaClause() );
      }
    | T_ALTER T_SEQUENCE T_IF T_EXISTS sequence_identifier SetSchemaClause
      {
        $$ = driver.makeAstNode<AlterSeqSchemaStmtAST>( $1,
                                                        $2,
                                                        $3, $4,
                                                        $5->asSequenceIdentifier(),
                                                        $6->asSetSchemaClause() );
      }
    | T_ALTER T_VIEW table_identifier SetSchemaClause
      {
        $$ = driver.makeAstNode<AlterViewSchemaStmtAST>( $1,
                                                         0,
                                                         $2,
                                                         0, 0,
                                                         $3->asTableIdentifier(),
                                                         $4->asSetSchemaClause() );
      }
    | T_ALTER T_VIEW T_IF T_EXISTS table_identifier SetSchemaClause
      {
        $$ = driver.makeAstNode<AlterViewSchemaStmtAST>( $1,
                                                         0,
                                                         $2,
                                                         $3, $4,
                                                         $5->asTableIdentifier(),
                                                         $6->asSetSchemaClause() );
      }
    | T_ALTER T_MATERIALIZED T_VIEW table_identifier SetSchemaClause
      {
        $$ = driver.makeAstNode<AlterViewSchemaStmtAST>( $1,
                                                         $2,
                                                         $3,
                                                         0, 0,
                                                         $4->asTableIdentifier(),
                                                         $5->asSetSchemaClause() );
      }
    | T_ALTER T_MATERIALIZED T_VIEW T_IF T_EXISTS table_identifier SetSchemaClause
      {
        $$ = driver.makeAstNode<AlterViewSchemaStmtAST>( $1,
                                                         $2,
                                                         $3,
                                                         $4, $5,
                                                         $6->asTableIdentifier(),
                                                         $7->asSetSchemaClause() );
      }
    | T_ALTER T_FOREIGN T_TABLE relation_expr SetSchemaClause
      {
        $$ = driver.makeAstNode<AlterTableSchemaStmtAST>( $1, $2,
                                                          $3,
                                                          0, 0,
                                                          $4->asRelationExpr(),
                                                          $5->asSetSchemaClause() );
      }
    | T_ALTER T_FOREIGN T_TABLE T_IF T_EXISTS relation_expr SetSchemaClause
      {
        $$ = driver.makeAstNode<AlterTableSchemaStmtAST>( $1, $2,
                                                          $3,
                                                          $4, $5,
                                                          $6->asRelationExpr(),
                                                          $7->asSetSchemaClause() );
      }
    | T_ALTER T_TYPE any_name SetSchemaClause
      {
        $$ = driver.makeAstNode<AlterTypeSchemaStmtAST>( $1,
                                                         $2,
                                                         $3->asAnyIdentifier(),
                                                         $4->asSetSchemaClause() );
      }
    ;

SetSchemaClause:
      T_SET T_SCHEMA name
      { $$ = driver.makeAstNode<SetSchemaClauseAST>( $1, $2, driver.createName(NameAST::Schema, $3) ); }
    ;

/*****************************************************************************
 *  ALTER FUNCTION
 *****************************************************************************/
AlterFunctionStmt:
      T_ALTER T_FUNCTION function_with_argtypes alterfunc_opt_list opt_restrict
      {
        $$ = driver.makeAstNode<AlterFunctionStmtAST>( $1,
                                                       $2,
                                                       $3->asFunctionWithArgTypes(),
                                                       $4,
                                                       $5 );
      }
    ;

alterfunc_opt_list:
      common_func_opt_item
      { $$ = driver.makeAstNode<AlterFunctionOptionListAST>( $1->asAlterFunctionOption() ); }
    | alterfunc_opt_list common_func_opt_item
      { $$ = driver.makeAstNode<AlterFunctionOptionListAST>( $1, $2->asAlterFunctionOption() ); }
    ;

opt_restrict:
      T_RESTRICT
    | /* EMPTY */ { $$ = 0; }
    ;

common_func_opt_item:
      T_CALLED T_ON T_NULL T_INPUT
      { $$ = driver.makeAstNode<CommonAlterFunctionOptAST>( $1, $2, $3, $4 ); }
    | T_RETURNS T_NULL T_ON T_NULL T_INPUT
      { $$ = driver.makeAstNode<CommonAlterFunctionOptAST>( $1, $2, $3, $4 ); }
    | T_STRICT
      { $$ = driver.makeAstNode<CommonAlterFunctionOptAST>( $1,  0,  0,  0 ); }
    | T_IMMUTABLE
      { $$ = driver.makeAstNode<CommonAlterFunctionOptAST>( $1,  0,  0,  0 ); }
    | T_STABLE
      { $$ = driver.makeAstNode<CommonAlterFunctionOptAST>( $1,  0,  0,  0 ); }
    | T_VOLATILE
      { $$ = driver.makeAstNode<CommonAlterFunctionOptAST>( $1,  0,  0,  0 ); }
    | T_EXTERNAL T_SECURITY T_DEFINER
      { $$ = driver.makeAstNode<CommonAlterFunctionOptAST>( $1, $2, $3,  0 ); }
    | T_EXTERNAL T_SECURITY T_INVOKER
      { $$ = driver.makeAstNode<CommonAlterFunctionOptAST>( $1, $2, $3,  0 ); }
    | T_SECURITY T_DEFINER
      { $$ = driver.makeAstNode<CommonAlterFunctionOptAST>( $1, $2,  0,  0 ); }
    | T_SECURITY T_INVOKER
      { $$ = driver.makeAstNode<CommonAlterFunctionOptAST>( $1, $2,  0,  0 ); }
    | T_LEAKPROOF
      { $$ = driver.makeAstNode<CommonAlterFunctionOptAST>( $1,  0,  0,  0 ); }
    | T_NOT T_LEAKPROOF
      { $$ = driver.makeAstNode<CommonAlterFunctionOptAST>( $1, $2,  0,  0 ); }
    | T_COST NumericOnly
      { $$ = driver.makeAstNode<CommonAlterFunctionOptAST>( $1, $2,  0,  0 ); }
    | T_ROWS NumericOnly
      { $$ = driver.makeAstNode<CommonAlterFunctionOptAST>( $1, $2,  0,  0 ); }
    | FunctionSetResetClause
    ;

FunctionSetResetClause:
      T_SET set_rest_more
      { $$ = driver.makeAstNode<RestAlterFunctionOptAST>( driver.makeAstNode<SetResetClauseAsSetAST>( $1, $2->asSetRest() ) ); }
    | VariableResetStmt
      { $$ = driver.makeAstNode<RestAlterFunctionOptAST>( driver.makeAstNode<SetResetClauseAsVarResetAST>( $1->asVariableResetStmt() ) ); }
    ;

/*****************************************************************************
 *    QUERY:
 *             ALTER FOREIGN TABLE relname [...]
 *****************************************************************************/

AlterForeignTableStmt:
      T_ALTER T_FOREIGN T_TABLE relation_expr alter_table_cmds
      {
        $$ = driver.makeAstNode<AlterForeignTableStmtAST>( $1,
                                                           $2,
                                                           $3,
                                                           0, 0,
                                                           $4->asRelationExpr(),
                                                           $5 );
      }
    | T_ALTER T_FOREIGN T_TABLE T_IF T_EXISTS relation_expr alter_table_cmds
      {
        $$ = driver.makeAstNode<AlterForeignTableStmtAST>( $1,
                                                           $2,
                                                           $3,
                                                           $4, $5,
                                                           $6->asRelationExpr(),
                                                           $7 );
      }
    ;

/*****************************************************************************
 *    QUERY :
 *      ALTER SERVER name [VERSION] [OPTIONS]
 ****************************************************************************/

AlterForeignServerStmt:
      T_ALTER T_SERVER name foreign_server_version alter_generic_options
      {
        $$ = driver.makeAstNode<AlterForeignServerStmtAST>( $1,
                                                            $2,
                                                            driver.createName(NameAST::Unknown, $3),
                                                            $4->asForeignServerVersion(),
                                                            $5->asAlterTableGenericCmd() );
      }
    | T_ALTER T_SERVER name foreign_server_version
      {
        $$ = driver.makeAstNode<AlterForeignServerStmtAST>( $1,
                                                            $2,
                                                            driver.createName(NameAST::Unknown, $3),
                                                            $4->asForeignServerVersion(),
                                                            (AlterTableGenericCmdAST *) 0 );
      }
    | T_ALTER T_SERVER name alter_generic_options
      {
        $$ = driver.makeAstNode<AlterForeignServerStmtAST>( $1,
                                                            $2,
                                                            driver.createName(NameAST::Unknown, $3),
                                                            (ForeignServerVersionAST *) 0,
                                                            $4->asAlterTableGenericCmd() );
      }
    ;

foreign_server_version:
      T_VERSION Sconst
      { $$ = driver.makeAstNode<ForeignServerVersionAST>( $1, $2 ); }
    | T_VERSION T_NULL
      { $$ = driver.makeAstNode<ForeignServerVersionAST>( $1, $2 ); }
    ;

/*****************************************************************************
 *    QUERY :
 *        ALTER FOREIGN DATA WRAPPER name options
 ****************************************************************************/

AlterFdwStmt:
      T_ALTER T_FOREIGN T_DATA T_WRAPPER name opt_fdw_options alter_generic_options
      {
        $$ = driver.makeAstNode<AlterFdwStmtAST>( $1,
                                                  $2,
                                                  $3,
                                                  $4,
                                                  driver.createName(NameAST::Unknown, $5),
                                                  $6,
                                                  $7->asAlterTableGenericCmd() );
      }
    | T_ALTER T_FOREIGN T_DATA T_WRAPPER name fdw_options
      {
        $$ = driver.makeAstNode<AlterFdwStmtAST>( $1,
                                                  $2,
                                                  $3,
                                                  $4,
                                                  driver.createName(NameAST::Unknown, $5),
                                                  $6,
                                                  (AlterTableGenericCmdAST *)0 );
      }
    ;

handler_name:
      name
      { $$ = driver.makeAstNode<HandlerIdentifierAST>( driver.createName(NameAST::Unknown, $1), (NameListAST *) 0 ); }
    | name attrs
      { $$ = driver.makeAstNode<HandlerIdentifierAST>( driver.createName(NameAST::Unknown, $1), $2 ); }
    ;

fdw_option:
      T_HANDLER handler_name
      { $$ = driver.makeAstNode<SetFdwOptionAST>( $1, $2->asHandlerIdentifier() ); }
    | T_NO T_HANDLER
      { $$ = driver.makeAstNode<ResetFdwOptionAST>( $1, $2 ); }
    | T_VALIDATOR handler_name
      { $$ = driver.makeAstNode<SetFdwOptionAST>( $1, $2->asHandlerIdentifier() ); }
    | T_NO T_VALIDATOR
      { $$ = driver.makeAstNode<ResetFdwOptionAST>( $1, $2 ); }
    ;

fdw_options:
      fdw_option
      { $$ = driver.makeAstNode<FdwOptionListAST>( $1->asFdwOption() ); }
    | fdw_options fdw_option
      { $$ = driver.makeAstNode<FdwOptionListAST>( $1, $2->asFdwOption() ); }
    ;

opt_fdw_options:
      fdw_options
    | /*EMPTY*/ { $$ = 0; }
    ;

/*****************************************************************************
 * ALTER EXTENSION name UPDATE [ TO version ]
 *****************************************************************************/

AlterExtensionStmt:
      T_ALTER T_EXTENSION name T_UPDATE alter_extension_opt_list
      {
        $$ = driver.makeAstNode<AlterExtensionStmtAST>( $1,
                                                        $2,
                                                        driver.createName(NameAST::Unknown, $3),
                                                        $4,
                                                        $5 );
      }
    ;

alter_extension_opt_list:
      alter_extension_opt_list alter_extension_opt_item
      { $$ = driver.makeAstNode<UpdateExtensionCmdListAST>( $1, $2->asUpdateExtensionCmd() ); }
    | /* EMPTY */ { $$ = 0; }
    ;

alter_extension_opt_item:
      T_TO NonReservedWord_or_Sconst
      { $$ = driver.makeAstNode<UpdateToExtensionCmdAST>( $1, $2 ); }
    ;

/*****************************************************************************
 * ALTER EXTENSION name ADD/DROP object-identifier
 *****************************************************************************/

AlterExtensionContentsStmt:
      T_ALTER T_EXTENSION name add_drop T_AGGREGATE func_name aggr_args
      {
        ExtensionCtxCmdAST *cmd = driver.makeAstNode<AggrExtensionCtxCmdAST>( $5, $6->asFunctionRef(), $7->asAggrArguments() );
        $$ = driver.makeAstNode<AlterExtensionCtxStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, cmd );
      }
    | T_ALTER T_EXTENSION name add_drop T_CAST T_LEFT_PAREN Typename T_AS Typename T_RIGHT_PAREN
      {
        ExtensionCtxCmdAST *cmd = driver.makeAstNode<CastExtensionCtxCmdAST>( $5, $6, $7->asType(), $8, $9->asType(), $10 );
        $$ = driver.makeAstNode<AlterExtensionCtxStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, cmd );
      }
    | T_ALTER T_EXTENSION name add_drop T_COLLATION any_name
      {
        ExtensionCtxCmdAST *cmd = driver.makeAstNode<SimpleExtensionCtxCmdAST>( $5, $6->asAnyIdentifier() );
        $$ = driver.makeAstNode<AlterExtensionCtxStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, cmd );
      }
    | T_ALTER T_EXTENSION name add_drop T_CONVERSION any_name
      {
        ExtensionCtxCmdAST *cmd = driver.makeAstNode<SimpleExtensionCtxCmdAST>( $5, $6->asAnyIdentifier() );
        $$ = driver.makeAstNode<AlterExtensionCtxStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, cmd );
      }
    | T_ALTER T_EXTENSION name add_drop T_DOMAIN any_name
      {
        ExtensionCtxCmdAST *cmd = driver.makeAstNode<SimpleExtensionCtxCmdAST>( $5, $6->asAnyIdentifier() );
        $$ = driver.makeAstNode<AlterExtensionCtxStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, cmd );
      }
    | T_ALTER T_EXTENSION name add_drop T_FUNCTION function_with_argtypes
      {
        ExtensionCtxCmdAST *cmd = driver.makeAstNode<FunctionExtensionCtxCmdAST>( $5, $6->asFunctionWithArgTypes() );
        $$ = driver.makeAstNode<AlterExtensionCtxStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, cmd );
      }
    | T_ALTER T_EXTENSION name add_drop opt_procedural T_LANGUAGE name
      {
        ExtensionCtxCmdAST *cmd = driver.makeAstNode<LangExtensionCtxCmdAST>( $5, $6, driver.createName(NameAST::Unknown, $7) );
        $$ = driver.makeAstNode<AlterExtensionCtxStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, cmd );
      }
    | T_ALTER T_EXTENSION name add_drop T_OPERATOR any_operator oper_argtypes
      {
        ExtensionCtxCmdAST *cmd = driver.makeAstNode<OpExtensionCtxCmdAST>( $5, $6->asOperand(), $7->asOperArgTypes() );
        $$ = driver.makeAstNode<AlterExtensionCtxStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, cmd );
      }
    | T_ALTER T_EXTENSION name add_drop T_OPERATOR T_CLASS any_name T_USING access_method
      {
        ExtensionCtxCmdAST *cmd = driver.makeAstNode<OpUsingExtensionCtxCmdAST>( $5, $6, $7->asAnyIdentifier(), $8, $9 );
        $$ = driver.makeAstNode<AlterExtensionCtxStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, cmd );
      }
    | T_ALTER T_EXTENSION name add_drop T_OPERATOR T_FAMILY any_name T_USING access_method
      {
        ExtensionCtxCmdAST *cmd = driver.makeAstNode<OpUsingExtensionCtxCmdAST>( $5, $6, $7->asAnyIdentifier(), $8, $9);
        $$ = driver.makeAstNode<AlterExtensionCtxStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, cmd );
      }
    | T_ALTER T_EXTENSION name add_drop T_SCHEMA name
      {
        ExtensionCtxCmdAST *cmd = driver.makeAstNode<SchemaExtensionCtxCmdAST>( $5, driver.createName(NameAST::Schema, $6) );
        $$ = driver.makeAstNode<AlterExtensionCtxStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, cmd );
      }
    | T_ALTER T_EXTENSION name add_drop T_EVENT T_TRIGGER name
      {
        ExtensionCtxCmdAST *cmd = driver.makeAstNode<TrgEventExtensionCtxCmdAST>( $5, $6, driver.createName(NameAST::Trigger, $7) );
        $$ = driver.makeAstNode<AlterExtensionCtxStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, cmd );
      }
    | T_ALTER T_EXTENSION name add_drop T_TABLE name
      {
        ExtensionCtxCmdAST *cmd = driver.makeAstNode<TableExtensionCtxCmdAST>(  0, $5, driver.createName(NameAST::Table, $6) );
        $$ = driver.makeAstNode<AlterExtensionCtxStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, cmd );
      }
    | T_ALTER T_EXTENSION name add_drop T_TEXT T_SEARCH T_PARSER any_name
      {
        ExtensionCtxCmdAST *cmd = driver.makeAstNode<TxtSearchExtensionCtxCmdAST>( $5, $6, $7, $8->asAnyIdentifier() );
        $$ = driver.makeAstNode<AlterExtensionCtxStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, cmd );
      }
    | T_ALTER T_EXTENSION name add_drop T_TEXT T_SEARCH T_DICTIONARY any_name
      {
        ExtensionCtxCmdAST *cmd = driver.makeAstNode<TxtSearchExtensionCtxCmdAST>( $5, $6, $7, $8->asAnyIdentifier() );
        $$ = driver.makeAstNode<AlterExtensionCtxStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, cmd );
      }
    | T_ALTER T_EXTENSION name add_drop T_TEXT T_SEARCH T_TEMPLATE any_name
      {
        ExtensionCtxCmdAST *cmd = driver.makeAstNode<TxtSearchExtensionCtxCmdAST>( $5, $6, $7, $8->asAnyIdentifier() );
        $$ = driver.makeAstNode<AlterExtensionCtxStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, cmd );
      }
    | T_ALTER T_EXTENSION name add_drop T_TEXT T_SEARCH T_CONFIGURATION any_name
      {
        ExtensionCtxCmdAST *cmd = driver.makeAstNode<TxtSearchExtensionCtxCmdAST>( $5, $6, $7, $8->asAnyIdentifier() );
        $$ = driver.makeAstNode<AlterExtensionCtxStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, cmd );
      }
    | T_ALTER T_EXTENSION name add_drop T_SEQUENCE name
      {
        ExtensionCtxCmdAST *cmd = driver.makeAstNode<SequenceExtensionCtxCmdAST>( $5, driver.createName(NameAST::Sequence, $6) );
        $$ = driver.makeAstNode<AlterExtensionCtxStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, cmd );
      }
    | T_ALTER T_EXTENSION name add_drop T_VIEW name
      {
        ExtensionCtxCmdAST *cmd = driver.makeAstNode<ViewExtensionCtxCmdAST>(  0, $5, driver.createName(NameAST::Table, $6) );
        $$ = driver.makeAstNode<AlterExtensionCtxStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, cmd );
      }
    | T_ALTER T_EXTENSION name add_drop T_MATERIALIZED T_VIEW name
      {
        ExtensionCtxCmdAST *cmd = driver.makeAstNode<ViewExtensionCtxCmdAST>( $5, $6, driver.createName(NameAST::Table, $7) );
        $$ = driver.makeAstNode<AlterExtensionCtxStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, cmd );
      }
    | T_ALTER T_EXTENSION name add_drop T_FOREIGN T_TABLE name
      {
        ExtensionCtxCmdAST *cmd = driver.makeAstNode<TableExtensionCtxCmdAST>( $5, $6, driver.createName(NameAST::Table, $7) );
        $$ = driver.makeAstNode<AlterExtensionCtxStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, cmd );
      }
    | T_ALTER T_EXTENSION name add_drop T_FOREIGN T_DATA T_WRAPPER name
      {
        ExtensionCtxCmdAST *cmd = driver.makeAstNode<FdwExtensionCtxCmdAST>( $5, $6, $7, driver.createName(NameAST::Unknown, $8) );
        $$ = driver.makeAstNode<AlterExtensionCtxStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, cmd );
      }
    | T_ALTER T_EXTENSION name add_drop T_SERVER name
      {
        ExtensionCtxCmdAST *cmd = driver.makeAstNode<ServerExtensionCtxCmdAST>( $5, driver.createName(NameAST::Unknown, $6) );
        $$ = driver.makeAstNode<AlterExtensionCtxStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, cmd );
      }
    | T_ALTER T_EXTENSION name add_drop T_TYPE any_name
      {
        ExtensionCtxCmdAST *cmd = driver.makeAstNode<TypeExtensionCtxCmdAST>( $5, $6->asAnyIdentifier() );
        $$ = driver.makeAstNode<AlterExtensionCtxStmtAST>( $1, $2, driver.createName(NameAST::Unknown, $3), $4, cmd );
      }
    ;

opt_procedural:
      T_PROCEDURAL
    | /*EMPTY*/ { $$ = 0; }
    ;

aggr_args:
      T_LEFT_PAREN type_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<ListAggrArgumentsAST>( $1, $2, $3 ); }
    | T_LEFT_PAREN T_STAR T_RIGHT_PAREN
      { $$ = driver.makeAstNode<StarAggrArgumentsAST>( $1, $2, $3 ); }
    ;

function_with_argtypes:
      func_name func_args
      { $$ = driver.makeAstNode<FunctionWithArgTypesAST>( $1->asFunctionRef(), $2->asFunctionArgTypes() ); }
    ;

func_args:
      T_LEFT_PAREN func_args_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<FunctionArgTypesAST>( $1, $2, $3 ); }
    | T_LEFT_PAREN T_RIGHT_PAREN
      { $$ = driver.makeAstNode<FunctionArgTypesAST>( $1, (FunctionArgListAST *) 0, $2 ); }
    ;

func_args_list:
      func_arg
      { $$ = driver.makeAstNode<FunctionArgListAST>( $1->asFunctionArg() ); }
    | func_args_list T_COMMA func_arg
      { $$ = driver.makeAstNode<FunctionArgListAST>( $1, $3->asFunctionArg() ); }
    ;

func_arg:
      arg_class param_name func_type
      { $$ = driver.makeAstNode<FunctionArgAST>( $1, driver.createName(NameAST::Unknown, $2),  0, $3->asFunctionType() ); }
    | param_name arg_class func_type
      { $$ = driver.makeAstNode<FunctionArgAST>(  0, driver.createName(NameAST::Unknown, $1), $2, $3->asFunctionType() ); }
    | param_name func_type
      { $$ = driver.makeAstNode<FunctionArgAST>(  0, driver.createName(NameAST::Unknown, $1),  0, $2->asFunctionType() ); }
    | arg_class func_type
      { $$ = driver.makeAstNode<FunctionArgAST>(  0, (NameAST *) 0, $1, $2->asFunctionType() ); }
    | func_type
      { $$ = driver.makeAstNode<FunctionArgAST>(  0, (NameAST *) 0,  0, $1->asFunctionType() ); }
    ;

arg_class:
      T_IN
    | T_OUT
    | T_INOUT
    | T_IN T_OUT  { $$ = $2; }
    | T_VARIADIC
    ;

oper_argtypes:
      T_LEFT_PAREN Typename T_RIGHT_PAREN
      { $$ = driver.makeAstNode<OperArgTypesAST>( $1, $2->asType(), 0, (TypeAST *) 0, $3 ); }
    | T_LEFT_PAREN Typename T_COMMA Typename T_RIGHT_PAREN
      { $$ = driver.makeAstNode<OperArgTypesAST>( $1, $2->asType(), $3, $4->asType(), $5 ); }
    | T_LEFT_PAREN T_NONE T_COMMA Typename T_RIGHT_PAREN
      { $$ = driver.makeAstNode<OperArgTypesAST>( $1, (TypeAST *) 0, $3, $4->asType(), $5 ); }
    | T_LEFT_PAREN Typename T_COMMA T_NONE T_RIGHT_PAREN
      { $$ = driver.makeAstNode<OperArgTypesAST>( $1, $2->asType(), $3, (TypeAST *) 0, $5 ); }
    ;

/*****************************************************************************
 *    ALTER TYPE enumtype ADD ...
 *****************************************************************************/

AlterEnumStmt:
      T_ALTER T_TYPE any_name T_ADD T_VALUE opt_if_not_exists Sconst
      {
        $$ = driver.makeAstNode<AlterEnumStmtAST>( $1,
                                                   $2,
                                                   $3->asAnyIdentifier(),
                                                   $4,
                                                   $5,
                                                   $6->asIfNotExists(),
                                                   $7,
                                                   0,
                                                   0 );
      }
    | T_ALTER T_TYPE any_name T_ADD T_VALUE opt_if_not_exists Sconst T_BEFORE Sconst
      {
        $$ = driver.makeAstNode<AlterEnumStmtAST>( $1,
                                                   $2,
                                                   $3->asAnyIdentifier(),
                                                   $4,
                                                   $5,
                                                   $6->asIfNotExists(),
                                                   $7,
                                                   $8,
                                                   $9 );
      }
    | T_ALTER T_TYPE any_name T_ADD T_VALUE opt_if_not_exists Sconst T_AFTER Sconst
      {
        $$ = driver.makeAstNode<AlterEnumStmtAST>( $1,
                                                   $2,
                                                   $3->asAnyIdentifier(),
                                                   $4,
                                                   $5,
                                                   $6->asIfNotExists(),
                                                   $7,
                                                   $8,
                                                   $9 );
      }
    ;

opt_if_not_exists:
      T_IF T_NOT T_EXISTS
      { $$ = driver.makeAstNode<IfNotExistsAST>( $1, $2, $3 ); }
    | empty_aka_optional
    ;

/*****************************************************************************
 * Manipulate a domain
 *****************************************************************************/

CreateDomainStmt:
      T_CREATE T_DOMAIN any_name opt_as Typename ColQualList
      {
        $$ = driver.makeAstNode<CreateDomainStmtAST>( $1,
                                                      $2,
                                                      $3->asAnyIdentifier(),
                                                      $4,
                                                      $5->asType(),
                                                      $6 );
      }
    ;

AlterDomainStmt:
      T_ALTER T_DOMAIN any_name alter_column_default
      {
        $$ = driver.makeAstNode<AlterDomainAltDefStmtAST>( $1,
                                                           $2,
                                                           $3->asAnyIdentifier(),
                                                           $4->asAlterDefaultColumn() );
      }
    | T_ALTER T_DOMAIN any_name T_DROP T_NOT T_NULL
      {
        $$ = driver.makeAstNode<AlterDomainChgNotNullStmtAST>( $1,
                                                               $2,
                                                               $3->asAnyIdentifier(),
                                                               $4,
                                                               $5,
                                                               $6 );
      }
    | T_ALTER T_DOMAIN any_name T_SET T_NOT T_NULL
      {
        $$ = driver.makeAstNode<AlterDomainChgNotNullStmtAST>( $1,
                                                               $2,
                                                               $3->asAnyIdentifier(),
                                                               $4,
                                                               $5,
                                                               $6 );
      }
    | T_ALTER T_DOMAIN any_name T_ADD TableConstraint
      {
        $$ = driver.makeAstNode<AlterDomainAddConstrStmtAST>( $1,
                                                              $2,
                                                              $3->asAnyIdentifier(),
                                                              $4,
                                                              $5->asTableConstraint() );
      }
    | T_ALTER T_DOMAIN any_name T_DROP T_CONSTRAINT               name opt_drop_behavior
      {
        $$ = driver.makeAstNode<AlterDomainDropConstrStmtAST>( $1,
                                                               $2,
                                                               $3->asAnyIdentifier(),
                                                               $4,
                                                               $5,
                                                               0, 0,
                                                               driver.createName(NameAST::Constraint, $6),
                                                               $7 );
      }
    | T_ALTER T_DOMAIN any_name T_DROP T_CONSTRAINT T_IF T_EXISTS name opt_drop_behavior
      {
        $$ = driver.makeAstNode<AlterDomainDropConstrStmtAST>( $1,
                                                               $2,
                                                               $3->asAnyIdentifier(),
                                                               $4,
                                                               $5,
                                                               $6, $7,
                                                               driver.createName(NameAST::Constraint, $8),
                                                               $9 );
      }
    | T_ALTER T_DOMAIN any_name T_VALIDATE T_CONSTRAINT name
      {
        $$ = driver.makeAstNode<AlterDomainValConstrStmtAST>( $1,
                                                              $2,
                                                              $3->asAnyIdentifier(),
                                                              $4,
                                                              $5,
                                                              driver.createName(NameAST::Constraint, $6) );
      }
    ;

opt_as:
      T_AS
    | /* EMPTY */ { $$ = 0; }
    ;

/*****************************************************************************
 *    ALTER DEFAULT PRIVILEGES statement
 *****************************************************************************/

AlterDefaultPrivilegesStmt:
      T_ALTER T_DEFAULT T_PRIVILEGES DefACLOptionList DefACLAction
      {
        $$ = driver.makeAstNode<AlterDefaultPrivilegesStmtAST>( $1,
                                                                $2,
                                                                $3,
                                                                $4,
                                                                $5->asDefACLAction() );
      }
    ;

DefACLOptionList:
      DefACLOptionList DefACLOption
      { $$ = driver.makeAstNode<DefACLOptionListAST>( $1, $2->asDefACLOption() ); }
    | /* EMPTY */ { $$ = 0; }
    ;

DefACLOption:
      T_IN T_SCHEMA schema_name_list
      { $$ = driver.makeAstNode<InSchemaDefACLOptionAST>( $1, $2, $3 ); }
    | T_FOR T_ROLE role_list
      { $$ = driver.makeAstNode<ForRoleUserDefACLOptionAST>( $1, $2, $3 ); }
    | T_FOR T_USER role_list
      { $$ = driver.makeAstNode<ForRoleUserDefACLOptionAST>( $1, $2, $3 ); }
    ;

DefACLAction:
      T_GRANT privileges T_ON defacl_privilege_target T_TO   grantee_list opt_grant_grant_option
      {
        $$ = driver.makeAstNode<GrantDefACLActionAST>( $1,
                                                       $2->asPrivileges(),
                                                       $3,
                                                       $4,
                                                       $5,
                                                       $6,
                                                       $7->asWithGrantOption() );
      }
    | T_REVOKE                        privileges T_ON defacl_privilege_target T_FROM grantee_list opt_drop_behavior
      {
        $$ = driver.makeAstNode<RevokeDefACLActionAST>( $1,  0,  0,  0,
                                                        $2->asPrivileges(),
                                                        $3,
                                                        $4,
                                                        $5,
                                                        $6,
                                                        $7 );
      }
    | T_REVOKE T_GRANT T_OPTION T_FOR privileges T_ON defacl_privilege_target T_FROM grantee_list opt_drop_behavior
      {
        $$ = driver.makeAstNode<RevokeDefACLActionAST>( $1, $2, $3, $4,
                                                        $5->asPrivileges(),
                                                        $6,
                                                        $7,
                                                        $8,
                                                        $9,
                                                        $10 );
      }
    ;

defacl_privilege_target:
      T_TABLES
    | T_FUNCTIONS
    | T_SEQUENCES
    | T_TYPES
    ;

grantee_list:
      grantee
      { $$ = driver.makeAstNode<GranteeListAST>( $1->asGrantee() ); }
    | grantee_list T_COMMA grantee
      { $$ = driver.makeAstNode<GranteeListAST>( $1, $3->asGrantee() ); }
    ;

grantee:
      RoleId
      { $$ = driver.makeAstNode<GranteeAST>(  0, $1->asRoleId() ); }
    | T_GROUP RoleId
      { $$ = driver.makeAstNode<GranteeAST>( $1, $2->asRoleId() ); }
    ;


opt_grant_grant_option:
      T_WITH T_GRANT T_OPTION
      { $$ = driver.makeAstNode<WithGrantOptionAST>( $1, $2, $3 ); }
    | empty_aka_optional
    ;

privileges:
      privilege_list
      { $$ = driver.makeAstNode<PrivilegesAsListAST>( $1 ); }
    | T_ALL
      { $$ = driver.makeAstNode<AllPrivilegesAST>( $1,  0,  0,  (NameListAST *) 0,  0 ); }
    | T_ALL T_PRIVILEGES
      { $$ = driver.makeAstNode<AllPrivilegesAST>( $1, $2,  0,  (NameListAST *) 0,  0 ); }
    | T_ALL              T_LEFT_PAREN columnList T_LEFT_PAREN
      { $$ = driver.makeAstNode<AllPrivilegesAST>( $1,  0, $2, $3, $4 ); }
    | T_ALL T_PRIVILEGES T_LEFT_PAREN columnList T_LEFT_PAREN
      { $$ = driver.makeAstNode<AllPrivilegesAST>( $1, $2, $3, $4, $5 ); }
    ;

privilege_list:
      privilege
      { $$ = driver.makeAstNode<CommonPrivilegeListAST>( $1->asCommonPrivilege() ); }
    | privilege_list T_COMMA privilege
      { $$ = driver.makeAstNode<CommonPrivilegeListAST>( $1, $3->asCommonPrivilege() ); }
    ;

privilege:
      T_SELECT opt_column_list
      { $$ = driver.makeAstNode<CommonPrivilegeAST>( $1, $2->asRefColumns() ); }
    | T_REFERENCES opt_column_list
      { $$ = driver.makeAstNode<CommonPrivilegeAST>( $1, $2->asRefColumns() ); }
    | T_CREATE opt_column_list
      { $$ = driver.makeAstNode<CommonPrivilegeAST>( $1, $2->asRefColumns() ); }
    | ColId opt_column_list
      { $$ = driver.makeAstNode<CommonPrivilegeAST>( $1, $2->asRefColumns() ); }
    ;

/*****************************************************************************
 *      ALTER DATABASE
 *****************************************************************************/

AlterDatabaseStmt:
      T_ALTER T_DATABASE database_name opt_with alterdb_opt_list
      { $$ = driver.makeAstNode<AlterDatabaseChgOptStmtAST>( $1, $2, $3->asName(), $4, $5 ); }
    | T_ALTER T_DATABASE database_name T_SET T_TABLESPACE name
      {
        $$ = driver.makeAstNode<AlterDatabaseSetTblspStmtAST>( $1,
                                                               $2,
                                                               $3->asName(),
                                                               $4,
                                                               $5,
                                                               driver.createName(NameAST::Unknown, $6) );
      }
    ;

AlterDatabaseSetStmt:
      T_ALTER T_DATABASE database_name SetResetClause
      {
        $$ = driver.makeAstNode<AlterDatabaseSetStmtAST>( $1,
                                                          $2,
                                                          $3->asName(),
                                                          $4->asSetResetClause() );
      }
    ;


alterdb_opt_list:
      alterdb_opt_list alterdb_opt_item
      { $$ = driver.makeAstNode<AlterDbOptionListAST>( $1, $2->asAlterDbOption() ); }
    | /* EMPTY */ { $$ = 0; }
    ;

alterdb_opt_item:
      T_CONNECTION T_LIMIT opt_equal SignedIconst
      { $$ = driver.makeAstNode<AlterDbConnLimitOptionAST>( $1, $2, $3, $4 ); }
    ;

opt_equal:
      T_EQUAL
    | /*EMPTY*/ { $$ = 0; }
    ;

/*****************************************************************************
 *      QUERIES :
 *          CREATE EVENT TRIGGER ...
 *          ALTER EVENT TRIGGER ...
 *****************************************************************************/

CreateEventTrigStmt:
      T_CREATE T_EVENT T_TRIGGER name T_ON ColLabel T_EXECUTE T_PROCEDURE func_name T_LEFT_PAREN T_RIGHT_PAREN
      {
        $$ = driver.makeAstNode<CreateEventTrigStmtAST>( $1,
                                                         $2,
                                                         $3,
                                                         driver.createName(NameAST::Trigger, $4),
                                                         $5,
                                                         driver.createName(NameAST::Unknown, $6),
                                                         0,
                                                         (EventTriggerWhenListAST *) 0,
                                                         $7,
                                                         $8,
                                                         $9->asFunctionRef(),
                                                         $10,
                                                         $11 );
      }
    | T_CREATE T_EVENT T_TRIGGER name T_ON ColLabel T_WHEN event_trigger_when_list T_EXECUTE T_PROCEDURE func_name T_LEFT_PAREN T_RIGHT_PAREN
      {
        $$ = driver.makeAstNode<CreateEventTrigStmtAST>( $1,
                                                         $2,
                                                         $3,
                                                         driver.createName(NameAST::Trigger, $4),
                                                         $5,
                                                         driver.createName(NameAST::Unknown, $6),
                                                         $7,
                                                         $8,
                                                         $9,
                                                         $10,
                                                         $11->asFunctionRef(),
                                                         $12,
                                                         $13 );
      }
    ;

event_trigger_when_list:
      event_trigger_when_item
      { $$ = driver.makeAstNode<EventTriggerWhenListAST>( $1->asEventTriggerWhen() ); }
    | event_trigger_when_list T_AND event_trigger_when_item
      { $$ = driver.makeAstNode<EventTriggerWhenListAST>( $1, $3->asEventTriggerWhen() ); }
    ;

event_trigger_when_item:
      ColId T_IN T_LEFT_PAREN event_trigger_value_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<EventTriggerWhenAST>( driver.createName(NameAST::Column, $1), $2, $3, $4, $5 ); }
    ;

event_trigger_value_list:
      Sconst
      { $$ = driver.makeAstNode<EventTriggerValueListAST>( driver.makeAstNode<EventTriggerValueAST>($1) ); }
    | event_trigger_value_list T_COMMA Sconst
      { $$ = driver.makeAstNode<EventTriggerValueListAST>( $1, driver.makeAstNode<EventTriggerValueAST>($3) ); }
    ;

AlterEventTrigStmt:
      T_ALTER T_EVENT T_TRIGGER name enable_trigger
      { $$ = driver.makeAstNode<AlterEventTrigStmtAST>( $1, $2, $3, driver.createName(NameAST::Trigger, $4), $5->asEventTriggerCmd() ); }
    ;

enable_trigger:
      T_ENABLE
      { $$ = driver.makeAstNode<EventTriggerCmdAST>( $1,  0 ); }
    | T_ENABLE T_REPLICA
      { $$ = driver.makeAstNode<EventTriggerCmdAST>( $1, $2 ); }
    | T_ENABLE T_ALWAYS
      { $$ = driver.makeAstNode<EventTriggerCmdAST>( $1, $2 ); }
    | T_DISABLE
      { $$ = driver.makeAstNode<EventTriggerCmdAST>( $1,  0 ); }
    ;

 /*****************************************************************************
 *            QUERY: UpdateStmt (UPDATE)
 *****************************************************************************/

UpdateStmt:
      opt_with_clause T_UPDATE relation_expr_opt_alias T_SET set_clause_list from_clause where_or_current_clause returning_clause
      {
        $$ = driver.makeAstNode<UpdateStmtAST>( $1->asWithClause(),
                                                $2,
                                                $3->asRelationAliasExpr(),
                                                $4,
                                                $5,
                                                $6->asFromClause(),
                                                $7->asWhereOrCurrentClause(),
                                                $8->asReturningClause() );
      }
    ;

set_clause_list:
      set_clause
      { $$ = driver.makeAstNode<SetClauseListAST>( $1->asSetClause() ); }
    | set_clause_list T_COMMA set_clause
      { $$ = driver.makeAstNode<SetClauseListAST>( $1, $3->asSetClause() ); }
    ;

set_clause:
      single_set_clause
    | multiple_set_clause
    ;

single_set_clause:
      set_target T_EQUAL ctext_expr
      { $$ = driver.makeAstNode<SingleSetClauseAST>( $1->asColumnRef(), $2, $3->asColumnExpression() ); }
    ;

multiple_set_clause:
      T_LEFT_PAREN set_target_list T_RIGHT_PAREN T_EQUAL ctext_row
      { $$ = driver.makeAstNode<MultipleSetClauseAST>( $1, $2, $3, $4, $5->asColumnExprRow() ); }
    ;

set_target:
      ColId opt_indirection
      { $$ = driver.makeAstNode<ColumnRefAST>( driver.createName(NameAST::Unknown, $1), $2 ); }
    ;

set_target_list:
      set_target
      { $$ = driver.makeAstNode<ColumnRefListAST>( $1->asColumnRef() ); }
    | set_target_list T_COMMA set_target
      { $$ = driver.makeAstNode<ColumnRefListAST>( $1, $3->asColumnRef() ); }
    ;

/*****************************************************************************
 *        QUERY:  INSERT STATEMENTS
 *****************************************************************************/

InsertStmt:
      opt_with_clause T_INSERT T_INTO table_identifier insert_rest returning_clause
      {
        $$ = driver.makeAstNode<InsertStmtAST>( $1->asWithClause(),
                                                $2,
                                                $3,
                                                $4->asTableIdentifier(),
                                                $5->asInsertRest(),
                                                $6->asReturningClause() );
      }
    ;

insert_rest:
      SelectStmt
      { $$ = driver.makeAstNode<CommonInsertRestAST>(  0,  (ColumnRefListAST *) 0,  0, $1->asSelectStmt() ); }
    | T_LEFT_PAREN insert_column_list T_RIGHT_PAREN SelectStmt
      { $$ = driver.makeAstNode<CommonInsertRestAST>( $1, $2, $3, $4->asSelectStmt() ); }
    | T_DEFAULT T_VALUES
      { $$ = driver.makeAstNode<DefaultValuesInsertRestAST>( $1, $2 ); }
    ;

insert_column_list:
      insert_column_item
      { $$ = driver.makeAstNode<ColumnRefListAST>( $1->asColumnRef() ); }
    | insert_column_list T_COMMA insert_column_item
      { $$ = driver.makeAstNode<ColumnRefListAST>( $1, $3->asColumnRef() ); }
    ;

insert_column_item:
      ColId opt_indirection
      { $$ = driver.makeAstNode<ColumnRefAST>( driver.createName(NameAST::Unknown, $1), $2 ); }
    ;

/*****************************************************************************
 *          QUERY:  DELETE STATEMENTS
 *****************************************************************************/

DeleteStmt:
      opt_with_clause T_DELETE T_FROM relation_expr_opt_alias using_clause where_or_current_clause returning_clause
      {
        $$ = driver.makeAstNode<DeleteStmtAST>( $1->asWithClause(),
                                                $2,
                                                $3,
                                                $4->asRelationAliasExpr(),
                                                $5->asDelUsingClause(),
                                                $6->asWhereOrCurrentClause(),
                                                $7->asReturningClause() );
      }
    ;

opt_with_clause:
      with_clause
    | empty_aka_optional
    ;

using_clause:
      T_USING from_list
      { $$ = driver.makeAstNode<DelUsingClauseAST>( $1, $2 ); }
    | empty_aka_optional
    ;

relation_expr_opt_alias:
      relation_expr     %prec UMINUS
      { $$ = driver.makeAstNode<RelationAliasExprAST>( $1->asRelationExpr(),  0, (NameAST *) 0 ); }
    | relation_expr ColId
      {
        RelationExprAST *rel = $1->asRelationExpr();
        NameAST *aliasName = driver.createName(NameAST::TableAlias, $2);
        rel->alias = driver.makeAstNode<TableAliasClauseAST>( 0, aliasName,  0, (NameListAST *) 0, 0 );
        $$ = driver.makeAstNode<RelationAliasExprAST>( rel, 0, aliasName );
      }
    | relation_expr T_AS ColId
      {
        RelationExprAST *rel = $1->asRelationExpr();
        NameAST *aliasName = driver.createName(NameAST::TableAlias, $3);
        rel->alias = driver.makeAstNode<TableAliasClauseAST>( 0, aliasName, $2, (NameListAST *) 0, 0 );
        $$ = driver.makeAstNode<RelationAliasExprAST>( $1->asRelationExpr(), $2, aliasName );
      }
    ;

returning_clause:
      T_RETURNING target_list
      { $$ = driver.makeAstNode<ReturningClauseAST>( $1, $2 ); }
    | empty_aka_optional
    ;

where_or_current_clause:
      T_WHERE a_expr
      { $$ = driver.makeAstNode<WhereNoCurrentClauseAST>( $1, $2->asExpression() ); }
    | T_WHERE T_CURRENT T_OF cursor_name
      { $$ = driver.makeAstNode<CurrenWhereClauseAST>( $1, $2, $3, driver.createName(NameAST::Unknown, $4) ); }
    | empty_aka_optional
    ;

/*****************************************************************************
 * Create a new Postgres DBMS role
 *****************************************************************************/

CreateRoleStmt:
      T_CREATE T_ROLE RoleId opt_with OptRoleList
      { $$ = driver.makeAstNode<CreateRoleStmtAST>( $1,
                                                    $2,
                                                    $3->asRoleId(),
                                                    $4,
                                                    $5 );
      }
    ;
    
opt_with:
      T_WITH
    | /*EMPTY*/ { $$ = 0; }
    ;

OptRoleList:
      OptRoleList CreateOptRoleElem
      { $$ = driver.makeAstNode<OptRoleListAST>( $1, $2->asOptRole() ); }
    | { $$ = 0; }
    ;
    
AlterOptRoleList:
      AlterOptRoleList AlterOptRoleElem
      { $$ = driver.makeAstNode<OptRoleListAST>( $1, $2->asOptRole() ); }
    | { $$ = 0; }
    ;

AlterOptRoleElem:
      T_PASSWORD Sconst                 { $$ = driver.makeAstNode<OptRoleStaticAST>( $1, $2,  0); }
    | T_PASSWORD T_NULL                 { $$ = driver.makeAstNode<OptRoleStaticAST>( $1, $2,  0); }
    | T_ENCRYPTED T_PASSWORD Sconst     { $$ = driver.makeAstNode<OptRoleStaticAST>( $1, $2, $3); }
    | T_UNENCRYPTED T_PASSWORD Sconst   { $$ = driver.makeAstNode<OptRoleStaticAST>( $1, $2, $3); }
    | T_INHERIT                         { $$ = driver.makeAstNode<OptRoleStaticAST>( $1,  0,  0); }
    | T_CONNECTION T_LIMIT SignedIconst { $$ = driver.makeAstNode<OptRoleStaticAST>( $1, $2, $3); }
    | T_VALID T_UNTIL Sconst            { $$ = driver.makeAstNode<OptRoleStaticAST>( $1, $2, $3); }
    | T_USER role_list                  { $$ = driver.makeAstNode<OptRoleWithRoleListAST>( $1,  0, $2); }
    | T_IDENTIFIER
      {
        QString optName = driver.identifierString($1);
        if ( optName != "superuser" && optName != "nosuperuser" &&
             optName != "createuser" && optName != "nocreateuser" &&
             optName != "createrole" && optName != "nocreaterole" &&
             optName != "isreplication" && optName != "noreplication" &&
             optName != "createdb" && optName != "nocreatedb" &&
             optName != "login" && optName != "nologin" && optName != "noinherit" ) {
           driver.error($1, "Unrecognized role option");
         }
         $$ = driver.makeAstNode<OptRoleStaticAST>( $1,  0,  0);
       }
    ;

CreateOptRoleElem:
      AlterOptRoleElem			{ $$ = $1; }
    /* The following are not supported by ALTER ROLE/USER/GROUP */
    | T_SYSID Iconst          { $$ = driver.makeAstNode<OptRoleStaticAST>( $1, $2, 0); }
    | T_ADMIN role_list       { $$ = driver.makeAstNode<OptRoleWithRoleListAST>( $1,  0, $2); }
    | T_ROLE role_list        { $$ = driver.makeAstNode<OptRoleWithRoleListAST>( $1,  0, $2); }
    | T_IN T_ROLE role_list   { $$ = driver.makeAstNode<OptRoleWithRoleListAST>( $1, $2, $3); }
    | T_IN T_GROUP role_list  { $$ = driver.makeAstNode<OptRoleWithRoleListAST>( $1, $2, $3); }
    ;

/*****************************************************************************
 * Create a new Postgres DBMS user (role with implied login ability)
 *****************************************************************************/

CreateUserStmt:
      T_CREATE T_USER RoleId opt_with OptRoleList
      {
        $$ = driver.makeAstNode<CreateRoleStmtAST>( $1,
                                                    $2,
                                                    $3->asRoleId(),
                                                    $4,
                                                    $5 );
      }
    ;

/*****************************************************************************
 * Alter a postgresql DBMS role
 *****************************************************************************/

AlterRoleStmt:
      T_ALTER T_ROLE RoleId opt_with AlterOptRoleList
      {
        $$ = driver.makeAstNode<AlterRoleStmtAST>( $1,
                                                   $2,
                                                   $3->asRoleId(),
                                                   $4,
                                                   $5 );
      }
    ;

opt_in_database:
      empty_aka_optional
    | T_IN T_DATABASE database_name
      { $$ = driver.makeAstNode<InDatabaseClauseAST>( $1, $2, $3->asDatabaseName() ); }
    ;

AlterRoleSetStmt:
      T_ALTER T_ROLE RoleId opt_in_database SetResetClause
      {
        $$ = driver.makeAstNode<AlterAnyRoleSetStmtAST>( $1,
                                                         $2,
                                                         $3->asRoleId(),
                                                         $4->asInDatabaseClause(),
                                                         $5->asSetResetClause() );
      }
    | T_ALTER T_ROLE T_ALL opt_in_database SetResetClause
      {
        $$ = driver.makeAstNode<AlterAllRolesSetStmtAST>( $1,
                                                          $2,
                                                          $3,
                                                          $4->asInDatabaseClause(),
                                                          $5->asSetResetClause() );
      }
    ;

/*****************************************************************************
 * Alter a postgresql DBMS user
 *****************************************************************************/

AlterUserStmt:
      T_ALTER T_USER RoleId opt_with AlterOptRoleList
      {
        $$ = driver.makeAstNode<AlterRoleStmtAST>( $1,
                                                   $2,
                                                   $3->asRoleId(),
                                                   $4,
                                                   $5 );
      }
    ;

AlterUserSetStmt:
      T_ALTER T_USER RoleId SetResetClause
      {
        $$ = driver.makeAstNode<AlterAnyRoleSetStmtAST>( $1,
                                                         $2,
                                                         $3->asRoleId(),
                                                         (InDatabaseClauseAST *) 0,
                                                         $4->asSetResetClause() );
      }
    ;

/*****************************************************************************
 * Create/Alter a postgresql group (role without login ability)
 *****************************************************************************/

CreateGroupStmt:
      T_CREATE T_GROUP RoleId opt_with OptRoleList
      { $$ = driver.makeAstNode<CreateGroupStmtAST>( $1, $2, $3->asRoleId(), $4, $5 ); }
    ;

AlterGroupStmt:
      T_ALTER T_GROUP RoleId add_drop T_USER role_list
      { $$ = driver.makeAstNode<AlterGroupStmtAST>( $1, $2, $3->asRoleId(), $4, $5, $6 ); }
    ;

add_drop:
      T_ADD
    | T_DROP
    ;

/*****************************************************************************
 * DISCARD { ALL | TEMP | PLANS }
 *****************************************************************************/

DiscardStmt:
      T_DISCARD T_ALL       { $$ = driver.makeAstNode<DiscardStmtAST>( $1, $2 ); }
    | T_DISCARD T_TEMP      { $$ = driver.makeAstNode<DiscardStmtAST>( $1, $2 ); }
    | T_DISCARD T_TEMPORARY { $$ = driver.makeAstNode<DiscardStmtAST>( $1, $2 ); }
    | T_DISCARD T_PLANS     { $$ = driver.makeAstNode<DiscardStmtAST>( $1, $2 ); }
    ;

/*****************************************************************************
 *	ALTER [ TABLE | INDEX | SEQUENCE | VIEW | MATERIALIZED VIEW ] variations
 *****************************************************************************/

AlterStmt:
      T_ALTER T_TABLE alter_relation_expr alter_table_cmds
      {
        StatementAST::Info *info = driver.currentStatementInfo();
        info->type = StatementAST::Info::Alter;
        info->target = StatementAST::Info::Table;
        RelationExprAST *relExpr = $3->asRelationExpr();
        TableIdentifierAST *tableIdent = driver.relationIdentifier(relExpr);
        if ( tableIdent ) {
          info->schemaName = tableIdent->schemaName;
          info->objectName = tableIdent->tableName;
        }
        driver.appendObjectIdentifierHelper(StatementAST::CompletionHelper::TableIdentifier, $2 + 1);
        AlterTableStmtAST *ast = driver.makeAstNode<AlterTableStmtAST>( $1, $2,  0,  0, relExpr, $4 );
        $$ = ast;
        if ( !$4 )
          driver.appendKeywordsHelper(ast->lastToken() - 1, "ALTER COLUMN;ADD COLUMN;DROP COLUMN;DROP COLUMN IF EXISTS;ADD CONSTRAINT;DROP CONSTRAINT;DROP CONSTRAINT IF EXISTS;VALIDATE CONSTRAINT;SET WITH OIDS;SET WITHOUT OIDS;CLUSTER ON;SET WITHOUT CLUSTER;ENABLE TRIGGER;ENABLE ALWAYS TRIGGER;ENABLE REPLICA TRIGGER;ENABLE TRIGGER ALL;ENABLE TRIGGER USER;DISABLE TRIGGER;DISABLE ALWAYS TRIGGER;DISABLE REPLICA TRIGGER;DISABLE TRIGGER ALL;DISABLE TRIGGER USER");
      }
    | T_ALTER T_TABLE T_IF T_EXISTS alter_relation_expr alter_table_cmds
      { $$ = driver.makeAstNode<AlterTableStmtAST>( $1, $2, $3, $4, $5->asRelationExpr(), $6 ); }
    | T_ALTER T_INDEX index_identifier alter_table_cmds
      { $$ = driver.makeAstNode<AlterIndexStmtAST>( $1, $2,  0,  0, $3->asIndexIdentifier(), $4 ); }
    | T_ALTER T_INDEX T_IF T_EXISTS index_identifier alter_table_cmds
      { $$ = driver.makeAstNode<AlterIndexStmtAST>( $1, $2, $3, $4, $5->asIndexIdentifier(), $6 ); }
    | T_ALTER T_SEQUENCE sequence_identifier alter_table_cmds
      { $$ = driver.makeAstNode<AlterSequenceStmtAST>( $1, $2,  0,  0, $3->asSequenceIdentifier(), $4 ); }
    | T_ALTER T_SEQUENCE T_IF T_EXISTS sequence_identifier alter_table_cmds
      { $$ = driver.makeAstNode<AlterSequenceStmtAST>( $1, $2, $3, $4, $5->asSequenceIdentifier(), $6 ); }
    | T_ALTER T_VIEW table_identifier alter_table_cmds
      { $$ = driver.makeAstNode<AlterViewStmtAST>( $1,  0, $2,  0,  0, $3->asTableIdentifier(), $4 ); }
    | T_ALTER T_VIEW T_IF T_EXISTS table_identifier alter_table_cmds
      { $$ = driver.makeAstNode<AlterViewStmtAST>( $1,  0, $2, $3, $4, $5->asTableIdentifier(), $6 ); }
    | T_ALTER T_MATERIALIZED T_VIEW table_identifier alter_table_cmds
      { $$ = driver.makeAstNode<AlterViewStmtAST>( $1, $2, $3,  0,  0, $4->asTableIdentifier(), $5 ); }
    | T_ALTER T_MATERIALIZED T_VIEW T_IF T_EXISTS table_identifier alter_table_cmds
      { $$ = driver.makeAstNode<AlterViewStmtAST>( $1, $2, $3, $4, $5, $6->asTableIdentifier(), $7 ); }
    ;

/*****************************************************************************
 *  QUERY:
 *    SELECT STATEMENTS
 *****************************************************************************/

SelectStmt:
      select_no_parens        %prec UMINUS
    | select_with_parens      %prec UMINUS
    ;

select_with_parens:
      T_LEFT_PAREN select_no_parens T_RIGHT_PAREN
      { $$ = driver.makeAstNode<SelectWithParensAST>( $1, $2->asSelectStmt(), $3 ); }
    | T_LEFT_PAREN select_with_parens T_RIGHT_PAREN
      { $$ = driver.makeAstNode<SelectWithParensAST>( $1, $2->asSelectStmt(), $3 ); }
    ;

select_no_parens:
      simple_select
      {
        $$ = driver.makeAstNode<SelectNoParensAST>( (WithClauseAST *) 0,
                                                    driver.makeAstNode<CoreSimpleSelectAST>( $1->asSimpleSelect() ),
                                                    (SortClauseAST *) 0,
                                                    (LockingLimitClauseAST *) 0 );
      }
    | select_clause sort_clause
      {
        $$ = driver.makeAstNode<SelectNoParensAST>( (WithClauseAST *) 0,
                                                    $1->asSelectClause(),
                                                    $2->asSortClause(),
                                                    (LockingLimitClauseAST *) 0 );
      }
    | select_clause opt_sort_clause for_locking_clause opt_select_limit
      {
        $$ = driver.makeAstNode<SelectNoParensAST>( (WithClauseAST *) 0,
                                                    $1->asSelectClause(),
                                                    $2->asSortClause(),
                                                    driver.makeAstNode<LockingFirstLimitClauseAST>( $3->asForLockingClause(),
                                                                                                    $4->asLimitClause() ) );
      }
    | select_clause opt_sort_clause select_limit opt_for_locking_clause
      {
        $$ = driver.makeAstNode<SelectNoParensAST>( (WithClauseAST *) 0,
                                                    $1->asSelectClause(),
                                                    $2->asSortClause(),
                                                    driver.makeAstNode<LockingLastLimitClauseAST>( $3->asLimitClause(),
                                                                                                   $4->asForLockingClause() ) );
      }
    | with_clause select_clause
      {
        $$ = driver.makeAstNode<SelectNoParensAST>( $1->asWithClause(),
                                                    $2->asSelectClause(),
                                                    (SortClauseAST *) 0,
                                                    (LockingLimitClauseAST *) 0 );
      }
    | with_clause select_clause sort_clause
      {
        $$ = driver.makeAstNode<SelectNoParensAST>( $1->asWithClause(),
                                                    $2->asSelectClause(),
                                                    $3->asSortClause(),
                                                    (LockingLimitClauseAST *) 0 );
      }
    | with_clause select_clause opt_sort_clause for_locking_clause opt_select_limit
      {
        $$ = driver.makeAstNode<SelectNoParensAST>( $1->asWithClause(),
                                                    $2->asSelectClause(),
                                                    $3->asSortClause(),
                                                    driver.makeAstNode<LockingFirstLimitClauseAST>( $4->asForLockingClause(),
                                                                                                    $5->asLimitClause() ) );
      }
    | with_clause select_clause opt_sort_clause select_limit opt_for_locking_clause
      {
        $$ = driver.makeAstNode<SelectNoParensAST>( $1->asWithClause(),
                                                    $2->asSelectClause(),
                                                    $3->asSortClause(),
                                                    driver.makeAstNode<LockingLastLimitClauseAST>( $4->asLimitClause(),
                                                                                                   $5->asForLockingClause() ) );
      }
    ;

select_clause:
      simple_select
      { $$ = driver.makeAstNode<CoreSimpleSelectAST>( $1->asSimpleSelect() ); }
    | select_with_parens
      { $$ = driver.makeAstNode<CompoundSelectClauseAST>( $1->asSelectWithParens() ); }
    ;

simple_select:
      T_SELECT opt_distinct target_list into_clause from_clause where_clause group_clause having_clause window_clause
      {
        $$ = driver.makeAstNode<SelectCoreAST>( $1,
                                                $2->asDistinctClause(),
                                                $3,
                                                $4->asIntoClause(),
                                                $5->asFromClause(),
                                                $6->asWhereClause(),
                                                $7->asGroupByClause(),
                                                $8->asHavingClause(),
                                                $9->asWindowClause() );
      }
    | values_clause
      { $$ = driver.makeAstNode<ValuesSimpleSelectAST>( $1->asValuesClause() ); }
    | T_TABLE relation_expr
      { $$ = driver.makeAstNode<TableSimpleSelectAST>( $1, $2->asRelationExpr() ); }
    | select_clause T_UNION opt_all select_clause
      { $$ = driver.makeAstNode<CompositeSimpleSelectAST>( $1->asSelectClause(),  $2, $3, $4->asSelectClause() ); }
    | select_clause T_INTERSECT opt_all select_clause
      { $$ = driver.makeAstNode<CompositeSimpleSelectAST>( $1->asSelectClause(),  $2, $3, $4->asSelectClause() ); }
    | select_clause T_EXCEPT opt_all select_clause
      { $$ = driver.makeAstNode<CompositeSimpleSelectAST>( $1->asSelectClause(),  $2, $3, $4->asSelectClause() ); }
    ;

window_clause:
      T_WINDOW window_definition_list
      { $$ = driver.makeAstNode<WindowClauseAST>( $1, $2 ); }
    | empty_aka_optional
    ;

window_definition_list:
      window_definition
      { $$ = driver.makeAstNode<WindowDefinitionListAST>( $1->asWindowDefinition() ); }
    | window_definition_list T_COMMA window_definition
      { $$ = driver.makeAstNode<WindowDefinitionListAST>( $1, $3->asWindowDefinition() ); }
    ;

window_definition:
      ColId T_AS window_specification
      { $$ = driver.makeAstNode<WindowDefinitionAST>( driver.createName(NameAST::Window, $1), $2, $3->asWindowSpecification() ); }
    ;

having_clause:
      T_HAVING a_expr
      { $$ = driver.makeAstNode<HavingClauseAST>( $1, $2->asExpression() ); }
    | empty_aka_optional
    ;

group_clause:
      T_GROUP T_BY expr_list
      { $$ = driver.makeAstNode<GroupByClauseAST>( $1, $2, $3 ); }
    | empty_aka_optional
    ;

where_clause:
      T_WHERE a_expr
      { $$ = driver.makeAstNode<WhereClauseAST>( $1, $2->asExpression() ); }
    | empty_aka_optional
    ;

from_clause:
      T_FROM from_list
      { $$ = driver.makeAstNode<FromClauseAST>( $1, $2 ); }
    | empty_aka_optional
    ;

from_list:
      table_ref
      { $$ = driver.makeAstNode<TableRefListAST>( $1->asTableRef() ); }
    | from_list T_COMMA table_ref
      { $$ = driver.makeAstNode<TableRefListAST>( $1, $3->asTableRef() ); }
    ;

alias_clause:
      T_AS ColId T_LEFT_PAREN name_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<TableAliasClauseAST>( $1, driver.createName(NameAST::TableAlias, $2), $3, $4, $5 ); }
    | T_AS ColId
      { $$ = driver.makeAstNode<TableAliasClauseAST>( $1, driver.createName(NameAST::TableAlias, $2),  0, (NameListAST *) 0, 0 ); }
    | ColId T_LEFT_PAREN name_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<TableAliasClauseAST>(  0, driver.createName(NameAST::TableAlias, $1), $2, $3, $4 ); }
    | ColId
      { $$ = driver.makeAstNode<TableAliasClauseAST>(  0, driver.createName(NameAST::TableAlias, $1),  0, (NameListAST *) 0, 0 ); }
    ;

opt_alias_clause:
      alias_clause
    | empty_aka_optional
    ;

func_alias_clause:
      alias_clause
      { $$ = driver.makeAstNode<TableFuncAliasClauseAST>( $1->asTableAliasClause() ); }
    | T_AS T_LEFT_PAREN TableFuncElementList T_RIGHT_PAREN
      { $$ = driver.makeAstNode<FuncFuncAliasClauseAST>( $1, (NameAST *) 0, $2, $3, $4 ); }
    | T_AS ColId T_LEFT_PAREN TableFuncElementList T_RIGHT_PAREN
      { $$ = driver.makeAstNode<FuncFuncAliasClauseAST>( $1, driver.createName(NameAST::Unknown, $2), $3, $4, $5 ); }
    | ColId T_LEFT_PAREN TableFuncElementList T_RIGHT_PAREN
      { $$ = driver.makeAstNode<FuncFuncAliasClauseAST>(  0, driver.createName(NameAST::Unknown, $1), $2, $3, $4 ); }
    | empty_aka_optional
    ;

TableFuncElement:
      ColId Typename opt_collate_clause
      {
        $$ = driver.makeAstNode<TableFuncElementAST>( driver.createName(NameAST::Unknown, $1),
                                                      $2->asType(),
                                                      $3->asCollateClause() );
      }
    ;

TableFuncElementList:
      TableFuncElement
      { $$ = driver.makeAstNode<TableFuncElementListAST>( $1->asTableFuncElement() ); }
    | TableFuncElementList T_COMMA TableFuncElement
      { $$ = driver.makeAstNode<TableFuncElementListAST>( $1, $3->asTableFuncElement() ); }
    ;

func_table: func_expr ;

table_ref:
      relation_expr opt_alias_clause
      {
        RelationExprAST *rel = $1->asRelationExpr();
        rel->alias = $2->asTableAliasClause();
        $$ = driver.makeAstNode<DirectTableRefAST>( rel, rel->alias );
      }
    | func_table func_alias_clause
      { $$ = driver.makeAstNode<FuncionTableRefAST>(  0, $1->asFunctionExpression(), $2->asFuncAliasClause() ); }
    | T_LATERAL func_table func_alias_clause
      { $$ = driver.makeAstNode<FuncionTableRefAST>( $1, $2->asFunctionExpression(), $3->asFuncAliasClause() ); }
    | select_with_parens opt_alias_clause
      { $$ = driver.makeAstNode<CompoundTableRefAST>(  0, $1->asSelectWithParens(), $2->asTableAliasClause() ); }
    | T_LATERAL select_with_parens opt_alias_clause
      { $$ = driver.makeAstNode<CompoundTableRefAST>( $1, $2->asSelectWithParens(), $3->asTableAliasClause() ); }
    | joined_table
      { $$ = driver.makeAstNode<JoinedTableRefAST>(  0, $1->asJoinedTable(),  0 ); }
    | T_LEFT_PAREN joined_table T_RIGHT_PAREN alias_clause
      { $$ = driver.makeAstNode<JoinedTableRefAST>( $1, $2->asJoinedTable(), $3 ); }
    ;

joined_table:
      T_LEFT_PAREN joined_table T_RIGHT_PAREN
      { $$ = driver.makeAstNode<CompoundJoinedTableAST>( $1, $2->asJoinedTable(), $3 ); }
    | table_ref T_CROSS T_JOIN table_ref
      { $$ = driver.makeAstNode<CrossJoinedTableAST>( $1->asTableRef(), $2, $3, $4->asTableRef() ); }
    | table_ref           join_type T_JOIN table_ref join_qual
      { $$ = driver.makeAstNode<CommonJoinedTableAST>( $1->asTableRef(),  0, $2->asJoinType(),  $3, $4->asTableRef(), $5->asJoinCondition() ); }
    | table_ref                     T_JOIN table_ref join_qual
      { $$ = driver.makeAstNode<CommonJoinedTableAST>( $1->asTableRef(),  0, (JoinTypeAST *) 0, $2, $3->asTableRef(), $4->asJoinCondition() ); }
    | table_ref T_NATURAL join_type T_JOIN table_ref
      { $$ = driver.makeAstNode<CommonJoinedTableAST>( $1->asTableRef(), $2,  $3->asJoinType(), $4, $5->asTableRef(), (JoinConditionAST *) 0 ); }
    | table_ref T_NATURAL           T_JOIN table_ref
      { $$ = driver.makeAstNode<CommonJoinedTableAST>( $1->asTableRef(), $2, (JoinTypeAST *) 0, $3, $4->asTableRef(), (JoinConditionAST *) 0 ); }
    ;

join_qual:
      T_USING T_LEFT_PAREN name_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<UsingJoinConditionAST>( $1, $2, $3, $4 ); }
    | T_ON a_expr
      { $$ = driver.makeAstNode<OnJoinConditionAST>( $1, $2->asExpression() ); }
    ;

join_type:
      T_FULL  join_outer
      { $$ = driver.makeAstNode<JoinTypeAST>( $1, $2 ); }
    | T_LEFT  join_outer
      { $$ = driver.makeAstNode<JoinTypeAST>( $1, $2 ); }
    | T_RIGHT join_outer
      { $$ = driver.makeAstNode<JoinTypeAST>( $1, $2 ); }
    |         T_INNER
      { $$ = driver.makeAstNode<JoinTypeAST>(  0, $1 ); }
    ;

join_outer:
      T_OUTER
    | /*EMPTY*/ { $$ = 0; }
    ;

func_name:
      type_function_name
      { $$ = driver.makeAstNode<FunctionRefAST>( driver.createName(NameAST::Routine, $1), (IndirectionListAST *) 0 ); }
    | ColId indirection
      { $$ = driver.makeAstNode<FunctionRefAST>( driver.createName(NameAST::Routine, $1), $2 ); }
    ;

extract_list:
      extract_arg T_FROM a_expr
      { $$ = driver.makeAstNode<ExtractExprAST>( $1, $2, $3->asExpression() ); }
    | empty_aka_optional
    ;

/* Allow delimited string Sconst in extract_arg as an SQL extension.
 * - thomas 2001-04-12
 */
extract_arg:
      T_IDENTIFIER
    | T_YEAR
    | T_MONTH
    | T_DAY
    | T_HOUR
    | T_MINUTE
    | T_SECOND
    | Sconst
    ;

param_name: type_function_name ;

func_arg_expr:
      a_expr
      { $$ = driver.makeAstNode<FunctionArgExprAST>( (NameAST *) 0, 0, $1->asExpression() ); }
    | param_name T_ASSIGN a_expr
      { $$ = driver.makeAstNode<FunctionArgExprAST>( driver.createName(NameAST::Unknown, $1), $2, $3->asExpression() ); }
    ;

func_arg_list:
      func_arg_expr
      { $$ = driver.makeAstNode<FunctionArgExprListAST>( $1->asFunctionArgExpr() ); }
    | func_arg_list T_COMMA func_arg_expr
      { $$ = driver.makeAstNode<FunctionArgExprListAST>( $1, $3->asFunctionArgExpr() ); }
    ;

opt_collate_clause:
      T_COLLATE any_name
      { $$ = driver.makeAstNode<CollateClauseAST>( $1, $2->asName() ); }
    | empty_aka_optional
    ;

opt_xml_root_standalone:
      T_COMMA T_STANDALONE T_YES
      { $$ = driver.makeAstNode<XmlRootStandaloneAST>( $1, $2, $3,  0 ); }
    | T_COMMA T_STANDALONE T_NO
      { $$ = driver.makeAstNode<XmlRootStandaloneAST>( $1, $2, $3,  0 ); }
    | T_COMMA T_STANDALONE T_NO T_VALUE
      { $$ = driver.makeAstNode<XmlRootStandaloneAST>( $1, $2, $3, $4 ); }
    | empty_aka_optional
    ;

over_clause:
      T_OVER window_specification
      { $$ = driver.makeAstNode<WindowOverClauseAST>( $1, $2->asWindowSpecification() ); }
    | T_OVER ColId
      { $$ = driver.makeAstNode<SimpleOverClauseAST>( $1, driver.createName(NameAST::Unknown, $2) ); }
    | empty_aka_optional
    ;

window_specification:
      T_LEFT_PAREN opt_existing_window_name opt_partition_clause opt_sort_clause opt_frame_clause T_RIGHT_PAREN
      {
        $$ = driver.makeAstNode<WindowSpecificationAST>( $1,
                                                         driver.createName(NameAST::Window, $2),
                                                         $3->asPartitionClause(),
                                                         $4->asSortClause(),
                                                         $5->asFrameClause(),
                                                         $6 );
      }
    ;

opt_frame_clause:
      T_RANGE frame_extent
      { $$ = driver.makeAstNode<FrameClauseAST>( $1, $2->asFrameExtent() ); }
    | T_ROWS frame_extent
      { $$ = driver.makeAstNode<FrameClauseAST>( $1, $2->asFrameExtent() ); }
    | empty_aka_optional
    ;

frame_extent:
      frame_bound
      { $$ = driver.makeAstNode<BoundFrameExtentAST>( $1->asFrameBound() ); }
    | T_BETWEEN frame_bound T_AND frame_bound
      { $$ = driver.makeAstNode<BetweenFrameExtentAST>( $1, $2->asFrameBound(), $3, $4->asFrameBound() ); }
    ;

frame_bound:
      T_UNBOUNDED T_PRECEDING
      { $$ = driver.makeAstNode<SimpleFrameBoundAST>( $1, $2 ); }
    | T_UNBOUNDED T_FOLLOWING
      { $$ = driver.makeAstNode<SimpleFrameBoundAST>( $1, $2 ); }
    | T_CURRENT T_ROW
      { $$ = driver.makeAstNode<SimpleFrameBoundAST>( $1, $2 ); }
    | a_expr T_PRECEDING
      { $$ = driver.makeAstNode<ExprFrameBoundAST>( $1->asExpression(), $2 ); }
    | a_expr T_FOLLOWING
      { $$ = driver.makeAstNode<ExprFrameBoundAST>( $1->asExpression(), $2 ); }
    ;

opt_existing_window_name:
      ColId
    | /*EMPTY*/ %prec Op  { $$ = 0; }
    ;

opt_partition_clause:
      T_PARTITION T_BY expr_list
      { $$ = driver.makeAstNode<PartitionClauseAST>( $1, $2,$3 ); }
    | empty_aka_optional
    ;

overlay_list:
      a_expr overlay_placing substr_from substr_for
    | a_expr overlay_placing substr_from
    ;

substr_from:
      T_FROM a_expr
      { $$ = driver.makeAstNode<SubstringFromAST>( $1, $2->asExpression() ); }
    ;

substr_for:
      T_FOR a_expr
      { $$ = driver.makeAstNode<SubstringForAST>( $1, $2->asExpression() ); }
    ;

overlay_placing:
      T_PLACING a_expr
    ;

position_list:
      b_expr T_IN b_expr
    | empty_aka_optional
    ;

substr_list:
      a_expr substr_from substr_for
      { $$ = driver.makeAstNode<StandardSubstringExprAST>( $1->asExpression(), $2->asSubstringFrom(), $3->asSubstringFor() ); }
    | a_expr substr_for substr_from
      { $$ = driver.makeAstNode<StandardSubstringExprAST>( $1->asExpression(), $3->asSubstringFrom(), $2->asSubstringFor() ); }
    | a_expr substr_from
      { $$ = driver.makeAstNode<StandardSubstringExprAST>( $1->asExpression(), $2->asSubstringFrom(), (SubstringForAST *) 0); }
    | a_expr substr_for
      { $$ = driver.makeAstNode<StandardSubstringExprAST>( $1->asExpression(), (SubstringFromAST *) 0, $2->asSubstringFor() ); }
    | expr_list
      { $$ = driver.makeAstNode<ListSubstringExprAST>( $1 ); }
    | empty_aka_optional
    ;

trim_list:
      a_expr T_FROM expr_list
      { $$ = driver.makeAstNode<TrimExprAST>( $1->asExpression(), $2, $3 ); }
    | T_FROM expr_list
      { $$ = driver.makeAstNode<TrimExprAST>( (ExpressionAST *) 0, $1, $2 ); }
    | expr_list
      { $$ = driver.makeAstNode<TrimExprAST>( (ExpressionAST *) 0,  0, $1 ); }
    ;

xml_attributes:
      T_XMLATTRIBUTES T_LEFT_PAREN xml_attribute_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<XmlAttributesAST>( $1, $2, $3, $4 ); }
    ;

xml_attribute_list:
      xml_attribute_el
      { $$ = driver.makeAstNode<XmlAttributeListAST>( $1->asXmlAttribute() ); }
    | xml_attribute_list T_COMMA xml_attribute_el
      { $$ = driver.makeAstNode<XmlAttributeListAST>( $1, $3->asXmlAttribute() ); }
    ;

xml_attribute_el:
      a_expr T_AS ColLabel
      { $$ = driver.makeAstNode<XmlAttributeAST>( $1->asExpression(), $2, driver.createName(NameAST::Unknown, $2) ); }
    | a_expr
      { $$ = driver.makeAstNode<XmlAttributeAST>( $1->asExpression(),  0, (NameAST *) 0 ); }
    ;

xml_root_version:
      T_VERSION a_expr
      { $$ = driver.makeAstNode<ExprXmlRootVersionAST>( $1, $2->asExpression() ); }
    | T_VERSION T_NO T_VALUE
      { $$ = driver.makeAstNode<NoValueXmlRootVersionAST>( $1, $2, $3 ); }
    ;

xml_whitespace_option:
      T_PRESERVE T_WHITESPACE
      { $$ = driver.makeAstNode<XmlWhitespaceOptionAST>( $1, $2 ); }
    | T_STRIP T_WHITESPACE
      { $$ = driver.makeAstNode<XmlWhitespaceOptionAST>( $1, $2 ); }
    | empty_aka_optional
    ;

xmlexists_argument:
      T_PASSING c_expr
      { $$ = driver.makeAstNode<XmlExistsArgumentAST>( $1,  0,  0, $2->asExpression(),  0,  0 ); }
    | T_PASSING c_expr T_BY T_REF
      { $$ = driver.makeAstNode<XmlExistsArgumentAST>( $1,  0,  0, $2->asExpression(), $3, $4 ); }
    | T_PASSING T_BY T_REF c_expr
      { $$ = driver.makeAstNode<XmlExistsArgumentAST>( $1, $2, $3, $4->asExpression(),  0,  0 ); }
    | T_PASSING T_BY T_REF c_expr T_BY T_REF
      { $$ = driver.makeAstNode<XmlExistsArgumentAST>( $1, $2, $3, $4->asExpression(), $5, $6 ); }
    ;

func_expr:
      func_name T_LEFT_PAREN T_RIGHT_PAREN over_clause
      {
        $$ = driver.makeAstNode<CommonFunctionExprAST>( $1->asFunctionRef(),
                                                        $2,
                                                        0,
                                                        (FunctionArgExprListAST *) 0,
                                                        (SortClauseAST *) 0,
                                                        0, 0, (FunctionArgExprAST *) 0,
                                                        $3,
                                                        $4->asOverClause() );
      }
    | func_name T_LEFT_PAREN func_arg_list T_RIGHT_PAREN over_clause
      {
        $$ = driver.makeAstNode<CommonFunctionExprAST>( $1->asFunctionRef(),
                                                        $2,
                                                        0,
                                                        $3,
                                                        (SortClauseAST *) 0,
                                                        0, 0, (FunctionArgExprAST *) 0,
                                                        $4,
                                                        $5->asOverClause() );
      }
    | func_name T_LEFT_PAREN T_VARIADIC func_arg_expr T_RIGHT_PAREN over_clause
      {
        $$ = driver.makeAstNode<CommonFunctionExprAST>( $1->asFunctionRef(),
                                                        $2,
                                                        0,
                                                        (FunctionArgExprListAST *) 0,
                                                        (SortClauseAST *) 0,
                                                        0, $3, $4->asFunctionArgExpr(),
                                                        $5,
                                                        $6->asOverClause() );
      }
    | func_name T_LEFT_PAREN func_arg_list T_COMMA T_VARIADIC func_arg_expr T_RIGHT_PAREN over_clause
      {
        $$ = driver.makeAstNode<CommonFunctionExprAST>( $1->asFunctionRef(),
                                                        $2,
                                                        0,
                                                        $3,
                                                        (SortClauseAST *) 0,
                                                        $4, $5, $6->asFunctionArgExpr(),
                                                        $7,
                                                        $8->asOverClause() );
      }
    | func_name T_LEFT_PAREN func_arg_list sort_clause T_RIGHT_PAREN over_clause
      {
        $$ = driver.makeAstNode<CommonFunctionExprAST>( $1->asFunctionRef(),
                                                        $2,
                                                        0,
                                                        $3,
                                                        $4->asSortClause(),
                                                        0, 0, (FunctionArgExprAST *) 0,
                                                        $5,
                                                        $6->asOverClause() );
      }
    | func_name T_LEFT_PAREN T_ALL func_arg_list opt_sort_clause T_RIGHT_PAREN over_clause
      {
        $$ = driver.makeAstNode<CommonFunctionExprAST>( $1->asFunctionRef(),
                                                        $2,
                                                        $3,
                                                        $4,
                                                        $5->asSortClause(),
                                                        0, 0, (FunctionArgExprAST *) 0,
                                                        $6,
                                                        $7->asOverClause() );
      }
    | func_name T_LEFT_PAREN T_DISTINCT func_arg_list opt_sort_clause T_RIGHT_PAREN over_clause
      {
        $$ = driver.makeAstNode<CommonFunctionExprAST>( $1->asFunctionRef(),
                                                        $2,
                                                        $3,
                                                        $4,
                                                        $5->asSortClause(),
                                                        0, 0, (FunctionArgExprAST *) 0,
                                                        $6,
                                                        $7->asOverClause() );
      }
    | func_name T_LEFT_PAREN T_STAR T_RIGHT_PAREN over_clause
      {
        $$ = driver.makeAstNode<CommonFunctionExprAST>( $1->asFunctionRef(),
                                                        $2,
                                                        $3,
                                                        (FunctionArgExprListAST *) 0,
                                                        (SortClauseAST *) 0,
                                                        0, 0, (FunctionArgExprAST *) 0,
                                                        $4,
                                                        $5->asOverClause() );
      }
    | T_COLLATION T_FOR T_LEFT_PAREN a_expr T_RIGHT_PAREN
      { $$ = driver.makeAstNode<CollationFunctionExprAST>( $1, $2, $3, $4->asExpression(), $5 ); }
    | T_CURRENT_DATE
      { $$ = driver.makeAstNode<SimpleFunctionExprAST>( $1,  0,  0,  0 ); }
    | T_CURRENT_TIME
      { $$ = driver.makeAstNode<SimpleFunctionExprAST>( $1,  0,  0,  0 ); }
    | T_CURRENT_TIME T_LEFT_PAREN Iconst T_RIGHT_PAREN
      { $$ = driver.makeAstNode<SimpleFunctionExprAST>( $1, $2, $3, $4 ); }
    | T_CURRENT_TIMESTAMP
      { $$ = driver.makeAstNode<SimpleFunctionExprAST>( $1,  0,  0,  0 ); }
    | T_CURRENT_TIMESTAMP T_LEFT_PAREN Iconst T_RIGHT_PAREN
      { $$ = driver.makeAstNode<SimpleFunctionExprAST>( $1, $2, $3, $4 ); }
    | T_LOCALTIME
      { $$ = driver.makeAstNode<SimpleFunctionExprAST>( $1,  0,  0,  0 ); }
    | T_LOCALTIME T_LEFT_PAREN Iconst T_RIGHT_PAREN
      { $$ = driver.makeAstNode<SimpleFunctionExprAST>( $1, $2, $3, $4 ); }
    | T_LOCALTIMESTAMP
      { $$ = driver.makeAstNode<SimpleFunctionExprAST>( $1,  0,  0,  0 ); }
    | T_LOCALTIMESTAMP T_LEFT_PAREN Iconst T_RIGHT_PAREN
      { $$ = driver.makeAstNode<SimpleFunctionExprAST>( $1, $2, $3, $4 ); }
    | T_CURRENT_ROLE
      { $$ = driver.makeAstNode<SimpleFunctionExprAST>( $1,  0,  0,  0 ); }
    | T_CURRENT_USER
      { $$ = driver.makeAstNode<SimpleFunctionExprAST>( $1,  0,  0,  0 ); }
    | T_SESSION_USER
      { $$ = driver.makeAstNode<SimpleFunctionExprAST>( $1,  0,  0,  0 ); }
    | T_USER
      { $$ = driver.makeAstNode<SimpleFunctionExprAST>( $1,  0,  0,  0 ); }
    | T_CURRENT_CATALOG
      { $$ = driver.makeAstNode<SimpleFunctionExprAST>( $1,  0,  0,  0 ); }
    | T_CURRENT_SCHEMA
      { $$ = driver.makeAstNode<SimpleFunctionExprAST>( $1,  0,  0,  0 ); }
    | T_CAST T_LEFT_PAREN a_expr T_AS Typename T_RIGHT_PAREN
      { $$ = driver.makeAstNode<CastFunctionExprAST>( $1, $2, $3->asExpression(), $4, $5->asType(), $6 ); }
    | T_EXTRACT T_LEFT_PAREN extract_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<ExtractFunctionExprAST>( $1, $2, $3->asExtractExpr(), $4 ); }
    | T_OVERLAY T_LEFT_PAREN overlay_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<OverlayFunctionExprAST>( $1, $2, $3->asOverlayExpr(), $4 ); }
    | T_POSITION T_LEFT_PAREN position_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<PositionFunctionExprAST>( $1, $2, $3->asPositionExpr(), $4 ); }
    | T_SUBSTRING T_LEFT_PAREN substr_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<SubstringFunctionExprAST>( $1, $2, $3->asSubstringExpr(), $4 ); }
    | T_TREAT T_LEFT_PAREN a_expr T_AS Typename T_RIGHT_PAREN
      { $$ = driver.makeAstNode<TraetFunctionExprAST>( $1, $2, $3->asExpression(), $4, $5->asType(), $6 ); }
    | T_TRIM T_LEFT_PAREN T_BOTH trim_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<TrimFunctionExprAST>( $1, $2, $3, $4->asTrimExpr(), $5 ); }
    | T_TRIM T_LEFT_PAREN T_LEADING trim_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<TrimFunctionExprAST>( $1, $2, $3, $4->asTrimExpr(), $5 ); }
    | T_TRIM T_LEFT_PAREN T_TRAILING trim_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<TrimFunctionExprAST>( $1, $2, $3, $4->asTrimExpr(), $5 ); }
    | T_TRIM T_LEFT_PAREN trim_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<TrimFunctionExprAST>( $1, $2,  0, $3->asTrimExpr(), $4 ); }
    | T_NULLIF T_LEFT_PAREN a_expr T_COMMA a_expr T_RIGHT_PAREN
      { $$ = driver.makeAstNode<NullifFunctionExprAST>( $1, $2, $3->asExpression(), $4, $5->asExpression(), $6 ); }
    | T_COALESCE T_LEFT_PAREN expr_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<CoalesceFunctionExprAST>( $1, $2, $3, $4 ); }
    | T_GREATEST T_LEFT_PAREN expr_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<GreatestFunctionExprAST>( $1, $2, $3, $4 ); }
    | T_LEAST T_LEFT_PAREN expr_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<LeastFunctionExprAST>( $1, $2, $3, $4 ); }
    | T_XMLCONCAT T_LEFT_PAREN expr_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<XmlconcatFunctionExprAST>( $1, $2, $3, $4 ); }
    | T_XMLELEMENT T_LEFT_PAREN T_NAME ColLabel T_RIGHT_PAREN
      {
        $$ = driver.makeAstNode<XmlelementFunctionExprAST>( $1, $2, $3,
                                                            driver.createName(NameAST::Unknown, $4),
                                                            0, (XmlAttributesAST *) 0,
                                                            0, (ExpressionListAST*) 0,
                                                            $4 );
      }
    | T_XMLELEMENT T_LEFT_PAREN T_NAME ColLabel T_COMMA xml_attributes T_RIGHT_PAREN
      {
        $$ = driver.makeAstNode<XmlelementFunctionExprAST>( $1, $2, $3,
                                                            driver.createName(NameAST::Unknown, $4),
                                                            $5, $6->asXmlAttributes(),
                                                            0, (ExpressionListAST*) 0,
                                                            $7 );
      }
    | T_XMLELEMENT T_LEFT_PAREN T_NAME ColLabel T_COMMA expr_list T_RIGHT_PAREN
      {
        $$ = driver.makeAstNode<XmlelementFunctionExprAST>( $1, $2, $3,
                                                            driver.createName(NameAST::Unknown, $4),
                                                            0, (XmlAttributesAST *) 0,
                                                            $5, $6,
                                                            $7 );
      }
    | T_XMLELEMENT T_LEFT_PAREN T_NAME ColLabel T_COMMA xml_attributes T_COMMA expr_list T_RIGHT_PAREN
      {
        $$ = driver.makeAstNode<XmlelementFunctionExprAST>( $1, $2, $3,
                                                            driver.createName(NameAST::Unknown, $4),
                                                            $5, $6->asXmlAttributes(),
                                                            $7, $8,
                                                            $9 );
      }
    | T_XMLEXISTS T_LEFT_PAREN c_expr xmlexists_argument T_RIGHT_PAREN
      { $$ = driver.makeAstNode<XmlexistsFunctionExprAST>( $1, $2, $3->asExpression(), $4->asXmlExistsArgument(), $5 ); }
    | T_XMLFOREST T_LEFT_PAREN xml_attribute_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<XmlforestFunctionExprAST>( $1, $2, $3, $4 ); }
    | T_XMLPARSE T_LEFT_PAREN document_or_content a_expr xml_whitespace_option T_RIGHT_PAREN
      { $$ = driver.makeAstNode<XmlparseFunctionExprAST>( $1, $2, $3, $4->asExpression(), $5->asXmlWhitespaceOption(), $6 ); }
    | T_XMLPI T_LEFT_PAREN T_NAME ColLabel T_RIGHT_PAREN
      { $$ = driver.makeAstNode<XmlpiFunctionExprAST>( $1, $2, $3, driver.createName(NameAST::Unknown, $4),  0, (ExpressionAST *) 0, $5 ); }
    | T_XMLPI T_LEFT_PAREN T_NAME ColLabel T_COMMA a_expr T_RIGHT_PAREN
      { $$ = driver.makeAstNode<XmlpiFunctionExprAST>( $1, $2, $3, driver.createName(NameAST::Unknown, $4), $5, $6->asExpression(), $7 ); }
    | T_XMLROOT T_LEFT_PAREN a_expr T_COMMA xml_root_version opt_xml_root_standalone T_RIGHT_PAREN
      { $$ = driver.makeAstNode<XmlrootFunctionExprAST>( $1, $2, $3->asExpression(), $4, $5->asXmlRootVersion(), $6->asXmlRootStandalone(), $7 ); }
    | T_XMLSERIALIZE T_LEFT_PAREN document_or_content a_expr T_AS SimpleTypename T_RIGHT_PAREN
      { $$ = driver.makeAstNode<XmlserializeFunctionExprAST>( $1, $2, $3, $4->asExpression(), $5, $6->asSimpleTypename(), $7 ); }
    ;

into_clause:
      T_INTO OptTempTableName
      { $$ = driver.makeAstNode<IntoClauseAST>( $1, $2->asOptTempTableName() ); }
    | empty_aka_optional
    ;

opt_table:
      T_TABLE
    | /*EMPTY*/ { $$ = 0; }
    ;

OptTempTableName:
      T_TEMPORARY opt_table table_identifier
      { $$ = driver.makeAstNode<OptTempTableNameAST>(  0, $1, $2, $3->asTableIdentifier() ); }
    | T_TEMP opt_table table_identifier
      { $$ = driver.makeAstNode<OptTempTableNameAST>(  0, $1, $2, $3->asTableIdentifier() ); }
    | T_LOCAL T_TEMPORARY opt_table table_identifier
      { $$ = driver.makeAstNode<OptTempTableNameAST>( $1, $2, $3, $4->asTableIdentifier() ); }
    | T_LOCAL T_TEMP opt_table table_identifier
      { $$ = driver.makeAstNode<OptTempTableNameAST>( $1, $2, $3, $4->asTableIdentifier() ); }
    | T_GLOBAL T_TEMPORARY opt_table table_identifier
      { $$ = driver.makeAstNode<OptTempTableNameAST>( $1, $2, $3, $4->asTableIdentifier() ); }
    | T_GLOBAL T_TEMP opt_table table_identifier
      { $$ = driver.makeAstNode<OptTempTableNameAST>( $1, $2, $3, $4->asTableIdentifier() ); }
    | T_UNLOGGED opt_table table_identifier
      { $$ = driver.makeAstNode<OptTempTableNameAST>( $1,  0, $2, $3->asTableIdentifier() ); }
    | T_TABLE table_identifier
      { $$ = driver.makeAstNode<OptTempTableNameAST>(  0,  0, $1, $2->asTableIdentifier() ); }
    | table_identifier
      { $$ = driver.makeAstNode<OptTempTableNameAST>(  0,  0,  0, $1->asTableIdentifier() ); }
    ;

with_clause:
      T_WITH cte_list
      { $$ = driver.makeAstNode<WithClauseAST>( $1,  0, $2 ); }
    | T_WITH T_RECURSIVE cte_list
      { $$ = driver.makeAstNode<WithClauseAST>( $1, $2, $3 ); }
    ;

cte_list:
      common_table_expr
      { $$ = driver.makeAstNode<CommonTableExprListAST>( $1->asCommonTableExpr() ); }
    | cte_list T_COMMA common_table_expr
      { $$ = driver.makeAstNode<CommonTableExprListAST>( $1, $3->asCommonTableExpr() ); }
    ;

common_table_expr:
      name opt_name_list T_AS T_LEFT_PAREN PreparableStmt T_RIGHT_PAREN
      {
        $$ = driver.makeAstNode<CommonTableExprAST>( driver.createName(NameAST::Column, $1),
                                                     $2->asCompoundColumnNames(),
                                                     $3,
                                                     $4,
                                                     $5->asDmlStatement(),
                                                     $6 );
      }
    ;

name:
      ColId
    ;

name_list:
      name
      { $$ = driver.makeAstNode<NameListAST>( driver.createName(NameAST::Column, $1) ); }
    | name_list T_COMMA name
      { $$ = driver.makeAstNode<NameListAST>( $1, driver.createName(NameAST::Column, $3) ); }
    ;

schema_name_list:
      name
      { $$ = driver.makeAstNode<NameListAST>( driver.createName(NameAST::Schema, $1) ); }
    | schema_name_list T_COMMA name
      { $$ = driver.makeAstNode<NameListAST>( $1, driver.createName(NameAST::Schema, $3) ); }
    ;

opt_name_list:
      T_LEFT_PAREN name_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<CompoundColumnNamesAST>( $1, $2, $3 ); }
    | empty_aka_optional
    ;

PreparableStmt:
      SelectStmt
    | InsertStmt
    | UpdateStmt
    | DeleteStmt
    ;

select_limit:
      limit_clause offset_clause
      { $$ = driver.makeAstNode<LimitOffsetClauseAST>( $1->asLimit(), $2->asOffset() ); }
    | offset_clause limit_clause
      { $$ = driver.makeAstNode<OffsetLimitClauseAST>( $1->asOffset(), $2->asLimit() ); }
    | limit_clause
      { $$ = driver.makeAstNode<LimitOffsetClauseAST>( $1->asLimit(), (OffsetAST *) 0 ); }
    | offset_clause
      { $$ = driver.makeAstNode<OffsetLimitClauseAST>( $1->asOffset(), (LimitAST *) 0 ); }
    ;

opt_select_limit:
      select_limit
    | empty_aka_optional
    ;

limit_clause:
      T_LIMIT select_limit_value
      { $$ = driver.makeAstNode<CommonLimitAST>( $1, $2->asLimitValue(),  0, (ExpressionAST *) 0 ); }
    | T_LIMIT select_limit_value T_COMMA select_offset_value
      { $$ = driver.makeAstNode<CommonLimitAST>( $1, $2->asLimitValue(), $3, $4->asExpression() ); }
    /* SQL:2008 syntax */
    | T_FETCH first_or_next opt_select_fetch_first_value row_or_rows T_ONLY
      { $$ = driver.makeAstNode<FetchLimitAST>( $1, $2, $3->asFetchFirstValue(), $4, $5 ); }
    ;

offset_clause:
      T_OFFSET select_offset_value
      { $$ = driver.makeAstNode<OffsetAST>( $1, $2->asExpression(),  0 ); }
    /* SQL:2008 syntax */
    | T_OFFSET select_offset_value2 row_or_rows
      { $$ = driver.makeAstNode<OffsetAST>( $1, $2->asExpression(), $3 ); }
    ;

opt_select_fetch_first_value:
      SignedIconst
      { $$ = driver.makeAstNode<ConstFetchFirstValueAST>( $1 ); }
    | T_LEFT_PAREN a_expr T_RIGHT_PAREN
      { $$ = driver.makeAstNode<CompoundFetchFirstValueAST>( $1, $2->asExpression(), $3 ); }
    | empty_aka_optional
    ;

select_limit_value:
      a_expr
      { $$ = driver.makeAstNode<ExprLimitValueAST>( $1->asExpression() ); }
    | T_ALL
      { $$ = driver.makeAstNode<AllLimitValueAST>( $1 ); }
    ;

select_offset_value:
      a_expr
    ;

select_offset_value2:
      c_expr
    ;

row_or_rows:
      T_ROW
    | T_ROWS
    ;

first_or_next:
      T_FIRST
    | T_NEXT
    ;

for_locking_clause:
      for_locking_items
      { $$ = driver.makeAstNode<CommonForLockingClauseAST>( $1 ); }
    | T_FOR T_READ T_ONLY
      { $$ = driver.makeAstNode<ReadOnlyForLockingClauseAST>( $1, $2, $3 ); }
    ;

opt_for_locking_clause:
      for_locking_clause
    | empty_aka_optional
    ;

for_locking_items:
      for_locking_item
      { $$ = driver.makeAstNode<LockingExprListAST>( $1->asLockingExpr() ); }
    | for_locking_items for_locking_item
      { $$ = driver.makeAstNode<LockingExprListAST>( $1, $2->asLockingExpr() ); }
    ;

for_locking_item:
      for_locking_strength locked_rels_list opt_nowait
      { $$ = driver.makeAstNode<LockingExprAST>( $1->asLockingStrength(), $2->asLockingRels(), $3 ); }
    ;

opt_nowait:
      T_NOWAIT
    | /*EMPTY*/ { $$ = 0; }
    ;

for_locking_strength:
      T_FOR T_UPDATE            { $$ = driver.makeAstNode<LockingStrengthAST>( $1, $2,  0,  0 ); }
    | T_FOR T_NO T_KEY T_UPDATE { $$ = driver.makeAstNode<LockingStrengthAST>( $1, $2, $3, $4 ); }
    | T_FOR T_SHARE             { $$ = driver.makeAstNode<LockingStrengthAST>( $1, $2,  0,  0 ); }
    | T_FOR T_KEY T_SHARE       { $$ = driver.makeAstNode<LockingStrengthAST>( $1, $2, $3,  0 ); }
    ;

locked_rels_list:
      T_OF table_identifier_list
      { $$ = driver.makeAstNode<LockingRelsAST>( $1, $2 ); }
    | empty_aka_optional
    ;

target_list:
      target_el
      { $$ = driver.makeAstNode<SelectTargetListAST>( $1->asSelectTarget() ); }
    | target_list T_COMMA target_el
      { $$ = driver.makeAstNode<SelectTargetListAST>( $1, $3->asSelectTarget() ); }
    ;

target_el:
      a_expr T_AS ColLabel
      {
        $$ = driver.makeAstNode<ExprSelectTargetAST>( $1->asExpression(),
                                                      $2,
                                                      driver.createName(NameAST::ColumnAlias, $3) );
      }
    | a_expr T_IDENTIFIER
      {
        $$ = driver.makeAstNode<ExprSelectTargetAST>( $1->asExpression(),
                                                      0,
                                                      driver.createName(NameAST::ColumnAlias, $2) );
      }
    | a_expr
      {
        $$ = driver.makeAstNode<ExprSelectTargetAST>( $1->asExpression(),
                                                      0,
                                                      (NameAST *) 0 );
      }
    | T_STAR
      { $$ = driver.makeAstNode<StarSelectTargetAST>( $1 ); }
    ;

values_clause:
      T_VALUES ctext_row
      { $$ = driver.makeAstNode<ValuesClauseAST>( $1, $2->asColumnExprRow() ); }
    ;

ctext_expr:
      a_expr
      { $$ = driver.makeAstNode<ExprColumnExpressionAST>( $1->asExpression() ); }
    | T_DEFAULT
      { $$ = driver.makeAstNode<DefaultColumnExpressionAST>( $1 ); }
    ;

ctext_expr_list:
      ctext_expr
      { $$ = driver.makeAstNode<ColumnExpressionListAST>( $1->asColumnExpression() ); }
    | ctext_expr_list T_COMMA ctext_expr
      { $$ = driver.makeAstNode<ColumnExpressionListAST>( $1, $3->asColumnExpression() ); }
    ;

ctext_row:
      T_LEFT_PAREN ctext_expr_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<ColumnExprRowAST>( $1, $2, $3 ); }
    ;

sort_clause:
      T_ORDER T_BY sortby_list
      { $$ = driver.makeAstNode<SortClauseAST>( $1, $2, $3 ); }
    ;

opt_sort_clause:
      sort_clause
    | empty_aka_optional
    ;

sortby_list:
      sortby
      { $$ = driver.makeAstNode<OrderExprListAST>( $1->asOrderExpr() ); }
    | sortby_list T_COMMA sortby
      { $$ = driver.makeAstNode<OrderExprListAST>( $1, $3->asOrderExpr() ); }
    ;

sortby:
      a_expr T_USING qual_all_Op opt_nulls_order
      { $$ = driver.makeAstNode<UsingOrderExprAST>( $1->asExpression(), $2, $3->asQualifiedOperator(), $4 ); }
    | a_expr opt_asc_desc opt_nulls_order
      { $$ = driver.makeAstNode<CommonOrderExprAST>( $1->asExpression(), $2, $3 ); }
    ;

opt_nulls_order:
      T_NULLS_FIRST
    | T_NULLS_LAST
    | /*EMPTY*/ { $$ = 0; }
    ;

opt_asc_desc:
      T_ASC
    | T_DESC
    | /*EMPTY*/ { $$ = 0; }
    ;

opt_distinct:
      T_DISTINCT
      { $$ = driver.makeAstNode<SimpleDistinctClauseAST>( $1 ); }
    | T_DISTINCT T_ON T_LEFT_PAREN expr_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<ExprListDistinctClauseAST>( $1, $2, $3, $4, $5 ); }
    | T_ALL
      { $$ = driver.makeAstNode<SimpleDistinctClauseAST>( $1 ); }
    | empty_aka_optional
    ;

opt_all:
      T_ALL
    | T_DISTINCT
    | /*EMPTY*/   { $$ = 0; }
    ;

/*****************************************************************************
 *	Type syntax
 *		SQL introduces a large amount of type-specific syntax.
 *		Define individual clauses to handle these cases, and use
 *		 the generic case to handle regular type-extensible Postgres syntax.
 *		- thomas 1997-10-10
 *****************************************************************************/

Typename:
      SimpleTypename opt_array_bounds
      { $$ = driver.makeAstNode<TypeAST>(  0, $1->asSimpleTypename(),  0, $2); }
    | T_SETOF SimpleTypename opt_array_bounds
      { $$ = driver.makeAstNode<TypeAST>( $1, $2->asSimpleTypename(),  0, $3); }
    /* SQL standard syntax, currently only one-dimensional */
    | SimpleTypename T_ARRAY T_LEFT_BRACKET Iconst T_RIGHT_BRACKET
      {
        $$ = driver.makeAstNode<TypeAST>( 0,
                                          $1->asSimpleTypename(),
                                          $2,
                                          driver.makeAstNode<ArrBoundListAST>( driver.makeAstNode<ArrBoundAST>($3, $4, $5) ) );
      }
    | T_SETOF SimpleTypename T_ARRAY T_LEFT_BRACKET Iconst T_RIGHT_BRACKET
      {
        $$ = driver.makeAstNode<TypeAST>( $1,
                                          $2->asSimpleTypename(),
                                          $3,
                                          driver.makeAstNode<ArrBoundListAST>( driver.makeAstNode<ArrBoundAST>($4, $5, $6) ) );
      }
    | SimpleTypename T_ARRAY
      { $$ = driver.makeAstNode<TypeAST>(  0, $1->asSimpleTypename(), $2, (ArrBoundListAST *) 0 ); }
    | T_SETOF SimpleTypename T_ARRAY
      { $$ = driver.makeAstNode<TypeAST>( $1, $2->asSimpleTypename(), $3, (ArrBoundListAST *) 0 ); }
    ;

opt_array_bounds:
      opt_array_bounds T_LEFT_BRACKET T_RIGHT_BRACKET
      { $$ = driver.makeAstNode<ArrBoundListAST>( $1, driver.makeAstNode<ArrBoundAST>( $2,  0, $3 ) ); }
    | opt_array_bounds T_LEFT_BRACKET Iconst T_RIGHT_BRACKET
      { $$ = driver.makeAstNode<ArrBoundListAST>( $1, driver.makeAstNode<ArrBoundAST>( $2, $3, $4 ) ); }
    | /*EMPTY*/ { $$ = 0; }
    ;

ConstTypename:
      Numeric
    | Bit
    | Character
    | ConstDatetime
    ;

SimpleTypename:
      GenericType
    | Numeric
    | Bit
    | Character
    | ConstDatetime
    | ConstInterval opt_interval
      { $$ = driver.makeAstNode<IntervalTypenameAST>( $1,  0,  0,  0, $2->asInterval() ); }
    | ConstInterval T_LEFT_PAREN Iconst T_RIGHT_PAREN opt_interval
      { $$ = driver.makeAstNode<IntervalTypenameAST>( $1, $2, $3, $4, $5->asInterval() ); }
    ;

opt_type_modifiers:
      T_LEFT_PAREN expr_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<TypeModifiersAST>( $1, (ExpressionListAST *) 0, $3); }
    | empty_aka_optional
    ;

GenericType:
      type_function_name opt_type_modifiers
      {
        $$ = driver.makeAstNode<GenericTypenameAST>( driver.createName(NameAST::Type, $1),
                                                     (NameListAST *) 0,
                                                     $2->asTypeModifiers() );
      }
    | type_function_name attrs opt_type_modifiers
      {
        $$ = driver.makeAstNode<GenericTypenameAST>( driver.createName(NameAST::Type, $1),
                                                     $2,
                                                     $3->asTypeModifiers() );
      }
    ;

opt_float:
      T_LEFT_PAREN Iconst T_RIGHT_PAREN
      { $$ = driver.makeAstNode<FloatPrecisionAST>( $1, $2, $3 ); }
    | empty_aka_optional
    ;

Numeric:
      T_INT                         { $$ = driver.makeAstNode<NumericTypenameAST>( $1 ); }
    | T_INT2                        { $$ = driver.makeAstNode<NumericTypenameAST>( $1 ); }
    | T_INT4                        { $$ = driver.makeAstNode<NumericTypenameAST>( $1 ); }
    | T_INT8                        { $$ = driver.makeAstNode<NumericTypenameAST>( $1 ); }
    | T_SERIAL                      { $$ = driver.makeAstNode<NumericTypenameAST>( $1 ); }
    | T_SERIAL2                     { $$ = driver.makeAstNode<NumericTypenameAST>( $1 ); }
    | T_SERIAL4                     { $$ = driver.makeAstNode<NumericTypenameAST>( $1 ); }
    | T_SERIAL8                     { $$ = driver.makeAstNode<NumericTypenameAST>( $1 ); }
    | T_SMALLINT                    { $$ = driver.makeAstNode<NumericTypenameAST>( $1 ); }
    | T_SMALLSERIAL                 { $$ = driver.makeAstNode<NumericTypenameAST>( $1 ); }
    | T_BIGINT                      { $$ = driver.makeAstNode<NumericTypenameAST>( $1 ); }
    | T_BIGSERIAL                   { $$ = driver.makeAstNode<NumericTypenameAST>( $1 ); }
    | T_REAL                        { $$ = driver.makeAstNode<NumericTypenameAST>( $1 ); }
    | T_FLOAT4                      { $$ = driver.makeAstNode<NumericTypenameAST>( $1 ); }
    | T_FLOAT8                      { $$ = driver.makeAstNode<NumericTypenameAST>( $1 ); }
    | T_FLOAT opt_float             { $$ = driver.makeAstNode<FloatTypenameAST>( $1, $2->asFloatPrecision() ); }
    | T_DOUBLE T_PRECISION          { $$ = driver.makeAstNode<DoubleTypenameAST>( $1, $2 ); }
    | T_DECIMAL opt_type_modifiers  { $$ = driver.makeAstNode<DecimalTypenameAST>( $1, $2->asTypeModifiers() ); }
    | T_DEC opt_type_modifiers      { $$ = driver.makeAstNode<DecimalTypenameAST>( $1, $2->asTypeModifiers() ); }
    | T_NUMERIC opt_type_modifiers  { $$ = driver.makeAstNode<DecimalTypenameAST>( $1, $2->asTypeModifiers() ); }
    | T_BOOL                        { $$ = driver.makeAstNode<NumericTypenameAST>( $1 ); }
    | T_BOOLEAN                     { $$ = driver.makeAstNode<NumericTypenameAST>( $1 ); }
    ;

Bit:
      BitWithLength
    | BitWithoutLength
    ;

BitWithLength:
      T_BIT opt_varying T_LEFT_PAREN expr_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<BitTypenameAST>( $1, $2, $3, $4, $5); }
    | T_VARBIT T_LEFT_PAREN expr_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<BitTypenameAST>( $1,  0, $2, $3, $4); }
    ;

BitWithoutLength:
      T_BIT opt_varying
      { $$ = driver.makeAstNode<BitTypenameAST>( $1, $2,  0, (ExpressionListAST *) 0,  0); }
    | T_VARBIT
      { $$ = driver.makeAstNode<BitTypenameAST>( $1,  0,  0, (ExpressionListAST *) 0,  0); }
    ;

Character:
      CharacterWithLength
    | CharacterWithoutLength
    ;

CharacterWithLength:
      character T_LEFT_PAREN Iconst T_RIGHT_PAREN opt_charset
      { $$ = driver.makeAstNode<CharacterTypenameAST>( $1->asCharacterTypeSpec(), $2, $3, $4, $5->asCharSet() ); }
    ;

CharacterWithoutLength:
      character opt_charset
      { $$ = driver.makeAstNode<CharacterTypenameAST>( $1->asCharacterTypeSpec(),  0,  0,  0, $2->asCharSet() ); }
    ;

character:
      T_CHARACTER opt_varying             { $$ = driver.makeAstNode<CharacterTypeSpecAST>(  0, $1, $2 ); }
    | T_CHAR opt_varying                  { $$ = driver.makeAstNode<CharacterTypeSpecAST>(  0, $1, $2 ); }
    | T_VARCHAR                           { $$ = driver.makeAstNode<CharacterTypeSpecAST>(  0, $1,  0 ); }
    | T_NATIONAL T_CHARACTER opt_varying  { $$ = driver.makeAstNode<CharacterTypeSpecAST>( $1, $2, $3 ); }
    | T_NATIONAL T_CHAR opt_varying       { $$ = driver.makeAstNode<CharacterTypeSpecAST>( $1, $2, $3 ); }
    | T_NCHAR opt_varying                 { $$ = driver.makeAstNode<CharacterTypeSpecAST>(  0, $1, $2 ); }
    ;

opt_charset:
      T_CHARACTER T_SET ColId   { $$ = driver.makeAstNode<CharSetAST>( $1, $2, $3 ); }
    | empty_aka_optional
    ;

opt_varying:
      T_VARYING
    | /*EMPTY*/ { $$ = 0; }
    ;

/*****************************************************************************
 * Expressions
 *****************************************************************************/

expr_list:
      a_expr
      { $$ = driver.makeAstNode<ExpressionListAST>( $1->asExpression() ); }
    | expr_list T_COMMA a_expr
      { $$ = driver.makeAstNode<ExpressionListAST>( $1, $3->asExpression() ); }
    ;

columnref:
      ColId
      { $$ = driver.makeAstNode<ColumnRefAST>( driver.createName(NameAST::Unknown, $1), (IndirectionListAST *) 0 ); }
    | ColId indirection
      { $$ = driver.makeAstNode<ColumnRefAST>( driver.createName(NameAST::Unknown, $1), $2 ); }
    ;

indirection_el:
      T_DOT attr_name
      { $$ = driver.makeAstNode<AttirbuteIndirectionAST>( $1, driver.createName(NameAST::Unknown, $2) ); }
    | T_DOT T_STAR
      { $$ = driver.makeAstNode<StarIndirectionAST>( $1, $2 ); }
    | T_LEFT_BRACKET a_expr T_RIGHT_BRACKET
      { $$ = driver.makeAstNode<IndexingIndirectionAST>( $1, $2->asExpression(),  0, (ExpressionAST *) 0, $3 ); }
    | T_LEFT_BRACKET a_expr T_COLON a_expr T_RIGHT_BRACKET
      { $$ = driver.makeAstNode<IndexingIndirectionAST>( $1, $2->asExpression(), $3, $4->asExpression(), $5 ); }
    ;

indirection:
      indirection_el
      { $$ = driver.makeAstNode<IndirectionListAST>( $1->asIndirection() ); }
    | indirection indirection_el
      { $$ = driver.makeAstNode<IndirectionListAST>( $1, $2->asIndirection() ); }
    ;

opt_indirection:
      opt_indirection indirection_el
    | /*EMPTY*/ { $$ = 0; }
    ;

c_expr:
      columnref
      { $$ = driver.makeAstNode<ColRefPrimaryExpressionAST>( $1->asColumnRef() ); }
    | AexprConst
      { $$ = driver.makeAstNode<ConstPrimaryExpressionAST>( $1->asConstant() ); }
    | T_PARAM opt_indirection
      { $$ = driver.makeAstNode<ParamPrimaryExpressionAST>( driver.createName(NameAST::Binding, $1), $2 ); }
    | T_LEFT_PAREN a_expr T_RIGHT_PAREN opt_indirection
      { $$ = driver.makeAstNode<CompoundPrimaryExpressionAST>( $1, $2->asExpression(), $3, $4 ); }
    | case_expr
      { $$ = driver.makeAstNode<CasePrimaryExpressionAST>( $1->asCaseExpr() ); }
    | func_expr
      { $$ = driver.makeAstNode<FunctionPrimaryExpressionAST>( $1->asFunctionExpression() ); }
    | select_with_parens                  %prec UMINUS
      { $$ = driver.makeAstNode<SubqueryPrimaryExpressionAST>(  0, $1->asSelectWithParens(), (IndirectionListAST *) 0 ); }
    | select_with_parens indirection
      { $$ = driver.makeAstNode<SubqueryPrimaryExpressionAST>(  0, $1->asSelectWithParens(), $2 ); }
    | T_EXISTS select_with_parens
      { $$ = driver.makeAstNode<SubqueryPrimaryExpressionAST>($1, $2->asSelectWithParens(), (IndirectionListAST *) 0 ); }
    | T_ARRAY select_with_parens
      { $$ = driver.makeAstNode<SubqueryPrimaryExpressionAST>($1, $2->asSelectWithParens(), (IndirectionListAST *) 0 ); }
    | T_ARRAY array_expr
      { $$ = driver.makeAstNode<ArrayPrimaryExpressionAST>( $1, $2->asArrayExpr() ); }
    | row
      { $$ = driver.makeAstNode<RowPrimaryExpressionAST>( $1->asRow() ); }
    ;

case_expr:
      T_CASE case_arg when_clause_list case_default T_END
      { $$ = driver.makeAstNode<CaseExprAST>( $1, $2->asCaseArg(), $3, $4->asCaseDefaultClause(), $5 ); }
    ;

case_arg:
      a_expr
      { $$ = driver.makeAstNode<CaseArgAST>( $1->asExpression() ); }
    | empty_aka_optional
    ;

when_clause_list:
      when_clause
      { $$ = driver.makeAstNode<CaseWhenClauseListAST>( $1->asCaseWhenClause() ); }
    | when_clause_list when_clause
      { $$ = driver.makeAstNode<CaseWhenClauseListAST>( $1, $2->asCaseWhenClause() ); }
    ;

when_clause:
      T_WHEN a_expr T_THEN a_expr
      { $$ = driver.makeAstNode<CaseWhenClauseAST>( $1, $2->asExpression(), $3, $4->asExpression() ); }
    ;

case_default:
      T_ELSE a_expr
      { $$ = driver.makeAstNode<CaseDefaultClauseAST>( $1, $2->asExpression() ); }
    | empty_aka_optional
    ;

array_expr:
      T_LEFT_BRACKET expr_list T_RIGHT_BRACKET
      { $$ = driver.makeAstNode<ExprListArrayExprAST>( $1, $2, $3 ); }
    | T_LEFT_BRACKET array_expr_list T_RIGHT_BRACKET
      { $$ = driver.makeAstNode<ArrListArrayExprAST>( $1, $2, $3 ); }
    | T_LEFT_BRACKET T_RIGHT_BRACKET
      { $$ = driver.makeAstNode<EmptyArrayExprAST>( $1, $2 ); }
    ;

array_expr_list:
      array_expr
      { $$ = driver.makeAstNode<ArrayExprListAST>( $1->asArrayExpr() ); }
    | array_expr_list T_COMMA array_expr
      { $$ = driver.makeAstNode<ArrayExprListAST>( $1, $3->asArrayExpr() ); }
    ;

AexprConst:
      Iconst
      { $$ = driver.makeAstNode<SimpleConstantAST>( $1 ); }
    | Sconst
      { $$ = driver.makeAstNode<SimpleConstantAST>( $1 ); }
    | func_name Sconst
      { $$ = driver.makeAstNode<FuncNameConstantAST>( $1->asFunctionRef(), 0, (FunctionArgExprListAST *) 0, 0, $2 ); }
    | func_name T_LEFT_PAREN func_arg_list T_RIGHT_PAREN Sconst
      { $$ = driver.makeAstNode<FuncNameConstantAST>( $1->asFunctionRef(), $2, $3, $4, $5 ); }
    | ConstTypename Sconst
      { $$ = driver.makeAstNode<TypenameConstantAST>( $1->asSimpleTypename(), $2 ); }
    | ConstInterval Sconst opt_interval
      { $$ = driver.makeAstNode<IntervalConstantAST>( $1,  0,  0,  0, $2, $3->asInterval() ); }
    | ConstInterval T_LEFT_PAREN Iconst T_RIGHT_PAREN Sconst opt_interval
      { $$ = driver.makeAstNode<IntervalConstantAST>( $1, $2, $3, $4, $5, $6->asInterval() ); }
    | T_TRUE
      { $$ = driver.makeAstNode<SimpleConstantAST>( $1 ); }
    | T_FALSE
      { $$ = driver.makeAstNode<SimpleConstantAST>( $1 ); }
    | T_NULL
      { $$ = driver.makeAstNode<SimpleConstantAST>( $1 ); }
    ;

b_expr:
      c_expr
    | b_expr T_TYPECAST Typename
      { $$ = driver.makeAstNode<TypecastExpressionAST>( $1->asExpression(), $2, $3->asType() ); }
    | T_PLUS b_expr                               %prec UMINUS
      { $$ = driver.makeAstNode<UnaryExpressionAST>( $1, $2->asExpression() ); }
    | T_DASH b_expr                               %prec UMINUS
      { $$ = driver.makeAstNode<UnaryExpressionAST>( $1, $2->asExpression() ); }
    | b_expr T_PLUS b_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | b_expr T_DASH b_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | b_expr T_STAR b_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | b_expr T_SLASH b_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | b_expr T_PERCENT b_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | b_expr T_CARET b_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | b_expr T_LESS b_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | b_expr T_LESS_EQUAL b_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | b_expr T_GREATER b_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | b_expr T_GREATER_EQUAL b_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | b_expr T_EQUAL b_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | b_expr T_EXCLAIM b_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | b_expr T_LESS_GREATER b_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | b_expr T_EXCLAIM_EQUAL b_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | b_expr T_PIPE_PIPE b_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | b_expr qual_Op b_expr                       %prec Op
      { $$ = driver.makeAstNode<CommonBinaryExpressionAST>( $1->asExpression(), $2->asQualifiedOperator(), $3->asExpression() ); }
    | qual_Op b_expr                              %prec Op
      { $$ = driver.makeAstNode<CommonUnaryExpresionAST>( $1->asQualifiedOperator(), $2->asExpression() ); }
    | b_expr qual_Op                              %prec POSTFIXOP
      { $$ = driver.makeAstNode<CommonPostfixExpressionAST>( $1->asExpression(), $2->asQualifiedOperator() ); }
    | b_expr T_IS T_DISTINCT T_FROM b_expr        %prec T_IS
      { $$ = driver.makeAstNode<IsDdistinctExpressionAST>( $1->asExpression(), $2,  0, $3, $4, $5->asExpression() ); }
    | b_expr T_IS T_NOT T_DISTINCT T_FROM b_expr  %prec T_IS
      { $$ = driver.makeAstNode<IsDdistinctExpressionAST>( $1->asExpression(), $2, $3, $4, $5, $6->asExpression() ); }
    | b_expr T_IS T_OF T_LEFT_PAREN type_list T_RIGHT_PAREN         %prec T_IS
      { $$ = driver.makeAstNode<IsTypeofExpressionAST>( $1->asExpression(), $2,  0, $3, $4, $5, $6 ); }
    | b_expr T_IS T_NOT T_OF T_LEFT_PAREN type_list T_RIGHT_PAREN   %prec T_IS
      { $$ = driver.makeAstNode<IsTypeofExpressionAST>( $1->asExpression(), $2, $3, $4, $5, $6, $7 ); }
    | b_expr T_IS T_DOCUMENT                                        %prec T_IS
      { $$ = driver.makeAstNode<IsExpressionAST>( $1->asExpression(), $2,  0, $3 ); }
    | b_expr T_IS T_NOT T_DOCUMENT                                  %prec T_IS
      { $$ = driver.makeAstNode<IsExpressionAST>( $1->asExpression(), $2, $3, $4 ); }
    ;

in_expr:
      select_with_parens
      { $$ = driver.makeAstNode<InValuesAsQueryAST>( $1->asSelectWithParens() ); }
    | T_LEFT_PAREN expr_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<InValuesAsExprListAST>( $1, $2, $3 ); }
    ;

a_expr:
      c_expr
    | a_expr T_TYPECAST Typename
      { $$ = driver.makeAstNode<TypecastExpressionAST>( $1->asExpression(), $2, $3->asType() ); }
    | a_expr T_COLLATE any_name
      { $$ = driver.makeAstNode<CollateExpressionAST>( $1->asExpression(), $2, $3->asAnyIdentifier() ); }
    | a_expr T_AT T_TIME T_ZONE a_expr            %prec T_AT
      { $$ = driver.makeAstNode<AtTimeZoneExpressionAST>( $1->asExpression(), $2, $3, $4, $5->asExpression() ); }
    | T_PLUS a_expr                               %prec UMINUS
      { $$ = driver.makeAstNode<UnaryExpressionAST>( $1, $2->asExpression() ); }
    | T_DASH a_expr                               %prec UMINUS
      { $$ = driver.makeAstNode<UnaryExpressionAST>( $1, $2->asExpression() ); }
    | a_expr T_PLUS a_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | a_expr T_DASH a_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | a_expr T_STAR a_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | a_expr T_SLASH a_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | a_expr T_PERCENT a_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | a_expr T_CARET a_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | a_expr T_LESS a_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | a_expr T_LESS_EQUAL a_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | a_expr T_GREATER a_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | a_expr T_GREATER_EQUAL a_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | a_expr T_EQUAL a_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | a_expr T_EXCLAIM a_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | a_expr T_LESS_GREATER a_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | a_expr T_EXCLAIM_EQUAL a_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | a_expr T_PIPE_PIPE a_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | a_expr qual_Op a_expr                       %prec Op
      { $$ = driver.makeAstNode<CommonBinaryExpressionAST>( $1->asExpression(), $2->asQualifiedOperator(), $3->asExpression() ); }
    | qual_Op a_expr                              %prec Op
      { $$ = driver.makeAstNode<CommonUnaryExpresionAST>( $1->asQualifiedOperator(), $2->asExpression() ); }
    | a_expr qual_Op                              %prec POSTFIXOP
      { $$ = driver.makeAstNode<CommonPostfixExpressionAST>( $1->asExpression(), $2->asQualifiedOperator() ); }
    | a_expr T_AND a_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | a_expr T_OR a_expr
      { $$ = driver.makeAstNode<BinaryExpressionAST>( $1->asExpression(), $2, $3->asExpression() ); }
    | T_NOT a_expr
      { $$ = driver.makeAstNode<UnaryExpressionAST>( $1, $2->asExpression() ); }
    | a_expr T_LIKE a_expr
      { $$ = driver.makeAstNode<LikeExpressionAST>( $1->asExpression(),  0, $2, $3->asExpression(),  0, (ExpressionAST *) 0 ); }
    | a_expr T_LIKE a_expr T_ESCAPE a_expr
      { $$ = driver.makeAstNode<LikeExpressionAST>( $1->asExpression(),  0, $2, $3->asExpression(), $4, $5->asExpression() ); }
    | a_expr T_NOT T_LIKE a_expr
      { $$ = driver.makeAstNode<LikeExpressionAST>( $1->asExpression(), $2, $3, $4->asExpression(),  0, (ExpressionAST *) 0 ); }
    | a_expr T_NOT T_LIKE a_expr T_ESCAPE a_expr
      { $$ = driver.makeAstNode<LikeExpressionAST>( $1->asExpression(), $2, $3, $4->asExpression(), $5, $6->asExpression() ); }
    | a_expr T_ILIKE a_expr
      { $$ = driver.makeAstNode<LikeExpressionAST>( $1->asExpression(),  0, $2, $3->asExpression(),  0, (ExpressionAST *) 0 ); }
    | a_expr T_ILIKE a_expr T_ESCAPE a_expr
      { $$ = driver.makeAstNode<LikeExpressionAST>( $1->asExpression(),  0, $2, $3->asExpression(),  0, (ExpressionAST *) 0 ); }
    | a_expr T_NOT T_ILIKE a_expr
      { $$ = driver.makeAstNode<LikeExpressionAST>( $1->asExpression(), $2, $3, $4->asExpression(),  0, (ExpressionAST *) 0 ); }
    | a_expr T_NOT T_ILIKE a_expr T_ESCAPE a_expr
      { $$ = driver.makeAstNode<LikeExpressionAST>( $1->asExpression(), $2, $3, $4->asExpression(), $5, $6->asExpression() ); }
    | a_expr T_SIMILAR T_TO a_expr                %prec T_SIMILAR
      { $$ = driver.makeAstNode<SimilarToExpressionAST>( $1->asExpression(),  0, $2, $3, $4->asExpression(),  0, (ExpressionAST *) 0 ); }
    | a_expr T_SIMILAR T_TO a_expr T_ESCAPE a_expr
      { $$ = driver.makeAstNode<SimilarToExpressionAST>( $1->asExpression(),  0, $2, $3, $4->asExpression(),  $5, $6->asExpression() ); }
    | a_expr T_NOT T_SIMILAR T_TO a_expr          %prec T_SIMILAR
      { $$ = driver.makeAstNode<SimilarToExpressionAST>( $1->asExpression(), $2, $3, $4, $5->asExpression(),  0, (ExpressionAST *) 0 ); }
    | a_expr T_NOT T_SIMILAR T_TO a_expr T_ESCAPE a_expr
      { $$ = driver.makeAstNode<SimilarToExpressionAST>( $1->asExpression(), $2, $3, $4, $5->asExpression(),  $6, $7->asExpression() ); }
    | a_expr T_IS T_NULL                          %prec T_IS
      { $$ = driver.makeAstNode<IsExpressionAST>( $1->asExpression(), $2,  0, $3 ); }
    | a_expr T_ISNULL
      { $$ = driver.makeAstNode<IsExpressionAST>( $1->asExpression(), $2,  0, $2 ); }
    | a_expr T_IS T_NOT T_NULL                    %prec T_IS
      { $$ = driver.makeAstNode<IsExpressionAST>( $1->asExpression(), $2, $3, $4 ); }
    | a_expr T_NOTNULL
      { $$ = driver.makeAstNode<IsExpressionAST>( $1->asExpression(), $2, $2, $2 ); }
    | row T_OVERLAPS row
      { $$ = driver.makeAstNode<RowOverlapsExpressionAST>( $1->asRow(), $2, $3->asRow() ); }
    | a_expr T_IS T_TRUE                          %prec T_IS
      { $$ = driver.makeAstNode<IsExpressionAST>( $1->asExpression(),  0, $2, $3 ); }
    | a_expr T_IS T_NOT T_TRUE                    %prec T_IS
      { $$ = driver.makeAstNode<IsExpressionAST>( $1->asExpression(), $2, $3, $4 ); }
    | a_expr T_IS T_FALSE                         %prec T_IS
      { $$ = driver.makeAstNode<IsExpressionAST>( $1->asExpression(),  0, $2, $3 ); }
    | a_expr T_IS T_NOT T_FALSE                   %prec T_IS
      { $$ = driver.makeAstNode<IsExpressionAST>( $1->asExpression(), $2, $3, $4 ); }
    | a_expr T_IS T_UNKNOWN                       %prec T_IS
      { $$ = driver.makeAstNode<IsExpressionAST>( $1->asExpression(),  0, $2, $3 ); }
    | a_expr T_IS T_NOT T_UNKNOWN                 %prec T_IS
      { $$ = driver.makeAstNode<IsExpressionAST>( $1->asExpression(), $2, $3, $4 ); }
    | a_expr T_IS T_DISTINCT T_FROM a_expr        %prec T_IS
      { $$ = driver.makeAstNode<IsDdistinctExpressionAST>( $1->asExpression(), $2,  0, $3, $4, $5->asExpression() ); }
    | a_expr T_IS T_NOT T_DISTINCT T_FROM a_expr  %prec T_IS
      { $$ = driver.makeAstNode<IsDdistinctExpressionAST>( $1->asExpression(), $2, $3, $4, $5, $6->asExpression() ); }
    | a_expr T_IS T_OF T_LEFT_PAREN type_list T_RIGHT_PAREN         %prec T_IS
      { $$ = driver.makeAstNode<IsTypeofExpressionAST>( $1->asExpression(), $2,  0, $3, $4, $5, $6 ); }
    | a_expr T_IS T_NOT T_OF T_LEFT_PAREN type_list T_RIGHT_PAREN   %prec T_IS
      { $$ = driver.makeAstNode<IsTypeofExpressionAST>( $1->asExpression(), $2, $3, $4, $5, $6, $7 ); }
    | a_expr T_BETWEEN opt_asymmetric b_expr T_AND b_expr           %prec T_BETWEEN
      { $$ = driver.makeAstNode<BetweenExpressionAST>( $1->asExpression(),  0, $2, $3, $4->asExpression(), $5, $6->asExpression() ); }
    | a_expr T_NOT T_BETWEEN opt_asymmetric b_expr T_AND b_expr     %prec T_BETWEEN
      { $$ = driver.makeAstNode<BetweenExpressionAST>( $1->asExpression(), $2, $3, $4, $5->asExpression(), $6, $7->asExpression() ); }
    | a_expr T_BETWEEN T_SYMMETRIC b_expr T_AND b_expr              %prec T_BETWEEN
      { $$ = driver.makeAstNode<BetweenExpressionAST>( $1->asExpression(),  0, $2, $3, $4->asExpression(), $5, $6->asExpression() ); }
    | a_expr T_NOT T_BETWEEN T_SYMMETRIC b_expr T_AND b_expr        %prec T_BETWEEN
      { $$ = driver.makeAstNode<BetweenExpressionAST>( $1->asExpression(), $2, $3, $4, $5->asExpression(), $6, $7->asExpression() ); }
    | a_expr T_IN in_expr
      { $$ = driver.makeAstNode<InExpressionAST>( $1->asExpression(),  0, $2, $3->asInValues() ); }
    | a_expr T_NOT T_IN in_expr
      { $$ = driver.makeAstNode<InExpressionAST>( $1->asExpression(), $2, $3, $4->asInValues() ); }
    | a_expr subquery_Op sub_type select_with_parens                %prec Op
      { $$ = driver.makeAstNode<SubqueryExpressionAST>( $1->asExpression(), $2->asSubqueryOperator(), $3, $4->asSelectWithParens() ); }
    | a_expr subquery_Op sub_type T_LEFT_PAREN a_expr T_RIGHT_PAREN %prec Op
      { $$ = driver.makeAstNode<SubExpressionExpressionAST>( $1->asExpression(), $2->asSubqueryOperator(), $3, $4, $5->asExpression(), $6 ); }
    | T_UNIQUE select_with_parens
      { $$ = driver.makeAstNode<UniqueExpressionAST>( $1, $2->asSelectWithParens() ); }
    | a_expr T_IS T_DOCUMENT                                        %prec T_IS
      { $$ = driver.makeAstNode<IsExpressionAST>( $1->asExpression(), $2,  0, $3 ); }
    | a_expr T_IS T_NOT T_DOCUMENT                                  %prec T_IS
      { $$ = driver.makeAstNode<IsExpressionAST>( $1->asExpression(), $2, $3, $4 ); }
    ;

sub_type:
      T_ANY
    | T_SOME
    | T_ALL
    ;

all_Op:
      Op
    | MathOp
    ;

MathOp:
      T_PLUS
    | T_DASH
    | T_STAR
    | T_SLASH
    | T_PERCENT
    | T_CARET
    | T_LESS
    | T_LESS_EQUAL
    | T_GREATER
    | T_GREATER_EQUAL
    | T_EQUAL
    | T_EXCLAIM
    | T_EXCLAIM_EQUAL
    | T_LESS_GREATER
    | T_PIPE_PIPE
    ;

qual_Op:
      Op
      { $$ = driver.makeAstNode<SimpleQualifiedOperatorAST>( $1 ); }
    | T_OPERATOR T_LEFT_PAREN any_operator T_RIGHT_PAREN
      { $$ = driver.makeAstNode<CompoundQualifiedOperatorAST>( $1, $2, $3->asOperand(), $4 ); }
    ;

qual_all_Op:
      all_Op
      { $$ = driver.makeAstNode<SimpleQualifiedOperatorAST>( $1 ); }
    | T_OPERATOR T_LEFT_PAREN any_operator T_RIGHT_PAREN
      { $$ = driver.makeAstNode<CompoundQualifiedOperatorAST>( $1, $2, $3->asOperand(), $4 ); }
    ;

subquery_Op:
      all_Op
      { $$ = driver.makeAstNode<QualSubqueryOperatorAST>( driver.makeAstNode<SimpleQualifiedOperatorAST>( $1 ) ); }
    | T_OPERATOR T_LEFT_PAREN any_operator T_RIGHT_PAREN
      { $$ = driver.makeAstNode<QualSubqueryOperatorAST>( driver.makeAstNode<CompoundQualifiedOperatorAST>( $1, $2, $3->asOperand(), $4 ) ); }
    | T_LIKE          { $$ = driver.makeAstNode<LikeSubqueryOperatorAST>(  0, $1 ); }
    | T_NOT T_LIKE    { $$ = driver.makeAstNode<LikeSubqueryOperatorAST>( $1, $2 ); }
    | T_ILIKE         { $$ = driver.makeAstNode<LikeSubqueryOperatorAST>(  0, $1 ); }
    | T_NOT T_ILIKE   { $$ = driver.makeAstNode<LikeSubqueryOperatorAST>( $1, $2 ); }
    ;

any_operator:
      all_Op
      { $$ = driver.makeAstNode<OperandAST>( (ColumnIdentifierAST *)0, 0, $1 ); }
    | ColId T_DOT all_Op
      {
        $$ = driver.makeAstNode<OperandAST>( driver.makeAstNode<ColumnIdentifierAST>( (NameAST *) 0,
                                                                                      0,
                                                                                      (NameAST *) 0,
                                                                                      0,
                                                                                      driver.createName(NameAST::Column, $1) ),
                                             $2,
                                             $3 );
      }
    | ColId T_DOT ColId T_DOT all_Op
      {
        $$ = driver.makeAstNode<OperandAST>( driver.makeAstNode<ColumnIdentifierAST>( (NameAST *) 0,
                                                                                      0,
                                                                                      driver.createName(NameAST::Table, $1),
                                                                                      $2,
                                                                                      driver.createName(NameAST::Column, $3) ),
                                             $4,
                                             $5 );
      }
    | ColId T_DOT ColId T_DOT ColId T_DOT all_Op
      {
        $$ = driver.makeAstNode<OperandAST>( driver.makeAstNode<ColumnIdentifierAST>( driver.createName(NameAST::Schema, $1),
                                                                                      $2,
                                                                                      driver.createName(NameAST::Table, $3),
                                                                                      $4,
                                                                                      driver.createName(NameAST::Column, $5) ),
                                             $6,
                                             $7 );
      }
    ;

/*****************************************************************************
* Second Level Rules
*****************************************************************************/

opt_asymmetric:
      T_ASYMMETRIC
    | /*EMPTY*/ { $$ = 0; }
    ;

type_list:
      Typename
      { $$ = driver.makeAstNode<TypeListAST>( $1->asType() ); }
    | type_list T_COMMA Typename
      { $$ = driver.makeAstNode<TypeListAST>( $1, $3->asType() ); }
    ;

row:
      T_ROW T_LEFT_PAREN expr_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<RowAST>( $1, $2, $3, $4 ); }
    | T_ROW T_LEFT_PAREN T_RIGHT_PAREN
      { $$ = driver.makeAstNode<RowAST>( $1, $2, (ExpressionListAST *) 0, $3 ); }
    | T_LEFT_PAREN expr_list T_COMMA a_expr T_RIGHT_PAREN
      { $$ = driver.makeAstNode<RowAST>(  0, $1, $2, $3 ); }
    ;


alter_table_cmds:
      alter_table_cmd
      { $$ = driver.makeAstNode<AlterTableCmdListAST>( $1->asAlterTableCmd() ); }
    | alter_table_cmds T_COMMA alter_table_cmd
      { $$ = driver.makeAstNode<AlterTableCmdListAST>( $1, $3->asAlterTableCmd() ); }
    | /*EMPTY*/ { $$ = 0; }
    ;

opt_column:
      T_COLUMN
    | /*EMPTY*/ { $$ = 0; }
    ;

opt_drop_behavior:
      T_CASCADE
    | T_RESTRICT
    | /* EMPTY */ { $$ = 0; }
    ;


alter_table_cmd:
      T_ADD columnDef
      {
        $$ = driver.makeAstNode<AlterTableAddColCmdAST>( $1,
                                                         0,
                                                         $2->asColumnDef() );
      }
    | T_ADD T_COLUMN columnDef
      {
        $$ = driver.makeAstNode<AlterTableAddColCmdAST>( $1,
                                                         $2,
                                                         $3->asColumnDef() );
      }
    | T_ALTER opt_column ColId
      {
        driver.appendColumnNameHelper($3, $3);
        $$ = driver.makeAstNode<AlterTableChgDefColCmdAST>( $1, $2, driver.createName(NameAST::Column, $3), (AlterDefaultColumnAST *) 0 );
        driver.appendKeywordsHelper($3 + 1, "SET;RESET;SET DATA TYPE;SET DEFAULT;DROP DEFAULT;SET STORAGE;SET NOT NULL;DROP NOT NULL;SET STATISTICS");
      }
    | T_ALTER opt_column ColId alter_column_default
      {
        driver.appendColumnNameHelper($3, $3);
        $$ = driver.makeAstNode<AlterTableChgDefColCmdAST>( $1, $2, driver.createName(NameAST::Column, $3), $4->asAlterDefaultColumn() );
      }
    | T_ALTER opt_column ColId T_DROP T_NOT T_NULL
      {
        driver.appendColumnNameHelper($3, $3);
        $$ = driver.makeAstNode<AlterTableChgNotNullCmdAST>( $1, $2, driver.createName(NameAST::Column, $3), $4, $5, $6 );
      }
    | T_ALTER opt_column ColId T_SET T_NOT T_NULL
      {
        driver.appendColumnNameHelper($3, $3);
        $$ = driver.makeAstNode<AlterTableChgNotNullCmdAST>( $1, $2, driver.createName(NameAST::Column, $3), $4, $5, $6 );
      }
    | T_ALTER opt_column ColId T_SET T_STATISTICS SignedIconst
      {
        driver.appendColumnNameHelper($3, $3);
        $$ = driver.makeAstNode<AlterTableSetStatCmdAST>( $1, $2, driver.createName(NameAST::Column, $3), $4, $5, $6 );
      }
    | T_ALTER opt_column ColId T_SET reloptions
      {
        driver.appendColumnNameHelper($3, $3);
        $$ = driver.makeAstNode<AlterTableChgReloptCmdAST>( $1, $2, driver.createName(NameAST::Column, $3), $4, $5->asRelOptions() );
      }
    | T_ALTER opt_column ColId T_RESET reloptions
      {
        driver.appendColumnNameHelper($3, $3);
        $$ = driver.makeAstNode<AlterTableChgReloptCmdAST>( $1, $2, driver.createName(NameAST::Column, $3), $4, $5->asRelOptions() );
      }
    | T_ALTER opt_column ColId T_SET T_STORAGE ColId
      {
        driver.appendColumnNameHelper($3, $3);
        $$ = driver.makeAstNode<AlterTableSetStorageCmdAST>( $1, $2, driver.createName(NameAST::Column, $3), $4, $5, driver.createName(NameAST::Unknown, $6) );
      }
    | T_DROP opt_column ColId opt_drop_behavior
      {
        driver.appendColumnNameHelper($3, $3);
        $$ = driver.makeAstNode<AlterTableDropColCmdAST>( $1, $2, 0, 0, driver.createName(NameAST::Column, $3), $4 );
        if ( !$4 )
          driver.appendKeywordsHelper($3, "RESTRICT;CASCADE");
      }
    | T_DROP opt_column T_IF T_EXISTS ColId opt_drop_behavior
      {
        driver.appendColumnNameHelper($5, $5);
        $$ = driver.makeAstNode<AlterTableDropColCmdAST>( $1, $2, $3, $4, driver.createName(NameAST::Column, $5), $6 );
        if ( !$6 )
          driver.appendKeywordsHelper($5, "RESTRICT;CASCADE");
      }
    | T_ALTER opt_column ColId opt_set_data T_TYPE Typename opt_collate_clause alter_using
      {
        driver.appendColumnNameHelper($3, $3);
        $$ = driver.makeAstNode<AlterTableSetTypeCmdAST>( $1,
                                                          $2,
                                                          driver.createName(NameAST::Column, $3),
                                                          $4->asSetData(),
                                                          $5,
                                                          $6->asType(),
                                                          $7->asCollateClause(),
                                                          $8->asUsingClause() );
      }
    | T_ADD TableConstraint
      { $$ = driver.makeAstNode<AlterTableAddConstrCmdAST>( $1, $2->asTableConstraint() ); }
    | T_VALIDATE T_CONSTRAINT name
      {
        driver.appendConstraintNameHelper($3);
        $$ = driver.makeAstNode<AlterTableValConstrCmdAST>( $1, $2, driver.createName(NameAST::Constraint, $3) );
      }
    | T_DROP T_CONSTRAINT T_IF T_EXISTS name opt_drop_behavior
      {
        driver.appendConstraintNameHelper($5);
        $$ = driver.makeAstNode<AlterTableDropConstrCmdAST>( $1, $2, $3, $4, driver.createName(NameAST::Constraint, $5), $6 );
        if ( !$6 )
          driver.appendKeywordsHelper($5, "RESTRICT;CASCADE");
      }
    | T_DROP T_CONSTRAINT name opt_drop_behavior
      {
        driver.appendConstraintNameHelper($3);
        $$ = driver.makeAstNode<AlterTableDropConstrCmdAST>( $1, $2, 0, 0, driver.createName(NameAST::Constraint, $3), $4 );
        if ( !$4 )
          driver.appendKeywordsHelper($3, "RESTRICT;CASCADE");
      }
    | T_SET T_WITH T_OIDS
      { $$ = driver.makeAstNode<AlterTableSetOidsCmdAST>( $1, $2, $3 ); }
    | T_SET T_WITHOUT T_OIDS
      { $$ = driver.makeAstNode<AlterTableSetOidsCmdAST>( $1, $2, $3 ); }
    | T_CLUSTER T_ON name
      { $$ = driver.makeAstNode<AlterTableClusterOnCmdAST>( $1, $2, driver.createName(NameAST::Unknown, $3) ); }
    | T_SET T_WITHOUT T_CLUSTER
      { $$ = driver.makeAstNode<AlterTableClusterOffCmdAST>( $1, $2, $3 ); }
    | T_ENABLE T_TRIGGER name
      {
        driver.appendTriggerNameHelper($3);
        $$ = driver.makeAstNode<AlterTableTriggerCmdAST>( $1,  0, $2, driver.makeAstNode<NameWildcardAST>( driver.createName(NameAST::Trigger, $3) ) );
      }
    | T_ENABLE T_ALWAYS T_TRIGGER name
      {
        driver.appendTriggerNameHelper($4);
        $$ = driver.makeAstNode<AlterTableTriggerCmdAST>( $1, $2, $3, driver.makeAstNode<NameWildcardAST>( driver.createName(NameAST::Trigger, $4) ) );
      }
    | T_ENABLE T_REPLICA T_TRIGGER name
      {
        driver.appendTriggerNameHelper($4);
        $$ = driver.makeAstNode<AlterTableTriggerCmdAST>( $1, $2, $3, driver.makeAstNode<NameWildcardAST>( driver.createName(NameAST::Trigger, $4) ) );
      }
    | T_ENABLE T_TRIGGER T_ALL
      { $$ = driver.makeAstNode<AlterTableTriggerCmdAST>( $1,  0, $2, driver.makeAstNode<AllOrUserWildcardAST>( $3 ) ); }
    | T_ENABLE T_TRIGGER T_USER
      { $$ = driver.makeAstNode<AlterTableTriggerCmdAST>( $1,  0, $2, driver.makeAstNode<AllOrUserWildcardAST>( $3 ) ); }
    | T_DISABLE T_TRIGGER name
      {
        driver.appendTriggerNameHelper($3);
        $$ = driver.makeAstNode<AlterTableTriggerCmdAST>( $1,  0, $2, driver.makeAstNode<NameWildcardAST>( driver.createName(NameAST::Trigger, $3) ) );
      }
    | T_DISABLE T_TRIGGER T_ALL
      { $$ = driver.makeAstNode<AlterTableTriggerCmdAST>( $1,  0, $2, driver.makeAstNode<AllOrUserWildcardAST>( $3 ) ); }
    | T_DISABLE T_TRIGGER T_USER
      { $$ = driver.makeAstNode<AlterTableTriggerCmdAST>( $1,  0, $2, driver.makeAstNode<AllOrUserWildcardAST>( $3 ) ); }
    | T_ENABLE T_RULE name
      { $$ = driver.makeAstNode<AlterTableRuleCmdAST>( $1,  0, $2, driver.createName(NameAST::Unknown, $3) ); }
    | T_ENABLE T_ALWAYS T_RULE name
      { $$ = driver.makeAstNode<AlterTableRuleCmdAST>( $1, $2, $2, driver.createName(NameAST::Unknown, $4) ); }
    | T_ENABLE T_REPLICA T_RULE name
      { $$ = driver.makeAstNode<AlterTableRuleCmdAST>( $1, $2, $2, driver.createName(NameAST::Unknown, $4) ); }
    | T_DISABLE T_RULE name
      { $$ = driver.makeAstNode<AlterTableRuleCmdAST>( $1,  0, $2, driver.createName(NameAST::Unknown, $3) ); }
    | T_INHERIT table_identifier_we
      { $$ = driver.makeAstNode<AlterTableInheritCmdAST>(  0, $1, $2->asTableIdentifier() ); }
    | T_NO T_INHERIT table_identifier_we
      { $$ = driver.makeAstNode<AlterTableInheritCmdAST>( $1, $2, $3->asTableIdentifier() ); }
    | T_OF any_name
      { $$ = driver.makeAstNode<AlterTableOfCmdAST>( $1, $2->asAnyIdentifier() ); }
    | T_NOT T_OF
      { $$ = driver.makeAstNode<AlterTableNotOfCmdAST>( $1, $2 ); }
    | T_OWNER T_TO RoleId
      { $$ = driver.makeAstNode<AlterTableOwnerCmdAST>( $1, $2, $3->asRoleId() ); }
    | T_SET T_TABLESPACE name
      { $$ = driver.makeAstNode<AlterTableTablespaceCmdAST>( $1, $2, driver.createName(NameAST::Unknown, $3) ); }
    | T_SET reloptions
      { $$ = driver.makeAstNode<AlterTableSetReloptCmdAST>( $1, $2->asRelOptions() ); }
    | T_RESET reloptions
      { $$ = driver.makeAstNode<AlterTableSetReloptCmdAST>( $1, $2->asRelOptions() ); }
    | alter_generic_options
    ;

alter_generic_options:
      T_OPTIONS T_LEFT_PAREN alter_generic_option_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<AlterTableGenericCmdAST>( $1, $2, $3, $4 ); }
    ;

alter_generic_option_list:
      alter_generic_option_elem
      { $$ = driver.makeAstNode<AlterGenericOptionListAST>( $1->asAlterGenericOption() ); }
    | alter_generic_option_list T_COMMA alter_generic_option_elem
      { $$ = driver.makeAstNode<AlterGenericOptionListAST>( $1, $3->asAlterGenericOption() ); }
    ;

alter_generic_option_elem:
      generic_option_elem
      { $$ = driver.makeAstNode<SetAddAlterGenericOptionAST>(  0, $1->asGenericOption() ); }
    | T_SET generic_option_elem
      { $$ = driver.makeAstNode<SetAddAlterGenericOptionAST>( $1, $2->asGenericOption() ); }
    | T_ADD generic_option_elem
      { $$ = driver.makeAstNode<SetAddAlterGenericOptionAST>( $1, $2->asGenericOption() ); }
    | T_DROP generic_option_name
      { $$ = driver.makeAstNode<DropAlterGenericOptionAST>( $1, $2 ); }
    ;

TableConstraint:
      T_CONSTRAINT name ConstraintElem
      { $$ = driver.makeAstNode<TableConstraintAST>( $1, driver.createName(NameAST::Constraint, $2), $3->asTableConstraintBody() ); }
    | ConstraintElem
      { $$ = driver.makeAstNode<TableConstraintAST>(  0, (NameAST *) 0, $1->asTableConstraintBody() ); }
    ;

ConstraintElem:
      T_CHECK T_LEFT_PAREN a_expr T_RIGHT_PAREN ConstraintAttributeSpec
      {
        driver.appendColumnNameHelper($2, $4);
        $$ = driver.makeAstNode<CheckTableConstraintAST>( $1, $2, $3->asExpression(), $4, $5 );
      }
    | T_UNIQUE T_LEFT_PAREN columnList T_RIGHT_PAREN opt_definition OptConsTableSpace ConstraintAttributeSpec
      {
        driver.appendColumnNameHelper($2, $4);
        $$ = driver.makeAstNode<UniqueTableConstraintAST>( $1, $2, $3, $4, $5->asConstraintDefinitions(), $6->asConstraintTablespace(), $7);
      }
    | T_UNIQUE ExistingIndex ConstraintAttributeSpec
      { $$ = driver.makeAstNode<IndUniqueTableConstraintAST>( $1, $2->asExistingIndex(), $3 ); }
    | T_PRIMARY T_KEY T_LEFT_PAREN columnList T_RIGHT_PAREN opt_definition OptConsTableSpace ConstraintAttributeSpec
      {
        driver.appendColumnNameHelper($3, $5);
        $$ = driver.makeAstNode<PrimKeyTableConstraintAST>( $1, $2, $3, $4, $5, $6->asConstraintDefinitions(), $7->asConstraintTablespace(), $8 );
      }
    | T_PRIMARY T_KEY ExistingIndex ConstraintAttributeSpec
      { $$ = driver.makeAstNode<IndPrimKeyTableConstraintAST>( $1, $2, $3->asExistingIndex(), $4 ); }
    | T_EXCLUDE access_method_clause T_LEFT_PAREN ExclusionConstraintList T_RIGHT_PAREN opt_definition OptConsTableSpace ExclusionWhereClause ConstraintAttributeSpec
      {
        $$ = driver.makeAstNode<ExcludeTableConstraintAST>( $1,
                                                            $2->asAccessMethodClause(),
                                                            $3,
                                                            $4,
                                                            $5,
                                                            $6->asConstraintDefinitions(),
                                                            $7->asConstraintTablespace(),
                                                            $8->asExclusionWhereClause(),
                                                            $9 );
      }
    | T_FOREIGN T_KEY T_LEFT_PAREN columnList T_RIGHT_PAREN
      {
        driver.appendColumnNameHelper($3, $5);
        $$ = driver.makeAstNode<FkTableConstraintAST>( $1, $2, $3, $4, $5, 0, (TableIdentifierAST *) 0, (RefColumnsAST *) 0, (KeyMatchAST *) 0, (KeyActionsAST *) 0, (ConstraintAttributeListAST *) 0 );
      }
    | T_FOREIGN T_KEY T_LEFT_PAREN columnList T_RIGHT_PAREN T_REFERENCES table_identifier opt_column_list key_match key_actions ConstraintAttributeSpec
      {
        driver.appendColumnNameHelper($3, $5);
        RefColumnsAST *refCols = $8->asRefColumns();
        TableIdentifierAST *refTable = $7->asTableIdentifier();
        if ( refTable ) {
          driver.appendObjectIdentifierHelper(StatementAST::CompletionHelper::TableIdentifier, $6 + 1);
          if ( refCols )
            driver.appendColumnNameHelper(refCols->firstToken(), refCols->lastToken(), refTable->schemaName, refTable->tableName);
        }
        $$ = driver.makeAstNode<FkTableConstraintAST>( $1, $2, $3, $4, $5, $6, refTable, refCols, $9->asKeyMatch(), $10->asKeyActions(), $11 );
      }
    ;

access_method_clause:
      T_USING access_method
      { $$ = driver.makeAstNode<AccessMethodClauseAST>( $1, driver.createName(NameAST::Unknown, $2) ); }
    | empty_aka_optional
    ;

ExistingIndex:
      T_USING T_INDEX index_name
      {
        driver.appendIndexNameHelper($2 + 1);
        $$ = driver.makeAstNode<ExistingIndexAST>( $1, $2, driver.createName(NameAST::Index, $3) );
      }
    ;

ConstraintAttributeSpec:
      /*EMPTY*/ { $$ = 0; }
    | ConstraintAttributeSpec ConstraintAttributeElem
      { $$ = driver.makeAstNode<ConstraintAttributeListAST>( $1, $2->asConstraintAttribute() ); }
    ;

ConstraintAttributeElem:
      T_NOT T_DEFERRABLE
      { $$ = driver.makeAstNode<DeferConstraintAttrAST>( $1, $2 ); }
    | T_DEFERRABLE
      { $$ = driver.makeAstNode<DeferConstraintAttrAST>(  0, $1 ); }
    | T_INITIALLY T_IMMEDIATE
      { $$ = driver.makeAstNode<InitiallyConstraintAttrAST>( $1, $2 ); }
    | T_INITIALLY T_DEFERRED
      { $$ = driver.makeAstNode<InitiallyConstraintAttrAST>( $1, $2 ); }
    | T_NOT T_VALID
      { $$ = driver.makeAstNode<NotValidConstraintAttrAST>( $1, $2 ); }
    | T_NO T_INHERIT
      { $$ = driver.makeAstNode<NoInheritConstraintAttrAST>( $1, $2 ); }
    ;

ExclusionConstraintList:
      ExclusionConstraintElem
      { $$ = driver.makeAstNode<ExclusionConstraintListAST>( $1->asExclusionConstraint() ); }
    | ExclusionConstraintList T_COMMA ExclusionConstraintElem
      { $$ = driver.makeAstNode<ExclusionConstraintListAST>( $1, $3->asExclusionConstraint() ); }
    ;

ExclusionConstraintElem:
      index_elem T_WITH any_operator
      {
        $$ = driver.makeAstNode<ExclusionConstraintAST>( $1->asIndexElement(),
                                                         $2,
                                                         0,
                                                         0,
                                                         $3->asOperand(),
                                                         0 );
      }
    /* allow OPERATOR() decoration for the benefit of ruleutils.c */
    | index_elem T_WITH T_OPERATOR T_LEFT_PAREN any_operator T_RIGHT_PAREN
      {
        $$ = driver.makeAstNode<ExclusionConstraintAST>( $1->asIndexElement(),
                                                         $2,
                                                         $3,
                                                         $4,
                                                         $5->asOperand(),
                                                         $6 );
      }
    ;

ExclusionWhereClause:
      T_WHERE T_LEFT_PAREN a_expr T_RIGHT_PAREN
      { $$ = driver.makeAstNode<ExclusionWhereClauseAST>( $1, $2, $3->asExpression(), $4 ); }
    | empty_aka_optional
    ;

index_elem:
      ColId opt_collate opt_class opt_asc_desc opt_nulls_order
      {
        $$ = driver.makeAstNode<ColIndexElementAST>( driver.createName(NameAST::Column, $1),
                                                    $2->asCollateClause(),
                                                    $3->asClassClause(),
                                                    $4,
                                                    $5 );
      }
    | func_expr opt_collate opt_class opt_asc_desc opt_nulls_order
      {
        $$ = driver.makeAstNode<FuncExprIndexElementAST>( $1->asFunctionExpression(),
                                                          $2->asCollateClause(),
                                                          $3->asClassClause(),
                                                          $4,
                                                          $5 );
      }
    | T_LEFT_PAREN a_expr T_RIGHT_PAREN opt_collate opt_class opt_asc_desc opt_nulls_order
      {
        $$ = driver.makeAstNode<ExprIndexElementAST>( $1,
                                                      $2->asExpression(),
                                                      $3,
                                                      $4->asCollateClause(),
                                                      $5->asClassClause(),
                                                      $6,
                                                      $7 );
      }
    ;

opt_collate:
      T_COLLATE any_name
      { $$ = driver.makeAstNode<CollateClauseAST>( $1, $2->asName() ); }
    | empty_aka_optional
    ;

opt_class:
      any_name
      { $$ = driver.makeAstNode<ClassClauseAST>(  0, $1->asName() ); }
    | T_USING any_name
      { $$ = driver.makeAstNode<ClassClauseAST>( $1, $2->asName() ); }
    | empty_aka_optional
    ;

opt_set_data:
      T_SET T_DATA
      { $$ = driver.makeAstNode<SetDataAST>( $1, $2 ); }
    | empty_aka_optional
    ;

alter_using:
      T_USING a_expr
      { $$ = driver.makeAstNode<UsingClauseAST>( $1, $2->asExpression() ); }
    | empty_aka_optional
    ;

reloptions:
      T_LEFT_PAREN reloption_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<RelOptionsAST>( $1, $2, $3 ); }
    ;

reloption_list:
      reloption_elem
      { $$ = driver.makeAstNode<RelOptionListAST>( $1->asRelOption() ); }
    | reloption_list T_COMMA reloption_elem
      { $$ = driver.makeAstNode<RelOptionListAST>( $1, $3->asRelOption() ); }
    ;

reloption_elem:
      ColLabel T_EQUAL def_arg
      {
        $$ = driver.makeAstNode<RelOptionAST>( (NameAST *) 0,
                                               0,
                                               driver.createName(NameAST::Unknown, $1),
                                               $2,
                                               $3->asDefinitionArgument() );
      }
    | ColLabel
      {
        $$ = driver.makeAstNode<RelOptionAST>( (NameAST *) 0,
                                               0,
                                               driver.createName(NameAST::Unknown, $1),
                                               0,
                                               (DefinitionArgumentAST *) 0 );
      }
    | ColLabel T_DOT ColLabel T_EQUAL def_arg
      {
        $$ = driver.makeAstNode<RelOptionAST>( driver.createName(NameAST::Unknown, $1),
                                               $2,
                                               driver.createName(NameAST::Unknown, $3),
                                               $4,
                                               $5->asDefinitionArgument() );
      }
    | ColLabel T_DOT ColLabel
      {
        $$ = driver.makeAstNode<RelOptionAST>( driver.createName(NameAST::Unknown, $1),
                                               $2,
                                               driver.createName(NameAST::Unknown, $3),
                                               0,
                                               (DefinitionArgumentAST *) 0 );
      }
    ;

alter_column_default:
      T_SET T_DEFAULT a_expr
      { $$ = driver.makeAstNode<SetAlterDefaultColumnAST>( $1, $2, $3->asExpression() ); }
    | T_DROP T_DEFAULT
      { $$ = driver.makeAstNode<DropAlterDefaultColumnAST>( $1, $2 ); }
    ;


attrs:
      T_DOT attr_name
      { $$ = driver.makeAstNode<NameListAST>( driver.createName(NameAST::Unknown, $1) ); }
    | attrs T_DOT attr_name
      { $$ = driver.makeAstNode<NameListAST>( $1, driver.createName(NameAST::Unknown, $3) ); }
    ;

columnDef:
      ColId Typename create_generic_options ColQualList
      {
        $$ = driver.makeAstNode<ColumnDefAST>( driver.createName(NameAST::Column, $1),
                                              $2->asType(),
                                              $3->asCreateGeneticOptions(),
                                              $4 );
      }
    ;

ColQualList:
      ColQualList ColConstraint
      { $$ = driver.makeAstNode<ColumnConstraintListAST>( $1, $2->asColumnConstraint() ); }
    | /*EMPTY*/ { $$ = 0; }
    ;


ColConstraint:
      T_CONSTRAINT name ColConstraintElem
      { $$ = driver.makeAstNode<CommonColConstraintAST>( $1, driver.createName(NameAST::Constraint, $2), $3->asConstraintBody() ); }
    | ColConstraintElem
      { $$ = driver.makeAstNode<CommonColConstraintAST>(  0, (NameAST *) 0, $1->asConstraintBody() ); }
    | ConstraintAttr
    | T_COLLATE any_name
      { $$ = driver.makeAstNode<CollateColConstraintAST>( $1, $2->asAnyIdentifier() ); }
    ;

ColConstraintElem:
      T_NOT T_NULL
      { $$ = driver.makeAstNode<NotNullConstraintBodyAST>( $1, $2 ); }
    |       T_NULL
      { $$ = driver.makeAstNode<NotNullConstraintBodyAST>(  0, $1 ); }
    | T_UNIQUE opt_definition OptConsTableSpace
      { $$ = driver.makeAstNode<UniqueConstraintBodyAST>( $1, $2->asConstraintDefinitions(), $3->asConstraintTablespace() ); }
    | T_PRIMARY T_KEY opt_definition OptConsTableSpace
      { $$ = driver.makeAstNode<PrimaryKeyConstraintBodyAST>( $1, $2, $3->asConstraintDefinitions(), $4->asConstraintTablespace() ); }
    | T_CHECK T_LEFT_PAREN a_expr T_RIGHT_PAREN opt_no_inherit
      {
        driver.appendColumnNameHelper($2, $4);  /*!TODO! columndef*/
        $$ = driver.makeAstNode<CheckConstraintBodyAST>( $1, $2, $3->asExpression(), $4, $5->asNoInherit() );
      }
    | T_DEFAULT b_expr
      { $$ = driver.makeAstNode<DefaultConstraintBodyAST>( $1, $2->asExpression() ); }
    | T_REFERENCES table_identifier opt_column_list key_match key_actions
      {
        TableIdentifierAST *refTable = $2->asTableIdentifier();
        RefColumnsAST *refCols = $3->asRefColumns();
        if ( refTable ) {
          driver.appendObjectIdentifierHelper(StatementAST::CompletionHelper::TableIdentifier, $1 + 1);
          if ( refCols )
            driver.appendColumnNameHelper(refCols->firstToken(), refCols->lastToken(), refTable->schemaName, refTable->tableName);
        }
        $$ = driver.makeAstNode<RefConstraintBodyAST>( $1, refTable, refCols, $4->asKeyMatch(), $5->asKeyActions() );
      }
    ;

opt_no_inherit:
      T_NO T_INHERIT
      { $$ = driver.makeAstNode<NoInheritAST>( $1, $2 ); }
    | empty_aka_optional
    ;

opt_definition:
      T_WITH definition
      { $$ = driver.makeAstNode<ConstraintDefinitionsAST>( $1, $2->asDefinition() ); }
    | empty_aka_optional
    ;

definition:
      T_LEFT_PAREN def_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<DefinitionAST>( $1, $2, $3 ); }
    ;

def_list:
      def_elem
      { $$ = driver.makeAstNode<DefinitionElemListAST>( $1->asDefinitionElem() ); }
    | def_list T_COMMA def_elem
      { $$ = driver.makeAstNode<DefinitionElemListAST>( $1, $3->asDefinitionElem() ); }
    ;

def_elem:
      ColLabel T_EQUAL def_arg
      { $$ = driver.makeAstNode<DefinitionElemAST>( $1, $2, $3->asDefinitionArgument() ); }
    | ColLabel
      { $$ = driver.makeAstNode<DefinitionElemAST>( $1,  0, (DefinitionArgumentAST *) 0 ); }
    ;

def_arg:
      func_type
      { $$ = driver.makeAstNode<FuncTyDefinitionArgumentAST>( $1->asFunctionType() ); }
    | reserved_keyword
      { $$ = driver.makeAstNode<SimpleDefinitionArgumentAST>( $1 ); }
    | qual_all_Op
      { $$ = driver.makeAstNode<QualOpDefinitionArgumentAST>( $1->asQualifiedOperator() ); }
    | NumericOnly
      { $$ = driver.makeAstNode<SimpleDefinitionArgumentAST>( $1 ); }
    | Sconst
      { $$ = driver.makeAstNode<SimpleDefinitionArgumentAST>( $1 ); }
    ;

func_type:
      Typename
      { $$ = driver.makeAstNode<SimpleFunctionTypeAST>( $1->asType() ); }
    | type_function_name attrs T_PERCENT T_TYPE
      { $$ = driver.makeAstNode<PercentFunctionTypeAST>(  0, driver.createName(NameAST::Unknown, $1), $2, $3, $4 ); }
    | T_SETOF type_function_name attrs T_PERCENT T_TYPE
      { $$ = driver.makeAstNode<PercentFunctionTypeAST>( $1, driver.createName(NameAST::Unknown, $2), $3, $4, $5 ); }
    ;

ConstraintAttr:
      T_DEFERRABLE
      { $$ = driver.makeAstNode<DeferColConstraintAST>(  0, $1 ); }
    | T_NOT T_DEFERRABLE
      { $$ = driver.makeAstNode<DeferColConstraintAST>( $1, $2 ); }
    | T_INITIALLY T_DEFERRED
      { $$ = driver.makeAstNode<InitiallyColConstraintAST>( $1, $2 ); }
    | T_INITIALLY T_IMMEDIATE
      { $$ = driver.makeAstNode<InitiallyColConstraintAST>( $1, $2 ); }
    ;

OptConsTableSpace:
      T_USING T_INDEX T_TABLESPACE name
      { $$ = driver.makeAstNode<ConstraintTablespaceAST>( $1, $2, $3, driver.createName(NameAST::Unknown, $4) ); }
    | empty_aka_optional
    ;

key_actions:
      key_update
      { $$ = driver.makeAstNode<UpdDelKeyActionsAST>( $1->asKeyUpdate(), (KeyDeleteAST *) 0 ); }
    | key_delete
      { $$ = driver.makeAstNode<DelUpdKeyActionsAST>( $1->asKeyDelete(), (KeyUpdateAST *) 0 ); }
    | key_update key_delete
      { $$ = driver.makeAstNode<UpdDelKeyActionsAST>( $1->asKeyUpdate(), $2->asKeyDelete() ); }
    | key_delete key_update
      { $$ = driver.makeAstNode<DelUpdKeyActionsAST>( $1->asKeyDelete(), $2->asKeyUpdate() ); }
    | empty_aka_optional
    ;

key_update:
      T_ON T_UPDATE key_action
      { $$ = driver.makeAstNode<KeyUpdateAST>( $1, $2, $3->asKeyAction() ); }
    ;

key_delete:
      T_ON T_DELETE key_action
      { $$ = driver.makeAstNode<KeyDeleteAST>( $1, $2, $3->asKeyAction() ); }
    ;

key_action:
      T_NO T_ACTION
      { $$ = driver.makeAstNode<KeyActionAST>( $1, $2 ); }
    | T_RESTRICT
      { $$ = driver.makeAstNode<KeyActionAST>( $1,  0 ); }
    | T_CASCADE
      { $$ = driver.makeAstNode<KeyActionAST>( $1,  0 ); }
    | T_SET T_NULL
      { $$ = driver.makeAstNode<KeyActionAST>( $1, $2 ); }
    | T_SET T_DEFAULT
      { $$ = driver.makeAstNode<KeyActionAST>( $1, $2 ); }
    ;

key_match:
      T_MATCH T_FULL
      { $$ = driver.makeAstNode<KeyMatchAST>( $1, $2 ); }
    | T_MATCH T_PARTIAL
      { $$ = driver.makeAstNode<KeyMatchAST>( $1, $2 ); }
    | T_MATCH T_SIMPLE
      { $$ = driver.makeAstNode<KeyMatchAST>( $1, $2 ); }
    | empty_aka_optional
    ;

opt_column_list:
      T_LEFT_PAREN columnList T_RIGHT_PAREN
      { $$ = driver.makeAstNode<RefColumnsAST>( $1, $2, $3 ); }
    | empty_aka_optional
    ;

columnList:
      columnElem
      { $$ = driver.makeAstNode<NameListAST>( driver.createName(NameAST::Column, $1) ); }
    | columnList T_COMMA columnElem
      { $$ = driver.makeAstNode<NameListAST>( $1, driver.createName(NameAST::Column, $3) ); }
    ;

columnElem:
      ColId
    ;

create_generic_options:
      T_OPTIONS T_LEFT_PAREN generic_option_list T_RIGHT_PAREN
      { $$ = driver.makeAstNode<CreateGeneticOptionsAST>( $1, $2, $3, $4 ); }
    | empty_aka_optional
    ;

generic_option_list:
      generic_option_elem
      { $$ = driver.makeAstNode<GenericOptionListAST>( $1->asGenericOption() ); }
    | generic_option_list T_COMMA generic_option_elem
      { $$ = driver.makeAstNode<GenericOptionListAST>( $1, $3->asGenericOption() ); }
    ;

generic_option_elem:
      generic_option_name generic_option_arg
      { $$ = driver.makeAstNode<GenericOptionAST>( $1, $2 ); }
    ;

generic_option_name:
      ColLabel
    ;

generic_option_arg:
      Sconst
    ;

alter_relation_expr:
      table_identifier
      { $$ = driver.makeAstNode<RelationExprSimpleAST>( $1->asTableIdentifier(), 0 ); }
    | table_identifier T_STAR
      { $$ = driver.makeAstNode<RelationExprSimpleAST>( $1->asTableIdentifier(), $2 ); }
    | T_ONLY table_identifier
      { $$ = driver.makeAstNode<RelationExprOnlyAST>( $1,  0, $2->asTableIdentifier(),  0); }
    | T_ONLY T_LEFT_PAREN table_identifier T_RIGHT_PAREN
      { $$ = driver.makeAstNode<RelationExprOnlyAST>( $1, $2, $3->asTableIdentifier(), $4); }
  ;

relation_expr:
      table_identifier
      {
        RelationExprAST *rel = driver.makeAstNode<RelationExprSimpleAST>( $1->asTableIdentifier(), 0 );
        $$ = rel;
        driver.addRelation(rel);
      }
    | table_identifier T_STAR
      {
        RelationExprAST *rel = driver.makeAstNode<RelationExprSimpleAST>( $1->asTableIdentifier(), $2 );
        $$ = rel;
        driver.addRelation(rel);
      }
    | T_ONLY table_identifier
      {
        RelationExprAST *rel = driver.makeAstNode<RelationExprOnlyAST>( $1,  0, $2->asTableIdentifier(),  0);
        $$ = rel;
        driver.addRelation(rel);
      }
    | T_ONLY T_LEFT_PAREN table_identifier T_RIGHT_PAREN
      {
        RelationExprAST *rel = driver.makeAstNode<RelationExprOnlyAST>( $1, $2, $3->asTableIdentifier(), $4);
        $$ = rel;
        driver.addRelation(rel);
      }
  ;

table_identifier:
      ColId T_DOT ColId
      {
        $$ = driver.makeAstNode<TableIdentifierAST>( driver.createName(NameAST::Schema, $1),
                                                     $2,
                                                     driver.createName(NameAST::Table, $3) );
      }
    | ColId
      {
        $$ = driver.makeAstNode<TableIdentifierAST>( (NameAST *) 0,
                                                     0,
                                                     driver.createName(NameAST::Table, $1) );
      }
    ;

table_identifier_we:
      ColId T_DOT ColId
      {
        $$ = driver.makeAstNode<TableIdentifierAST>( driver.createName(NameAST::Schema, $1),
                                                     $2,
                                                     driver.createName(NameAST::Table, $3) );
      }
    | ColId T_DOT
      {
        $$ = driver.makeAstNode<TableIdentifierAST>( driver.createName(NameAST::Schema, $1),
                                                     $2,
                                                     (NameAST *) 0 );
      }
    | ColId
      {
        $$ = driver.makeAstNode<TableIdentifierAST>( (NameAST *) 0,
                                                     0,
                                                     driver.createName(NameAST::Table, $1) );
      }
    ;

column_identifier:
      ColId T_DOT ColId T_DOT ColId
      {
        $$ = driver.makeAstNode<ColumnIdentifierAST>( driver.createName(NameAST::Schema, $1),
                                                      $2,
                                                      driver.createName(NameAST::Table, $3),
                                                      $4,
                                                      driver.createName(NameAST::Column, $5) );
      }
    | ColId T_DOT ColId T_DOT
    {
      $$ = driver.makeAstNode<ColumnIdentifierAST>( driver.createName(NameAST::Schema, $1),
                                                    $2,
                                                    driver.createName(NameAST::Table, $3),
                                                    $4,
                                                    (NameAST *) 0 );
    }
    | ColId T_DOT ColId
      {
        $$ = driver.makeAstNode<ColumnIdentifierAST>( (NameAST *) 0,
                                                      0,
                                                      driver.createName(NameAST::Table, $1),
                                                      $2,
                                                      driver.createName(NameAST::Column, $3) );
      }
    | ColId T_DOT
      {
        $$ = driver.makeAstNode<ColumnIdentifierAST>( (NameAST *) 0,
                                                      0,
                                                      driver.createName(NameAST::Table, $1),
                                                      $2,
                                                      (NameAST *) 0 );
      }
    | ColId
      {
        $$ = driver.makeAstNode<ColumnIdentifierAST>( (NameAST *) 0,
                                                      0,
                                                      (NameAST *) 0,
                                                      0,
                                                      driver.createName(NameAST::Column, $1) );
      }
;

index_identifier:
      ColId T_DOT ColId
      {
        $$ = driver.makeAstNode<IndexIdentifierAST>( driver.createName(NameAST::Schema, $1),
                                                     $2,
                                                     driver.createName(NameAST::Index, $3) );
      }
    | ColId
      {
        $$ = driver.makeAstNode<IndexIdentifierAST>( (NameAST *) 0,
                                                     0,
                                                     driver.createName(NameAST::Index, $1) );
      }
    ;

trigger_identifier:
      ColId T_DOT ColId
      {
        $$ = driver.makeAstNode<TriggerIdentifierAST>( driver.createName(NameAST::Schema, $1),
                                                       $2,
                                                       driver.createName(NameAST::Trigger, $3) );
      }
    | ColId
      {
        $$ = driver.makeAstNode<TriggerIdentifierAST>( (NameAST *) 0,
                                                       0,
                                                       driver.createName(NameAST::Trigger, $1) );
      }
    ;

constraint_identifier:
      ColId T_DOT ColId
      {
        $$ = driver.makeAstNode<ConstraintIdentifierAST>( driver.createName(NameAST::Schema, $1),
                                                          $2,
                                                          driver.createName(NameAST::Constraint, $3) );
      }
    | ColId
      {
        $$ = driver.makeAstNode<ConstraintIdentifierAST>( (NameAST *) 0,
                                                          0,
                                                          driver.createName(NameAST::Constraint, $1) );
      }
    ;

sequence_identifier:
      ColId T_DOT ColId
      {
        $$ = driver.makeAstNode<SequenceIdentifierAST>( driver.createName(NameAST::Schema, $1),
                                                        $2,
                                                        driver.createName(NameAST::Sequence, $3) );
      }
    | ColId
      {
        $$ = driver.makeAstNode<SequenceIdentifierAST>( (NameAST *) 0,
                                                        0,
                                                        driver.createName(NameAST::Sequence, $1) );
      }
    ;

SetResetClause:
      T_SET set_rest
      { $$ = driver.makeAstNode<SetResetClauseAsSetAST>( $1, $2->asSetRest() ); }
    | VariableResetStmt
      { $$ = driver.makeAstNode<SetResetClauseAsVarResetAST>( $1->asVariableResetStmt() ); }
    ;

VariableResetStmt:
      T_RESET var_name                          { $$ = driver.makeAstNode<SetRestResetVarNameAST>( $1, $2 ); }
    | T_RESET T_TIME T_ZONE                     { $$ = driver.makeAstNode<SetRestResetTimeZoneAST>( $1, $2, $3 ); }
    | T_RESET T_TRANSACTION T_ISOLATION T_LEVEL { $$ = driver.makeAstNode<SetRestResetTransLevelAST>( $1, $2, $3, $4 ); }
    | T_RESET T_SESSION T_AUTHORIZATION         { $$ = driver.makeAstNode<SetRestResetAuthorizationAST>( $1, $2, $3 ); }
    | T_RESET T_ALL                             { $$ = driver.makeAstNode<SetRestResetAllAST>( $1 ); }
    ;

set_rest:
      T_TRANSACTION transaction_mode_list
      { $$ = driver.makeAstNode<SetRestTransactionAST>(  0,  0,  0, $1, $2); }
    | T_SESSION T_CHARACTERISTICS T_AS T_TRANSACTION transaction_mode_list
      { $$ = driver.makeAstNode<SetRestTransactionAST>( $1, $2, $3, $4, $5); }
    | set_rest_more
    ;

set_rest_more:
      /* Generic SET syntaxes: */
      var_name T_TO var_list            { $$ = driver.makeAstNode<SetRestVarNameToValuesAST>( $1, $2, $3 ); }
    | var_name T_EQUAL var_list         { $$ = driver.makeAstNode<SetRestVarNameToValuesAST>( $1, $2, $3 ); }
    | var_name T_TO T_DEFAULT           { $$ = driver.makeAstNode<SetRestVarNameToAST>( $1, $2, $3 ); }
    | var_name T_EQUAL T_DEFAULT        { $$ = driver.makeAstNode<SetRestVarNameToAST>( $1, $2, $3 ); }
    | var_name T_FROM T_CURRENT         { $$ = driver.makeAstNode<SetRestVarNameToAST>( $1, $2, $3 ); }
      /* Special syntaxes mandated by SQL standard: */
    | T_TIME T_ZONE zone_value          { $$ = driver.makeAstNode<SetRestZoneValueAST>( $1, $2, $3->asZoneValue()); }
    | T_CATALOG Sconst                  { $$ = driver.makeAstNode<SetRestCatalogAST>( $1, $2 ); }
    | T_SCHEMA Sconst                   { $$ = driver.makeAstNode<SetRestSchemaAST>( $1, $2 ); }
    | T_NAMES opt_encoding              { $$ = driver.makeAstNode<SetRestEncodingAST>( $1, $2 ); }
    | T_ROLE NonReservedWord_or_Sconst  { $$ = driver.makeAstNode<SetRestRoleAST>( $1, $2 ); }
    | T_SESSION T_AUTHORIZATION NonReservedWord_or_Sconst
      { $$ = driver.makeAstNode<SetRestSessionAuthAST>( $1, $2, $3 ); }
    | T_SESSION T_AUTHORIZATION T_DEFAULT
      { $$ = driver.makeAstNode<SetRestSessionAuthAST>( $1, $2, $3 ); }
    | T_XML T_OPTION document_or_content
      { $$ = driver.makeAstNode<SetRestXmlOptionAST>( $1, $2, $3 ); }
      /* Special syntaxes invented by PostgreSQL: */
    | T_TRANSACTION T_SNAPSHOT Sconst
      { $$ = driver.makeAstNode<SetRestTransactioSnapshotAST>( $1, $2, $3 ); }
    ;

var_name:
      ColId
      { $$ = driver.makeAstNode<NameListAST>( driver.createName(NameAST::Variable, $1) ); }
    | var_name T_DOT ColId
      { $$ = driver.makeAstNode<NameListAST>( $1, driver.createName(NameAST::Variable, $3) ); }
    ;

var_list:
      var_value
      { $$ = driver.makeAstNode<VarValueListAST>( $1->asVarValue() ); }
    | var_list T_COMMA var_value
      { $$ = driver.makeAstNode<VarValueListAST>( $1, $3->asVarValue() ); }
    ;

var_value:
      opt_boolean_or_string { $$ = driver.makeAstNode<VarValueAST>( $1 ); }
    | NumericOnly           { $$ = driver.makeAstNode<VarValueAST>( $1 ); }
    ;

zone_value:
      Sconst        { $$ = driver.makeAstNode<ZoneValueSimpleAST>( $1 ); }
    | T_IDENTIFIER  { $$ = driver.makeAstNode<ZoneValueSimpleAST>( $1 ); }
    | ConstInterval Sconst opt_interval
      { $$ = driver.makeAstNode<ZoneValueIntervalAST>( $1,  0, $2,  0,  0, $3->asInterval() ); }
    | ConstInterval T_LEFT_PAREN Iconst T_RIGHT_PAREN Sconst opt_interval
      { $$ = driver.makeAstNode<ZoneValueIntervalAST>( $1, $2, $3, $4, $5, $6->asInterval() ); }
    | NumericOnly   { $$ = driver.makeAstNode<ZoneValueSimpleAST>( $1 ); }
    | T_DEFAULT     { $$ = driver.makeAstNode<ZoneValueSimpleAST>( $1 ); }
    | T_LOCAL       { $$ = driver.makeAstNode<ZoneValueSimpleAST>( $1 ); }
    ;

ConstDatetime:
      T_TIMESTAMP T_LEFT_PAREN Iconst T_RIGHT_PAREN opt_timezone
      { $$ = driver.makeAstNode<DatetimeTypenameAST>( $1, $2, $3, $4, $5->asTimeZoneSpec() ); }
    | T_TIMESTAMP opt_timezone
      { $$ = driver.makeAstNode<DatetimeTypenameAST>( $1,  0,  0,  0, $2->asTimeZoneSpec() ); }
    | T_TIME T_LEFT_PAREN Iconst T_RIGHT_PAREN opt_timezone
      { $$ = driver.makeAstNode<DatetimeTypenameAST>( $1, $2, $3, $4, $5->asTimeZoneSpec() ); }
    | T_TIME opt_timezone
      { $$ = driver.makeAstNode<DatetimeTypenameAST>( $1,  0,  0,  0, $2->asTimeZoneSpec() ); }
    ;

opt_timezone:
      T_WITH_TIME T_ZONE    { $$ = driver.makeAstNode<TimeZoneSpecAST>( $1, $1 + 1, $2 ); }
    | T_WITHOUT T_TIME T_ZONE { $$ = driver.makeAstNode<TimeZoneSpecAST>( $1, $2, $3 ); }
    | empty_aka_optional
    ;

ConstInterval:
      T_INTERVAL
    ;

NonReservedWord_or_Sconst:
      NonReservedWord
    | Sconst
    ;

NumericOnly: SignedIconst ;

document_or_content:
      T_DOCUMENT
    | T_CONTENT
    ;

opt_boolean_or_string:
      T_TRUE
    | T_FALSE
    | T_ON
    | NonReservedWord_or_Sconst
    ;

opt_encoding:
      Sconst
    | T_DEFAULT
    | /*EMPTY*/ { $$ = 0; }
    ;

opt_interval:
      T_YEAR                          { $$ = driver.makeAstNode<IntervalSimpleAST>( $1 ); }
    | T_MONTH                         { $$ = driver.makeAstNode<IntervalSimpleAST>( $1 ); }
    | T_DAY                           { $$ = driver.makeAstNode<IntervalSimpleAST>( $1 ); }
    | T_HOUR                          { $$ = driver.makeAstNode<IntervalSimpleAST>( $1 ); }
    | T_MINUTE                        { $$ = driver.makeAstNode<IntervalSimpleAST>( $1 ); }
    | interval_second                 { $$ = driver.makeAstNode<IntervalSimpleToSecondAST>( 0, 0, $1->asIntervalSecond() ); }
    | T_YEAR T_TO T_MONTH             { $$ = driver.makeAstNode<IntervalSimpleToAST>( $1, $2, $3 ); }
    | T_DAY T_TO T_HOUR               { $$ = driver.makeAstNode<IntervalSimpleToAST>( $1, $2, $3 ); }
    | T_DAY T_TO T_MINUTE             { $$ = driver.makeAstNode<IntervalSimpleToAST>( $1, $2, $3 ); }
    | T_DAY T_TO interval_second      { $$ = driver.makeAstNode<IntervalSimpleToSecondAST>( $1, $2, $3->asIntervalSecond() ); }
    | T_HOUR T_TO T_MINUTE            { $$ = driver.makeAstNode<IntervalSimpleToAST>( $1, $2, $3 ); }
    | T_HOUR T_TO interval_second     { $$ = driver.makeAstNode<IntervalSimpleToSecondAST>( $1, $2, $3->asIntervalSecond() ); }
    | T_MINUTE T_TO interval_second   { $$ = driver.makeAstNode<IntervalSimpleToSecondAST>( $1, $2, $3->asIntervalSecond() ); }
    | empty_aka_optional
    ;

interval_second:
      T_SECOND
      { $$ = driver.makeAstNode<IntervalSecondAST>($1,  0,  0,  0); }
    | T_SECOND T_LEFT_PAREN Iconst T_RIGHT_PAREN
      { $$ = driver.makeAstNode<IntervalSecondAST>($1, $2, $3, $4); }
    ;

transaction_mode_list:
      transaction_mode_item
      { $$ = driver.makeAstNode<TransactionModeListAST>( $1->asTransactionMode() ); }
    | transaction_mode_list T_COMMA transaction_mode_item
      { $$ = driver.makeAstNode<TransactionModeListAST>( $1, $3->asTransactionMode() ); }
    | transaction_mode_list transaction_mode_item
      { $$ = driver.makeAstNode<TransactionModeListAST>( $1, $2->asTransactionMode() ); }
    ;

transaction_mode_item:
      T_ISOLATION T_LEVEL iso_level
      { $$ = driver.makeAstNode<TransactionModeIsoLevelAST>( $1, $2, $3->asIsoLevel() ); }
    | T_READ T_ONLY
      { $$ = driver.makeAstNode<TransactionModeReadWriteAST>( $1, $2 ); }
    | T_READ T_WRITE
      { $$ = driver.makeAstNode<TransactionModeReadWriteAST>( $1, $2 ); }
    | T_DEFERRABLE
      { $$ = driver.makeAstNode<TransactionModeDeferAST>(  0, $1 ); }
    | T_NOT T_DEFERRABLE
      { $$ = driver.makeAstNode<TransactionModeDeferAST>( $1, $2 ); }
    ;

iso_level:
      T_READ T_UNCOMMITTED  { $$ = driver.makeAstNode<IsoLevelAST>( $1, $2 ); }
    | T_READ T_COMMITTED    { $$ = driver.makeAstNode<IsoLevelAST>( $1, $2 ); }
    | T_REPEATABLE T_READ   { $$ = driver.makeAstNode<IsoLevelAST>( $1, $2 ); }
    | T_SERIALIZABLE        { $$ = driver.makeAstNode<IsoLevelAST>( $1,  0 ); }
    ;

Iconst: T_DIGIT ;
Sconst: T_CHARSTR ;

RoleId:		NonReservedWord { $$ = driver.makeAstNode<RoleIdAST>( driver.createName(NameAST::Role, $1) ); }
    ;

ColId:
      T_IDENTIFIER
    | T_STRSTR
    | unreserved_keyword
    | col_name_keyword
    ;

cursor_name:
      name
    ;

any_name:
      ColId
      { $$ = driver.makeAstNode<AnyIdentifierAST>( driver.createName(NameAST::Unknown, $1), (NameListAST *) 0 ); }
    | ColId attrs
      { $$ = driver.makeAstNode<AnyIdentifierAST>( driver.createName(NameAST::Unknown, $1), $2 ); }
    ;

database_name:
      ColId   { $$ = driver.makeAstNode<DatabaseNameAST>( driver.createName(NameAST::Database, $1) ); }
    ;

type_function_name:
      T_IDENTIFIER
    | unreserved_keyword
    | type_func_name_keyword
    ;

attr_name: ColLabel ;

index_name: ColId ;

access_method: ColId ;

role_list:
      RoleId
      { $$ = driver.makeAstNode<RoleIdListAST>( $1->asRoleId() ); }
    | role_list T_COMMA RoleId
      { $$ = driver.makeAstNode<RoleIdListAST>( $1, $3->asRoleId() ); }
    ;

SignedIconst:
      Iconst          { $$ = $1; }
    | T_PLUS Iconst   { $$ = $2; }
    | T_DASH Iconst   { $$ = $2; }
    ;

table_identifier_list:
      table_identifier
      {
        TableIdentifierAST *tableIdent = $1->asTableIdentifier();
        if ( tableIdent )
          $$ = driver.makeAstNode<TableIdentifierListAST>( tableIdent );
      }
    | table_identifier_list T_COMMA table_identifier
      {
        TableIdentifierAST *tableIdent = $3->asTableIdentifier();
        if ( tableIdent )
          $$ = driver.makeAstNode<TableIdentifierListAST>( $1, tableIdent );
      }
    ;

index_identifier_list:
      index_identifier
      { $$ = driver.makeAstNode<IndexIdentifierListAST>( $1->asIndexIdentifier() ); }
    | index_identifier_list T_COMMA index_identifier
      { $$ = driver.makeAstNode<IndexIdentifierListAST>( $1, $3->asIndexIdentifier() ); }
    ;

sequence_identifier_list:
      sequence_identifier
      { $$ = driver.makeAstNode<SequenceIdentifierListAST>( $1->asSequenceIdentifier() ); }
    | sequence_identifier_list T_COMMA sequence_identifier
      { $$ = driver.makeAstNode<SequenceIdentifierListAST>( $1, $3->asSequenceIdentifier() ); }
    ;

/*qualified_name: sequence_identifier_list
      ColId
    | ColId indirection
    ;*/

NonReservedWord:
      T_IDENTIFIER
    | unreserved_keyword
    | col_name_keyword
    | type_func_name_keyword
    ;

ColLabel:
      T_IDENTIFIER
    | T_STRSTR
    | unreserved_keyword
    | col_name_keyword
    | type_func_name_keyword
    | reserved_keyword
;

unreserved_keyword:
      T_ABORT
    | T_ABSOLUTE
    | T_ACCESS
    | T_ACTION
    | T_ADD
    | T_ADMIN
    | T_AFTER
    | T_AGGREGATE
    | T_ALSO
    | T_ALTER
    | T_ALWAYS
    | T_ASSERTION
    | T_ASSIGNMENT
    | T_AT
    | T_ATTRIBUTE
    | T_BACKWARD
    | T_BEFORE
    | T_BEGIN
    | T_BY
    | T_CACHE
    | T_CALLED
    | T_CASCADE
    | T_CASCADED
    | T_CATALOG
    | T_CHAIN
    | T_CHARACTERISTICS
    | T_CHECKPOINT
    | T_CLASS
    | T_CLOSE
    | T_CLUSTER
    | T_COMMENT
    | T_COMMENTS
    | T_COMMIT
    | T_COMMITTED
    | T_CONFIGURATION
    | T_CONNECTION
    | T_CONSTRAINTS
    | T_CONTENT
    | T_CONTINUE
    | T_CONVERSION
    | T_COPY
    | T_COST
    | T_CSV
    | T_CURRENT
    | T_CURSOR
    | T_CYCLE
    | T_DATA
    | T_DATABASE
    | T_DAY
    | T_DEALLOCATE
    | T_DECLARE
    | T_DEFAULTS
    | T_DEFERRED
    | T_DEFINER
    | T_DELETE
    | T_DELIMITER
    | T_DELIMITERS
    | T_DICTIONARY
    | T_DISABLE
    | T_DISCARD
    | T_DOCUMENT
    | T_DOMAIN
    | T_DOUBLE
    | T_DROP
    | T_EACH
    | T_ENABLE
    | T_ENCODING
    | T_ENCRYPTED
    | T_ENUM
    | T_ESCAPE
    | T_EVENT
    | T_EXCLUDE
    | T_EXCLUDING
    | T_EXCLUSIVE
    | T_EXECUTE
    | T_EXPLAIN
    | T_EXTENSION
    | T_EXTERNAL
    | T_FAMILY
    | T_FIRST
    | T_FOLLOWING
    | T_FORCE
    | T_FORWARD
    | T_FUNCTION
    | T_FUNCTIONS
    | T_GLOBAL
    | T_GRANTED
    | T_HANDLER
    | T_HEADER
    | T_HOLD
    | T_HOUR
    | T_IDENTITY
    | T_IF
    | T_IMMEDIATE
    | T_IMMUTABLE
    | T_IMPLICIT
    | T_INCLUDING
    | T_INCREMENT
    | T_INDEX
    | T_INDEXES
    | T_INHERIT
    | T_INHERITS
    | T_INLINE
    | T_INPUT
    | T_INSENSITIVE
    | T_INSERT
    | T_INSTEAD
    | T_INVOKER
    | T_ISOLATION
    | T_KEY
    | T_LABEL
    | T_LANGUAGE
    | T_LARGE
    | T_LAST
    | T_LC_COLLATE
    | T_LC_CTYPE
    | T_LEAKPROOF
    | T_LEVEL
    | T_LISTEN
    | T_LOAD
    | T_LOCAL
    | T_LOCATION
    | T_LOCK
    | T_MAPPING
    | T_MATCH
    | T_MATERIALIZED
    | T_MAXVALUE
    | T_MINUTE
    | T_MINVALUE
    | T_MODE
    | T_MONTH
    | T_MOVE
    | T_NAME
    | T_NAMES
    | T_NEXT
    | T_NO
    | T_NOTHING
    | T_NOTIFY
    | T_NOWAIT
    | T_NULLS
    | T_OBJECT
    | T_OF
    | T_OFF
    | T_OIDS
    | T_OPERATOR
    | T_OPTION
    | T_OPTIONS
    | T_OWNED
    | T_OWNER
    | T_PARSER
    | T_PARTIAL
    | T_PARTITION
    | T_PASSING
    | T_PASSWORD
    | T_PLANS
    | T_PRECEDING
    | T_PREPARE
    | T_PREPARED
    | T_PRESERVE
    | T_PRIOR
    | T_PRIVILEGES
    | T_PROCEDURAL
    | T_PROCEDURE
    | T_PROGRAM
    | T_QUOTE
    | T_RANGE
    | T_READ
    | T_REASSIGN
    | T_RECHECK
    | T_RECURSIVE
    | T_REF
    | T_REFRESH
    | T_REINDEX
    | T_RELATIVE
    | T_RELEASE
    | T_RENAME
    | T_REPEATABLE
    | T_REPLACE
    | T_REPLICA
    | T_RESET
    | T_RESTART
    | T_RESTRICT
    | T_RETURNS
    | T_REVOKE
    | T_ROLE
    | T_ROLLBACK
    | T_ROWS
    | T_RULE
    | T_SAVEPOINT
    | T_SCHEMA
    | T_SCROLL
    | T_SEARCH
    | T_SECOND
    | T_SECURITY
    | T_SEQUENCE
    | T_SEQUENCES
    | T_SERIALIZABLE
    | T_SERVER
    | T_SESSION
    | T_SET
    | T_SHARE
    | T_SHOW
    | T_SIMPLE
    | T_SNAPSHOT
    | T_STABLE
    | T_STANDALONE
    | T_START
    | T_STATEMENT
    | T_STATISTICS
    | T_STDIN
    | T_STDOUT
    | T_STORAGE
    | T_STRICT
    | T_STRIP
    | T_SYSID
    | T_SYSTEM
    | T_TABLES
    | T_TABLESPACE
    | T_TEMP
    | T_TEMPLATE
    | T_TEMPORARY
    | T_TEXT
    | T_TRANSACTION
    | T_TRIGGER
    | T_TRUNCATE
    | T_TRUSTED
    | T_TYPE
    | T_TYPES
    | T_UNBOUNDED
    | T_UNCOMMITTED
    | T_UNENCRYPTED
    | T_UNKNOWN
    | T_UNLISTEN
    | T_UNLOGGED
    | T_UNTIL
    | T_UPDATE
    | T_VACUUM
    | T_VALID
    | T_VALIDATE
    | T_VALIDATOR
    | T_VALUE
    | T_VARYING
    | T_VERSION
    | T_VIEW
    | T_VOLATILE
    | T_WHITESPACE
    | T_WITHOUT
    | T_WORK
    | T_WRAPPER
    | T_WRITE
    | T_XML
    | T_YEAR
    | T_YES
    | T_ZONE
    ;

col_name_keyword:
      T_BETWEEN
    | T_BIGINT
    | T_BIT
    | T_BOOLEAN
    | T_CHAR
    | T_CHARACTER
    | T_COALESCE
    | T_DEC
    | T_DECIMAL
    | T_EXISTS
    | T_EXTRACT
    | T_FLOAT
    | T_GREATEST
    | T_INOUT
    | T_INT
    | T_INTEGER
    | T_INTERVAL
    | T_LEAST
    | T_NATIONAL
    | T_NCHAR
    | T_NONE
    | T_NULLIF
    | T_NUMERIC
    | T_OUT
    | T_OVERLAY
    | T_POSITION
    | T_PRECISION
    | T_REAL
    | T_ROW
    | T_SETOF
    | T_SMALLINT
    | T_SUBSTRING
    | T_TIME
    | T_TIMESTAMP
    | T_TREAT
    | T_TRIM
    | T_VALUES
    | T_VARCHAR
    | T_XMLATTRIBUTES
    | T_XMLCONCAT
    | T_XMLELEMENT
    | T_XMLEXISTS
    | T_XMLFOREST
    | T_XMLPARSE
    | T_XMLPI
    | T_XMLROOT
    | T_XMLSERIALIZE
    ;

type_func_name_keyword:
      T_AUTHORIZATION
    | T_BINARY
    | T_COLLATION
    | T_CONCURRENTLY
    | T_CROSS
    | T_CURRENT_SCHEMA
    | T_FREEZE
    | T_FULL
    | T_ILIKE
    | T_INNER
    | T_IS
    | T_ISNULL
    | T_JOIN
    | T_LEFT
    | T_LIKE
    | T_NATURAL
    | T_NOTNULL
    | T_OUTER
    | T_OVER
    | T_OVERLAPS
    | T_RIGHT
    | T_SIMILAR
    | T_VERBOSE
    ;

reserved_keyword:
      T_ALL
    | T_ANALYSE
    | T_ANALYZE
    | T_AND
    | T_ANY
    | T_ARRAY
    | T_AS
    | T_ASC
    | T_ASYMMETRIC
    | T_BOTH
    | T_CASE
    | T_CAST
    | T_CHECK
    | T_COLLATE
    | T_COLUMN
    | T_CONSTRAINT
    | T_CREATE
    | T_CURRENT_CATALOG
    | T_CURRENT_DATE
    | T_CURRENT_ROLE
    | T_CURRENT_TIME
    | T_CURRENT_TIMESTAMP
    | T_CURRENT_USER
    | T_DEFAULT
    | T_DEFERRABLE
    | T_DESC
    | T_DISTINCT
    | T_DO
    | T_ELSE
    | T_END
    | T_EXCEPT
    | T_FALSE
    | T_FETCH
    | T_FOR
    | T_FOREIGN
    | T_FROM
    | T_GRANT
    | T_GROUP
    | T_HAVING
    | T_IN
    | T_INITIALLY
    | T_INTERSECT
    | T_INTO
    | T_LATERAL
    | T_LEADING
    | T_LIMIT
    | T_LOCALTIME
    | T_LOCALTIMESTAMP
    | T_NOT
    | T_NULL
    | T_OFFSET
    | T_ON
    | T_ONLY
    | T_OR
    | T_ORDER
    | T_PLACING
    | T_PRIMARY
    | T_REFERENCES
    | T_RETURNING
    | T_SELECT
    | T_SESSION_USER
    | T_SOME
    | T_SYMMETRIC
    | T_TABLE
    | T_THEN
    | T_TO
    | T_TRAILING
    | T_TRUE
    | T_UNION
    | T_UNIQUE
    | T_USER
    | T_USING
    | T_VARIADIC
    | T_WHEN
    | T_WHERE
    | T_WINDOW
    | T_WITH
    ;
/*
builtin_func_name:
      T_DAY
    | T_MAX
    | T_MIN
    | T_SUM
    | T_CAST
    | T_HOUR
    | T_COUNT
    | T_MONTH
    | T_MINUTE
    | T_SECOND
    | T_SUBSTRING
    ;

builtin_type_name:
      T_BIGINT
    | T_BIGSERIAL
    | T_BIT
    | T_VARBIT
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
    | T_INT2
    | T_INT4
    | T_INT8
    | T_SERIAL2
    | T_SERIAL4
    | T_SERIAL8
    | T_BOOL
    | T_FLOAT8
    | T_FLOAT4
    | T_TIMETZ
    | T_TIMESTAMPTZ
    | T_INT
    | T_CHAR
    | T_DATE
    | T_YEAR
    | T_ZONE
    | T_FLOAT
    | T_DOUBLE
    | T_OBJECT
    | T_BOOLEAN
    | T_DECIMAL
    | T_INTEGER
    | T_NUMERIC
    | T_VARCHAR
    | T_VARYING
    | T_SMALLINT
    | T_CHARACTER
    ;*/

%%

void Postgres::PostgresParser::error( const std::string &err_message )
{
   std::cerr << "Error: " << err_message << "\n";
}
