%decl QLalrOracleParser.h
%impl QLalrOracleParser.cpp
%parser QLalrOracleParserTable
%token_prefix T_QLO_
%expect 1

%token FEED_ORACLE "feed Oracle"
%token FEED_EXPRESSION "feed expression"

%token ERROR "error - Unknown Token"
%token IDENTIFIER "identifier"
%token NUMBER "number constant"
%token CHARSTR "quoted string"
%token STRSTR "double quoted string"
%token TRUE "true"
%token FALSE "false"

%token GROUP_FUNC "group function"
%token BUILTIN_FUNC "builtin function"

%token BIT "bit"
%token INT "int"
%token BLOB "blob"
%token CHAR "char"
%token CLOB "clob"
%token DATE "date"
%token YEAR "year"
%token ZONE "zone"
%token FLOAT "float"
%token NCHAR "nchar"
%token NCLOB "nclob"
%token BINARY "binary"
%token DOUBLE "double"
%token OBJECT "object"
%token BOOLEAN "boolean"
%token DECIMAL "decimal"
%token INTEGER "integer"
%token NATURAL "natural"
%token NUMERIC "numeric"
%token VARCHAR "varchar"
%token NATIONAL "national"   
%token SMALLINT "smallint"
%token VARCHAR2 "varchar2"
%token CHARARACTER "chararacter"

%token AMPER "&"
%token CARET "^"
%token COLON ":"
%token ASSIGN ":="
%token COMMA ","
%token SLASH "/"
%token DOT "."
%token DOTDOT ".."
%token EQUAL "="
%token EXCLAIM "!"
%token EXCLAIM_EQUAL "!="
%token GREATER ">"
%token GREATER_EQUAL ">="
%token LEFT_BRACE "{"
%token LEFT_BRACKET "["
%token LESS "<"
%token LESS_EQUAL "<="
%token LESS_GREATER "<>"
%token LEFT_PAREN "("
%token DASH "-"
%token PERCENT "%"
%token PIPE "|"
%token PIPE_PIPE "||"
%token PLUS "+"
%token RIGHT_BRACE "}"
%token RIGHT_BRACKET "]"
%token RIGHT_PAREN ")"
%token SEMICOLON ";"
%token STAR "*"

%token AS "as"
%token AT "at"
%token BY "by"
%token DO "do"
%token GO "go"
%token IF "if"
%token IN "in"
%token IS "is"
%token NO "no"
%token OF "of"
%token ON "on"
%token OR "or"
%token TO "to"
%token ADD "add"
%token ALL "all"
%token AND "and"
%token ANY "any"
%token ARE "are"
%token ASC "asc"
%token DEC "dec"
%token END "end"
%token FOR "for"
%token GET "get"
%token KEY "key"
%token MAP "map"
%token NEW "new"
%token NOT "not"
%token OLD "old"
%token OUT "out"
%token REF "ref"
%token ROW "row"
%token SET "set"
%token SQL "sql"
%token BODY "body"
%token BOTH "both"
%token BULK "bulk"
%token CALL "call"
%token CASE "case"
%token CUBE "cube"
%token DATA "data"
%token DESC "desc"
%token DROP "drop"
%token EACH "each"
%token ELSE "else"
%token EXEC "exec"
%token EXIT "exit"
%token FREE "free"
%token FROM "from"
%token FULL "full"
%token GOTO "goto"
%token HOLD "hold"
%token INTO "into"
%token JOIN "join"
%token LEFT "left"
%token LIKE "like"
%token LOCK "lock"
%token LOOP "loop"
%token NEXT "next"
%token NONE "none"
%token NULL "null"
%token ONLY "only"
%token OPEN "open"
%token OVER "over"
%token PATH "path"
%token READ "read"
%token REAL "real"
%token ROLE "role"
%token ROWS "rows"
%token SETS "sets"
%token SOME "some"
%token THEN "then"
%token TIME "time"
%token TRUE "true"
%token TYPE "type"
%token UNDO "undo"
%token USER "user"
%token VIEW "view"
%token WHEN "when"
%token WITH "with"
%token WORK "work"
%token AFTER "after"
%token ALTER "alter"
%token ARRAY "array"
%token BEGIN "begin"
%token CHECK "check"
%token CLOSE "close"
%token CONST "const"
%token CROSS "cross"
%token CYCLE "cycle"
%token DEPTH "depth"
%token DEREF "deref"
%token FALSE "false"
%token FETCH "fetch"
%token FORCE "force"
%token GRANT "grant"
%token GROUP "group"
%token INDEX "index"
%token INNER "inner"
%token INOUT "inout"
%token INPUT "input"
%token LARGE "large"
%token LEAVE "leave"
%token LEVEL "level"
%token LIMIT "limit"
%token LOCAL "local"
%token MATCH "match"
%token MINUS "minus"
%token NAMES "names"
%token ORDER "order"
%token OUTER "outer"
%token PRIOR "prior"
%token RANGE "range"
%token READS "reads"
%token RIGHT "right"
%token SCOPE "scope"
%token SPACE "space"
%token START "start"
%token STATE "state"
%token STORE "store"
%token TABLE "table"
%token TREAT "treat"
%token UNDER "under"
%token UNION "union"
%token UNTIL "until"
%token USAGE "usage"
%token USING "using"
%token VALUE "value"
%token WHERE "where"
%token WHILE "while"
%token WRITE "write"
%token ACTION "action"
%token ATOMIC "atomic"
%token BEFORE "before"
%token COLUMN "column"
%token COMMIT "commit"
%token CREATE "create"
%token CURSOR "cursor"
%token DELETE "delete"
%token DOMAIN "domain"
%token ELSEIF "elseif"
%token EQUALS "equals"
%token ESCAPE "escape"
%token EXCEPT "except"
%token EXISTS "exists"
%token FILTER "filter"
%token GLOBAL "global"
%token HAVING "having"
%token INSERT "insert"
%token METHOD "method"
%token MODULE "module"
%token NOWAIT "nowait"
%token OPTION "option"
%token OUTPUT "output"
%token PUBLIC "public"
%token RECORD "record"
%token REPEAT "repeat"
%token RESULT "result"
%token RETURN "return"
%token REVOKE "revoke"
%token ROLLUP "rollup"
%token SCHEMA "schema"
%token SCROLL "scroll"
%token SEARCH "search"
%token SELECT "select"
%token SIGNAL "signal"
%token STATIC "static"
%token SYSTEM "system"
%token UNIQUE "unique"
%token UNNEST "unnest"
%token UPDATE "update"
%token VALUES "values"
%token VARRAY "varray"
%token WINDOW "window"
%token WITHIN "within"
%token BETWEEN "between"
%token BREADTH "bReadth"
%token CASCADE "cascade"
%token CATALOG "catalog"
%token COLLATE "collate"
%token COLLECT "collect"
%token COMMENT "comment"
%token CONNECT "connect"
%token CURRENT "current"
%token DECLARE "declare"
%token DEFAULT "default"
%token DYNAMIC "dynamic"
%token EXECUTE "execute"
%token FOREIGN "foreign"
%token GENERAL "general"
%token HANDLER "handler"
%token ITERATE "iterate"
%token LATERAL "lateral"
%token LEADING "leading"
%token LOCATOR "locator"
%token PACKAGE "package"
%token PARTIAL "partial"
%token PREPARE "prepare"
%token PRIMARY "primary"
%token RELEASE "release"
%token REPLACE "replace"
%token RETURNS "returns"
%token ROUTINE "routine"
%token ROWTYPE "rowtype"
%token SECTION "section"
%token SESSION "session"
%token SIMILAR "similar"
%token STORAGE "storage"
%token SUBTYPE "subtype"
%token TRIGGER "trigger"
%token UNKNOWN "unknown"
%token VARYING "varying"
%token WITHOUT "without"
%token ABSOLUTE "absolute"
%token ALLOCATE "allocate"
%token CASCADED "cascaded"
%token CONSTANT "constant"
%token CONTINUE "continue"
%token DEFERRED "deferred"
%token DESCRIBE "describe"
%token DISTINCT "distinct"
%token EXTERNAL "external"
%token FUNCTION "function"
%token GROUPING "grouping"
%token IDENTITY "identity"
%token INTERVAL "interval"
%token LANGUAGE "language"
%token MODIFIES "modifies"
%token OVERLAPS "overlaps"
%token PRESERVE "preserve"
%token RELATIVE "relative"
%token RESIGNAL "resignal"
%token RESTRICT "restrict"
%token ROLLBACK "rollback"
%token SPECIFIC "specific"
%token SQLSTATE "sqlstate"
%token TRAILING "trailing"
%token TRUNCATE "truncate"
%token WHENEVER "whenever"
%token ASSERTION "assertion"
%token COLLATION "collation"
%token CONDITION "condition"
%token EXCEPTION "exception"
%token IMMEDIATE "immediate"
%token INDICATOR "indicator"
%token INITIALLY "initially"
%token INTERSECT "intersect"
%token ISOLATION "isolation"
%token LOCALTIME "localtime"
%token PARAMETER "parameter"
%token PARTITION "partition"
%token PIPELINED "pipelined"
%token PRECISION "precision"
%token PROCEDURE "procedure"
%token RECURSIVE "recursive"
%token RELIES_ON "relies_on"
%token RETURNING "returning"
%token SAVEPOINT "savepoint"
%token SENSITIVE "sensitive"
%token STATEMENT "statement"
%token SYMMETRIC "symmetric"
%token TEMPORARY "temporary"
%token TIMESTAMP "timestamp"
%token ASENSITIVE "asensitive"
%token ASYMMETRIC "asymmetric"
%token CONNECTION "connection"
%token CONSTRAINT "constraint"
%token DEALLOCATE "deallocate"
%token DEFERRABLE "deferrable"
%token DESCRIPTOR "descriptor"
%token DISCONNECT "disconnect"
%token ORDINALITY "ordinality"
%token PRIVILEGES "privileges"
%token REFERENCES "references"
%token SQLWARNING "sqlwarning"
%token CONSTRAINTS "constraints"
%token CONSTRUCTOR "constructor"
%token DIAGNOSTICS "diagnostics"
%token INSENSITIVE "insensitive"
%token REFERENCING "referencing"
%token SYSTEM_USER "system_user"
%token TRANSACTION "transaction"
%token TRANSLATION "translation"
%token CURRENT_DATE "current_date"
%token CURRENT_PATH "current_path"
%token CURRENT_ROLE "current_role"
%token CURRENT_TIME "current_time"
%token CURRENT_USER "current_user"
%token RESULT_CACHE "result_cache"
%token SESSION_USER "session_user"
%token SPECIFICTYPE "specifictype"
%token SQLEXCEPTION "sqlexception"
%token AUTHORIZATION "authorization"
%token CORRESPONDING "corresponding"
%token DETERMINISTIC "deterministic"
%token TIMEZONE_HOUR "timezone_hour"
%token LOCALTIMESTAMP "localtimestamp"
%token TIMEZONE_MINUTE "timezone_minute"
%token CURRENT_TIMESTAMP "current_timestamp"

