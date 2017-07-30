#include <QTextCursor>
#include <QTextDocument>
#include <QTextBlock>
#include <QDebug>

#include "KarelAutoCompleter.h"
#include "KarelSimpleLexer.h"

namespace Karel {

static bool shouldInsertMatchingText(QChar lookAhead)
{
    switch (lookAhead.unicode()) {
    case '{': case '}':
    case ']': case ')':
    case ';': case ',':
        return true;

    default:
        if (lookAhead.isSpace())
            return true;

        return false;
    } // switch

    return false;
}

AutoCompleter::AutoCompleter()
    : TextEditor::AutoCompleter()
{ }

AutoCompleter::~AutoCompleter()
{ }

bool AutoCompleter::contextAllowsAutoParentheses(const QTextCursor &cursor, const QString &textToInsert) const
{
    if ( !shouldInsertMatchingText(cursor.document()->characterAt(cursor.selectionEnd())) )
        return false;

    if ( textToInsert.length() == 1 ) {
        QChar ch;
        ch = textToInsert.at(0);
        if ( ch == QLatin1Char('"') || ch == QLatin1Char('\'') )
            return !isInCommentStrict(cursor);
    }

    return !isInComment(cursor);
}

bool AutoCompleter::isInComment(const QTextCursor &cursor) const
{
    SimpleLexer tokenize;
    const QList<Token> &tokens = tokenize(cursor.block().text(), true);
    const int tokenIdx = SimpleLexer::tokenBefore(tokens, qMax(0, cursor.positionInBlock() - 1));
    const Token &tk = (tokenIdx == -1) ? Token() : tokens.at(tokenIdx);

    return tk.isComment() || tk.isLiteral();
}

QString AutoCompleter::insertMatchingBrace(const QTextCursor &cursor, const QString &text, QChar la, int *skippedChars) const
{
    if ( text.length() == 1 ) {
        switch (text.at(0).toLatin1()) {
            case '{': return QStringLiteral("}");
            case '[': return QStringLiteral("]");
            case '(': return QStringLiteral(")");
            case '"': return QStringLiteral("\"");
            case '\'': return QStringLiteral("\'");
        }

        return QString();
    }

    return TextEditor::AutoCompleter::insertMatchingBrace(cursor, text, la, skippedChars);
}

bool AutoCompleter::isInCommentStrict(const QTextCursor &cursor) const
{
    SimpleLexer tokenize;
    const QList<Token> &tokens = tokenize(cursor.block().text(), true);
    const int tokenIdx = SimpleLexer::tokenBefore(tokens, qMax(0, cursor.positionInBlock() - 1));
    const Token &tk = (tokenIdx == -1) ? Token() : tokens.at(tokenIdx);

    return tk.isComment();
}

}   // namespace Karel
