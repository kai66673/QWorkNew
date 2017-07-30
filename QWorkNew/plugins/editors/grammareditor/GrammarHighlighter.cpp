#include <QDebug>

#include <basetextdocumentlayout.h>
#include "cppdoxygen.h"

#include "GrammarHighlighter.h"
#include "GrammarLexer.h"
#include "SettingsManager.h"
#include "grammareditorconstants.h"
#include "HighlighterFormatsManager.h"

namespace Grammar {

GrammarHighlighter::GrammarHighlighter( QTextDocument *document )
    : TextEditor::SyntaxHighlighter(document)
{
}

void GrammarHighlighter::highlightBlock( const QString &text )
{
    const int previousState = previousBlockState();
    int state = 0;
    int grammarState = 0;
    int initialBraceDepth = 0;
    if (previousState != -1) {
        state = previousState & 0x0f;
        grammarState = (previousState & 0xf0) >> 4;
        initialBraceDepth = previousState >> 8;
    }

    int braceDepth = initialBraceDepth;
    GrammarLexer tokenize;

    int initialState = state;

    const QList<CPlusPlus::Token> tokens = tokenize(text, initialState, grammarState, braceDepth);
    state = tokenize.state(); // refresh the state

    int foldingIndent = initialBraceDepth;
    if (TextEditor::TextBlockUserData *userData = TextEditor::BaseTextDocumentLayout::testUserData(currentBlock())) {
        userData->setFoldingIndent(0);
        userData->setFoldingStartIncluded(false);
        userData->setFoldingEndIncluded(false);
    }

    if (tokens.isEmpty()) {
        setCurrentBlockState(previousState);
        TextEditor::BaseTextDocumentLayout::clearParentheses(currentBlock());
        if (text.length()) // the empty line can still contain whitespace
            setFormat(0, text.length(), m_cppFormats[CppVisualWhitespace]);
        if ( grammarState == Definition_BodyRule )
            foldingIndent++;
        TextEditor::BaseTextDocumentLayout::setFoldingIndent(currentBlock(), foldingIndent);
        return;
    }

    const unsigned firstNonSpace = tokens.first().begin();

    TextEditor::Parentheses parentheses;
    parentheses.reserve(20); // assume wizard level ;-)

    bool expectPreprocessorKeyword = false;
    bool onlyHighlightComments = false;

    for (int i = 0; i < tokens.size(); ++i) {
        const CPlusPlus::Token &tk = tokens.at(i);

        unsigned previousTokenEnd = 0;
        if (i != 0) {
            // mark the whitespaces
            previousTokenEnd = tokens.at(i - 1).begin() +
                               tokens.at(i - 1).length();
        }

        if (previousTokenEnd != tk.begin()) {
            setFormat(previousTokenEnd, tk.begin() - previousTokenEnd,
                      m_cppFormats[CppVisualWhitespace]);
        }

        if (tk.is(CPlusPlus::T_LPAREN) || tk.is(CPlusPlus::T_LBRACE) || tk.is(CPlusPlus::T_LBRACKET)) {
            const QChar c = text.at(tk.begin());
            parentheses.append(TextEditor::Parenthesis(TextEditor::Parenthesis::Opened, c, tk.begin()));
            if (tk.is(CPlusPlus::T_LBRACE)) {
                ++braceDepth;

                // if a folding block opens at the beginning of a line, treat the entire line
                // as if it were inside the folding block
                if (tk.begin() == firstNonSpace) {
                    ++foldingIndent;
                    TextEditor::BaseTextDocumentLayout::userData(currentBlock())->setFoldingStartIncluded(true);
                }
            }
        } else if (tk.is(CPlusPlus::T_RPAREN) || tk.is(CPlusPlus::T_RBRACE) || tk.is(CPlusPlus::T_RBRACKET)) {
            const QChar c = text.at(tk.begin());
            parentheses.append(TextEditor::Parenthesis(TextEditor::Parenthesis::Closed, c, tk.begin()));
            if (tk.is(CPlusPlus::T_RBRACE)) {
                --braceDepth;
                if (braceDepth < foldingIndent) {
                    // unless we are at the end of the block, we reduce the folding indent
                    if (i == tokens.size()-1 || tokens.at(i+1).is(CPlusPlus::T_SEMICOLON))
                        TextEditor::BaseTextDocumentLayout::userData(currentBlock())->setFoldingEndIncluded(true);
                    else
                        foldingIndent = qMin(braceDepth, foldingIndent);
                }
            }
        }

        bool highlightCurrentWordAsPreprocessor = expectPreprocessorKeyword;

        if (expectPreprocessorKeyword)
            expectPreprocessorKeyword = false;

        if (onlyHighlightComments && !tk.isComment())
            continue;

        if (i == 0 && tk.is(CPlusPlus::T_POUND)) {
            highlightLine(text, tk.begin(), tk.length(), m_cppFormats[CppPreprocessorFormat]);
            expectPreprocessorKeyword = true;
        } else if (highlightCurrentWordAsPreprocessor &&
                   (tk.isKeyword() || tk.isExecSqlKeyword() || tk.is(CPlusPlus::T_IDENTIFIER)) && isPPKeyword(text.midRef(tk.begin(), tk.length()))) {
            setFormat(tk.begin(), tk.length(), m_cppFormats[CppPreprocessorFormat]);
            const QStringRef ppKeyword = text.midRef(tk.begin(), tk.length());
            if (ppKeyword == QLatin1String("error")
                    || ppKeyword == QLatin1String("warning")
                    || ppKeyword == QLatin1String("pragma")) {
                onlyHighlightComments = true;
            }

        } else if (tk.is(CPlusPlus::T_NUMERIC_LITERAL))
            setFormat(tk.begin(), tk.length(), m_cppFormats[CppNumberFormat]);

        else if (tk.is(CPlusPlus::T_STRING_LITERAL) || tk.is(CPlusPlus::T_CHAR_LITERAL) ||
                 tk.is(CPlusPlus::T_ANGLE_STRING_LITERAL) || tk.is(CPlusPlus::T_AT_STRING_LITERAL))
            highlightLine(text, tk.begin(), tk.length(), m_cppFormats[CppStringFormat]);

        else if (tk.is(CPlusPlus::T_EXEC_SQL_INCLUDE_FILENAME))
            setFormat(tk.begin(), tk.length(), m_cppFormats[CppStringFormat]);

        else if (tk.is(CPlusPlus::T_WIDE_STRING_LITERAL) || tk.is(CPlusPlus::T_WIDE_CHAR_LITERAL))
            highlightLine(text, tk.begin(), tk.length(), m_cppFormats[CppStringFormat]);

        else if (tk.isComment()) {

            if (tk.is(CPlusPlus::T_COMMENT) || tk.is(CPlusPlus::T_CPP_COMMENT))
                highlightLine(text, tk.begin(), tk.length(), m_cppFormats[CppCommentFormat]);

            else // a doxygen comment
                highlightDoxygenComment(text, tk.begin(), tk.length());

            // we need to insert a close comment parenthesis, if
            //  - the line starts in a C Comment (initalState != 0)
            //  - the first token of the line is a T_COMMENT (i == 0 && tk.is(T_COMMENT))
            //  - is not a continuation line (tokens.size() > 1 || ! state)
            if (initialState && i == 0 && (tokens.size() > 1 || ! state)) {
                --braceDepth;
                // unless we are at the end of the block, we reduce the folding indent
                if (i == tokens.size()-1)
                    TextEditor::BaseTextDocumentLayout::userData(currentBlock())->setFoldingEndIncluded(true);
                else
                    foldingIndent = qMin(braceDepth, foldingIndent);
                const int tokenEnd = tk.begin() + tk.length() - 1;
                parentheses.append(TextEditor::Parenthesis(TextEditor::Parenthesis::Closed, QLatin1Char('-'), tokenEnd));

                // clear the initial state.
                initialState = 0;
            }

        } else if (tk.isKeyword() || tk.isObjCAtKeyword())
            setFormat(tk.begin(), tk.length(), m_cppFormats[CppKeywordFormat]);

        else if ( tk.is(T_GR_KEYWORD) )
            setFormat(tk.begin(), tk.length(), m_grammarFormats[GrammarKeyword]);

        else if ( tk.is(T_GR_RULE_DEF_IDENTIFIER) )
            setFormat(tk.begin(), tk.length(), m_grammarFormats[GrammarRuleDefinition]);

        else if ( tk.is(T_GR_RULE_ITEM_IDENTIFIER) )
            setFormat(tk.begin(), tk.length(), m_grammarFormats[GrammarRuleItem]);

        else if ( tk.is(T_GR_VARIABLE_IDENTIFIER) )
            setFormat(tk.begin(), tk.length(), m_grammarFormats[GrammarVariable]);

        else if (tk.isExecSqlKeyword())
            setFormat(tk.begin(), tk.length(), m_cppFormats[SqlKeywordFormat]);

        else if (tk.isOperator())
            setFormat(tk.begin(), tk.length(), m_cppFormats[CppOperatorFormat]);

        else if (i == 0 && tokens.size() > 1 && tk.is(CPlusPlus::T_IDENTIFIER) && tokens.at(1).is(CPlusPlus::T_COLON))
            setFormat(tk.begin(), tk.length(), m_cppFormats[CppLabelFormat]);

        else if (tk.is(CPlusPlus::T_IDENTIFIER))
            highlightWord(text.midRef(tk.begin(), tk.length()), tk.begin(), tk.length());

    }

    // mark the trailing white spaces
    {
        const CPlusPlus::Token tk = tokens.last();
        const int lastTokenEnd = tk.begin() + tk.length();
        if (text.length() > lastTokenEnd)
            highlightLine(text, lastTokenEnd, text.length() - lastTokenEnd, QTextCharFormat());
    }

    if (! initialState && state && ! tokens.isEmpty()) {
        parentheses.append(TextEditor::Parenthesis(TextEditor::Parenthesis::Opened, QLatin1Char('+'),
                                       tokens.last().begin()));
        ++braceDepth;
    }

    TextEditor::BaseTextDocumentLayout::setParentheses(currentBlock(), parentheses);

    // if the block is ifdefed out, we only store the parentheses, but

    // do not adjust the brace depth.
    if (TextEditor::BaseTextDocumentLayout::ifdefedOut(currentBlock())) {
        braceDepth = initialBraceDepth;
        foldingIndent = initialBraceDepth;
    }

    if ( grammarState == Definition_BodyRule ) {
        foldingIndent++;
        TextEditor::BaseTextDocumentLayout::userData(currentBlock())->setFoldingStartIncluded(false);
        TextEditor::BaseTextDocumentLayout::userData(currentBlock())->setFoldingEndIncluded(true);
    }

    TextEditor::BaseTextDocumentLayout::setFoldingIndent(currentBlock(), foldingIndent);

    setCurrentBlockState((braceDepth << 8) | (tokenize.grState() << 4) | tokenize.state());
}

void GrammarHighlighter::highlightWord(QStringRef /*word*/, int /*position*/, int /*length*/)
{ }

void GrammarHighlighter::highlightLine(const QString &text, int position, int length, const QTextCharFormat &format)
{
    const QTextCharFormat visualSpaceFormat = m_cppFormats[CppVisualWhitespace];

    const int end = position + length;
    int index = position;

    while (index != end) {
        const bool isSpace = text.at(index).isSpace();
        const int start = index;

        do { ++index; }
        while (index != end && text.at(index).isSpace() == isSpace);

        const int tokenLength = index - start;
        if (isSpace)
            setFormat(start, tokenLength, visualSpaceFormat);
        else if (format.isValid())
            setFormat(start, tokenLength, format);
    }
}

void GrammarHighlighter::highlightDoxygenComment(const QString &text, int position, int /*length*/)
{
    int initial = position;

    const QChar *uc = text.unicode();
    const QChar *it = uc + position;

    const QTextCharFormat &format = m_cppFormats[CppDoxygenCommentFormat];
    const QTextCharFormat &kwFormat = m_cppFormats[CppDoxygenTagFormat];

    while (! it->isNull()) {
        if (it->unicode() == QLatin1Char('\\') ||
            it->unicode() == QLatin1Char('@')) {
            ++it;

            const QChar *start = it;
            while (it->isLetterOrNumber() || it->unicode() == '_')
                ++it;

            int k = CppTools::classifyDoxygenTag(start, it - start);
            if (k != CppTools::T_DOXY_IDENTIFIER) {
                highlightLine(text, initial, start - uc - initial, format);
                setFormat(start - uc - 1, it - start + 1, kwFormat);
                initial = it - uc;
            }
        } else
            ++it;
    }

    highlightLine(text, initial, it - uc - initial, format);
}

bool GrammarHighlighter::isPPKeyword(const QStringRef &text) const
{
    switch (text.length())
    {
    case 2:
        if (text.at(0) == 'i' && text.at(1) == 'f')
            return true;
        break;

    case 4:
        if (text.at(0) == 'e' && text == QLatin1String("elif"))
            return true;
        else if (text.at(0) == 'e' && text == QLatin1String("else"))
            return true;
        break;

    case 5:
        if (text.at(0) == 'i' && text == QLatin1String("ifdef"))
            return true;
        else if (text.at(0) == 'u' && text == QLatin1String("undef"))
            return true;
        else if (text.at(0) == 'e' && text == QLatin1String("endif"))
            return true;
        else if (text.at(0) == 'e' && text == QLatin1String("error"))
            return true;
        break;

    case 6:
        if (text.at(0) == 'i' && text == QLatin1String("ifndef"))
            return true;
        if (text.at(0) == 'i' && text == QLatin1String("import"))
            return true;
        else if (text.at(0) == 'd' && text == QLatin1String("define"))
            return true;
        else if (text.at(0) == 'p' && text == QLatin1String("pragma"))
            return true;
        break;

    case 7:
        if (text.at(0) == 'i' && text == QLatin1String("include"))
            return true;
        else if (text.at(0) == 'w' && text == QLatin1String("warning"))
            return true;
        break;

    case 12:
        if (text.at(0) == 'i' && text == QLatin1String("include_next"))
            return true;
        break;

    default:
        break;
    }

    return false;
}

void GrammarHighlighter::onFormatSettingsChanged( bool forceRehilight )
{
    {
        HighlighterFormatData *formatData = HighlighterFormatsManager::highlighterFormatData(tr("Cpp"));
        for (int i = 0; i < NumCppFormats; i++)
            formatData->fillTextCharFormat(i, m_cppFormats[i]);
    }

    {
        HighlighterFormatData *formatData = HighlighterFormatsManager::highlighterFormatData(tr("Grammar"));
        for (int i = 0; i < GrammarFormatsCount; i++)
            formatData->fillTextCharFormat(i, m_grammarFormats[i]);
    }

    if ( forceRehilight )
        rehighlight();
}

}   // namespace Grammar
