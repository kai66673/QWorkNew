#ifndef GRAMMARLEXER_H
#define GRAMMARLEXER_H

#include <QList>

#include "Token.h"

namespace Grammar {

enum Kind {
    T_GR_KEYWORD = CPlusPlus::T_LAST_CPP_KIND + 1,
    T_GR_RULE_DEF_IDENTIFIER,
    T_GR_RULE_ITEM_IDENTIFIER,
    T_GR_VARIABLE_IDENTIFIER
};

enum GrammarState {
    Declaration = 0,
    Definition_StartRule,
    Definition_BodyRule,
    CppSection
};

class GrammarLexer
{
public:
    GrammarLexer()
        : _lastState(0)
        , _lastGrammarState(0)
        , _skipComments(false)
        , _endedJoined(false)
        , _singleLineCommentsEnabled(false)
    {}

    ~GrammarLexer() {}

    QList<CPlusPlus::Token> operator()(const QString &text, int state = 0, int grState = 0, int braceDepth = 0);

    void onFormatSettingsChanged( bool forceRehilight = true );

    int state() const
    { return _lastState; }

    int grState() const
    { return qMin(_lastGrammarState, 3); }

private:
    int _lastState;
    int _lastGrammarState;
    bool _skipComments: 1;
    bool _endedJoined: 1;
    bool _singleLineCommentsEnabled: 1;
};

}   // namespace Grammar

#endif // GRAMMARLEXER_H
