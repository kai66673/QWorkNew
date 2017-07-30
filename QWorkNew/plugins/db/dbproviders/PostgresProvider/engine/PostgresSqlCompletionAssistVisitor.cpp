#include "PostgresSqlCompletionAssistVisitor.h"
#include "DbMetadataProvider.h"
#include "SqlLexer.h"
#include "postgres_sql_constants.h"
#include "PostgresParser.tab.hh"

namespace Postgres {

static TableIdentifierAST *relationIdentifier( RelationExprAST *rel )
{
    if ( RelationExprSimpleAST *rel1 = rel->asRelationExprSimple() )
        return rel1->tableIdent;
    if ( RelationExprOnlyAST *rel2 = rel->asRelationExprOnly() )
        return rel2->tableIdent;
    return 0;
}

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

    indexes.append(index.toUpper());
    schema->indexes.append(index.toUpper());
    return false;
}

bool TableType::findOrInsertTrigger( const QString &trigger )
{
    foreach ( const QString &triggerName, triggers )
        if ( !triggerName.compare(trigger, Qt::CaseInsensitive) )
            return true;

    triggers.append(trigger.toUpper());
    schema->triggers.append(trigger.toUpper());
    return false;
}

bool TableType::findOrInsertConstraint( const QString &constraint )
{
    qDebug() << "?? TableType::findOrInsertConstraint" << constraint;
    foreach ( const QString &constraintName, constraints )
        if ( !constraintName.compare(constraint, Qt::CaseInsensitive) )
            return true;

    constraints.append(constraint.toUpper());
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
        for ( StatementListAST *iter = ast->statements; iter; iter = iter->next ) {
            m_currentTableType = 0;
            if ( CreateStatementAST *createStatement = iter->value->asCreateStatement() ) {
                createStatement->accept(this);
            } else if ( AlterTableStmtAST *alterTableStatement = iter->value->asAlterTableStmt() ) {
                if ( alterTableStatement->relation ) {
                    TableIdentifierAST *tableIdent = relationIdentifier(alterTableStatement->relation);
                    if ( tableIdent && tableIdent->tableName ) {
                        QString tableName = QString(tableIdent->tableName->name->chars());
                        QString schemaName = "";
                        if ( tableIdent->schemaName )
                            schemaName = QString(tableIdent->schemaName->name->chars());
                        m_currentTableType = m_globalScope->findOrInsertTable(schemaName, tableName);
                        if ( m_currentTableType ) {
                            for ( AlterTableCmdListAST *command = alterTableStatement->alterCommands; command; command = command->next ) {
                                if ( AlterTableAddConstrCmdAST *addConstraint = command->value->asAlterTableAddConstrCmd() ) {
                                    if ( addConstraint->constraint && addConstraint->constraint->constraintName )
                                        m_currentTableType->findOrInsertConstraint(addConstraint->constraint->constraintName->name->chars());
                                } else if ( AlterTableAddColCmdAST *addColumn = command->value->asAlterTableAddColCmd() ) {
                                    if ( addColumn->colDef && addColumn->colDef->columnName )
                                        m_currentTableType->findOrInsertColumn(addColumn->colDef->columnName->name->chars());
                                }
                            }
                        }
                    }
                }
            }
        }
    }

protected:
    bool visit( CreateTrigStmtAST *ast ) {
        if ( ast ) {
            if ( ast->triggerName && ast->tableIdent && ast->tableIdent->tableName ) {
                QString tableName = QString(ast->tableIdent->tableName->name->chars());
                QString schemaName = "";
                if ( ast->tableIdent->schemaName )
                    schemaName = QString(ast->tableIdent->schemaName->name->chars());
                m_currentTableType = m_globalScope->findOrInsertTable(schemaName, tableName);
                if ( m_currentTableType )
                    m_currentTableType->findOrInsertTrigger(ast->triggerName->name->chars());
            }
        }
        return false;
    }

    bool visit( CreateIndexStatementAST *ast ) {
        if ( ast ) {
            if ( ast->indexName && ast->tableIdent && ast->tableIdent->tableName ) {
                QString tableName = QString(ast->tableIdent->tableName->name->chars());
                QString schemaName = "";
                if ( ast->tableIdent->schemaName )
                    schemaName = QString(ast->tableIdent->schemaName->name->chars());
                m_currentTableType = m_globalScope->findOrInsertTable(schemaName, tableName);
                if ( m_currentTableType )
                    m_currentTableType->findOrInsertIndex(ast->indexName->name->chars());
            }
        }
        return false;
    }

    bool visit( CreateTableStmtAST * ast ) {
        if ( ast ) {
            if ( ast->tableIdent && ast->tableIdent->tableName ) {
                QString tableName = QString(ast->tableIdent->tableName->name->chars());
                QString schemaName = "";
                if ( ast->tableIdent->schemaName )
                    schemaName = QString(ast->tableIdent->schemaName->name->chars());
                m_currentTableType = m_globalScope->findOrInsertTable(schemaName, tableName, true);
                if ( m_currentTableType )
                    accept(ast->createClause);
            }
        }
        return false;
    }

    bool visit( ColumnDefAST *ast ) {
        if ( m_currentTableType && ast && ast->columnName ) {
            m_currentTableType->findOrInsertColumn(ast->columnName->name->chars());
        }
        return false;
    }

