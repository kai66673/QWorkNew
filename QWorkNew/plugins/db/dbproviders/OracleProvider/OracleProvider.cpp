#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QAction>
#include <QFileInfo>
#include <QDebug>

#include "Core.h"
#include "OracleProvider.h"
#include "OracleParserEngine.h"
#include "OracleAuthWidget.h"
#include "DbUtils.h"
#include "oracle_sql_constants.h"

namespace Database {
namespace Oracle {

Provider::Provider( QObject *parent )
    : QObject(parent)
    , m_engine(new ::Oracle::OracleParserEngine())
{
}

Provider::~Provider()
{
    delete m_engine;
}

BaseConnectionInfo *Provider::createConnection( const QString &connName, const QStringList &parameters ) const
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QOCI", connName);
    db.setDatabaseName(parameters[0]);
    db.setUserName(parameters[1]);
    db.setPassword(parameters[2]);

    if ( !db.open() ) {
        QMessageBox::critical( Core::Storage::mainWindow()->widget(),
                               tr("Cannot open database"),
                               tr("Unable to establish a database connection.\nClick Cancel to exit."),
                               QMessageBox::Cancel );
        return 0;
    }

    QString title = parameters[0] + "(" + parameters[1] + ")";

    QString descr;
    descr  = "Connection type: Oracle ([Q]OCI)\n";
    descr += "Database source: " + parameters[0] + "\n";
    descr += "      User name: " + parameters[1];

