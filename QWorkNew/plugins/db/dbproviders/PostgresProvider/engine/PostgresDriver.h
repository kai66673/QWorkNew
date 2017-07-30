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
        , m_currentStatementRelations(new QList<RelationExprAST *>() )
        , m_currentStatementInfo(new StatementAST::Info())
        , m_currentStatementHelpers(new QList<StatementAST::CompletionHelper *>())
   { }

   virtual ~PostgresDriver();

   TranslationUnitAST *parse();
   TableIdentifierAST *relationIdentifier( RelationExprAST *rel );
   void addRelation( RelationExprAST *rel );
   void addStatement( StatementAST *ast );
   void addUnknownStatement();

   StatementAST::Info *currentStatementInfo() { return m_currentStatementInfo; }
   void appendKeywordsHelper( int start_token, const QString &keywords );
   void appendObjectIdentifierHelper( StatementAST::CompletionHelper::HelperType type, int start_token );
   void appendColumnNameHelper( int start_token, int end_token, NameAST *schemaName = 0, NameAST *objectName = 0 );
   void appendConstraintNameHelper( int start_token );
   void appendTriggerNameHelper( int start_token );
   void appendIndexNameHelper( int start_token );

   void error( int index, const QString &message )
   { m_translationUnit->errorString(index, message); }

   void error( AST *ast, const QString &message )
   { m_translationUnit->errorString( ast->firstToken(), ast->lastToken() - 1, message ); }

   NameAST *createName( NameAST::Kind kind, int offset ) {
       if ( offset )
          return new (_pool) NameAST(kind, offset, m_translationUnit->tokenAt(offset).identifier);
       return 0;
   }

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

   template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10>
   T *makeAstNode(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10)
   {
       T *node = new (_pool) T (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
       return node;
   }

   template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11>
   T *makeAstNode(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11)
   {
       T *node = new (_pool) T (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
       return node;
   }

   template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12>
   T *makeAstNode(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12)
   {
       T *node = new (_pool) T (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
       return node;
   }

   template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13>
   T *makeAstNode(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13)
   {
       T *node = new (_pool) T (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
       return node;
   }

   template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, typename A10, typename A11, typename A12, typename A13, typename A14>
   T *makeAstNode(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9, A10 a10, A11 a11, A12 a12, A13 a13, A14 a14)
   {
       T *node = new (_pool) T (a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);
       return node;
   }

private:
   PostgresParser *parser;
   PostgresScanner *scanner;
   Sql::TranslationUnit *m_translationUnit;
   Utils::MemoryPool *_pool;

   StatementListAST *m_statements;
   QSet<int> m_unknownStatementsStartPositionSet;

   QList<RelationExprAST *> *m_currentStatementRelations;
   StatementAST::Info *m_currentStatementInfo;
   QList<StatementAST::CompletionHelper *> *m_currentStatementHelpers;
};

} /* end namespace Postgres */

#endif /* END __POSTGRESDRIVER_H */
