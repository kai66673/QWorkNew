#ifndef DBSQLITEDATASOURCEPROVIDER_H
#define DBSQLITEDATASOURCEPROVIDER_H

#include "DataSource.h"

class DbSqliteDataSourceProvider : public RPML::IDataSourceProvider
{
public:
    DbSqliteDataSourceProvider();

    virtual int typeId() const { return 2; }
    virtual QString typeDescription() const { return QStringLiteral("Db SQLITE"); }
    virtual RPML::IDataSourceConnectWidget *createConnectWidget( QWidget *parent = 0 ) const;
    virtual RPML::IDataSourceQuery *createDataSourceQuery() const;

    virtual bool connect( RPML::DataSource *source, QString &errorMessage ) const;
    virtual void disconnect( RPML::DataSource *source ) const;

    virtual RPML::IDataSourceAssistor *createDataSourceAssistor( RPML::DataSource *source ) const;
};

#endif // DBSQLITEDATASOURCEPROVIDER_H
