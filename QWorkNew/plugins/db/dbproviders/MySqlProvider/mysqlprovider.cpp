#include "Core.h"
#include "DbUtils.h"

#include "mysqlauthwidget.h"
#include "mysqlprovider.h"
#include "mysql_sql_constants.h"

namespace Database {
namespace MySql {

Provider::Provider(QObject *parent)
    : QObject(parent)
{ }

BaseConnectionInfo *Provider::createConnection(const QString &connName, const QStringList &parameters) const
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", connName);
    db.setDatabaseName(parameters[0]);
    db.setHostName(parameters[1]);
    db.setPort(parameters[2].toInt());
    db.setUserName(parameters[3]);
    db.setPassword(parameters[4]);

    if ( !db.open() ) {
        QMessageBox::critical( Core::Storage::mainWindow()->widget(),
                               tr("Cannot open database"),
                               tr("Unable to establish a database connection.\nClick Cancel to exit."),
                               QMessageBox::Cancel );
        return nullptr;
    }

    QString title = parameters[0] + "(" + parameters[3] + ")";

    QString descr;
    descr  = "Connection type: [Q]MYSQL\n";
    descr += "  Database Name: " + parameters[0] + "\n";
    descr += "           Host: " + parameters[1] + "\n";
    descr += "           Port: " + parameters[2] + "\n";
    descr += "      User Name: " + parameters[3];

