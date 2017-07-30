#ifndef SQLCHECKNAMES_H
#define SQLCHECKNAMES_H

#include "SqlDocument.h"
#include "SqlSemanticInfo.h"
#include "ASTVisitor.h"
#include "AST.h"
#include "TranslationUnit.h"

#include <QRunnable>
#include <QFutureInterface>
#include <QFuture>
#include <QThread>
#include <QThreadPool>

namespace Sql {
namespace Editor {

class SqlCheckNames: public QRunnable, public QFutureInterface<Sql::CheckName::Use *>, public BaseWalkReflector
{
public:
    virtual ~SqlCheckNames();

    virtual bool isWalkCanceled() const;
    virtual void invokeSendData( const QVariant &data );

    virtual void run();

    typedef Sql::CheckName::Use Use;
    typedef QFuture<Use *> SqlFuture;

    SqlFuture start()
    {
        this->setRunnable(this);
        this->reportStarted();
        SqlFuture future = this->future();
        QThreadPool::globalInstance()->start(this, QThread::LowestPriority);
        return future;
    }

    static SqlFuture go( Document::Ptr doc );

    static QMap<int, QVector<Use *> > chunks( const QFuture<Use *> &future, int from, int to );

    void flush();
    void addUse( Use *use );

protected:
    SqlCheckNames( Document::Ptr doc );

private:
    Document::Ptr m_doc;
    QVector<Use *> _usages;
    bool _flushRequested;
    unsigned _flushLine;
};

} // namespace Editor
} // namespace Sql

#endif // SQLCHECKNAMES_H
