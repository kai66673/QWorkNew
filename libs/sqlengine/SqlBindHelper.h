#ifndef SQLBINDHELPER_H
#define SQLBINDHELPER_H

#include <QString>
#include <QSet>
#include <QMap>
#include <QSqlQuery>

#include "sqlengine_global.h"
#include "ASTVisitor.h"
#include "AST.h"
#include "SqlDocument.h"
#include "IMainWindow.h"

namespace Sql {

struct SqlBindValue {
    SqlBindValue( int type_ = 0, const QString &value_ = "" )
        : type(type_)
        , value(value_)
    {}

    int     type;
    QString value;
};

class SQL_EXPORT SqlBindHelper
{
public:
    SqlBindHelper();

    bool defineBindValues( const QSet<QString> &bindNames, QMap<QString, SqlBindValue> &bindings );

private:
    QMap<QString, SqlBindValue> bindingsHistory;
};

} // Sql

#endif // SQLBINDHELPER_H
