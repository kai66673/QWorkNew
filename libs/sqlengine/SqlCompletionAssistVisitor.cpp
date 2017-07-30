#include "SqlCompletionAssistVisitor.h"
#include "DbMetadataProvider.h"
#include "SqlLexer.h"

namespace Sql {

SqlCompletionAssistVisitor::SqlCompletionAssistVisitor( TranslationUnit *unit )
    : ASTVisitor(unit)
    , m_ast(0)
    , m_creationStorage(new SqlCreationStorage())
{
}

SqlCompletionAssistVisitor::~SqlCompletionAssistVisitor()
{
    delete m_creationStorage;
}

int SqlCompletionAssistVisitor::fillCompletions( QList<TextEditor::BasicProposalItem *> *completions, TranslationUnitAST *ast,
                                                 unsigned position, int startPosition, unsigned completionOperator )
{
    if ( !ast )
        return -1;

    SqlCreateScopeVisitor binder(translationUnit(), m_creationStorage);
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

    if ( m_completionOperator == T_COLON ) {
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
                    if ( m_completions->isEmpty() && m_completionOperator == T_EOF_SYMBOL )
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
    return DbMetadataProvider::getInstance()->checkSchemaName(schemaName);
}

void SqlCompletionAssistVisitor::addKeywords()
{
    if ( m_completionOperator == T_IDENTIFIER )
        Token::populateKeywords(*m_completions);
}

void SqlCompletionAssistVisitor::addSchemas()
{
    DbMetadataProvider::getInstance()->fillSchemas(*m_completions);
}

void SqlCompletionAssistVisitor::addTablesAndViews( const QString &schemaName )
{
    DbMetadataProvider *provider = DbMetadataProvider::getInstance();
    m_creationStorage->addTables(m_completions);
    provider->fillSchemaObjects(schemaName, *m_completions, Sql::Constants::DbSchemaObjectType::C_TABLE);
    provider->fillSchemaObjects(schemaName, *m_completions, Sql::Constants::DbSchemaObjectType::C_VIEW);
}

void SqlCompletionAssistVisitor::addTables( const QString &schemaName )
{
    m_creationStorage->addTables(m_completions);
    DbMetadataProvider::getInstance()->fillSchemaObjects(schemaName, *m_completions, Sql::Constants::DbSchemaObjectType::C_TABLE);
}

void SqlCompletionAssistVisitor::addViews( const QString &schemaName )
{
    DbMetadataProvider::getInstance()->fillSchemaObjects(schemaName, *m_completions, Sql::Constants::DbSchemaObjectType::C_VIEW);
}

void SqlCompletionAssistVisitor::addIndices( const QString &schemaName )
{
    m_creationStorage->addIndexes(m_completions);
    DbMetadataProvider::getInstance()->fillSchemaObjects(schemaName, *m_completions, Sql::Constants::DbSchemaObjectType::C_INDEX);
}

void SqlCompletionAssistVisitor::addTriggers( const QString &schemaName )
{
    DbMetadataProvider::getInstance()->fillSchemaObjects(schemaName, *m_completions, Sql::Constants::DbSchemaObjectType::C_TRIGGER);
}

void SqlCompletionAssistVisitor::addSchemaTableFields( const QString &schemaName, const QString &tableName )
{
    m_creationStorage->addTableColumns(tableName, m_completions);
    DbMetadataProvider::getInstance()->fillTableColumns(schemaName, tableName, *m_completions);
}

void SqlCompletionAssistVisitor::addSchemaViewFields( const QString &schemaName, const QString &viewName )
{
    DbMetadataProvider::getInstance()->fillViewColumns(schemaName, viewName, *m_completions);
}

void SqlCompletionAssistVisitor::addConstraints( const QString &schemaName, const QString &tableName )
{
    m_creationStorage->addTableConstraints(tableName, m_completions);
    DbMetadataProvider::getInstance()->fillTableConstraints(schemaName, tableName, *m_completions);
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
    if ( m_completionOperator == T_EOF_SYMBOL ) {

        qDebug() << "SqlCompletionAssistVisitor::completeSimpleSequences" << m_tokenIndexBefore << firstToken;

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
        accept(ast->select_core_list);
        accept(ast->order_by_block);
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

bool SqlCompletionAssistVisitor::visit( SchemaTableNameAST *ast )
{
    if ( ast ) {
        QString schemaName = (ast->schemaName ? ast->schemaName->name->chars() : "");
        QString tableName  = (ast->tableName  ? ast->tableName->name->chars() : "" );
        if ( !tableName.isEmpty() )
            m_tableAliasesToNames[tableName] = qMakePair(tableName, schemaName);
    }
    return false;
}

bool SqlCompletionAssistVisitor::visit( TableAliasAST *ast )
{
    if ( ast ) {
        if ( ast->schemaTableName ) {
            QString schemaName = (ast->schemaTableName->schemaName ? ast->schemaTableName->schemaName->name->chars() : "");
            QString tableName  = (ast->schemaTableName->tableName  ? ast->schemaTableName->tableName->name->chars() : "" );
            QString aliasName  = ast->name->chars();
            if ( !tableName.isEmpty() && !aliasName.isEmpty() )
                m_tableAliasesToNames[aliasName] = qMakePair(tableName, schemaName);
        }
    }
    return false;
}

void SqlCompletionAssistVisitor::complete( SelectStatementAST *ast )
{
    if ( LimitToBlockAST *limitToBlockAst = ast->limit_to_block ) {
        if ( m_tokenIndexBefore >= limitToBlockAst->firstToken()) {
            if ( m_tokenIndexBefore == limitToBlockAst->firstToken() ) {
                if ( !limitToBlockAst->to_token )
                    addSimpleKeyword("TO");
                return;
            }
            if ( !limitToBlockAst->row_token ) {
                addSimpleKeyword("ROW");
                addSimpleKeyword("ROWS");
            }
            return;
        }
    }

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

    for (SelectCompoundListAST *iter = ast->select_compound_list; iter; iter = iter->next) {
        SelectCompoundAST *selectCompoundItem = iter->value;
        if ( selectCompoundItem ) {
            if ( m_tokenIndexBefore >= selectCompoundItem->firstToken() && m_tokenIndexBefore <= selectCompoundItem->lastToken() ) {
                addSimpleKeyword("SELECT");
                if ( selectCompoundItem->firstToken() != selectCompoundItem->lastToken() && m_tokenIndexBefore == selectCompoundItem->firstToken() ) {
                    addSimpleKeyword("ALL");
                    addSimpleKeyword("DISTINCT");
                }
                return;
            }
        }
    }

    for ( SelectCoreListAST *iter = ast->select_core_list; iter; iter = iter->next)
        if ( complete(iter->value, iter->next) )
            return;

    if ( translationUnit()->tokenAt(m_tokenIndexBefore).kind() != T_IDENTIFIER )
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

    if ( ast->where_token && m_tokenIndexBefore >= ast->where_token ) {
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
        accept(ast->upd_list);
        accept(ast->whereExpr);
        complete(ast);
    }

    return false;
}

bool SqlCompletionAssistVisitor::complete( UpdateStatementAST *ast )
{
    if ( ast->set_token && m_tokenIndexBefore >= ast->set_token)
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
        accept(ast->columns_list);
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
        accept(ast->whereExpr);
        complete(ast);
    }
    return false;
}

bool SqlCompletionAssistVisitor::complete( DeleteStatementAST *ast )
{
    if ( ast->where_token && m_tokenIndexBefore >= ast->where_token )
        completeSchemaTableViewField();
    else
        completeSchemaTableView();
    return true;
}

bool SqlCompletionAssistVisitor::visit( CreateTableStatementAST *ast )
{
    if ( ast ) {
        accept(ast->schemaTableName);
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
                                    if ( refClause->refSchemaTableName ) {
                                        if ( refClause->refSchemaTableName->tableName )
                                            refTableName = refClause->refSchemaTableName->tableName->name->chars();
                                        if ( refClause->refSchemaTableName->schemaName )
                                            refSchemaName = refClause->refSchemaTableName->schemaName->name->chars();
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
        accept(ast->schemaViewName);
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
        accept(ast->schemaIndexName);
        accept(ast->schemaTableName);
        accept(ast->columns_list);
        complete(ast);
    }
    return false;
}

bool SqlCompletionAssistVisitor::complete( CreateIndexStatementAST *ast )
{
    if ( ast->lparen_token && m_tokenIndexBefore >= ast->lparen_token ) {
        completeSchemaTableViewField();
        return true;
    }

    if ( ast->schemaTableName && m_tokenIndexBefore >= ast->schemaTableName->firstToken() ) {
        completeSchemaTableViewField();
        return true;
    }

    if ( ast->schemaIndexName && m_tokenIndexBefore >= ast->schemaIndexName->firstToken() ) {
        completeSchemaIndex();
        return true;
    }

    addKeywords();
    return true;
}

bool SqlCompletionAssistVisitor::visit( CreateTriggerStatementAST *ast )
{
    if ( ast ) {
        accept(ast->schemaTableName);
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
                                        if ( m_tokenIndexBefore == statement->lastToken() && tokenKind() == T_COMMA ) {
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
                            if ( tokenKind() == T_COMMA || tokenKind() == T_LPAREN ||
                                 (tokenKind() == T_IDENTIFIER && (tokenKind(-1) == T_COMMA || tokenKind(-1) == T_LPAREN)) ) {
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
        if ( m_completionOperator == T_EOF_SYMBOL ) {
            if ( tokenKeyword() == T_2_AS ) {
                return true;
            }
            if ( tokenKeyword() == T_3_NEW || tokenKeyword() == T_3_OLD ) {
                if ( tokenKeyword(1) != T_2_AS ) {
                    addSimpleKeyword("AS");
                }
                return true;
            }
            if ( tokenKeyword() == T_11_REFERENCING || tokenKind() == T_IDENTIFIER ) {
                if ( tokenKeyword(1) != T_3_NEW && tokenKeyword(1) != T_3_OLD ) {
                    addSimpleKeyword("NEW");
                    addSimpleKeyword("OLD");
                    if ( tokenKind() == T_IDENTIFIER && tokenKeyword(1) != T_4_WHEN )
                        addSimpleKeyword("WHEN");
                }
                return true;
            }
        }
        if ( m_completionOperator == T_IDENTIFIER ) {
            addSimpleKeyword("AS");
            addSimpleKeyword("NEW");
            addSimpleKeyword("OLD");
            addSimpleKeyword("WHEN");
        }
        return true;
    }

    if ( ast->schemaTableName && ( m_tokenIndexBefore >= ast->schemaTableName->lastToken() ||
                                  (m_completionOperator == T_EOF_SYMBOL && m_tokenIndexBefore >= ast->schemaTableName->lastToken() - 1) ) ) {
        if ( !ast->triggerRefClause )
            addSimpleKeyword("REFERENCING");
        return true;
    }

    if ( ast->on_token && m_tokenIndexBefore >= ast->on_token ) {
        completeSchemaTable(false);
        return true;
    }

    if ( ast->of_token && m_tokenIndexBefore >= ast->of_token ) {
        if ( ast->schemaTableName ) {
            QString tableName = ast->schemaTableName->tableName->name->chars();
            if ( !tableName.isEmpty() ) {
                QString schemaName = "";
                if ( ast->schemaTableName->schemaName )
                    schemaName = ast->schemaTableName->schemaName->name->chars();
                DbMetadataProvider::getInstance()->fillTableColumns(schemaName, tableName, *m_completions);
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

    if ( ast->schemaTriggerName && ( m_tokenIndexBefore >= ast->schemaTriggerName->lastToken() ||
                                     (m_completionOperator == T_EOF_SYMBOL && m_tokenIndexBefore >= ast->schemaTriggerName->lastToken() - 1) ) ) {
        if ( m_tokenIndexBefore == ast->schemaTriggerName->lastToken() && m_completionOperator == T_DOT ) {
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
        accept(ast->schemaTableName);
        complete(ast);
    }
    return false;
}

bool SqlCompletionAssistVisitor::complete( DropTableStatementAST *ast )
{
    if ( ast->schemaTableName && m_tokenIndexBefore >= ast->schemaTableName->firstToken() ) {
        completeSchemaTable();
        return true;
    }

    addKeywords();
    return true;
}

bool SqlCompletionAssistVisitor::visit( DropViewStatementAST *ast )
{
    if ( ast ) {
        accept(ast->schemaViewName);
        complete(ast);
    }
    return false;
}

bool SqlCompletionAssistVisitor::complete( DropViewStatementAST *ast )
{
    if ( ast->schemaViewName && m_tokenIndexBefore >= ast->schemaViewName->firstToken() ) {
        completeSchemaView();
        return true;
    }

    addKeywords();
    return true;
}

bool SqlCompletionAssistVisitor::visit( DropIndexStatementAST *ast )
{
    if ( ast ) {
        accept(ast->schemaIndexName);
        complete(ast);
    }
    return false;
}

bool SqlCompletionAssistVisitor::complete( DropIndexStatementAST *ast )
{
    if ( ast->schemaIndexName && m_tokenIndexBefore >= ast->schemaIndexName->firstToken() ) {
        completeSchemaIndex();
        return true;
    }

    addKeywords();
    return true;
}

bool SqlCompletionAssistVisitor::visit( DropTriggerStatementAST *ast )
{
    if ( ast ) {
        accept(ast->schemaTriggerName);
        complete(ast);
    }
    return false;
}

bool SqlCompletionAssistVisitor::complete( DropTriggerStatementAST *ast )
{
    if ( ast->schemaTriggerName && m_tokenIndexBefore >= ast->schemaTriggerName->firstToken() ) {
        completeSchemaTrigger();
        return true;
    }

    addKeywords();
    return true;
}

bool SqlCompletionAssistVisitor::visit( TruncateStatementAST *ast )
{
    if ( ast ) {
        accept(ast->schemaTableName);
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
        accept(ast->schemaTableName);
        complete(ast);
    }
    return false;
}

bool SqlCompletionAssistVisitor::complete( CommentOnTableStatementAST *ast )
{
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
        accept(ast->schemaTableName);
        complete(ast);
    }
    return false;
}

bool SqlCompletionAssistVisitor::complete( CommentOnColumnStatementAST *ast )
{
    if ( ast->column_token && m_tokenIndexBefore >= ast->column_token ) {
        completeSchemaTableViewField();
        return true;
    }

    addKeywords();
    return true;
}

bool SqlCompletionAssistVisitor::visit( AlterTableStatementAST *ast )
{
    if ( ast ) {
        accept(ast->schemaTableName);
        accept(ast->alterTableClause);
        complete(ast);
    }
    return false;
}

bool SqlCompletionAssistVisitor::complete( AlterTableStatementAST *ast )
{
    QString tableName = "";
    QString schemaName = "";

    if ( ast->schemaTableName ) {
        if ( ast->schemaTableName->tableName )
            tableName = ast->schemaTableName->tableName->name->chars();
        if ( ast->schemaTableName->schemaName )
            schemaName = ast->schemaTableName->schemaName->name->chars();
    }

    if ( m_completionOperator == T_EOF_SYMBOL ) {
        if ( ast->schemaTableName ) {
            if ( m_tokenIndexBefore == ast->schemaTableName->lastToken() - 1 && !ast->alterTableClause ) {
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
                 translationUnit()->tokenAt(m_tokenIndexBefore).kind() == T_IDENTIFIER && m_completionOperator != T_EOF_SYMBOL ) {
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
                 ( m_tokenIndexBefore == addColAst->column_token + 1 && translationUnit()->tokenAt(m_tokenIndexBefore).kind() == T_IDENTIFIER &&
                   m_completionOperator != T_EOF_SYMBOL ) ) {
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

#define CHECK_IND_PREV_FOR_IDENT(index) ( m_tokenIndexBefore == (index) && m_completionOperator == T_EOF_SYMBOL ) || ( m_tokenIndexBefore == (index) + 1 && m_completionOperator == T_IDENTIFIER )
#define CHECK_IND_PREV_FOR_IDENT1(index) ( m_tokenIndexBefore == (index) ) || ( m_tokenIndexBefore == (index) - 1 && m_completionOperator == T_IDENTIFIER )
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
                    if ( refClause->refSchemaTableName ) {
                        if ( refClause->refSchemaTableName->tableName )
                            refTableName = refClause->refSchemaTableName->tableName->name->chars();
                        if ( refClause->refSchemaTableName->schemaName )
                            refSchemaName = refClause->refSchemaTableName->schemaName->name->chars();
                    }
                    if ( !refTableName.isEmpty() )
                        addSchemaTableFields(refSchemaName, refTableName);
                }
                else if ( fkAst->refClause->references_token && m_tokenIndexBefore >= fkAst->refClause->references_token ) {
                    completeSchemaTable();
                }
            }
            else if ( fkAst->rparen_token && m_tokenIndexBefore >= fkAst->rparen_token ) {
                if ( ( m_tokenIndexBefore == fkAst->rparen_token && m_completionOperator == T_EOF_SYMBOL ) ||
                     (  m_tokenIndexBefore == fkAst->rparen_token + 1 && m_completionOperator == T_IDENTIFIER ) )
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

void SqlCompletionAssistVisitor::completeSchemaTableViewField( bool kwd )
{
    if ( m_tokenIndexBefore == -1 || m_completionOperator == T_EOF_SYMBOL )
        return;

    TranslationUnit *unit = translationUnit();

    unsigned k0 = tokenKind(-4);
    unsigned k1 = tokenKind(-3);
    unsigned k2 = tokenKind(-2);
    unsigned k3 = tokenKind(-1);
    unsigned k4 = tokenKind();

    QString schemaName;
    QString tableName = "";

    if ( k0 == T_IDENTIFIER && k1 == T_DOT && k2 == T_IDENTIFIER && k3 == T_DOT && k4 == T_IDENTIFIER ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 4).identifier->chars();
        tableName  = unit->tokenAt(m_tokenIndexBefore - 2).identifier->chars();
        addSchemaTableFields(schemaName, tableName);
        addSchemaViewFields(schemaName,tableName);
        return;
    }

    else if ( k1 == T_IDENTIFIER && k2 == T_DOT && k3 == T_IDENTIFIER && k4 == T_DOT ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 3).identifier->chars();
        tableName  = unit->tokenAt(m_tokenIndexBefore - 1).identifier->chars();
        addSchemaTableFields(schemaName, tableName);
        addSchemaViewFields(schemaName, tableName);
        return;
    }

    else if ( k2 == T_IDENTIFIER && k3 == T_DOT && k4 == T_IDENTIFIER ) {
        tableName = unit->tokenAt(m_tokenIndexBefore - 2 ).identifier->chars();
    }

    else if ( k3 == T_IDENTIFIER && k4 == T_DOT ) {
        tableName = unit->tokenAt(m_tokenIndexBefore - 1 ).identifier->chars();
    }

    else if ( k4 == T_IDENTIFIER ) {
        tableName = "";
    }

    else {
        if ( kwd && k4 != T_IDENTIFIER )
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
    if ( m_tokenIndexBefore == -1 || m_completionOperator == T_EOF_SYMBOL )
        return;

    TranslationUnit *unit = translationUnit();

    unsigned k0 = tokenKind(-2);
    unsigned k1 = tokenKind(-1);
    unsigned k2 = tokenKind();

    bool prepared = false;
    QString schemaName = "";

    if ( k0 == T_IDENTIFIER && k1 == T_DOT && k2 == T_IDENTIFIER ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 2).identifier->chars();
        prepared = true;
    }
    else if ( k1 == T_IDENTIFIER && k2 == T_DOT ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 1).identifier->chars();
        prepared = true;
    }
    else if ( k2 == T_IDENTIFIER ) {
        addSchemas();
        prepared = true;
    }

    if ( prepared )
        addTablesAndViews(schemaName);

    if ( kwd && k2 != T_DOT )
        addKeywords();
}

void SqlCompletionAssistVisitor::completeSchemaTable( bool kwd )
{
    if ( m_tokenIndexBefore == -1 || m_completionOperator == T_EOF_SYMBOL )
        return;

    TranslationUnit *unit = translationUnit();

    unsigned k0 = tokenKind(-2);
    unsigned k1 = tokenKind(-1);
    unsigned k2 = tokenKind();

    bool prepared = false;
    QString schemaName = "";

    if ( k0 == T_IDENTIFIER && k1 == T_DOT && k2 == T_IDENTIFIER ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 2).identifier->chars();
        prepared = true;
    }
    else if ( k1 == T_IDENTIFIER && k2 == T_DOT ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 1).identifier->chars();
        prepared = true;
    }
    else if ( k2 == T_IDENTIFIER ) {
        addSchemas();
        prepared = true;
    }

    if ( prepared )
        addTables(schemaName);

    if ( kwd && k2 != T_DOT )
        addKeywords();
}

void SqlCompletionAssistVisitor::completeSchemaView( bool kwd )
{
    if ( m_tokenIndexBefore == -1 || m_completionOperator == T_EOF_SYMBOL )
        return;

    TranslationUnit *unit = translationUnit();

    unsigned k0 = tokenKind(-2);
    unsigned k1 = tokenKind(-1);
    unsigned k2 = tokenKind();

    bool prepared = false;
    QString schemaName = "";

    if ( k0 == T_IDENTIFIER && k1 == T_DOT && k2 == T_IDENTIFIER ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 2).identifier->chars();
        prepared = true;
    }
    else if ( k1 == T_IDENTIFIER && k2 == T_DOT ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 1).identifier->chars();
        prepared = true;
    }
    else if ( k2 == T_IDENTIFIER ) {
        addSchemas();
        prepared = true;
    }

    if ( prepared )
        addViews(schemaName);

    if ( kwd && k2 != T_DOT )
        addKeywords();
}

void SqlCompletionAssistVisitor::completeSchemaIndex( bool kwd )
{
    if ( m_tokenIndexBefore == -1 || m_completionOperator == T_EOF_SYMBOL )
        return;

    TranslationUnit *unit = translationUnit();

    unsigned k0 = tokenKind(-2);
    unsigned k1 = tokenKind(-1);
    unsigned k2 = tokenKind();

    bool prepared = false;
    QString schemaName = "";

    if ( k0 == T_IDENTIFIER && k1 == T_DOT && k2 == T_IDENTIFIER ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 2).identifier->chars();
        prepared = true;
    }
    else if ( k1 == T_IDENTIFIER && k2 == T_DOT ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 1).identifier->chars();
        prepared = true;
    }
    else if ( k2 == T_IDENTIFIER ) {
        addSchemas();
        prepared = true;
    }

    if ( prepared )
        addIndices(schemaName);

    if ( kwd && k2 != T_DOT )
        addKeywords();
}

void SqlCompletionAssistVisitor::completeSchemaTrigger( bool kwd )
{
    if ( m_tokenIndexBefore == -1 || m_completionOperator == T_EOF_SYMBOL )
        return;

    TranslationUnit *unit = translationUnit();

    unsigned k0 = tokenKind(-2);
    unsigned k1 = tokenKind(-1);
    unsigned k2 = tokenKind();

    bool prepared = false;
    QString schemaName = "";

    if ( k0 == T_IDENTIFIER && k1 == T_DOT && k2 == T_IDENTIFIER ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 2).identifier->chars();
        prepared = true;
    }
    else if ( k1 == T_IDENTIFIER && k2 == T_DOT ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 1).identifier->chars();
        prepared = true;
    }
    else if ( k2 == T_IDENTIFIER ) {
        addSchemas();
        prepared = true;
    }

    if ( prepared )
        addTriggers(schemaName);

    if ( kwd && k2 != T_DOT )
        addKeywords();
}

void SqlCompletionAssistVisitor::prepareArroundContext()
{
    TranslationUnit *unit = translationUnit();

    for (int i = 0, j = m_tokenIndexBefore - 4; i < 9; i++, j++) {
        if ( j > 0 ) {
            m_arroundTokenKeywords[i] = unit->tokenKeyword(j);
            m_arroundTokenKinds[i] = unit->tokenKind(j);
        }
        else {
            m_arroundTokenKeywords[i] = T_EOF_SYMBOL;
            m_arroundTokenKinds[i] = T_EOF_SYMBOL;
        }
    }
}

}   // namespace Sql
