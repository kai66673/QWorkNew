#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMimeData>

#include <QDebug>

#include "DbPostgresDataSourceProvider.h"
#include "DbPostgresDataSourceConnectWidget.h"

DbPostgresDataSourceProvider::DbPostgresDataSourceProvider()
{
}

RPML::IDataSourceConnectWidget *DbPostgresDataSourceProvider::createConnectWidget( QWidget *parent ) const
{
    return new DbPostgresDataSourceConnectWidget(parent);
}

RPML::IDataSourceQuery *DbPostgresDataSourceProvider::createDataSourceQuery() const
{
    return new RPML::DbDataSourceQuery();
}

bool DbPostgresDataSourceProvider::connect( RPML::DataSource *source, QString &errorMessage ) const
{
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", source->connectName());
        QList<RPML::DataSource::ConnectParameter> * connectParameters = source->connectParameters();

        for (int i = 0; i < connectParameters->size(); i++) {
            if ( connectParameters->at(i).name == QLatin1String("database") ) {
                db.setDatabaseName(connectParameters->at(i).value);
            } else if ( connectParameters->at(i).name == QLatin1String("host") ) {
                db.setHostName(connectParameters->at(i).value);
            } else if ( connectParameters->at(i).name == QLatin1String("port") ) {
                db.setPort(connectParameters->at(i).value.toInt());
            } else if ( connectParameters->at(i).name == QLatin1String("user") ) {
                db.setUserName(connectParameters->at(i).value);
            } else if ( connectParameters->at(i).name == QLatin1String("password") ) {
                db.setPassword(connectParameters->at(i).value);
            }
        }

        if ( db.open() )
            return true;
    }

    QSqlDatabase::removeDatabase(source->connectName());
    errorMessage = QObject::tr("Unable to establish a database connection");
    return false;
}

void DbPostgresDataSourceProvider::disconnect( RPML::DataSource *source ) const
{
    QSqlDatabase::removeDatabase(source->connectName());
}

namespace {

static void fillSchemas( RPML::DataSourceObjectItem *parent, const QString &connectName )
{
    QSqlDatabase db = QSqlDatabase::database(connectName);

    if ( db.isOpen() ) {
        QSqlQuery query(db);
        query.exec("SELECT schemata.schema_name FROM information_schema.schemata WHERE schemata.schema_owner <> 'postgres'");
        while ( query.next() )
            parent->childrens.push_back( new RPML::DataSourceObjectItem(1, query.value(0).toString(), true, false, parent) );
    }
}

static void fillTablesAndViews( RPML::DataSourceObjectItem *parent, const QString &connectName )
{
    QSqlDatabase db = QSqlDatabase::database(connectName);

    if ( db.isOpen() ) {
        QSqlQuery query(db);

        query.exec("SELECT pg_tables.tablename FROM pg_catalog.pg_tables WHERE pg_tables.schemaname = '" +
                   parent->name + "' ORDER BY pg_tables.tablename");
        while ( query.next() )
            parent->childrens.push_back( new RPML::DataSourceObjectItem(2, query.value(0).toString(), true, true, parent) );

        query.clear();

        query.exec("SELECT pg_views.viewname FROM pg_catalog.pg_views WHERE pg_views.schemaname = '" +
                   parent->name + "' ORDER BY pg_views.viewname");
        while ( query.next() )
            parent->childrens.push_back( new RPML::DataSourceObjectItem(3, query.value(0).toString(), true, true, parent) );
    }
}

static void fillTableColumns( RPML::DataSourceObjectItem *parent, const QString &connectName )
{
    QSqlDatabase db = QSqlDatabase::database(connectName);

    if ( db.isOpen() ) {
        QSqlQuery query(db);
        query.exec("SELECT columns.column_name, columns.data_type FROM information_schema.columns WHERE columns.table_schema = '" +
                   parent->parent->name + "' AND columns.table_name = '" +
                   parent->name + "' ORDER BY columns.ordinal_position");
        while ( query.next() )
            parent->childrens.push_back( new RPML::DataSourceObjectItem(4, query.value(0).toString(), false, false, parent) );
    }
}

static void fillViewColumns( RPML::DataSourceObjectItem *parent, const QString &connectName )
{
    QSqlDatabase db = QSqlDatabase::database(connectName);

    if ( db.isOpen() ) {
        QSqlQuery query(db);
        query.exec("SELECT view_column_usage.column_name FROM information_schema.view_column_usage WHERE view_column_usage.view_schema = '" +
                   parent->parent->name + "' AND view_column_usage.view_name = '" + parent->name + "'");
        while ( query.next() )
            parent->childrens.push_back( new RPML::DataSourceObjectItem(4, query.value(0).toString(), false, false, parent) );
    }
}

class DbPostgresAssistor: public RPML::IDataSourceAssistor
{
public:
    DbPostgresAssistor( const QString &connectName )
        : RPML::IDataSourceAssistor()
        , m_connectName(connectName)
    {
    }

    virtual void loadChildrens( RPML::DataSourceObjectItem *parent ) {
        switch ( parent->type ) {
            case 0: fillSchemas(parent, m_connectName);         break;
            case 1: fillTablesAndViews(parent, m_connectName);  break;
            case 2: fillTableColumns(parent, m_connectName);    break;
            case 3: fillViewColumns(parent, m_connectName);     break;
        }
    }

    virtual QString iconName( int type ) const {
        switch ( type ) {
            case 1: return QStringLiteral(":/images/db_ds_schema");
            case 2: return QStringLiteral(":/images/db_ds_table");
            case 3: return QStringLiteral(":/images/db_ds_view");
            case 4: return QStringLiteral(":/images/db_ds_column");
        }
        return QString();
    }

private:
    QString m_connectName;
};

}

RPML::IDataSourceAssistor *DbPostgresDataSourceProvider::createDataSourceAssistor( RPML::DataSource *source ) const
{
    return new DbPostgresAssistor(source->connectName());
}


