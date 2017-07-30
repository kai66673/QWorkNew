#include "SqlLinkFinder.h"
#include "SqlToken.h"
#include "qtcassert.h"

namespace Sql {

static Core::ILink *createSubobjectLink( Database::DbConnection *connection,
                                         const QString &objectName,
                                         const QString &subobjectName,
                                         unsigned objectType,
                                         unsigned objectSubtype,
                                         int begin, int end )
{
    if ( connection ) {
        if ( QList<Database::IDbCatalogItem *> *objects = connection->schemaObjectObjects( "", objectName, objectType, objectSubtype ) ) {
            for (int i = 0; i < objects->size(); i++) {
                Database::IDbCatalogItem *object = objects->at(i);
                if ( !subobjectName.compare(object->name(), Qt::CaseInsensitive) ) {
                    return new Sql::DetailsLink(begin, end, connection, object);
                }
            }
        }
    }
    return 0;
}

namespace {     // Anonimous namespace

class ColumnFinder: protected ASTVisitor
{
private:
    struct TableColumns {
        QList<NameAST *> columns;

        void insert( NameAST *columnNameAst ) {
            QString columnName(columnNameAst->name->chars());
            columnName = columnName.toUpper();
            foreach ( NameAST *c, columns ) {
                if ( !columnName.compare(c->name->chars(), Qt::CaseInsensitive) )
                    return;
            }
            columns << columnNameAst;
        }

        NameAST *find( const QString &columnName ) const {
            foreach ( NameAST *c, columns ) {
                if ( !columnName.compare(c->name->chars(), Qt::CaseInsensitive) )
                    return c;
            }
            return 0;
        }
    };

public:
    ColumnFinder( TranslationUnit *unit, Database::DbConnection *connection )
        : ASTVisitor(unit)
        , m_translationUnit(unit)
        , m_connection(connection)
        , m_currentColumns(0)
    { }

    virtual ~ColumnFinder() {
        qDeleteAll(m_tableColumns.values());
    }

    Core::ILink *operator () ( TranslationUnitAST *ast, StatementAST *currentStatemenAst, int begin, int end,
                               const QList<NameAST *> &tableNames, const QString &columnName ) {
        m_begin = begin;
        m_end = end;

        bool currentStatemenNotProceded = true;
        for ( StatementListAST *iter = ast->statement_list; iter && currentStatemenNotProceded; iter = iter->next ) {
            if ( StatementAST *statement = iter->value ) {
                if ( statement == currentStatemenAst )
                    currentStatemenNotProceded = false;
                if ( statement->asCreateTableStatement() || statement->asCreateViewStatement() || statement->asAlterTableStatement() ) {
                    m_currentColumns = 0;
                    statement->accept(this);
                }
            }
        }

        foreach ( NameAST *tableNameAst, tableNames ) {
            if ( Core::ILink *link = linkForTableColumn(tableNameAst, columnName) )
                return link;
        }

        return 0;
    }

protected:
    virtual bool visit( CreateTableStatementAST *ast ) {
        if ( ast && ast->schemaTableName && ast->schemaTableName->tableName ) {
            QString tableName(ast->schemaTableName->tableName->name->chars());
            tableName = tableName.toUpper();
            m_currentColumns = new TableColumns();
            m_currenActionIsAdd = true;
            m_tableColumns[tableName] = m_currentColumns;
        }
        return true;
    }

    virtual bool visit( CreateViewStatementAST *ast ) {
        if ( ast && ast->schemaViewName && ast->schemaViewName->tableName ) {
            QString tableName(ast->schemaViewName->tableName->name->chars());
            tableName = tableName.toUpper();
            m_currentColumns = new TableColumns();
            m_currenActionIsAdd = true;
            m_tableColumns[tableName] = m_currentColumns;
        }
        return true;
    }

