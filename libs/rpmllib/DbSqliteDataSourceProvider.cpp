#include <QSqlDatabase>
#include <QSqlQuery>

#include "DbSqliteDataSourceProvider.h"
#include "DbSqliteDataSourceConnectWidget.h"

DbSqliteDataSourceProvider::DbSqliteDataSourceProvider()
{
}

RPML::IDataSourceConnectWidget *DbSqliteDataSourceProvider::createConnectWidget( QWidget *parent ) const
{
    return new DbSqliteDataSourceConnectWidget(parent);
}

RPML::IDataSourceQuery *DbSqliteDataSourceProvider::createDataSourceQuery() const
{
    return new RPML::DbDataSourceQuery();
}

bool DbSqliteDataSourceProvider::connect( RPML::DataSource *source, QString &errorMessage ) const
{
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", source->connectName());
        QList<RPML::DataSource::ConnectParameter> * connectParameters = source->connectParameters();

        for (int i = 0; i < connectParameters->size(); i++) {
            if ( connectParameters->at(i).name == QLatin1String("database") ) {
                db.setDatabaseName(connectParameters->at(i).value);
            }
        }

        if ( db.open() )
            return true;
    }

    QSqlDatabase::removeDatabase(source->connectName());
    errorMessage = QObject::tr("Unable to establish a database connection");
    return false;
}

void DbSqliteDataSourceProvider::disconnect( RPML::DataSource *source ) const
{
    QSqlDatabase::removeDatabase(source->connectName());
}

namespace {

static void fillTablesAndViews( RPML::DataSourceObjectItem *parent, const QString &connectName )
{
    QSqlDatabase db = QSqlDatabase::database(connectName);

    if ( db.isOpen() ) {
        QSqlQuery query(db);

        query.exec("SELECT name FROM sqlite_master WHERE type = 'table' ORDER BY name");
        while ( query.next() )
            parent->childrens.push_back( new RPML::DataSourceObjectItem(1, query.value(0).toString(), true, true, parent) );

        query.clear();

        query.exec("SELECT name FROM sqlite_master WHERE type = 'view' ORDER BY name");
        while ( query.next() )
            parent->childrens.push_back( new RPML::DataSourceObjectItem(2, query.value(0).toString(), true, true, parent) );
    }
}

static void fillTableColumns( RPML::DataSourceObjectItem *parent, const QString &connectName )
{
    QSqlDatabase db = QSqlDatabase::database(connectName);

    if ( db.isOpen() ) {
        QSqlQuery query(db);
        query.exec("pragma table_info('" + parent->name + "')");
        while ( query.next() )
            parent->childrens.push_back( new RPML::DataSourceObjectItem(3, query.value(1).toString(), false, false, parent) );
    }
}

class DbSqliteAssistor: public RPML::IDataSourceAssistor
{
public:
    DbSqliteAssistor( const QString &connectName )
        : RPML::IDataSourceAssistor()
        , m_connectName(connectName)
    {}

    virtual void loadChildrens( RPML::DataSourceObjectItem *parent ) {
        switch ( parent->type ) {
            case 0: fillTablesAndViews(parent, m_connectName);  break;
            case 1: fillTableColumns(parent, m_connectName);    break;
            case 2: fillTableColumns(parent, m_connectName);    break;
        }
    }

    virtual QString iconName( int type ) const {
        switch ( type ) {
            case 1: return QStringLiteral(":/images/db_ds_table");
            case 2: return QStringLiteral(":/images/db_ds_view");
            case 3: return QStringLiteral(":/images/db_ds_column");
        }
        return QString();
    }

private:
    QString m_connectName;
};

}

RPML::IDataSourceAssistor *DbSqliteDataSourceProvider::createDataSourceAssistor( RPML::DataSource *source ) const
{
    return new DbSqliteAssistor(source->connectName());
}


