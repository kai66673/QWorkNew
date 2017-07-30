#ifndef POSTGRESPARSERENGINE_H
#define POSTGRESPARSERENGINE_H

#include "SqlToken.h"
#include "IParserEngine.h"

namespace Postgres {

class PostgresParserEngine : public Sql::IParserEngine
{
public:
    PostgresParserEngine();

    virtual QList<Sql::Token> parseTextLine( const QString &text, int state );
    virtual const char *name( const Sql::Token &tk ) const;
    virtual Sql::ISemanticParser *createSemanticParser( Sql::TranslationUnit *unit ) const;

    virtual Sql::ClassifyFunc keyworClassifyFunction();
    virtual bool hasFutureNullIdent() const;
};

} // Postgres

#endif // POSTGRESPARSERENGINE_H
