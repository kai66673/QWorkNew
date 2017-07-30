#ifndef POSTGRESCOMPLETIONASSISTVISITOR_H
#define POSTGRESCOMPLETIONASSISTVISITOR_H

#include "PostgresAST.h"
#include "PostgresASTVisitor.h"
#include "PostgresASTNoVisitor.h"

namespace Postgres {

class SchemaScope;
class FileScope;

class TableType
{
    friend class SchemaScope;
    friend class FileScope;

public:
    TableType( const QString &tableName_, SchemaScope *schema_ )
        : tableName(tableName_)
        , schema(schema_)
    {}

    void findOrInsertColumn( const QString &column );
    bool findOrInsertIndex( const QString &index );
    bool findOrInsertTrigger( const QString &trigger );
    bool findOrInsertConstraint( const QString &constraint );

private:
    QString tableName;
    QStringList columns;
    QStringList indexes;
    QStringList triggers;
    QStringList constraints;

    SchemaScope *schema;
};

class SchemaScope
{
    friend class TableType;
    friend class FileScope;

public:
    virtual ~SchemaScope() { qDeleteAll(tables); }

    TableType *findOrInsertTable( const QString &tableName );
    TableType *findTable( const QString &tableName );
    TableType *insertTable( const QString &tableName );

private:
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
        m_scopes[m_defaultSchemaName.toUpper()] = new SchemaScope;
    }
    virtual ~FileScope() { qDeleteAll(m_scopes.values()); }

    TableType *findOrInsertTable( const QString &schemaName, const QString &tableName , bool isCreateTable = false );

    void addTables( const QString &schemaName, const QString &exceptTableName, QList<TextEditor::BasicProposalItem *> *completions );
    void addTableColumns( const QString &schemaName, const QString &tableName, QList<TextEditor::BasicProposalItem *> *completions );
    void addTableConstraints( const QString &schemaName, const QString &tableName, QList<TextEditor::BasicProposalItem *> *completions );
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

class SqlCompletionAssistVisitor: protected ASTNoVisitor
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

    void completeCommon( StatementAST *statement );
    bool visit( DropTableStmtAST *ast );
    bool visit( DropViewStmtAST *ast );
    bool visit( DropTrigStmtAST *ast );
    bool visit( CommonCommentStmtAST *ast );

    bool checkSchemaName( const QString &schemaName );
    void addKeywords();
    void addSchemaObjects( const QString &schemaName, int objectType );
    void addSchemas();
    void addTablesAndViews( const QString &schemaName , const QString &exceptTableName = "" );
    void addTables( const QString &schemaName, const QString exceptName );
    void addViews( const QString &schemaName );
    void addIndices( const QString &schemaName );
    void addTriggers( const QString &schemaName );
    void addTypes( const QString &schemaName );
    void addColumnName( const QString &columnName );
    void addSchemaTableFields( const QString &schemaName, const QString &tableName, const QStringList &substitutes = QStringList() );
    void addSchemaViewFields( const QString &schemaName, const QString &viewName, const QStringList &substitutes = QStringList() );
    void addSchemaTableTriggers( const QString &schemaName, const QString &tableName );
    void addSchemaTableIndexes( const QString &schemaName, const QString &tableName );
    void addAllSchamasTriggers();
    void addConstraints( const QString &schemaName, const QString &tableName = "" );
    void addSequnces( const QString &schemaName );
    void addSimpleKeyword( const QString &text );
    void addSimpleSequence( const QString &text );
    void addSimpleField( const QString &field );

    void completeSimpleSequences( unsigned firstToken );
    void addObjectKeywords(const QString &prefix = "" , const QString &availableSuffix = "" );

protected:
    void completeSchema( bool kwd = true );
    void completeSchemaTableViewField( bool kwd = true , bool ts = false );
    void completeSchemaTableView( bool kwd = true );
    void completeObjectIdentifier( int objectType, int start_token );
    void completeSchemaObject( int objectType, bool kwd = true );
    void completeSchemaTrigger( bool kwd = true );
    void completeTargetIdentifier( StatementAST::Info::Target target );

private:
    QMap<QString, QPair<QString, QString> > m_tableAliasesToNames;

    void prepareArroundContext();
    int tokenKeyword( unsigned index = 0 ) const { return m_arroundTokenKeywords[index + 4]; }
    unsigned tokenKind( unsigned index = 0 ) const { return m_arroundTokenKinds[index + 4]; }

    int m_arroundTokenKeywords[9];
    unsigned m_arroundTokenKinds[9];

    QString m_defaultSchemaName;
    FileScope *m_globalScope;
    StatementAST *m_currentStatement;
    bool m_completed;

    QString m_exceptName;
};

}   // namespace Postgres

#endif // POSTGRESCOMPLETIONASSISTVISITOR_H
