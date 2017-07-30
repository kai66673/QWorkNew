#ifndef POSTGRESCOMPLETIONASSISTVISITOR_H
#define POSTGRESCOMPLETIONASSISTVISITOR_H

#include "PostgresAST.h"
#include "PostgresASTVisitor.h"

namespace Postgres {

class TableType
{
public:
    TableType( const QString &tableName_)
        : tableName(tableName_)
    {}

    void findOrInsertColumn( const QString &column );
    bool findOrInsertIndex( const QString &index );

    QString tableName;
    QStringList columns;
    QStringList indexes;
    QStringList triggers;
    QStringList constraints;
};

class SchemaScope {
public:
    virtual ~SchemaScope() { qDeleteAll(tables); }

    TableType *findOrInsertTable( const QString &tableName );

    QList<TableType *> tables;
    QStringList indexes;
    QStringList triggers;

};

class FileScope
{
public:
    FileScope( const QString &defaultSchemaName )
        : m_defaultSchemaName(defaultSchemaName)
    {
        m_scopes[m_defaultSchemaName] = new SchemaScope;
    }
    virtual ~FileScope() { qDeleteAll(m_scopes.values()); }

    TableType *findOrInsertTable( const QString &schemaName, const QString &tableName );
    bool findOrInsertIndex( const QString &schemaName, const QString &indexName );

    void addTables( const QString &schemaName, const QString &exceptTableName, QList<TextEditor::BasicProposalItem *> *completions );
    void addTableColumns( const QString &schemaName, const QString &tableName, QList<TextEditor::BasicProposalItem *> *completions );
    void addIndexes( const QString &schemaName, QList<TextEditor::BasicProposalItem *> *completions );

private:
    QString m_defaultSchemaName;
    QMap<QString, SchemaScope *> m_scopes;
};

class SqlBindingsVisitor: public ASTVisitor
{
public:
    SqlBindingsVisitor( Sql::TranslationUnit *unit )
        : ASTVisitor(unit)
    {}

    void fillCompletitions (QList<TextEditor::BasicProposalItem *> *completions ) {
        foreach ( const QString &name, m_names ) {
            TextEditor::BasicProposalItem *item = new TextEditor::BasicProposalItem;
            item ->setText(name);
            item->setIcon(QIcon(":/images/sql_ci_par.png"));
            completions->append(item);
        }
    }

protected:
    bool visit( NameAST *ast) {
        if ( ast->kind == NameAST::Binding )
            m_names << QString(ast->name->chars());
        return true;
    }

private:
    QStringList m_names;
};

class SqlCompletionAssistVisitor: protected ASTVisitor
{
public:
    SqlCompletionAssistVisitor( Sql::TranslationUnit *unit );
    virtual ~SqlCompletionAssistVisitor();

    int fillCompletions( QList<TextEditor::BasicProposalItem *> *completions, TranslationUnitAST *ast,
                         unsigned position, int startPosition, unsigned completionOperator );

private:
    int  m_startPosition;
    unsigned m_completionOperator;
    int m_tokenIndexBefore;
    QList<TextEditor::BasicProposalItem *> *m_completions;
    TranslationUnitAST *m_ast;

    bool checkSchemaName( const QString &schemaName );
    void addKeywords();
    void addSchemas();
    void addTablesAndViews( const QString &schemaName , const QString &exceptTableName = "" );
    void addTables( const QString &schemaName );
    void addViews( const QString &schemaName );
    void addIndices( const QString &schemaName );
    void addTriggers( const QString &schemaName );
    void addTypes( const QString &schemaName );
    void addColumnName( const QString &columnName );
    void addSchemaTableFields( const QString &schemaName, const QString &tableName );
    void addSchemaViewFields( const QString &schemaName, const QString &viewName );
    void addSchemaTriggersTriggers( const QString &schemaName, const QString &tableName );
    void addConstraints( const QString &schemaName, const QString &tableName );
    void addSimpleKeyword( const QString &text );
    void addSimpleSequence( const QString &text );
    void addSimpleField( const QString &field );

    void completeSimpleSequences( unsigned firstToken );

protected:
    bool visit( SelectStatementAST *ast );
    void complete( SelectStatementAST *ast );
    bool complete( SelectCoreAST *ast, bool hasNext );

    bool visit( UpdateStatementAST *ast );
    bool complete( UpdateStatementAST *ast );

    bool visit( InsertStatementAST *ast );
    bool complete( InsertStatementAST *ast );

    bool visit( DeleteStatementAST *ast );
    bool complete( DeleteStatementAST *ast );

    bool visit( CreateTableStatementAST *ast );
    bool complete( CreateTableStatementAST *ast );
    bool complete( CreateTableDirectClauseAST *ast );

    bool visit( CreateViewStatementAST *ast );
    bool complete( CreateViewStatementAST *ast );

    bool visit( CreateIndexStatementAST *ast );
    bool complete( CreateIndexStatementAST *ast );

    bool visit( CreateTriggerStatementAST *ast );
    bool complete( CreateTriggerStatementAST *ast );

    bool visit( DropTableStatementAST *ast );
    bool complete( DropTableStatementAST *ast );

    bool visit( DropViewStatementAST *ast );
    bool complete( DropViewStatementAST *ast );

    bool visit( DropIndexStatementAST *ast );
    bool complete( DropIndexStatementAST *ast );

    bool visit( DropTriggerStatementAST *ast );
    bool complete( DropTriggerStatementAST *ast );

    bool visit( TruncateStatementAST *ast );
    bool complete( TruncateStatementAST *ast );

    bool visit( CommentOnTableStatementAST *ast );
    bool complete( CommentOnTableStatementAST *ast );

    bool visit( CommentOnColumnStatementAST *ast );
    bool complete( CommentOnColumnStatementAST *ast );

    bool visit( AlterTableStatementAST *ast );
    bool complete( AlterTableStatementAST *ast );
    bool complete( TableConstraintAST *ast, const QString &schemaName, const QString &tableName, QStringList &createTableFields /*= QStringList()*/ );

    bool visit( DirectSingleSourceAST *ast );
    bool visit( TableIdentifierAST *ast );

    void completeSchemaTableViewField( bool kwd = true , bool ts = false );
    void completeSchemaTableView( bool kwd = true );
    void completeSchemaTable( bool kwd = true );
    void completeSchemaView( bool kwd = true );
    void completeSchemaIndex( bool kwd = true );
    void completeSchemaTrigger( bool kwd = true );

private:
    QMap<QString, QPair<QString, QString> > m_tableAliasesToNames;

    void prepareArroundContext();
    int tokenKeyword( unsigned index = 0 ) const { return m_arroundTokenKeywords[index + 4]; }
    unsigned tokenKind( unsigned index = 0 ) const { return m_arroundTokenKinds[index + 4]; }

    int m_arroundTokenKeywords[9];
    unsigned m_arroundTokenKinds[9];

    QString m_defaultSchemaName;
    FileScope *m_globalScope;
};

}   // namespace Postgres

#endif // POSTGRESCOMPLETIONASSISTVISITOR_H
