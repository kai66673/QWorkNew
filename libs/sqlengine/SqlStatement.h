#ifndef SQLSTATEMENT_H
#define SQLSTATEMENT_H

#include <QString>
#include <QStringList>

#include "sqlengine_global.h"

namespace Sql {

struct SQL_EXPORT SqlStatement
{
    SqlStatement( bool isSelectStatement_,
                  bool isDdlStatement_,
                  bool isBlockStatement_,
                  bool isCommitStatement_,
                  bool isRollbackStatement_,
                  const QString &statementText_ = "" )
        : isSelectStatement(isSelectStatement_)
        , isDdlStatement(isDdlStatement_)
        , isBlockStatement(isBlockStatement_)
        , isCommitStatement(isCommitStatement_)
        , isRollbackStatement(isRollbackStatement_)
        , statementText(statementText_)
    {}

    bool isSelectStatement;
    bool isDdlStatement;
    bool isBlockStatement;
    bool isCommitStatement;
    bool isRollbackStatement;

    QString statementText;

    QStringList bindNames;
};

}   // namespace Sql

#endif // SQLSTATEMENT_H
