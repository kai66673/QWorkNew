#include "SqlScope.h"

namespace Sql {

////////////////////////////////////////////////////////////////////////////////////////////////////////
// TableType
void TableType::findOrInsertColumn( const QString &column )
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

bool TableType::findOrInsertConsatrint( const QString &constraint )
{
    foreach ( const QString &constraintName, constraints )
        if ( !constraintName.compare(constraint, Qt::CaseInsensitive) )
            return true;

    constraints.append(constraint);
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// SchemaScope
TableType *SqlCreationStorage::findOrInsertTable(const QString &tableName)
{
    foreach ( TableType *table, tables )
        if ( !table->tableName.compare(tableName, Qt::CaseInsensitive) )
            return table;
    TableType *newTable = new TableType(tableName.toUpper());
    tables.append(newTable);
    return newTable;
}

bool SqlCreationStorage::findOrInsertIndex( const QString &indexName )
{
    foreach ( const QString &index, indexes )
        if ( !index.compare(indexName, Qt::CaseInsensitive) )
            return true;

    indexes.append(indexName.toUpper());
    return false;
}

void SqlCreationStorage::addTables( QList<TextEditor::BasicProposalItem *> *completions )
{
    foreach ( TableType *table, tables ) {
        TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
        item ->setText(table->tableName);
        item->setIcon(QIcon(":/images/dbb_table.png"));
        completions->append(item);
    }
}

void SqlCreationStorage::addTableColumns( const QString &tableName, QList<TextEditor::BasicProposalItem *> *completions )
{
    foreach ( TableType *table, tables ) {
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

void SqlCreationStorage::addTableConstraints( const QString &tableName, QList<TextEditor::BasicProposalItem *> *completions )
{
    foreach ( TableType *table, tables ) {
        if ( !table->tableName.compare(tableName) ) {
            foreach ( const QString &name, table->constraints ) {
                TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
                item ->setText(name);
                item->setIcon(QIcon(":/images/dbb_constraint.png"));
                completions->append(item);
            }

            return;
        }
    }
}

void SqlCreationStorage::addTableIndexes(const QString &tableName, QList<TextEditor::BasicProposalItem *> *completions)
{
    foreach ( TableType *table, tables ) {
        if ( !table->tableName.compare(tableName) ) {
            foreach ( const QString &name, table->indexes ) {
                TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
                item ->setText(name);
                item->setIcon(QIcon(":/images/dbb_index.png"));
                completions->append(item);
            }

            return;
        }
    }
}

void SqlCreationStorage::addIndexes( QList<TextEditor::BasicProposalItem *> *completions )
{
    foreach ( const QString &index, indexes ) {
        TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
        item ->setText(index);
        item->setIcon(QIcon(":/images/dbb_index.png"));
        completions->append(item);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// SqlScopeVisitor
void SqlCreateScopeVisitor::bind( TranslationUnitAST *ast )
{
    for ( StatementListAST *iter = ast->statement_list; iter; iter = iter->next ) {
        if ( StatementAST *statement = iter->value ) {
            m_currentTableType = 0;
            if ( CreateStatementAST *createStatement = statement->asCreateStatement() )
                createStatement->accept(this);
            else if ( AlterTableStatementAST *alterStatement = statement->asAlterTableStatement() )
                alterStatement->accept(this);
        }
    }
}

bool SqlCreateScopeVisitor::visit( CreateTableStatementAST *ast )
{
    if ( ast ) {
        if ( SchemaTableNameAST *tableIdent =  ast->schemaTableName ) {
            if ( TableNameAST *tableNameAst = tableIdent->tableName ) {
                m_currentTableType = m_creationStorage->findOrInsertTable(tableNameAst->name->chars());
                if ( ast->tableClause ) {
                    if ( CreateTableDirectClauseAST *directCreateAst = ast->tableClause->asCreateTableDirectClause() )
                        accept(directCreateAst);
                }
            }
        }
    }
    return false;
}

bool SqlCreateScopeVisitor::visit( CreateIndexStatementAST *ast )
{
    if ( ast ) {
        if ( ast->schemaIndexName && ast->schemaTableName ) {
            if ( IndexNameAST *indexIdent = ast->schemaIndexName->indexName ) {
                if ( TableNameAST *tableIdent = ast->schemaTableName->tableName ) {
                    m_currentTableType = m_creationStorage->findOrInsertTable(tableIdent->name->chars());
                    if ( m_currentTableType ) {
                        m_creationStorage->findOrInsertIndex(indexIdent->name->chars());
                    }
                }
            }
        }
    }

    return false;
}

bool SqlCreateScopeVisitor::visit( AlterTableStatementAST *ast )
{
    if ( ast && ast->schemaTableName && ast->schemaTableName->tableName &&
         ast->alterTableClause && ast->alterTableClause->asAlterTableAddClause() ) {
        m_currentTableType = m_creationStorage->findOrInsertTable(ast->schemaTableName->tableName->name->chars());
        if ( AlterTableAddColumnClauseAST *addColumnClauseAst = ast->alterTableClause->asAlterTableAddColumnClause() ) {
            if ( addColumnClauseAst->column && addColumnClauseAst->column->columnName )
                m_currentTableType->findOrInsertColumn(addColumnClauseAst->column->columnName->name->chars());
        } else if ( AlterTableAddConstraintClauseAST *addConstraintClauseAsr = ast->alterTableClause->asAlterTableAddConstraintClause() ) {
            if ( addConstraintClauseAsr->constraint && addConstraintClauseAsr->constraint->constraintName )
                m_currentTableType->findOrInsertConsatrint(addConstraintClauseAsr->constraint->constraintName->name->chars());
        }
    }
    return false;
}

bool SqlCreateScopeVisitor::visit( ColumnDefinitionAST *ast )
{
    if (  m_currentTableType && ast && ast->columnName ) {
        m_currentTableType->findOrInsertColumn(ast->columnName->name->chars());
        for ( ColumnConstraintListAST *iter = ast->constraintList; iter; iter = iter->next ) {
            if ( ColumnConstraintAST *colConstrainAst = iter->value ) {
                if ( ConstraintNameAST *constraintNameAst = colConstrainAst->constraintName ) {
                    m_currentTableType->findOrInsertConsatrint(constraintNameAst->name->chars());
                }
            }
        }
    }
    return false;
}

bool SqlCreateScopeVisitor::visit( ConstraintNameAST *ast )
{
    if ( ast && m_currentTableType ) {
        m_currentTableType->findOrInsertConsatrint(ast->name->chars());
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
// SqlIdentifiersScopedTree
SqlIdentifiersScopedTree::SqlIdentifiersScopedTree( SqlIdentifiersScopedTree *parent )
    : m_parent(parent)
{}

SqlIdentifiersScopedTree::~SqlIdentifiersScopedTree()
{
    qDeleteAll(m_scopes);
}

SqlIdentifiersScopedTree *SqlIdentifiersScopedTree::createChildScope()
{
    SqlIdentifiersScopedTree *scope = new SqlIdentifiersScopedTree(this);
    m_scopes << scope;
    return scope;
}

void SqlIdentifiersScopedTree::addIdentifier( NameAST *ident )
{
    m_nameIdentifiers << ident;
}

void SqlIdentifiersScopedTree::fillIdentifiers( QStringList &identifiers )
{
    foreach ( NameAST *name, m_nameIdentifiers )
        identifiers << QString(name->name->chars()).toUpper();

    if ( m_parent )
        m_parent->fillIdentifiers(identifiers);
}

unsigned SqlIdentifiersScopedTree::linkTokenIndex( const QString &name )
{
    foreach ( NameAST *nameAst, m_nameIdentifiers )
        if ( !name.compare(nameAst->name->chars(), Qt::CaseInsensitive) ) {
            return nameAst->name_token;
        }

    if ( m_parent )
        return m_parent->linkTokenIndex(name);

    return 0;
}

}   //  namespace Sql