%start toplevel

/:
/********************************************************************************
** QLalr Parser Header
********************************************************************************/
#include "$header"
#include "QLalrOracleAST.h"
#include "SqlToken.h"
#include "TranslationUnit.h"
#include "MemoryPool.h"
#include <vector>
#include <stack>

namespace QLalrOracle {

class Parser: public $table
{
public:
    union Value {
        void *ptr;
        AST *ast;
        NameAST *name;
        List<AST *> *ast_list;
        StatementAST *statement;
        List<StatementAST *> *statement_list;
        ValueExpressionAST *expression;
        List<ValueExpressionAST *> *expression_list;
        ColumnIdentifierAST *column_ident;
        List<ColumnIdentifierAST *> *column_ident_list;
        TableIdentifierAST *table_ident;
        List<TableIdentifierAST *> *table_ident_list;
        BaseSelectColumnItemAST *sel_col;
        List<BaseSelectColumnItemAST *> *sel_col_list;
        TranslationUnitAST *translation_unit;
        AST::Kind kind;
    };
    
    Parser( Sql::TranslationUnit *translationUnit );
    ~Parser();

    TranslationUnitAST *parse() {
        if (AST *u = parse(T_QLO_FEED_ORACLE))
            return u->asTranslationUnit();
        return 0;
    }

    BaseExpressionAST *parseExpression() {
        if (AST *u = parse(T_QLO_FEED_EXPRESSION))
            return u->asBaseExpression();
        return 0;
    }

    AST *parse(int startToken);
    
private:
    int &location(int n) { return _locationStack[_tos + n - 1]; }
    Value &sym(int n) { return _symStack[_tos + n - 1]; }
    AST *&ast(int n) { return _symStack[_tos + n - 1].ast; }
    NameAST *&name(int n) { return _symStack[_tos + n - 1].name; }
    ValueExpressionAST *&expression(int n) { return _symStack[_tos + n - 1].expression; }
    StatementAST *&statement(int n) { return _symStack[_tos + n - 1].statement; }
    ColumnIdentifierAST *&column_ident(int n) { return _symStack[_tos + n - 1].column_ident; }
    TableIdentifierAST *&table_ident(int n) { return _symStack[_tos + n - 1].table_ident; }
    BaseSelectColumnItemAST *&select_column(int n) { return _symStack[_tos + n - 1].sel_col; }
    
    inline int consumeToken() {
        if (_index < int(_tokens->size()))
            return _index++;
        return _tokens->size() - 1;
    }
    inline const Sql::Token &tokenAt(int index) const {
        if (index == 0)
            return _startToken;
        return _tokens->at(index);
    }
    inline int tokenKind(int index) const {
        if (index == 0)
            return _startToken.m_kindGlobal;
        return _tokens->at(index).m_kindGlobal;
    }
    void reduce(int ruleno);

    void warning(int index, const QString &message)
    {
        _translationUnit->warningString(index, message);
    }

    void error(int index, const QString &message)
    {
        _translationUnit->errorString(index, message);
    }

    template <typename T>
    T *makeAstNode()
    {
        T *node = new (_pool) T ();
        return node;
    }
    
    template <typename T>
    T *makeName()
    {
        T *node = new (_pool) T(yyloc, _tokens->at(yyloc).identifier);
        return node;
    }

    template <typename T>
    T *makeName(int offset)
    {
        T *node = new (_pool) T(offset, _tokens->at(offset).identifier);
        return node;
    }

    template <typename T, typename A1>
    T *makeAstNode(A1 a1)
    {
        T *node = new (_pool) T (a1);
        return node;
    }

    template <typename T, typename A1, typename A2>
    T *makeAstNode(A1 a1, A2 a2)
    {
        T *node = new (_pool) T (a1, a2);
        return node;
    }

    template <typename T, typename A1, typename A2, typename A3>
    T *makeAstNode(A1 a1, A2 a2, A3 a3)
    {
        T *node = new (_pool) T (a1, a2, a3);
        return node;
    }

    template <typename T, typename A1, typename A2, typename A3, typename A4>
    T *makeAstNode(A1 a1, A2 a2, A3 a3, A4 a4)
    {
        T *node = new (_pool) T (a1, a2, a3, a4);
        return node;
    }

