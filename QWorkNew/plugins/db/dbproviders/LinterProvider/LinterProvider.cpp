#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QAction>

#include <QDebug>

#include "Core.h"
#include "LinterProvider.h"
#include "LinterAuthWidget.h"
#include "DbUtils.h"
#include "DbCatalog.h"
#include "DbLoader.h"
#include "linter_sql_constants.h"

namespace Database {
namespace Linter {

Provider::Provider( QObject *parent )
    : QObject(parent)
{ }

Provider::~Provider()
{ }

BaseConnectionInfo *Provider::createConnection( const QString &connName, const QStringList &parameters ) const
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QLINTER", connName);
    db.setDatabaseName(parameters[0]);
    if ( !parameters[1].isEmpty() )
        db.setPort(parameters[1].toInt());
    db.setUserName(parameters[2]);
    db.setPassword(parameters[3]);

    if ( !db.open() ) {
        QMessageBox::critical( Core::Storage::mainWindow()->widget(),
                               tr("Cannot open database"),
                               tr("Unable to establish a database connection.\nClick Cancel to exit."),
                               QMessageBox::Cancel );
        return 0;
    }

    QString hostName = parameters[0].isEmpty() ? "[Default]" : parameters[0];

    QString title = hostName + "(" + parameters[2] + ")";

    QString descr;
    descr  = "Connection type: [Q]LINTER\n";
    descr += "  Database Name: " + hostName + "\n";
    descr += "           Port: " + parameters[1] + "\n";
    descr += "      User Name: " + parameters[2];

