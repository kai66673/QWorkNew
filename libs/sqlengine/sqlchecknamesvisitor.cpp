#include "sqlchecknamesvisitor.h"

namespace Sql {

bool SqlCheckNamesVisitor::preVisit( AST * /*ast*/ )
{
    if ( m_reflector->isWalkCanceled() )
        return false;

    return true;
}

void SqlCheckNamesVisitor::postVisit( AST * /*ast*/ )
{
}

void SqlCheckNamesVisitor::addUse( unsigned tokenIndex, CheckName::Use::Kind kind )
{
    if (! tokenIndex)
        return;

    const Token &tok = tokenAt(tokenIndex);
    if (tok.generated())
        return;

    unsigned line, column;
    getTokenStartPosition(tokenIndex, &line, &column);
    const unsigned length = tok.length();

    CheckName::Use *use = new CheckName::Use(line, column, length, kind);
    m_reflector->invokeSendData(qVariantFromValue((void *) use));
}

} // namespace Sql
