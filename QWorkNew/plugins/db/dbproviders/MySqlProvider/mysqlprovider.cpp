#include "Core.h"
#include "mysqlauthwidget.h"
#include "mysqlprovider.h"

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

IDbDetailsFactory *Provider::createDbDetailsFactory(const QString &connName, const QString &schemaName, int schemaObjectType, int schemaObjectSubtype, const QString &objName) const
{
    return nullptr;
}

#ifndef HAVE_QT5
Q_EXPORT_PLUGIN2(Provider, Provider)
#endif

} // MySql
} // Database