    template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5>
    T *makeAstNode(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
    {
        T *node = new (_pool) T (a1, a2, a3, a4, a5);
        return node;
    }

    template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
    T *makeAstNode(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
    {
        T *node = new (_pool) T (a1, a2, a3, a4, a5, a6);
        return node;
    }

    template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
    T *makeAstNode(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
    {
        T *node = new (_pool) T (a1, a2, a3, a4, a5, a6, a7);
        return node;
    }

    template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
    T *makeAstNode(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
    {
        T *node = new (_pool) T (a1, a2, a3, a4, a5, a6, a7, a8);
        return node;
    }

    template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
    T *makeAstNode(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
    {
        T *node = new (_pool) T (a1, a2, a3, a4, a5, a6, a7, a8, a9);
        return node;
    }

private:
    Sql::TranslationUnit *_translationUnit;
    Sql::MemoryPool *_pool;
    
    int _tos;
    int _index;
    int yyloc;
    int yytoken;
    int yyrecovering;
    bool _recovered;
    Sql::Token _startToken;
    std::vector<int> _stateStack;
    std::vector<int> _locationStack;
    std::vector<Value> _symStack;
    std::vector<Sql::Token> *_tokens;
};

} // namespace QLalrOracle
:/

/.
/********************************************************************************
** QLalr Parser Source
********************************************************************************/
#include "QLalrOracleParser.h"
#include "QLalrOracleKeywords.h"
#include <iostream>
#include <cstdio>
#include <cassert>
#include <QDebug>

using namespace QLalrOracle;

Parser::Parser( Sql::TranslationUnit *translationUnit)
    : _translationUnit(translationUnit)
    , _pool(_translationUnit->memoryPool())
    , _tos(-1), _index(0), yyloc(-1), yytoken(-1), yyrecovering(0), _recovered(false)
    , _tokens(translationUnit->tokens())
{
    _stateStack.resize(128);
    _locationStack.resize(128);
    _symStack.resize(128);

    for (int i = 1; i < _tokens->size(); i++) {
        Sql::Token &tk = _tokens->at(i);
        switch ( tk.kind() ) {
            case Sql::T_IDENTIFIER: {
                if ( int id1 = tk.keyid1() ) {
                    switch (id1) {
                        case T_2_AS:               tk.m_kindGlobal = T_QLO_AS;                   break;
                        case T_2_AT:               tk.m_kindGlobal = T_QLO_AT;                   break;
                        case T_2_BY:               tk.m_kindGlobal = T_QLO_BY;                   break;
                        case T_2_DO:               tk.m_kindGlobal = T_QLO_DO;                   break;
                        case T_2_GO:               tk.m_kindGlobal = T_QLO_GO;                   break;
                        case T_2_IF:               tk.m_kindGlobal = T_QLO_IF;                   break;
                        case T_2_IN:               tk.m_kindGlobal = T_QLO_IN;                   break;
                        case T_2_IS:               tk.m_kindGlobal = T_QLO_IS;                   break;
                        case T_2_NO:               tk.m_kindGlobal = T_QLO_NO;                   break;
                        case T_2_OF:               tk.m_kindGlobal = T_QLO_OF;                   break;
                        case T_2_ON:               tk.m_kindGlobal = T_QLO_ON;                   break;
                        case T_2_OR:               tk.m_kindGlobal = T_QLO_OR;                   break;
                        case T_2_TO:               tk.m_kindGlobal = T_QLO_TO;                   break;
                        case T_3_ADD:              tk.m_kindGlobal = T_QLO_ADD;                  break;
                        case T_3_ALL:              tk.m_kindGlobal = T_QLO_ALL;                  break;
                        case T_3_AND:              tk.m_kindGlobal = T_QLO_AND;                  break;
                        case T_3_ANY:              tk.m_kindGlobal = T_QLO_ANY;                  break;
                        case T_3_ARE:              tk.m_kindGlobal = T_QLO_ARE;                  break;
                        case T_3_ASC:              tk.m_kindGlobal = T_QLO_ASC;                  break;
                        case T_3_DEC:              tk.m_kindGlobal = T_QLO_DEC;                  break;
                        case T_3_END:              tk.m_kindGlobal = T_QLO_END;                  break;
                        case T_3_FOR:              tk.m_kindGlobal = T_QLO_FOR;                  break;
                        case T_3_GET:              tk.m_kindGlobal = T_QLO_GET;                  break;
                        case T_3_KEY:              tk.m_kindGlobal = T_QLO_KEY;                  break;
                        case T_3_MAP:              tk.m_kindGlobal = T_QLO_MAP;                  break;
                        case T_3_NEW:              tk.m_kindGlobal = T_QLO_NEW;                  break;
                        case T_3_NOT:              tk.m_kindGlobal = T_QLO_NOT;                  break;
                        case T_3_OLD:              tk.m_kindGlobal = T_QLO_OLD;                  break;
                        case T_3_OUT:              tk.m_kindGlobal = T_QLO_OUT;                  break;
                        case T_3_REF:              tk.m_kindGlobal = T_QLO_REF;                  break;
                        case T_3_ROW:              tk.m_kindGlobal = T_QLO_ROW;                  break;
                        case T_3_SET:              tk.m_kindGlobal = T_QLO_SET;                  break;
                        case T_3_SQL:              tk.m_kindGlobal = T_QLO_SQL;                  break;
                        case T_4_BODY:             tk.m_kindGlobal = T_QLO_BODY;                 break;
                        case T_4_BOTH:             tk.m_kindGlobal = T_QLO_BOTH;                 break;
                        case T_4_BULK:             tk.m_kindGlobal = T_QLO_BULK;                 break;
                        case T_4_CALL:             tk.m_kindGlobal = T_QLO_CALL;                 break;
                        case T_4_CASE:             tk.m_kindGlobal = T_QLO_CASE;                 break;
                        case T_4_CUBE:             tk.m_kindGlobal = T_QLO_CUBE;                 break;
                        case T_4_DATA:             tk.m_kindGlobal = T_QLO_DATA;                 break;
                        case T_4_DESC:             tk.m_kindGlobal = T_QLO_DESC;                 break;
                        case T_4_DROP:             tk.m_kindGlobal = T_QLO_DROP;                 break;
                        case T_4_EACH:             tk.m_kindGlobal = T_QLO_EACH;                 break;
                        case T_4_ELSE:             tk.m_kindGlobal = T_QLO_ELSE;                 break;
                        case T_4_EXEC:             tk.m_kindGlobal = T_QLO_EXEC;                 break;
                        case T_4_EXIT:             tk.m_kindGlobal = T_QLO_EXIT;                 break;
                        case T_4_FREE:             tk.m_kindGlobal = T_QLO_FREE;                 break;
                        case T_4_FROM:             tk.m_kindGlobal = T_QLO_FROM;                 break;
                        case T_4_FULL:             tk.m_kindGlobal = T_QLO_FULL;                 break;
                        case T_4_GOTO:             tk.m_kindGlobal = T_QLO_GOTO;                 break;
                        case T_4_HOLD:             tk.m_kindGlobal = T_QLO_HOLD;                 break;
                        case T_4_INTO:             tk.m_kindGlobal = T_QLO_INTO;                 break;
                        case T_4_JOIN:             tk.m_kindGlobal = T_QLO_JOIN;                 break;
                        case T_4_LEFT:             tk.m_kindGlobal = T_QLO_LEFT;                 break;
                        case T_4_LIKE:             tk.m_kindGlobal = T_QLO_LIKE;                 break;
                        case T_4_LOCK:             tk.m_kindGlobal = T_QLO_LOCK;                 break;
                        case T_4_LOOP:             tk.m_kindGlobal = T_QLO_LOOP;                 break;
                        case T_4_NEXT:             tk.m_kindGlobal = T_QLO_NEXT;                 break;
                        case T_4_NONE:             tk.m_kindGlobal = T_QLO_NONE;                 break;
                        case T_4_NULL:             tk.m_kindGlobal = T_QLO_NULL;                 break;
                        case T_4_ONLY:             tk.m_kindGlobal = T_QLO_ONLY;                 break;
                        case T_4_OPEN:             tk.m_kindGlobal = T_QLO_OPEN;                 break;
                        case T_4_OVER:             tk.m_kindGlobal = T_QLO_OVER;                 break;
                        case T_4_PATH:             tk.m_kindGlobal = T_QLO_PATH;                 break;
                        case T_4_READ:             tk.m_kindGlobal = T_QLO_READ;                 break;
                        case T_4_REAL:             tk.m_kindGlobal = T_QLO_REAL;                 break;
                        case T_4_ROLE:             tk.m_kindGlobal = T_QLO_ROLE;                 break;
                        case T_4_ROWS:             tk.m_kindGlobal = T_QLO_ROWS;                 break;
                        case T_4_SETS:             tk.m_kindGlobal = T_QLO_SETS;                 break;
                        case T_4_SOME:             tk.m_kindGlobal = T_QLO_SOME;                 break;
                        case T_4_THEN:             tk.m_kindGlobal = T_QLO_THEN;                 break;
                        case T_4_TIME:             tk.m_kindGlobal = T_QLO_TIME;                 break;
                        case T_4_TRUE:             tk.m_kindGlobal = T_QLO_TRUE;                 break;
                        case T_4_TYPE:             tk.m_kindGlobal = T_QLO_TYPE;                 break;
                        case T_4_UNDO:             tk.m_kindGlobal = T_QLO_UNDO;                 break;
                        case T_4_USER:             tk.m_kindGlobal = T_QLO_USER;                 break;
                        case T_4_VIEW:             tk.m_kindGlobal = T_QLO_VIEW;                 break;
                        case T_4_WHEN:             tk.m_kindGlobal = T_QLO_WHEN;                 break;
                        case T_4_WITH:             tk.m_kindGlobal = T_QLO_WITH;                 break;
                        case T_4_WORK:             tk.m_kindGlobal = T_QLO_WORK;                 break;
                        case T_5_AFTER:            tk.m_kindGlobal = T_QLO_AFTER;                break;
                        case T_5_ALTER:            tk.m_kindGlobal = T_QLO_ALTER;                break;
                        case T_5_ARRAY:            tk.m_kindGlobal = T_QLO_ARRAY;                break;
                        case T_5_BEGIN:            tk.m_kindGlobal = T_QLO_BEGIN;                break;
                        case T_5_CHECK:            tk.m_kindGlobal = T_QLO_CHECK;                break;
                        case T_5_CLOSE:            tk.m_kindGlobal = T_QLO_CLOSE;                break;
                        case T_5_CONST:            tk.m_kindGlobal = T_QLO_CONST;                break;
                        case T_5_CROSS:            tk.m_kindGlobal = T_QLO_CROSS;                break;
                        case T_5_CYCLE:            tk.m_kindGlobal = T_QLO_CYCLE;                break;
                        case T_5_DEPTH:            tk.m_kindGlobal = T_QLO_DEPTH;                break;
                        case T_5_DEREF:            tk.m_kindGlobal = T_QLO_DEREF;                break;
                        case T_5_FALSE:            tk.m_kindGlobal = T_QLO_FALSE;                break;
                        case T_5_FETCH:            tk.m_kindGlobal = T_QLO_FETCH;                break;
                        case T_5_FORCE:            tk.m_kindGlobal = T_QLO_FORCE;                break;
                        case T_5_GRANT:            tk.m_kindGlobal = T_QLO_GRANT;                break;
                        case T_5_GROUP:            tk.m_kindGlobal = T_QLO_GROUP;                break;
                        case T_5_INDEX:            tk.m_kindGlobal = T_QLO_INDEX;                break;
                        case T_5_INNER:            tk.m_kindGlobal = T_QLO_INNER;                break;
                        case T_5_INOUT:            tk.m_kindGlobal = T_QLO_INOUT;                break;
                        case T_5_INPUT:            tk.m_kindGlobal = T_QLO_INPUT;                break;
                        case T_5_LARGE:            tk.m_kindGlobal = T_QLO_LARGE;                break;
                        case T_5_LEAVE:            tk.m_kindGlobal = T_QLO_LEAVE;                break;
                        case T_5_LEVEL:            tk.m_kindGlobal = T_QLO_LEVEL;                break;
                        case T_5_LIMIT:            tk.m_kindGlobal = T_QLO_LIMIT;                break;
                        case T_5_LOCAL:            tk.m_kindGlobal = T_QLO_LOCAL;                break;
                        case T_5_MATCH:            tk.m_kindGlobal = T_QLO_MATCH;                break;
                        case T_5_MINUS:            tk.m_kindGlobal = T_QLO_MINUS;                break;
                        case T_5_NAMES:            tk.m_kindGlobal = T_QLO_NAMES;                break;
                        case T_5_ORDER:            tk.m_kindGlobal = T_QLO_ORDER;                break;
                        case T_5_OUTER:            tk.m_kindGlobal = T_QLO_OUTER;                break;
                        case T_5_PRIOR:            tk.m_kindGlobal = T_QLO_PRIOR;                break;
                        case T_5_RANGE:            tk.m_kindGlobal = T_QLO_RANGE;                break;
                        case T_5_READS:            tk.m_kindGlobal = T_QLO_READS;                break;
                        case T_5_RIGHT:            tk.m_kindGlobal = T_QLO_RIGHT;                break;
                        case T_5_SCOPE:            tk.m_kindGlobal = T_QLO_SCOPE;                break;
                        case T_5_SPACE:            tk.m_kindGlobal = T_QLO_SPACE;                break;
                        case T_5_START:            tk.m_kindGlobal = T_QLO_START;                break;
                        case T_5_STATE:            tk.m_kindGlobal = T_QLO_STATE;                break;
                        case T_5_STORE:            tk.m_kindGlobal = T_QLO_STORE;                break;
                        case T_5_TABLE:            tk.m_kindGlobal = T_QLO_TABLE;                break;
                        case T_5_TREAT:            tk.m_kindGlobal = T_QLO_TREAT;                break;
                        case T_5_UNDER:            tk.m_kindGlobal = T_QLO_UNDER;                break;
                        case T_5_UNION:            tk.m_kindGlobal = T_QLO_UNION;                break;
                        case T_5_UNTIL:            tk.m_kindGlobal = T_QLO_UNTIL;                break;
                        case T_5_USAGE:            tk.m_kindGlobal = T_QLO_USAGE;                break;
                        case T_5_USING:            tk.m_kindGlobal = T_QLO_USING;                break;
                        case T_5_VALUE:            tk.m_kindGlobal = T_QLO_VALUE;                break;
                        case T_5_WHERE:            tk.m_kindGlobal = T_QLO_WHERE;                break;
                        case T_5_WHILE:            tk.m_kindGlobal = T_QLO_WHILE;                break;
                        case T_5_WRITE:            tk.m_kindGlobal = T_QLO_WRITE;                break;
                        case T_6_ACTION:           tk.m_kindGlobal = T_QLO_ACTION;               break;
                        case T_6_ATOMIC:           tk.m_kindGlobal = T_QLO_ATOMIC;               break;
                        case T_6_BEFORE:           tk.m_kindGlobal = T_QLO_BEFORE;               break;
                        case T_6_COLUMN:           tk.m_kindGlobal = T_QLO_COLUMN;               break;
                        case T_6_COMMIT:           tk.m_kindGlobal = T_QLO_COMMIT;               break;
                        case T_6_CREATE:           tk.m_kindGlobal = T_QLO_CREATE;               break;
                        case T_6_CURSOR:           tk.m_kindGlobal = T_QLO_CURSOR;               break;
                        case T_6_DELETE:           tk.m_kindGlobal = T_QLO_DELETE;               break;
                        case T_6_DOMAIN:           tk.m_kindGlobal = T_QLO_DOMAIN;               break;
                        case T_6_ELSEIF:           tk.m_kindGlobal = T_QLO_ELSEIF;               break;
                        case T_6_EQUALS:           tk.m_kindGlobal = T_QLO_EQUALS;               break;
                        case T_6_ESCAPE:           tk.m_kindGlobal = T_QLO_ESCAPE;               break;
                        case T_6_EXCEPT:           tk.m_kindGlobal = T_QLO_EXCEPT;               break;
                        case T_6_EXISTS:           tk.m_kindGlobal = T_QLO_EXISTS;               break;
                        case T_6_FILTER:           tk.m_kindGlobal = T_QLO_FILTER;               break;
                        case T_6_GLOBAL:           tk.m_kindGlobal = T_QLO_GLOBAL;               break;
                        case T_6_HAVING:           tk.m_kindGlobal = T_QLO_HAVING;               break;
                        case T_6_INSERT:           tk.m_kindGlobal = T_QLO_INSERT;               break;
                        case T_6_METHOD:           tk.m_kindGlobal = T_QLO_METHOD;               break;
                        case T_6_MODULE:           tk.m_kindGlobal = T_QLO_MODULE;               break;
                        case T_6_OPTION:           tk.m_kindGlobal = T_QLO_OPTION;               break;
                        case T_6_OUTPUT:           tk.m_kindGlobal = T_QLO_OUTPUT;               break;
                        case T_6_PUBLIC:           tk.m_kindGlobal = T_QLO_PUBLIC;               break;
                        case T_6_RECORD:           tk.m_kindGlobal = T_QLO_RECORD;               break;
                        case T_6_REPEAT:           tk.m_kindGlobal = T_QLO_REPEAT;               break;
                        case T_6_RESULT:           tk.m_kindGlobal = T_QLO_RESULT;               break;
                        case T_6_RETURN:           tk.m_kindGlobal = T_QLO_RETURN;               break;
                        case T_6_REVOKE:           tk.m_kindGlobal = T_QLO_REVOKE;               break;
                        case T_6_ROLLUP:           tk.m_kindGlobal = T_QLO_ROLLUP;               break;
                        case T_6_SCHEMA:           tk.m_kindGlobal = T_QLO_SCHEMA;               break;
                        case T_6_SCROLL:           tk.m_kindGlobal = T_QLO_SCROLL;               break;
                        case T_6_SEARCH:           tk.m_kindGlobal = T_QLO_SEARCH;               break;
                        case T_6_SELECT:           tk.m_kindGlobal = T_QLO_SELECT;               break;
                        case T_6_SIGNAL:           tk.m_kindGlobal = T_QLO_SIGNAL;               break;
                        case T_6_STATIC:           tk.m_kindGlobal = T_QLO_STATIC;               break;
                        case T_6_SYSTEM:           tk.m_kindGlobal = T_QLO_SYSTEM;               break;
                        case T_6_UNIQUE:           tk.m_kindGlobal = T_QLO_UNIQUE;               break;
                        case T_6_UNNEST:           tk.m_kindGlobal = T_QLO_UNNEST;               break;
                        case T_6_UPDATE:           tk.m_kindGlobal = T_QLO_UPDATE;               break;
                        case T_6_VALUES:           tk.m_kindGlobal = T_QLO_VALUES;               break;
                        case T_6_VARRAY:           tk.m_kindGlobal = T_QLO_VARRAY;               break;
                        case T_6_WINDOW:           tk.m_kindGlobal = T_QLO_WINDOW;               break;
                        case T_6_WITHIN:           tk.m_kindGlobal = T_QLO_WITHIN;               break;
                        case T_7_BETWEEN:          tk.m_kindGlobal = T_QLO_BETWEEN;              break;
                        case T_7_BREADTH:          tk.m_kindGlobal = T_QLO_BREADTH;              break;
                        case T_7_CASCADE:          tk.m_kindGlobal = T_QLO_CASCADE;              break;
                        case T_7_CATALOG:          tk.m_kindGlobal = T_QLO_CATALOG;              break;
                        case T_7_COLLATE:          tk.m_kindGlobal = T_QLO_COLLATE;              break;
                        case T_7_COLLECT:          tk.m_kindGlobal = T_QLO_COLLECT;              break;
                        case T_7_COMMENT:          tk.m_kindGlobal = T_QLO_COMMENT;              break;
                        case T_7_CONNECT:          tk.m_kindGlobal = T_QLO_CONNECT;              break;
                        case T_7_CURRENT:          tk.m_kindGlobal = T_QLO_CURRENT;              break;
                        case T_7_DECLARE:          tk.m_kindGlobal = T_QLO_DECLARE;              break;
                        case T_7_DEFAULT:          tk.m_kindGlobal = T_QLO_DEFAULT;              break;
                        case T_7_DYNAMIC:          tk.m_kindGlobal = T_QLO_DYNAMIC;              break;
                        case T_7_EXECUTE:          tk.m_kindGlobal = T_QLO_EXECUTE;              break;
                        case T_7_FOREIGN:          tk.m_kindGlobal = T_QLO_FOREIGN;              break;
                        case T_7_GENERAL:          tk.m_kindGlobal = T_QLO_GENERAL;              break;
                        case T_7_HANDLER:          tk.m_kindGlobal = T_QLO_HANDLER;              break;
                        case T_7_ITERATE:          tk.m_kindGlobal = T_QLO_ITERATE;              break;
                        case T_7_LATERAL:          tk.m_kindGlobal = T_QLO_LATERAL;              break;
                        case T_7_LEADING:          tk.m_kindGlobal = T_QLO_LEADING;              break;
                        case T_7_LOCATOR:          tk.m_kindGlobal = T_QLO_LOCATOR;              break;
                        case T_7_PACKAGE:          tk.m_kindGlobal = T_QLO_PACKAGE;              break;
                        case T_7_PARTIAL:          tk.m_kindGlobal = T_QLO_PARTIAL;              break;
                        case T_7_PREPARE:          tk.m_kindGlobal = T_QLO_PREPARE;              break;
                        case T_7_PRIMARY:          tk.m_kindGlobal = T_QLO_PRIMARY;              break;
                        case T_7_RELEASE:          tk.m_kindGlobal = T_QLO_RELEASE;              break;
                        case T_7_REPLACE:          tk.m_kindGlobal = T_QLO_REPLACE;              break;
                        case T_7_RETURNS:          tk.m_kindGlobal = T_QLO_RETURNS;              break;
                        case T_7_ROUTINE:          tk.m_kindGlobal = T_QLO_ROUTINE;              break;
                        case T_7_ROWTYPE:          tk.m_kindGlobal = T_QLO_ROWTYPE;              break;
                        case T_7_SECTION:          tk.m_kindGlobal = T_QLO_SECTION;              break;
                        case T_7_SESSION:          tk.m_kindGlobal = T_QLO_SESSION;              break;
                        case T_7_SIMILAR:          tk.m_kindGlobal = T_QLO_SIMILAR;              break;
                        case T_7_STORAGE:          tk.m_kindGlobal = T_QLO_STORAGE;              break;
                        case T_7_SUBTYPE:          tk.m_kindGlobal = T_QLO_SUBTYPE;              break;
                        case T_7_TRIGGER:          tk.m_kindGlobal = T_QLO_TRIGGER;              break;
                        case T_7_UNKNOWN:          tk.m_kindGlobal = T_QLO_UNKNOWN;              break;
                        case T_7_VARYING:          tk.m_kindGlobal = T_QLO_VARYING;              break;
                        case T_7_WITHOUT:          tk.m_kindGlobal = T_QLO_WITHOUT;              break;
                        case T_8_ABSOLUTE:         tk.m_kindGlobal = T_QLO_ABSOLUTE;             break;
                        case T_8_ALLOCATE:         tk.m_kindGlobal = T_QLO_ALLOCATE;             break;
                        case T_8_CASCADED:         tk.m_kindGlobal = T_QLO_CASCADED;             break;
                        case T_8_CONSTANT:         tk.m_kindGlobal = T_QLO_CONSTANT;             break;
                        case T_8_CONTINUE:         tk.m_kindGlobal = T_QLO_CONTINUE;             break;
                        case T_8_DEFERRED:         tk.m_kindGlobal = T_QLO_DEFERRED;             break;
                        case T_8_DESCRIBE:         tk.m_kindGlobal = T_QLO_DESCRIBE;             break;
                        case T_8_DISTINCT:         tk.m_kindGlobal = T_QLO_DISTINCT;             break;
                        case T_8_EXTERNAL:         tk.m_kindGlobal = T_QLO_EXTERNAL;             break;
                        case T_8_FUNCTION:         tk.m_kindGlobal = T_QLO_FUNCTION;             break;
                        case T_8_GROUPING:         tk.m_kindGlobal = T_QLO_GROUPING;             break;
                        case T_8_IDENTITY:         tk.m_kindGlobal = T_QLO_IDENTITY;             break;
                        case T_8_INTERVAL:         tk.m_kindGlobal = T_QLO_INTERVAL;             break;
                        case T_8_LANGUAGE:         tk.m_kindGlobal = T_QLO_LANGUAGE;             break;
                        case T_8_MODIFIES:         tk.m_kindGlobal = T_QLO_MODIFIES;             break;
                        case T_8_OVERLAPS:         tk.m_kindGlobal = T_QLO_OVERLAPS;             break;
                        case T_8_PRESERVE:         tk.m_kindGlobal = T_QLO_PRESERVE;             break;
                        case T_8_RELATIVE:         tk.m_kindGlobal = T_QLO_RELATIVE;             break;
                        case T_8_RESIGNAL:         tk.m_kindGlobal = T_QLO_RESIGNAL;             break;
                        case T_8_RESTRICT:         tk.m_kindGlobal = T_QLO_RESTRICT;             break;
                        case T_8_ROLLBACK:         tk.m_kindGlobal = T_QLO_ROLLBACK;             break;
                        case T_8_SPECIFIC:         tk.m_kindGlobal = T_QLO_SPECIFIC;             break;
                        case T_8_SQLSTATE:         tk.m_kindGlobal = T_QLO_SQLSTATE;             break;
                        case T_8_TRAILING:         tk.m_kindGlobal = T_QLO_TRAILING;             break;
                        case T_8_TRUNCATE:         tk.m_kindGlobal = T_QLO_TRUNCATE;             break;
                        case T_8_WHENEVER:         tk.m_kindGlobal = T_QLO_WHENEVER;             break;
                        case T_9_ASSERTION:        tk.m_kindGlobal = T_QLO_ASSERTION;            break;
                        case T_9_COLLATION:        tk.m_kindGlobal = T_QLO_COLLATION;            break;
                        case T_9_CONDITION:        tk.m_kindGlobal = T_QLO_CONDITION;            break;
                        case T_9_EXCEPTION:        tk.m_kindGlobal = T_QLO_EXCEPTION;            break;
                        case T_9_IMMEDIATE:        tk.m_kindGlobal = T_QLO_IMMEDIATE;            break;
                        case T_9_INDICATOR:        tk.m_kindGlobal = T_QLO_INDICATOR;            break;
                        case T_9_INITIALLY:        tk.m_kindGlobal = T_QLO_INITIALLY;            break;
                        case T_9_INTERSECT:        tk.m_kindGlobal = T_QLO_INTERSECT;            break;
                        case T_9_ISOLATION:        tk.m_kindGlobal = T_QLO_ISOLATION;            break;
                        case T_9_LOCALTIME:        tk.m_kindGlobal = T_QLO_LOCALTIME;            break;
                        case T_9_PARAMETER:        tk.m_kindGlobal = T_QLO_PARAMETER;            break;
                        case T_9_PARTITION:        tk.m_kindGlobal = T_QLO_PARTITION;            break;
                        case T_9_PIPELINED:        tk.m_kindGlobal = T_QLO_PIPELINED;            break;
                        case T_9_PRECISION:        tk.m_kindGlobal = T_QLO_PRECISION;            break;
                        case T_9_PROCEDURE:        tk.m_kindGlobal = T_QLO_PROCEDURE;            break;
                        case T_9_RECURSIVE:        tk.m_kindGlobal = T_QLO_RECURSIVE;            break;
                        case T_9_RELIES_ON:        tk.m_kindGlobal = T_QLO_RELIES_ON;            break;
                        case T_9_RETURNING:        tk.m_kindGlobal = T_QLO_RETURNING;            break;
                        case T_9_SAVEPOINT:        tk.m_kindGlobal = T_QLO_SAVEPOINT;            break;
                        case T_9_SENSITIVE:        tk.m_kindGlobal = T_QLO_SENSITIVE;            break;
                        case T_9_STATEMENT:        tk.m_kindGlobal = T_QLO_STATEMENT;            break;
                        case T_9_SYMMETRIC:        tk.m_kindGlobal = T_QLO_SYMMETRIC;            break;
                        case T_9_TEMPORARY:        tk.m_kindGlobal = T_QLO_TEMPORARY;            break;
                        case T_9_TIMESTAMP:        tk.m_kindGlobal = T_QLO_TIMESTAMP;            break;
                        case T_10_ASENSITIVE:      tk.m_kindGlobal = T_QLO_ASENSITIVE;           break;
                        case T_10_ASYMMETRIC:      tk.m_kindGlobal = T_QLO_ASYMMETRIC;           break;
                        case T_10_CONNECTION:      tk.m_kindGlobal = T_QLO_CONNECTION;           break;
                        case T_10_CONSTRAINT:      tk.m_kindGlobal = T_QLO_CONSTRAINT;           break;
                        case T_10_DEALLOCATE:      tk.m_kindGlobal = T_QLO_DEALLOCATE;           break;
                        case T_10_DEFERRABLE:      tk.m_kindGlobal = T_QLO_DEFERRABLE;           break;
                        case T_10_DESCRIPTOR:      tk.m_kindGlobal = T_QLO_DESCRIPTOR;           break;
                        case T_10_DISCONNECT:      tk.m_kindGlobal = T_QLO_DISCONNECT;           break;
                        case T_10_ORDINALITY:      tk.m_kindGlobal = T_QLO_ORDINALITY;           break;
                        case T_10_PRIVILEGES:      tk.m_kindGlobal = T_QLO_PRIVILEGES;           break;
                        case T_10_REFERENCES:      tk.m_kindGlobal = T_QLO_REFERENCES;           break;
                        case T_10_SQLWARNING:      tk.m_kindGlobal = T_QLO_SQLWARNING;           break;
                        case T_11_CONSTRAINTS:     tk.m_kindGlobal = T_QLO_CONSTRAINTS;          break;
                        case T_11_CONSTRUCTOR:     tk.m_kindGlobal = T_QLO_CONSTRUCTOR;          break;
                        case T_11_DIAGNOSTICS:     tk.m_kindGlobal = T_QLO_DIAGNOSTICS;          break;
                        case T_11_INSENSITIVE:     tk.m_kindGlobal = T_QLO_INSENSITIVE;          break;
                        case T_11_REFERENCING:     tk.m_kindGlobal = T_QLO_REFERENCING;          break;
                        case T_11_SYSTEM_USER:     tk.m_kindGlobal = T_QLO_SYSTEM_USER;          break;
                        case T_11_TRANSACTION:     tk.m_kindGlobal = T_QLO_TRANSACTION;          break;
                        case T_11_TRANSLATION:     tk.m_kindGlobal = T_QLO_TRANSLATION;          break;
                        case T_12_CURRENT_DATE:    tk.m_kindGlobal = T_QLO_CURRENT_DATE;         break;
                        case T_12_CURRENT_PATH:    tk.m_kindGlobal = T_QLO_CURRENT_PATH;         break;
                        case T_12_CURRENT_ROLE:    tk.m_kindGlobal = T_QLO_CURRENT_ROLE;         break;
                        case T_12_CURRENT_TIME:    tk.m_kindGlobal = T_QLO_CURRENT_TIME;         break;
                        case T_12_CURRENT_USER:    tk.m_kindGlobal = T_QLO_CURRENT_USER;         break;
                        case T_12_RESULT_CACHE:    tk.m_kindGlobal = T_QLO_RESULT_CACHE;         break;
                        case T_12_SESSION_USER:    tk.m_kindGlobal = T_QLO_SESSION_USER;         break;
                        case T_12_SPECIFICTYPE:    tk.m_kindGlobal = T_QLO_SPECIFICTYPE;         break;
                        case T_12_SQLEXCEPTION:    tk.m_kindGlobal = T_QLO_SQLEXCEPTION;         break;
                        case T_13_AUTHORIZATION:   tk.m_kindGlobal = T_QLO_AUTHORIZATION;        break;
                        case T_13_CORRESPONDING:   tk.m_kindGlobal = T_QLO_CORRESPONDING;        break;
                        case T_13_DETERMINISTIC:   tk.m_kindGlobal = T_QLO_DETERMINISTIC;        break;
                        case T_13_TIMEZONE_HOUR:   tk.m_kindGlobal = T_QLO_TIMEZONE_HOUR;        break;
                        case T_14_LOCALTIMESTAMP:  tk.m_kindGlobal = T_QLO_LOCALTIMESTAMP;       break;
                        case T_15_TIMEZONE_MINUTE: tk.m_kindGlobal = T_QLO_TIMEZONE_MINUTE;      break;
                        case T_17_CURRENT_TIMESTAMP: tk.m_kindGlobal = T_QLO_CURRENT_TIMESTAMP;  break;
                        default: tk.m_kindGlobal = T_QLO_IDENTIFIER;                             break;
                    }
                } else if ( int id2 = tk.keyid2() ) {
                    switch ( id2 ) {
                        case T_3_AVG:
                        case T_3_MAX:
                        case T_3_MIN:
                        case T_3_SUM:
                        case T_5_COUNT:
                            tk.m_kindGlobal = T_QLO_GROUP_FUNC;
                            break;
                        default:
                            tk.m_kindGlobal = T_QLO_BUILTIN_FUNC;
                            break;
                    }
                } else if ( int id3 = _tokens->at(i).keyid3() ) {
                    switch ( id3 ) {
                        case T_3_BIT:       tk.m_kindGlobal = T_QLO_BIT;          break;
                        case T_3_INT:       tk.m_kindGlobal = T_QLO_INT;          break;
                        case T_4_BLOB:      tk.m_kindGlobal = T_QLO_BLOB;         break;
                        case T_4_CHAR:      tk.m_kindGlobal = T_QLO_CHAR;         break;
                        case T_4_CLOB:      tk.m_kindGlobal = T_QLO_CLOB;         break;
                        case T_4_DATE:      tk.m_kindGlobal = T_QLO_DATE;         break;
                        case T_4_YEAR:      tk.m_kindGlobal = T_QLO_YEAR;         break;
                        case T_4_ZONE:      tk.m_kindGlobal = T_QLO_ZONE;         break;
                        case T_5_FLOAT:     tk.m_kindGlobal = T_QLO_FLOAT;        break;
                        case T_5_NCHAR:     tk.m_kindGlobal = T_QLO_NCHAR;        break;
                        case T_5_NCLOB:     tk.m_kindGlobal = T_QLO_NCLOB;        break;
                        case T_6_BINARY:    tk.m_kindGlobal = T_QLO_BINARY;       break;
                        case T_6_DOUBLE:    tk.m_kindGlobal = T_QLO_DOUBLE;       break;
                        case T_6_OBJECT:    tk.m_kindGlobal = T_QLO_OBJECT;       break;
                        case T_7_BOOLEAN:   tk.m_kindGlobal = T_QLO_BOOLEAN;      break;
                        case T_7_DECIMAL:   tk.m_kindGlobal = T_QLO_DECIMAL;      break;
                        case T_7_INTEGER:   tk.m_kindGlobal = T_QLO_INTEGER;      break;
                        case T_7_NATURAL:   tk.m_kindGlobal = T_QLO_NATURAL;      break;
                        case T_7_NUMERIC:   tk.m_kindGlobal = T_QLO_NUMERIC;      break;
                        case T_7_VARCHAR:   tk.m_kindGlobal = T_QLO_VARCHAR;      break;
                        case T_8_NATIONAL:  tk.m_kindGlobal = T_QLO_NATIONAL;     break;
                        case T_8_SMALLINT:  tk.m_kindGlobal = T_QLO_SMALLINT;     break;
                        case T_8_VARCHAR2:  tk.m_kindGlobal = T_QLO_VARCHAR2;     break;
                        case T_9_CHARACTER: tk.m_kindGlobal = T_QLO_CHARARACTER;  break;
                        default:            tk.m_kindGlobal = T_QLO_IDENTIFIER;   break;
                    }
                } else {
                    tk.m_kindGlobal = T_QLO_IDENTIFIER;
                }
                break;
            }
            case Sql::T_NUMERIC_LITERAL:  tk.m_kindGlobal = T_QLO_NUMBER;      break;
            case Sql::T_CHAR_LITERAL:     tk.m_kindGlobal = T_QLO_CHARSTR;     break;
            case Sql::T_STRING_LITERAL:   tk.m_kindGlobal = T_QLO_STRSTR;      break;
            
            case Sql::T_AMPER:         tk.m_kindGlobal = T_QLO_AMPER;          break;
            case Sql::T_CARET:         tk.m_kindGlobal = T_QLO_CARET;          break;
            case Sql::T_COLON:         tk.m_kindGlobal = T_QLO_COLON;          break;
            case Sql::T_ASSIGN:        tk.m_kindGlobal = T_QLO_ASSIGN;         break;
            case Sql::T_COMMA:         tk.m_kindGlobal = T_QLO_COMMA;          break;
            case Sql::T_SLASH:         tk.m_kindGlobal = T_QLO_SLASH;          break;
            case Sql::T_DOT:           tk.m_kindGlobal = T_QLO_DOT;            break;
            case Sql::T_DOTDOT:        tk.m_kindGlobal = T_QLO_DOTDOT;         break;
            case Sql::T_EQUAL:         tk.m_kindGlobal = T_QLO_EQUAL;          break;
            case Sql::T_EXCLAIM:       tk.m_kindGlobal = T_QLO_EXCLAIM;        break;
            case Sql::T_EXCLAIM_EQUAL: tk.m_kindGlobal = T_QLO_EXCLAIM_EQUAL;  break;
            case Sql::T_GREATER:       tk.m_kindGlobal = T_QLO_GREATER;        break;
            case Sql::T_GREATER_EQUAL: tk.m_kindGlobal = T_QLO_GREATER_EQUAL;  break;
            case Sql::T_LBRACE:        tk.m_kindGlobal = T_QLO_LEFT_BRACE;     break;
            case Sql::T_LBRACKET:      tk.m_kindGlobal = T_QLO_LEFT_BRACKET;   break;
            case Sql::T_LESS:          tk.m_kindGlobal = T_QLO_LESS;           break;
            case Sql::T_LESS_EQUAL:    tk.m_kindGlobal = T_QLO_LESS_EQUAL;     break;
            case Sql::T_LESS_GREATER:  tk.m_kindGlobal = T_QLO_LESS_GREATER;   break;
            case Sql::T_LPAREN:        tk.m_kindGlobal = T_QLO_LEFT_PAREN;     break;
            case Sql::T_MINUS:         tk.m_kindGlobal = T_QLO_DASH;           break;
            case Sql::T_PERCENT:       tk.m_kindGlobal = T_QLO_PERCENT;        break;
            case Sql::T_PIPE:          tk.m_kindGlobal = T_QLO_PIPE;           break;
            case Sql::T_PIPE_PIPE:     tk.m_kindGlobal = T_QLO_PIPE_PIPE;      break;
            case Sql::T_PLUS:          tk.m_kindGlobal = T_QLO_PLUS;           break;
            case Sql::T_RBRACE:        tk.m_kindGlobal = T_QLO_RIGHT_BRACE;    break;
            case Sql::T_RBRACKET:      tk.m_kindGlobal = T_QLO_RIGHT_BRACKET;  break;
            case Sql::T_RPAREN:        tk.m_kindGlobal = T_QLO_RIGHT_PAREN;    break;
            case Sql::T_SEMICOLON:     tk.m_kindGlobal = T_QLO_SEMICOLON;      break;
            case Sql::T_STAR:          tk.m_kindGlobal = T_QLO_STAR;           break;
            default:              tk.m_kindGlobal = T_QLO_ERROR;          break;
        }
    }

    _index = 0;
}

Parser::~Parser()
{
}

AST *Parser::parse(int startToken)
{
    int action = 0;
    yytoken = -1;
    yyloc = -1;
    void *yyval = 0; // value of the current token.

    _recovered = false;
    _tos = -1;
    _startToken.m_kindGlobal = startToken;

    do {
    again:
        if (unsigned(++_tos) == _stateStack.size()) {
            _stateStack.resize(_tos * 2);
            _locationStack.resize(_tos * 2);
            _symStack.resize(_tos * 2);
        }

        _stateStack[_tos] = action;

        if (yytoken == -1 && -TERMINAL_COUNT != action_index[action]) {
            yyloc = consumeToken();
            yytoken = tokenKind(yyloc);
            if (yyrecovering)
                --yyrecovering;
            yyval = _tokens->at(yyloc).ptr;
        }

        action = t_action(action, yytoken);
        if (action > 0) {
            if (action == ACCEPT_STATE) {
                --_tos;
                return _symStack[0].translation_unit;
            }
            _symStack[_tos].ptr = yyval;
            _locationStack[_tos] = yyloc;
            yytoken = -1;
        } else if (action < 0) {
            const int ruleno = -action - 1;
            const int N = rhs[ruleno];
            _tos -= N;
            reduce(ruleno);
            action = nt_action(_stateStack[_tos], lhs[ruleno] - TERMINAL_COUNT);
        } else if (action == 0) {
            // const int line = _tokens->at(yyloc).line + 1;
            QString message = QLatin1String("Syntax error");
            if (yytoken != -1) {
                const QLatin1String s(spell[yytoken]);
                message = QString::fromLatin1("Unexpected token `%1'").arg(s);
            }

            for (; _tos; --_tos) {
                const int state = _stateStack[_tos];

                static int tks[] = {
                    T_QLO_RIGHT_BRACE, T_QLO_RIGHT_PAREN, T_QLO_RIGHT_BRACKET,
                    T_QLO_SEMICOLON, T_QLO_COLON, T_QLO_COMMA,
                    T_QLO_NUMBER, T_QLO_IDENTIFIER,
                    T_QLO_LEFT_BRACE, T_QLO_LEFT_PAREN, T_QLO_LEFT_BRACKET,
                    T_QLO_WHILE,
                    0
                };

                for (int *tptr = tks; *tptr; ++tptr) {
                    const int next = t_action(state, *tptr);
                    if (next > 0) {
                        if (! yyrecovering && ! _recovered) {
                            _recovered = true;
                            error(yyloc, QString::fromLatin1("Expected `%1'").arg(QLatin1String(spell[*tptr])));
                        }

                        yyrecovering = 3;
                        if (*tptr == T_QLO_IDENTIFIER)
                            yyval = (void *) _tokens->at(yyloc).identifier;
                        /* else if (*tptr == T_QLO_NUMBER)
                            yyval = (void *) _engine->identifier(QLatin1String("$0")); */
                        else
                            yyval = 0;

                        _symStack[_tos].ptr = yyval;
                        _locationStack[_tos] = yyloc;
                        yytoken = -1;

                        action = next;
                        goto again;
                    }
                }
            }

            if (! _recovered) {
                _recovered = true;
                error(yyloc, message);
            }
        }

    } while (action);

    return 0;
}
./

/.
void Parser::reduce(int ruleno)
{
switch(ruleno) {
./

column_name ::= IDENTIFIER ;
/.
case $rule_number: { name(1) = makeName<ColumnNameAST>(); } break;
./

table_name ::= IDENTIFIER ;
/.
case $rule_number: { name(1) = makeName<TableNameAST>(); } break;
./

schema_name ::= IDENTIFIER ;
/.
case $rule_number: { name(1) = makeName<SchemaNameAST>(); } break;
./

funcion_name ::= IDENTIFIER ;
/.
case $rule_number: { name(1) = makeName<FunctionNameAST>(); } break;
./

column_identifier ::= column_name ;
/.
case $rule_number: { /*ast(1) = makeAstNode<ColumnIdentifierAST>(0, 0, name(1));*/ } break;
./

column_identifier ::= table_name DOT column_name;
/.
case $rule_number: { /*ast(1) = makeAstNode<ColumnIdentifierAST>(0, name(1), name(3));*/ } break;
./

column_identifier ::= schema_name DOT table_name DOT column_name;
/.
case $rule_number: { /*ast(1) = makeAstNode<ColumnIdentifierAST>(name(1), name(3), name(5));*/ } break;
./

primary_expression ::= simple_exp ;
/.
case $rule_number: { ast(1) = makeAstNode<LiteralExpressionAST>(location(1)); } break;
./

primary_expression ::= TRUE ;
/.
case $rule_number: { ast(1) = makeAstNode<LiteralExpressionAST>(location(1)); } break;
./

primary_expression ::= FALSE ;
/.
case $rule_number: { ast(1) = makeAstNode<LiteralExpressionAST>(location(1)); } break;
./

group_func_specification ::= empty ;
/.
case $rule_number: {} break;
./

group_func_specification ::= STAR ;
/.
case $rule_number: {} break;
./

group_func_specification ::= ALL ;
/.
case $rule_number: {} break;
./

group_func_specification ::= DISTINCT ;
/.
case $rule_number: {} break;
./

primary_expression ::= GROUP_FUNC LEFT_PAREN group_func_specification expression RIGHT_PAREN ;
/.
case $rule_number: { /*ast(1) = makeAstNode<GroupFunctionCallAST>(expression(4));*/ } break;
./

comma_expression_list ::= empty ;
/.
case $rule_number: {} break;
./

comma_expression_list ::= expression ;
/.
case $rule_number: {
    if ( sym(1).expression ) {
        sym(1).expression_list = makeAstNode< List<ValueExpressionAST *> >(sym(1).expression);
    } else {
        sym(1).expression_list = 0;
    }
} break;
./

comma_expression_list ::= comma_expression_list COMMA expression ;
/.
case $rule_number: {
    if ( sym(1).expression_list && sym(3).expression ) {
        sym(1).expression_list = makeAstNode< List<ValueExpressionAST *> >(sym(1).expression_list, sym(3).expression);
    } else if ( !sym(1).expression_list ) {
        if ( sym(3).expression ) {
            sym(1).expression_list = makeAstNode< List<ValueExpressionAST *> >(sym(3).expression);
        } else {
            sym(1).expression_list = 0;
        }
    }
} break;
./

primary_expression ::= BUILTIN_FUNC LEFT_PAREN comma_expression_list RIGHT_PAREN ;
/.
case $rule_number: { /*ast(1) = makeAstNode<BuiltinFunctionCallAST>(sym(3).expression_list);*/ } break;
./

primary_expression ::= IDENTIFIER LEFT_PAREN comma_expression_list RIGHT_PAREN ;
/.
case $rule_number: { /*ast(1) = makeAstNode<FunctionCallAST>(0, 0, name(1), sym(3).expression_list);*/ } break;
./

primary_expression ::= IDENTIFIER DOT IDENTIFIER LEFT_PAREN comma_expression_list RIGHT_PAREN ;
/.
case $rule_number: { /*ast(1) = makeAstNode<FunctionCallAST>(0, name(1), name(3), sym(5).expression_list);*/ } break;
./

primary_expression ::= IDENTIFIER DOT IDENTIFIER DOT IDENTIFIER LEFT_PAREN comma_expression_list RIGHT_PAREN ;
/.
case $rule_number: { /*ast(1) = makeAstNode<FunctionCallAST>(name(1), name(3), name(5), sym(7).expression_list);*/ } break;
./

primary_expression ::= column_identifier ;
/.
case $rule_number: {} break;
./

primary_expression ::= LEFT_PAREN expression RIGHT_PAREN ;
/.
case $rule_number: { ast(1) = ast(2); } break;
./

unary_expression ::= primary_expression ;
/.
case $rule_number: {} break;
./

unary_expression ::= unary_operator primary_expression ;
/.
case $rule_number: { /*ast(1) = makeAstNode<UnaryExpressionAST>(sym(1).kind, expression(2));*/ } break;
./

unary_operator ::= PLUS ;
/.
case $rule_number: { sym(1).kind = AST::Kind_UnaryPlus; } break;
./

unary_operator ::= DASH ;
/.
case $rule_number: { sym(1).kind = AST::Kind_UnaryMinus; } break;
./

multiplicative_expression ::= unary_expression ;
/.
case $rule_number: {} break;
./

multiplicative_expression ::= multiplicative_expression STAR unary_expression ;
/.
case $rule_number: { /*ast(1) = makeAstNode<BinaryExpressionAST>(AST::Kind_Multiply, expression(1), expression(3));*/ } break;
./

multiplicative_expression ::= multiplicative_expression SLASH unary_expression ;
/.
case $rule_number: { /*ast(1) = makeAstNode<BinaryExpressionAST>(AST::Kind_Divide, expression(1), expression(3));*/ } break;
./

additive_expression ::= multiplicative_expression ;
/.
case $rule_number: {} break;
./

additive_expression ::= additive_expression PLUS multiplicative_expression ;
/.
case $rule_number: { /*ast(1) = makeAstNode<BinaryExpressionAST>(AST::Kind_Plus, expression(1), expression(3));*/ } break;
./

additive_expression ::= additive_expression DASH multiplicative_expression ;
/.
case $rule_number: { /*ast(1) = makeAstNode<BinaryExpressionAST>(AST::Kind_Minus, expression(1), expression(3));*/ } break;
./

additive_expression ::= additive_expression PIPE_PIPE multiplicative_expression ;
/.
case $rule_number: { /*ast(1) = makeAstNode<BinaryExpressionAST>(AST::Kind_PipePipe, expression(1), expression(3));*/ } break;
./

expression ::= additive_expression ;
/.
case $rule_number: {} break;
./

statement ::= SEMICOLON ;
/.
case $rule_number: { ast(1) = 0; } break;
./

select_columns ::= STAR ;
/.
case $rule_number: { ast(1) = makeAstNode<StarSelectColumnAST>(location(1)); } break;
./

select_columns_item ::= table_identifier DOT STAR ;
/.
case $rule_number: { select_column(1) = makeAstNode<TableSelectColumnsItemAST>(table_ident(1), location(2), location(3)); } break;
./

select_columns_item ::= expression AS IDENTIFIER;
/.
case $rule_number: { ast(1) = makeAstNode<ValueSelectColumnItemAST>(expression(1), location(2), makeName<ColumnAliasAST>(location(3))); } break;
./

select_columns_item ::= expression IDENTIFIER;
/.
case $rule_number: { ast(1) = makeAstNode<ValueSelectColumnItemAST>(expression(1), 0, makeName<ColumnAliasAST>(location(2))); } break;
./

select_columns_item ::= expression ;
/.
case $rule_number: { ast(1) = makeAstNode<ValueSelectColumnItemAST>(expression(1), 0, (ColumnAliasAST *)0); } break;
./

list_select_columns ::= list_select_columns COMMA select_columns_item ;
/.
case $rule_number: {
    if ( sym(1).sel_col_list && sym(3).sel_col ) {
        sym(1).sel_col_list = makeAstNode< List<BaseSelectColumnItemAST *> >(sym(1).sel_col_list, sym(3).sel_col);
    } else if ( !sym(1).sel_col_list ) {
        if ( sym(3).sel_col ) {
            sym(1).sel_col_list = makeAstNode< List<BaseSelectColumnItemAST *> >(sym(3).sel_col);
        } else {
            sym(1).sel_col_list = 0;
        }
    }
} break;
./

list_select_columns ::= select_columns_item ;
/.
case $rule_number: {
    if ( sym(1).sel_col ) {
        sym(1).sel_col_list = makeAstNode< List<BaseSelectColumnItemAST *> >(sym(1).sel_col);
    } else {
        sym(1).sel_col_list = 0;
    }
} break;
./

select_columns ::= list_select_columns ;
/.
case $rule_number: { ast(1) = makeAstNode<ListSelectColumnAST>(sym(1).sel_col_list); } break;
./

table_identifier ::= table_name ;
/.
case $rule_number: { /*ast(1) = makeAstNode<TableIdentifierAST>(0, name(1));*/ } break;
./

table_identifier ::= schema_name DOT table_name;
/.
case $rule_number: { /*ast(1) = makeAstNode<TableIdentifierAST>(name(1), name(3));*/ } break;
./

simple_exp ::= NUMBER ;
simple_exp ::= CHARSTR ;

simple_exp_comma_list ::= simple_exp COMMA simple_exp_comma_list ;
simple_exp_comma_list ::= simple_exp ;

expression_set ::= LEFT_PAREN select_statement RIGHT_PAREN ;
expression_set ::= LEFT_PAREN simple_exp_comma_list RIGHT_PAREN ;
expression_set ::= NUMBER ;
expression_set ::= CHARSTR ;

compare_operation ::= EQUAL ;
compare_operation ::= LESS ;
compare_operation ::= GREATER ;
compare_operation ::= EXCLAIM_EQUAL ;
compare_operation ::= LESS_GREATER ;
compare_operation ::= LESS_EQUAL ;
compare_operation ::= GREATER_EQUAL ;

logical_factor ::= NOT EXISTS LEFT_PAREN select_statement RIGHT_PAREN ;
logical_factor ::=     EXISTS LEFT_PAREN select_statement RIGHT_PAREN ;
logical_factor ::= LEFT_PAREN condition RIGHT_PAREN ;
logical_factor ::= expression NOT IN expression_set ;
logical_factor ::= expression     IN expression_set ;
logical_factor ::= expression NOT LIKE CHARSTR ;
logical_factor ::= expression     LIKE CHARSTR ;
logical_factor ::= expression NOT BETWEEN expression AND expression ;
logical_factor ::= expression     BETWEEN expression AND expression ;
logical_factor ::= expression IS NOT NULL ;
logical_factor ::= expression IS     NULL ;
logical_factor ::= expression compare_operation expression ;
logical_factor ::= expression compare_operation ALL LEFT_PAREN select_statement RIGHT_PAREN ;
logical_factor ::= expression compare_operation ANY LEFT_PAREN select_statement RIGHT_PAREN ;
logical_factor ::= expression compare_operation     LEFT_PAREN select_statement RIGHT_PAREN ;

logical_factor_list ::= logical_factor AND logical_factor_list ;
logical_factor_list ::= logical_factor ;

logical_term_list ::= logical_factor_list OR logical_term_list ;
logical_term_list ::= logical_factor_list ;

condition ::= NOT logical_term_list ;
condition ::= logical_term_list ;

join_condition ::= ON condition ;
/.
case $rule_number: { ast(1) = makeAstNode<JoinConditionAST>(location(1), ast(2)->asConditionExpression()); } break;
./

join_source_list ::= COMMA single_select_source join_source_list ;
join_source_list ::=                      JOIN single_select_source join_condition join_source_list ;
join_source_list ::=         INNER        JOIN single_select_source join_condition join_source_list ;
join_source_list ::=         OUTER        JOIN single_select_source join_condition join_source_list ;
join_source_list ::=                 LEFT JOIN single_select_source join_condition join_source_list ;
join_source_list ::=                RIGHT JOIN single_select_source join_condition join_source_list ;
join_source_list ::=         INNER   LEFT JOIN single_select_source join_condition join_source_list ;
join_source_list ::=         INNER  RIGHT JOIN single_select_source join_condition join_source_list ;
join_source_list ::=         OUTER   LEFT JOIN single_select_source join_condition join_source_list ;
join_source_list ::=         OUTER  RIGHT JOIN single_select_source join_condition join_source_list ;
join_source_list ::=                 FULL JOIN single_select_source                join_source_list ;
join_source_list ::=                CROSS JOIN single_select_source                join_source_list ;
join_source_list ::= LEFT_PAREN join_source_list RIGHT_PAREN ;
/.
case $rule_number: { ast(1) = ast(2); } break;
./
join_source_list ::= empty ;
/.
case $rule_number: {} break;
./

select_source ::= single_select_source join_source_list ;
/.
case $rule_number: {} break;
./

single_select_source ::= table_identifier AS IDENTIFIER ;
/.
case $rule_number: { ast(1) = makeAstNode<DirectSingleSourceAST>(table_ident(1), location(2), makeName<TableAliasAST>(location(3))); } break;
./

single_select_source ::= table_identifier IDENTIFIER ;
/.
case $rule_number: { ast(1) = makeAstNode<DirectSingleSourceAST>(table_ident(1), 0, makeName<TableAliasAST>(location(2))); } break;
./

single_select_source ::= table_identifier ;
/.
case $rule_number: { ast(1) = makeAstNode<DirectSingleSourceAST>(table_ident(1), 0, (TableAliasAST *)0); } break;
./

single_select_source ::= LEFT_PAREN select_statement RIGHT_PAREN AS IDENTIFIER ;
/.
case $rule_number: { ast(1) = makeAstNode<SubQuerySingleSourceAST>(location(1), statement(2)->asSelectStatement(), location(3), location(4), makeName<TableAliasAST>(location(5))); } break;
./

single_select_source ::= LEFT_PAREN select_statement RIGHT_PAREN IDENTIFIER ;
/.
case $rule_number: { ast(1) = makeAstNode<SubQuerySingleSourceAST>(location(1), statement(2)->asSelectStatement(), location(3), 0, makeName<TableAliasAST>(location(4))); } break;
./

single_select_source ::= LEFT_PAREN select_statement RIGHT_PAREN ;
/.
case $rule_number: { ast(1) = makeAstNode<SubQuerySingleSourceAST>(location(1), statement(2)->asSelectStatement(), location(3), 0, (TableAliasAST *)0); } break;
./

where_clause ::= WHERE condition ;

where_clause ::= empty ;
/.
case $rule_number: {} break;
./

start_with_clause ::= START WITH condition ;
start_with_clause ::= empty ;

prior_connect_clause ::= PRIOR expression compare_operation expression ;
connect_prior_clause ::= expression compare_operation expression PRIOR ;

connect_clause ::= CONNECT BY prior_connect_clause start_with_clause ;
connect_clause ::= CONNECT BY connect_prior_clause start_with_clause ;

connect_clause ::= empty ;
/.
case $rule_number: {} break;
./

group_clause ::= GROUP BY comma_expression_list ;
group_clause ::= GROUP BY comma_expression_list HAVING condition ;
group_clause ::= empty ;
/.
case $rule_number: {} break;
./

set_clause ::= UNION ALL select_core_item ;
set_clause ::= UNION DISTINCT select_core_item ;
set_clause ::= UNION select_core_item ;
set_clause ::= INTERSECT select_core_item ;
set_clause ::= MINUS select_core_item ;

set_clause ::= empty ;
/.
case $rule_number: {} break;
./

ordered_expression ::= expression ASC ;
/.
case $rule_number: {} break;
./

ordered_expression ::= expression DESC ;
/.
case $rule_number: {} break;
./

ordered_expression ::= expression ;
/.
case $rule_number: {} break;
./

comma_ordered_expression_list ::= comma_ordered_expression_list COMMA ordered_expression ;
/.
case $rule_number: {
    if ( sym(1).expression_list && sym(3).expression ) {
        sym(1).expression_list = makeAstNode< List<ValueExpressionAST *> >(sym(1).expression_list, sym(3).expression);
    } else if ( !sym(1).expression_list ) {
        if ( sym(3).expression ) {
            sym(1).expression_list = makeAstNode< List<ValueExpressionAST *> >(sym(3).expression);
        } else {
            sym(1).expression_list = 0;
        }
    }
} break;
./

comma_ordered_expression_list ::= ordered_expression ;
/.
case $rule_number: {
    if ( sym(1).expression ) {
        sym(1).expression_list = makeAstNode< List<ValueExpressionAST *> >(sym(1).expression);
    } else {
        sym(1).expression_list = 0;
    }
} break;
./

order_clause ::= ORDER BY comma_ordered_expression_list ;
/.
case $rule_number: { /*ast(1) = makeAstNode<OrderByBlockAST>(sym(3).expression_list);*/ } break;
./

order_clause ::= empty ;
/.
case $rule_number: {} break;
./

nowait_clause ::= NOWAIT ;
/.
case $rule_number: {} break;
./

nowait_clause ::= empty ;
/.
case $rule_number: {} break;
./

comma_column_identifier_list ::= comma_column_identifier_list COMMA column_identifier ;
/.
case $rule_number: {
    if ( sym(1).column_ident_list && sym(3).column_ident ) {
        sym(1).column_ident_list = makeAstNode< List<ColumnIdentifierAST *> >(sym(1).column_ident_list, sym(3).column_ident);
    } else if ( !sym(1).column_ident_list ) {
        if ( sym(3).column_ident ) {
            sym(1).column_ident_list = makeAstNode< List<ColumnIdentifierAST *> >(sym(3).column_ident);
        } else {
            sym(1).column_ident_list = 0;
        }
    }
} break;
./

comma_column_identifier_list ::= column_identifier ;
/.
case $rule_number: {
    if ( sym(1).column_ident ) {
        sym(1).column_ident_list = makeAstNode< List<ColumnIdentifierAST *> >(sym(1).column_ident);
    } else {
        sym(1).column_ident_list = 0;
    }
} break;
./

update_clause ::= FOR UPDATE OF comma_column_identifier_list nowait_clause ;
/.
case $rule_number: { ast(1) = makeAstNode<ForUpdateClauseAST>(location(1), location(2), location(3), sym(4).column_ident_list, location(5)); } break;
./

update_clause ::= empty ;
/.
case $rule_number: {} break;
./

select_spec ::= empty ;
/.
case $rule_number: {} break;
./

select_spec ::= ALL ;
/.
case $rule_number: {} break;
./

select_spec ::= DISTINCT ;
/.
case $rule_number: {} break;
./

translation_unit ::= toplevel_statement_list ;
/.
case $rule_number: { ast(1) = makeAstNode<TranslationUnitAST>(sym(1).statement_list); } break;
./

toplevel_statement_list ::= toplevel_statement ;
/.
case $rule_number: {
    if ( sym(1).statement ) {
        sym(1).statement_list = makeAstNode< List<StatementAST *> >(sym(1).statement);
    } else {
        sym(1).statement_list = 0;
    }
} break;
./

toplevel_statement_list ::= toplevel_statement_list toplevel_statement ;
/.
case $rule_number: {
    if ( sym(1).statement_list && sym(2).statement ) {
        sym(1).statement_list = makeAstNode< List<StatementAST *> >(sym(1).statement_list, sym(2).statement);
    } else if ( !sym(1).statement_list ) {
        if ( sym(2).statement ) {
            sym(1).statement_list = makeAstNode< List<StatementAST *> >(sym(2).statement);
        } else {
            sym(1).statement_list = 0;
        }
    }
} break;
./

toplevel_statement ::= select_statement SEMICOLON ;
/.
case $rule_number: {} break;
./

toplevel_statement ::= SEMICOLON ;
/.
case $rule_number: {} break;
./

select_statement ::= select_core_item order_clause update_clause ;
/.
case $rule_number: {
    ast(1) = makeAstNode<SelectStatementAST>( sym(1).ast->asSelectCore(),
                                              sym(2).ast->asOrderByBlock(),
                                              sym(3).ast->asForUpdateClause() );
} break;
./

select_core_item ::= SELECT select_spec select_columns FROM select_source where_clause connect_clause group_clause set_clause ;
/.
case $rule_number: {
    ast(1) = makeAstNode<SelectCoreAST>( location(1),                       /* SELECT token */
                                         location(2),                       /* ALL|DISTINCT token */
                                         sym(3).ast->asBaseSelectColumns(), /* select columns */
                                         location(4),                       /* FROM token */
                                         sym(5).ast->asJoinSource(),        /* select source */
                                         sym(6).ast->asWhereClause(),       /* where ... */
                                         sym(7).ast->asConnectByClause(),   /* connect by ...*/
                                         sym(8).ast->asGroupByBlock(),      /* group by ... */
                                         sym(9).ast->asSetClause()          /* UNION | INTERSECT | MINUS <select_core> */ );
} break;
./

empty ::= ;
/.
case $rule_number: { ast(1) = makeAstNode<EmptyAST>(); } break;
./

toplevel ::= FEED_ORACLE translation_unit ;
/.
case $rule_number: { ast(1) = ast(2); } break;
./

toplevel ::= FEED_EXPRESSION expression ;
/.
case $rule_number: { ast(1) = ast(2); } break;
./

/.
} // end switch
} // end Parser::reduce()
./
