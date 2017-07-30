#include "PostgresSqlCheckNamesVisitor.h"

namespace Postgres {

bool SqlCheckNamesVisitor::preVisit( AST * /*ast*/ )
{
    if ( m_reflector->isWalkCanceled() )
        return false;

    return true;
}

void SqlCheckNamesVisitor::postVisit( AST * /*ast*/ )
{
}

bool SqlCheckNamesVisitor::visit( ColumnRefAST *ast )
{
    if ( ast ) {
        if ( !ast->qualifiers ) {
            ast->firstName->kind = NameAST::Column;
        } else {
            QList<NameAST *> attrs;
            bool isStarAttr = false;
            for ( IndirectionListAST *iter = ast->qualifiers; iter; iter = iter->next ) {
                if ( AttirbuteIndirectionAST *attr = iter->value->asAttirbuteIndirection() )
                    attrs << attr->attrName;
                else if ( iter->value->asStarIndirection() ) {
                    if ( iter->next ) {
                        translationUnit()->errorString(iter->value->lastToken(), "Invalid Indirection for Column Reference");
                    }
                    isStarAttr = true;
                    break;
                }
            }
            if ( isStarAttr ) {
                if ( attrs.size() == 0 ) {
                    ast->firstName->kind = NameAST::Table;
                } else if ( attrs.size() == 1 ) {
                    ast->firstName->kind = NameAST::Schema;
                    attrs[0]->kind = NameAST::Table;
                } else {
                    translationUnit()->errorString(ast->firstName->firstToken(), attrs[attrs.size() - 1]->lastToken(), "Too Long Indirection");
                }
            } else {
                if ( attrs.size() == 0 ) {
                    ast->firstName->kind = NameAST::Column;
                } else if ( attrs.size() == 1 ) {
                    ast->firstName->kind = NameAST::Table;
                    attrs[0]->kind = NameAST::Column;
                } else if ( attrs.size() > 1 ) {
                    ast->firstName->kind = NameAST::Schema;
                    attrs[0]->kind = NameAST::Table;
                    attrs[1]->kind = NameAST::Column;
                }
            }
        }
    }

    return true;
}

void SqlCheckNamesVisitor::addUse( unsigned tokenIndex, Sql::CheckName::Use::Kind kind )
{
    if (! tokenIndex)
        return;

    const Sql::Token &tok = tokenAt(tokenIndex);
    if (tok.generated())
        return;

    unsigned line, column;
    getTokenStartPosition(tokenIndex, &line, &column);
    const unsigned length = tok.length();

    Sql::CheckName::Use *use = new Sql::CheckName::Use(line, column, length, kind);
    m_reflector->invokeSendData(qVariantFromValue((void *) use));
}

} // namespace Postgres
