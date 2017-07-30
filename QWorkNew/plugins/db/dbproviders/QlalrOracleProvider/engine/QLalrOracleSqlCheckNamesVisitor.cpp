#include "QLalrOracleSqlCheckNamesVisitor.h"

namespace QLalrOracle {

bool SqlCheckNamesVisitor::preVisit( AST * /*ast*/ )
{
    if ( m_reflector->isWalkCanceled() )
        return false;

    return true;
}

void SqlCheckNamesVisitor::postVisit( AST * /*ast*/ )
{
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

} // namespace QLalrOracle