    return new BaseConnectionInfo(connName, title, descr, parameters[2].toUpper());
}

IDbConnectWidget *Provider::createAuthWidget( QWidget *parent ) const
{
    return new LinterAuthWidget(parent);
}

MetadataQuery::Ptr Provider::schemasMetadataQuery() const
{
    return MetadataQuery::create("select username from all_users where user_id > 0 and username <> '' order by username");
}

MetadataQuery::Ptr Provider::objectsMetadataQuery( const QString &schemaName, int schemaObjectType ) const
{
    switch ( schemaObjectType ) {

        case Sql::Constants::DbSchemaObjectType::C_TABLE:
            return MetadataQuery::create("select table_name from all_tables where owner = \'" + schemaName.toUpper() + "\' order by table_name");

        case Sql::Constants::DbSchemaObjectType::C_VIEW:
            return MetadataQuery::create("SELECT object_name FROM all_objects WHERE owner = '" + schemaName.toUpper() + "' and object_type = 'VIEW' ORDER BY object_name");

        case Sql::Constants::DbSchemaObjectType::C_INDEX:
            return MetadataQuery::create("SELECT index_name, table_name FROM all_indexes WHERE owner = '" + schemaName.toUpper() + "' ORDER BY index_name", 0, 2);

        case Sql::Constants::DbSchemaObjectType::C_TRIGGER:
            return MetadataQuery::create("SELECT trigger_name, table_name FROM sys.all_triggers WHERE owner = '" + schemaName.toUpper() + "' ORDER BY trigger_name", 0, 2);

        case Sql::Constants::DbSchemaObjectType::C_CONSTRAINT:
            return MetadataQuery::create("select constraint_name, table_name from all_constraints where owner = '" + schemaName.toUpper() + "' order by constraint_name", 0, 2);

        case Sql::Constants::DbSchemaObjectType::C_PROCEDURE:
            return MetadataQuery::create("select procedure_name from system.procedures where procedure_schem = '" + schemaName.toUpper() + "' and procedure_name not like 'Trigger #%' order by procedure_name");
    }

    return MetadataQuery::Ptr(0);
}

MetadataQuery::Ptr Provider::subobjectsMetadataQuery( const QString &schemaName, int schemaObjectType, const QString &objName, int schemaObjectSubtype ) const
{
    switch ( schemaObjectType ) {

        case Sql::Constants::DbSchemaObjectType::C_TABLE:
            switch ( schemaObjectSubtype ) {
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_COLUMN:
                    return MetadataQuery::create("select column_name, table_name from all_tab_columns where owner = '" + schemaName.toUpper() + "' and table_name = '" + objName.toUpper() + "' order by column_id", 0, 2);
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_INDEX:
                    return MetadataQuery::create("select index_name, table_name from all_indexes where owner = '" + schemaName.toUpper() + "' and table_name = '" + objName.toUpper() + "' order by index_name", 0, 2);
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_TRIGGER:
                    return MetadataQuery::create("SELECT trigger_name, table_name FROM sys.all_triggers WHERE owner = '" + schemaName.toUpper() + "' and table_name = '" + objName.toUpper() + "' ORDER BY trigger_name");
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_CONSTRAINT:
                    return MetadataQuery::create("select constraint_name, table_name from all_constraints where owner = '" + schemaName.toUpper() + "' and table_name = '" + objName.toUpper() + "' order by constraint_name", 0, 2);
                default:
                    return MetadataQuery::Ptr(0);
            }

        case Sql::Constants::DbSchemaObjectType::C_VIEW:
            switch ( schemaObjectSubtype ) {
                case Sql::Constants::DbSchemaObjectSubtype::C_VIEW_COLUMN:
                    return MetadataQuery::create("select column_name, table_name from all_tab_columns where owner = '" + schemaName.toUpper() + "' and table_name = '" + objName.toUpper() + "' order by column_id", 0, 2);
                default:
                    return MetadataQuery::Ptr(0);
            }
    }

    return MetadataQuery::Ptr(0);
}

MetadataQuery::Ptr Provider::objectTableNameMetadataQuery( const QString &schemaName, int schemaObjectType, const QString &objName ) const
{
    switch ( schemaObjectType ) {

        case Sql::Constants::DbSchemaObjectType::C_INDEX:
            return MetadataQuery::create("SELECT table_name FROM all_indexes WHERE owner = '" + schemaName.toUpper() + "' and index_name = '" + objName.toUpper() + "'");

        case Sql::Constants::DbSchemaObjectType::C_TRIGGER:
            return MetadataQuery::create("SELECT table_name FROM sys.all_triggers WHERE owner = '" + schemaName.toUpper() + "' and trigger_name = '" + objName.toUpper() + "'");

        case Sql::Constants::DbSchemaObjectType::C_CONSTRAINT:
            return MetadataQuery::create("select table_name from all_constraints where owner = '" + schemaName.toUpper() + "' and constraint_name = '" + objName.toUpper() + "'");
    }

    return MetadataQuery::Ptr(0);
}

QStringList Provider::additionalSchemaCategories() const
{
    return QStringList() << QObject::tr("Procedures");
}

QString Provider::additionalSchemaCategoryIconName( int schemaObjectType ) const
{
    switch ( schemaObjectType ) {
        case Sql::Constants::DbSchemaObjectType::C_PROCEDURE: return QStringLiteral(":/images/dbb_funcproc_list.png");
    }

    return QStringLiteral("");
}

QString Provider::additionalSchemaObjectIconName( int schemaObjectType , ItemProperties * ) const
{
    switch ( schemaObjectType ) {
        case Sql::Constants::DbSchemaObjectType::C_PROCEDURE: return QStringLiteral(":/images/dbb_function.png");
    }

    return QStringLiteral("");
}

QStringList Provider::additionalSchemaObjectCategories( unsigned schemaObjectType ) const
{
    Q_UNUSED(schemaObjectType)
    return QStringList();
}

QString Provider::additionalSchemaObjectCategoryIconName( int schemaObjectType, int schemaObjectSubtype ) const
{
    Q_UNUSED(schemaObjectType)
    Q_UNUSED(schemaObjectSubtype)
    return QStringLiteral("");
}

QString Provider::additionalSchemaSubobjectIconName(int schemaObjectType, int schemaObjectSubtype) const
{
    Q_UNUSED(schemaObjectType)
    Q_UNUSED(schemaObjectSubtype)
    return QStringLiteral("");
}

static IDbDetailsFactory *createTableDbDetailsFactory( const QString &connName, const QString &schemaName, const QString &tableName )
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
        queryString = QString( "SELECT comments from all_tab_comments where owner = '%1' and table_name = '%2'").arg(schemaName.toUpper()).arg(tableName.toUpper());
        QString description = "";
        if ( Database::Utils::executeQueryWithLog(&query, queryString) )
            if ( query.next() )
                description = query.value(0).toString().trimmed();
        t->addItem(Database::DbDetailsFormViewFactory::FormItem(QObject::tr("Description"), description));
    }

    // Table Columns Info
    Database::DbDetailsTableFactory *c = new Database::DbDetailsTableFactory( QStringList() << QObject::tr("Name")
                                                                                            << QObject::tr("Data Type")
                                                                                            << QObject::tr("Data Length")
                                                                                            << QObject::tr("Data Precision")
                                                                                            << QObject::tr("Nullable") );
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = QString( "SELECT column_name, data_type, data_length, data_precision, nullable "
                               "  FROM all_tab_columns "
                               " where owner = '%1' "
                               "   and table_name = '%2' "
                               " order by column_id" ).arg(schemaName.toUpper()).arg(tableName.toUpper());
        if ( Database::Utils::executeQueryWithLog(&query, queryString) )
            while ( query.next() ) {
                QStringList row;
                row << query.value(0).toString();
                row << query.value(1).toString();
                row << query.value(2).toString();
                row << query.value(3).toString();
                row << query.value(4).toString();
                c->addRow(row);
            }
    }

    // Table Indexes Info
    Database::DbDetailsTableWithTableFactory *in = new Database::DbDetailsTableWithTableFactory( QStringList() << QObject::tr("Name")
                                                                                                               << QObject::tr("Unique"),
                                                                                                 QStringList() << QObject::tr("Column") );
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = QString( "select index_name, uniqueness from all_indexes "
                               " where owner = '%1' and table_name = '%2' order by index_name")
                .arg(schemaName.toUpper()).arg(tableName.toUpper());
        if ( Database::Utils::executeQueryWithLog(&query, queryString) ) {
            QList<QStringList> data;
            while ( query.next() ) {
                QStringList item;
                item << query.value(0).toString().trimmed();
                item << query.value(1).toString().trimmed();
                data << item;
            }
            for (int i = 0; i < data.size(); i++) {
                QList<QStringList> indColumns;
                queryString = "select column_name from all_ind_columns where index_owner = '" +schemaName.toUpper() +
                              "' and index_name = '" + data[i][0].toUpper() + "' order by column_position";
                if ( Database::Utils::executeQueryWithLog(&query, queryString) ) {
                    while ( query.next() ) {
                        QStringList item;
                        item << query.value(0).toString();
                        indColumns << item;
                    }
                }
                in->addRow(data[i], indColumns);
            }
        }
    }

    // Table Triggers Info
    Database::DbDetailsTableWithSourceFactory *tr = new Database::DbDetailsTableWithSourceFactory( QStringList() << QObject::tr("Name")
                                                                                                                 << QObject::tr("Type")
                                                                                                                 << QObject::tr("Event") );
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = QString( "select trigger_name, trigger_type, triggering_event from sys.all_triggers "
                               " where owner = '%1' and table_name = '%2' order by trigger_name" )
                .arg(schemaName.toUpper()).arg(tableName.toUpper());
        if ( Database::Utils::executeQueryWithLog(&query, queryString) ) {
            QList<QStringList> data;
            while ( query.next() ) {
                QStringList item;
                item << query.value(0).toString().trimmed();
                item << query.value(1).toString().trimmed();
                item << query.value(2).toString().trimmed();
                data << item;
            }
            for (int i = 0; i < data.size(); i++) {
                queryString = "select text from all_source where owner = \'" + schemaName.toUpper() +
                              "\' and type = \'TRIGGER\' and name = \'" + data[i][0].toUpper() + "\' order by \"LINE\"";
                QString triggerSource = "";
                if ( Database::Utils::executeQueryWithLog(&query, queryString) ) {
                    while ( query.next() )
                        triggerSource += query.value(0).toString();
                }
                tr->addRow(data[i], triggerSource);
            }
        }
    }

    // Table Constraints Info
    Database::DbDetailsTableWithSourceFactory *cn = new Database::DbDetailsTableWithSourceFactory( QStringList() << QObject::tr("Name")
                                                                                                                 << QObject::tr("Type") );
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = QString( "select constraint_name, constraint_type from all_constraints"
                               " where owner = '%1' and table_name = '%2' order by constraint_name" )
                .arg(schemaName.toUpper()).arg(tableName.toUpper());
        if ( Database::Utils::executeQueryWithLog(&query, queryString) ) {
            QList<QStringList> data;
            while ( query.next() ) {
                QStringList item;
                QString   constraintType;
                item << query.value(0).toString().trimmed();
                constraintType = query.value(1).toString().trimmed();
                if ( constraintType == QLatin1String("R") ) {
                    item << QObject::tr("Referential");
                }
                else if ( constraintType == QLatin1String("U") ) {
                    item << QObject::tr("Unique");
                }
                else if ( constraintType == QLatin1String("P") ) {
                    item << QObject::tr("Primary Key");
                }
                else if ( constraintType == QLatin1String("C") ) {
                    item << QObject::tr("Conditional");
                }
                else if ( constraintType == QLatin1String("O") ) {
                    item << QObject::tr("Read Only");
                }
                else item << "";
                data << item;
            }
            for (int i = 0; i < data.size(); i++) {
                QString constraintSource = "";
                queryString = "select search_condition from all_constraints where owner = \'" + schemaName.toUpper() +
                        "\' and constraint_name = '" + data[i][0].toUpper() + "'";
                if ( Database::Utils::executeQueryWithLog(&query, queryString) ) {
                    if ( query.next() ) {
                        constraintSource = query.value(0).toString();
                    }
                }
                cn->addRow(data[i], constraintSource);
            }
        }
    }

    Database::DbDetailsTabbedFactory *tab = new Database::DbDetailsTabbedFactory;
    tab->addTab(QObject::tr("Table Info"), t);
    tab->addTab(QObject::tr("Columns Info"), c);
    tab->addTab(QObject::tr("Indexes"), in);
    tab->addTab(QObject::tr("Triggers"), tr);
    tab->addTab(QObject::tr("Constraints"), cn);

    return tab;
}