    return new BaseConnectionInfo(connName, title, descr, parameters[0], true);
}

IDbConnectWidget *Provider::createAuthWidget(QWidget *parent) const
{
    return new MySqlAuthWidget(parent);
}

MetadataQuery::Ptr Provider::schemasMetadataQuery() const
{
    return MetadataQuery::create("SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME NOT IN ('information_schema', 'performance_schema')");
}

MetadataQuery::Ptr Provider::objectsMetadataQuery(const QString &schemaName, int schemaObjectType) const
{
    switch (schemaObjectType) {
        case Sql::Constants::DbSchemaObjectType::C_TABLE:
            return MetadataQuery::create("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = '"
                                         + schemaName + "' AND TABLE_TYPE <> 'VIEW' ORDER BY 1");
        case Sql::Constants::DbSchemaObjectType::C_VIEW:
            return MetadataQuery::create("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.VIEWS WHERE TABLE_SCHEMA = '"
                                         + schemaName + "' ORDER BY 1");
        case Sql::Constants::DbSchemaObjectType::C_INDEX:
            return MetadataQuery::create("SELECT DISTINCT INDEX_NAME, TABLE_NAME FROM INFORMATION_SCHEMA.STATISTICS WHERE TABLE_SCHEMA = '"
                                         + schemaName + "' AND INDEX_NAME <> 'PRIMARY' ORDER BY 1", 0, 2);
        case Sql::Constants::DbSchemaObjectType::C_TRIGGER:
            return MetadataQuery::create("SELECT TRIGGER_NAME, EVENT_OBJECT_TABLE FROM INFORMATION_SCHEMA.TRIGGERS WHERE TRIGGER_SCHEMA = '"
                                         + schemaName + "' ORDER BY 1", 0, 2);
        case Sql::Constants::DbSchemaObjectType::C_CONSTRAINT:
            return MetadataQuery::create("SELECT CONSTRAINT_NAME, TABLE_NAME FROM INFORMATION_SCHEMA.TABLE_CONSTRAINTS WHERE TABLE_SCHEMA = '"
                                         + schemaName + "' AND CONSTRAINT_NAME <> 'PRIMARY' ORDER BY 1", 0, 2);
        case Sql::Constants::DbSchemaObjectType::C_ROUTINE:
            return MetadataQuery::create("SELECT ROUTINE_NAME FROM INFORMATION_SCHEMA.ROUTINES WHERE ROUTINE_SCHEMA = '"
                                         + schemaName + "' ORDER BY 1");
    }

    return MetadataQuery::Ptr(nullptr);
}

MetadataQuery::Ptr Provider::subobjectsMetadataQuery(const QString &schemaName, int schemaObjectType, const QString &objName, int schemaObjectSubtype) const
{
    switch (schemaObjectType) {
        case Sql::Constants::DbSchemaObjectType::C_TABLE:
            switch (schemaObjectSubtype) {
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_COLUMN:
                    return MetadataQuery::create("SELECT COLUMN_NAME, DATA_TYPE  FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = '"
                                                 + schemaName + "' AND TABLE_NAME = '" + objName + "' ORDER BY ORDINAL_POSITION", 0, 2);
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_INDEX:
                    return MetadataQuery::create("SELECT INDEX_NAME FROM INFORMATION_SCHEMA.STATISTICS WHERE TABLE_SCHEMA = '"
                                                 + schemaName + "' AND TABLE_NAME = '" + objName + "' AND INDEX_NAME <> 'PRIMARY' ORDER BY 1");
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_TRIGGER:
                    return MetadataQuery::create("SELECT TRIGGER_NAME FROM INFORMATION_SCHEMA.TRIGGERS WHERE TRIGGER_SCHEMA = '"
                                                 + schemaName + "' AND EVENT_OBJECT_TABLE = '" + objName + "' ORDER BY 1");
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_CONSTRAINT:
                    return MetadataQuery::create("SELECT CONSTRAINT_NAME FROM INFORMATION_SCHEMA.TABLE_CONSTRAINTS WHERE TABLE_SCHEMA = '"
                                                 + schemaName + "' AND TABLE_NAME = '" + objName + "' AND CONSTRAINT_NAME <> 'PRIMARY' ORDER BY 1");
                default:
                    return MetadataQuery::Ptr(nullptr);
            }

        case Sql::Constants::DbSchemaObjectType::C_VIEW:
            switch (schemaObjectSubtype) {
                case Sql::Constants::DbSchemaObjectSubtype::C_VIEW_COLUMN:
                    return MetadataQuery::create("SELECT COLUMN_NAME, DATA_TYPE  FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_SCHEMA = '"
                                                 + schemaName + "' AND TABLE_NAME = '" + objName + "' ORDER BY ORDINAL_POSITION", 0, 2);
                default:
                    return MetadataQuery::Ptr(nullptr);
            }
    }

    return MetadataQuery::Ptr(nullptr);
}

MetadataQuery::Ptr Provider::objectTableNameMetadataQuery(const QString &schemaName, int schemaObjectType, const QString &objName) const
{
    switch ( schemaObjectType ) {

        case Sql::Constants::DbSchemaObjectType::C_INDEX:
            return MetadataQuery::create("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.STATISTICS WHERE TABLE_SCHEMA = '"
                                         + schemaName + "' AND INDEX_NAME = '" + objName + "'");

        case Sql::Constants::DbSchemaObjectType::C_TRIGGER:
            return MetadataQuery::create("SELECT EVENT_OBJECT_TABLE FROM INFORMATION_SCHEMA.TRIGGERS WHERE TRIGGER_SCHEMA = '"
                                         + schemaName + "' AND TRIGGER_NAME = '" + objName + "'");

        case Sql::Constants::DbSchemaObjectType::C_CONSTRAINT:
            return MetadataQuery::create("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLE_CONSTRAINTS WHERE TABLE_SCHEMA = '"
                                         + schemaName + "' AND CONSTRAINT_NAME = '" + objName + "'");
    }

    return MetadataQuery::Ptr(nullptr);
}

QStringList Provider::additionalSchemaCategories() const
{
    return QStringList() << QObject::tr("Routines");
}

QString Provider::additionalSchemaCategoryIconName(int schemaObjectType) const
{
    switch (schemaObjectType) {
        case Sql::Constants::DbSchemaObjectType::C_ROUTINE:
            return QStringLiteral(":/images/dbb_funcproc_list.png");
    }

    return QStringLiteral("");
}

QString Provider::additionalSchemaObjectIconName(int schemaObjectType, ItemProperties *properties) const
{
    switch (schemaObjectType) {
        case Sql::Constants::DbSchemaObjectType::C_ROUTINE:
            return QStringLiteral(":/images/dbb_function.png");
    }

    return QStringLiteral("");
}

static IDbDetailsFactory *createTableDbDetailsFactory(const QString &connName, const QString &schemaName, const QString &tableName)
{
    QSqlDatabase db = QSqlDatabase::database(connName);

    if ( !db.isOpen() ) {
        Core::Storage::warning(QObject::tr("Database connection is not opened."));
        return nullptr;
    }

    // Table Info
    Database::DbDetailsFormViewFactory *t = new Database::DbDetailsFormViewFactory;
    {
        t->addItem(Database::DbDetailsFormViewFactory::FormItem(QObject::tr("Name"), tableName));
        QSqlQuery query(db);
        QString   queryString;
        queryString = QString("SELECT TABLE_COMMENT FROM INFORMATION_SCHEMA.TABLES "
                              " WHERE TABLE_SCHEMA = '%1' "
                              "   AND TABLE_NAME = '%2'").arg(schemaName).arg(tableName);
        QString description = "";
        if (Database::Utils::executeQueryWithLog(&query, queryString))
            if ( query.next() )
                description = query.value(0).toString();
        t->addItem(Database::DbDetailsFormViewFactory::FormItem(QObject::tr("Description"), description));
    }

    // Table Columns Info
    Database::DbDetailsTableFactory *c = new Database::DbDetailsTableFactory(QStringList() << QObject::tr("Name")
                                                                                           << QObject::tr("Description")
                                                                                           << QObject::tr("Data Type")
                                                                                           << QObject::tr("Nullable") );
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = QString("SELECT COLUMN_NAME, COLUMN_COMMENT, COLUMN_TYPE, IS_NULLABLE FROM INFORMATION_SCHEMA.COLUMNS "
                              " WHERE TABLE_SCHEMA = '%1' "
                              "   AND TABLE_NAME = '%2' "
                              " ORDER BY ORDINAL_POSITION").arg(schemaName).arg(tableName);
        if (Database::Utils::executeQueryWithLog(&query, queryString))
            while (query.next()) {
                QStringList row;
                row << query.value(0).toString();
                row << query.value(1).toString();
                row << query.value(2).toString();
                row << query.value(3).toString();
                c->addRow(row);
            }
    }

