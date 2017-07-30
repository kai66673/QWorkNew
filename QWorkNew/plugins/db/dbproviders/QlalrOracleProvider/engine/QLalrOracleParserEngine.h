#ifndef QLALR_ORACLEPARSERENGINE_H
#define QLALR_ORACLEPARSERENGINE_H

#include "IParserEngine.h"

namespace Sql {
class IParserEngine;
class Token;
class TranslationUnit;
}

namespace QLalrOracle {

class OracleParserEngine: public Sql::IParserEngine
{
public:
    OracleParserEngine();

    virtual QList<Sql::Token> parseTextLine( const QString &text, int state );
    virtual const char *name( const Sql::Token &tk ) const;
    virtual Sql::ISemanticParser *createSemanticParser( Sql::TranslationUnit *unit ) const;

    virtual Sql::ClassifyFunc keyworClassifyFunction();
    virtual bool hasFutureNullIdent() const;
};

}   // namespace QLalrOracle

#endif // QLALR_ORACLEPARSERENGINE_H
