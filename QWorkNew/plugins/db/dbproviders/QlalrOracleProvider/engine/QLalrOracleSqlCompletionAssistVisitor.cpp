#include "QLalrOracleSqlCompletionAssistVisitor.h"
#include "DbMetadataProvider.h"
#include "SqlLexer.h"
#include "QLalrOracleKeywords.h"

namespace QLalrOracle {

void TableType::findOrInsertColumn(const QString &column )
{
    foreach ( const QString &columnName, columns )
        if ( !columnName.compare(column, Qt::CaseInsensitive) )
            return;
    columns.append(column.toUpper());
}

bool TableType::findOrInsertIndex( const QString &index )
{
    foreach ( const QString &indexName, indexes )
        if ( !indexName.compare(index, Qt::CaseInsensitive) )
            return true;

    indexes.append(index);
    return false;
}

class SqlScopeBindVisitor: protected ASTVisitor
{
public:
    SqlScopeBindVisitor( Sql::TranslationUnit *unit, FileScope *globalScope )
        : ASTVisitor(unit)
        , m_globalScope(globalScope)
        , m_currentTableType(0)
    {}

    void bind( TranslationUnitAST *ast ) {
        for ( StatementListAST *iter = ast->statement_list; iter; iter = iter->next ) {
            m_currentTableType = 0;
            if ( CreateStatementAST *createStatement = iter->value->asCreateStatement() )
                createStatement->accept(this);
        }
    }

protected:
    bool visit( CreateTableStatementAST *ast ) {
        if ( ast ) {
            if ( TableIdentifierAST *tableIdent = ast->tableIdent ) {
                QString tableName = "";
                if ( tableIdent->tableName )
                    tableName = QString(tableIdent->tableName->name->chars());
                if ( !tableName.isEmpty() ) {
                    QString schemaName = "";
                    if ( tableIdent->schemaName )
                        schemaName = QString(tableIdent->schemaName->name->chars());
                    m_currentTableType = m_globalScope->findOrInsertTable(schemaName, tableName);
                    accept(ast->tableClause);
                }
            }
        }
        return false;
    }
    bool visit( CreateIndexStatementAST *ast ){
        if ( ast->indexIdent && ast->indexIdent->indexName && ast->indexClause ) {
            if ( TableCreateIndexClauseAST *tableClause = ast->indexClause->asTableCreateIndexClause() ) {
                if ( tableClause->tableIdent && tableClause->tableIdent->tableName ) {
                    QString tableName = QString(tableClause->tableIdent->tableName->name->chars());
                    QString schemaName = "";
                    if ( tableClause->tableIdent->schemaName )
                        schemaName = QString(tableClause->tableIdent->schemaName->name->chars());
                    m_currentTableType = m_globalScope->findOrInsertTable(schemaName, tableName);
                    if ( m_currentTableType ) {
                        m_currentTableType->findOrInsertIndex(ast->indexIdent->indexName->name->chars());
                        schemaName = "";
                        if ( ast->indexIdent->schemaName )
                            schemaName = QString(ast->indexIdent->schemaName->name->chars());
                        m_globalScope->findOrInsertIndex(schemaName, ast->indexIdent->indexName->name->chars());
                    }
                }
            }
        }

        return false;
    }