    return new BaseConnectionInfo(connName, title, descr, parameters[1].toUpper());
}

IDbConnectWidget *Provider::createAuthWidget( QWidget *parent ) const
{
    return new OracleAuthWidget(parent);
}

MetadataQuery::Ptr Provider::schemasMetadataQuery() const
{
    return MetadataQuery::create("select username from dba_users where account_status = \'OPEN\' order by username");
}

MetadataQuery::Ptr Provider::objectsMetadataQuery( const QString &schemaName, int schemaObjectType ) const
{
    switch ( schemaObjectType ) {

        case Sql::Constants::DbSchemaObjectType::C_TABLE:
            return MetadataQuery::create("select table_name from all_tables where owner = \'" + schemaName.toUpper() + "\' order by table_name");

        case Sql::Constants::DbSchemaObjectType::C_VIEW:
            return MetadataQuery::create("select view_name from all_views where owner = \'" + schemaName.toUpper() + "\' order by view_name");

        case Sql::Constants::DbSchemaObjectType::C_INDEX:
            return MetadataQuery::create("select index_name, table_name from all_indexes where owner = \'" + schemaName.toUpper() + "\' order by index_name", 0, 2);

        case Sql::Constants::DbSchemaObjectType::C_TRIGGER:
            return MetadataQuery::create("select trigger_name, table_name from all_triggers where owner = \'" + schemaName.toUpper() + "\' order by trigger_name", 0, 2);

        case Sql::Constants::DbSchemaObjectType::C_CONSTRAINT:
            return MetadataQuery::create("select constraint_name, table_name from all_constraints where owner = \'" + schemaName.toUpper() + "\' order by constraint_name", 0, 2);

        case Sql::Constants::DbSchemaObjectType::C_PACKAGE:
            return MetadataQuery::create("select object_name from all_objects where owner = \'" + schemaName.toUpper() + "\' and object_type = \'PACKAGE\' order by 1");

        case Sql::Constants::DbSchemaObjectType::C_PACKAGE_BODY:
            return MetadataQuery::create("select object_name from all_objects where owner = \'" + schemaName.toUpper() + "\' and object_type = \'PACKAGE BODY\' order by 1");

        case Sql::Constants::DbSchemaObjectType::C_FUNCTION:
            return MetadataQuery::create("select object_name from all_objects where owner = \'" + schemaName.toUpper() + "\' and object_type = \'FUNCTION\' order by 1");

        case Sql::Constants::DbSchemaObjectType::C_PROCEDURE:
            return MetadataQuery::create("select object_name from all_objects where owner = \'" + schemaName.toUpper() + "\' and object_type = \'PROCEDURE\' order by 1");
    }

    return MetadataQuery::Ptr(0);
}

MetadataQuery::Ptr Provider::subobjectsMetadataQuery( const QString &schemaName, int schemaObjectType, const QString &objName, int schemaObjectSubtype ) const
{
    switch ( schemaObjectType ) {

        case Sql::Constants::DbSchemaObjectType::C_TABLE:
            switch ( schemaObjectSubtype ) {
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_COLUMN:
                    return MetadataQuery::create( "select column_name, data_type from all_tab_columns where owner = \'" + schemaName.toUpper() +
                                                  "\' and table_name = \'" + objName.toUpper() + "\' order by column_id", 0, 2 );
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_INDEX:
                    return MetadataQuery::create( "select index_name from all_indexes where owner = \'" + schemaName.toUpper() +
                                                  "\' and table_name = \'" + objName.toUpper() +"\' order by index_name") ;
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_TRIGGER:
                    return MetadataQuery::create( "select trigger_name from all_triggers where owner = \'" + schemaName.toUpper() +
                                                  "\' and table_name = \'" + objName.toUpper() + "\' order by trigger_name" );
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_CONSTRAINT:
                    return MetadataQuery::create( "select constraint_name from all_constraints where owner = \'" + schemaName.toUpper() +
                                                  "\' and table_name = \'" + objName.toUpper() + "\' order by constraint_name");
                default:
                    return MetadataQuery::Ptr(0);
            }

        case Sql::Constants::DbSchemaObjectType::C_VIEW:
            switch ( schemaObjectSubtype ) {
                case Sql::Constants::DbSchemaObjectSubtype::C_VIEW_COLUMN:
                    return MetadataQuery::create( "select column_name, data_type from all_tab_columns where owner = \'" + schemaName.toUpper() +
                                                  "\' and table_name = \'" + objName.toUpper() + "\' order by column_id", 0, 2 );
                default:
                    return MetadataQuery::Ptr(0);
            }

        case Sql::Constants::DbSchemaObjectType::C_PACKAGE:
            switch (schemaObjectSubtype) {
                case Sql::Constants::DbSchemaObjectSubtype::C_PACKAGE_FUNCTION:
                    return MetadataQuery::create( QString( "select p.procedure_name from all_procedures p\n"
                                                           "  left join user_arguments a on p.object_id = a.object_id\n"
                                                           "   and p.procedure_name = a.object_name and a.position = 0\n"
                                                           " where p.owner = \'%1\' and p.object_name = \'%2\'\n"
                                                           "   and p.procedure_name is not null\n"
                                                           "   and a.object_id is not null\n"
                                                           " order by p.subprogram_id").arg(schemaName.toUpper()).arg(objName.toUpper() ) );
                case Sql::Constants::DbSchemaObjectSubtype::C_PACKAGE_PROCEDURE:
                    return MetadataQuery::create( QString( "select p.procedure_name from all_procedures p\n"
                                                           "  left join user_arguments a on p.object_id = a.object_id\n"
                                                           "   and p.procedure_name = a.object_name and a.position = 0\n"
                                                           " where p.owner = \'%1\' and p.object_name = \'%2\'\n"
                                                           "   and p.procedure_name is not null\n"
                                                           "   and a.object_id is null\n"
                                                           " order by p.subprogram_id").arg(schemaName.toUpper()).arg(objName.toUpper() ) );
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
            return MetadataQuery::create("select table_name from all_indexes where owner = \'" + schemaName.toUpper() + "\' and index_name = \'" + objName.toUpper() + "\'");

        case Sql::Constants::DbSchemaObjectType::C_TRIGGER:
            return MetadataQuery::create("select table_name from all_triggers where owner = \'" + schemaName.toUpper() + "\' and trigger_name = \'" + objName.toUpper() + "\'");

        case Sql::Constants::DbSchemaObjectType::C_CONSTRAINT:
            return MetadataQuery::create("select table_name from all_constraints where owner = \'" + schemaName.toUpper() + "\' and constraint_name = \'" + objName.toUpper() + "\'");
    }

    return MetadataQuery::Ptr(0);
}

QStringList Provider::additionalSchemaCategories() const
{
    return QStringList() << QObject::tr("Packages")
                         << QObject::tr("Package Bodies")
                         << QObject::tr("Functions")
                         << QObject::tr("Procedures");
}

QString Provider::additionalSchemaCategoryIconName( int schemaObjectType ) const
{
    switch ( schemaObjectType ) {
        case Sql::Constants::DbSchemaObjectType::C_PACKAGE:         return QStringLiteral(":/images/dbb_modules.png");
        case Sql::Constants::DbSchemaObjectType::C_PACKAGE_BODY:    return QStringLiteral(":/images/dbb_modules.png");
        case Sql::Constants::DbSchemaObjectType::C_FUNCTION:        return QStringLiteral(":/images/dbb_funcproc_list.png");
        case Sql::Constants::DbSchemaObjectType::C_PROCEDURE:       return QStringLiteral(":/images/dbb_funcproc_list.png");
    }

    return QStringLiteral("");
}

QString Provider::additionalSchemaObjectIconName( int schemaObjectType , ItemProperties * ) const
{
    switch ( schemaObjectType ) {
        case Sql::Constants::DbSchemaObjectType::C_PACKAGE:         return QStringLiteral(":/images/dbb_module.png");
        case Sql::Constants::DbSchemaObjectType::C_PACKAGE_BODY:    return QStringLiteral(":/images/dbb_module.png");
        case Sql::Constants::DbSchemaObjectType::C_FUNCTION:        return QStringLiteral(":/images/dbb_function.png");
        case Sql::Constants::DbSchemaObjectType::C_PROCEDURE:       return QStringLiteral(":/images/dbb_procedure.png");
    }

    return QStringLiteral("");
}

QStringList Provider::additionalSchemaObjectCategories( unsigned schemaObjectType ) const
{
    if ( schemaObjectType == Sql::Constants::DbSchemaObjectType::C_PACKAGE )
        return QStringList() << QObject::tr("Functions") << QObject::tr("Procedures");
    return QStringList();
}

QString Provider::additionalSchemaObjectCategoryIconName (int schemaObjectType, int schemaObjectSubtype ) const
{
    if ( schemaObjectType == Sql::Constants::DbSchemaObjectType::C_PACKAGE ) {
        switch ( schemaObjectSubtype ) {
            case Sql::Constants::DbSchemaObjectSubtype::C_PACKAGE_FUNCTION:  return QStringLiteral(":/images/dbb_funcproc_list.png");
            case Sql::Constants::DbSchemaObjectSubtype::C_PACKAGE_PROCEDURE: return QStringLiteral(":/images/dbb_funcproc_list.png");
        }
    }

    return QStringLiteral("");
}

QString Provider::additionalSchemaSubobjectIconName( int schemaObjectType, int schemaObjectSubtype ) const
{
    if ( schemaObjectType == Sql::Constants::DbSchemaObjectType::C_PACKAGE ) {
        switch ( schemaObjectSubtype ) {
            case Sql::Constants::DbSchemaObjectSubtype::C_PACKAGE_FUNCTION:  return QStringLiteral(":/images/dbb_function.png");
            case Sql::Constants::DbSchemaObjectSubtype::C_PACKAGE_PROCEDURE: return QStringLiteral(":/images/dbb_procedure.png");
        }
    }

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
        QString   queryString = "select comments from all_tab_comments where owner = :schemaName and table_name = :tableName";
        query.prepare(queryString);
        query.bindValue(":schemaName", schemaName.toUpper());
        query.bindValue(":tableName", tableName.toUpper());
        QString remark = "";
        if ( Database::Utils::executeQueryWithLog(&query) )
            if ( query.next() )
                remark = query.value(0).toString();
        t->addItem(Database::DbDetailsFormViewFactory::FormItem( QObject::tr("Description"), remark ));
        queryString = "select DBMS_METADATA.GET_DDL(\'TABLE\', \'" + tableName.toUpper() + "\', \'" + schemaName.toUpper() + "\') from dual";
        QString source = "";
        if ( Database::Utils::executeQueryWithLog(&query, queryString) )
            if ( query.next() )
                source = query.value(0).toString();
        t->addItem(Database::DbDetailsFormViewFactory::FormItem( QObject::tr("Metadata DDL"), source,
                                                                 Database::DbDetailsFormViewFactory::SourceText ));
    }

    // Table Columns Info
    Database::DbDetailsTableFactory *c = new Database::DbDetailsTableFactory( QStringList() << QObject::tr("Name")
                                                                                            << QObject::tr("Type")
                                                                                            << QObject::tr("Length")
                                                                                            << QObject::tr("Description") );
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = " select t.column_name, t.data_type, t.data_length, t.data_precision, t.data_scale, c.comments from all_tab_columns t, all_col_comments c"
                      " where t.table_name = c.table_name and t.column_name = c.column_name and t.table_name = :tableName"
                      " and t.OWNER = c.owner and t.OWNER = :schemaName"
                      " order by t.table_name, t.column_id";
        query.prepare(queryString);
        query.bindValue(":schemaName", schemaName.toUpper());
        query.bindValue(":tableName", tableName.toUpper());
        if ( Database::Utils::executeQueryWithLog(&query) )
            while ( query.next() ) {
                QStringList row;
                QString fieldType;
                row << query.value(0).toString();
                fieldType = query.value(1).toString();
                if ( fieldType == QLatin1String("NUMBER") ) {
                    fieldType = QString("NUMBER(%1,%2)").arg(query.value(3).toInt()).arg(query.value(4).toInt());
                }
                row << fieldType;
                row << query.value(2).toString();
                row << query.value(5).toString();
                c->addRow(row);
            }
    }

