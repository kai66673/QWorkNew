#ifndef __POSTGRESDRIVER_H
#define __POSTGRESDRIVER_H

#include <QSet>

#include "TranslationUnit.h"
#include "PostgresASTfwd.h"
#include "PostgresAST.h"
#include "PostgresScanner.h"
#include "PostgresParser.tab.hh"

namespace Postgres {

class PostgresDriver {
public:
   PostgresDriver( Sql::TranslationUnit *translationUnit )
        : parser(0)
        , scanner(0)
        , m_translationUnit(translationUnit)
        , _pool(translationUnit->memoryPool())
        , m_statements(0)
   { }

   virtual ~PostgresDriver();

   TranslationUnitAST *parse();
   void addStatement( StatementAST *ast );
   void addUnknownStatement();

   void error( int index, const QString &message )
   { m_translationUnit->errorString(index, message); }

   void error( AST *ast, const QString &message )
   { m_translationUnit->errorString( ast->firstToken(), ast->lastToken() - 1, message ); }

   NameAST *createName( NameAST::Kind kind, int offset )
   { return new (_pool) NameAST(kind, offset, m_translationUnit->tokenAt(offset).identifier); }

   QString identifierString( int offset )
   { return QString(m_translationUnit->tokenAt(offset).identifier->chars()).toLower(); }

   template <typename T>
   T *makeAstNode()
   {
       T *node = new (_pool) T ();
       return node;
   }

   template <typename T, typename A1>
   T *makeAstNode(A1 a1)
   {
       T *node = new (_pool) T (a1);
       return node;
   }

   template <typename T, typename A1, typename A2>
   T *makeAstNode(A1 a1, A2 a2)
   {
       T *node = new (_pool) T (a1, a2);
       return node;
   }

   template <typename T, typename A1, typename A2, typename A3>
   T *makeAstNode(A1 a1, A2 a2, A3 a3)
   {
       T *node = new (_pool) T (a1, a2, a3);
       return node;
   }

   template <typename T, typename A1, typename A2, typename A3, typename A4>
   T *makeAstNode(A1 a1, A2 a2, A3 a3, A4 a4)
   {
       T *node = new (_pool) T (a1, a2, a3, a4);
       return node;
   }

   template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5>
   T *makeAstNode(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
   {
       T *node = new (_pool) T (a1, a2, a3, a4, a5);
       return node;
   }

   template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
   T *makeAstNode(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
   {
       T *node = new (_pool) T (a1, a2, a3, a4, a5, a6);
       return node;
   }

   template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
   T *makeAstNode(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
   {
       T *node = new (_pool) T (a1, a2, a3, a4, a5, a6, a7);
       return node;
   }

   template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
   T *makeAstNode(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
   {
       T *node = new (_pool) T (a1, a2, a3, a4, a5, a6, a7, a8);
       return node;
   }

   template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
   T *makeAstNode(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
   {
       T *node = new (_pool) T (a1, a2, a3, a4, a5, a6, a7, a8, a9);
       return node;
   }

private:
   PostgresParser *parser;
   PostgresScanner *scanner;
   Sql::TranslationUnit *m_translationUnit;
   Sql::MemoryPool *_pool;

   StatementListAST *m_statements;
   QSet<int> m_unknownStatementsStartPositionSet;
};

} /* end namespace Postgres */

#endif /* END __POSTGRESDRIVER_H */