    bool visit( ColumnDefinitionAST *ast ) {
        if (  m_currentTableType && ast && ast->columnName ) {
            m_currentTableType->findOrInsertColumn(ast->columnName->name->chars());
        }
        return false;
    }


private:
    FileScope *m_globalScope;
    TableType *m_currentTableType;
};

SqlCompletionAssistVisitor::SqlCompletionAssistVisitor( Sql::TranslationUnit *unit )
    : ASTVisitor(unit)
    , m_ast(0)
{
    m_defaultSchemaName = Sql::DbMetadataProvider::getInstance()->defaultSchemaName();
    m_globalScope = new FileScope(m_defaultSchemaName);
}

SqlCompletionAssistVisitor::~SqlCompletionAssistVisitor()
{
    delete m_globalScope;
}

int SqlCompletionAssistVisitor::fillCompletions( QList<TextEditor::BasicProposalItem *> *completions, TranslationUnitAST *ast,
                                                 unsigned position, int startPosition, unsigned completionOperator )
{
    if ( !ast )
        return -1;

    SqlScopeBindVisitor binder(translationUnit(), m_globalScope);
    binder.bind(ast);

    m_ast = ast;
    m_completions = completions;
    m_startPosition = startPosition;
    m_completionOperator = completionOperator;

    m_tokenIndexBefore = -1;

    for (unsigned i = 1; i < translationUnit()->tokenCount(); i++)
        if ( translationUnit()->getTokenOffset(i) >= position ) {
            m_tokenIndexBefore = i - 1;
            break;
        }

    prepareArroundContext();

    if ( m_completionOperator == Sql::T_COLON ) {
        StatementAST *currentAst = 0;
        for ( StatementListAST *iter= m_ast->statement_list; iter; iter = iter->next ) {
            StatementAST *statement = iter->value;
            if ( !statement )
                continue;
            if ( m_tokenIndexBefore >= statement->firstToken() && m_tokenIndexBefore <= statement->lastToken() ) {
                currentAst = statement;
                break;
            }
        }
        if ( currentAst ) {
            // Complete Bindings
            SqlBindingsVisitor bindingsVisitor(translationUnit());
            m_ast->accept(&bindingsVisitor);
            bindingsVisitor.fillCompletitions(m_completions);
        }
    }

    else {
        for ( StatementListAST *iter = m_ast->statement_list; iter; iter = iter->next ) {
            StatementAST *statement = iter->value;
            if ( !statement )
                continue;
            if ( m_tokenIndexBefore >= statement->firstToken() && m_tokenIndexBefore <= statement->lastToken() ) {
                if ( statement->asInvalidStatement() )
                    completeSimpleSequences(statement->firstToken());
                else {
                    statement->accept(this);
                    if ( m_completions->isEmpty() && m_completionOperator == Sql::T_EOF_SYMBOL )
                        completeSimpleSequences(statement->firstToken());
                }
                break;
            }
        }
    }


    return m_startPosition;
}

bool SqlCompletionAssistVisitor::checkSchemaName( const QString &schemaName )
{
    return Sql::DbMetadataProvider::getInstance()->checkSchemaName(schemaName);
}

void SqlCompletionAssistVisitor::addKeywords()
{
    if ( m_completionOperator == Sql::T_IDENTIFIER )
        Sql::Token::populateKeywords(*m_completions);
}

void SqlCompletionAssistVisitor::addSchemas()
{
    Sql::DbMetadataProvider::getInstance()->fillSchemas(*m_completions);
}

void SqlCompletionAssistVisitor::addTablesAndViews( const QString &schemaName, const QString &exceptTableName )
{
    Sql::DbMetadataProvider *provider = Sql::DbMetadataProvider::getInstance();
    m_globalScope->addTables(schemaName, exceptTableName, m_completions);
    provider->fillSchemaObjects(schemaName, *m_completions, Sql::Constants::DbSchemaObjectType::C_TABLE);
    provider->fillSchemaObjects(schemaName, *m_completions, Sql::Constants::DbSchemaObjectType::C_VIEW);
}

void SqlCompletionAssistVisitor::addTables( const QString &schemaName )
{
    m_globalScope->addTables(schemaName, "", m_completions);
    Sql::DbMetadataProvider::getInstance()->fillSchemaObjects(schemaName, *m_completions, Sql::Constants::DbSchemaObjectType::C_TABLE);
}

void SqlCompletionAssistVisitor::addViews( const QString &schemaName )
{
    Sql::DbMetadataProvider::getInstance()->fillSchemaObjects(schemaName, *m_completions, Sql::Constants::DbSchemaObjectType::C_VIEW);
}

void SqlCompletionAssistVisitor::addIndices( const QString &schemaName )
{
    m_globalScope->addIndexes(schemaName, m_completions);
    Sql::DbMetadataProvider::getInstance()->fillSchemaObjects(schemaName, *m_completions, Sql::Constants::DbSchemaObjectType::C_INDEX);
}

void SqlCompletionAssistVisitor::addTriggers( const QString &schemaName )
{
    Sql::DbMetadataProvider::getInstance()->fillSchemaObjects(schemaName, *m_completions, Sql::Constants::DbSchemaObjectType::C_TRIGGER);
}

void SqlCompletionAssistVisitor::addSchemaTableFields( const QString &schemaName, const QString &tableName )
{
    m_globalScope->addTableColumns(schemaName, tableName, m_completions);
    Sql::DbMetadataProvider::getInstance()->fillTableColumns(schemaName, tableName, *m_completions);
}

void SqlCompletionAssistVisitor::addSchemaViewFields( const QString &schemaName, const QString &viewName )
{
    Sql::DbMetadataProvider::getInstance()->fillViewColumns(schemaName, viewName, *m_completions);
}

void SqlCompletionAssistVisitor::addConstraints( const QString &schemaName, const QString &tableName )
{
    Sql::DbMetadataProvider::getInstance()->fillTableConstraints(schemaName, tableName, *m_completions);
}

void SqlCompletionAssistVisitor::addSimpleKeyword(const QString &text)
{
    TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
    item ->setText(text);
    item->setIcon(QIcon(":/images/sql_ci_keyword.png"));
    m_completions->append(item);
}

void SqlCompletionAssistVisitor::addSimpleSequence( const QString &text )
{
    TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
    item ->setText(text);
    item->setIcon(QIcon(":/images/sql_keyword.png"));
    m_completions->append(item);
}

void SqlCompletionAssistVisitor::addSimpleField(const QString &field)
{
    TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
    item ->setText(field);
    item->setIcon(QIcon(":/images/sql_ci_column.png"));
    m_completions->append(item);
}

void SqlCompletionAssistVisitor::completeSimpleSequences( unsigned firstToken )
{
    if ( m_completionOperator == Sql::T_EOF_SYMBOL ) {

        if ( m_tokenIndexBefore == firstToken ) {
            switch ( translationUnit()->tokenKeyword(firstToken) ) {
                case T_5_ALTER:
                    addSimpleKeyword("TABLE");
                    break;
                case T_6_SELECT:
                    addSimpleSequence("* FROM");
                    break;
                case T_6_INSERT:
                    addSimpleKeyword("INTO");
                    break;
                case T_6_DELETE:
                    addSimpleKeyword("FROM");
                    break;
                case T_6_CREATE:
                case T_4_DROP:
                    addSimpleKeyword("TABLE");
                    addSimpleKeyword("VIEW");
                    addSimpleKeyword("INDEX");
                    addSimpleKeyword("TRIGGER");
                    break;
                case T_7_COMMENT:
                    addSimpleSequence("ON TABLE");
                    addSimpleSequence("ON COLUMN");
                    break;
            }
            return;
        }
    }
    else
        addKeywords();
}

bool SqlCompletionAssistVisitor::visit( SelectStatementAST *ast )
{
    if ( ast ) {
        accept(ast->select_core);
        accept(ast->order_by_block);
        accept(ast->forupdate_clause);
        complete(ast);
    }

    return false;
}

bool SqlCompletionAssistVisitor::visit( DirectSingleSourceAST *ast )
{
    if ( ast ) {
        if ( ast->schemaTableName ) {
            if ( ast->schemaTableName->tableName ) {
                QString schemaName = "";
                QString aliasName;
                QString tableName(ast->schemaTableName->tableName->name->chars());
                if ( ast->tableAlias )
                    aliasName = QString(ast->tableAlias->name->chars());
                else
                    aliasName = tableName;
                if ( ast->schemaTableName->schemaName )
                    schemaName = QString(ast->schemaTableName->schemaName->name->chars());
                m_tableAliasesToNames[aliasName] = qMakePair(tableName, schemaName);
            }
        }
    }

    return false;
}

bool SqlCompletionAssistVisitor::visit( TableIdentifierAST *ast )
{
    if ( ast && ast->tableName ) {
        QString schemaName = "";
        if ( ast->schemaName )
            schemaName = QString(ast->schemaName->name->chars());
        QString tableName = QString(ast->tableName->name->chars());
        m_tableAliasesToNames[tableName] = qMakePair(tableName, schemaName);
    }

    return false;
}

void SqlCompletionAssistVisitor::complete( SelectStatementAST *ast )
{
    if ( OrderByBlockAST *orderByBlockAst = ast->order_by_block ) {
        if ( m_tokenIndexBefore >= orderByBlockAst->firstToken() ) {
            if ( m_tokenIndexBefore == orderByBlockAst->firstToken() ) {
                if ( !orderByBlockAst->by_token )
                    addSimpleKeyword("BY");
                return;
            }
            completeSchemaTableViewField();
            return;
        }
    }

    if ( ast->select_core )
        if ( complete(ast->select_core, false) )
            return;

    if ( translationUnit()->tokenAt(m_tokenIndexBefore).kind() != Sql::T_IDENTIFIER )
        addKeywords();
}

bool SqlCompletionAssistVisitor::complete(SelectCoreAST *ast , bool hasNext )
{
    if ( !ast )
        return false;

    if ( hasNext && m_tokenIndexBefore > ast->lastToken() )
        return false;

    if ( GroupByBlockAST *groupByBlockAst = ast->groupBy ) {
        if ( m_tokenIndexBefore >= groupByBlockAst->firstToken() ) {
            if ( m_tokenIndexBefore == groupByBlockAst->firstToken() ) {
                if ( !groupByBlockAst->by_token )
                    addSimpleKeyword("BY");
                return true;
            }
            completeSchemaTableViewField();
            return true;
        }
    }

    if ( ast->whereClause && m_tokenIndexBefore >= ast->whereClause->firstToken() ) {
        completeSchemaTableViewField();
        return true;
    }

    if ( ast->joinSource && m_tokenIndexBefore >= ast->joinSource->firstToken() ) {
        completeSchemaTableViewField();
        return true;
    }

    if ( ast->from_token && m_tokenIndexBefore >= ast->from_token ) {
        completeSchemaTableView();
        return true;
    }

    completeSchemaTableViewField();
    return true;
}

bool SqlCompletionAssistVisitor::visit( UpdateStatementAST *ast )
{
    if ( ast ) {
        accept(ast->source);
        accept(ast->setClause);
        accept(ast->whereClause);
        complete(ast);
    }

    return false;
}

bool SqlCompletionAssistVisitor::complete( UpdateStatementAST *ast )
{
    if ( ast->setClause && m_tokenIndexBefore >= ast->setClause->firstToken() )
        completeSchemaTableViewField();
    else
        completeSchemaTableView();

    completeSchemaTableView();
    return true;
}

bool SqlCompletionAssistVisitor::visit( InsertStatementAST *ast )
{
    if ( ast ) {
        accept(ast->source);
        accept(ast->insertColumns);
        accept(ast->insClause);
        complete(ast);
    }
    return false;
}

bool SqlCompletionAssistVisitor::complete( InsertStatementAST *ast )
{
    if ( ast->insClause ) {
        if ( InsertClauseAsSelectAST *selAst = ast->insClause->asInsertClauseAsSelect() ) {
            if ( m_tokenIndexBefore >= selAst->firstToken() && selAst->selectStatement ) {
                complete(selAst->selectStatement->asSelectStatement());
                return true;
            }
        }
    }

    if ( ast->source ) {
        if ( !ast->insClause && m_completionOperator == Sql::T_EOF_SYMBOL &&
             ( ast->insertColumns && m_tokenIndexBefore >= ast->insertColumns->lastToken() - 1 ) ||
             ( !ast->insertColumns && m_tokenIndexBefore >= ast->source->lastToken() - 1 ) ) {
            addSimpleSequence("SELECT * FROM ");
            addSimpleSequence("VALUES ()");
            return true;
        }
        if ( m_tokenIndexBefore >= ast->source->firstToken() && m_tokenIndexBefore <= ast->source->lastToken() ) {
            completeSchemaTableView();
            return true;
        }
    }

    completeSchemaTableViewField();
    return true;
}

bool SqlCompletionAssistVisitor::visit( DeleteStatementAST *ast )
{
    if ( ast ) {
        accept(ast->source);
        accept(ast->whereClause);
        complete(ast);
    }
    return false;
}

bool SqlCompletionAssistVisitor::complete( DeleteStatementAST *ast )
{
    if ( ast->whereClause && m_tokenIndexBefore >= ast->whereClause->firstToken() )
        completeSchemaTableViewField();
    else
        completeSchemaTableView();
    return true;
}

bool SqlCompletionAssistVisitor::visit( CreateTableStatementAST *ast )
{
    if ( ast ) {
        accept(ast->tableIdent);
        accept(ast->tableClause);
        complete(ast);
    }
    return false;
}

bool SqlCompletionAssistVisitor::complete( CreateTableStatementAST *ast )
{
    if ( ast->tableClause ) {
        if ( CreateTableAsSelectClauseAST *selAst = ast->tableClause->asCreateTableAsSelectClause() ) {
            if ( m_tokenIndexBefore >= selAst->firstToken() && selAst->selectStatement ) {
                complete(selAst->asSelectStatement());
                return true;
            }
        }
        else if ( CreateTableDirectClauseAST *dirAst = ast->tableClause->asCreateTableDirectClause() ) {
            if ( m_tokenIndexBefore >= dirAst->firstToken()) {
                if ( complete(dirAst) )
                    return true;
            }
        }
    }

    if ( ast->table_token && m_tokenIndexBefore >= ast->table_token &&
         ( !ast->tableClause || (ast->tableClause && m_tokenIndexBefore <= ast->tableClause->firstToken()) ) ) {
        completeSchemaTable();
        return true;
    }

    addKeywords();
    return true;
}

bool SqlCompletionAssistVisitor::complete( CreateTableDirectClauseAST *ast )
{
    if ( ast->rparen_token && m_tokenIndexBefore >= ast->rparen_token ) {
        addKeywords();
        return true;
    }

    QStringList createTableFields;
    {
        for (ColumnDefinitionListAST *col_iter = ast->column_def_list; col_iter; col_iter = col_iter->next) {
            if ( ColumnDefinitionAST *col = col_iter->value )
                if ( col->columnName )
                    if ( col->columnName->name )
                        createTableFields << col->columnName->name->chars();
        }
    }

    for (TableConstraintListAST *iter = ast->constraintList; iter; iter = iter->next) {
        TableConstraintAST *constrAst = iter->value;
        if ( !constrAst )
            continue;
        if ( m_tokenIndexBefore >= constrAst->firstToken() && m_tokenIndexBefore < constrAst->lastToken() ) {
            complete(constrAst, "", "", createTableFields);
            return true;
        }
    }

    {
        for ( ColumnDefinitionListAST *col_iter = ast->column_def_list; col_iter; col_iter = col_iter->next ) {
            if ( ColumnDefinitionAST *col = col_iter->value ) {
                for ( ColumnConstraintListAST *cons_iter = col->constraintList; cons_iter; cons_iter = cons_iter->next ) {
                    if ( ColumnConstraintAST *cons = cons_iter->value ) {
                        if ( CheckColumnConstraintAST *cc = cons->asCheckColumnConstraint() ) {
                            if ( m_tokenIndexBefore >= cc->firstToken() && m_tokenIndexBefore <= cc->lastToken() ) {
                                addKeywords();
                                addSimpleField(col->columnName->name->chars());
                                return true;
                            }
                        }
                        else if ( ReferencesColumnConstraintAST *rc = cons->asReferencesColumnConstraint() ) {
                            FkReferencesClauseAST *refClause = rc->refClause;
                            if ( !refClause )
                                continue;
                            if ( m_tokenIndexBefore > rc->firstToken() && m_tokenIndexBefore <= rc->lastToken() ) {
                                if ( refClause->ref_lparen_token && m_tokenIndexBefore >= refClause->ref_lparen_token ) {
                                    QString refTableName = "";
                                    QString refSchemaName = "";
                                    if ( refClause->tableIdent ) {
                                        if ( refClause->tableIdent->tableName )
                                            refTableName = refClause->tableIdent->tableName->name->chars();
                                        if ( refClause->tableIdent->schemaName )
                                            refSchemaName = refClause->tableIdent->schemaName->name->chars();
                                    }
                                    if ( !refTableName.isEmpty() )
                                        addSchemaTableFields(refSchemaName, refTableName);
                                }
                                else if ( refClause->references_token && m_tokenIndexBefore >= refClause->references_token ) {
                                    completeSchemaTable();
                                }
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }

    addKeywords();
    return true;
}

bool SqlCompletionAssistVisitor::visit( CreateViewStatementAST *ast )
{
    if ( ast ) {
        accept(ast->viewIdent);
        accept(ast->selectStatement);
        complete(ast);
    }
    return false;
}

bool SqlCompletionAssistVisitor::complete( CreateViewStatementAST *ast )
{
    if ( ast->selectStatement ) {
        if ( m_tokenIndexBefore >= ast->selectStatement->firstToken() ) {
            complete(ast->selectStatement->asSelectStatement());
            return true;
        }
    }
    if ( ast->view_token && m_tokenIndexBefore >= ast->view_token &&
         ( !ast->selectStatement || (ast->selectStatement && m_tokenIndexBefore <= ast->selectStatement->firstToken()) ) ) {
        completeSchemaView();
        return true;
    }

    addKeywords();
    return true;
}

bool SqlCompletionAssistVisitor::visit( CreateIndexStatementAST *ast )
{
    if ( ast ) {
        accept(ast->indexIdent);
        accept(ast->indexClause);
        complete(ast);
    }
    return false;
}

bool SqlCompletionAssistVisitor::complete( CreateIndexStatementAST *ast )
{
    if ( ast->indexClause && m_tokenIndexBefore >= ast->indexClause->firstToken() ) {
        if ( ast->indexClause->asTableCreateIndexClause() ) {
            completeSchemaTableViewField();
            return true;
        }
        else {
            addKeywords();
            return true;
        }
    }

    if ( ast->indexIdent && m_tokenIndexBefore >= ast->indexIdent->firstToken() ) {
        completeSchemaIndex();
        return true;
    }

    addKeywords();
    return true;
}

bool SqlCompletionAssistVisitor::visit( CreateTriggerStatementAST *ast )
{
    if ( ast ) {
        accept(ast->tableIdent);
        if ( ast->triggerRefClause ) {
            accept(ast->triggerRefClause->newAlias);
            accept(ast->triggerRefClause->oldAlias);
        }
        complete(ast);
    }
    return false;
}

bool SqlCompletionAssistVisitor::complete( CreateTriggerStatementAST *ast )
{
    if ( ast->triggerBody && m_tokenIndexBefore >= ast->triggerBody->firstToken() ) {
        if ( CreateRdbTriggerBodyAST *rdbBody = ast->triggerBody->asCreateRdbTriggerBody() ) {
            for (CreateRdbTriggerBodyListAST *iter = rdbBody->item_list; iter; iter = iter->next) {
                if ( CreateRdbTriggerBodyItemAST *item = iter->value ) {
                    if ( m_tokenIndexBefore <= item->lastToken() ) {
                        if ( item->each_token && m_tokenIndexBefore >= item->each_token ) {
                            if ( !item->row_or_statement_token ) {
                                addSimpleKeyword("ROW");
                                addSimpleKeyword("STATEMENT");
                            }
                        }
                        else if ( item->for_token && m_tokenIndexBefore >= item->for_token ) {
                            if ( !item->each_token ) {
                                addSimpleKeyword("EACH");
                            }
                        }
                        else if ( item->rparen_token && m_tokenIndexBefore >= item->rparen_token ) {
                            if ( !item->for_token ) {
                                addSimpleKeyword("FOR");
                            }
                        }
                        else if ( item->lparen_token && m_tokenIndexBefore >= item->lparen_token ) {
                            for ( StatementListAST *stat_list = item->stat_list; stat_list; stat_list = stat_list->next ) {
                                if ( StatementAST *statement = stat_list->value ) {
                                    if ( m_tokenIndexBefore <= statement->lastToken() ) {
                                        if ( m_tokenIndexBefore == statement->lastToken() && tokenKind() == Sql::T_COMMA ) {
                                            addSimpleKeyword("DELETE");
                                            addSimpleKeyword("ERROR");
                                            addSimpleKeyword("INSERT");
                                            addSimpleKeyword("UPDATE");
                                            return true;
                                        }
                                        if ( DeleteStatementAST *delStatement = statement->asDeleteStatement() ) {
                                            visit(delStatement);
                                            complete(delStatement);
                                        }
                                        else if ( InsertStatementAST *insStatement = statement->asInsertStatement() ) {
                                            visit(insStatement);
                                            complete(insStatement);
                                        }
                                        else if ( UpdateStatementAST *updStatement = statement->asUpdateStatement() ) {
                                            visit(updStatement);
                                            complete(updStatement);
                                        }
                                        return true;
                                    }
                                }
                            }
                            if ( tokenKind() == Sql::T_COMMA || tokenKind() == Sql::T_LPAREN ||
                                 (tokenKind() == Sql::T_IDENTIFIER && (tokenKind(-1) == Sql::T_COMMA || tokenKind(-1) == Sql::T_LPAREN)) ) {
                                addSimpleKeyword("DELETE");
                                addSimpleKeyword("ERROR");
                                addSimpleKeyword("INSERT");
                                addSimpleKeyword("UPDATE");
                            }
                        }
                        else if ( item->rparen_token && m_tokenIndexBefore >= item->rparen_token ) {}
                        else if ( item->lparen_when_token && m_tokenIndexBefore >= item->lparen_when_token ) {
                            completeSchemaTableViewField(false);
                        }
                        return true;
                    }
                }
            }
        }
        /// TODO: Oracle/SQLite (BEGIN ... END;) or Db2 (BEGIN ATOMIC ... END;) dialects
        // else if (...)
        return true;
    }

    if ( ast->triggerRefClause && m_tokenIndexBefore >= ast->triggerRefClause->lastToken() - 1 ) {
        if ( m_completionOperator == Sql::T_EOF_SYMBOL ) {
            if ( tokenKeyword() == T_2_AS ) {
                return true;
            }
            if ( tokenKeyword() == T_3_NEW || tokenKeyword() == T_3_OLD ) {
                if ( tokenKeyword(1) != T_2_AS ) {
                    addSimpleKeyword("AS");
                }
                return true;
            }
            if ( tokenKeyword() == T_11_REFERENCING || tokenKind() == Sql::T_IDENTIFIER ) {
                if ( tokenKeyword(1) != T_3_NEW && tokenKeyword(1) != T_3_OLD ) {
                    addSimpleKeyword("NEW");
                    addSimpleKeyword("OLD");
                    if ( tokenKind() == Sql::T_IDENTIFIER && tokenKeyword(1) != T_4_WHEN )
                        addSimpleKeyword("WHEN");
                }
                return true;
            }
        }
        if ( m_completionOperator == Sql::T_IDENTIFIER ) {
            addSimpleKeyword("AS");
            addSimpleKeyword("NEW");
            addSimpleKeyword("OLD");
            addSimpleKeyword("WHEN");
        }
        return true;
    }

    if ( ast->tableIdent && ( m_tokenIndexBefore >= ast->tableIdent->lastToken() ||
                                  (m_completionOperator == Sql::T_EOF_SYMBOL && m_tokenIndexBefore >= ast->tableIdent->lastToken() - 1) ) ) {
        if ( !ast->triggerRefClause )
            addSimpleKeyword("REFERENCING");
        return true;
    }

    if ( ast->on_token && m_tokenIndexBefore >= ast->on_token ) {
        completeSchemaTable(false);
        return true;
    }

    if ( ast->of_token && m_tokenIndexBefore >= ast->of_token ) {
        if ( ast->tableIdent ) {
            QString tableName = ast->tableIdent->tableName->name->chars();
            if ( !tableName.isEmpty() ) {
                QString schemaName = "";
                if ( ast->tableIdent->schemaName )
                    schemaName = ast->tableIdent->schemaName->name->chars();
                Sql::DbMetadataProvider::getInstance()->fillTableColumns(schemaName, tableName, *m_completions);
            }
        }
        if ( !ast->on_token )
            addSimpleKeyword("ON");
        return true;
    }

    if ( ast->action_token && m_tokenIndexBefore >= ast->action_token ) {
        if ( !ast->on_token )
            addSimpleKeyword("ON");
        if ( !ast->of_token && (tokenKeyword() == T_6_UPDATE || tokenKeyword(-1) == T_6_UPDATE) )
            addSimpleKeyword("OF");
        return true;
    }

    if ( ast->before_or_after_token && m_tokenIndexBefore >= ast->before_or_after_token ) {
        if ( !ast->action_token ) {
            addSimpleKeyword("DELETE");
            addSimpleKeyword("INSERT");
            addSimpleKeyword("UPDATE");
        }
        return true;
    }

    if ( ast->triggeeIdent && ( m_tokenIndexBefore >= ast->triggeeIdent->lastToken() ||
                                     (m_completionOperator == Sql::T_EOF_SYMBOL && m_tokenIndexBefore >= ast->triggeeIdent->lastToken() - 1) ) ) {
        if ( m_tokenIndexBefore == ast->triggeeIdent->lastToken() && m_completionOperator == Sql::T_DOT ) {
            completeSchemaTrigger(false);
            return true;
        }
        if ( !ast->before_or_after_token ) {
            addSimpleKeyword("AFTER");
            addSimpleKeyword("BEFORE");
        }
        return true;
    }

    if ( ast->trigger_token && m_tokenIndexBefore >= ast->trigger_token ) {
        completeSchemaTrigger(false);
        return true;
    }

    addKeywords();
    return true;
}

bool SqlCompletionAssistVisitor::visit( DropTableStatementAST *ast )
{
    if ( ast ) {
        accept(ast->tableIdent);
        complete(ast);
    }
    return false;
}

bool SqlCompletionAssistVisitor::complete( DropTableStatementAST *ast )
{
    if ( ast->tableIdent && m_tokenIndexBefore >= ast->tableIdent->firstToken() ) {
        completeSchemaTable();
        return true;
    }

    addKeywords();
    return true;
}

bool SqlCompletionAssistVisitor::visit( DropViewStatementAST *ast )
{
    if ( ast ) {
        accept(ast->viewIdent);
        complete(ast);
    }
    return false;
}

bool SqlCompletionAssistVisitor::complete( DropViewStatementAST *ast )
{
    if ( ast->viewIdent && m_tokenIndexBefore >= ast->viewIdent->firstToken() ) {
        completeSchemaView();
        return true;
    }

    addKeywords();
    return true;
}

bool SqlCompletionAssistVisitor::visit( DropIndexStatementAST *ast )
{
    if ( ast ) {
        accept(ast->indexIdent);
        complete(ast);
    }
    return false;
}

bool SqlCompletionAssistVisitor::complete( DropIndexStatementAST *ast )
{
    if ( ast->indexIdent && m_tokenIndexBefore >= ast->indexIdent->firstToken() ) {
        completeSchemaIndex();
        return true;
    }

    addKeywords();
    return true;
}

bool SqlCompletionAssistVisitor::visit( DropTriggerStatementAST *ast )
{
    if ( ast ) {
        accept(ast->triggerIdent);
        complete(ast);
    }
    return false;
}

bool SqlCompletionAssistVisitor::complete( DropTriggerStatementAST *ast )
{
    if ( ast->triggerIdent && m_tokenIndexBefore >= ast->triggerIdent->firstToken() ) {
        completeSchemaTrigger();
        return true;
    }

    addKeywords();
    return true;
}

bool SqlCompletionAssistVisitor::visit( TruncateStatementAST *ast )
{
    if ( ast ) {
        accept(ast->tableIdent);
        complete(ast);
    }
    return false;
}

bool SqlCompletionAssistVisitor::complete( TruncateStatementAST *ast )
{
    if ( ast->table_token && m_tokenIndexBefore >= ast->table_token ) {
        completeSchemaTable();
        return true;
    }

    addKeywords();
    return true;
}

bool SqlCompletionAssistVisitor::visit( CommentOnTableStatementAST *ast )
{
    if ( ast ) {
        accept(ast->tableIdent);
        complete(ast);
    }
    return false;
}

bool SqlCompletionAssistVisitor::complete( CommentOnTableStatementAST *ast )
{
    if ( ast->tableIdent && m_tokenIndexBefore >= ast->tableIdent->lastToken() - 1 && !ast->is_token && m_completionOperator == Sql::T_EOF_SYMBOL ) {
        addSimpleSequence("IS ''");
        return true;
    }

    if ( ast->table_token && m_tokenIndexBefore >= ast->table_token ) {
        completeSchemaTableView();
        return true;
    }

    addKeywords();
    return true;
}

bool SqlCompletionAssistVisitor::visit( CommentOnColumnStatementAST *ast )
{
    if ( ast ) {
        accept(ast->columnIdent);
        complete(ast);
    }
    return false;
}

bool SqlCompletionAssistVisitor::complete( CommentOnColumnStatementAST *ast )
{
    if ( ast->columnIdent && m_tokenIndexBefore >= ast->columnIdent->lastToken() - 1 && !ast->is_token && m_completionOperator == Sql::T_EOF_SYMBOL ) {
        addSimpleSequence("IS ''");
        return true;
    }

    if ( ast->column_token && m_tokenIndexBefore >= ast->column_token ) {
        completeSchemaTableViewField(false, true);
        return true;
    }

    addKeywords();
    return true;
}

bool SqlCompletionAssistVisitor::visit( AlterTableStatementAST *ast )
{
    if ( ast ) {
        accept(ast->tableIdent);
        accept(ast->alterTableClause);
        complete(ast);
    }
    return false;
}

bool SqlCompletionAssistVisitor::complete( AlterTableStatementAST *ast )
{
    QString tableName = "";
    QString schemaName = "";

    if ( ast->tableIdent ) {
        if ( ast->tableIdent->tableName )
            tableName = ast->tableIdent->tableName->name->chars();
        if ( ast->tableIdent->schemaName )
            schemaName = ast->tableIdent->schemaName->name->chars();
    }

    if ( m_completionOperator == Sql::T_EOF_SYMBOL ) {
        if ( ast->tableIdent ) {
            if ( m_tokenIndexBefore == ast->tableIdent->lastToken() - 1 && !ast->alterTableClause ) {
                addSimpleSequence("ADD CONSTRAINT");
                addSimpleSequence("ADD COLUMN");
                addSimpleSequence("DROP CONSTRAINT");
                addSimpleSequence("DROP COLUMN");
                return true;
            }
        }
    }

    if ( ast->alterTableClause && m_tokenIndexBefore >= ast->alterTableClause->firstToken() ) {

        if ( AlterTableDropClauseAST *dropAst = ast->alterTableClause->asAlterTableDropClause() ) {
            if ( m_tokenIndexBefore == dropAst->firstToken() ) {
                if ( translationUnit()->tokenKeyword(m_tokenIndexBefore + 1) != T_10_CONSTRAINT &&
                     translationUnit()->tokenKeyword(m_tokenIndexBefore + 1) != T_6_COLUMN ) {
                    addSimpleKeyword("COLUMN");
                    addSimpleKeyword("CONSTRAINT");
                }
                return true;
            }
            if ( m_tokenIndexBefore == dropAst->drop_object_type_token ) {
                if ( !tableName.isEmpty() ) {
                    if ( translationUnit()->tokenKeyword(m_tokenIndexBefore) == T_10_CONSTRAINT )
                        addConstraints(schemaName, tableName);
                    else
                        addSchemaTableFields(schemaName, tableName);
                }
                return true;
            }
            if ( m_tokenIndexBefore == dropAst->drop_object_type_token + 1 &&
                 translationUnit()->tokenAt(m_tokenIndexBefore).kind() == Sql::T_IDENTIFIER && m_completionOperator != Sql::T_EOF_SYMBOL ) {
                if ( !tableName.isEmpty() ) {
                    if ( translationUnit()->tokenKeyword(m_tokenIndexBefore - 1) == T_10_CONSTRAINT )
                        addConstraints(schemaName, tableName);
                    else
                        addSchemaTableFields(schemaName, tableName);
                }
                return true;
            }
        }

        else if ( AlterTableAddColumnClauseAST *addColAst = ast->alterTableClause->asAlterTableAddColumnClause() ) {
            if ( m_tokenIndexBefore == addColAst->column_token ||
                 ( m_tokenIndexBefore == addColAst->column_token + 1 && translationUnit()->tokenAt(m_tokenIndexBefore).kind() == Sql::T_IDENTIFIER &&
                   m_completionOperator != Sql::T_EOF_SYMBOL ) ) {
                if ( !tableName.isEmpty() )
                    addSchemaTableFields(schemaName, tableName);
                return true;
            }
        }

        else if ( AlterTableAddConstraintClauseAST *addConstrAst = ast->alterTableClause->asAlterTableAddConstraintClause() ) {
            if ( addConstrAst->constraint ) {
                QStringList tmp;
                if ( complete(addConstrAst->constraint, schemaName, tableName, tmp) )
                    return true;
            }
        }
    }

    else if ( ast->table_token && m_tokenIndexBefore >= ast->table_token ) {
        completeSchemaTable();
        return true;
    }

    addKeywords();
    return true;
}

#define CHECK_IND_PREV_FOR_IDENT(index) ( m_tokenIndexBefore == (index) && m_completionOperator == Sql::T_EOF_SYMBOL ) || ( m_tokenIndexBefore == (index) + 1 && m_completionOperator == Sql::T_IDENTIFIER )
#define CHECK_IND_PREV_FOR_IDENT1(index) ( m_tokenIndexBefore == (index) ) || ( m_tokenIndexBefore == (index) - 1 && m_completionOperator == Sql::T_IDENTIFIER )
#define CHECK_IND_LESS(index) (index) && m_tokenIndexBefore >= (index)

bool SqlCompletionAssistVisitor::complete( TableConstraintAST *ast , const QString &schemaName, const QString &tableName , QStringList &createTableFields )
{
    if ( !ast || (tableName.isEmpty() && createTableFields.isEmpty()) )
        return false;

    if ( m_tokenIndexBefore < ast->firstToken() )
        return false;

    if ( CHECK_IND_PREV_FOR_IDENT1(ast->constraint_token) ) {
        addConstraints(schemaName, tableName);
        return true;
    }

    switch ( ast->tableConstraintType() ) {

        case TableConstraintAST::UniqueOrPrimaryKey: {
            UniqueOrPkTableConstraintAST *uAst = ast->asUniqueOrPkTableConstraint();
            if ( CHECK_IND_LESS(uAst->rparen_token) ) {
                // nothing to do
            }
            else if ( CHECK_IND_LESS(uAst->lparen_token) ) {
                if ( createTableFields.isEmpty() ) {
                    addSchemaTableFields(schemaName, tableName);
                }
                else {
                    foreach ( const QString &field, createTableFields ) {
                        TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
                        item->setText(field);
                        item->setIcon(QIcon(":/images/sql_ci_column.png"));
                        m_completions->append(item);
                    }
                }
            }
            else if ( CHECK_IND_PREV_FOR_IDENT(uAst->unique_or_primary_token) && !uAst->key_token /*&& tokenKeyword() == T_7_PRIMARY*/ ) {
                addSimpleKeyword("KEY");
            }
            else if ( !uAst->unique_or_primary_token && uAst->constraintName->name_token ) {
                if ( CHECK_IND_PREV_FOR_IDENT(uAst->constraintName->name_token) ) {
                    addSimpleKeyword("CHECK");
                    addSimpleKeyword("FOREIGN");
                    addSimpleKeyword("PRIMARY");
                    addSimpleKeyword("UNIQUE");
                }
            }
            break;
        }

        case TableConstraintAST::Check: {
            CheckTableConstraintAST *chAst = ast->asCheckTableConstraint();
            if ( chAst->lparen_token && m_tokenIndexBefore >= chAst->lparen_token &&
                 ( !chAst->rparen_token || (chAst->rparen_token && m_tokenIndexBefore < chAst->rparen_token) ) ) {
                if ( createTableFields.isEmpty() ) {
                    addSchemaTableFields(schemaName, tableName);
                }
                else {
                    foreach ( const QString &field, createTableFields ) {
                        TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
                        item->setText(field);
                        item->setIcon(QIcon(":/images/sql_ci_column.png"));
                        m_completions->append(item);
                    }
                }
            }
            break;
        }

        case TableConstraintAST::ForeignKey: {
            FkTableConstraintAST *fkAst = ast->asFkTableConstraint();
            if ( fkAst->refClause && m_tokenIndexBefore >= fkAst->refClause->firstToken() ) {
                if ( fkAst->refClause->ref_lparen_token && m_tokenIndexBefore >= fkAst->refClause->ref_lparen_token ) {
                    QString refTableName = "";
                    QString refSchemaName = schemaName;
                    FkReferencesClauseAST *refClause = fkAst->refClause;
                    if ( refClause->tableIdent ) {
                        if ( refClause->tableIdent->tableName )
                            refTableName = refClause->tableIdent->tableName->name->chars();
                        if ( refClause->tableIdent->schemaName )
                            refSchemaName = refClause->tableIdent->schemaName->name->chars();
                    }
                    if ( !refTableName.isEmpty() )
                        addSchemaTableFields(refSchemaName, refTableName);
                }
                else if ( fkAst->refClause->references_token && m_tokenIndexBefore >= fkAst->refClause->references_token ) {
                    completeSchemaTable();
                }
            }
            else if ( fkAst->rparen_token && m_tokenIndexBefore >= fkAst->rparen_token ) {
                if ( ( m_tokenIndexBefore == fkAst->rparen_token && m_completionOperator == Sql::T_EOF_SYMBOL ) ||
                     (  m_tokenIndexBefore == fkAst->rparen_token + 1 && m_completionOperator == Sql::T_IDENTIFIER ) )
                    addSimpleKeyword("REFERENCES");
            }
            else if ( fkAst->lparen_token && m_tokenIndexBefore >= fkAst->lparen_token ) {
                if ( createTableFields.isEmpty() ) {
                    addSchemaTableFields(schemaName, tableName);
                }
                else {
                    foreach ( const QString &field, createTableFields ) {
                        TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
                        item->setText(field);
                        item->setIcon(QIcon(":/images/sql_ci_column.png"));
                        m_completions->append(item);
                    }
                }
            }
            else if ( CHECK_IND_PREV_FOR_IDENT(fkAst->foreign_token) && !fkAst->key_token ) {
                addSimpleKeyword("KEY");
            }
            break;
        }
    }

    return true;
}

void SqlCompletionAssistVisitor::completeSchemaTableViewField( bool kwd, bool ts )
{
    if ( m_tokenIndexBefore == -1 || m_completionOperator == Sql::T_EOF_SYMBOL )
        return;

    Sql::TranslationUnit *unit = translationUnit();

    unsigned k0 = tokenKind(-4);
    unsigned k1 = tokenKind(-3);
    unsigned k2 = tokenKind(-2);
    unsigned k3 = tokenKind(-1);
    unsigned k4 = tokenKind();

    QString schemaName;
    QString tableName = "";

    if ( k0 == Sql::T_IDENTIFIER && k1 == Sql::T_DOT && k2 == Sql::T_IDENTIFIER && k3 == Sql::T_DOT && k4 == Sql::T_IDENTIFIER ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 4).identifier->chars();
        tableName  = unit->tokenAt(m_tokenIndexBefore - 2).identifier->chars();
        addSchemaTableFields(schemaName, tableName);
        addSchemaViewFields(schemaName,tableName);
        return;
    }

    else if ( k1 == Sql::T_IDENTIFIER && k2 == Sql::T_DOT && k3 == Sql::T_IDENTIFIER && k4 == Sql::T_DOT ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 3).identifier->chars();
        tableName  = unit->tokenAt(m_tokenIndexBefore - 1).identifier->chars();
        addSchemaTableFields(schemaName, tableName);
        addSchemaViewFields(schemaName, tableName);
        return;
    }

    else if ( k2 == Sql::T_IDENTIFIER && k3 == Sql::T_DOT && k4 == Sql::T_IDENTIFIER ) {
        tableName = unit->tokenAt(m_tokenIndexBefore - 2 ).identifier->chars();
    }

    else if ( k3 == Sql::T_IDENTIFIER && k4 == Sql::T_DOT ) {
        tableName = unit->tokenAt(m_tokenIndexBefore - 1 ).identifier->chars();
        if ( ts )
            m_tableAliasesToNames[tableName] = qMakePair(tableName, QString(""));
    }

    else if ( k4 == Sql::T_IDENTIFIER ) {
        tableName = "";
    }

    else {
        if ( kwd && k4 != Sql::T_IDENTIFIER )
            addKeywords();
        return;
    }

    QStringList tables;
    QStringList tableSchemas;
    QMap<QString, QPair<QString, QString> >::const_iterator i = m_tableAliasesToNames.constBegin();
    while ( i != m_tableAliasesToNames.constEnd() ) {
        if ( tableName.isEmpty() || !tableName.compare(i.key(), Qt::CaseInsensitive) /*|| !expr.compare(i.value(), Qt::CaseInsensitive)*/ ) {
             tables << i.value().first;
             tableSchemas << i.value().second;
        }
        ++i;
    }
    for (int j = 0; j < tables.size(); j++) {
        addSchemaTableFields(tableSchemas[j], tables[j]);
        addSchemaViewFields(tableSchemas[j], tables[j]);
    }

    completeSchemaTableView(kwd);
}

void SqlCompletionAssistVisitor::completeSchemaTableView( bool kwd )
{
    if ( m_tokenIndexBefore == -1 || m_completionOperator == Sql::T_EOF_SYMBOL )
        return;

    Sql::TranslationUnit *unit = translationUnit();

    unsigned k0 = tokenKind(-2);
    unsigned k1 = tokenKind(-1);
    unsigned k2 = tokenKind();

    bool prepared = false;
    QString schemaName = "";
    QString exceptTableName = "";

    if ( k0 == Sql::T_IDENTIFIER && k1 == Sql::T_DOT && k2 == Sql::T_IDENTIFIER ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 2).identifier->chars();
        prepared = true;
    }
    else if ( k1 == Sql::T_IDENTIFIER && k2 == Sql::T_DOT ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 1).identifier->chars();
        prepared = true;
    }
    else if ( k2 == Sql::T_IDENTIFIER ) {
        addSchemas();
        exceptTableName = unit->tokenAt(m_tokenIndexBefore).identifier->chars();
        prepared = true;
    }

    if ( prepared )
        addTablesAndViews(schemaName, exceptTableName);

    if ( kwd && k2 != Sql::T_DOT )
        addKeywords();
}

void SqlCompletionAssistVisitor::completeSchemaTable( bool kwd )
{
    if ( m_tokenIndexBefore == -1 || m_completionOperator == Sql::T_EOF_SYMBOL )
        return;

    Sql::TranslationUnit *unit = translationUnit();

    unsigned k0 = tokenKind(-2);
    unsigned k1 = tokenKind(-1);
    unsigned k2 = tokenKind();

    bool prepared = false;
    QString schemaName = "";

    if ( k0 == Sql::T_IDENTIFIER && k1 == Sql::T_DOT && k2 == Sql::T_IDENTIFIER ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 2).identifier->chars();
        prepared = true;
    }
    else if ( k1 == Sql::T_IDENTIFIER && k2 == Sql::T_DOT ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 1).identifier->chars();
        prepared = true;
    }
    else if ( k2 == Sql::T_IDENTIFIER ) {
        addSchemas();
        prepared = true;
    }

    if ( prepared )
        addTables(schemaName);

    if ( kwd && k2 != Sql::T_DOT )
        addKeywords();
}

void SqlCompletionAssistVisitor::completeSchemaView( bool kwd )
{
    if ( m_tokenIndexBefore == -1 || m_completionOperator == Sql::T_EOF_SYMBOL )
        return;

    Sql::TranslationUnit *unit = translationUnit();

    unsigned k0 = tokenKind(-2);
    unsigned k1 = tokenKind(-1);
    unsigned k2 = tokenKind();

    bool prepared = false;
    QString schemaName = "";

    if ( k0 == Sql::T_IDENTIFIER && k1 == Sql::T_DOT && k2 == Sql::T_IDENTIFIER ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 2).identifier->chars();
        prepared = true;
    }
    else if ( k1 == Sql::T_IDENTIFIER && k2 == Sql::T_DOT ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 1).identifier->chars();
        prepared = true;
    }
    else if ( k2 == Sql::T_IDENTIFIER ) {
        addSchemas();
        prepared = true;
    }

    if ( prepared )
        addViews(schemaName);

    if ( kwd && k2 != Sql::T_DOT )
        addKeywords();
}

