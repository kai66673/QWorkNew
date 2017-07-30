
/********************************************************************************
** QLalr Parser Source
********************************************************************************/
#include "QLalrOracleParser.h"
#include "QLalrOracleKeywords.h"
#include "QLalrOracleSqlStatementsBindVisitor.h"
#include "QLalrOracleSqlCompletionAssistVisitor.h"
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
    
    // _tokens->erase(_tokens->begin() + _tokens->size() - 5, _tokens->begin() + _tokens->size());

    for (unsigned i = 1; i < _tokens->size(); i++) {
        Sql::Token &tk = _tokens->at(i);
        switch ( tk.kind() ) {
            case Sql::T_EOF_SYMBOL:  tk.m_kindGlobal = EOF_SYMBOL; break;
            case Sql::T_IDENTIFIER: tk.m_kindGlobal = T_QLO_IDENTIFIER; break;
            case Sql::T_NULL_IDENTIFIER: tk.m_kindGlobal = T_QLO_NULL_IDENTIFIER; break;
            case Sql::T_KEYWORD: {
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
                        case T_4_RELY:             tk.m_kindGlobal = T_QLO_RELY;                 break;
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
                        case T_6_BITMAP:           tk.m_kindGlobal = T_QLO_BITMAP;               break;
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
                        case T_6_NORELY:           tk.m_kindGlobal = T_QLO_NORELY;               break;
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
                            tk.m_kindGlobal = T_QLO_GROUP_FUNC;
                            break;
                        case T_5_COUNT:
                            tk.m_kindGlobal = T_QLO_COUNT;
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
                        case T_9_CHARACTER: tk.m_kindGlobal = T_QLO_CHARACTER;    break;
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

Sql::BaseWalker *Parser::parseDocument( Sql::ParseTarget target )
{
    switch ( target ) {
        case Sql::ParseSqlNames:{
            if ( TranslationUnitAST *node = /* parse() */ parseStatements() )
                return new SqlCheckNamesWalker(node, _translationUnit);
            }
            return 0;
    }

    return 0;
}

void Parser::fillSqlStatements( QList<Sql::SqlStatement *> *statements,
                                Database::ExecuteType executeType, ITextDocument *textDocument )
{
    if ( TranslationUnitAST *node = parseStatements() ) {
        SqlStatementsBindVisitor visitor(_translationUnit);
        visitor.fillStatementsList(statements, node, executeType, textDocument);
    }
}

void Parser::fillStatementFromText( QList<Sql::SqlStatement *> *statements, const QString &statementText )
{
    if ( TranslationUnitAST *node = parseStatements() ) {
        SqlStatementsBindVisitor visitor(_translationUnit);
        visitor.fillStatementFromText(statements, node, statementText);
    }
}

int Parser::fillCompletions( QList<TextEditor::BasicProposalItem *> *completions, unsigned position,
                             int startPosition, unsigned completionOperator )
{
    if ( TranslationUnitAST *node = parseStatements() ) {
        SqlCompletionAssistVisitor visitor(_translationUnit);
        return visitor.fillCompletions(completions, node, position, startPosition, completionOperator);
    }
    return -1;
}
TranslationUnitAST *Parser::parseStatements()
{
    int index = 1;
    StatementListAST *statements = 0;
    
    do {
        unsigned line;
        unsigned column;
        _translationUnit->getTokenStartPosition(index, &line, &column);
        
        _statementOffset = index - 1;
        _statementLength = 1;
        while ( _tokens->at(index).m_kindGlobal != T_QLO_SEMICOLON && 
                _tokens->at(index).m_kindGlobal != EOF_SYMBOL ) {
            _statementLength++;
            index++;
        }
        StatementAST *statement = parseOneStatement();
        if ( !statement ) {
            statement = makeAstNode<InvalidStatementAST>( _statementOffset + 1,
                                                          statementTokenKind(1) == T_QLO_SELECT );
        }
        statement->start_line = line;
        statement->start_pos = column;
        _translationUnit->getTokenEndPosition(index - 1, &line, &column);
        statement->end_line = line;
        if ( !line )
            column++;
        statement->end_pos = column;
        if ( statements )
            statements = makeAstNode<StatementListAST>(statements, statement);
        else
            statements = makeAstNode<StatementListAST>(statement);
            
        index++;
    } while ( _tokens->at(index).m_kindGlobal != EOF_SYMBOL );
    
    return makeAstNode<TranslationUnitAST>(statements);
}

StatementAST *Parser::parseOneStatement()
{
    int action = 0;
    yytoken = -1;
    yyloc = -1;
    void *yyval = 0; // value of the current token.

    _tos = -1;
    _index = 0;

    do {
        if (unsigned(++_tos) == _stateStack.size()) {
            _stateStack.resize(_tos * 2);
            _locationStack.resize(_tos * 2);
            _symStack.resize(_tos * 2);
        }

        _stateStack[_tos] = action;

        if (yytoken == -1 && -TERMINAL_COUNT != action_index[action]) {
            yyloc = statementConsumeToken();
            yytoken = statementTokenKind(yyloc);
            yyval = _tokens->at(yyloc + _statementOffset).ptr;
        }

        action = t_action(action, yytoken);
        if (action > 0) {
            if (action == ACCEPT_STATE) {
                --_tos;
                return _symStack[0].statement;
            }
            _symStack[_tos].ptr = yyval;
            _locationStack[_tos] = yyloc + _statementOffset;
            yytoken = -1;
        } else if (action < 0) {
            const int ruleno = -action - 1;
            const int N = rhs[ruleno];
            _tos -= N;
            reduce(ruleno);
            action = nt_action(_stateStack[_tos], lhs[ruleno] - TERMINAL_COUNT);
        } else if (action == 0) {
            QString message = QLatin1String("Syntax error");
            if (yytoken != -1) {
                const QLatin1String s(spell[yytoken]);
                message = QString::fromLatin1("Unexpected token `%1'").arg(s);
            }
            error(yyloc + _statementOffset, message);
            if ( _symStack[0].ast ) {
                if ( StatementAST *statement = _symStack[0].ast->asStatement() )
                    return statement;
            }
        }

    } while (action);
    
    return 0;
}

AST *Parser::parse(int startToken)
{
    int action = 0;
    yytoken = -1;
    yyloc = -1;
    void *yyval = 0; // value of the current token.

    _tos = -1;
    _startToken.m_kindGlobal = startToken;

    do {
        if (unsigned(++_tos) == _stateStack.size()) {
            _stateStack.resize(_tos * 2);
            _locationStack.resize(_tos * 2);
            _symStack.resize(_tos * 2);
        }

        _stateStack[_tos] = action;

        if (yytoken == -1 && -TERMINAL_COUNT != action_index[action]) {
            yyloc = consumeToken();
            yytoken = tokenKind(yyloc);
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
            QString message = QLatin1String("Syntax error");
            if (yytoken != -1) {
                const QLatin1String s(spell[yytoken]);
                message = QString::fromLatin1("Unexpected token `%1'").arg(s);
            }
            error(yyloc, message);
        }

    } while (action);

    return 0;
}

void Parser::reduce(int ruleno)
{
switch(ruleno) {

case 0: {} break;

case 1: { location(1) = 0; } break;

case 2:{
    NameAST *nameAst = 0;
    if ( location(3) )
        nameAst = createName(NameAST::Table, location(3));
    else
        error(location(2), "Identifier Expected");
    ast(1) = makeAstNode<TableIdentifierAST>( createName(NameAST::Schema, location(1)),
                                              location(2),
                                              nameAst );
} break;

case 3:{
    ast(1) = makeAstNode<TableIdentifierAST>( (NameAST *) 0, 0,
                                              createName(NameAST::Table, location(1)) );
} break;

case 4:{
    NameAST *nameAst = 0;
    if ( location(3) )
        nameAst = createName(NameAST::Index, location(3));
    else
        error(location(2), "Identifier Expected");
    ast(1) = makeAstNode<IndexIdentifierAST>( createName(NameAST::Schema, location(1)),
                                              location(2),
                                              nameAst );
} break;

case 5:{
    ast(1) = makeAstNode<IndexIdentifierAST>( (NameAST *) 0, 0,
                                              createName(NameAST::Index, location(1)) );
} break;

case 6:{
    NameAST *nameAst = 0;
    if ( location(3) )
        nameAst = createName(NameAST::Trigger, location(3));
    else
        error(location(2), "Identifier Expected");
    ast(1) = makeAstNode<TriggerIdentifierAST>( createName(NameAST::Schema, location(1)),
                                                location(2),
                                                nameAst );
} break;

case 7:{
    ast(1) = makeAstNode<TriggerIdentifierAST>( (NameAST *) 0, 0,
                                                createName(NameAST::Trigger, location(1)) );
} break;

case 8: { ast(1) = makeAstNode<JoinConditionAST>(location(1), condition(2)); } break;

case 9: { ast(1) = makeAstNode<DirectSingleSourceAST>( table_ident(1), createName(NameAST::TableAlias, location(2)) ); } break;

case 10: { ast(1) = makeAstNode<DirectSingleSourceAST>(table_ident(1), (NameAST *)0); } break;

case 11: {
    ast(1) = makeAstNode<SubQuerySingleSourceAST>( location(1),
                                                   statement(2)->asSelectStatement(),
                                                   location(3),
                                                   location(4),
                                                   createName(NameAST::TableAlias, location(5)) );
} break;

case 12: {
    ast(1) = makeAstNode<SubQuerySingleSourceAST>( location(1),
                                                   statement(2)->asSelectStatement(),
                                                   location(3), 0,
                                                   createName(NameAST::TableAlias, location(4)) );
} break;

case 13: { ast(1) = makeAstNode<SubQuerySingleSourceAST>(location(1), statement(2)->asSelectStatement(), location(3), 0, (NameAST *)0); } break;

case 14: {
    ast(1) = makeAstNode<JoinJoinListAST>( 0, 0, location(1),
                                           ast(2)->asSingleSource(),
                                           ast(3)->asJoinCondition(),
                                           ast(4)->asBaseJoinList() );
} break;

case 15: {
    ast(1) = makeAstNode<JoinJoinListAST>( location(1), 0, location(2),
                                           ast(3)->asSingleSource(),
                                           ast(4)->asJoinCondition(),
                                           ast(5)->asBaseJoinList() );
} break;

case 16: {
    ast(1) = makeAstNode<JoinJoinListAST>( location(1), 0, location(2),
                                           ast(3)->asSingleSource(),
                                           ast(4)->asJoinCondition(),
                                           ast(5)->asBaseJoinList() );
} break;

case 17: {
    ast(1) = makeAstNode<JoinJoinListAST>( 0, location(1), location(2),
                                           ast(3)->asSingleSource(),
                                           ast(4)->asJoinCondition(),
                                           ast(5)->asBaseJoinList() );
} break;

case 18: {
    ast(1) = makeAstNode<JoinJoinListAST>( 0, location(1), location(2),
                                           ast(3)->asSingleSource(),
                                           ast(4)->asJoinCondition(),
                                           ast(5)->asBaseJoinList() );
} break;

case 19: {
    ast(1) = makeAstNode<JoinJoinListAST>( location(1), location(2), location(3),
                                           ast(4)->asSingleSource(),
                                           ast(5)->asJoinCondition(),
                                           ast(6)->asBaseJoinList() );
} break;

case 20: {
    ast(1) = makeAstNode<JoinJoinListAST>( location(1), location(2), location(3),
                                           ast(4)->asSingleSource(),
                                           ast(5)->asJoinCondition(),
                                           ast(6)->asBaseJoinList() );
} break;

case 21: {
    ast(1) = makeAstNode<JoinJoinListAST>( location(1), location(2), location(3),
                                           ast(4)->asSingleSource(),
                                           ast(5)->asJoinCondition(),
                                           ast(6)->asBaseJoinList() );
} break;

case 22: {
    ast(1) = makeAstNode<JoinJoinListAST>( location(1), location(2), location(3),
                                           ast(4)->asSingleSource(),
                                           ast(5)->asJoinCondition(),
                                           ast(6)->asBaseJoinList() );
} break;

case 23: {
    ast(1) = makeAstNode<JoinJoinListAST>( 0, location(1), location(2),
                                           ast(3)->asSingleSource(),
                                           (JoinConditionAST *)0,
                                           ast(4)->asBaseJoinList() );
} break;

case 24: {
    ast(1) = makeAstNode<JoinJoinListAST>( 0, location(1), location(2),
                                           ast(3)->asSingleSource(),
                                           (JoinConditionAST *)0,
                                           ast(4)->asBaseJoinList() );
} break;

case 25: {} break;

case 26: {
    ast(1) = makeAstNode<CompoundJoinListAST>( location(1),
                                               ast(2)->asBaseJoinList(),
                                               location(3) );
} break;

case 27: {
    ast(1) = makeAstNode<CommaJoinListAST>( location(1),
                                            ast(2)->asSingleSource(),
                                            ast(3)->asBaseJoinList() );
} break;

case 28: {} break;

case 29: {
    ast(1) = makeAstNode<JoinSourceAST>( ast(1)->asSingleSource(),
                                         ast(2)->asBaseJoinList() );
} break;

case 30: {} break;

case 31: {} break;

case 32: {} break;

case 33: {
    select_column(1) = makeAstNode<TableSelectColumnsItemAST>( makeAstNode<TableIdentifierAST>( createName(NameAST::Schema, location(1)),
                                                                                                location(2),
                                                                                                createName(NameAST::Table, location(3)) ),
                                                               location(4), location(5) );
} break;

case 34: { 
    select_column(1) = makeAstNode<TableSelectColumnsItemAST>( makeAstNode<TableIdentifierAST>( (NameAST *)0, 0,
                                                                                                 createName(NameAST::Table, location(1)) ),
                                                               location(2), location(3));
} break;

case 35: {
    ast(1) = makeAstNode<ValueSelectColumnItemAST>( expression(1),
                                                    location(2),
                                                    createName(NameAST::ColumnAlias, location(3)) );
} break;

case 36: {
    ast(1) = makeAstNode<ValueSelectColumnItemAST>( expression(1), 0,
                                                    createName(NameAST::ColumnAlias, location(2)) );
} break;

case 37: { ast(1) = makeAstNode<ValueSelectColumnItemAST>(expression(1), 0, (NameAST *) 0); } break;

case 38: {
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

case 39: {
    if ( sym(1).sel_col ) {
        sym(1).sel_col_list = makeAstNode< List<BaseSelectColumnItemAST *> >(sym(1).sel_col);
    } else {
        sym(1).sel_col_list = 0;
    }
} break;

case 40: { ast(1) = makeAstNode<ListSelectColumnAST>(sym(1).sel_col_list); } break;

case 41: { ast(1) = makeAstNode<StarSelectColumnAST>(location(1)); } break;

case 42: { ast(1) = makeAstNode<WhereClauseAST>(location(1), condition(2)); } break;

case 43: {} break;

case 44: { ast(1) = makeAstNode<StartWithClauseAST>(location(1), location(2), condition(3)); } break;

case 45: {} break;

case 46: { ast(1) = makeAstNode<StartConnectPriorAST>(location(1), expression(2), location(3), expression(4)); } break;

case 47: { ast(1) = makeAstNode<EndConnectPriorAST>(expression(1), location(2), expression(3), location(4)); } break;

case 48: {
    ast(1) = makeAstNode<ConnectByClauseAST>( location(1),
                                              location(2),
                                              ast(3)->asBaseConnectPriorClause(),
                                              ast(4)->asStartWithClause() );
} break;

case 49: {} break;

case 50: {
    List<ValueExpressionAST *> *expr_list = sym(3).expression_list;
    ast(1) = makeAstNode<GroupByBlockAST>( location(1),
                                           location(2),
                                           expr_list,
                                           0,
                                           (ConditionExpressionAST *)0 );
    if ( !expr_list )
        error(location(2) + 1, "Empty Expression List");
} break;

case 51: {
    List<ValueExpressionAST *> *expr_list = sym(3).expression_list;
    ast(1) = makeAstNode<GroupByBlockAST>( location(1),
                                           location(2),
                                           expr_list,
                                           location(4),
                                           condition(5) );
    if ( !expr_list )
        error(location(2) + 1, "Empty Expression List");
} break;

case 52: { error(location(1) + 1, "Expexted Token `BY'"); } break;

case 53: {} break;

case 54: {
    ast(1) = makeAstNode<SetClauseAST>( makeAstNode<UnionSetSpecAST>(location(1), location(2)),
                                        ast(3)->asSelectCore() );
} break;

case 55: {
    ast(1) = makeAstNode<SetClauseAST>( makeAstNode<UnionSetSpecAST>(location(1), location(2)),
                                        ast(3)->asSelectCore() );
} break;

case 56: {
    ast(1) = makeAstNode<SetClauseAST>( makeAstNode<UnionSetSpecAST>(location(1), 0),
                                        ast(2)->asSelectCore() );
} break;

case 57: {
    ast(1) = makeAstNode<SetClauseAST>( makeAstNode<IntersectSetSpecAST>(location(1)),
                                        ast(2)->asSelectCore() );
} break;

case 58: {
    ast(1) = makeAstNode<SetClauseAST>( makeAstNode<MinusSetSpecAST>(location(1)),
                                        ast(2)->asSelectCore() );
} break;

case 59: {} break;

case 60: { /* RULE `select_core_item` */
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

case 61: { /* RULE `select_core_item` */
    ast(1) = makeAstNode<SelectCoreAST>( location(1),                       /* SELECT token */
                                         location(2),                       /* ALL|DISTINCT token */
                                         (BaseSelectColumnsAST *)0,         /* select columns */
                                         location(3),                       /* FROM token */
                                         sym(4).ast->asJoinSource(),        /* select source */
                                         sym(5).ast->asWhereClause(),       /* where ... */
                                         sym(6).ast->asConnectByClause(),   /* connect by ...*/
                                         sym(7).ast->asGroupByBlock(),      /* group by ... */
                                         sym(8).ast->asSetClause()          /* UNION | INTERSECT | MINUS <select_core> */ );
    error(location(3), "Selected Columns Expected");
} break;

case 62: {
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

case 63: {
    if ( sym(1).column_ident ) {
        sym(1).column_ident_list = makeAstNode< List<ColumnIdentifierAST *> >(sym(1).column_ident);
    } else {
        sym(1).column_ident_list = 0;
    }
} break;

case 64: {} break;

case 65: {} break;

case 66: { location(1) = 0; } break;

case 67: { ast(1) = makeAstNode<OrderingTermAST>(expression(1), location(2)); } break;

case 68: {
    if ( sym(1).ordered_expression_list && sym(3).ordered_expression ) {
        sym(1).ordered_expression_list = makeAstNode< List<OrderingTermAST *> >(sym(1).ordered_expression_list, sym(3).ordered_expression);
    } else if ( !sym(1).ordered_expression_list ) {
        if ( sym(3).ordered_expression ) {
            sym(1).ordered_expression_list = makeAstNode< List<OrderingTermAST *> >(sym(3).ordered_expression);
        } else {
            sym(1).ordered_expression_list = 0;
        }
    }
} break;

case 69: {
    if ( sym(1).ordered_expression ) {
        sym(1).ordered_expression_list = makeAstNode< List<OrderingTermAST *> >(sym(1).ordered_expression);
    } else {
        sym(1).ordered_expression_list = 0;
    }
} break;

case 70: { ast(1) = makeAstNode<OrderByBlockAST>(location(1), location(2), sym(3).ordered_expression_list); } break;

case 71: {} break;

case 72: {} break;

case 73: {} break;

case 74: { ast(1) = makeAstNode<ForUpdateClauseAST>(location(1), location(2), location(3), sym(4).column_ident_list, location(5)); } break;

case 75: {} break;

case 76: {
    ast(1) = makeAstNode<SelectStatementAST>( sym(1).ast->asSelectCore(),
                                              sym(2).ast->asOrderByBlock(),
                                              sym(3).ast->asForUpdateClause() );
} break;

case 77: {
    ast(1) = makeAstNode<DeleteStatementAST>( location(1),
                                              location(2),
                                              makeAstNode<DirectSingleSourceAST>(table_ident(3), createName(NameAST::TableAlias, location(4))),
                                              ast(5)->asWhereClause() );
} break;

case 78: {
    ast(1) = makeAstNode<DeleteStatementAST>( location(1),
                                              location(2),
                                              makeAstNode<DirectSingleSourceAST>(table_ident(3), (NameAST *) 0),
                                              ast(4)->asWhereClause() );
} break;

case 79: {
    ast(1) = makeAstNode<InsertColumnsClauseAST>( location(1),
                                                  sym(2).column_ident_list,
                                                  location(3) );
} break;

case 80: {} break;

case 81: {
    ast(1) = makeAstNode<InsertClauseAsValuesAST>( location(1),
                                                   location(2),
                                                   sym(3).expression_list,
                                                   location(4) );
} break;

case 82: {
    ast(1) = makeAstNode<InsertClauseAsSelectAST>(statement(1));
} break;

case 83: {} break;

case 84: {
    ast(1) = makeAstNode<InsertStatementAST>( location(1),
                                              location(2),
                                              makeAstNode<DirectSingleSourceAST>( table_ident(3), createName(NameAST::TableAlias, location(4)) ),
                                              ast(5)->asInsertColumnsClause(),
                                              ast(6)->asInsertClause() );
} break;

case 85: {
    ast(1) = makeAstNode<InsertStatementAST>( location(1),
                                              location(2),
                                              makeAstNode<DirectSingleSourceAST>(table_ident(3), (NameAST *) 0),
                                              ast(4)->asInsertColumnsClause(),
                                              ast(5)->asInsertClause() );
} break;

case 86: {
    ast(1) = makeAstNode<AssignmentExpressionAST>( column_ident(1),
                                                   location(2),
                                                   expression(3) );
} break;

case 87: {
    ast(1) = makeAstNode<AssignmentExpressionAST>( column_ident(1),
                                                   0,
                                                   (ValueExpressionAST *) 0 );
} break;

case 88: {
    if ( sym(1).update_assign_list && sym(3).update_assign ) {
        sym(1).update_assign_list = makeAstNode<AssignmentExpressionListAST>(sym(1).update_assign_list, sym(3).update_assign);
    } else if ( !sym(1).update_assign_list ) {
        if ( sym(3).update_assign ) {
            sym(1).update_assign_list = makeAstNode<AssignmentExpressionListAST>(sym(3).update_assign);
        } else {
            sym(1).update_assign_list = 0;
        }
    }
} break;

case 89: {
    if ( sym(1).update_assign ) {
        sym(1).update_assign_list = makeAstNode<AssignmentExpressionListAST>(sym(1).update_assign);
    } else {
        sym(1).update_assign_list = 0;
    }
} break;

case 90: { sym(1).update_assign_list = 0; } break;

case 91: {
    ast(1) = makeAstNode<UpdateSetClauseAST>( location(1), sym(2).update_assign_list );
} break;

case 92: {} break;

case 93: {
    ast(1) = makeAstNode<UpdateStatementAST>( location(1),
                                              makeAstNode<DirectSingleSourceAST>( table_ident(2), createName(NameAST::TableAlias, location(3)) ),
                                              ast(4)->asUpdateSetClause(),
                                              ast(5)->asWhereClause() );
} break;

case 94: {
    ast(1) = makeAstNode<UpdateStatementAST>( location(1),
                                              makeAstNode<DirectSingleSourceAST>(table_ident(2), (NameAST *) 0),
                                              ast(3)->asUpdateSetClause(),
                                              ast(4)->asWhereClause() );
} break;

case 95:{
    ast(1) = makeAstNode<CommentOnTableStatementAST>( location(1),
                                                      location(2),
                                                      location(3),
                                                      table_ident(4),
                                                      location(5),
                                                      makeAstNode<LiteralExpressionAST>(location(6)) );
}break;

case 96:{
    ast(1) = makeAstNode<CommentOnTableStatementAST>( location(1),
                                                      location(2),
                                                      location(3),
                                                      table_ident(4),
                                                      0, (LiteralExpressionAST *)0 );
}break;

case 97:{
    ast(1) = makeAstNode<CommentOnColumnStatementAST>( location(1),
                                                       location(2),
                                                       location(3),
                                                       column_ident(4),
                                                       location(5),
                                                       makeAstNode<LiteralExpressionAST>(location(6)) );
}break;

case 98:{
    ast(1) = makeAstNode<CommentOnColumnStatementAST>( location(1),
                                                       location(2),
                                                       location(3),
                                                       column_ident(4),
                                                       0, (LiteralExpressionAST *)0 );
}break;

case 99: {
    ast(1) = makeAstNode<DropTableStatementAST>( location(1),
                                                 location(2),
                                                 table_ident(3) );
} break;

case 100: {
    ast(1) = makeAstNode<DropViewStatementAST>( location(1),
                                                location(2),
                                                table_ident(3) );
} break;

case 101: {
    ast(1) = makeAstNode<DropIndexStatementAST>( location(1),
                                                 location(2),
                                                 index_ident(3) );
} break;

case 102: {
    ast(1) = makeAstNode<DropTriggerStatementAST>( location(1),
                                                   location(2),
                                                   trigger_ident(3) );
} break;

case 103: {
    ast(1) = makeAstNode<TruncateStatementAST>( location(1),
                                                location(2),
                                                table_ident(3) );
} break;

case 104: {
    ast(1) = makeAstNode<SortedColumnNameAST>( createName(NameAST::Column, location(1)),
                                               location(2) );
}

case 105: {
    ast(1) = createName(NameAST::Column, location(1));
} break;

case 106: {
    if ( sym(1).column_name_list && sym(3).column_name ) {
        sym(1).column_name_list = makeAstNode<NameListAST>(sym(1).column_name_list, sym(3).column_name);
    } else if ( !sym(1).column_name_list ) {
        if ( sym(3).column_name ) {
            sym(1).column_name_list = makeAstNode<NameListAST>(sym(3).column_name);
        } else {
            sym(1).column_name_list = 0;
        }
    }
} break;

case 107: {
    if ( sym(1).column_name ) {
        sym(1).column_name_list = makeAstNode<NameListAST>(sym(1).column_name);
    } else {
        sym(1).column_name_list = 0;
    }
} break;

case 108: { sym(1).column_name_list = 0; } break;

case 109: {
    if ( sym(1).ordered_column_list && sym(3).ordered_column ) {
        sym(1).ordered_column_list = makeAstNode<SortedColumnNameListAST>(sym(1).ordered_column_list, sym(3).ordered_column);
    } else if ( !sym(1).ordered_column_list ) {
        if ( sym(3).ordered_column ) {
            sym(1).ordered_column_list = makeAstNode<SortedColumnNameListAST>(sym(3).ordered_column);
        } else {
            sym(1).ordered_column_list = 0;
        }
    }
} break;

case 110: {
    if ( sym(1).ordered_column ) {
        sym(1).ordered_column_list = makeAstNode<SortedColumnNameListAST>(sym(1).ordered_column);
    } else {
        sym(1).ordered_column_list = 0;
    }
} break;

case 111: { sym(1).ordered_column_list = 0; } break;

case 112: {
    ast(1) = makeAstNode<FkReferencesClauseAST>( location(1),
                                                 table_ident(2),
                                                 location(3),
                                                 sym(4).column_name_list,
                                                 location(4) );
} break;

case 113: {
    ast(1) = makeAstNode<FkReferencesClauseAST>( location(1),
                                                 table_ident(2),
                                                 0,
                                                 (NameListAST *) 0,
                                                 0 );
} break;

case 114: {
    ast(1) = makeAstNode<UniqueOrPkTableConstraintAST>( location(1),
                                                        createName(NameAST::Constraint, location(2)),
                                                        location(3),
                                                        location(4),
                                                        location(5),
                                                        sym(6).ordered_column_list,
                                                        location(7) );
} break;

case 115: {
    ast(1) = makeAstNode<UniqueOrPkTableConstraintAST>( 0, (NameAST *) 0,
                                                        location(1),
                                                        location(2),
                                                        location(3),
                                                        sym(4).ordered_column_list,
                                                        location(5) );
} break;

case 116: {
    ast(1) = makeAstNode<UniqueOrPkTableConstraintAST>( location(1),
                                                        createName(NameAST::Constraint, location(2)),
                                                        location(3),
                                                        location(4),
                                                        location(5),
                                                        sym(6).ordered_column_list,
                                                        location(7) );
} break;

case 117: {
    ast(1) = makeAstNode<UniqueOrPkTableConstraintAST>( 0, (NameAST *) 0,
                                                        location(1),
                                                        location(2),
                                                        location(3),
                                                        sym(4).ordered_column_list,
                                                        location(5) );
} break;

case 118: {
    ast(1) = makeAstNode<CheckTableConstraintAST>( location(1),
                                                   createName(NameAST::Constraint, location(2)),
                                                   location(3),
                                                   location(4),
                                                   condition(5),
                                                   location(6) );
} break;

case 119: {
    ast(1) = makeAstNode<CheckTableConstraintAST>( 0, (NameAST *) 0,
                                                   location(1),
                                                   location(2),
                                                   condition(3),
                                                   location(4) );
} break;

case 120: {
    ast(1) = makeAstNode<FkTableConstraintAST>( location(1),
                                                createName(NameAST::Constraint, location(2)),
                                                location(3),
                                                location(4),
                                                location(5),
                                                sym(6).column_name_list,
                                                location(7),
                                                ast(8)->asForeignKeyClause() );
} break;

case 121: {
    ast(1) = makeAstNode<FkTableConstraintAST>( 0, (NameAST *) 0,
                                                location(1),
                                                location(2),
                                                location(3),
                                                sym(4).column_name_list,
                                                location(5),
                                                ast(6)->asForeignKeyClause() );
} break;

case 122: {
    if ( sym(1).table_constraint_list && sym(3).table_constraint ) {
        sym(1).table_constraint_list = makeAstNode<TableConstraintListAST>(sym(1).table_constraint_list, sym(3).table_constraint);
    } else if ( !sym(1).table_constraint_list ) {
        if ( sym(3).table_constraint ) {
            sym(1).table_constraint_list = makeAstNode<TableConstraintListAST>(sym(3).table_constraint);
        } else {
            sym(1).table_constraint_list = 0;
        }
    }
} break;

case 123: {
    if ( sym(1).table_constraint ) {
        sym(1).table_constraint_list = makeAstNode<TableConstraintListAST>(sym(1).table_constraint);
    } else {
        sym(1).table_constraint_list = 0;
    }
} break;

case 124: { sym(1).table_constraint_list = 0; } break;

case 125: { ast(1) = makeAstNode<ConstraintDeferStateAST>( location(1), location(2), location(3), location(4) ); } break;

case 126: { ast(1) = makeAstNode<ConstraintDeferStateAST>( location(1), location(2), location(3), 0 ); } break;

case 127: { ast(1) = makeAstNode<ConstraintDeferStateAST>( location(1), location(2), location(3), location(4) ); } break;

case 128: { ast(1) = makeAstNode<ConstraintDeferStateAST>( location(1), location(2), location(3), 0 ); } break;

case 129: { ast(1) = makeAstNode<ConstraintDeferStateAST>( location(1), location(2), 0, 0 ); } break;

case 130: { ast(1) = makeAstNode<ConstraintDeferStateAST>( location(1), location(2), location(3), location(4) ); } break;

case 131: { ast(1) = makeAstNode<ConstraintDeferStateAST>( location(1), location(2), location(3), 0 ); } break;

case 132: { ast(1) = makeAstNode<ConstraintDeferStateAST>( location(1), location(2), 0, 0 ); } break;

case 133: { ast(1) = makeAstNode<ConstraintDeferStateAST>( location(1), location(2), 0, 0 ); } break;

case 134: { ast(1) = makeAstNode<ConstraintDeferStateAST>( location(1), 0, 0, 0 ); } break;

case 135: { ast(1) = 0; } break;

case 136: {} break;

case 137: {} break;

case 138: { location(1) = 0; } break;

case 139: {
    ast(1) = makeAstNode<UsingIndexClauseAST>( location(1),
                                               location(2),
                                               (IndexIdentifierAST *) 0,
                                               location(3),
                                               (CreateIndexStatementAST *) ast(4),
                                               location(5) );
} break;

case 140: {
    ast(1) = makeAstNode<UsingIndexClauseAST>( location(1),
                                               location(2),
                                               index_ident(3),
                                               0,
                                               (CreateIndexStatementAST *) 0,
                                               0 );
} break;

case 141: { ast(1) = 0; } break;

case 142: {
    ast(1) = makeAstNode<ConstraintStateAST>( (ConstraintDeferStateAST *) ast(1),
                                              location(2),
                                              (UsingIndexClauseAST *) ast(3),
                                              0,
                                              0,
                                              (ExceptionsClauseAST *) 0 );
} break;

case 143: {
    ast(1) = makeAstNode<PkColumnConstraintAST>( location(1),
                                                 createName(NameAST::Constraint, location(2)),
                                                 location(3), location(4), location(5),
                                                 ast(6)->asConstraintState() );
} break;

case 144: {
    ast(1) = makeAstNode<PkColumnConstraintAST>( 0, (NameAST *) 0, location(1), location(2), location(3), ast(4)->asConstraintState() );
} break;

case 145: {
    ast(1) = makeAstNode<NotNullColumnConstraintAST>( location(1),
                                                      createName(NameAST::Constraint, location(2)),
                                                      location(3), location(4),
                                                      ast(5)->asConstraintState() );
} break;

case 146: {
    ast(1) = makeAstNode<NotNullColumnConstraintAST>( 0, (NameAST *) 0, location(1), location(2), ast(3)->asConstraintState() );
} break;

case 147: {
    ast(1) = makeAstNode<UniqueColumnConstraintAST>( location(1),
                                                     createName(NameAST::Constraint, location(2)),
                                                     location(3),
                                                     ast(4)->asConstraintState() );
} break;

case 148: {
    ast(1) = makeAstNode<UniqueColumnConstraintAST>( 0, (NameAST *) 0, location(1), ast(2)->asConstraintState() );
} break;

case 149: {
    ast(1) = makeAstNode<CheckColumnConstraintAST>( location(1),
                                                    createName(NameAST::Constraint, location(2)),
                                                    location(3),
                                                    location(4),
                                                    condition(5),
                                                    location(6),
                                                    ast(7)->asConstraintState() );
} break;

case 150: {
    ast(1) = makeAstNode<CheckColumnConstraintAST>( 0, (NameAST *) 0,
                                                    location(1),
                                                    location(2),
                                                    condition(3),
                                                    location(4),
                                                    ast(5)->asConstraintState() );
} break;

case 151: {
    ast(1) = makeAstNode<ReferencesColumnConstraintAST>( location(1),
                                                         createName(NameAST::Constraint, location(2)),
                                                         ast(3)->asForeignKeyClause(),
                                                         ast(4)->asConstraintState() );
} break;

case 152: {
    ast(1) = makeAstNode<ReferencesColumnConstraintAST>( 0, (NameAST *) 0, ast(1)->asForeignKeyClause(), ast(2)->asConstraintState() );
} break;

case 153: {
    MAKE_LIST_NEXT(ColumnConstraintListAST, 1, 2, column_constraint_list, column_constraint)
    /* if ( sym(1).column_constraint_list && sym(2).column_constraint ) {
        sym(1).column_constraint_list = makeAstNode<ColumnConstraintListAST>(sym(1).column_constraint_list, sym(2).column_constraint);
    } else if ( !sym(1).column_constraint_list ) {
        if ( sym(3).column_constraint ) {
            sym(1).column_constraint_list = makeAstNode<ColumnConstraintListAST>(sym(2).column_constraint);
        } else {
            sym(1).column_constraint_list = 0;
        }
    } */
} break;

case 154: {
    MAKE_LIST_FIRST(ColumnConstraintListAST, 1, column_constraint_list, column_constraint)
    /*
    if ( sym(1).column_constraint ) {
        sym(1).column_constraint_list = makeAstNode<ColumnConstraintListAST>(sym(1).column_constraint);
    } else {
        sym(1).column_constraint_list = 0;
    } */
} break;

case 155: {} break;

case 156: {} break;

case 157: {} break;

case 158: {} break;

case 159: {} break;

case 160: {} break;

case 161: {} break;

case 162: {} break;

case 163: {} break;

case 164: {} break;

case 165: {} break;

case 166: {} break;

case 167: {} break;

case 168: {} break;

case 169: {} break;

case 170: {} break;

case 171: {} break;

case 172: {} break;

case 173: {} break;

case 174: {} break;

case 175: {} break;

case 176: {} break;

case 177: {} break;

case 178: {} break;

case 179: {} break;

case 180: {} break;

case 181: {} break;

case 182: {
    ast(1) = makeAstNode<CharacterTypeAST>( location(1),
                                            location(2),
                                            location(3),
                                            location(2) );
} break;

case 183: {
    ast(1) = makeAstNode<NumberTypeAST>( location(1),
                                         location(2),
                                         location(3),
                                         location(4),
                                         location(5),
                                         location(6) );
} break;

case 184: {
    ast(1) = makeAstNode<NumberTypeAST>( location(1),
                                         location(2),
                                         location(3),
                                         0,
                                         0,
                                         location(4) );
} break;

case 185: {
    ast(1) = makeAstNode<SimpleTypeAST>( location(1) );
} break;

case 186: {} break;

case 187: {} break;

case 188: {} break;

case 189: {
    ast(1) = makeAstNode<ColumnDefinitionAST>( createName(NameAST::Column, location(1)),
                                               ast(2)->asBaseType(),
                                               location(3),
                                               ast(4)->asConstantExpression(),
                                               sym(5).column_constraint_list );
} break;

case 190: {
    ast(1) = makeAstNode<ColumnDefinitionAST>( createName(NameAST::Column, location(1)),
                                               ast(2)->asBaseType(),
                                               location(3),
                                               ast(4)->asConstantExpression(),
                                               (ColumnConstraintListAST *) 0 );
} break;

case 191: {
    ast(1) = makeAstNode<ColumnDefinitionAST>( createName(NameAST::Column, location(1)),
                                               ast(2)->asBaseType(), 0,
                                               (ConstantExpressionAST *) 0,
                                               sym(3).column_constraint_list );
} break;

case 192: {
    ast(1) = makeAstNode<ColumnDefinitionAST>( createName(NameAST::Column, location(1)),
                                               ast(2)->asBaseType(), 0,
                                               (ConstantExpressionAST *) 0,
                                               (ColumnConstraintListAST *) 0 );
} break;

case 193: {
    MAKE_LIST_NEXT(ColumnDefinitionListAST, 1, 3, column_definition_list, column_definition)
    /*
    if ( sym(1).column_definition_list && sym(3).column_definition ) {
        sym(1).column_definition_list = makeAstNode<ColumnDefinitionListAST>(sym(1).column_definition_list, sym(3).column_definition);
    } else if ( !sym(1).column_definition_list ) {
        if ( sym(3).column_definition ) {
            sym(1).column_definition_list = makeAstNode<ColumnDefinitionListAST>(sym(3).column_definition);
        } else {
            sym(1).column_definition_list = 0;
        }
    } */
} break;

case 194: {
    MAKE_LIST_FIRST(ColumnDefinitionListAST, 1, column_definition_list, column_definition)
    /*
    if ( sym(1).column_definition ) {
        sym(1).column_definition_list = makeAstNode<ColumnDefinitionListAST>(sym(1).column_definition);
    } else {
        sym (1).column_definition_list = 0;
    } */
} break;

case 195: { sym(1).column_definition = 0; } break;

case 196: {
    ast(1) = makeAstNode<CreateTableAsSelectClauseAST>( location(1), statement(2) );
} break;

case 197: {
    ast(1) = makeAstNode<CreateTableDirectClauseAST>( location(1),
                                                      sym(2).col_def_list,
                                                      sym(4).table_constraint_list,
                                                      location(5) );
} break;

case 198: {
    ast(1) = makeAstNode<CreateTableDirectClauseAST>( location(1),
                                                      sym(2).col_def_list,
                                                      (TableConstraintListAST *) 0,
                                                      location(3) );
} break;

case 199: {
    ast(1) = makeAstNode<CreateTableStatementAST>( location(1),
                                                   location(2),
                                                   table_ident(3),
                                                   ast(4)->asCreateTableClause() );
} break;

case 200: {
    ast(1) = makeAstNode<CreateTableStatementAST>( location(1),
                                                   location(2),
                                                   table_ident(3),
                                                   (CreateTableClauseAST *) 0 );
} break;

case 201: {
    ast(1) = makeAstNode<CreateViewStatementAST>( location(1),
                                                  location(2),
                                                  table_ident(3),
                                                  location(4),
                                                  statement(5) );
} break;

case 202: {
    ast(1) = makeAstNode<CreateViewStatementAST>( location(1),
                                                  location(2),
                                                  table_ident(3),
                                                  0,
                                                  (StatementAST *) 0 );
} break;

case 203: {} break;

case 204: {} break;

case 205: { location(1) = 0; } break;

case 206: {
    ast(1) = makeAstNode<TableCreateIndexClauseAST>( location(1),
                                                     table_ident(2),
                                                     location(3),
                                                     sym(4).ordered_expression_list,
                                                     location(5) );
} break;

case 207: {
    ast(1) = makeAstNode<TableCreateIndexClauseAST>( location(1),
                                                     table_ident(2),
                                                     0,
                                                     (OrderingTermListAST *) 0,
                                                     0 );
} break;

case 208: {} break;

case 209: {} break;

case 210: {
    ast(1) = makeAstNode<CreateIndexStatementAST>( location(1),
                                                   location(2),
                                                   location(3),
                                                   index_ident(4),
                                                   ast(5)->asBaseCreateIndexClause() );
} break;

case 211: {
    NameAST *nameAst = 0;
    if ( location(5) )
        nameAst = createName(NameAST::Column, location(5));
    else
        error(location(4), "Identifier Expected");
    ast(1) = makeAstNode<ColumnIdentifierAST>( createName(NameAST::Schema, location(1)),
                                               location(2),
                                               createName(NameAST::Table, location(3)),
                                               location(4),
                                               nameAst );
} break;

case 212: {
    NameAST *nameAst = 0;
    if ( location(3) )
        nameAst = createName(NameAST::Column, location(3));
    else
        error(location(2), "Identifier Expected");
    ast(1) = makeAstNode<ColumnIdentifierAST>( (NameAST *) 0, 0,
                                               createName(NameAST::Table, location(1)),
                                               location(2),
                                               nameAst );
} break;

case 213: {
    ast(1) = makeAstNode<ColumnIdentifierAST>( (NameAST *) 0, 0,
                                               (NameAST *) 0, 0,
                                               createName(NameAST::Column, location(1)) );
} break;

case 214: {
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

case 215: {
    if ( sym(1).expression ) {
        sym(1).expression_list = makeAstNode< List<ValueExpressionAST *> >(sym(1).expression);
    } else {
        sym(1).expression_list = 0;
    }
} break;

case 216: { sym(1).expression_list = 0; } break;

case 217: {} break;

case 218: {} break;

case 219: {} break;

case 220: { ast(1) = makeAstNode<BinaryConditionExpressionAST>(condition(1), location(2), condition(3)); } break;

case 221: {} break;

case 222: { ast(1) = makeAstNode<BinaryConditionExpressionAST>(condition(1), location(2), condition(3)); } break;

case 223: { ast(1) = makeAstNode<NotConditionExpressionAST>(location(1), condition(2)); } break;

case 224: {} break;

case 225: {} break;

case 226: { ast(1) = makeAstNode<CompoundConditionExpressionAST>(location(1), condition(2), location(3)); } break;

case 227: {} break;

case 228: {} break;

case 229: {} break;

case 230: {} break;

case 231: {} break;

case 232: {} break;

case 233: { ast(1) = makeAstNode<TrueConditionExpressionAST>(location(1)); } break;

case 234: { ast(1) = makeAstNode<FalseConditionExpressionAST>(location(1)); } break;

case 235: {    /* +2 Conflicts: shift/reduce */
    ast(1) = makeAstNode<ConditionAsValueExpressionAST>(expression(1));
    error(ast(1)->firstToken(), ast(1)->lastToken(), "Invalid Relational Operation");
} break;

case 236: { ast(1) = makeAstNode<TrueConditionExpressionAST>(location(1)); } break;

case 237: {} break;

case 238: {} break;

case 239: {} break;

case 240: {} break;

case 241: {} break;

case 242: {} break;

case 243: { ast(1) = makeAstNode<NumericExpressionAST>(location(1)); } break;

case 244: { ast(1) = makeAstNode<LiteralExpressionAST>(location(1)); } break;

case 245: { ast(1) = makeAstNode<SubQueryExpressionAST>(location(1), ast(2)->asSelectStatement(), location(3)); } break;

case 246: {  /* +1 Conflicts: shift/reduce */
    List<ValueExpressionAST *> *expr_list = sym(2).expression_list;
    ast(1) = makeAstNode<ValueListExpressionAST>(location(1), expr_list, location(3));
    if ( !expr_list )
        error(location(3), "Empty Expression Set");
} break;

case 247: { ast(1) = makeAstNode<ValueListExpressionAST>(0, makeAstNode< List<ValueExpressionAST *> >(sym(1).expression), 0); } break;

case 248: { ast(1) = makeAstNode<CompareConditionExpressionAST>(expression(1), location(2), expression(3)); } break;

case 249: { 
    ast(1) = makeAstNode<CompareConditionExpressionAST>(expression(1), location(2), (ValueExpressionAST *) 0); 
    error(location(2) + 1, "Expected Expression");
} break;

case 250: { ast(1) = makeAstNode<BetweenConditionExpressionAST>(expression(1), location(2), location(3), expression(4), location(5), expression(6)); } break;

case 251: {
    ast(1) = makeAstNode<BetweenConditionExpressionAST>(expression(1), location(2), location(3), expression(4), location(5), (ValueExpressionAST *) 0);
    error(location(5) + 1, "Expected Expression");
} break;

case 252: { ast(1) = makeAstNode<BetweenConditionExpressionAST>(expression(1), 0, location(2), expression(3), location(4), expression(5)); } break;

case 253: {
    ast(1) = makeAstNode<BetweenConditionExpressionAST>(expression(1), 0, location(2), expression(3), location(4), (ValueExpressionAST *) 0);
    error(location(4) + 1, "Expected Expression");
} break;

case 254: { ast(1) = makeAstNode<LikeConditionExpressionAST>( expression(1), location(2), location(3), expression(4) ); } break;

case 255: {
    ast(1) = makeAstNode<LikeConditionExpressionAST>( expression(1), location(2), location(3), (ValueExpressionAST *) 0 );
    error(location(3) + 1, "Expected Expression");
} break;

case 256: { ast(1) = makeAstNode<LikeConditionExpressionAST>( expression(1), 0, location(2), expression(3) ); } break;

case 257: {
    ast(1) = makeAstNode<LikeConditionExpressionAST>( expression(1), 0, location(2), (ValueExpressionAST *) 0 );
    error(location(2) + 1, "Expected Expression");
} break;

case 258: { ast(1) = makeAstNode<InConditionExpressionAST>(expression(1), location(2), location(3), ast(4)->asCompoundRValueExpression()); } break;

case 259: { ast(1) = makeAstNode<InConditionExpressionAST>(expression(1), 0, location(2), ast(3)->asCompoundRValueExpression()); } break;

case 260: { ast(1) = makeAstNode<IsNullConditionExpressionAST>(expression(1), location(2), location(3), location(4)); } break;

case 261: {
    ast(1) = makeAstNode<IsNullConditionExpressionAST>(expression(1), location(2), location(3), 0);
    error(location(3) + 1, "Expected Token `NULL'");
} break;

case 262: { ast(1) = makeAstNode<IsNullConditionExpressionAST>(expression(1), location(2), 0, location(3)); } break;

case 263: {
    ast(1) = makeAstNode<IsNullConditionExpressionAST>(expression(1), location(2), 0, 0);
    error(location(2) + 1, "Expected Token(s) `NULL|NOT NULL'");
} break;

case 264: {
    ast(1) = makeAstNode<ExistsConditionExpressionAST>( location(1),
                                                        makeAstNode<SubQueryExpressionAST>( location(2),
                                                                                            ast(3)->asSelectStatement(),
                                                                                            location(4) ) );
} break;

case 265: {} break;

case 266: { ast(1) = makeAstNode<BinaryValueExpressionAST>(expression(1), location(2), expression(3)); } break;

case 267: { ast(1) = makeAstNode<BinaryValueExpressionAST>(expression(1), location(2), expression(3)); } break;

case 268: { ast(1) = makeAstNode<BinaryValueExpressionAST>(expression(1), location(2), expression(3)); } break;

case 269: {} break;

case 270: { ast(1) = makeAstNode<BinaryValueExpressionAST>(expression(1), location(2), expression(3)); } break;

case 271: { ast(1) = makeAstNode<BinaryValueExpressionAST>(expression(1), location(2), expression(3)); } break;

case 272: {} break;

case 273: {} break;

case 274: {} break;

case 275: { ast(1) = makeAstNode<UnaryValueExpressionAST>(location(1), expression(2)); } break;

case 276: {} break;

case 277: {
    ast(1) = makeAstNode<BuiltinFunctionCallAST>( location(1),
                                                  location(2),
                                                  sym(3).expression_list,
                                                  location(4) );
} break;

case 278: {
    ast(1) = makeAstNode<CountStarCallAST>( location(1),
                                            location(2),
                                            location(3),
                                            location(4) );
} break;

case 279: {
    ast(1) = makeAstNode<CountExprCallAST>( location(1),
                                            location(2),
                                            location(3),
                                            expression(4),
                                            location(5) );
} break;

case 280: {
    ast(1) = makeAstNode<CountExprCallAST>( location(1),
                                            location(2),
                                            location(3),
                                            expression(4),
                                            location(5) );
} break;

case 281: {
    ast(1) = makeAstNode<CountExprCallAST>( location(1),
                                            location(2),
                                            0,
                                            expression(3),
                                            location(4) );
} break;

case 282: {
    ast(1) = makeAstNode<BuiltinFunctionCallAST>( location(1),
                                                  location(2),
                                                  sym(3).expression_list,
                                                  location(4) );
} break;

case 283: {
    ast(1) = makeAstNode<FunctionCallAST>( makeAstNode<FunctionIdentifierAST>( (NameAST *) 0, 0,
                                                                               (NameAST *) 0, 0,
                                                                               createName(NameAST::Routine, location(1)) ),
                                           location(2),
                                           sym(3).expression_list,
                                           location(4) );
} break;

case 284: {
    ast(1) = makeAstNode<FunctionCallAST>( makeAstNode<FunctionIdentifierAST>( (NameAST *) 0, 0,
                                                                               createName(NameAST::Package, location(1)),
                                                                               location(2),
                                                                               createName(NameAST::Routine, location(3)) ),
                                           location(4),
                                           sym(5).expression_list,
                                           location(6) );
} break;

case 285: {
    ast(1) = makeAstNode<FunctionCallAST>( makeAstNode<FunctionIdentifierAST>( createName(NameAST::Schema, location(1)),
                                                                               location(2),
                                                                               createName(NameAST::Package, location(3)),
                                                                               location(4),
                                                                               createName(NameAST::Routine, location(5)) ),
                                           location(6),
                                           sym(7).expression_list,
                                           location(8) );
} break;

case 286: {} break;

case 287: { ast(1) = makeAstNode<CompoundValueExpressionAST>(location(1), expression(2), location(3)); } break;

case 288: {
    NameAST *nameAst = 0;
    if ( location(2) )
        nameAst = createName(NameAST::Binding, location(2));
    else
        error(location(1), "Identifier Expected");
    ast(1) = makeAstNode<ParameterExpressionAST>( location(1), nameAst );
} break;

case 289: {} break;

case 290: { ast(1) = ast(2); } break;

case 291: { ast(1) = makeAstNode<TranslationUnitAST>(sym(1).statement_list); } break;

case 292: {
    if ( sym(1).statement ) {
        sym(1).statement_list = makeAstNode< List<StatementAST *> >(sym(1).statement);
    } else {
        sym(1).statement_list = 0;
    }
} break;

case 293: {
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

case 294: { sym(1).statement_list = 0; } break;

case 295: {} break;

case 296: { ast(1) = ast(2); } break;

case 297: {} break;

case 298: {} break;

case 299: {} break;

case 300: {} break;

case 301: {} break;

case 302: {} break;

case 303: {} break;

case 304: {} break;

case 305: {} break;

case 306: {} break;

case 307: {} break;

case 308: {} break;

case 309: {} break;

case 310: { ast(1) = ast(2); } break;

case 311: { ast(1) = makeAstNode<EmptyAST>(); location(1) = 0; } break;

} // end switch
} // end Parser::reduce()
