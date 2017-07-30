#ifndef SQLSCOPE_H
#define SQLSCOPE_H

#include "AST.h"
#include "ASTVisitor.h"

namespace Sql {

class TableType
{
public:
    TableType( const QString &tableName_)
        : tableName(tableName_)
    {}

    void findOrInsertColumn( const QString &column );
    bool findOrInsertIndex( const QString &index );
    bool findOrInsertConsatrint( const QString &index );

    QString tableName;
    QStringList columns;
    QStringList indexes;
    QStringList triggers;
    QStringList constraints;
};

class SqlCreationStorage {
public:
    virtual ~SqlCreationStorage() { qDeleteAll(tables); }

    TableType *findOrInsertTable( const QString &tableName );
    bool findOrInsertIndex( const QString &indexName );

    void addTables( QList<TextEditor::BasicProposalItem *> *completions );
    void addTableColumns( const QString &tableName, QList<TextEditor::BasicProposalItem *> *completions );
    void addTableConstraints( const QString &tableName, QList<TextEditor::BasicProposalItem *> *completions );
    void addIndexes( QList<TextEditor::BasicProposalItem *> *completions );

    QList<TableType *> tables;
    QStringList indexes;
    QStringList triggers;

};

class SqlCreateScopeVisitor: protected ASTVisitor
{
public:
    SqlCreateScopeVisitor( Sql::TranslationUnit *unit, SqlCreationStorage *globalScope )
        : ASTVisitor(unit)
        , m_creationStorage(globalScope)
        , m_currentTableType(0)
    {}

    void bind( TranslationUnitAST *ast );

protected:
    bool visit( CreateTableStatementAST *ast );
    bool visit( CreateIndexStatementAST *ast );
    bool visit( AlterTableStatementAST *ast );

    bool visit( ColumnDefinitionAST *ast );
    bool visit( ConstraintNameAST *ast );


private:
    SqlCreationStorage *m_creationStorage;
    TableType *m_currentTableType;
};

class SqlIdentifiersScopedTree
{
public:
    SqlIdentifiersScopedTree( SqlIdentifiersScopedTree *parent = 0 );
    virtual ~SqlIdentifiersScopedTree();

    SqlIdentifiersScopedTree *parent() { return m_parent; }
    SqlIdentifiersScopedTree *createChildScope();

    void addIdentifier( NameAST *ident );

    void fillIdentifiers( QStringList &identifiers );
    unsigned linkTokenIndex( const QString &name );

private:
    SqlIdentifiersScopedTree *m_parent;
    QList<SqlIdentifiersScopedTree *> m_scopes;
    QList<NameAST *> m_nameIdentifiers;
};

}   // namespace Sql

#endif // SQLSCOPE_H
