#include <string>
#include <cstring>

#include "KarelToken.h"

namespace Karel {

static const char *token_names[] = {
    (""),
    ("<error>"),

    ("<comment>"),

    ("<identifier>"),
    ("<identifier>"),

    ("<numeric literal>"),
    ("<string literal>"),

    ("&"),
    ("^"),
    (":"),
    (","),
    ("/"),
    ("."),
    ("="),
    ("!"),
    (">"),
    (">="),
    ("{"),
    ("["),
    ("<"),
    ("<="),
    ("<>"),
    ("("),
    ("-"),
    ("|"),
    ("||"),
    ("+"),
    ("}"),
    ("]"),
    (")"),
    (";"),
    ("*")
};

////////////////////////////////////////////////////////////////////////////////
/// Literal
Literal::Literal(const char *chars, unsigned size)
{
    _chars = new char[size + 1];
    _charsOrig = new char[size + 1];

    for (unsigned i = 0; i < size; i++) {
        _chars[i] = chars[i];
        _charsOrig[i] = chars[i];
        if ( _chars[i] >= 'a' && _chars[i] <= 'z' )
            _chars[i] -= 32;
    }

    _chars[size] = '\0';
    _charsOrig[size] = '\0';
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

const char *Literal::charsOrig() const
{ return _charsOrig; }

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
    : Literal(chars, size)
{ }

StringLiteral::~StringLiteral()
{ }

////////////////////////////////////////////////////////////////////////////////
/// NumericLiteral
enum {
    NumericLiteralIsInt,
    NumericLiteralIsDouble
};

NumericLiteral::NumericLiteral(const char *chars, unsigned size)
    : Literal(chars, size), _flags(0)
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
Identifier::Identifier( const char *chars, unsigned size )
    : Literal(chars, size)
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
{ }

Token::~Token()
{ }

static const char *num_spell = "<NUMERIC>";
static const char *string_spell = "<STRING>";

const char *Token::spell() const
{
    if ( isKeyword() )
        return identifier->chars();

    switch (f.kind) {
        case T_IDENTIFIER:      return identifier->chars();
        case T_NUMERIC_LITERAL: return num_spell;
        case T_STRING_LITERAL:  return string_spell;
    }

    return name(f.kind);
}

void Token::reset()
{
    flags = 0;
    offset = 0;
    ptr = 0;
    keywordFlags = 0;
}

const char *Token::name(int kind)
{ return token_names[kind]; }

}   // namespace Karel
