#include <QDebug>

#include "PostgresScanner.h"

namespace Postgres {

PostgresScanner::PostgresScanner( Sql::TranslationUnit *translationUnit )
   : m_translationUnit(translationUnit)
   , m_index(0)
   , yylval(0)
   , m_tokens(translationUnit->tokens())
{ }

int PostgresScanner::yylex()
{
   m_index++;

   const Sql::Token &tk = m_tokens->at(m_index);
   yylval->location = m_index;

   switch ( tk.kind() ) {
      case Sql::T_EOF_SYMBOL:       return PostgresParser::token::T_EOF;
      case Sql::T_IDENTIFIER:       return PostgresParser::token::T_IDENTIFIER;

      case Sql::T_NUMERIC_LITERAL:  return PostgresParser::token::T_DIGIT;
      case Sql::T_CHAR_LITERAL:     return PostgresParser::token::T_CHARSTR;
      case Sql::T_STRING_LITERAL:   return PostgresParser::token::T_STRSTR;

      case Sql::T_AMPER:            return PostgresParser::token::T_AMPER;
      case Sql::T_CARET:            return PostgresParser::token::T_CARET;
      case Sql::T_COLON:            return PostgresParser::token::T_COLON;
      case Sql::T_ASSIGN:           return PostgresParser::token::T_ASSIGN;
      case Sql::T_COMMA:            return PostgresParser::token::T_COMMA;
      case Sql::T_SLASH:            return PostgresParser::token::T_SLASH;
      case Sql::T_DOT:              return PostgresParser::token::T_DOT;
      case Sql::T_DOTDOT:           return PostgresParser::token::T_DOTDOT;
      case Sql::T_EQUAL:            return PostgresParser::token::T_EQUAL;
      case Sql::T_EXCLAIM:          return PostgresParser::token::T_EXCLAIM;
      case Sql::T_EXCLAIM_EQUAL:    return PostgresParser::token::T_EXCLAIM_EQUAL;
      case Sql::T_GREATER:          return PostgresParser::token::T_GREATER;
      case Sql::T_GREATER_EQUAL:    return PostgresParser::token::T_GREATER_EQUAL;
      case Sql::T_LBRACE:           return PostgresParser::token::T_LEFT_BRACE;
      case Sql::T_LBRACKET:         return PostgresParser::token::T_LEFT_BRACKET;
      case Sql::T_LESS:             return PostgresParser::token::T_LESS;
      case Sql::T_LESS_EQUAL:       return PostgresParser::token::T_LESS_EQUAL;
      case Sql::T_LESS_GREATER:     return PostgresParser::token::T_LESS_GREATER;
      case Sql::T_LPAREN:           return PostgresParser::token::T_LEFT_PAREN;
      case Sql::T_MINUS:            return PostgresParser::token::T_DASH;
      case Sql::T_PERCENT:          return PostgresParser::token::T_PERCENT;
      case Sql::T_PIPE:             return PostgresParser::token::T_PIPE;
      case Sql::T_PIPE_PIPE:        return PostgresParser::token::T_PIPE_PIPE;
      case Sql::T_PLUS:             return PostgresParser::token::T_PLUS;
      case Sql::T_RBRACE:           return PostgresParser::token::T_RIGHT_BRACE;
      case Sql::T_RBRACKET:         return PostgresParser::token::T_RIGHT_BRACKET;
      case Sql::T_RPAREN:           return PostgresParser::token::T_RIGHT_PAREN;
      case Sql::T_SEMICOLON:        return PostgresParser::token::T_SEMICOLON;
      case Sql::T_STAR:             return PostgresParser::token::T_STAR;

      case Sql::T_KEYWORD:
         if ( int id1 = tk.keyid1() ) {
            return id1;
         } else if ( int id2 = tk.keyid2() ) {
            return id2;
         } else if ( int id3 = tk.keyid3() ) {
            return id3;
         }

      default:
         return PostgresParser::token::T_EOF;
   }

   return PostgresParser::token::T_EOF;
}

}  // namespace Postgres
