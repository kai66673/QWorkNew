#ifndef DBLINTERDATASOURCEPROVIDER_H
#define DBLINTERDATASOURCEPROVIDER_H

#include "DataSource.h"
#include "DataSourceAssistor.h"

class DbLinterDataSourceProvider : public RPML::IDataSourceProvider
{
public:
    DbLinterDataSourceProvider();

    virtual int typeId() const { return 3; }
    virtual QString typeDescription() const { return QStringLiteral("Db Linter"); }
    virtual RPML::IDataSourceConnectWidget *createConnectWidget( QWidget *parent = 0 ) const;
    virtual RPML::IDataSourceQuery *createDataSourceQuery() const;

    virtual bool connect( RPML::DataSource *source, QString &errorMessage ) const;
    virtual void disconnect( RPML::DataSource *source ) const;

    virtual RPML::IDataSourceAssistor *createDataSourceAssistor( RPML::DataSource *source ) const;
};

#endif // DBLINTERDATASOURCEPROVIDER_H
