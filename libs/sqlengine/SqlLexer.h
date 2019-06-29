#ifndef SQLLEXER_H
#define SQLLEXER_H

#include <QList>

#include "SqlToken.h"
#include "IParserEngine.h"
#include "TranslationUnit.h"

namespace Sql {

enum KindKeyword1 {
  T_2_AS = 1,
  T_2_AT,
  T_2_BY,
  T_2_DO,
  T_2_GO,
  T_2_IF,
  T_2_IN,
  T_2_IS,
  T_2_NO,
  T_2_OF,
  T_2_ON,
  T_2_OR,
  T_2_TO,
  T_3_ADD,
  T_3_ALL,
  T_3_AND,
  T_3_ANY,
  T_3_ARE,
  T_3_ASC,
  T_3_DEC,
  T_3_END,
  T_3_FOR,
  T_3_GET,
  T_3_KEY,
  T_3_MAP,
  T_3_NEW,
  T_3_NOT,
  T_3_OLD,
  T_3_OUT,
  T_3_REF,
  T_3_ROW,
  T_3_SET,
  T_3_SQL,
  T_4_BOTH,
  T_4_CALL,
  T_4_CASE,
  T_4_CUBE,
  T_4_DATA,
  T_4_DESC,
  T_4_DROP,
  T_4_EACH,
  T_4_ELSE,
  T_4_EXEC,
  T_4_EXIT,
  T_4_FREE,
  T_4_FROM,
  T_4_FULL,
  T_4_GOTO,
  T_4_HOLD,
  T_4_INTO,
  T_4_JOIN,
  T_4_LEFT,
  T_4_LIKE,
  T_4_LOOP,
  T_4_NEXT,
  T_4_NONE,
  T_4_NULL,
  T_4_ONLY,
  T_4_OPEN,
  T_4_OVER,
  T_4_PATH,
  T_4_READ,
  T_4_REAL,
  T_4_ROLE,
  T_4_ROWS,
  T_4_SETS,
  T_4_SOME,
  T_4_THEN,
  T_4_TIME,
  T_4_TRUE,
  T_4_UNDO,
  T_4_USER,
  T_4_VIEW,
  T_4_WHEN,
  T_4_WITH,
  T_4_WORK,
  T_5_AFTER,
  T_5_ALTER,
  T_5_ARRAY,
  T_5_BEGIN,
  T_5_CHECK,
  T_5_CLOSE,
  T_5_CROSS,
  T_5_CYCLE,
  T_5_DEPTH,
  T_5_DEREF,
  T_5_FALSE,
  T_5_FETCH,
  T_5_GRANT,
  T_5_GROUP,
  T_5_INDEX,
  T_5_INNER,
  T_5_INOUT,
  T_5_INPUT,
  T_5_LARGE,
  T_5_LEAVE,
  T_5_LEVEL,
  T_5_LIMIT,
  T_5_LOCAL,
  T_5_MATCH,
  T_5_MINUS,
  T_5_NAMES,
  T_5_ORDER,
  T_5_OUTER,
  T_5_PRIOR,
  T_5_RANGE,
  T_5_READS,
  T_5_RIGHT,
  T_5_SCOPE,
  T_5_SPACE,
  T_5_START,
  T_5_STATE,
  T_5_TABLE,
  T_5_TREAT,
  T_5_UNDER,
  T_5_UNION,
  T_5_UNTIL,
  T_5_USAGE,
  T_5_USING,
  T_5_VALUE,
  T_5_WHERE,
  T_5_WHILE,
  T_5_WRITE,
  T_6_ACTION,
  T_6_ATOMIC,
  T_6_BEFORE,
  T_6_COLUMN,
  T_6_COMMIT,
  T_6_CREATE,
  T_6_CURSOR,
  T_6_DELETE,
  T_6_DOMAIN,
  T_6_ELSEIF,
  T_6_EQUALS,
  T_6_ESCAPE,
  T_6_EXCEPT,
  T_6_EXISTS,
  T_6_FILTER,
  T_6_GLOBAL,
  T_6_HAVING,
  T_6_INSERT,
  T_6_METHOD,
  T_6_MODIFY,
  T_6_MODULE,
  T_6_OPTION,
  T_6_OUTPUT,
  T_6_PUBLIC,
  T_6_REPEAT,
  T_6_RENAME,
  T_6_RESULT,
  T_6_RETURN,
  T_6_REVOKE,
  T_6_ROLLUP,
  T_6_SCHEMA,
  T_6_SCROLL,
  T_6_SEARCH,
  T_6_SELECT,
  T_6_SIGNAL,
  T_6_STATIC,
  T_6_SYSTEM,
  T_6_UNIQUE,
  T_6_UNNEST,
  T_6_UPDATE,
  T_6_VALUES,
  T_6_WINDOW,
  T_6_WITHIN,
  T_7_BETWEEN,
  T_7_BREADTH,
  T_7_CASCADE,
  T_7_CATALOG,
  T_7_COLLATE,
  T_7_COMMENT,
  T_7_CONNECT,
  T_7_CURRENT,
  T_7_DECLARE,
  T_7_DEFAULT,
  T_7_DYNAMIC,
  T_7_EXECUTE,
  T_7_FOREIGN,
  T_7_GENERAL,
  T_7_HANDLER,
  T_7_ITERATE,
  T_7_LATERAL,
  T_7_LEADING,
  T_7_LOCATOR,
  T_7_PARTIAL,
  T_7_PREPARE,
  T_7_PRIMARY,
  T_7_RELEASE,
  T_7_REPLACE,
  T_7_RETURNS,
  T_7_ROUTINE,
  T_7_SECTION,
  T_7_SESSION,
  T_7_SIMILAR,
  T_7_TRIGGER,
  T_7_UNKNOWN,
  T_7_WITHOUT,
  T_8_ABSOLUTE,
  T_8_ALLOCATE,
  T_8_CASCADED,
  T_8_CONTINUE,
  T_8_DEFERRED,
  T_8_DESCRIBE,
  T_8_DISTINCT,
  T_8_EXTERNAL,
  T_8_FUNCTION,
  T_8_GROUPING,
  T_8_IDENTITY,
  T_8_INTERVAL,
  T_8_LANGUAGE,
  T_8_MODIFIES,
  T_8_OVERLAPS,
  T_8_PRESERVE,
  T_8_RELATIVE,
  T_8_RESIGNAL,
  T_8_RESTRICT,
  T_8_ROLLBACK,
  T_8_SPECIFIC,
  T_8_SQLSTATE,
  T_8_TRAILING,
  T_8_TRUNCATE,
  T_8_WHENEVER,
  T_9_ASSERTION,
  T_9_COLLATION,
  T_9_CONDITION,
  T_9_EXCEPTION,
  T_9_IMMEDIATE,
  T_9_INDICATOR,
  T_9_INITIALLY,
  T_9_INTERSECT,
  T_9_ISOLATION,
  T_9_LOCALTIME,
  T_9_PARAMETER,
  T_9_PARTITION,
  T_9_PRECISION,
  T_9_PROCEDURE,
  T_9_RECURSIVE,
  T_9_SAVEPOINT,
  T_9_SENSITIVE,
  T_9_STATEMENT,
  T_9_SYMMETRIC,
  T_9_TEMPORARY,
  T_9_TIMESTAMP,
  T_10_ASENSITIVE,
  T_10_ASYMMETRIC,
  T_10_CONNECTION,
  T_10_CONSTRAINT,
  T_10_DEALLOCATE,
  T_10_DEFERRABLE,
  T_10_DESCRIPTOR,
  T_10_DISCONNECT,
  T_10_ORDINALITY,
  T_10_PRIVILEGES,
  T_10_REFERENCES,
  T_10_SQLWARNING,
  T_11_CONSTRAINTS,
  T_11_CONSTRUCTOR,
  T_11_DIAGNOSTICS,
  T_11_INSENSITIVE,
  T_11_REFERENCING,
  T_11_SYSTEM_USER,
  T_11_TRANSACTION,
  T_11_TRANSLATION,
  T_12_CURRENT_DATE,
  T_12_CURRENT_PATH,
  T_12_CURRENT_ROLE,
  T_12_CURRENT_TIME,
  T_12_CURRENT_USER,
  T_12_SESSION_USER,
  T_12_SPECIFICTYPE,
  T_12_SQLEXCEPTION,
  T_13_AUTHORIZATION,
  T_13_CORRESPONDING,
  T_13_DETERMINISTIC,
  T_13_TIMEZONE_HOUR,
  T_14_LOCALTIMESTAMP,
  T_15_TIMEZONE_MINUTE,
  T_17_CURRENT_TIMESTAMP
};

enum KindKeyword2 {
  T_3_DAY = 1,
  T_3_MAX,
  T_3_MIN,
  T_3_PAD,
  T_3_SUM,
  T_4_CAST,
  T_4_HOUR,
  T_4_LAST,
  T_4_SIZE,
  T_5_COUNT,
  T_5_FIRST,
  T_5_FOUND,
  T_5_MONTH,
  T_6_MINUTE,
  T_6_SECOND,
  T_9_SUBSTRING
};

enum KindKeyword3 {
  T_3_BIT = 1,
  T_3_INT,
  T_4_BLOB,
  T_4_CHAR,
  T_4_CLOB,
  T_4_DATE,
  T_4_YEAR,
  T_4_ZONE,
  T_5_FLOAT,
  T_5_NCHAR,
  T_5_NCLOB,
  T_6_BINARY,
  T_6_DOUBLE,
  T_6_OBJECT,
  T_7_BOOLEAN,
  T_7_DECIMAL,
  T_7_INTEGER,
  T_7_NATURAL,
  T_7_NUMERIC,
  T_7_VARCHAR,
  T_7_VARYING,
  T_8_NATIONAL,
  T_8_SMALLINT,
  T_9_CHARACTER
};

class SQL_EXPORT Lexer
{
    Lexer( const Lexer &other );
    void operator = ( const Lexer &other );

public:
    enum State {
        State_Default,
        State_MultiLineComment
    };

