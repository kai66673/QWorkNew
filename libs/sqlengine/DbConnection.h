#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <QMainWindow>
#include <QQueue>
#include <QFuture>
#include <QFutureWatcher>
#include <QObject>
#include <QTabWidget>

#include "sqlengine_global.h"
#include "DbLoader.h"
#include "DbCatalog.h"
#include "SqlBindHelper.h"
#include "IMainWindow.h"

namespace Database {

struct DbJob;
struct DbJobItem;

struct SQL_EXPORT DbFilter {
    enum FilterType {
        NoFilter = 0,
        StartsWith,
        EndsWith,
        Contains
    };

    DbFilter( const QString &filterText_ = "", FilterType filterType_ = NoFilter )
        : filterText(filterText_)
        , filterType(filterType_)
    {}

    QString filterText;
    FilterType filterType;
};

class SQL_EXPORT DbConnection: public QObject
{
    Q_OBJECT

public:
    DbConnection( IDbLoader *loader_, BaseConnectionInfo *connectionInfo_,
                  IDbCatalogRootItem *rootItem, DbCatalogModel *model, Sql::SqlBindHelper *sqlHelper );

    inline BaseConnectionInfo *connectionInfo() const { return m_connectionInfo; }
    inline QString connectionName() const { return m_databaseName; }
    inline QString connectionTitle() const { return m_connectionTitle; }
    inline IDbLoader *loader() const { return m_loader; }
    inline bool transaction() const { return m_transaction; }
    inline QString defaultSchemaName() const { return m_connectionInfo->defaultSchemaName(); }

    bool closeConnection( QMainWindow *mainWindow );
    bool confirmEndStartedTransaction( QMainWindow *mainWindow );

    bool startTransaction();
    bool commitTransaction();
    bool rollbackTransaction();

    QList<IDbCatalogItem *> *schemas() const { return m_rootItem->schemas(); }
    QList<IDbCatalogItem *> *schemaObjects( const QString &schemaName, unsigned objectType );
    QList<IDbCatalogItem *> *schemaObjectObjects( const QString &schemaName, const QString &objectName, unsigned objectType, unsigned objectSubtype );
    QList<IDbCatalogItem *> *tableColumns( const QString &schemaName, const QString &tableName );
    QList<IDbCatalogItem *> *viewColumns( const QString &schemaName, const QString &tableName );
    QList<IDbCatalogItem *> *tableConstraints( const QString &schemaName, const QString &tableName );
    QList<IDbCatalogItem *> *tableTriggers( const QString &schemaName, const QString &tableName );
    QList<IDbCatalogItem *> *tableIndexes( const QString &schemaName, const QString &tableName );

    bool noJobs() const;
    void startNewJob( ExecuteType executeType );
    void startQueryDataNewJob( const QString &schemaName, const QString &tableName );

    void waitDetailsForFinished();

    inline DbFilter *filter() { return &m_filter; }

    inline void setDbModel( DbCatalogModel *model ) { m_model = model; }
    inline void setDbRootItem( IDbCatalogRootItem *rootItem ) { m_rootItem = rootItem; }

    void requestDetails( IDbCatalogItem *item );

signals:
    void statementExecuted( const QString &queryString, bool success, bool isSelectStattement, const QString &message, QList <QList <QVariant> > *queryData );
    void transactionChanged( DbConnection *connection );
    void connectionClosed( DbConnection *connection );
    void detailsRequested( IDbCatalogItem *item, DbConnection *connection );

public slots:
    void onJobItemExecuted();

private:
    IDbLoader *m_loader;
    bool m_transaction;
    BaseConnectionInfo *m_connectionInfo;

    QString m_databaseName;
    QString m_connectionTitle;

    bool hasTransactionFeature;

    IDbCatalogRootItem *m_rootItem;
    DbCatalogModel *m_model;

    QQueue<DbJob *> m_jobs;
    QFuture<DbJobItem *> m_jobItemFuture;
    QFutureWatcher<DbJobItem *> m_jobItemFutureWatcher;
    bool m_failSaveTransaction;

    Sql::SqlBindHelper *m_sqlHelper;
    void wake();
    bool isTransactionStatement( Sql::SqlStatement *statement );

    QFuture <IDbDetailsFactory *> m_detailsFuture;

    DbFilter m_filter;
};

} // Database

Q_DECLARE_METATYPE(QList<QList <QVariant> > *)

#endif // DBCONNECTION_H
