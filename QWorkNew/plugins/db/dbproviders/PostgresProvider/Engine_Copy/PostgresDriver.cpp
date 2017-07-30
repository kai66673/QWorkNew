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

void PostgresDriver::addStatement( StatementAST *ast )
{
    if ( ast ) {
        m_statements = new (_pool) StatementListAST(m_statements, ast);
        unsigned line;
        unsigned column;
        m_translationUnit->getTokenStartPosition(ast->firstToken(), &line, &column);
        ast->start_line = line;
        ast->start_pos = column;
        m_translationUnit->getTokenEndPosition(ast->lastToken(), &line, &column);
        ast->end_line = line;
        if ( !line )
            column++;
        ast->end_pos = column;
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

    qDebug() << "PostgresDriver::addUnknownStatement" << firstToken << lastToken;

    UnknownStatementAST *ast = new (_pool) UnknownStatementAST( firstToken, lastToken,
                                                                m_translationUnit->tokenAt(firstToken).keyid1() == PostgresParser::token::T_SELECT );
    addStatement(ast);
}

}   // namespace Postgres