    // Table Indexes Info
    Database::DbDetailsTableFactory *i = new Database::DbDetailsTableFactory(QStringList() << QObject::tr("Name")
                                                                                           << QObject::tr("Description")
                                                                                           << QObject::tr("Column")
                                                                                           << QObject::tr("Position")
                                                                                           << QObject::tr("Nullable")
                                                                                           << QObject::tr("Index Type"));
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = QString("SELECT INDEX_NAME, INDEX_COMMENT, COLUMN_NAME, SEQ_IN_INDEX, NULLABLE, INDEX_TYPE FROM INFORMATION_SCHEMA.STATISTICS "
                              " WHERE TABLE_SCHEMA = '%1' "
                              "   AND TABLE_NAME = '%2' "
                              " ORDER BY 1, 4").arg(schemaName).arg(tableName);
        if (Database::Utils::executeQueryWithLog(&query, queryString))
            while (query.next()) {
                QStringList row;
                row << query.value(0).toString();
                row << query.value(1).toString();
                row << query.value(2).toString();
                row << query.value(3).toString();
                row << query.value(4).toString();
                row << query.value(5).toString();
                i->addRow(row);
            }
    }

    // Table Triggers Info
    Database::DbDetailsTableWithSourceFactory *tr = new Database::DbDetailsTableWithSourceFactory(QStringList() << QObject::tr("Name")
                                                                                                                << QObject::tr("Event Action")
                                                                                                                << QObject::tr("Action Timing"));
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = QString("SELECT TRIGGER_NAME, EVENT_MANIPULATION, ACTION_TIMING, ACTION_STATEMENT FROM INFORMATION_SCHEMA.TRIGGERS "
                              " WHERE TRIGGER_SCHEMA = '%1' "
                              "   AND EVENT_OBJECT_TABLE = '%2' "
                              " ORDER BY 1").arg(schemaName).arg(tableName);
        if (Database::Utils::executeQueryWithLog(&query, queryString)) {
            while ( query.next() )
                tr->addRow(QStringList() << query.value(0).toString() << query.value(1).toString() << query.value(2).toString(),
                           query.value(3).toString());
        }
    }

    // Table Constraints Info
    Database::DbDetailsTableFactory *cn = new Database::DbDetailsTableFactory(QStringList() << QObject::tr("Name")
                                                                                            << QObject::tr("Type"));
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = QString("SELECT CONSTRAINT_NAME, CONSTRAINT_TYPE FROM INFORMATION_SCHEMA.TABLE_CONSTRAINTS "
                              " WHERE TABLE_SCHEMA = '%1' "
                              "   AND TABLE_NAME = '%2' "
                              " ORDER BY 1").arg(schemaName).arg(tableName);
        if (Database::Utils::executeQueryWithLog(&query, queryString))
            while (query.next()) {
                QStringList row;
                row << query.value(0).toString();
                row << query.value(1).toString();
                cn->addRow(row);
            }
    }

    Database::DbDetailsTabbedFactory *tab = new Database::DbDetailsTabbedFactory;
    tab->addTab(QObject::tr("Table Info"), t);
    tab->addTab(QObject::tr("Columns Info"), c);
    tab->addTab(QObject::tr("Indexes"), i);
    tab->addTab(QObject::tr("Triggers"), tr);
    tab->addTab(QObject::tr("Constraints"), cn);

    return tab;
}

