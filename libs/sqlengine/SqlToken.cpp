#include <cstring>
#include <cstdlib>
#include <string>

#include "SqlToken.h"

namespace Sql {

static const char *token_names[] = {
    (""),
    ("<error>"),

    ("<SQL comment>"),
    ("<SQL multyline comment>"),

    ("<identifier>"),

    ("<numeric literal>"),
    ("<char literal>"),
    ("<string literal>"),

    ("&"),
    ("^"),
    (":"),
    (","),
    ("/"),
    ("."),
    ("="),
    ("!"),
    ("!="),
    (">"),
    (">="),
    ("{"),
    ("["),
    ("<"),
    ("<="),
    ("<>"),
    ("("),
    ("-"),
    ("%"),
    ("|"),
    ("||"),
    ("+"),
    ("}"),
    ("]"),
    (")"),
    (";"),
    ("*"),


    // SQL keywords
    ("AS"), ("BY"), ("IN"), ("IS"), ("OF"), ("ON"), ("OR"), ("TO"),
    ("ADD"), ("ALL"), ("AND"), ("ASC"), ("END"), ("FOR"), ("KEY"), ("MAX"), ("MIN"), ("NOT"), ("ROW"), ("SET"), ("SQL"), ("SUM"),
    ("DESC"), ("DROP"), ("EXEC"), ("FROM"), ("GOTO"), ("HOLD"), ("INTO"), ("JOIN"), ("LEFT"), ("LIKE"), ("LIST"), ("OPEN"), ("ONLY"), ("READ"), ("ROWS"), ("WITH"),
    ("ALIAS"), ("ALTER"), ("BEGIN"), ("CLOSE"), ("COUNT"), ("CROSS"), ("FETCH"), ("GROUP"), ("INDEX"), ("LIMIT"), ("ORDER"), ("RIGHT"), ("TABLE"), ("WHERE"), ("WRITE"),
    ("COLUMN"), ("CURSOR"), ("DELETE"), ("INSERT"), ("MODULE"), ("SELECT"), ("SHARED"), ("UNIQUE"), ("UPDATE"), ("VALUES"),
    ("BETWEEN"), ("CURRENT"), ("DECLARE"), ("INCLUDE"), ("PREPARE"), ("PRIMARY"),
    ("C$INCLUDE"), ("DISTINCT"), ("EXTERNAL"), ("FILENAME"), ("FUNCTION"), ("PRESERVE"), ("SQLERROR"), ("TRUNCATE"), ("WHENEVER"),
    ("EXCLUSIVE"), ("PROCEDURE"), ("RESERVING"),
    ("CONSTRAINT"), ("SQLWARNING"), ("TRANSACTION")

};

/*
 keyword1 - стандартные для SQL
 kewword2 - стандартные встроенные функции
 keyword3 - типы данных
*/

static const char *token_keyword1_names[] = {
  (""),
  ("AS"),
  ("AT"),
  ("BY"),
  ("DO"),
  ("GO"),
  ("IF"),
  ("IN"),
  ("IS"),
  ("NO"),
  ("OF"),
  ("ON"),
  ("OR"),
  ("TO"),
  ("ADD"),
  ("ALL"),
  ("AND"),
  ("ANY"),
  ("ARE"),
  ("ASC"),
  ("DEC"),
  ("END"),
  ("FOR"),
  ("GET"),
  ("KEY"),
  ("MAP"),
  ("NEW"),
  ("NOT"),
  ("OLD"),
  ("OUT"),
  ("REF"),
  ("ROW"),
  ("SET"),
  ("SQL"),
  ("BOTH"),
  ("CALL"),
  ("CASE"),
  ("CUBE"),
  ("DATA"),
  ("DESC"),
  ("DROP"),
  ("EACH"),
  ("ELSE"),
  ("EXEC"),
  ("EXIT"),
  ("FREE"),
  ("FROM"),
  ("FULL"),
  ("GOTO"),
  ("HOLD"),
  ("INTO"),
  ("JOIN"),
  ("LEFT"),
  ("LIKE"),
  ("LOOP"),
  ("NEXT"),
  ("NONE"),
  ("NULL"),
  ("ONLY"),
  ("OPEN"),
  ("OVER"),
  ("PATH"),
  ("READ"),
  ("REAL"),
  ("ROLE"),
  ("ROWS"),
  ("SETS"),
  ("SOME"),
  ("THEN"),
  ("TIME"),
  ("TRUE"),
  ("UNDO"),
  ("USER"),
  ("VIEW"),
  ("WHEN"),
  ("WITH"),
  ("WORK"),
  ("AFTER"),
  ("ALTER"),
  ("ARRAY"),
  ("BEGIN"),
  ("CHECK"),
  ("CLOSE"),
  ("CROSS"),
  ("CYCLE"),
  ("DEPTH"),
  ("DEREF"),
  ("FALSE"),
  ("FETCH"),
  ("GRANT"),
  ("GROUP"),
  ("INDEX"),
  ("INNER"),
  ("INOUT"),
  ("INPUT"),
  ("LARGE"),
  ("LEAVE"),
  ("LEVEL"),
  ("LIMIT"),
  ("LOCAL"),
  ("MATCH"),
  ("MINUS"),
  ("NAMES"),
  ("ORDER"),
  ("OUTER"),
  ("PRIOR"),
  ("RANGE"),
  ("READS"),
  ("RIGHT"),
  ("SCOPE"),
  ("SPACE"),
  ("START"),
  ("STATE"),
  ("TABLE"),
  ("TREAT"),
  ("UNDER"),
  ("UNION"),
  ("UNTIL"),
  ("USAGE"),
  ("USING"),
  ("VALUE"),
  ("WHERE"),
  ("WHILE"),
  ("WRITE"),
  ("ACTION"),
  ("ATOMIC"),
  ("BEFORE"),
  ("COLUMN"),
  ("COMMIT"),
  ("CREATE"),
  ("CURSOR"),
  ("DELETE"),
  ("DOMAIN"),
  ("ELSEIF"),
  ("EQUALS"),
  ("ESCAPE"),
  ("EXCEPT"),
  ("EXISTS"),
  ("FILTER"),
  ("GLOBAL"),
  ("HAVING"),
  ("INSERT"),
  ("METHOD"),
  ("MODULE"),
  ("OPTION"),
  ("OUTPUT"),
  ("PUBLIC"),
  ("REPEAT"),
  ("RESULT"),
  ("RETURN"),
  ("REVOKE"),
  ("ROLLUP"),
  ("SCHEMA"),
  ("SCROLL"),
  ("SEARCH"),
  ("SELECT"),
  ("SIGNAL"),
  ("STATIC"),
  ("SYSTEM"),
  ("UNIQUE"),
  ("UNNEST"),
  ("UPDATE"),
  ("VALUES"),
  ("WINDOW"),
  ("WITHIN"),
  ("BETWEEN"),
  ("BREADTH"),
  ("CASCADE"),
  ("CATALOG"),
  ("COLLATE"),
  ("COMMENT"),
  ("CONNECT"),
  ("CURRENT"),
  ("DECLARE"),
  ("DEFAULT"),
  ("DYNAMIC"),
  ("EXECUTE"),
  ("FOREIGN"),
  ("GENERAL"),
  ("HANDLER"),
  ("ITERATE"),
  ("LATERAL"),
  ("LEADING"),
  ("LOCATOR"),
  ("PARTIAL"),
  ("PREPARE"),
  ("PRIMARY"),
  ("RELEASE"),
  ("REPLACE"),
  ("RETURNS"),
  ("ROUTINE"),
  ("SECTION"),
  ("SESSION"),
  ("SIMILAR"),
  ("TRIGGER"),
  ("UNKNOWN"),
  ("WITHOUT"),
  ("ABSOLUTE"),
  ("ALLOCATE"),
  ("CASCADED"),
  ("CONTINUE"),
  ("DEFERRED"),
  ("DESCRIBE"),
  ("DISTINCT"),
  ("EXTERNAL"),
  ("FUNCTION"),
  ("GROUPING"),
  ("IDENTITY"),
  ("INTERVAL"),
  ("LANGUAGE"),
  ("MODIFIES"),
  ("OVERLAPS"),
  ("PRESERVE"),
  ("RELATIVE"),
  ("RESIGNAL"),
  ("RESTRICT"),
  ("ROLLBACK"),
  ("SPECIFIC"),
  ("SQLSTATE"),
  ("TRAILING"),
  ("TRUNCATE"),
  ("WHENEVER"),
  ("ASSERTION"),
  ("COLLATION"),
  ("CONDITION"),
  ("EXCEPTION"),
  ("IMMEDIATE"),
  ("INDICATOR"),
  ("INITIALLY"),
  ("INTERSECT"),
  ("ISOLATION"),
  ("LOCALTIME"),
  ("PARAMETER"),
  ("PARTITION"),
  ("PRECISION"),
  ("PROCEDURE"),
  ("RECURSIVE"),
  ("SAVEPOINT"),
  ("SENSITIVE"),
  ("STATEMENT"),
  ("SYMMETRIC"),
  ("TEMPORARY"),
  ("TIMESTAMP"),
  ("ASENSITIVE"),
  ("ASYMMETRIC"),
  ("CONNECTION"),
  ("CONSTRAINT"),
  ("DEALLOCATE"),
  ("DEFERRABLE"),
  ("DESCRIPTOR"),
  ("DISCONNECT"),
  ("ORDINALITY"),
  ("PRIVILEGES"),
  ("REFERENCES"),
  ("SQLWARNING"),
  ("CONSTRAINTS"),
  ("CONSTRUCTOR"),
  ("DIAGNOSTICS"),
  ("INSENSITIVE"),
  ("REFERENCING"),
  ("SYSTEM_USER"),
  ("TRANSACTION"),
  ("TRANSLATION"),
  ("CURRENT_DATE"),
  ("CURRENT_PATH"),
  ("CURRENT_ROLE"),
  ("CURRENT_TIME"),
  ("CURRENT_USER"),
  ("SESSION_USER"),
  ("SPECIFICTYPE"),
  ("SQLEXCEPTION"),
  ("AUTHORIZATION"),
  ("CORRESPONDING"),
  ("DETERMINISTIC"),
  ("TIMEZONE_HOUR"),
  ("LOCALTIMESTAMP"),
  ("TIMEZONE_MINUTE"),
  ("CURRENT_TIMESTAMP")
};

static const char *token_keyword2_names[] = {
  (""),
  ("DAY"),
  ("MAX"),
  ("MIN"),
  ("PAD"),
  ("SUM"),
  ("CAST"),
  ("HOUR"),
  ("LAST"),
  ("SIZE"),
  ("COUNT"),
  ("FIRST"),
  ("FOUND"),
  ("MONTH"),
  ("MINUTE"),
  ("SECOND"),
  ("SUBSTRING")
};

static const char *token_keyword3_names[] = {
  (""),
  ("BIT"),
  ("INT"),
  ("BLOB"),
  ("CHAR"),
  ("CLOB"),
  ("DATE"),
  ("YEAR"),
  ("ZONE"),
  ("FLOAT"),
  ("NCHAR"),
  ("NCLOB"),
  ("BINARY"),
  ("DOUBLE"),
  ("OBJECT"),
  ("BOOLEAN"),
  ("DECIMAL"),
  ("INTEGER"),
  ("NATURAL"),
  ("NUMERIC"),
  ("VARCHAR"),
  ("VARYING"),
  ("NATIONAL"),
  ("SMALLINT"),
  ("CHARACTER")
};

////////////////////////////////////////////////////////////////////////////////
/// Literal
Literal::Literal(const char *chars, unsigned size, bool toUpp)
    : _next(0), _index(0)
{
    _chars = new char[size + 1];

    std::strncpy(_chars, chars, size);
    if ( toUpp ) {
        for (unsigned i = 0; i < size; i++ )
            if ( _chars[i] >= 'a' && _chars[i] <= 'z' )
                _chars[i] -= 32;
    }
    _chars[size] = '\0';
    _size = size;

    _hashCode = hashCode(_chars, _size);
}

Literal::~Literal()
{ delete[] _chars; }

bool Literal::equalTo(const Literal *other) const
{
    if (! other)
        return false;
    else if (this == other)
        return true;
    else if (hashCode() != other->hashCode())
        return false;
    else if (size() != other->size())
        return false;
    return ! std::strcmp(chars(), other->chars());
}

Literal::iterator Literal::begin() const
{ return _chars; }

Literal::iterator Literal::end() const
{ return _chars + _size; }

const char *Literal::chars() const
{ return _chars; }

char Literal::at(unsigned index) const
{ return _chars[index]; }

unsigned Literal::size() const
{ return _size; }

unsigned Literal::hashCode() const
{ return _hashCode; }

unsigned Literal::hashCode(const char *chars, unsigned size)
{
    unsigned h = 0;
    for (unsigned i = 0; i < size; ++i)
        h = (h >> 5) - h + chars[i];
    return h;
}

////////////////////////////////////////////////////////////////////////////////
/// StringLiteral
StringLiteral::StringLiteral(const char *chars, unsigned size)
    : Literal(chars, size, false)
{ }

StringLiteral::~StringLiteral()
{ }

////////////////////////////////////////////////////////////////////////////////
/// StringLiteral
CharLiteral::CharLiteral(const char *chars, unsigned size)
    : Literal(chars, size, false)
{ }

CharLiteral::~CharLiteral()
{ }

////////////////////////////////////////////////////////////////////////////////
/// NumericLiteral
enum {
    NumericLiteralIsInt,
    NumericLiteralIsDouble
};

NumericLiteral::NumericLiteral(const char *chars, unsigned size)
    : Literal(chars, size, false), _flags(0)
{
    f._type = NumericLiteralIsInt;

    const char *begin = chars;
    const char *end = begin + size;
    const char *it = end - 1;

    for (const char *dot = it; it != begin - 1; --it) {
        if (*dot == '.') {
            f._type = NumericLiteralIsDouble;
            break;
        }
    }
}

NumericLiteral::~NumericLiteral()
{ }

bool NumericLiteral::isInt() const
{ return f._type == NumericLiteralIsInt; }

bool NumericLiteral::isDouble() const
{ return f._type == NumericLiteralIsDouble; }

////////////////////////////////////////////////////////////////////////////////
/// Identifier
Identifier::Identifier( const char *chars, unsigned size, bool toUpp )
    : Literal(chars, size, toUpp)
{ }

Identifier::~Identifier()
{ }

////////////////////////////////////////////////////////////////////////////////
/// Token
Token::Token()
    : flags(0)
    , offset(0)
    , ptr(0)
    , keywordFlags(0)
    , m_kindGlobal(0)
{
}

Token::~Token()
{
}

void Token::reset()
{
    flags = 0;
    offset = 0;
    ptr = 0;
    keywordFlags = 0;
}

const char *Token::name(int kind)
{
    return token_names[kind];
}

const char *Token::name1(int kind)
{
    return token_keyword1_names[kind];
}

void Token::populateKeywords( QList<TextEditor::BasicProposalItem *> &items )
{
    {
        int count = sizeof(token_keyword1_names) / sizeof(char *);
        for (int i = 1; i < count; i++) {
            TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
            item->setIcon(QIcon(":/images/sql_ci_keyword.png"));
            item->setText(QLatin1String(token_keyword1_names[i]));
            items.append(item);
        }
    }

    {
        int count = sizeof(token_keyword2_names) / sizeof(char *);
        for (int i = 1; i < count; i++) {
            TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
            item->setIcon(QIcon(":/images/sql_ci_func.png"));
            item->setText(QLatin1String(token_keyword2_names[i]));
            items.append(item);
        }
    }

    {
        int count = sizeof(token_keyword3_names) / sizeof(char *);
        for (int i = 1; i < count; i++) {
            TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
            item->setIcon(QIcon(":/images/sql_ci_type.png"));
            item->setText(QLatin1String(token_keyword3_names[i]));
            items.append(item);
        }
    }
}

static const char *num_spell = "<NUMERIC>";
static const char *char_spell = "<CHARACTERS>";
static const char *string_spell = "<STRING>";

const char *Token::spell() const
{
    if ( isKeyword() ) {
//        switch (kf.keytype) {
//            case 1: return token_keyword1_names[kf.keyid];
//            case 2: return token_keyword2_names[kf.keyid];
//            case 3: return token_keyword3_names[kf.keyid];
//        }
//        return 0;
        return identifier->chars();
    }

    switch (f.kind) {
        case T_IDENTIFIER:
            return identifier->chars();

        case T_NUMERIC_LITERAL:
            return num_spell;
        case T_CHAR_LITERAL:
            return char_spell;
        case T_STRING_LITERAL:
            return string_spell;
    }

    return name(f.kind);
}

} // Sql
