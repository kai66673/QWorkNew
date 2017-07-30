#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QAction>
#include <QFileInfo>
#include <QDebug>

#include "Core.h"
#include "SqliteProvider.h"
#include "SqliteAuthWidget.h"
#include "DbUtils.h"
#include "DbCatalog.h"
#include "sql_constants.h"
#include "DbLoader.h"

namespace Database {
namespace SQLITE {

Provider::Provider( QObject *parent )
    : QObject(parent)
{
}

BaseConnectionInfo *Provider::createConnection( const QString &connName, const QStringList &parameters ) const
{
    QFileInfo fi(parameters[0]);
    if ( !fi.exists() ) {
        QMessageBox::critical( Core::Storage::mainWindow()->widget(),
                               tr("Cannot open database"),
                               tr("Unable to establish a database connection:\nFile \"%1\" not Exists").arg(parameters[0]) );
        return 0;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connName);
    db.setDatabaseName(parameters[0]);

    if ( !db.open() ) {
        QMessageBox::critical( Core::Storage::mainWindow()->widget(),
                               tr("Cannot open database"),
                               tr("Unable to establish a database connection.\nClick Cancel to exit."),
                               QMessageBox::Cancel );
        return 0;
    }

    QString title = fi.fileName();

    QString descr;
    descr  = "Connection type: [Q]SQLITE\n";
    descr += "  Database file: " + parameters[0];

    return new BaseConnectionInfo(connName, title, descr);
}

IDbConnectWidget *Provider::createAuthWidget( QWidget *parent ) const
{
    return new SqliteAuthWidget(parent);
}

MetadataQuery::Ptr Provider::schemasMetadataQuery() const
{
    return MetadataQuery::Ptr(0);
}

MetadataQuery::Ptr Provider::objectsMetadataQuery( const QString &schemaName, int schemaObjectType ) const
{
    Q_UNUSED(schemaName)

    switch ( schemaObjectType ) {

        case Sql::Constants::DbSchemaObjectType::C_TABLE:
            return MetadataQuery::create("SELECT name FROM sqlite_master WHERE type = \'table\' ORDER BY name");

        case Sql::Constants::DbSchemaObjectType::C_VIEW:
            return MetadataQuery::create("SELECT name FROM sqlite_master WHERE type = \'view\' ORDER BY name");

        case Sql::Constants::DbSchemaObjectType::C_INDEX:
            return MetadataQuery::create("SELECT name, tbl_name FROM sqlite_master WHERE type = \'index\' ORDER BY name", 0, 2);

        case Sql::Constants::DbSchemaObjectType::C_TRIGGER:
            return MetadataQuery::create("SELECT name, tbl_name FROM sqlite_master WHERE type = \'trigger\' ORDER BY name", 0, 2);

        case Sql::Constants::DbSchemaObjectType::C_CONSTRAINT:
            /// TODO: How!?;
            return MetadataQuery::Ptr(0);
    }

    return MetadataQuery::Ptr(0);
}

MetadataQuery::Ptr Provider::subobjectsMetadataQuery( const QString &schemaName, int schemaObjectType, const QString &objName, int schemaObjectSubtype ) const
{
    Q_UNUSED(schemaName)

    switch ( schemaObjectType ) {

        case Sql::Constants::DbSchemaObjectType::C_TABLE:
            switch ( schemaObjectSubtype ) {
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_COLUMN:
                    return MetadataQuery::create("pragma table_info(\'" + objName + "\')", 1, 2);
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_INDEX:
                    return MetadataQuery::create("SELECT name FROM sqlite_master WHERE type = \'index\' AND upper(tbl_name) = \'" + objName.toUpper() + "\' ORDER BY name");
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_TRIGGER:
                    return MetadataQuery::create("SELECT name FROM sqlite_master WHERE type = \'trigger\' AND upper(tbl_name) = \'" + objName.toUpper() + "\' ORDER BY name");
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_CONSTRAINT:
                    /// TODO: How!?
                    return MetadataQuery::Ptr(0);
                default:
                    return MetadataQuery::Ptr(0);
            }

        case Sql::Constants::DbSchemaObjectType::C_VIEW:
            switch ( schemaObjectSubtype ) {
                case Sql::Constants::DbSchemaObjectSubtype::C_VIEW_COLUMN:
                    return MetadataQuery::create("pragma table_info(\'" + objName + "\')", 1, 2);
                default:
                    return MetadataQuery::Ptr(0);
            }
    }

    return MetadataQuery::Ptr(0);
}

MetadataQuery::Ptr Provider::objectTableNameMetadataQuery( const QString &schemaName, int schemaObjectType, const QString &objName ) const
{
    Q_UNUSED(schemaName)

    switch ( schemaObjectType ) {

        case Sql::Constants::DbSchemaObjectType::C_INDEX:
            return MetadataQuery::create("SELECT tbl_name FROM sqlite_master WHERE type = \'index\' AND upper(name) = \'" + objName.toUpper() + "\' ORDER BY name");

        case Sql::Constants::DbSchemaObjectType::C_TRIGGER:
            return MetadataQuery::create("SELECT tbl_name FROM sqlite_master WHERE type = \'trigger\' AND upper(name) = \'" + objName.toUpper() + "\' ORDER BY name");

        case Sql::Constants::DbSchemaObjectType::C_CONSTRAINT:
            /// TODO: How!?
            return MetadataQuery::Ptr(0);
    }

    return MetadataQuery::Ptr(0);
}

static IDbDetailsFactory *createTableDbDetailsFactory( const QString &connName, const QString &tableName )
{
    QSqlDatabase db = QSqlDatabase::database(connName);

    if ( !db.isOpen() ) {
        Core::Storage::warning(QObject::tr("Database connection is not opened."));
        return 0;
    }

    // Table Info
    Database::DbDetailsFormViewFactory *t = new Database::DbDetailsFormViewFactory;
    {
        t->addItem(Database::DbDetailsFormViewFactory::FormItem(QObject::tr("Name"), tableName));
        QSqlQuery query(db);
        QString   queryString;
        queryString = "SELECT sql FROM sqlite_master WHERE type in (\'table\', \'view\') AND UPPER(name) = \'" + tableName.toUpper() + "\'";
        QString source = "";
        if ( Database::Utils::executeQueryWithLog(&query, queryString) )
            if ( query.next() )
                source = query.value(0).toString();
        t->addItem(Database::DbDetailsFormViewFactory::FormItem( QObject::tr("Source"), source, Database::DbDetailsFormViewFactory::SourceText ));
    }

    // Table Columns Info
    Database::DbDetailsTableFactory *c = new Database::DbDetailsTableFactory( QStringList() << QObject::tr("Name")
                                                                                            << QObject::tr("Type")
                                                                                            << QObject::tr("Nullable")
                                                                                            << QObject::tr("Default Value")
                                                                                            << QObject::tr("Primary Key") );
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = "pragma table_info(\'" + tableName + "\')";
        if ( Database::Utils::executeQueryWithLog(&query, queryString) )
            while ( query.next() ) {
                QStringList row;
                row << query.value(1).toString();
                row << query.value(2).toString();
                row << (query.value(3).toInt() == 0 ? QObject::tr("Yes") : QObject::tr("No"));
                row << query.value(4).toString();
                row << (query.value(5).toInt() == 0 ? QObject::tr("No") : QObject::tr("Yes"));
                c->addRow(row);
            }
    }

