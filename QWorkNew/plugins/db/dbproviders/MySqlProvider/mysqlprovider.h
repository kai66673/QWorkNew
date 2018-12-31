#ifndef MYSQLPROVIDER_H
#define MYSQLPROVIDER_H

#include "DbLoader.h"

namespace Database {

namespace MySql {

class IDbLoader;

class Provider: public QObject, public Database::IDbLoader
{
    Q_OBJECT
    Q_INTERFACES(Database::IDbLoader)
#ifdef HAVE_QT5
    Q_PLUGIN_METADATA(IID "QWorkBase.IDbLoader/1.0")
#endif

public:
    Provider(QObject *parent = nullptr);

    // IDbLoader interface
    QString databaseType() const override { return QStringLiteral("MYSQL"); }
    BaseConnectionInfo *createConnection(const QString &connName, const QStringList &parameters) const override;
    IDbConnectWidget *createAuthWidget(QWidget *parent) const override;
    TopLevel supportedTopLevel() const override { return Database::IDbLoader::MultiSchema; }
    bool isDdlStatementTransactionSupported() const override { return true; }
    MetadataQuery::Ptr schemasMetadataQuery() const override;
    MetadataQuery::Ptr objectsMetadataQuery(const QString &schemaName, int schemaObjectType) const override;
    MetadataQuery::Ptr subobjectsMetadataQuery(const QString &schemaName, int schemaObjectType, const QString &objName, int schemaObjectSubtype) const override;
    MetadataQuery::Ptr objectTableNameMetadataQuery(const QString &schemaName, int schemaObjectType, const QString &objName) const override;
    IDbDetailsFactory *createDbDetailsFactory(const QString &connName, const QString &schemaName, int schemaObjectType, int schemaObjectSubtype, const QString &objName) const override;
};

} // MySql
} // Database

#endif // MYSQLPROVIDER_H
