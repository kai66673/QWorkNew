#ifndef DBPOSTGRESDATASOURCEPROVIDER_H
#define DBPOSTGRESDATASOURCEPROVIDER_H

#include "DataSource.h"
#include "DataSourceAssistor.h"

class DbPostgresDataSourceProvider : public RPML::IDataSourceProvider
{
public:
    DbPostgresDataSourceProvider();

    virtual int typeId() const { return 1; }
    virtual QString typeDescription() const { return QStringLiteral("Db Postgres"); }
    virtual RPML::IDataSourceConnectWidget *createConnectWidget( QWidget *parent = 0 ) const;
    virtual RPML::IDataSourceQuery *createDataSourceQuery() const;

    virtual bool connect( RPML::DataSource *source, QString &errorMessage ) const;
    virtual void disconnect( RPML::DataSource *source ) const;

    virtual RPML::IDataSourceAssistor *createDataSourceAssistor( RPML::DataSource *source ) const;
};

#endif // DBPOSTGRESDATASOURCEPROVIDER_H
