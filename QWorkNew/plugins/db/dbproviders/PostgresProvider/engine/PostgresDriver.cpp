#include <cctype>
#include <fstream>
#include <cassert>

#include <QDebug>

#include "PostgresDriver.h"
#include "PostgresAST.h"

namespace Postgres {

PostgresDriver::~PostgresDriver()
{
   delete(scanner);
   scanner = 0;
   delete(parser);
   parser = 0;

   delete m_currentStatementRelations;
   delete m_currentStatementInfo;

   qDeleteAll(*m_currentStatementHelpers);
   delete m_currentStatementHelpers;
}

TranslationUnitAST *PostgresDriver::parse()
{
   delete(scanner);
   scanner = new PostgresScanner(m_translationUnit);

   delete(parser);
   parser = new PostgresParser( (*scanner), (*this) );

   m_unknownStatementsStartPositionSet.clear();
   parser->parse();
   return new (_pool) TranslationUnitAST(m_statements);
}

TableIdentifierAST *PostgresDriver::relationIdentifier( RelationExprAST *rel )
{
    if ( RelationExprSimpleAST *rel1 = rel->asRelationExprSimple() )
        return rel1->tableIdent;
    if ( RelationExprOnlyAST *rel2 = rel->asRelationExprOnly() )
        return rel2->tableIdent;
    return 0;
}

void PostgresDriver::addRelation( RelationExprAST *rel )
{
    qDebug() << "PostgresDriver::addRelation" << rel;
    if ( rel )
        m_currentStatementRelations->append(rel);
}

void PostgresDriver::addStatement( StatementAST *ast )
{
    if ( ast ) {
        m_statements = new (_pool) StatementListAST(m_statements, ast);
        unsigned line;
        unsigned column;
        qDebug() << "PostgresDriver::addStatement" << ast->firstToken() << ast->lastToken() << ast->asCreateTableStmt();
        m_translationUnit->getTokenStartPosition(ast->firstToken(), &line, &column);
        ast->start_line = line;
        ast->start_pos = column;
        m_translationUnit->getTokenEndPosition(ast->lastToken(), &line, &column);
        ast->end_line = line;
        if ( !line )
            column++;
        ast->end_pos = column;
        ast->relations = m_currentStatementRelations;
        m_currentStatementRelations = new QList<RelationExprAST *>();
        ast->info = m_currentStatementInfo;
        m_currentStatementInfo = new StatementAST::Info();
        ast->helpers = m_currentStatementHelpers;
        m_currentStatementHelpers = new QList<StatementAST::CompletionHelper *>();
    }
}

void PostgresDriver::addUnknownStatement()
{
    int firstToken = 1;
    if ( m_statements ) {
        firstToken = m_statements->value->lastToken();
        while ( m_translationUnit->tokenKind(firstToken) == Sql::T_SEMICOLON )
            firstToken++;
    }

    if ( m_unknownStatementsStartPositionSet.contains(firstToken) )
        return;
    m_unknownStatementsStartPositionSet.insert(firstToken);

    int lastToken = firstToken + 1;
    while ( m_translationUnit->tokenKind(lastToken) != Sql::T_SEMICOLON &&
            m_translationUnit->tokenKind(lastToken) != Sql::T_EOF_SYMBOL )
        lastToken++;
    lastToken--;

    qDebug() << "PostgresDriver::addUnknownStatement" << firstToken << lastToken;
    UnknownStatementAST *ast = new (_pool) UnknownStatementAST( firstToken, lastToken );
    addStatement(ast);
}

void PostgresDriver::appendKeywordsHelper( int start_token, const QString &keywords )
{
    StatementAST::CompletionHelper *helper = new StatementAST::CompletionHelper();
    helper->start_token = start_token;
    helper->type = StatementAST::CompletionHelper::Keywords;
    helper->keywords = keywords;
    m_currentStatementHelpers->append(helper);
}

void PostgresDriver::appendObjectIdentifierHelper( StatementAST::CompletionHelper::HelperType type, int start_token )
{
    qDebug() << "PostgresDriver::appendTableIdentifierHelper" << start_token;
    StatementAST::CompletionHelper *helper = new StatementAST::CompletionHelper();
    helper->start_token = start_token;
    helper->type = type;
    m_currentStatementHelpers->append(helper);

}

void PostgresDriver::appendColumnNameHelper( int start_token, int end_token , NameAST *schemaName, NameAST *objectName )
{
    qDebug() << "== PostgresDriver::appendColumnNameHelper" << m_currentStatementInfo->objectName;
    StatementAST::CompletionHelper *helper = new StatementAST::CompletionHelper();
    helper->type = StatementAST::CompletionHelper::ColumnName;
    helper->start_token = start_token;
    helper->end_token = end_token;
    helper->schemaName = schemaName;
    helper->objectName = objectName;
    m_currentStatementHelpers->append(helper);
}

void PostgresDriver::appendConstraintNameHelper( int start_token )
{
    StatementAST::CompletionHelper *helper = new StatementAST::CompletionHelper();
    helper->type = StatementAST::CompletionHelper::ConstraintName;
    helper->start_token = start_token;
    m_currentStatementHelpers->append(helper);
}

void PostgresDriver::appendTriggerNameHelper(int start_token)
{
    StatementAST::CompletionHelper *helper = new StatementAST::CompletionHelper();
    helper->type = StatementAST::CompletionHelper::TriggerName;
    helper->start_token = start_token;
    m_currentStatementHelpers->append(helper);
}

void PostgresDriver::appendIndexNameHelper( int start_token )
{
    StatementAST::CompletionHelper *helper = new StatementAST::CompletionHelper();
    helper->type = StatementAST::CompletionHelper::IndexName;
    helper->start_token = start_token;
    m_currentStatementHelpers->append(helper);
}

}   // namespace Postgres