    virtual bool visit( ExprResultColumnAST *ast ) {
        if ( ast && ast->expr && m_currenActionIsAdd && m_currentColumns ) {
            if ( ColumnAliasAST *aliasAst = ast->columnAlias ) {
                m_currentColumns->insert(aliasAst);
            } else if ( ColumnExpressionAST *colExpr = ast->expr->asColumnExpression() ) {
                if ( colExpr->column )
                    m_currentColumns->insert(colExpr->column);
            }
        }
        return false;
    }

    virtual bool visit( ColumnDefinitionAST *ast ) {
        if ( ast && m_currenActionIsAdd && m_currentColumns ) {
            if ( ColumnNameAST *columnNameAst = ast->columnName ) {
                m_currentColumns->insert(columnNameAst);
            }
        }
        return false;
    }

    virtual bool visit( AlterTableStatementAST *ast ) {
        if ( ast && ast->schemaTableName && ast->schemaTableName->tableName ) {
            QString tableName(ast->schemaTableName->tableName->name->chars());
            tableName = tableName.toUpper();
            QMap<QString, TableColumns *>::iterator it = m_tableColumns.find(tableName);
            if ( it == m_tableColumns.end() ) {
                m_currentColumns = new TableColumns();
                m_tableColumns[tableName] = m_currentColumns;
            }
            else {
                m_currentColumns = *it;
            }
            return true;
        }
        return false;
    }

    virtual bool visit( AlterTableDropClauseAST *ast ) {
        if ( ast ) {
            m_currenActionIsAdd = false;
        }
        return true;
    }

    virtual bool visit( AlterTableAddColumnClauseAST *ast ) {
        if ( ast ) {
            m_currenActionIsAdd = true;
        }
        return true;
    }

    virtual bool visit( ColumnNameAST *ast ) {
        if ( ast && m_currentColumns && m_currenActionIsAdd ) {
            m_currentColumns->insert(ast);
        }
        return false;
    }

private:
    Core::ILink *createTextTargetLinkForToken( unsigned token )
    {
        unsigned line;
        unsigned column;
        m_translationUnit->getTokenStartPosition(token, &line, &column);
        if ( !line )
            column++;
        return new Core::TextTargetLink(m_begin, m_end, "", line, column - 1);
    }

    Core::ILink *linkForTableColumn( NameAST *tableAst, const QString &columnName ) {
        QString tableName(tableAst->name->chars());
        tableName = tableName.toUpper();
        if ( Core::ILink *link = createSubobjectLink( m_connection,
                                                      tableName,
                                                      columnName,
                                                      Sql::Constants::DbSchemaObjectType::C_TABLE,
                                                      Sql::Constants::DbSchemaObjectSubtype::C_TABLE_COLUMN,
                                                      m_begin, m_end ) )
            return link;
        if ( Core::ILink *link = createSubobjectLink( m_connection,
                                                      tableName,
                                                      columnName,
                                                      Sql::Constants::DbSchemaObjectType::C_VIEW,
                                                      Sql::Constants::DbSchemaObjectSubtype::C_VIEW_COLUMN,
                                                      m_begin, m_end ) )
            return link;
        QMap<QString, TableColumns *>::iterator it = m_tableColumns.find(tableName);
        if ( it != m_tableColumns.end() ) {
            TableColumns *tableColumns = *it;
            if ( NameAST *linkAst = tableColumns->find(columnName) ) {
                return createTextTargetLinkForToken(linkAst->name_token);
            }
        }
        return 0;
    }

    TranslationUnit *m_translationUnit;
    Database::DbConnection *m_connection;
    int m_begin;
    int m_end;

