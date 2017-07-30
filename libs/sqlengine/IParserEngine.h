#ifndef IPARSERENGINE_H
#define IPARSERENGINE_H

#include <QList>

#include "sqlengine_global.h"

#include "SqlToken.h"
#include "ASTfwd.h"

namespace Sql {

class TranslationUnit;

typedef void (*ClassifyFunc)(const char *, int, Token &);

class SQL_EXPORT IParserEngine
{
public:
    IParserEngine();
    virtual ~IParserEngine() {}

    virtual QList<Token> parseTextLine( const QString &text, int state ) = 0;
    virtual const char *name( const Token &tk ) const = 0;
    virtual ISemanticParser *createSemanticParser( TranslationUnit *unit ) const = 0;

    virtual ClassifyFunc keyworClassifyFunction() = 0;
    virtual bool hasFutureNullIdent() const = 0;

    int state() const { return _lastState; }

protected:
    int _lastState;
};

} // Sql

#endif // IPARSERENGINE_H
