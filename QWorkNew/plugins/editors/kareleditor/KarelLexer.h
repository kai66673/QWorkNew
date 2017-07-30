#ifndef KAREL_LEXER_H
#define KAREL_LEXER_H

#include <QList>

#include "KarelToken.h"
#include "TranslationUnit.h"

namespace Karel {

class Lexer
{
    Lexer( const Lexer &other );
    void operator = ( const Lexer &other );

public:
    Lexer(TranslationUnit *unit);
    Lexer( const char *firstChar, const char *lastChar );
    ~Lexer();

    void scan(Token *tok);

    inline void operator()( Token *tok )
    { scan(tok); }

    unsigned tokenOffset() const;
    unsigned tokenLength() const;
    const char *tokenBegin() const;
    const char *tokenEnd() const;
    unsigned currentLine() const;

    bool scanKeywords() const;
    void setScanKeywords(bool onoff);

    void setStartWithNewline(bool enabled);

private:
    void scan_helper(Token *tok );
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
    union {
        unsigned _flags;
        Flags f;
    };
    unsigned _currentLine;
};

} // namespace Karel

#endif // KAREL_LEXER_H