void SqlCompletionAssistVisitor::completeSchemaIndex( bool kwd )
{
    if ( m_tokenIndexBefore == -1 || m_completionOperator == Sql::T_EOF_SYMBOL )
        return;

    Sql::TranslationUnit *unit = translationUnit();

    unsigned k0 = tokenKind(-2);
    unsigned k1 = tokenKind(-1);
    unsigned k2 = tokenKind();

    bool prepared = false;
    QString schemaName = "";

    if ( k0 == Sql::T_IDENTIFIER && k1 == Sql::T_DOT && k2 == Sql::T_IDENTIFIER ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 2).identifier->chars();
        prepared = true;
    }
    else if ( k1 == Sql::T_IDENTIFIER && k2 == Sql::T_DOT ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 1).identifier->chars();
        prepared = true;
    }
    else if ( k2 == Sql::T_IDENTIFIER ) {
        addSchemas();
        prepared = true;
    }

    if ( prepared )
        addIndices(schemaName);

    if ( kwd && k2 != Sql::T_DOT )
        addKeywords();
}

void SqlCompletionAssistVisitor::completeSchemaTrigger( bool kwd )
{
    if ( m_tokenIndexBefore == -1 || m_completionOperator == Sql::T_EOF_SYMBOL )
        return;

    Sql::TranslationUnit *unit = translationUnit();

    unsigned k0 = tokenKind(-2);
    unsigned k1 = tokenKind(-1);
    unsigned k2 = tokenKind();

    bool prepared = false;
    QString schemaName = "";

    if ( k0 == Sql::T_IDENTIFIER && k1 == Sql::T_DOT && k2 == Sql::T_IDENTIFIER ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 2).identifier->chars();
        prepared = true;
    }
    else if ( k1 == Sql::T_IDENTIFIER && k2 == Sql::T_DOT ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 1).identifier->chars();
        prepared = true;
    }
    else if ( k2 == Sql::T_IDENTIFIER ) {
        addSchemas();
        prepared = true;
    }

    if ( prepared )
        addTriggers(schemaName);

    if ( kwd && k2 != Sql::T_DOT )
        addKeywords();
}

