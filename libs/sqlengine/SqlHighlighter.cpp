#include <QDebug>

#include <basetextdocumentlayout.h>

#include "SqlParserEngine.h"
#include "SqlParserEngineManager.h"
#include "SqlHighlighter.h"
#include "SettingsManager.h"
#include "sql_constants.h"
#include "HighlighterFormatsManager.h"

namespace Sql {
namespace Editor {

SqlHighlighter::SqlHighlighter( QTextDocument *document )
    : TextEditor::SyntaxHighlighter(document)
{ }

SqlHighlighter::~SqlHighlighter()
{ }

void SqlHighlighter::highlightBlock( const QString &text )
{
    const int previousState = previousBlockState();
    int state = 0;
    int initialBraceDepth = 0;

    if (previousState != -1) {
        state = previousState & 0xff;
        initialBraceDepth = previousState >> 8;
    }

    int braceDepth = initialBraceDepth;
    int initialState = state;

    IParserEngine *sqlEngine = SqlParserEngineManager::instance()->currentEngine();

    const QList<Token> tokens = sqlEngine->parseTextLine(text, initialState);
    state = sqlEngine->state(); // refresh the state

    int foldingIndent = initialBraceDepth;
    if (TextEditor::TextBlockUserData *userData = TextEditor::BaseTextDocumentLayout::testUserData(currentBlock())) {
        userData->setFoldingIndent(0);
        userData->setFoldingStartIncluded(false);
        userData->setFoldingEndIncluded(false);
    }

    if ( tokens.isEmpty() ) {
        setCurrentBlockState(previousState);
        TextEditor::BaseTextDocumentLayout::clearParentheses(currentBlock());
        if (text.length()) // the empty line can still contain whitespace
            setFormat(0, text.length(), m_formats[Sql::SqlVisualWhitespace]);
        TextEditor::BaseTextDocumentLayout::setFoldingIndent(currentBlock(), foldingIndent);
        return;
    }

    const unsigned firstNonSpace = tokens.first().begin();

    TextEditor::Parentheses parentheses;
    parentheses.reserve(20); // assume wizard level ;-)

    for (int i = 0; i < tokens.size(); ++i) {
        const Token &tk = tokens.at(i);

        unsigned previousTokenEnd = 0;
        if (i != 0) {
            // mark the whitespaces
            previousTokenEnd = tokens.at(i - 1).begin() +
                               tokens.at(i - 1).length();
        }

        if (previousTokenEnd != tk.begin()) {
            setFormat(previousTokenEnd, tk.begin() - previousTokenEnd,
                      m_formats[Sql::SqlVisualWhitespace]);
        }

        if (tk.is(T_LPAREN) || tk.is(T_LBRACE) || tk.is(T_LBRACKET)) {
            const QChar c = text.at(tk.begin());
            parentheses.append(TextEditor::Parenthesis(TextEditor::Parenthesis::Opened, c, tk.begin()));
            if (tk.is(T_LBRACE)) {
                ++braceDepth;

                // if a folding block opens at the beginning of a line, treat the entire line
                // as if it were inside the folding block
                if (tk.begin() == firstNonSpace) {
                    ++foldingIndent;
                    TextEditor::BaseTextDocumentLayout::userData(currentBlock())->setFoldingStartIncluded(true);
                }
            }
        } else if (tk.is(T_RPAREN) || tk.is(T_RBRACE) || tk.is(T_RBRACKET)) {
            const QChar c = text.at(tk.begin());
            parentheses.append(TextEditor::Parenthesis(TextEditor::Parenthesis::Closed, c, tk.begin()));
            if (tk.is(T_RBRACE)) {
                --braceDepth;
                if (braceDepth < foldingIndent) {
                    // unless we are at the end of the block, we reduce the folding indent
                    if (i == tokens.size()-1 || tokens.at(i+1).is(T_SEMICOLON))
                        TextEditor::BaseTextDocumentLayout::userData(currentBlock())->setFoldingEndIncluded(true);
                    else
                        foldingIndent = qMin(braceDepth, foldingIndent);
                }
            }
        }

        if ( tk.is(T_NUMERIC_LITERAL) )
            setFormat(tk.begin(), tk.length(), m_formats[Sql::SqlNumberFormat]);

        else if ( /*tk.is(T_STRING_LITERAL) || */tk.is(T_CHAR_LITERAL) )
            highlightLine(text, tk.begin(), tk.length(), m_formats[Sql::SqlStringFormat]);

        else if ( tk.isComment() ) {
            highlightLine(text, tk.begin(), tk.length(), m_formats[Sql::SqlCommentFormat]);

            // we need to insert a close comment parenthesis, if
            //  - the line starts in a C Comment (initalState != 0)
            //  - the first token of the line is a T_COMMENT (i == 0 && tk.is(T_COMMENT))
            //  - is not a continuation line (tokens.size() > 1 || ! state)
            if ( initialState && i == 0 && (tokens.size() > 1 || ! state) ) {
                --braceDepth;
                // unless we are at the end of the block, we reduce the folding indent
                if ( i == tokens.size() - 1 )
                    TextEditor::BaseTextDocumentLayout::userData(currentBlock())->setFoldingEndIncluded(true);
                else
                    foldingIndent = qMin(braceDepth, foldingIndent);
                const int tokenEnd = tk.begin() + tk.length() - 1;
                parentheses.append(TextEditor::Parenthesis(TextEditor::Parenthesis::Closed, QLatin1Char('-'), tokenEnd));

                // clear the initial state.
                initialState = 0;
            }

        } else if ( tk.isKeyword() ) {
            switch ( tk.keywordType() ) {
                case 1:  setFormat(tk.begin(), tk.length(), m_formats[Sql::SqlKeyword1Format]); break;
                case 2:  setFormat(tk.begin(), tk.length(), m_formats[Sql::SqlKeyword2Format]); break;
                case 3:  setFormat(tk.begin(), tk.length(), m_formats[Sql::SqlKeyword3Format]); break;
                case 4:  setFormat(tk.begin(), tk.length(), m_formats[Sql::SqlKeyword4Format]); break;
                case 5:  setFormat(tk.begin(), tk.length(), m_formats[Sql::SqlKeyword5Format]); break;
                case 6:  setFormat(tk.begin(), tk.length(), m_formats[Sql::SqlKeyword6Format]); break;
                case 7:  setFormat(tk.begin(), tk.length(), m_formats[Sql::SqlKeyword7Format]); break;
                case 8:  setFormat(tk.begin(), tk.length(), m_formats[Sql::SqlKeyword8Format]); break;
                default: setFormat(tk.begin(), tk.length(), m_formats[Sql::SqlKeyword1Format]); break;
            }

        } else if ( tk.isOperator() )
            setFormat(tk.begin(), tk.length(), m_formats[Sql::SqlOperatorFormat]);

//        else if ( tk.is(T_IDENTIFIER) )
//            highlightWord(text.midRef(tk.begin(), tk.length()), tk.begin(), tk.length());

    }

    // mark the trailing white spaces
    {
        const Token tk = tokens.last();
        const int lastTokenEnd = tk.begin() + tk.length();
        if (text.length() > lastTokenEnd)
            highlightLine(text, lastTokenEnd, text.length() - lastTokenEnd, QTextCharFormat());
    }

    if ( !initialState && state && ! tokens.isEmpty() ) {
        parentheses.append(TextEditor::Parenthesis(TextEditor::Parenthesis::Opened, QLatin1Char('+'),
                                       tokens.last().begin()));
        ++braceDepth;
    }

    TextEditor::BaseTextDocumentLayout::setParentheses(currentBlock(), parentheses);

    // if the block is ifdefed out, we only store the parentheses, but

    // do not adjust the brace depth.
    if ( TextEditor::BaseTextDocumentLayout::ifdefedOut(currentBlock()) ) {
        braceDepth = initialBraceDepth;
        foldingIndent = initialBraceDepth;
    }

    TextEditor::BaseTextDocumentLayout::setFoldingIndent(currentBlock(), foldingIndent);

    // optimization: if only the brace depth changes, we adjust subsequent blocks
    // to have QSyntaxHighlighter stop the rehighlighting
    int currentState = currentBlockState();
    if ( currentState != -1 ) {
        int oldState = currentState & 0xff;
        int oldBraceDepth = currentState >> 8;
        if ( oldState == sqlEngine->state() && oldBraceDepth != braceDepth ) {
            int delta = braceDepth - oldBraceDepth;
            QTextBlock block = currentBlock().next();
            while (block.isValid() && block.userState() != -1) {
                TextEditor::BaseTextDocumentLayout::changeBraceDepth(block, delta);
                TextEditor::BaseTextDocumentLayout::changeFoldingIndent(block, delta);
                block = block.next();
            }
        }
    }

    setCurrentBlockState((braceDepth << 8) | sqlEngine->state());
}

void SqlHighlighter::highlightLine(const QString &text, int position, int length,
                                   const QTextCharFormat &format)
{
    const QTextCharFormat visualSpaceFormat = m_formats[Sql::SqlVisualWhitespace];

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

void SqlHighlighter::highlightWord(QStringRef /*word*/, int /*position*/, int /*length*/)
{ }

void SqlHighlighter::onFormatSettingsChanged( bool forceRehilight )
{
    HighlighterFormatData *formatData = HighlighterFormatsManager::highlighterFormatData(tr("SQL"));

    for (int i = 0; i < Sql::NumSqlFormats; i++)
        formatData->fillTextCharFormat(i, m_formats[i]);

    if ( forceRehilight )
        rehighlight();
}

} // Editor
} // Sql