private:
    FileScope *m_globalScope;
    TableType *m_currentTableType;
};

SqlCompletionAssistVisitor::SqlCompletionAssistVisitor( Sql::TranslationUnit *unit )
    : ASTNoVisitor(unit)
    , m_ast(0)
    , m_currentStatement(0)
    , m_completed(false)
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
        for ( StatementListAST *iter= m_ast->statements; iter; iter = iter->next ) {
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
        for ( StatementListAST *iter = m_ast->statements; iter; iter = iter->next ) {
            StatementAST *statement = iter->value;
            if ( !statement )
                continue;
            if ( m_tokenIndexBefore >= statement->firstToken() && m_tokenIndexBefore <= statement->lastToken() ) {
                m_currentStatement = statement;
                accept(statement);
                if ( !m_completed ) {
                    completeCommon(statement);
                    if ( !m_completed ) {
                        completeSchemaTableViewField();
                        if ( /*m_completions->isEmpty()*/!m_completed && m_completionOperator == Sql::T_EOF_SYMBOL )
                            completeSimpleSequences(statement->firstToken());
                    }
                }
                break;
            }
        }
    }


    return m_startPosition;
}

void SqlCompletionAssistVisitor::completeCommon( StatementAST *statement )
{
    if ( statement->helpers ) {
        for (int i = statement->helpers->size() - 1; i >= 0; i-- ) {
            StatementAST::CompletionHelper *helper = statement->helpers->at(i);
            if ( helper ) {
                qDebug() << "^^SqlCompletionAssistVisitor::completeCommon" << m_tokenIndexBefore << helper->start_token << helper->type;
                if ( m_tokenIndexBefore >= helper->start_token ) {
                    switch ( helper->type ) {
                        case StatementAST::CompletionHelper::Keywords:
                            if ( m_completionOperator == Sql::T_EOF_SYMBOL ) {
                                QStringList variants = helper->keywords.split(';');
                                for (int j = 0; j < variants.size(); j++) {
                                    if ( variants[j].contains(' ') )
                                        addSimpleSequence(variants[j]);
                                    else
                                        addSimpleKeyword(variants[j]);
                                    m_completed = true;
                                }
                            }
                            break;
                        case StatementAST::CompletionHelper::TableIdentifier:
                            if ( m_completionOperator != Sql::T_EOF_SYMBOL ) {
                                completeObjectIdentifier(Sql::Constants::DbSchemaObjectType::C_TABLE, helper->start_token);
                                m_completed = true;
                            }
                            break;
                        case StatementAST::CompletionHelper::ViewIdentifier:
                            if ( m_completionOperator != Sql::T_EOF_SYMBOL ) {
                                completeObjectIdentifier(Sql::Constants::DbSchemaObjectType::C_VIEW, helper->start_token);
                                m_completed = true;
                            }
                            break;
                        case StatementAST::CompletionHelper::IndexIdentifier:
                            if ( m_completionOperator != Sql::T_EOF_SYMBOL ) {
                                completeObjectIdentifier(Sql::Constants::DbSchemaObjectType::C_INDEX, helper->start_token);
                                m_completed = true;
                            }
                            break;
                        case StatementAST::CompletionHelper::SequenceIdentifier:
                            if ( m_completionOperator != Sql::T_EOF_SYMBOL ) {
                                completeObjectIdentifier(Sql::Constants::DbSchemaObjectType::C_SEQUENCE, helper->start_token);
                                m_completed = true;
                            }
                            break;
                        case StatementAST::CompletionHelper::ColumnName:
                            if ( m_tokenIndexBefore <= helper->end_token && m_completionOperator != Sql::T_EOF_SYMBOL ) {
                                QString schemaName = "";
                                if ( helper->objectName ) {
                                    if ( helper->schemaName && helper->schemaName->name )
                                        schemaName = helper->schemaName->name->chars();
                                    addSchemaTableFields(schemaName, helper->objectName->name->chars());
                                } else if ( statement->info->objectName ) {
                                    if ( statement->info->schemaName && statement->info->schemaName->name )
                                        schemaName = statement->info->schemaName->name->chars();
                                    addSchemaTableFields(schemaName, statement->info->objectName->name->chars());
                                }
                                m_completed = true;
                            }
                            break;
                        case StatementAST::CompletionHelper::ConstraintName:
                            if ( m_tokenIndexBefore == helper->start_token && m_completionOperator != Sql::T_EOF_SYMBOL ) {
                                QString schemaName = "";
                                if ( helper->objectName ) {
                                    if ( helper->schemaName && helper->schemaName->name )
                                        schemaName = helper->schemaName->name->chars();
                                    addConstraints(schemaName, helper->objectName->name->chars());
                                } else if ( statement->info->objectName ) {
                                    if ( statement->info->schemaName && statement->info->schemaName->name )
                                        schemaName = statement->info->schemaName->name->chars();
                                    addConstraints(schemaName, statement->info->objectName->name->chars());
                                }
                                m_completed = true;
                            }
                            break;
                        case StatementAST::CompletionHelper::TriggerName:
                            if ( m_tokenIndexBefore == helper->start_token && m_completionOperator != Sql::T_EOF_SYMBOL ) {
                                QString schemaName = "";
                                if ( helper->objectName ) {
                                    if ( helper->schemaName && helper->schemaName->name )
                                        schemaName = helper->schemaName->name->chars();
                                    addSchemaTableTriggers(schemaName, helper->objectName->name->chars());
                                } else if ( statement->info->objectName ) {
                                    if ( statement->info->schemaName && statement->info->schemaName->name )
                                        schemaName = statement->info->schemaName->name->chars();
                                    addSchemaTableTriggers(schemaName, statement->info->objectName->name->chars());
                                }
                                m_completed = true;
                            }
                            break;
                        case StatementAST::CompletionHelper::IndexName:
                            if ( m_tokenIndexBefore == helper->start_token && m_completionOperator != Sql::T_EOF_SYMBOL ) {
                                QString schemaName = "";
                                if ( helper->objectName ) {
                                    if ( helper->schemaName && helper->schemaName->name )
                                        schemaName = helper->schemaName->name->chars();
                                    addSchemaTableIndexes(schemaName, helper->objectName->name->chars());
                                } else if ( statement->info->objectName ) {
                                    if ( statement->info->schemaName && statement->info->schemaName->name )
                                        schemaName = statement->info->schemaName->name->chars();
                                    addSchemaTableIndexes(schemaName, statement->info->objectName->name->chars());
                                }
                                m_completed = true;
                            }
                            break;
                    }
                }
            }
        }
    }
    if ( !m_completed ) {
        if ( StatementAST::Info *info = statement->info ) {
            switch ( info->type ) {
                case StatementAST::Info::Alter:
                case StatementAST::Info::Comment:
                case StatementAST::Info::Drop: {
                    m_completed = true;
                    completeTargetIdentifier(info->target);
                    break;
                }
            }
        }
    }
}

