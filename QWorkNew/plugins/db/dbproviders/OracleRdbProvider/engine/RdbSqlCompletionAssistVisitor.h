#ifndef RDBSQLCOMPLETIONASSISTVISITOR_H
#define RDBSQLCOMPLETIONASSISTVISITOR_H

#include "RdbAST.h"
#include "RdbASTVisitor.h"
#include "TranslationUnit.h"
#include "RdbSqlScope.h"

namespace RDB {

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
    unsigned m_tokenIndexBefore;
    QList<TextEditor::BasicProposalItem *> *m_completions;
    TranslationUnitAST *m_ast;

    void fillBlockCompletions( BlockStatementAST *ast );
    void fillModuleCompletions( CreateModuleStatementAST *ast );

    bool checkSchemaName( const QString &schemaName );
    void addKeywords();
    void addSchemas();
    void addTablesAndViews( const QString &schemaName );
    void addTables( const QString &schemaName );
    void addViews( const QString &schemaName );
    void addIndices( const QString &schemaName );
    void addTriggers( const QString &schemaName );
    void addModules( const QString &schemaName );
    void addFunctionsAndProcedures( const QString &schemaName );
    void addAreas( const QString &schemaName );
    void addSchemaTableFields( const QString &schemaName, const QString &tableName );
    void addSchemaViewFields( const QString &schemaName, const QString &viewName );
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
    void complete( UpdateStatementAST *ast );

    bool visit( InsertStatementAST *ast );
    void complete( InsertStatementAST *ast );

    bool visit( DeleteStatementAST *ast );
    void complete( DeleteStatementAST *ast );

    bool visit( CreateTableStatementAST *ast );
    void complete( CreateTableStatementAST *ast );
    bool complete( CreateTableDirectClauseAST *ast );

    bool visit( CreateViewStatementAST *ast );
    void complete( CreateViewStatementAST *ast );

    bool visit( CreateIndexStatementAST *ast );
    void complete( CreateIndexStatementAST *ast );

    bool visit( CreateTriggerStatementAST *ast );
    void complete( CreateTriggerStatementAST *ast );

    bool visit( DropTableStatementAST *ast );
    void complete( DropTableStatementAST *ast );

    bool visit( DropViewStatementAST *ast );
    void complete( DropViewStatementAST *ast );

    bool visit( DropIndexStatementAST *ast );
    void complete( DropIndexStatementAST *ast );

    bool visit( DropTriggerStatementAST *ast );
    void complete( DropTriggerStatementAST *ast );

    bool visit( DropModuleStatementAST *ast );
    void complete( DropModuleStatementAST *ast );

    bool visit( CallStatementAST *ast );
    void complete( CallStatementAST *ast );

    bool visit( TruncateStatementAST *ast );
    void complete( TruncateStatementAST *ast );

    bool visit( CommentOnTableStatementAST *ast );
    void complete( CommentOnTableStatementAST *ast );

    bool visit( CommentOnColumnStatementAST *ast );
    void complete( CommentOnColumnStatementAST *ast );

    bool visit( AlterTableStatementAST *ast );
    void complete( AlterTableStatementAST *ast );
    bool complete( TableConstraintAST *ast, const QString &schemaName, const QString &tableName, QStringList &createTableFields /*= QStringList()*/ );

    bool visit( DirectSingleSourceAST *ast );
    bool visit( SchemaTableNameAST *ast );
    bool visit( TableAliasAST *ast );

    bool visit( CreateStorageMapStatementAST *ast );
    bool complete( CreateStorageMapStatementAST *ast );

    void completeSchemaTableViewField( bool kwd = true );
    void completeSchemaTableView( bool kwd = true );
    void completeSchemaTable( bool kwd = true );
    void completeSchemaView( bool kwd = true );
    void completeSchemaIndex( bool kwd = true );
    void completeSchemaTrigger( bool kwd = true );
    void completeModule();

private:
    QMap<QString, QPair<QString, QString> > m_tableAliasesToNames;

    void prepareArroundContext();
    int tokenKeyword( unsigned index = 0 ) const { return m_arroundTokenKeywords[index + 4]; }
    unsigned tokenKind( unsigned index = 0 ) const { return m_arroundTokenKinds[index + 4]; }

    int m_arroundTokenKeywords[9];
    unsigned m_arroundTokenKinds[9];

    SqlCreationStorage *m_creationStorage;
};

}   // namespace RDB

#endif // RDBSQLCOMPLETIONASSISTVISITOR_H