void SqlCompletionAssistVisitor::prepareArroundContext()
{
    Sql::TranslationUnit *unit = translationUnit();

    for (int i = 0, j = m_tokenIndexBefore - 4; i < 9; i++, j++) {
        if ( j > 0 ) {
            m_arroundTokenKeywords[i] = unit->tokenKeyword(j);
            m_arroundTokenKinds[i] = unit->tokenKind(j);
        }
        else {
            m_arroundTokenKeywords[i] = Sql::T_EOF_SYMBOL;
            m_arroundTokenKinds[i] = Sql::T_EOF_SYMBOL;
        }
    }
}

TableType *SchemaScope::findOrInsertTable(const QString &tableName)
{
    foreach ( TableType *table, tables )
        if ( !table->tableName.compare(tableName, Qt::CaseInsensitive) )
            return table;
    TableType *newTable = new TableType(tableName.toUpper());
    tables.append(newTable);
    return newTable;
}

TableType *FileScope::findOrInsertTable(const QString &schemaName, const QString &tableName)
{
    QString sName = ( schemaName.isEmpty() ? m_defaultSchemaName : schemaName );
    SchemaScope *schema = 0;
    if ( m_scopes.find(sName) == m_scopes.end()  ) {
        schema = new SchemaScope;
        m_scopes[sName] = schema;
    }
    else
        schema = m_scopes[sName];
    return schema->findOrInsertTable(tableName);
}