bool SqlCompletionAssistVisitor::visit( DropTableStmtAST *ast )
{
    if ( ast ) {
        if ( ast->tables && m_tokenIndexBefore >= ast->tables->lastToken() - 1 &&
             m_completionOperator == Sql::T_EOF_SYMBOL && !ast->t_behavior ) {
            addSimpleKeyword("CASCADE");
            addSimpleKeyword("RESTRICT");
            m_completed = true;
        }
    }

    return false;
}

bool SqlCompletionAssistVisitor::visit( DropViewStmtAST *ast )
{
    if ( ast ) {
        if ( ast->tables && m_tokenIndexBefore >= ast->tables->lastToken() - 1 &&
             m_completionOperator == Sql::T_EOF_SYMBOL && !ast->t_behavior ) {
            addSimpleKeyword("CASCADE");
            addSimpleKeyword("RESTRICT");
            m_completed = true;
        }
    }

    return false;
}

bool SqlCompletionAssistVisitor::visit( DropTrigStmtAST *ast )
{
    if ( ast ) {
        if ( ast->tableIdent && m_tokenIndexBefore >= ast->tableIdent->lastToken() - 1 &&
             m_completionOperator == Sql::T_EOF_SYMBOL && !ast->t_behavior ) {
            addSimpleKeyword("CASCADE");
            addSimpleKeyword("RESTRICT");
            m_completed = true;
        }

        else if ( ast->triggerName && m_tokenIndexBefore >= ast->triggerName->lastToken() - 1 &&
             m_completionOperator == Sql::T_EOF_SYMBOL && !ast->t_on ) {
            qDebug()<< "SqlCompletionAssistVisitor::visit( DropTrigStmtAST *ast ) 01";
            Sql::DbMetadataProvider::getInstance()->fillTriggerSchemaTable(ast->triggerName->name->chars(), *m_completions);
            m_completed = true;
        }

        else if ( ast->triggerName && m_tokenIndexBefore == ast->triggerName->lastToken() - 1 &&
                  m_completionOperator != Sql::T_EOF_SYMBOL && !ast->t_on ) {
            qDebug()<< "SqlCompletionAssistVisitor::visit( DropTrigStmtAST *ast ) 02";
            addAllSchamasTriggers();
            m_completed = true;
        }

    }

    return false;
}

