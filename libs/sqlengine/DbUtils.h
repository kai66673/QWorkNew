#ifndef DBUTILS_H
#define DBUTILS_H

#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>

#include "DbLoader.h"
#include "DbCatalog.h"

#include "sqlengine_global.h"
#include "LogManager.h"

namespace Database {

class SQL_EXPORT Utils
{
public:

    static QString loadString( BaseConnectionInfo *connectionInfo, const MetadataQuery *q );

    template <class T> static bool loadMetadata( BaseConnectionInfo *connectionInfo, QList<IDbCatalogItem *> *objects, IDbCatalogItem *parent, const MetadataQuery *q ) {
        if ( !connectionInfo || !q )
            return false;
        switch ( q->columnCount ) {
            case 1: return loadMetadata1<T>(connectionInfo, objects, parent, q);
            case 2: return loadMetadata2<T>(connectionInfo, objects, parent, q);
        }
        return false;
    }

    template <class T> static bool loadMetadata1( BaseConnectionInfo *connectionInfo, QList<IDbCatalogItem *> *objects, IDbCatalogItem *parent, const MetadataQuery *q ) {
        if ( !connectionInfo || !q )
            return false;

        if ( DbAccessor::Ptr accessor = connectionInfo->accessor() ) {
            if ( DbQuery::Ptr query = accessor->query() ) {
                if ( query->exec(q->queryText, true) ) {
                    while ( query->next() ) {
                        IDbCatalogItem *object = new T(query->value(q->firstColumnIndex).toString(), parent);
                        for (int i = 0; i < q->properties.size(); i++)
                            object->setProperty( q->properties[i].second,
                                                 query->value(q->properties[i].first) );
                        objects->append(object);
                    }

                    return true;
                }
            }
        }

        return false;
    }

    template <class T> static bool loadMetadata2( BaseConnectionInfo *connectionInfo, QList<IDbCatalogItem *> *objects, IDbCatalogItem *parent, const MetadataQuery *q ) {
        if ( !connectionInfo || !q )
            return false;

        if ( DbAccessor::Ptr accessor = connectionInfo->accessor() ) {
            if ( DbQuery::Ptr query = accessor->query() ) {
                if ( query->exec(q->queryText, true) ) {
                    while ( query->next() ) {
                        IDbCatalogItem *object = new T(query->value(q->firstColumnIndex).toString(), parent, query->value(q->firstColumnIndex + 1).toString());
                        for (int i = 0; i < q->properties.size(); i++)
                            object->setProperty( q->properties[i].second,
                                                 query->value(q->properties[i].first) );
                        objects->append(object);
                    }

                    return true;
                }
            }
        }

        return false;
    }

    template <class T> static bool loadDatabaseMetadata( BaseConnectionInfo *connectionInfo, QList<IDbCatalogItem *> *objects, DbDatabaseCategoryItem *parent, const MetadataQuery *q ) {
        if ( !connectionInfo || !q )
            return false;

        if ( DbAccessor::Ptr accessor = connectionInfo->accessor() ) {
            if ( DbQuery::Ptr query = accessor->query() ) {
                if ( query->exec(q->queryText, true) ) {
                    while ( query->next() ) {
                        IDbCatalogItem *object = new T(query->value(q->firstColumnIndex).toString(), parent);
                        for (int i = 0; i < q->properties.size(); i++)
                            object->setProperty( q->properties[i].second,
                                                 query->value(q->properties[i].first) );
                        objects->append(object);
                    }

                    return true;
                }
            }
        }

        return false;
    }

    static bool executeQueryWithLog( QSqlQuery *query, const QString &queryString = "" );
};

} // Database

#endif // DBUTILS_H
