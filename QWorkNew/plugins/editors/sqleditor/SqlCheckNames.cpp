#include "SqlCheckNames.h"

#include "AST.h"

namespace Sql {
namespace Editor {

SqlCheckNames::SqlCheckNames( Document::Ptr doc )
    : m_doc(doc)
    , _flushRequested(false)
    , _flushLine(0)
{
}

SqlCheckNames::~SqlCheckNames()
{
    qDeleteAll(_usages);
}

bool SqlCheckNames::isWalkCanceled() const
{
    return isCanceled();
}

void SqlCheckNames::invokeSendData( const QVariant &data )
{
    Use *use = (Use *)data.value<void *>();
    addUse(use);
}

void SqlCheckNames::addUse( Use *use )
{
    if (_usages.size() >= 50) {
        if (_flushRequested && use->line != _flushLine)
            flush();
        else if (! _flushRequested) {
            _flushRequested = true;
            _flushLine = use->line;
        }
    }

    _usages.append(use);
}

void SqlCheckNames::run()
{
    if (! isCanceled()) {
        m_doc->translationUnit()->walk(this);
        flush();
    }

    reportFinished();
}

SqlCheckNames::SqlFuture SqlCheckNames::go(Document::Ptr doc)
{
    return (new SqlCheckNames(doc))->start();
}

QMap<int, QVector<SqlCheckNames::Use *> > SqlCheckNames::chunks(const QFuture<Use *> &future, int from, int to)
{
    QMap<int, QVector<Use *> > chunks;

    for (int i = from; i < to; ++i) {
        Use *use = future.resultAt(i);

        const int blockNumber = use->line;
        chunks[blockNumber].append(use);
    }

    return chunks;
}

void SqlCheckNames::flush()
{
    _flushRequested = false;
    _flushLine = 0;

    if (_usages.isEmpty())
        return;

    reportResults(_usages);
    _usages.clear();
}

} // namespace Editor
} // namespace Sql
