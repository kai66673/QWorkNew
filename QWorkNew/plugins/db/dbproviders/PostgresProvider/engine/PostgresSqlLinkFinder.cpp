#include "PostgresSqlLinkFinder.h"

namespace Postgres {

SqlLinkFinder::SqlLinkFinder( Sql::TranslationUnit *unit )
    : ASTVisitor(unit)
    , m_translationUnit(unit)
    , m_ast(0)
    , m_currentStatemenAst(0)
    , m_linkTokenIndex(-1)
    , m_begin(-1)
    , m_end(-1)
    , m_link(0)
{
}

Core::ILink *SqlLinkFinder::findLinkAt( TranslationUnitAST *ast, int position )
{
    m_position = (unsigned) position;
    m_ast = ast;
    for (unsigned i = 1; i < m_translationUnit->tokenCount() - 1; i++) {
        if ( m_translationUnit->getTokenOffset(i) >= m_position  ) {
            const Sql::Token &tk = m_translationUnit->tokenAt(i - 1);
            if ( tk.begin() <= m_position && tk.end() >= m_position ) {
                if ( !tk.isIdentifier() )
                    return 0;
                m_linkTokenIndex = i - 1;
                m_begin = tk.begin();
                m_end = tk.end();
                m_linkText = QString(tk.identifier->chars()).toUpper();
                return findLinkInternal(ast);
            }
            return 0;
        }
    }
    return 0;
}

Sql::DetailsLink *SqlLinkFinder::findSchemaLink()
{
    if ( QList<Database::IDbCatalogItem *> *schemas = m_connection->schemas() ) {
        foreach ( Database::IDbCatalogItem *schemaItem, *schemas ) {
            if ( !m_linkText.compare(schemaItem->name(), Qt::CaseInsensitive) )
                return new Sql::DetailsLink(m_begin, m_end, m_connection, schemaItem);
        }
    }

    return 0;
}

Sql::DetailsLink *SqlLinkFinder::findSchemaObjectLink( const QString &schemaName, int objectType )
{
    if ( QList<Database::IDbCatalogItem *> *objects = m_connection->schemaObjects(schemaName, objectType) ) {
        foreach ( Database::IDbCatalogItem *objectItem, *objects ) {
            if ( !m_linkText.compare(objectItem->name(), Qt::CaseInsensitive) )
                return new Sql::DetailsLink(m_begin, m_end, m_connection, objectItem);
        }
    }

    return 0;
}

Core::ILink *SqlLinkFinder::findLinkInternal( TranslationUnitAST *ast )
{
    for ( StatementListAST *iter = ast->statements; iter; iter = iter->next ) {
        StatementAST *statement = iter->value;
        if ( statement ) {
            if ( m_linkTokenIndex <= statement->lastToken() ) {
                if ( m_linkTokenIndex >= statement->firstToken() )
                    return findLinkInStatement(statement);
                return 0;
            }
        }
    }
    return 0;
}

Core::ILink *SqlLinkFinder::findLinkInStatement( StatementAST *ast )
{
    m_currentStatemenAst = ast;
    return findLinkInSingleStatement(ast);
}

Core::ILink *SqlLinkFinder::findLinkInSingleStatement( AST *ast )
{
    if ( m_provider = Sql::DbMetadataProvider::getInstance() ) {
        if ( m_connection = m_provider->connection() ) {
            ast->accept(this);
            return m_link;
        }
    }
    return 0;
}

bool SqlLinkFinder::visit( TableIdentifierAST *ast )
{
    if ( !m_link && ast ) {
        if ( ast->schemaName && m_linkTokenIndex == ast->schemaName->name_token )
            m_link = findSchemaLink();
        else if ( ast->tableName && m_linkTokenIndex == ast->tableName->name_token ) {
            QString schemaName = "";
            if ( ast->schemaName && ast->schemaName->name )
                schemaName = ast->schemaName->name->chars();
            m_link = findSchemaObjectLink(schemaName, Sql::Constants::DbSchemaObjectType::C_TABLE);
            if ( !m_link )
                m_link = findSchemaObjectLink(schemaName, Sql::Constants::DbSchemaObjectType::C_VIEW);
        }
    }
    return false;
}

bool SqlLinkFinder::visit( IndexIdentifierAST *ast )
{
    if ( !m_link && ast ) {
        if ( ast->schemaName && m_linkTokenIndex == ast->schemaName->name_token )
            m_link = findSchemaLink();
        else if ( ast->indexName && m_linkTokenIndex == ast->indexName->name_token ) {
            QString schemaName = "";
            if ( ast->schemaName && ast->schemaName->name )
                schemaName = ast->schemaName->name->chars();
            m_link = findSchemaObjectLink(schemaName, Sql::Constants::DbSchemaObjectType::C_INDEX);
        }
    }
    return false;
}

}   // namespace Postgres
