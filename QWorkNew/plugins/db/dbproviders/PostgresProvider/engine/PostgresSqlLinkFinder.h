#ifndef POSTGRESSQLLINKFINDER_H
#define POSTGRESSQLLINKFINDER_H

#include "PostgresASTVisitor.h"
#include "PostgresAST.h"
#include "DbMetadataProvider.h"
#include "DetailsLink.h"

namespace Postgres {

class SqlLinkFinder: protected ASTVisitor
{
public:
    SqlLinkFinder( Sql::TranslationUnit *unit );
    Core::ILink *findLinkAt( TranslationUnitAST *ast, int position );

    Sql::DetailsLink *findSchemaLink();
    Sql::DetailsLink *findSchemaObjectLink( const QString &schemaName, int objectType );

protected:
    virtual bool preVisit( AST * /*ast*/ ) { return (m_link == 0); }

    bool visit( TableIdentifierAST *ast );
    bool visit( IndexIdentifierAST *ast );

private:
    Core::ILink *findLinkInternal( TranslationUnitAST *ast );
    Core::ILink *findLinkInStatement( StatementAST *ast );
    Core::ILink *findLinkInSingleStatement( AST *ast );

    Sql::TranslationUnit *m_translationUnit;
    TranslationUnitAST *m_ast;
    StatementAST *m_currentStatemenAst;
    int m_linkTokenIndex;
    unsigned m_position;
    int m_begin;
    int m_end;
    QString m_linkText;

    Core::ILink *m_link;
    Sql::DbMetadataProvider *m_provider;
    Database::DbConnection *m_connection;
};

}   // namespace Postgres

#endif // POSTGRESSQLLINKFINDER_H
