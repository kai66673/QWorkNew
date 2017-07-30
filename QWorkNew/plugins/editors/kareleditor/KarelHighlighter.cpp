#include "KarelHighlighter.h"
#include "KarelLexer.h"
#include "HighlighterFormatsManager.h"

#include <basetextdocumentlayout.h>

namespace Karel {
namespace Editor {

KarelHighlighter::KarelHighlighter(QTextDocument *document)
    : TextEditor::SyntaxHighlighter(document)
{ }

KarelHighlighter::~KarelHighlighter()
{ }

void KarelHighlighter::highlightBlock(const QString &text)
{
    const QByteArray bytes = text.toLatin1();
    const char *firstChar = bytes.constData();
    const char *lastChar = firstChar + bytes.size();

    Lexer lex(firstChar, lastChar);
    lex.setScanKeywords(true);

    QList<Token> tokens;

    for (;;) {
        Token tk;
        lex(&tk);
        if (tk.is(T_EOF_SYMBOL)) {
            break;
        }

        tokens.append(tk);
    }

    if ( tokens.isEmpty() ) {
        if ( !text.isEmpty() )
            setFormat(0, text.length(), m_formats[KarelVisualWhitespace]);
        return;
    }

    int braceDepth = previousBlockState();
    if ( braceDepth == -1 )
        braceDepth = 0;
    TextEditor::Parentheses parentheses;
    parentheses.reserve(20); // assume wizard level ;-)

    for (int i = 0; i < tokens.size(); i++) {
        const Token &tk = tokens.at(i);

        unsigned previousTokenEnd = 0;
        if ( i != 0 )
            previousTokenEnd = tokens.at(i - 1).begin() +
                               tokens.at(i - 1).length();

        if ( previousTokenEnd != tk.begin() ) {
            setFormat(previousTokenEnd, tk.begin() - previousTokenEnd,
                      m_formats[KarelVisualWhitespace]);
        }

        if ( tk.is(T_NUMERIC_LITERAL) )
            setFormat(tk.begin(), tk.length(), m_formats[KarelNumberFormar]);

        else if ( tk.is(T_STRING_LITERAL) )
            setFormat(tk.begin(), tk.length(), m_formats[KarelStringFormar]);

        else if ( tk.isComment() )
            highlightLine(text, tk.begin(), tk.length(), m_formats[KarelCommentFormat]);

        else if ( tk.isKeyword() ) {
            switch ( tk.keywordType() ) {
                case 1:  setFormat(tk.begin(), tk.length(), m_formats[KarelKeyword1Format]); break;
                case 2:  setFormat(tk.begin(), tk.length(), m_formats[KarelKeyword2Format]); break;
                case 3:  setFormat(tk.begin(), tk.length(), m_formats[KarelKeyword3Format]); break;
                case 4:  setFormat(tk.begin(), tk.length(), m_formats[KarelKeyword4Format]); break;
                case 5:  setFormat(tk.begin(), tk.length(), m_formats[KarelKeyword5Format]); break;
                default: setFormat(tk.begin(), tk.length(), m_formats[KarelKeyword1Format]); break;
            }
        }

        else if  ( tk.isOperator() ) {
            setFormat(tk.begin(), tk.length(), m_formats[KarelOperatorFormat]);
            switch ( tk.f.kind ) {
            case T_LBRACE:
            case T_LBRACKET:
            case T_LPAREN:
                parentheses.append(TextEditor::Parenthesis(TextEditor::Parenthesis::Opened, text.at(tk.begin()), tk.begin()));
                braceDepth++;
                break;
            case T_RBRACE:
            case T_RBRACKET:
            case T_RPAREN:
                parentheses.append(TextEditor::Parenthesis(TextEditor::Parenthesis::Closed, text.at(tk.begin()), tk.begin()));
                braceDepth--;
                if ( braceDepth < 0 )
                    braceDepth = 0;
                break;
            }
        }
    }


    {   // mark the trailing white spaces
        const Token tk = tokens.last();
        const int lastTokenEnd = tk.begin() + tk.length();
        if (text.length() > lastTokenEnd)
            highlightLine(text, lastTokenEnd, text.length() - lastTokenEnd, QTextCharFormat());
    }

    TextEditor::BaseTextDocumentLayout::setParentheses(currentBlock(), parentheses);
    setCurrentBlockState(braceDepth);
}

void KarelHighlighter::onFormatSettingsChanged(bool forceRehilight)
{
    HighlighterFormatData *formatData = HighlighterFormatsManager::highlighterFormatData(tr("KAREL"));

    for (int i = 0; i < NumKarelFormats; i++)
        formatData->fillTextCharFormat(i, m_formats[i]);

    if ( forceRehilight )
        rehighlight();
}

void KarelHighlighter::highlightLine(const QString &text, int position, int length,
                                     const QTextCharFormat &format)
{
    const QTextCharFormat &visualSpaceFormat = m_formats[KarelVisualWhitespace];

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

}   // namespace Editor
}   // namespace Karel
