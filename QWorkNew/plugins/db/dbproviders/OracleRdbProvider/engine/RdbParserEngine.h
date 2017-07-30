#ifndef RDBPARSERENGINE_H
#define RDBPARSERENGINE_H

#include "IParserEngine.h"

namespace Sql {
class IParserEngine;
class Token;
class TranslationUnit;
}

namespace RDB {

class RdbParserEngine: public Sql::IParserEngine
{
public:
    RdbParserEngine();

    virtual QList<Sql::Token> parseTextLine( const QString &text, int state );
    virtual const char *name( const Sql::Token &tk ) const;
    virtual Sql::ISemanticParser *createSemanticParser( Sql::TranslationUnit *unit ) const;

    virtual Sql::ClassifyFunc keyworClassifyFunction();
    virtual bool hasFutureNullIdent() const;
};

}   // namespace RDB

#endif // RDBPARSERENGINE_H
