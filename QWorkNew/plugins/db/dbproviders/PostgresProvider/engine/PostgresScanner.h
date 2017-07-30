#ifndef __POSTGRESSCANNER_H
#define __POSTGRESSCANNER_H

#include <vector>

#include "PostgresParser.tab.hh"
#include "TranslationUnit.h"

namespace Postgres {

class PostgresScanner
{
public:
   PostgresScanner( Sql::TranslationUnit *translationUnit );

   int yylex( PostgresParser::semantic_type *lval )
   {
      yylval = lval;
      return( yylex() );
   }

private:
   int yylex();
   Sql::TranslationUnit *m_translationUnit;
   int m_index;
   PostgresParser::semantic_type *yylval;
   std::vector<Sql::Token> *m_tokens;
};

} /* end namespace Postgres */

#endif /* END __POSTGRESSCANNER_H */