static IDbDetailsFactory *createViewDbDetailsFactory( const QString &connName, const QString &schemaName, const QString &viewName )
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
        QString   queryString = QString( "select comments from all_tab_comments where owner = '%1' and table_name = '%2'" )
                .arg(schemaName.toUpper()).arg(viewName.toUpper());
        QString remark = "";
        if ( Database::Utils::executeQueryWithLog(&query, queryString) )
            if ( query.next() )
                remark = query.value(0).toString();
        t->addItem(Database::DbDetailsFormViewFactory::FormItem( QObject::tr("Description"), remark ));
    }

    // Table Columns Info
    Database::DbDetailsTableFactory *c = new Database::DbDetailsTableFactory( QStringList() << QObject::tr("Name")
                                                                                            << QObject::tr("Data Type")
                                                                                            << QObject::tr("Data Length")
                                                                                            << QObject::tr("Data Precision")
                                                                                            << QObject::tr("Nullable") );
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = QString( "SELECT column_name, data_type, data_length, data_precision, nullable "
                               "  FROM all_tab_columns "
                               " where owner = '%1' "
                               "   and table_name = '%2' "
                               " order by column_id" ).arg(schemaName.toUpper()).arg(viewName.toUpper());
        if ( Database::Utils::executeQueryWithLog(&query, queryString) )
            while ( query.next() ) {
                QStringList row;
                row << query.value(0).toString();
                row << query.value(1).toString();
                row << query.value(2).toString();
                row << query.value(3).toString();
                row << query.value(4).toString();
                c->addRow(row);
            }
    }

    Database::DbDetailsTabbedFactory *tab = new Database::DbDetailsTabbedFactory;
    tab->addTab(QObject::tr("View Info"), t);
    tab->addTab(QObject::tr("Used Columns"), c);

    return tab;
}