bool SqlCompletionAssistVisitor::visit( CommonCommentStmtAST *ast )
{
    if ( ast ) {
        m_completed = true;

        if ( ast->obj && m_tokenIndexBefore >= ast->obj->lastToken() - 1 &&
             m_completionOperator == Sql::T_EOF_SYMBOL && !ast->commentText ) {
            addSimpleSequence("IS ''");
        }

        else if ( ast->t_on && m_tokenIndexBefore >= ast->t_on && m_completionOperator != Sql::T_EOF_SYMBOL
                  && ( !ast->commentText || (ast->commentText && m_tokenIndexBefore <= ast->commentText->firstToken()) ) ) {
            if ( StatementAST::Info *info = ast->info )
                completeTargetIdentifier(info->target);
        }
    }

    return false;
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

void SqlCompletionAssistVisitor::addSchemaObjects( const QString &schemaName, int objectType )
{
    switch ( objectType ) {
        case Sql::Constants::DbSchemaObjectType::C_TABLE:       addTables(schemaName, m_exceptName);      break;
        case Sql::Constants::DbSchemaObjectType::C_VIEW:        addViews(schemaName);       break;
        case Sql::Constants::DbSchemaObjectType::C_INDEX:       addIndices(schemaName);     break;
        case Sql::Constants::DbSchemaObjectType::C_SEQUENCE:    addSequnces(schemaName);    break;
        case Sql::Constants::DbSchemaObjectType::C_CONSTRAINT:  addConstraints(schemaName); break;
    }
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

void SqlCompletionAssistVisitor::addTables( const QString &schemaName, const QString exceptName )
{
    m_globalScope->addTables(schemaName, exceptName, m_completions);
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

void SqlCompletionAssistVisitor::addTypes(const QString &schemaName)
{
    Sql::DbMetadataProvider::getInstance()->fillSchemaObjects(schemaName, *m_completions, Sql::Constants::DbSchemaObjectType::C_TYPE);
    completeSchemaObject(Sql::Constants::DbSchemaObjectType::C_TABLE, false);
}

void SqlCompletionAssistVisitor::addColumnName( const QString &columnName )
{
    TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
    item ->setText(columnName);
    item->setIcon(QIcon(":/images/dbb_field_text.png"));
    m_completions->append(item);
}

void SqlCompletionAssistVisitor::addSchemaTableFields( const QString &schemaName, const QString &tableName, const QStringList &substitutes )
{
    m_globalScope->addTableColumns(schemaName, tableName, m_completions);
    Sql::DbMetadataProvider::getInstance()->fillTableColumns(schemaName, tableName, *m_completions, substitutes);
}

void SqlCompletionAssistVisitor::addSchemaViewFields( const QString &schemaName, const QString &viewName, const QStringList &substitutes )
{
    Sql::DbMetadataProvider::getInstance()->fillViewColumns(schemaName, viewName, *m_completions, substitutes);
}

void SqlCompletionAssistVisitor::addSchemaTableTriggers( const QString &schemaName, const QString &tableName )
{
    Sql::DbMetadataProvider::getInstance()->fillTableTriggers(schemaName, tableName, *m_completions);
}

void SqlCompletionAssistVisitor::addSchemaTableIndexes( const QString &schemaName, const QString &tableName )
{
    Sql::DbMetadataProvider::getInstance()->fillTableIndexes(schemaName, tableName, *m_completions);
}

void SqlCompletionAssistVisitor::addAllSchamasTriggers()
{
    Sql::DbMetadataProvider::getInstance()->fillAllTriggers(*m_completions);
}

void SqlCompletionAssistVisitor::addConstraints( const QString &schemaName, const QString &tableName )
{
    Sql::DbMetadataProvider *provider = Sql::DbMetadataProvider::getInstance();
    if ( tableName.isEmpty() )
        provider->fillSchemaObjects(schemaName, *m_completions, Sql::Constants::DbSchemaObjectType::C_CONSTRAINT);
    else {
        m_globalScope->addTableConstraints(schemaName, tableName, m_completions);
        provider->fillTableConstraints(schemaName, tableName, *m_completions);
    }
}

void SqlCompletionAssistVisitor::addSequnces( const QString &schemaName )
{
    Sql::DbMetadataProvider::getInstance()->fillSchemaObjects(schemaName, *m_completions, Sql::Constants::DbSchemaObjectType::C_SEQUENCE);
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
                case PostgresParser::token::T_ALTER:
                    addSimpleKeyword("TABLE ");
                    break;
                case PostgresParser::token::T_SELECT:
                    addSimpleSequence("* FROM ");
                    break;
                case PostgresParser::token::T_INSERT:
                    addSimpleKeyword("INTO ");
                    break;
                case PostgresParser::token::T_DELETE:
                    addSimpleKeyword("FROM ");
                    break;
                case PostgresParser::token::T_CREATE:
                case PostgresParser::token::T_DROP:
                    addObjectKeywords("", " IF EXISTS ");
                    break;
                case PostgresParser::token::T_COMMENT:
                    addObjectKeywords("ON ", "");
                    addSimpleSequence("ON COLUMN ");
                    break;
            }
            return;
        }
    }
    else
        addKeywords();
}