bool FileScope::findOrInsertIndex( const QString &schemaName, const QString &indexName )
{
    QString sName = ( schemaName.isEmpty() ? m_defaultSchemaName : schemaName );
    SchemaScope *schema = 0;
    if ( m_scopes.find(sName) == m_scopes.end()  ) {
        schema = new SchemaScope;
        m_scopes[sName] = schema;
    }
    else
        schema = m_scopes[sName];

    foreach ( const QString &index, schema->indexes )
        if ( !index.compare(indexName, Qt::CaseInsensitive) )
            return true;

    schema->indexes.append(indexName.toUpper());
    return false;
}

void FileScope::addTables( const QString &schemaName, const QString &exceptTableName, QList<TextEditor::BasicProposalItem *> *completions )
{
    QString sName = ( schemaName.isEmpty() ? m_defaultSchemaName : schemaName );
    if ( m_scopes.find(sName) == m_scopes.end() )
        return;
    SchemaScope *schema = m_scopes[sName];
    foreach ( TableType *table, schema->tables ) {
        if ( table->tableName.compare(exceptTableName, Qt::CaseInsensitive) ) {
            TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
            item ->setText(table->tableName);
            item->setIcon(QIcon(":/images/dbb_table.png"));
            completions->append(item);
        }
    }
}

void FileScope::addTableColumns( const QString &schemaName, const QString &tableName, QList<TextEditor::BasicProposalItem *> *completions )
{
    QString sName = ( schemaName.isEmpty() ? m_defaultSchemaName : schemaName );
    if ( m_scopes.find(sName) == m_scopes.end() )
        return;
    SchemaScope *schema = m_scopes[sName];
    foreach ( TableType *table, schema->tables ) {
        if ( !table->tableName.compare(tableName) ) {
            foreach ( const QString &name, table->columns ) {
                TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
                item ->setText(name);
                item->setIcon(QIcon(":/images/dbb_field_text.png"));
                completions->append(item);
            }

            return;
        }
    }
}

void FileScope::addIndexes( const QString &schemaName, QList<TextEditor::BasicProposalItem *> *completions )
{
    QString sName = ( schemaName.isEmpty() ? m_defaultSchemaName : schemaName );
    if ( m_scopes.find(sName) == m_scopes.end() )
        return;
    SchemaScope *schema = m_scopes[sName];
    foreach ( const QString &index, schema->indexes ) {
        TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
        item ->setText(index);
        item->setIcon(QIcon(":/images/dbb_index.png"));
        completions->append(item);
    }
}

}   // namespace QLalrOracle