    Lexer(TranslationUnit *unit);
    Lexer( const char *firstChar, const char *lastChar, ClassifyFunc classifyFunc );
    ~Lexer();

    void scan( Token *tok, unsigned prevKind );

    inline void operator()( Token *tok, unsigned prevKind )
    { scan(tok, prevKind); }

    unsigned tokenOffset() const;
    unsigned tokenLength() const;
    const char *tokenBegin() const;
    const char *tokenEnd() const;
    unsigned currentLine() const;

    bool scanKeywords() const;
    void setScanKeywords(bool onoff);

    void setStartWithNewline(bool enabled);

    int state() const;
    void setState( int state );

    void setClassifyFunc( ClassifyFunc classifyFunc ) { _classifyFunc = classifyFunc; }

private:
    void scan_helper( Token *tok , unsigned prevKind );
    void setSource( const char *firstChar, const char *lastChar );

    inline void yyinp()
    {
        if (++_currentChar == _lastChar)
            _yychar = 0;
        else {
            _yychar = *_currentChar;
            if (_yychar == '\n') {
                ++_currentLine;
                if (_translationUnit)
                    _translationUnit->pushLineOffset(_currentChar - _firstChar);
            }
        }
    }

    inline unsigned char nextChar()
    {
        if ( _currentChar + 1 == _lastChar )
            return 0;
        return *(_currentChar + 1);
    }

private:
    TranslationUnit *_translationUnit;
    struct Flags {
        unsigned _scanKeywords: 1;
    };

    const char *_firstChar;
    const char *_currentChar;
    const char *_lastChar;
    const char *_tokenStart;
    unsigned char _yychar;
    int _state;
    union {
        unsigned _flags;
        Flags f;
    };
    unsigned _currentLine;
    ClassifyFunc _classifyFunc;
};

} // Sql

#endif // SQLLEXER_H
