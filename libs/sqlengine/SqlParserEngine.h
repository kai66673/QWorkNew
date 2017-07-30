#ifndef SQLPARSERENGINE_H
#define SQLPARSERENGINE_H

#include "IParserEngine.h"

namespace Sql {

class SQL_EXPORT SqlParserEngine : public IParserEngine
{
public:
    SqlParserEngine();

    virtual QList<Token> parseTextLine( const QString &text, int state );
    virtual const char *name( const Token &tk ) const;
    virtual ISemanticParser *createSemanticParser( TranslationUnit *unit ) const;

    virtual ClassifyFunc keyworClassifyFunction();
    virtual bool hasFutureNullIdent() const;
};

} // Sql

#endif // SQLPARSERENGINE_H
