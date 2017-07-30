#ifndef ORACLEPARSERENGINE_H
#define ORACLEPARSERENGINE_H

#include "IParserEngine.h"

namespace Sql {
class IParserEngine;
class Token;
class TranslationUnit;
}

namespace Oracle {

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

}   // namespace Oracle

#endif // ORACLEPARSERENGINE_H
