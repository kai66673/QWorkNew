#include <cctype>
#include <QStringRef>

#include <QDebug>

#include "SqlLexer.h"
#include "Control.h"

namespace Sql {

Lexer::Lexer(TranslationUnit *unit)
    : _translationUnit(unit),
      _state(State_Default),
      _flags(0),
      _currentLine(1)
{
    f._scanKeywords = true;
    setSource(_translationUnit->firstSourceChar(),
              _translationUnit->lastSourceChar());
}
Lexer::Lexer( const char *firstChar, const char *lastChar, ClassifyFunc classifyFunc )
    : _translationUnit(0)
    , _state(State_Default)
    , _flags(0)
    , _currentLine(1)
    , _classifyFunc(classifyFunc)
{
    setSource(firstChar, lastChar);
}

Lexer::~Lexer()
{ }

void Lexer::setSource( const char *firstChar, const char *lastChar )
{
    _firstChar = firstChar;
    _lastChar = lastChar;
    _currentChar = _firstChar - 1;
    _tokenStart = _currentChar;
    _yychar = '\n';
}

void Lexer::setStartWithNewline( bool enabled )
{
    if (enabled)
        _yychar = '\n';
    else
        _yychar = ' ';
}

int Lexer::state() const
{ return _state; }

void Lexer::setState( int state )
{ _state = state; }

bool Lexer::scanKeywords() const
{ return f._scanKeywords; }

void Lexer::setScanKeywords( bool onoff )
{ f._scanKeywords = onoff; }

unsigned Lexer::tokenOffset() const
{ return _tokenStart - _firstChar; }

unsigned Lexer::tokenLength() const
{ return _currentChar - _tokenStart; }

const char *Lexer::tokenBegin() const
{ return _tokenStart; }

const char *Lexer::tokenEnd() const
{ return _currentChar; }

unsigned Lexer::currentLine() const
{ return _currentLine; }

void Lexer::scan( Token *tok , unsigned prevKind )
{
    tok->reset();
    scan_helper(tok, prevKind);
    tok->f.length = _currentChar - _tokenStart;
}

void Lexer::scan_helper( Token *tok, unsigned prevKind )
{
    while (_yychar && std::isspace(_yychar)) {
        if (_yychar == '\n') {
            tok->f.joined = false;
            tok->f.newline = true;
        } else {
            tok->f.whitespace = true;
        }
        yyinp();
    }

    _tokenStart = _currentChar;
    tok->offset = _currentChar - _firstChar;

    if ( _state == State_MultiLineComment ) {

        if ( !_yychar ) {
            tok->f.kind = T_EOF_SYMBOL;
            return;
        }

        while ( _yychar ) {
            if ( _yychar != '*' )
                yyinp();
            else {
                yyinp();
                if ( _yychar == '/' ) {
                    yyinp();
                    _state = State_Default;
                    break;
                }
            }
        }

        tok->f.kind = T_MULTYLINE_COMMENT;
        return; // done
    }

    if ( !_yychar ) {
        tok->f.kind = T_EOF_SYMBOL;
        return;
    }

    unsigned char ch = _yychar;
    yyinp();

    switch (ch) {
        case '"':
        {
            tok->f.kind = T_STRING_LITERAL;
            const char *yytext = _currentChar - 1;

            while ( _yychar && _yychar != '"' ) {
                if ( _yychar == '\n' )
                    break;
                yyinp();
            }

            if ( _yychar == '"' )
                yyinp();
            int yylen = _currentChar - yytext;
            if ( _translationUnit )
                tok->identifier = _translationUnit->control()->identifier(yytext + 1, yylen - 2, true);
        } break;
        case '\'':
        {
            tok->f.kind = T_CHAR_LITERAL;

            while ( _yychar && _yychar != '\'' ) {
                if ( _yychar == '\n' )
                    break;
                yyinp();
            }

            if ( _yychar == '\'' )
                yyinp();
        } break;
        case '{':
            tok->f.kind = T_LBRACE;
            break;

        case '}':
            tok->f.kind = T_RBRACE;
            break;

        case '[':
            tok->f.kind = T_LBRACKET;
            break;

        case ']':
            tok->f.kind = T_RBRACKET;
            break;

        case '(':
            tok->f.kind = T_LPAREN;
            break;

        case ')':
            tok->f.kind = T_RPAREN;
            break;

        case ';':
            tok->f.kind = T_SEMICOLON;
            break;

        case ':':
            if ( _yychar == '=' ) {
                yyinp();
                tok->f.kind = T_ASSIGN;
            } else {
                tok->f.kind = T_COLON;
            }
            break;

        case '.':
            if ( std::isdigit(_yychar) ) {
                do {
                    if ( _yychar == 'e' || _yychar == 'E' ) {
                        yyinp();
                        if ( _yychar == '-' || _yychar == '+' ) {
                            yyinp();
                        }
                    } else if ( std::isalnum(_yychar) || _yychar == '.' ) {
                        yyinp();
                    } else {
                        break;
                    }
                } while (_yychar);
                tok->f.kind = T_NUMERIC_LITERAL;
            } else if ( _yychar == '.' ) {
                yyinp();
                tok->f.kind = T_DOTDOT;
            } else {
                tok->f.kind = T_DOT;
            }
            break;

        case '+':
            tok->f.kind = T_PLUS;
            break;

        case '*':
            tok->f.kind = T_STAR;
            break;

        case '-':
            if ( _yychar == '-' ) {
                yyinp();

                while ( _yychar && _yychar != '\n' )
                    yyinp();

                tok->f.kind = T_COMMENT;
            }
            else {
                tok->f.kind = T_MINUS;
            }
            break;

        case '/':
            if ( _yychar == '*' ) {
                yyinp();

                while ( _yychar ) {
                    if ( _yychar != '*' ) {
                        yyinp();
                    } else {
                        yyinp();
                        if ( _yychar == '/' )
                            break;
                    }
                }

                if ( _yychar )
                    yyinp();
                else
                    _state = State_MultiLineComment;

                tok->f.kind = T_MULTYLINE_COMMENT;
            }
            else {
                tok->f.kind = T_SLASH;
            }
            break;

        case '%':
            tok->f.kind = T_PERCENT;
            break;

        case '^':
            tok->f.kind = T_CARET;
            break;

        case '&':
            tok->f.kind = T_AMPER;
            break;

        case '|':
            if ( _yychar == '|' ) {
                yyinp();
                tok->f.kind = T_PIPE_PIPE;
            } else {
                tok->f.kind = T_PIPE;
            }
            break;

        case '!':
            if (_yychar == '=') {
                yyinp();
                tok->f.kind = T_EXCLAIM_EQUAL;
            } else {
                tok->f.kind = T_EXCLAIM;
            }
            break;

        case '=':
            tok->f.kind = T_EQUAL;
            break;

        case '<':
            if ( _yychar == '>' ) {
                yyinp();
                tok->f.kind = T_LESS_GREATER;
            } else if ( _yychar == '=' ) {
                yyinp();
                tok->f.kind = T_LESS_EQUAL;
            } else {
                tok->f.kind = T_LESS;
            }
            break;

        case '>':
            if (_yychar == '=') {
                yyinp();
                tok->f.kind = T_GREATER_EQUAL;
            } else {
                tok->f.kind = T_GREATER;
            }
            break;

        case ',':
            tok->f.kind = T_COMMA;
            break;

        default:
            if ( std::isalpha(ch) || ch == '_' || ch == '$' ) {
                const char *yytext = _currentChar - 1;
                while ( std::isalnum(_yychar) || _yychar == '_' || _yychar == '$' )
                    yyinp();
                int yylen = _currentChar - yytext;
                if ( f._scanKeywords )
                    (*_classifyFunc)(yytext, yylen, *tok);
                tok->f.kind = T_IDENTIFIER;
                if ( tok->isKeyword() )
                    tok->f.kind = T_KEYWORD;
                if ( _translationUnit )
                    tok->identifier = _translationUnit->control()->identifier(yytext, yylen, prevKind == T_COLON ? false : true);
            } else if ( std::isdigit(ch) ) {
                while ( _yychar ) {
                    if ( _yychar == 'e' || _yychar == 'E' ) {
                        yyinp();
                        if ( _yychar == '-' || _yychar == '+' ) {
                            yyinp();
                        }
                    } else if ( std::isalnum(_yychar) || _yychar == '.' ) {
                        yyinp();
                    } else {
                        break;
                    }
                }
                tok->f.kind = T_NUMERIC_LITERAL;
            } else {
                tok->f.kind = T_ERROR;
            }
            break;
    }
}

} // Sql
