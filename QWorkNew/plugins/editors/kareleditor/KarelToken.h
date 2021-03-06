#ifndef KAREL_TOKEN_H
#define KAREL_TOKEN_H

namespace Karel {

enum Kind {
    T_EOF_SYMBOL = 0,
    T_ERROR,

    T_COMMENT,

    T_IDENTIFIER,
    T_NULL_IDENTIFIER,

    T_FIRST_LITERAL,
    T_NUMERIC_LITERAL = T_FIRST_LITERAL,
    T_STRING_LITERAL,
    T_LAST_LITERAL = T_STRING_LITERAL,

    T_FIRST_OPERATOR,
    T_AMPER = T_FIRST_OPERATOR,
    T_CARET,
    T_COLON,
    T_COMMA,
    T_SLASH,
    T_DOT,
    T_EQUAL,
    T_EXCLAIM,
    T_GREATER,
    T_GREATER_EQUAL,
    T_LBRACE,
    T_LBRACKET,
    T_LESS,
    T_LESS_EQUAL,
    T_LESS_GREATER,
    T_LPAREN,
    T_MINUS,
    T_PIPE,
    T_PIPE_PIPE,
    T_PLUS,
    T_RBRACE,
    T_RBRACKET,
    T_RPAREN,
    T_SEMICOLON,
    T_STAR,
    T_LAST_OPERATOR = T_STAR,

    T_KEYWORD
};

class Literal
{
    Literal(const Literal &other);
    void operator =(const Literal &other);

public:
    typedef const char *iterator;
    typedef iterator const_iterator;

public:
    Literal(const char *chars, unsigned size);
    virtual ~Literal();

    iterator begin() const;
    iterator end() const;

    char at(unsigned index) const;
    const char *chars() const;
    const char *charsOrig() const;
    unsigned size() const;

    unsigned hashCode() const;
    static unsigned hashCode(const char *chars, unsigned size);

    bool equalTo(const Literal *other) const;

private:
    char *_chars;
    char *_charsOrig;
    unsigned _size;
    unsigned _hashCode;
};

class StringLiteral: public Literal
{
public:
    StringLiteral(const char *chars, unsigned size);
    virtual ~StringLiteral();
};

class NumericLiteral: public Literal
{
public:
    NumericLiteral(const char *chars, unsigned size);
    virtual ~NumericLiteral();

    bool isInt() const;
    bool isDouble() const;

private:
    struct Flags {
        unsigned _type      : 8;
    };
    union {
        unsigned _flags;
        Flags f;
    };
};

class Identifier: public Literal
{
public:
    Identifier(const char *chars, unsigned size);
    virtual ~Identifier();
};

class Token
{
public:
    Token();
    virtual ~Token();

    inline bool is(unsigned k) const    { return f.kind == k; }
    inline bool isNot(unsigned k) const { return f.kind != k; }
    const char *spell() const;
    void reset();

    inline unsigned kind() const { return f.kind; }
    inline bool newline() const { return f.newline; }
    inline bool whitespace() const { return f.whitespace; }
    inline bool joined() const { return f.joined; }
    inline bool expanded() const { return f.expanded; }
    inline bool generated() const { return f.generated; }
    inline unsigned length() const { return f.length; }

    inline unsigned begin() const
    { return offset; }

    inline unsigned end() const
    { return offset + f.length; }

    inline bool isLiteral() const
    { return f.kind >= T_FIRST_LITERAL && f.kind <= T_LAST_LITERAL; }

    inline bool isOperator() const
    { return f.kind >= T_FIRST_OPERATOR && f.kind <= T_LAST_OPERATOR; }

    inline bool isKeyword()  const { return keywordFlags != 0; }
    inline bool isIdentifier() const { return f.kind == T_IDENTIFIER; }

    inline bool isKeyword1() const { return kf.keyword1; }
    inline bool isKeyword2() const { return kf.keyword2; }
    inline bool isKeyword3() const { return kf.keyword3; }
    inline bool isKeyword4() const { return kf.keyword4; }
    inline bool isKeyword5() const { return kf.keyword5; }
    inline bool isKeyword6() const { return kf.keyword6; }
    inline bool isKeyword7() const { return kf.keyword7; }
    inline bool isKeyword8() const { return kf.keyword8; }

    inline unsigned keywordType() const { return kf.keytype; }

    inline unsigned keyid() const { return kf.keyid; }

    inline bool isComment() const
    { return f.kind == T_COMMENT; }


    static const char *name(int kind);
    static const char *name1(int kind);

    struct Flags {
        unsigned kind       : 8;
        unsigned newline    : 1;
        unsigned whitespace : 1;
        unsigned joined     : 1;
        unsigned expanded   : 1;
        unsigned generated  : 1;
        unsigned pad        : 3;
        unsigned length     : 16;
    };
    union {
        unsigned flags;
        Flags f;
    };

    unsigned offset;

    union {
        void *ptr;
        const Literal           *literal;
        const NumericLiteral    *number;
        const StringLiteral     *string;
        const Identifier        *identifier;
        unsigned close_brace;
        unsigned lineno;
    };

    struct KeywordFlags {
        unsigned keyword1 : 1;
        unsigned keyword2 : 1;
        unsigned keyword3 : 1;
        unsigned keyword4 : 1;
        unsigned keyword5 : 1;
        unsigned keyword6 : 1;
        unsigned keyword7 : 1;
        unsigned keyword8 : 1;
        unsigned keyid    : 16;
        unsigned keytype  : 8;
    };
    union {
        unsigned keywordFlags;
        KeywordFlags kf;
    };

};

}   // namespace Karel

#endif // KAREL_TOKEN_H
