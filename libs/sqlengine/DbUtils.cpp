#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

#include "DbUtils.h"

namespace Database {

QString Utils::loadString ( BaseConnectionInfo *connectionInfo, const MetadataQuery *q )
{
    if ( !connectionInfo || !q )
        return QStringLiteral("");

    if ( DbAccessor::Ptr accessor = connectionInfo->accessor() ) {
        if ( DbQuery::Ptr query = accessor->query() ) {
            if ( query->exec(q->queryText, true) ) {
                if ( query->next() )
                    return query->value(q->firstColumnIndex).toString();
            }
        }
    }

    return QStringLiteral("");
}

bool Utils::executeQueryWithLog( QSqlQuery *query, const QString &queryString )
{
    QString txt = queryString;
    if ( txt.isEmpty() )
        txt = query->lastQuery();

    bool queryResult;
    if ( queryString.isEmpty() )
        queryResult = query->exec();
    else
        queryResult = query->exec(queryString);

    if ( queryResult )
        Core::LogManager::addMessage( Core::LogMessage( QObject::tr("Query executed:\n%1").arg(txt),
                                                        QObject::tr("Sql.Spy"), Core::LogMessage::Information, QColor(192, 255, 255) ) );
    else
        Core::LogManager::addMessage( Core::LogMessage( QObject::tr("Query:\n%1\nExecuted with Error:\n%2").arg(txt).arg(query->lastError().text()),
                                                        QObject::tr("Sql.Spy"), Core::LogMessage::Information, QColor(192, 31, 31) ) );

    return queryResult;
}

} // Database