void SqlCompletionAssistVisitor::addObjectKeywords( const QString &prefix, const QString &availableSuffix )
{
    if ( prefix.isEmpty() ) {
        addSimpleKeyword("TABLE" );
        addSimpleKeyword("VIEW ");
        addSimpleKeyword("INDEX");
        addSimpleKeyword("TRIGGER ");
        addSimpleKeyword("LANGUAGE ");
        addSimpleKeyword("CAST");
        addSimpleKeyword("ASSERTION ");
        addSimpleKeyword("ROLE ");
        addSimpleKeyword("USER ");
        addSimpleKeyword("FUNCTION ");
        addSimpleKeyword("AGGREGATE ");
        addSimpleKeyword("SCHEMA ");
        addSimpleKeyword("DATABASE ");
        addSimpleKeyword("OPERATOR ");
        addSimpleKeyword("TABLESPACE ");
        addSimpleKeyword("SEQUENCE ");
        addSimpleKeyword("TYPE ");
        addSimpleKeyword("DOMAIN ");
        addSimpleKeyword("COLLATION ");
        addSimpleKeyword("CONVERSION ");
        addSimpleKeyword("EXTENSION ");
        addSimpleKeyword("RULE ");
        addSimpleKeyword("SERVER ");
    }
    else {
        addSimpleSequence(prefix + "TABLE" );
        addSimpleSequence(prefix + "VIEW ");
        addSimpleSequence(prefix + "INDEX ");
        addSimpleSequence(prefix + "TRIGGER ");
        addSimpleSequence(prefix + "LANGUAGE ");
        addSimpleSequence(prefix + "CAST");
        addSimpleSequence(prefix + "ASSERTION ");
        addSimpleSequence(prefix + "ROLE ");
        addSimpleSequence(prefix + "USER ");
        addSimpleSequence(prefix + "FUNCTION ");
        addSimpleSequence(prefix + "AGGREGATE ");
        addSimpleSequence(prefix + "SCHEMA ");
        addSimpleSequence(prefix + "DATABASE ");
        addSimpleSequence(prefix + "OPERATOR ");
        addSimpleSequence(prefix + "TABLESPACE ");
        addSimpleSequence(prefix + "SEQUENCE ");
        addSimpleSequence(prefix + "TYPE ");
        addSimpleSequence(prefix + "DOMAIN ");
        addSimpleSequence(prefix + "COLLATION ");
        addSimpleSequence(prefix + "CONVERSION ");
        addSimpleSequence(prefix + "EXTENSION ");
        addSimpleSequence(prefix + "RULE ");
        addSimpleSequence(prefix + "SERVER ");
    }

    addSimpleSequence(prefix + "FOREIGN TABLE ");
    addSimpleSequence(prefix + "FOREIGN DATA WRAPPER ");
    addSimpleSequence(prefix + "MATERIALIZED VIEW ");
    addSimpleSequence(prefix + "EVENT TRIGGER ");
    addSimpleSequence(prefix + "OPERATOR CLASS ");
    addSimpleSequence(prefix + "OPERATOR FAMILY ");
    addSimpleSequence(prefix + "USER MAPPING ");
    addSimpleSequence(prefix + "TEXT SEARCH PARSER ");
    addSimpleSequence(prefix + "TEXT SEARCH DICTIONARY ");
    addSimpleSequence(prefix + "TEXT SEARCH TEMPLATE ");
    addSimpleSequence(prefix + "TEXT SEARCH CONFIGURATION ");
    addSimpleSequence(prefix + "INDEX CONCURRENTLY ");

    if ( !availableSuffix.isEmpty() ) {
        addSimpleSequence(prefix + "TABLE" + availableSuffix);
        addSimpleSequence(prefix + "FOREIGN DATA WRAPPER" + availableSuffix);
        addSimpleSequence(prefix + "FOREIGN TABLE" + availableSuffix);
        addSimpleSequence(prefix + "VIEW" + availableSuffix);
        addSimpleSequence(prefix + "MATERIALIZED VIEW " + availableSuffix);
        addSimpleSequence(prefix + "INDEX" + availableSuffix);
        addSimpleSequence(prefix + "INDEX CONCURRENTLY" + availableSuffix);
        addSimpleSequence(prefix + "TABLE" + availableSuffix);
        addSimpleSequence(prefix + "TRIGGER" + availableSuffix);
        addSimpleSequence(prefix + "LANGUAGE" + availableSuffix);
        addSimpleSequence(prefix + "OPERATOR CLASS" + availableSuffix);
        addSimpleSequence(prefix + "OPERATOR FAMILY" + availableSuffix);
        addSimpleSequence(prefix + "CAST" + availableSuffix);
        addSimpleSequence(prefix + "ROLE" + availableSuffix);
        addSimpleSequence(prefix + "USER" + availableSuffix);
        addSimpleSequence(prefix + "USER MAPPING" + availableSuffix);
        addSimpleSequence(prefix + "FUNCTION" + availableSuffix);
        addSimpleSequence(prefix + "AGGREGATE" + availableSuffix);
        addSimpleSequence(prefix + "OPERATOR" + availableSuffix);
        addSimpleSequence(prefix + "DATABASE" + availableSuffix);
        addSimpleSequence(prefix + "TABLESPACE" + availableSuffix);
        addSimpleSequence(prefix + "SEQUENCE" + availableSuffix);
        addSimpleSequence(prefix + "SCHEMA" + availableSuffix);
        addSimpleSequence(prefix + "RULE" + availableSuffix);
        addSimpleSequence(prefix + "SERVER" + availableSuffix);
    }
}