    QMap<QString, TableColumns *> m_tableColumns;
    TableColumns *m_currentColumns;
    bool m_currenActionIsAdd;
};

}   // Anonimous namespace

SqlLinkFinder::SqlLinkFinder( TranslationUnit *unit )
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
            const Token &tk = m_translationUnit->tokenAt(i - 1);
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

bool SqlLinkFinder::visit( SelectCoreAST *ast )
{
    if ( ast ) {
        if ( ast->joinSource )
            ast->joinSource->accept(this);
        if ( ast->columnList )
            accept(ast->columnList);
        if ( ast->whereExpr )
            ast->whereExpr->accept(this);
        if ( ast->groupBy )
            ast->groupBy->accept(this);
    }
    return false;
}

bool SqlLinkFinder::visit( FkReferencesClauseAST *ast )
{
    if ( ast && ast->refSchemaTableName && ast->refSchemaTableName->tableName && ast->ref_col_list &&
         m_linkTokenIndex >= ast->ref_col_list->firstToken() && m_linkTokenIndex <= ast->ref_col_list->lastToken() ) {
        m_tableAliasesToNames.clear();
        m_tableAliasesToNames[QString(ast->refSchemaTableName->tableName->name->chars()).toUpper()] = ast->refSchemaTableName->tableName;
        return true;
    }

    return false;
}

bool SqlLinkFinder::visit( TableNameAST *ast )
{
    if ( !m_link && ast && m_linkTokenIndex == ast->name_token ) {
        m_link = findSchemaObjectLink(Constants::DbSchemaObjectType::C_TABLE);
        if ( !m_link ) {
            m_link = findSchemaObjectLink(Constants::DbSchemaObjectType::C_VIEW);
            if ( !m_link )
                m_link = findCreateTableLink(QString(ast->name->chars()));
        }
    }
    return false;
}

bool SqlLinkFinder::visit( ColumnNameAST *ast )
{
    if ( !m_link && ast && m_linkTokenIndex == ast->name_token ) {
        if ( ast->tableOfColumn ) {
            QString aliasName = QString(ast->tableOfColumn->name->chars()).toUpper();
            QMap<QString, NameAST *>::iterator it = m_tableAliasesToNames.find(aliasName);
            if ( it != m_tableAliasesToNames.end() ) {
                ColumnFinder finder(m_translationUnit, m_connection);
                m_link = finder( m_ast, m_currentStatemenAst, m_begin, m_end, QList<NameAST *>() << (*it), m_linkText );
            }
        }
        else {
            ColumnFinder finder(m_translationUnit, m_connection);
            m_link = finder( m_ast, m_currentStatemenAst, m_begin, m_end, m_tableAliasesToNames.values(), m_linkText );
        }
    }
    return true;
}

bool SqlLinkFinder::visit( StarResultColumnAST *ast )
{
    if ( ast && ast->schemaTableName && ast->schemaTableName->tableName && m_linkTokenIndex == ast->schemaTableName->tableName->name_token ) {
        QMap<QString, NameAST *>::iterator it = m_tableAliasesToNames.find(m_linkText);
        if ( it != m_tableAliasesToNames.end() ) {
            m_link = createTextTargetLinkForToken((*it)->name_token);
        }
    }
    return false;
}

bool SqlLinkFinder::visit( SchemaTableNameAST *ast )
{
    if ( ast && ast->tableName ) {
        m_tableAliasesToNames[QString(ast->tableName->name->chars()).toUpper()] = ast->tableName;
    }
    return true;
}

bool SqlLinkFinder::visit( TableAliasAST *ast )
{
    if ( ast ) {
        if ( ast->schemaTableName && ast->schemaTableName->tableName ) {
            m_tableAliasesToNames[QString(ast->name->chars()).toUpper()] = ast->schemaTableName->tableName;
        }
        if ( !m_link && m_linkTokenIndex == ast->name_token ) {
            QMap<QString, NameAST *>::iterator it = m_tableAliasesToNames.find(m_linkText);
            if ( it != m_tableAliasesToNames.end() ) {
                m_link = createTextTargetLinkForToken((*it)->name_token);
            }
        }
    }
    return false;
}

bool SqlLinkFinder::visit( IndexNameAST *ast )
{
    if ( !m_link && ast && m_linkTokenIndex == ast->name_token ) {
        m_link = findSchemaObjectLink(Constants::DbSchemaObjectType::C_INDEX);
        if ( !m_link )
            m_link = findCreateIndexLink(QString(ast->name->chars()));
    }
    return false;
}

bool SqlLinkFinder::visit( TriggerNameAST *ast )
{
    if ( !m_link && ast && m_linkTokenIndex == ast->name_token ) {
        m_link = findSchemaObjectLink(Constants::DbSchemaObjectType::C_TRIGGER);
        if ( !m_link )
            m_link = findCreateTriggerLink(QString(ast->name->chars()));
    }
    return false;
}

bool SqlLinkFinder::visit( ConstraintNameAST *ast )
{
    if ( !m_link && ast && m_linkTokenIndex == ast->name_token ) {
        m_link = findSchemaObjectLink(Constants::DbSchemaObjectType::C_CONSTRAINT);
        if ( !m_link )
            m_link = findCreateConstraintLink(QString(ast->name->chars()));
    }
    return false;
}

Core::ILink *SqlLinkFinder::findLinkInternal( TranslationUnitAST *ast )
{
    for ( StatementListAST *iter = ast->statement_list; iter; iter = iter->next ) {
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
    if ( m_provider = DbMetadataProvider::getInstance() ) {
        if ( m_connection = m_provider->connection() ) {
            ast->accept(this);
            return m_link;
        }
    }
    return 0;
}

DetailsLink *SqlLinkFinder::findSchemaObjectLink( unsigned objectType )
{
    if ( QList<Database::IDbCatalogItem *> *objects = m_connection->schemaObjects("", objectType) ) {
        for (int i = 0; i < objects->size(); i++) {
            Database::IDbCatalogItem *object = objects->at(i);
            if ( !m_linkText.compare(object->name(), Qt::CaseInsensitive) ) {
                return new DetailsLink(m_begin, m_end, m_connection, object);
            }
        }
    }
    return 0;
}

Core::ILink *SqlLinkFinder::findCreateTableLink( const QString &tableName )
{
    if ( m_ast ) {
        bool currentAstNotProceded = true;
        for ( StatementListAST *iter = m_ast->statement_list; iter && currentAstNotProceded; iter = iter->next ) {
            if ( StatementAST *statement = iter->value ) {
                if ( m_currentStatemenAst == statement )
                    currentAstNotProceded = false;
                if ( CreateTableStatementAST *createTableAst = statement->asCreateTableStatement() ) {
                    if ( createTableAst->schemaTableName && createTableAst->schemaTableName->tableName ) {
                        TableNameAST *tableNameAst = createTableAst->schemaTableName->tableName;
                        if ( !tableName.compare(tableNameAst->name->chars(), Qt::CaseInsensitive) ) {
                            return createTextTargetLinkForToken(tableNameAst->name_token);
                        }
                    }
                } else if ( CreateViewStatementAST *createViewAst = statement->asCreateViewStatement() ) {
                    if ( createViewAst->schemaViewName && createViewAst->schemaViewName->tableName ) {
                        TableNameAST *tableNameAst = createViewAst->schemaViewName->tableName;
                        if ( !tableName.compare(tableNameAst->name->chars(), Qt::CaseInsensitive) ) {
                            return createTextTargetLinkForToken(tableNameAst->name_token);
                        }
                    }
                }
            }
        }
    }

    return 0;
}

Core::ILink *SqlLinkFinder::findCreateIndexLink( const QString &indexName )
{
    if ( m_ast ) {
        bool currentAstNotProceded = true;
        for ( StatementListAST *iter = m_ast->statement_list; iter && currentAstNotProceded; iter = iter->next ) {
            if ( StatementAST *statement = iter->value ) {
                if ( m_currentStatemenAst == statement )
                    currentAstNotProceded = false;
                if ( CreateIndexStatementAST *createIndexAst = statement->asCreateIndexStatement() ) {
                    if ( createIndexAst->schemaIndexName && createIndexAst->schemaIndexName->indexName ) {
                        IndexNameAST *indexNameAst = createIndexAst->schemaIndexName->indexName;
                        if ( !indexName.compare(indexNameAst->name->chars(), Qt::CaseInsensitive) ) {
                            return createTextTargetLinkForToken(indexNameAst->name_token);
                        }
                    }
                }
            }
        }
    }

    return 0;
}

Core::ILink *SqlLinkFinder::findCreateTriggerLink( const QString &triggerName )
{
    if ( m_ast ) {
        bool currentAstNotProceded = true;
        for ( StatementListAST *iter = m_ast->statement_list; iter && currentAstNotProceded; iter = iter->next ) {
            if ( StatementAST *statement = iter->value ) {
                if ( m_currentStatemenAst == statement )
                    currentAstNotProceded = false;
                if ( CreateTriggerStatementAST *createTriggerAst = statement->asCreateTriggerStatement() ) {
                    if ( createTriggerAst->schemaTriggerName && createTriggerAst->schemaTriggerName->triggerName ) {
                        TriggerNameAST *triggerNameAst = createTriggerAst->schemaTriggerName->triggerName;
                        if ( !triggerName.compare(triggerNameAst->name->chars(), Qt::CaseInsensitive) ) {
                            return createTextTargetLinkForToken(triggerNameAst->name_token);
                        }
                    }
                }
            }
        }
    }

    return 0;
}

Core::ILink *SqlLinkFinder::findCreateConstraintLink( const QString &constraintName )
{
    if ( m_ast ) {
        bool currentAstNotProceded = true;
        for ( StatementListAST *iter = m_ast->statement_list; iter && currentAstNotProceded; iter = iter->next ) {
            if ( StatementAST *statement = iter->value ) {
                if ( m_currentStatemenAst == statement )
                    currentAstNotProceded = false;
                if ( CreateTableStatementAST *createTableAst = statement->asCreateTableStatement() ) {
                    if ( createTableAst->tableClause ) {
                        if ( CreateTableDirectClauseAST *createClause = createTableAst->tableClause->asCreateTableDirectClause() ) {
                            for ( TableConstraintListAST *iter = createClause->constraintList; iter; iter = iter->next ) {
                                if ( TableConstraintAST *constraint = iter->value ) {
                                    if ( ConstraintNameAST *constraintNameAst = constraint->constraintName ) {
                                        if ( !constraintName.compare(constraintNameAst->name->chars()) ) {
                                            return createTextTargetLinkForToken(constraintNameAst->name_token);
                                        }
                                    }
                                }
                            }
                            for ( ColumnDefinitionListAST *it = createClause->column_def_list; it; it = it->next ) {
                                if ( ColumnDefinitionAST *columnDefAst = it->value ) {
                                    for ( ColumnConstraintListAST *itc = columnDefAst->constraintList; itc; itc = itc->next ) {
                                        if ( ColumnConstraintAST *colConstrainAst = itc->value ) {
                                            if ( ConstraintNameAST *constraintNameAst = colConstrainAst->constraintName ) {
                                                if ( !constraintName.compare(constraintNameAst->name->chars()) ) {
                                                    return createTextTargetLinkForToken(constraintNameAst->name_token);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                } else if ( AlterTableStatementAST *alterTableAst = statement->asAlterTableStatement() ) {
                    if ( alterTableAst->alterTableClause ) {
                        if ( AlterTableAddConstraintClauseAST *addClause = alterTableAst->alterTableClause->asAlterTableAddConstraintClause() ) {
                            if ( TableConstraintAST *constraint = addClause->constraint ) {
                                if ( ConstraintNameAST *constraintNameAst = constraint->constraintName ) {
                                    if ( !constraintName.compare(constraintNameAst->name->chars()) ) {
                                        return createTextTargetLinkForToken(constraintNameAst->name_token);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}

/// TODO: To Static Method
Core::ILink *SqlLinkFinder::createTextTargetLinkForToken( unsigned token )
{
    unsigned line;
    unsigned column;
    m_translationUnit->getTokenStartPosition(token, &line, &column);
    if ( !line )
        column++;
    return new Core::TextTargetLink(m_begin, m_end, "", line, column - 1);
}

}   // namespace RDB
