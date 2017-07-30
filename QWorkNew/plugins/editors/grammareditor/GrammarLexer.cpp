#include <QByteArray>
#include <QString>
#include <QDebug>

#include "GrammarLexer.h"
#include "Lexer.h"
#include "Literals.h"

namespace Grammar {

static void incState( int &state )
{
    switch (state) {
        case Declaration:       state = Definition_StartRule;   break;
        default:                state = CppSection;             break;
    }
}

QList<CPlusPlus::Token> GrammarLexer::operator()(const QString &text, int state, int grammarState, int braceDepth)
{
    QList<CPlusPlus::Token> tokens;

    const QByteArray bytes = text.toLatin1();
    const char *firstChar = bytes.constData();
    const char *lastChar = firstChar + bytes.size();

    if ( braceDepth < 0 )
        braceDepth = 0;

    CPlusPlus::Lexer lex(firstChar, lastChar);
    lex.setQtMocRunEnabled(false);
    lex.setObjCEnabled(false);
    lex.setStartWithNewline(true);
    lex.setObjCEnabled(false);
    lex.setSingleLineCommentEnabled(_singleLineCommentsEnabled);
    lex.setExtraExecSqlEnabled(false);

    if (! _skipComments)
        lex.setScanCommentTokens(true);

    if (state != -1)
        lex.setState(state & 0xff);
    lex.setSqlState(0);

    bool inPreproc = false;
    bool cppCode = ( (braceDepth > 0 && grammarState == Definition_BodyRule) ||
                     (braceDepth > 0 && grammarState == Declaration) ||
                     grammarState >= CppSection );
    lex.setScanKeywords(cppCode);

    unsigned prevTokenKind = CPlusPlus::T_EOF_SYMBOL;
    for (;;) {
        CPlusPlus::Token tk;
        lex(&tk);
        if (tk.is(CPlusPlus::T_EOF_SYMBOL)) {
            _endedJoined = tk.joined();
            break;
        }

        if ( !cppCode ) {
            inPreproc = false;
            if ( tk.is(CPlusPlus::T_PERCENT_PERCENT) )
                incState(grammarState);
            else if ( prevTokenKind == CPlusPlus::T_PERCENT && tk.is(CPlusPlus::T_IDENTIFIER) && grammarState == Declaration ) {
                QStringRef ident = text.midRef(lex.tokenOffset(), lex.tokenLength());
                if ( ident == QLatin1String("token") || ident == QLatin1String("type") ||
                     ident == QLatin1String("left") || ident == QLatin1String("right") ||
                     ident == QLatin1String("start") || ident == QLatin1String("nonassoc") ||
                     ident == QLatin1String("union") )
                    tk.f.kind = T_GR_KEYWORD;
            }
            else if ( grammarState >= Definition_StartRule && grammarState <= Definition_BodyRule ) {
                if ( tk.is(CPlusPlus::T_SEMICOLON) ) {
                    grammarState = Definition_StartRule;
                } else if ( tk.is(CPlusPlus::T_COLON) ) {
                    grammarState = Definition_BodyRule;
                } else if ( tk.is(CPlusPlus::T_IDENTIFIER) ) {
                    tk.f.kind = ( grammarState == Definition_StartRule ? T_GR_RULE_DEF_IDENTIFIER : T_GR_RULE_ITEM_IDENTIFIER );
                }
            }
        }

        else {
            QStringRef spell = text.midRef(lex.tokenOffset(), lex.tokenLength());
            lex.setScanAngleStringLiteralTokens(false);
            if (tk.f.newline && tk.is(CPlusPlus::T_POUND))
                inPreproc = true;
            else if (inPreproc && tokens.size() == 1 && tk.is(CPlusPlus::T_IDENTIFIER) && spell == QLatin1String("include")) {
                lex.setScanAngleStringLiteralTokens(true);
                tk.f.kind = CPlusPlus::T_IDENTIFIER;
            } else if ( tk.is(CPlusPlus::T_IDENTIFIER) && spell.at(0) == '$' && spell.length() > 1 && grammarState == Definition_BodyRule ) {
                if ( spell.length() == 2 && spell.at(1) == '$' ) {
                    tk.f.kind = T_GR_VARIABLE_IDENTIFIER;
                } else {
                    bool isDigit = true;
                    for (int i = 1; i < spell.length(); i++) {
                        if ( spell.at(i) < '0' || spell.at(i) > '9' ) {
                            isDigit = false;
                            break;
                        }
                    }
                    if ( isDigit )
                        tk.f.kind = T_GR_VARIABLE_IDENTIFIER;
                }
            }
        }

        if ( tk.is(CPlusPlus::T_LBRACE) )
            braceDepth++;
        else if ( tk.is(CPlusPlus::T_RBRACE) && braceDepth > 0 )
            braceDepth--;

        cppCode = ( (braceDepth > 0 && grammarState == Definition_BodyRule) ||
                    (braceDepth > 0 && grammarState == Declaration) ||
                    grammarState >= CppSection );
        lex.setScanKeywords(cppCode);
        prevTokenKind = tk.f.kind;

        tokens.append(tk);
    }

    _lastState = lex.state();
    _lastGrammarState = grammarState;
    return tokens;
}

}   // namespace Grammar