    // Table Indexes Info
    Database::DbDetailsTableWithTableFactory *in = new Database::DbDetailsTableWithTableFactory( QStringList() << QObject::tr("Name")
                                                                                                               << QObject::tr("Unique")
                                                                                                               << QObject::tr("Compression"),
                                                                                                 QStringList() << QObject::tr("Column")
                                                                                                               << QObject::tr("Order") );
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = "select index_name, uniqueness, compression from all_indexes "
                      " where owner = :schemaName and table_name = :tableName order by index_name";
        query.prepare(queryString);
        query.bindValue(":schemaName", schemaName.toUpper());
        query.bindValue(":tableName", tableName.toUpper());
        if ( Database::Utils::executeQueryWithLog(&query) ) {
            QList<QStringList> data;
            while ( query.next() ) {
                QStringList item;
                item << query.value(0).toString().trimmed();
                item << (query.value(1).toString() == QLatin1String("UNIQUE") ? QObject::tr("Yes") : QObject::tr("No"));
                item << query.value(2).toString();
                data << item;
            }
            for (int i = 0; i < data.size(); i++) {
                QList<QStringList> indColumns;
                queryString = "select column_name, descend from all_ind_columns where index_owner = \'" +schemaName.toUpper() +
                              "\' and index_name = :indexName order by column_position";
                query.prepare(queryString);
                query.bindValue(":indexName", data[i][0].toUpper());
                query.exec();
                while ( query.next() ) {
                    QStringList item;
                    item << query.value(0).toString();
                    item << query.value(1).toString();
                    indColumns << item;
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
        queryString = "select trigger_name, trigger_type, triggering_event from all_triggers "
                      " where owner = :schemaName and table_name = :tableName order by trigger_name";
        query.prepare(queryString);
        query.bindValue(":schemaName", schemaName.toUpper());
        query.bindValue(":tableName", tableName.toUpper());
        if ( Database::Utils::executeQueryWithLog(&query) ) {
            QList<QStringList> data;
            while ( query.next() ) {
                QStringList item;
                item << query.value(0).toString();
                item << query.value(1).toString();
                item << query.value(2).toString();
                data << item;
            }
            for (int i = 0; i < data.size(); i++) {
                queryString = "select text from all_source where owner = \'" + schemaName.toUpper() +
                              "\' and type = \'TRIGGER\' and name = \'" + data[i][0].toUpper() + "\' order by line";
                query.exec(queryString);
                QString triggerSource = "";
                while ( query.next() ) {
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
        queryString = "select constraint_name, constraint_type from all_constraints"
                      " where owner = :schemaName and table_name = :tableName order by constraint_name";
        query.prepare(queryString);
        query.bindValue(":schemaName", schemaName.toUpper());
        query.bindValue(":tableName", tableName.toUpper());
        if ( Database::Utils::executeQueryWithLog(&query) ) {
            QList<QStringList> data;
            while ( query.next() ) {
                QStringList item;
                QString   constraintType;
                item << query.value(0).toString();
                constraintType = query.value(1).toString();
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
                        "\' and constraint_name = :constraintName";
                query.prepare(queryString);
                query.bindValue(":constraintName", data[i][0].toUpper());
                query.exec();
                if ( query.next() ) {
                    constraintSource = query.value(0).toString();
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
        QString   queryString = "select comments from all_tab_comments where owner = :schemaName and table_name = :tableName";
        query.prepare(queryString);
        query.bindValue(":schemaName", schemaName.toUpper());
        query.bindValue(":tableName", viewName.toUpper());
        QString remark = "";
        if ( Database::Utils::executeQueryWithLog(&query) )
            if ( query.next() )
                remark = query.value(0).toString();
        t->addItem(Database::DbDetailsFormViewFactory::FormItem( QObject::tr("Description"), remark ));
        queryString = "select DBMS_METADATA.GET_DDL(\'VIEW\', \'" + viewName.toUpper() + "\', \'" + schemaName.toUpper() + "\') from dual";
        QString source = "";
        if ( Database::Utils::executeQueryWithLog(&query, queryString) )
            if ( query.next() )
                source = query.value(0).toString();
        t->addItem(Database::DbDetailsFormViewFactory::FormItem( QObject::tr("Metadata DDL"), source,
                                                                 Database::DbDetailsFormViewFactory::SourceText ));
    }

    // View Columns Info
    Database::DbDetailsTableFactory *c = new Database::DbDetailsTableFactory( QStringList() << QObject::tr("Name")
                                                                                            << QObject::tr("Type")
                                                                                            << QObject::tr("Length")
                                                                                            << QObject::tr("Description") );
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = " select t.column_name, t.data_type, t.data_length, t.data_precision, t.data_scale, c.comments from all_tab_columns t, all_col_comments c"
                      " where t.table_name = c.table_name and t.column_name = c.column_name and t.table_name = :tableName"
                      " and t.OWNER = c.owner and t.OWNER = :schemaName"
                      " order by t.table_name, t.column_id";
        query.prepare(queryString);
        query.bindValue(":schemaName", schemaName.toUpper());
        query.bindValue(":tableName", viewName.toUpper());
        if ( Database::Utils::executeQueryWithLog(&query) )
            while ( query.next() ) {
                QStringList row;
                QString fieldType;
                row << query.value(0).toString();
                fieldType = query.value(1).toString();
                if ( fieldType == QLatin1String("NUMBER") ) {
                    fieldType = QString("NUMBER(%1,%2)").arg(query.value(3).toInt()).arg(query.value(4).toInt());
                }
                row << fieldType;
                row << query.value(2).toString();
                row << query.value(5).toString();
                c->addRow(row);
            }
    }

    Database::DbDetailsTabbedFactory *tab = new Database::DbDetailsTabbedFactory;
    tab->addTab(QObject::tr("View Info"), t);
    tab->addTab(QObject::tr("Columns Info"), c);

    return tab;

}

static IDbDetailsFactory *createFunctionDbDetailsFactory( const QString &connName, const QString &schemaName, const QString &funcName )
{
    QSqlDatabase db = QSqlDatabase::database(connName);

    if ( !db.isOpen() ) {
        Core::Storage::warning(QObject::tr("Database connection is not opened."));
        return 0;
    }

    QSqlQuery query(db);
    QString   queryString;

    queryString = "select text from all_source where owner = \'" + schemaName.toUpper() +
            "\' and type in (\'PROCEDURE\', \'FUNCTION\')" +
            " and name = :funcName order by line";
    query.prepare(queryString);
    query.bindValue(":funcName", funcName.toUpper());
    query.exec();
    QString funcSource = "";
    if ( Database::Utils::executeQueryWithLog(&query) )
        while ( query.next() )
            funcSource += query.value(0).toString();

    Database::DbDetailsSourceFactory *s = new Database::DbDetailsSourceFactory(funcSource);
    return s;
}

static IDbDetailsFactory *createPackageDbDetailsFactory( const QString &connName, const QString &schemaName, const QString &moduleName )
{
    QSqlDatabase db = QSqlDatabase::database(connName);

    if ( !db.isOpen() ) {
        Core::Storage::warning(QObject::tr("Database connection is not opened."));
        return 0;
    }

    QSqlQuery query(db);
    QString   queryString;

    queryString = "select text from all_source where owner = \'" + schemaName.toUpper() +
                  "\' and name = :moduleName and type = 'PACKAGE' order by line";
    query.prepare(queryString);
    query.bindValue(":moduleName", moduleName.toUpper());
    query.exec();
    QString packageSource = "";
    if ( Database::Utils::executeQueryWithLog(&query) )
        while ( query.next() )
            packageSource += query.value(0).toString();

    queryString = "select text from all_source where owner = \'" + schemaName.toUpper() +
                  "\' and name = :moduleName and type = 'PACKAGE BODY' order by line";
    query.prepare(queryString);
    query.bindValue(":moduleName", moduleName.toUpper());
    query.exec();
    QString packageBodySource = "";
    if ( Database::Utils::executeQueryWithLog(&query) )
        while ( query.next() )
            packageBodySource += query.value(0).toString();

    Database::DbDetailsSourceFactory *package       = new Database::DbDetailsSourceFactory(packageSource);
    Database::DbDetailsSourceFactory *packageBody   = new Database::DbDetailsSourceFactory(packageBodySource);

    Database::DbDetailsTabbedFactory *t = new Database::DbDetailsTabbedFactory();
    t->addTab(QObject::tr("Package"), package);
    t->addTab(QObject::tr("Package Body"), packageBody);

    return t;
}

IDbDetailsFactory *Provider::createDbDetailsFactory( const QString &connName, const QString &schemaName, int schemaObjectType,
                                                     int schemaObjectSubtype, const QString &objName ) const
{
    Q_UNUSED(schemaObjectSubtype)

    switch ( schemaObjectType ) {
        case Sql::Constants::DbSchemaObjectType::C_TABLE:           return createTableDbDetailsFactory(connName, schemaName, objName);
        case Sql::Constants::DbSchemaObjectType::C_VIEW:            return createViewDbDetailsFactory(connName, schemaName, objName);
        case Sql::Constants::DbSchemaObjectType::C_FUNCTION:        return createFunctionDbDetailsFactory(connName, schemaName, objName);
        case Sql::Constants::DbSchemaObjectType::C_PROCEDURE:       return createFunctionDbDetailsFactory(connName, schemaName, objName);
        case Sql::Constants::DbSchemaObjectType::C_PACKAGE:         return createPackageDbDetailsFactory(connName, schemaName, objName);
        case Sql::Constants::DbSchemaObjectType::C_PACKAGE_BODY:    return createPackageDbDetailsFactory(connName, schemaName, objName);
    }

    return 0;
}

#ifndef HAVE_QT5
Q_EXPORT_PLUGIN2(Provider, Provider)
#endif

} // Oracle
} // Database