static IDbDetailsFactory *createSourceDbDetailsFactory( const QString &connName, const QString &schemaName, const QString &funcName )
{
    QSqlDatabase db = QSqlDatabase::database(connName);

    if ( !db.isOpen() ) {
        Core::Storage::warning(QObject::tr("Database connection is not opened."));
        return 0;
    }

    QString funcSource = "";
    QSqlQuery query(db);
    QString   queryString;

    queryString = "select text from all_source where owner = \'" + schemaName.toUpper() +
            "' and type = 'PROCEDURE' and name = '" + funcName.toUpper() + "' order by \"LINE\"";
    if ( Database::Utils::executeQueryWithLog(&query, queryString) )
        while ( query.next() )
            funcSource += query.value(0).toString();

    Database::DbDetailsSourceFactory *s = new Database::DbDetailsSourceFactory(funcSource);
    return s;
}

static IDbDetailsFactory *createSequenceDbDetailsFactory( const QString &connName, const QString &schemaName, const QString &seqName )
{
    QSqlDatabase db = QSqlDatabase::database(connName);

    if ( !db.isOpen() ) {
        Core::Storage::warning(QObject::tr("Database connection is not opened."));
        return 0;
    }

    Database::DbDetailsFormViewFactory *t = new Database::DbDetailsFormViewFactory;

    QString data_type = "";
    QString increment = "";
    QString minimum_value = "";
    QString maximum_value = "";
    QString start_value = "";
    QString cycle_option = "";

    QSqlQuery query(db);
    QString   queryString;
    queryString = QString( "SELECT data_type, increment, minimum_value, maximum_value, start_value, cycle_option "
                           "  FROM information_schema.sequences "
                           " WHERE sequence_schema = '%1' "
                           "   AND sequence_name = '%2'" ).arg(schemaName).arg(seqName);
    if ( Database::Utils::executeQueryWithLog(&query, queryString) ) {
        if ( query.next() ) {
            data_type       = query.value(0).toString();
            increment       = query.value(1).toString();
            minimum_value   = query.value(2).toString();
            maximum_value   = query.value(3).toString();
            start_value     = query.value(4).toString();
            cycle_option    = query.value(5).toString();
        }
    }

    t->addItem( Database::DbDetailsFormViewFactory::FormItem(QObject::tr("Name"), seqName) );
    t->addItem( Database::DbDetailsFormViewFactory::FormItem(QObject::tr("Data Type"), data_type) );
    t->addItem( Database::DbDetailsFormViewFactory::FormItem(QObject::tr("Increment"), increment) );
    t->addItem( Database::DbDetailsFormViewFactory::FormItem(QObject::tr("Minimum"), minimum_value) );
    t->addItem( Database::DbDetailsFormViewFactory::FormItem(QObject::tr("Maximum"), maximum_value) );
    t->addItem( Database::DbDetailsFormViewFactory::FormItem(QObject::tr("Start"), start_value) );
    t->addItem( Database::DbDetailsFormViewFactory::FormItem(QObject::tr("Sycled"), cycle_option) );

    return t;
}

IDbDetailsFactory *Provider::createDbDetailsFactory( const QString &connName, const QString &schemaName, int schemaObjectType,
                                                     int schemaObjectSubtype, const QString &objName ) const
{
    Q_UNUSED(schemaName)
    Q_UNUSED(schemaObjectSubtype)

    switch ( schemaObjectType ) {
        case Sql::Constants::DbSchemaObjectType::C_TABLE:               return createTableDbDetailsFactory(connName, schemaName, objName );
        case Sql::Constants::DbSchemaObjectType::C_VIEW:                return createViewDbDetailsFactory(connName, schemaName, objName);
        case Sql::Constants::DbSchemaObjectType::C_PROCEDURE:           return createSourceDbDetailsFactory(connName, schemaName, objName);
    }

    return 0;
}

#ifndef HAVE_QT5
Q_EXPORT_PLUGIN2(Provider, Provider)
#endif

} // Linter
} // Database
