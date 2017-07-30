#ifndef RDBSQLLINKFINDER_H
#define RDBSQLLINKFINDER_H

#include "ILink.h"
#include "RdbAST.h"
#include "RdbASTVisitor.h"
#include "TranslationUnit.h"
#include "DetailsLink.h"
#include "DbMetadataProvider.h"
#include "DbConnection.h"

namespace RDB {

class SqlLinkFinder: protected ASTVisitor
{
public:
    SqlLinkFinder( Sql::TranslationUnit *unit );
    Core::ILink *findLinkAt( TranslationUnitAST *ast, int position );

protected:
    virtual bool preVisit( AST * /*ast*/ ) { return (m_link == 0); }

    bool visit( SelectCoreAST *ast );
    bool visit( FkReferencesClauseAST *ast );
    bool visit( TableNameAST *ast );
    bool visit( ColumnNameAST *ast );
    bool visit( StarResultColumnAST *ast );
    bool visit( SchemaTableNameAST *ast );
    bool visit( TableAliasAST *ast );
    bool visit( IndexNameAST *ast );
    bool visit( TriggerNameAST *ast );
    bool visit( ConstraintNameAST *ast );

private:
    Core::ILink *findLinkInternal( TranslationUnitAST *ast );
    Core::ILink *findLinkInStatement( StatementAST *ast );
    Core::ILink *findLinkInBlockStatement( BlockStatementAST *ast );
    Core::ILink *findLinkInSingleStatement( AST *ast );

    Sql::DetailsLink *findSchemaObjectLink( unsigned objectType );
    Core::ILink *findCreateTableLink( const QString &tableName );
    Core::ILink *findCreateTableColumnLink( const QStringList &tables, const QString &columnName );
    Core::ILink *findCreateIndexLink( const QString &indexName );
    Core::ILink *findCreateTriggerLink( const QString &triggerName );
    Core::ILink *findCreateConstraintLink( const QString &constraintName );

    Core::ILink *createTextTargetLinkForToken( unsigned token );

    Sql::TranslationUnit *m_translationUnit;
    TranslationUnitAST *m_ast;
    StatementAST *m_currentStatemenAst;
    unsigned m_linkTokenIndex;
    unsigned m_position;
    int m_begin;
    int m_end;
    QString m_linkText;

    Core::ILink *m_link;
    Sql::DbMetadataProvider *m_provider;
    Database::DbConnection *m_connection;

    QMap<QString, NameAST *> m_tableAliasesToNames;
};

}   // namespace RDB

#endif // RDBSQLLINKFINDER_H
