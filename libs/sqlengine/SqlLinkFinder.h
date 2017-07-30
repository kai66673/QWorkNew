#ifndef SQLLINKFINDER_H
#define SQLLINKFINDER_H

#include "ILink.h"
#include "AST.h"
#include "ASTVisitor.h"
#include "TranslationUnit.h"
#include "DetailsLink.h"
#include "DbMetadataProvider.h"
#include "DbConnection.h"

namespace Sql {

class SqlLinkFinder: protected ASTVisitor
{
public:
    SqlLinkFinder( TranslationUnit *unit );
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
    Core::ILink *findLinkInSingleStatement( AST *ast );

    DetailsLink *findSchemaObjectLink( unsigned objectType );
    Core::ILink *findCreateTableLink( const QString &tableName );
    Core::ILink *findCreateIndexLink( const QString &indexName );
    Core::ILink *findCreateTriggerLink( const QString &triggerName );
    Core::ILink *findCreateConstraintLink( const QString &constraintName );

    Core::ILink *createTextTargetLinkForToken( unsigned token );

    TranslationUnit *m_translationUnit;
    TranslationUnitAST *m_ast;
    StatementAST *m_currentStatemenAst;
    unsigned m_linkTokenIndex;
    unsigned m_position;
    int m_begin;
    int m_end;
    QString m_linkText;

    Core::ILink *m_link;
    DbMetadataProvider *m_provider;
    Database::DbConnection *m_connection;

    QMap<QString, NameAST *> m_tableAliasesToNames;
};

}   // namespace Sql

#endif // SQLLINKFINDER_H