    // Table Indexes Info
    Database::DbDetailsTableWithTableFactory *in = new Database::DbDetailsTableWithTableFactory( QStringList() << QObject::tr("Name")
                                                                                                               << QObject::tr("Unique"),
                                                                                                 QStringList() << QObject::tr("Column")
                                                                                                               << QObject::tr("Position") );
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = "pragma index_list(\'" + tableName.toUpper() + "\')";
        if ( Database::Utils::executeQueryWithLog(&query, queryString) ) {
            QList<QStringList> data;
            while ( query.next() ) {
                QStringList item;
                item << query.value(1).toString();
                item << (query.value(2).toInt() == 0 ? QObject::tr("No") : QObject::tr("Yes"));
                data << item;
            }
            for (int i = 0; i < data.size(); i++) {
                QList<QStringList> indColumns;
                queryString = "pragma index_info(\'" + data[i][0].toUpper() + "\')";
                query.exec(queryString);
                while ( query.next() ) {
                    QStringList item;
                    item << query.value(2).toString();
                    item << query.value(0).toString();
                    indColumns << item;
                }
                in->addRow(data[i], indColumns);
            }
        }
    }

    // Table Triggers Info
    Database::DbDetailsTableWithSourceFactory *tr = new Database::DbDetailsTableWithSourceFactory( QStringList() << QObject::tr("Name") );
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = "select name from sqlite_master where tbl_name = \'" + tableName + "\' and type = \'trigger\' order by name";
        if ( Database::Utils::executeQueryWithLog(&query, queryString) ) {
            QList<QStringList> data;
            while ( query.next() ) {
                QStringList item;
                item << query.value(0).toString();
                data << item;
            }
            for (int i = 0; i < data.size(); i++) {
                queryString = "SELECT sql FROM sqlite_master WHERE UPPER(name) = \'" + data[i][0].toUpper() + "\' AND type = \'trigger\' ";
                query.exec(queryString);
                QString triggerSource = "";
                if ( query.next() ) {
                    triggerSource = query.value(0).toString();
                }
                tr->addRow(data[i], triggerSource);
            }
        }
    }

    // Table Constraints Info
    Database::DbDetailsTableWithSourceFactory *cn = new Database::DbDetailsTableWithSourceFactory( QStringList() << QObject::tr("Name")
                                                                                                                 << QObject::tr("Type") );
    {
        /// TODO: How!!?
    }

    Database::DbDetailsTabbedFactory *tab = new Database::DbDetailsTabbedFactory;
    tab->addTab(QObject::tr("Table Info"), t);
    tab->addTab(QObject::tr("Columns Info"), c);
    tab->addTab(QObject::tr("Indexes"), in);
    tab->addTab(QObject::tr("Triggers"), tr);
    tab->addTab(QObject::tr("Constraints"), cn);

    return tab;
}