#define IDENT_OR_STR(k) ( (k) == Sql::T_IDENTIFIER || (k) == Sql::T_STRING_LITERAL )

void SqlCompletionAssistVisitor::completeSchema( bool kwd )
{
    unsigned k = tokenKind();

    if ( IDENT_OR_STR(k) )
         addSchemas();

     if ( kwd && k != Sql::T_DOT )
         addKeywords();
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

    if ( IDENT_OR_STR(k0) && k1 == Sql::T_DOT && IDENT_OR_STR(k2) && k3 == Sql::T_DOT && IDENT_OR_STR(k4) ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 4).identifier->chars();
        tableName  = unit->tokenAt(m_tokenIndexBefore - 2).identifier->chars();
        addSchemaTableFields(schemaName, tableName);
        addSchemaViewFields(schemaName,tableName);
        return;
    }

    else if ( IDENT_OR_STR(k1) && k2 == Sql::T_DOT && IDENT_OR_STR(k3) && k4 == Sql::T_DOT ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 3).identifier->chars();
        tableName  = unit->tokenAt(m_tokenIndexBefore - 1).identifier->chars();
        addSchemaTableFields(schemaName, tableName);
        addSchemaViewFields(schemaName, tableName);
        return;
    }

    else if ( IDENT_OR_STR(k2) && k3 == Sql::T_DOT && IDENT_OR_STR(k4) ) {
        tableName = unit->tokenAt(m_tokenIndexBefore - 2 ).identifier->chars();
    }

    else if ( IDENT_OR_STR(k3) && k4 == Sql::T_DOT ) {
        tableName = unit->tokenAt(m_tokenIndexBefore - 1 ).identifier->chars();
        if ( ts )
            m_tableAliasesToNames[tableName] = qMakePair(tableName, QString(""));
    }

    else if ( IDENT_OR_STR(k4) ) {
        tableName = "";
    }

    else {
        if ( kwd && k4 != Sql::T_IDENTIFIER && k4 != Sql::T_STRING_LITERAL )
            addKeywords();
        return;
    }

    if ( m_currentStatement ) {
        QStringList tables;
        QStringList tableSchemas;
        QList<QStringList> substitutes;

        if ( tableName.isEmpty() ) {
            // Add All Columns
            foreach ( RelationExprAST *rel, *(m_currentStatement->relations) ) {
                TableIdentifierAST *tableIdent = relationIdentifier(rel);
                if ( tableIdent->tableName ) {
                    tables << QString(tableIdent->tableName->name->chars());
                    substitutes << QStringList();
                    if ( tableIdent->schemaName )
                        tableSchemas << QString(tableIdent->schemaName->name->chars());
                    else
                        tableSchemas << "";
                }
            }
        }
        else {
            tables << tableName;
            tableSchemas << "";
            substitutes << QStringList();
            foreach ( RelationExprAST *rel, *(m_currentStatement->relations) ) {
                if ( !rel->alias )
                     continue;
                if ( !QString(rel->alias->aliasName->name->chars()).compare(tableName, Qt::CaseInsensitive ) ) {
                    TableIdentifierAST *tableIdent = relationIdentifier(rel);
                    if ( tableIdent->tableName ) {
                        tables << QString(tableIdent->tableName->name->chars());
                        if ( tableIdent->schemaName )
                            tableSchemas << QString(tableIdent->schemaName->name->chars());
                        else
                            tableSchemas << "";
                        QStringList columnAliases;
                        for ( NameListAST *colName = rel->alias->columns; colName; colName = colName->next )
                            columnAliases << QString(colName->value->name->chars());
                        substitutes << columnAliases;
                    }
                }
            }
        }

        for (int j = 0; j < tables.size(); j++) {
            addSchemaTableFields(tableSchemas[j], tables[j], substitutes[j]);
            addSchemaViewFields(tableSchemas[j], tables[j], substitutes[j]);
        }
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

    if ( IDENT_OR_STR(k0) && k1 == Sql::T_DOT && IDENT_OR_STR(k2) ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 2).identifier->chars();
        prepared = true;
    }
    else if ( IDENT_OR_STR(k1) && k2 == Sql::T_DOT ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 1).identifier->chars();
        prepared = true;
    }
    else if ( IDENT_OR_STR(k2) ) {
        addSchemas();
        exceptTableName = unit->tokenAt(m_tokenIndexBefore).identifier->chars();
        prepared = true;
    }

    if ( prepared )
        addTablesAndViews(schemaName, exceptTableName);

    if ( kwd && k2 != Sql::T_DOT )
        addKeywords();
}