static IDbDetailsFactory *createViewDbDetailsFactory(const QString &connName, const QString &schemaName, const QString &tableName)
{
    QSqlDatabase db = QSqlDatabase::database(connName);

    if ( !db.isOpen() ) {
        Core::Storage::warning(QObject::tr("Database connection is not opened."));
        return nullptr;
    }

    // View Info
    Database::DbDetailsFormViewFactory *t = new Database::DbDetailsFormViewFactory;
    {
        t->addItem(Database::DbDetailsFormViewFactory::FormItem(QObject::tr("Name"), tableName));
        QSqlQuery query(db);
        QString   queryString;
        queryString = QString("SELECT TABLE_COMMENT FROM INFORMATION_SCHEMA.TABLES "
                              " WHERE TABLE_SCHEMA = '%1' "
                              "   AND TABLE_NAME = '%2'").arg(schemaName).arg(tableName);
        QString description = "";
        if (Database::Utils::executeQueryWithLog(&query, queryString))
            if ( query.next() )
                description = query.value(0).toString();
        t->addItem(Database::DbDetailsFormViewFactory::FormItem(QObject::tr("Description"), description));
    }

    // View Columns Info
    Database::DbDetailsTableFactory *c = new Database::DbDetailsTableFactory(QStringList() << QObject::tr("Name")
                                                                                           << QObject::tr("Description")
                                                                                           << QObject::tr("Data Type")
                                                                                           << QObject::tr("Nullable") );
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = QString("SELECT COLUMN_NAME, COLUMN_COMMENT, COLUMN_TYPE, IS_NULLABLE FROM INFORMATION_SCHEMA.COLUMNS "
                              " WHERE TABLE_SCHEMA = '%1' "
                              "   AND TABLE_NAME = '%2' "
                              " ORDER BY ORDINAL_POSITION").arg(schemaName).arg(tableName);
        if (Database::Utils::executeQueryWithLog(&query, queryString))
            while (query.next()) {
                QStringList row;
                row << query.value(0).toString();
                row << query.value(1).toString();
                row << query.value(2).toString();
                row << query.value(3).toString();
                c->addRow(row);
            }
    }

    Database::DbDetailsTabbedFactory *tab = new Database::DbDetailsTabbedFactory;
    tab->addTab(QObject::tr("Table Info"), t);
    tab->addTab(QObject::tr("Columns Info"), c);

    return tab;
}

static IDbDetailsFactory *createSourceDbDetailsFactory(const QString &connName, const QString &schemaName, const QString &funcName)
{
    QSqlDatabase db = QSqlDatabase::database(connName);

    if ( !db.isOpen() ) {
        Core::Storage::warning(QObject::tr("Database connection is not opened."));
        return nullptr;
    }

    QString source = "";
    QSqlQuery query(db);
    QString   queryString;

    /// TODO: implement routine parameters and return type for functions from INFORMATION_SCHEMA.PARAMETERS
    queryString = QString("SELECT ROUTINE_DEFINITION, ROUTINE_TYPE FROM INFORMATION_SCHEMA.ROUTINES "
                          " WHERE ROUTINE_SCHEMA = '%1' "
                          "   AND ROUTINE_NAME = '%2'").arg(schemaName).arg(funcName);
    if (Database::Utils::executeQueryWithLog(&query, queryString))
        if (query.next()) {
            source = query.value(1).toString() == "FUNCTION" ? "FUNCTION " : "PROCEDURE ";
            source += funcName + "\n";
            source += query.value(0).toString();
        }

    Database::DbDetailsSourceFactory *s = new Database::DbDetailsSourceFactory(source);
    return s;
}

IDbDetailsFactory *Provider::createDbDetailsFactory(const QString &connName, const QString &schemaName, int schemaObjectType, int schemaObjectSubtype, const QString &objName) const
{
    Q_UNUSED(schemaName)
    Q_UNUSED(schemaObjectSubtype)

    switch ( schemaObjectType ) {
        case Sql::Constants::DbSchemaObjectType::C_TABLE:
            return createTableDbDetailsFactory(connName, schemaName, objName);
        case Sql::Constants::DbSchemaObjectType::C_VIEW:
            return createViewDbDetailsFactory(connName, schemaName, objName);
        case Sql::Constants::DbSchemaObjectType::C_ROUTINE:
            return createSourceDbDetailsFactory(connName, schemaName, objName);
    }

    return nullptr;
}

#ifndef HAVE_QT5
Q_EXPORT_PLUGIN2(Provider, Provider)
#endif

} // MySql
} // Database