static IDbDetailsFactory *createViewDbDetailsFactory( const QString &connName, const QString &viewName )
{
    QSqlDatabase db = QSqlDatabase::database(connName);

    if ( !db.isOpen() ) {
        Core::Storage::warning(QObject::tr("Database connection is not opened."));
        return 0;
    }

    // View Info
    Database::DbDetailsFormViewFactory *t = new Database::DbDetailsFormViewFactory;
    {
        t->addItem(Database::DbDetailsFormViewFactory::FormItem(QObject::tr("Name"), viewName));
        QSqlQuery query(db);
        QString   queryString;
        queryString = "SELECT sql FROM sqlite_master WHERE type in (\'table\', \'view\') AND UPPER(name) = \'" + viewName.toUpper() + "\'";
        QString source = "";
        if ( Database::Utils::executeQueryWithLog(&query, queryString) )
            if ( query.next() )
                source = query.value(0).toString();
        t->addItem(Database::DbDetailsFormViewFactory::FormItem( QObject::tr("Source"), source, Database::DbDetailsFormViewFactory::SourceText ));
    }

    // Table Columns Info
    Database::DbDetailsTableFactory *c = new Database::DbDetailsTableFactory( QStringList() << QObject::tr("Name")
                                                                                            << QObject::tr("Type")
                                                                                            << QObject::tr("Nullable")
                                                                                            << QObject::tr("Default Value")
                                                                                            << QObject::tr("Primary Key") );
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = "pragma table_info(\'" + viewName + "\')";
        if ( Database::Utils::executeQueryWithLog(&query, queryString) )
            while ( query.next() ) {
                QStringList row;
                row << query.value(1).toString();
                row << query.value(2).toString();
                row << (query.value(3).toInt() == 0 ? QObject::tr("Yes") : QObject::tr("No"));
                row << query.value(4).toString();
                row << (query.value(5).toInt() == 0 ? QObject::tr("No") : QObject::tr("Yes"));
                c->addRow(row);
            }
    }

    Database::DbDetailsTabbedFactory *tab = new Database::DbDetailsTabbedFactory;
    tab->addTab(QObject::tr("View Info"), t);
    tab->addTab(QObject::tr("Columns Info"), c);

    return tab;
}

IDbDetailsFactory *Provider::createDbDetailsFactory( const QString &connName, const QString &schemaName, int schemaObjectType,
                                                     int schemaObjectSubtype, const QString &objName ) const
{
    Q_UNUSED(schemaName)
    Q_UNUSED(schemaObjectSubtype)

    switch ( schemaObjectType ) {
        case Sql::Constants::DbSchemaObjectType::C_TABLE:   return createTableDbDetailsFactory(connName, objName );
        case Sql::Constants::DbSchemaObjectType::C_VIEW:    return createViewDbDetailsFactory(connName, objName);
    }

    return 0;
}

#ifndef HAVE_QT5
Q_EXPORT_PLUGIN2(Provider, Provider)
#endif

} // SQLITE
} // Database
