#include "KarelSimpleLexer.h"

namespace Karel {

QList<Token> SimpleLexer::operator()(const QString &text, bool scanKeywords)
{
    const QByteArray bytes = text.toLatin1();
    const char *firstChar = bytes.constData();
    const char *lastChar = firstChar + bytes.size();
    Lexer tokenize(firstChar, lastChar);
    tokenize.setScanKeywords(scanKeywords);
    Token tk;
    QList<Token> tokens;
    do {
        tokenize(&tk);
        tokens.push_back(tk);
    } while (tk.f.kind);

    return tokens;
}

int SimpleLexer::tokenBefore(const QList<Token> &tokens, unsigned offset)
{
    for (int index = tokens.size() - 1; index >= 0; --index) {
        const Token &tk = tokens.at(index);
        if (tk.begin() <= offset + 1)
            return index;
    }

    return -1;
}

}   // namespace Karel
