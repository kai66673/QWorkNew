#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMimeData>

#include <QDebug>

#include "DbLinterDataSourceProvider.h"
#include "DbLinterDataSourceConnectWidget.h"

DbLinterDataSourceProvider::DbLinterDataSourceProvider()
{
}

RPML::IDataSourceConnectWidget *DbLinterDataSourceProvider::createConnectWidget( QWidget *parent ) const
{
    return new DbLinterDataSourceConnectWidget(parent);
}

RPML::IDataSourceQuery *DbLinterDataSourceProvider::createDataSourceQuery() const
{
    return new RPML::DbDataSourceQuery();
}

bool DbLinterDataSourceProvider::connect( RPML::DataSource *source, QString &errorMessage ) const
{
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QLINTER", source->connectName());
        QList<RPML::DataSource::ConnectParameter> * connectParameters = source->connectParameters();

        for (int i = 0; i < connectParameters->size(); i++) {
            if ( connectParameters->at(i).name == QLatin1String("database") ) {
                db.setDatabaseName(connectParameters->at(i).value);
            } else if ( connectParameters->at(i).name == QLatin1String("host") ) {
                if ( !connectParameters->at(i).value.isEmpty() ) {
                    db.setPort(connectParameters->at(i).value.toInt());
                }
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

void DbLinterDataSourceProvider::disconnect( RPML::DataSource *source ) const
{
    QSqlDatabase::removeDatabase(source->connectName());
}

namespace {

static void fillSchemas( RPML::DataSourceObjectItem *parent, const QString &connectName )
{
    QSqlDatabase db = QSqlDatabase::database(connectName);

    if ( db.isOpen() ) {
        QSqlQuery query(db);
        query.exec("select username from all_users where user_id > 0 and username <> '' order by username");
        while ( query.next() )
            parent->childrens.push_back( new RPML::DataSourceObjectItem(1, query.value(0).toString().trimmed(), true, false, parent) );
    }
}

static void fillTablesAndViews( RPML::DataSourceObjectItem *parent, const QString &connectName )
{
    QSqlDatabase db = QSqlDatabase::database(connectName);

    if ( db.isOpen() ) {
        QSqlQuery query(db);

        query.exec("SELECT object_name FROM all_objects WHERE owner = '" + parent->name.toUpper() + "' and object_type = 'TABLE' ORDER BY object_name");
        while ( query.next() )
            parent->childrens.push_back( new RPML::DataSourceObjectItem(2, query.value(0).toString().trimmed(), true, true, parent) );

        query.clear();

        query.exec("SELECT object_name FROM all_objects WHERE owner = '" + parent->name.toUpper() + "' and object_type = 'TABLE' ORDER BY object_name");
        while ( query.next() )
            parent->childrens.push_back( new RPML::DataSourceObjectItem(3, query.value(0).toString().trimmed(), true, true, parent) );
    }
}

static void fillColumns( RPML::DataSourceObjectItem *parent, const QString &connectName )
{
    QSqlDatabase db = QSqlDatabase::database(connectName);

    if ( db.isOpen() ) {
        QSqlQuery query(db);
        query.exec("select column_name, table_name from all_tab_columns where owner = '" + parent->parent->name.toUpper() + "' and table_name = '" + parent->name.toUpper() + "' order by column_id");
        while ( query.next() )
            parent->childrens.push_back( new RPML::DataSourceObjectItem(4, query.value(0).toString().trimmed(), false, false, parent) );
    }
}

class DbLinterAssistor: public RPML::IDataSourceAssistor
{
public:
    DbLinterAssistor( const QString &connectName )
        : RPML::IDataSourceAssistor()
        , m_connectName(connectName)
    {
    }

    virtual void loadChildrens( RPML::DataSourceObjectItem *parent ) {
        switch ( parent->type ) {
            case 0: fillSchemas(parent, m_connectName);         break;
            case 1: fillTablesAndViews(parent, m_connectName);  break;
            case 2: fillColumns(parent, m_connectName);         break;
            case 3: fillColumns(parent, m_connectName);         break;
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

RPML::IDataSourceAssistor *DbLinterDataSourceProvider::createDataSourceAssistor( RPML::DataSource *source ) const
{
    return new DbLinterAssistor(source->connectName());
}


