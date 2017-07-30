#ifndef KARELSIMPLELEXER_H
#define KARELSIMPLELEXER_H

#include "KarelLexer.h"

namespace Karel {

class SimpleLexer
{
public:
    QList<Token> operator()(const QString &text, bool scanKeywords = false);

    static int tokenBefore(const QList<Token> &tokens, unsigned offset);
};

}   // namespace Karel

#endif // KARELSIMPLELEXER_H