void SqlCompletionAssistVisitor::completeObjectIdentifier( int objectType, int start_token )
{
    Sql::TranslationUnit *unit = translationUnit();
    start_token--;

    if ( unit->tokenAt(start_token + 1).keyid1() == Postgres::PostgresParser::token::T_IF &&
         unit->tokenAt(start_token + 2).keyid1() == Postgres::PostgresParser::token::T_EXISTS )
        start_token += 2;
    else if ( unit->tokenAt(start_token + 1).keyid1() == Postgres::PostgresParser::token::T_IF &&
              unit->tokenAt(start_token + 2).keyid1() == Postgres::PostgresParser::token::T_NOT &&
              unit->tokenAt(start_token + 3).keyid1() == Postgres::PostgresParser::token::T_EXISTS )
        start_token += 3;

    unsigned k0 = unit->tokenAt(start_token + 1).kind();

    qDebug() << "SqlCompletionAssistVisitor::completeObjectIdentifier" << start_token << k0;

    if ( !IDENT_OR_STR(k0) )
        return;

    if ( m_tokenIndexBefore == start_token + 1 ) {
        addSchemas();
        m_exceptName = unit->tokenAt(start_token + 1).identifier->chars();
        addSchemaObjects("", objectType);
        qDebug() << "SqlCompletionAssistVisitor::completeTableIdentifier 01" << start_token << k0;
        return;
    }

    unsigned k1 = unit->tokenAt(start_token + 2).kind();
    if ( k1 != Sql::T_DOT ) {
        return;
    }

    unsigned k2 = unit->tokenAt(start_token + 3).kind();
    if ( m_tokenIndexBefore == start_token + 2 || ( m_tokenIndexBefore == start_token + 3 && IDENT_OR_STR(k2) ) ) {
        m_exceptName = "";
        if ( m_tokenIndexBefore == start_token + 3 )
            m_exceptName = unit->tokenAt(start_token + 3).identifier->chars();
        addSchemaObjects(unit->tokenAt(start_token + 1).identifier->chars(), objectType);
    }
}

void SqlCompletionAssistVisitor::completeSchemaObject( int objectType, bool kwd )
{
    if ( m_tokenIndexBefore == -1 || m_completionOperator == Sql::T_EOF_SYMBOL )
        return;

    Sql::TranslationUnit *unit = translationUnit();

    unsigned k0 = tokenKind(-2);
    unsigned k1 = tokenKind(-1);
    unsigned k2 = tokenKind();

    bool prepared = false;
    QString schemaName = "";

    m_exceptName = "";

    if ( IDENT_OR_STR(k0) && k1 == Sql::T_DOT && IDENT_OR_STR(k2) ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 2).identifier->chars();
        m_exceptName = unit->tokenAt(m_tokenIndexBefore).identifier->chars();
        prepared = true;
    }
    else if ( IDENT_OR_STR(k1) && k2 == Sql::T_DOT ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 1).identifier->chars();
        prepared = true;
    }
    else if ( IDENT_OR_STR(k2) ) {
        addSchemas();
        m_exceptName = unit->tokenAt(m_tokenIndexBefore).identifier->chars();
        prepared = true;
    }

    if ( prepared )
        addSchemaObjects(schemaName, objectType);

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

    if ( IDENT_OR_STR(k0) && k1 == Sql::T_DOT && IDENT_OR_STR(k2) ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 2).identifier->chars();
        prepared = true;
    }
    else if ( IDENT_OR_STR(k1) && k2 == Sql::T_DOT ) {
        schemaName = unit->tokenAt(m_tokenIndexBefore - 1).identifier->chars();
        prepared = true;
    }
    else if ( IDENT_OR_STR(k2) ) {
        addSchemas();
        prepared = true;
    }

    if ( prepared )
        addTriggers(schemaName);

    if ( kwd && k2 != Sql::T_DOT )
        addKeywords();
}

void SqlCompletionAssistVisitor::completeTargetIdentifier( StatementAST::Info::Target target )
{
    switch ( target ) {
        case StatementAST::Info::Schema:
            completeSchema();
            break;
        case StatementAST::Info::Column:
            completeSchemaTableViewField();
            break;
        case StatementAST::Info::Table:
            completeSchemaObject(Sql::Constants::DbSchemaObjectType::C_TABLE);
            break;
        case StatementAST::Info::View:
            completeSchemaObject(Sql::Constants::DbSchemaObjectType::C_VIEW);
            break;
        case StatementAST::Info::Index:
            completeSchemaObject(Sql::Constants::DbSchemaObjectType::C_INDEX);
            break;
        case StatementAST::Info::Trigger:
            qDebug() << "comm complete (trigger)";
            addAllSchamasTriggers();
            break;
        case StatementAST::Info::Constraint:
            completeSchemaObject(Sql::Constants::DbSchemaObjectType::C_CONSTRAINT);
            break;
        case StatementAST::Info::Sequence:
            completeSchemaObject(Sql::Constants::DbSchemaObjectType::C_SEQUENCE);
            break;
        default:
            if ( m_completionOperator != Sql::T_EOF_SYMBOL )
                completeSchema();
            m_completed = false;
            break;
    }
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

TableType *SchemaScope::findOrInsertTable( const QString &tableName )
{
    foreach ( TableType *table, tables )
        if ( !table->tableName.compare(tableName, Qt::CaseInsensitive) )
            return table;
    TableType *newTable = new TableType(tableName.toUpper(), this);
    tables.append(newTable);
    return newTable;
}

TableType *SchemaScope::findTable( const QString &tableName )
{
    foreach ( TableType *table, tables )
        if ( !table->tableName.compare(tableName, Qt::CaseInsensitive) )
            return table;
    return 0;
}

TableType *SchemaScope::insertTable( const QString &tableName )
{
    TableType *newTable = new TableType(tableName.toUpper(), this);
    tables.append(newTable);
    return newTable;
}

TableType *FileScope::findOrInsertTable(const QString &schemaName, const QString &tableName, bool isCreateTable )
{
    QString sName = ( schemaName.isEmpty() ? m_defaultSchemaName.toUpper() : schemaName.toUpper() );
    SchemaScope *schema = 0;
    if ( m_scopes.find(sName) == m_scopes.end()  ) {
        schema = new SchemaScope;
        m_scopes[sName] = schema;
    }
    else
        schema = m_scopes[sName];

    if ( isCreateTable )
        return schema->findOrInsertTable(tableName);

    if ( TableType *table = schema->findTable(tableName) )
        return table;

    if ( Sql::DbMetadataProvider::getInstance()->isTableInSchema(sName, tableName) )
        return schema->insertTable(tableName);

    return 0;
}

void FileScope::addTables( const QString &schemaName, const QString &exceptTableName, QList<TextEditor::BasicProposalItem *> *completions )
{
    QString sName = ( schemaName.isEmpty() ? m_defaultSchemaName.toUpper() : schemaName.toUpper() );
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
    qDebug() << "$$$ FileScope::addTableColumns" << schemaName << tableName;
    QString sName = ( schemaName.isEmpty() ? m_defaultSchemaName.toUpper() : schemaName.toUpper() );
    if ( m_scopes.find(sName) == m_scopes.end() )
        return;
    SchemaScope *schema = m_scopes[sName];
    foreach ( TableType *table, schema->tables ) {
        if ( !table->tableName.compare(tableName, Qt::CaseInsensitive) ) {
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

void FileScope::addTableConstraints( const QString &schemaName, const QString &tableName, QList<TextEditor::BasicProposalItem *> *completions )
{
    QString sName = ( schemaName.isEmpty() ? m_defaultSchemaName.toUpper() : schemaName.toUpper() );
    if ( m_scopes.find(sName) == m_scopes.end() )
        return;
    SchemaScope *schema = m_scopes[sName];
    foreach ( TableType *table, schema->tables ) {
        if ( !table->tableName.compare(tableName, Qt::CaseInsensitive) ) {
            foreach ( const QString &name, table->constraints ) {
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
    QString sName = ( schemaName.isEmpty() ? m_defaultSchemaName.toUpper() : schemaName.toUpper() );
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

}   // namespace Postgres
