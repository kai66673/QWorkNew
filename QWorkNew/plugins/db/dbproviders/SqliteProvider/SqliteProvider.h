#ifndef __SQLITE_PROVIDER_H

#define __SQLITE_PROVIDER_H

#include <QObject>
#include <QString>
#include <QVariant>

#include "DbLoader.h"

namespace Database {

class IDbLoader;

namespace SQLITE {

class Provider : public QObject, public Database::IDbLoader
{
    Q_OBJECT
    Q_INTERFACES(Database::IDbLoader)
#ifdef HAVE_QT5
    Q_PLUGIN_METADATA(IID "QWorkBase.IDbLoader/1.0")
#endif

public:
    Provider( QObject *parent = 0 );

    virtual QString databaseType() const { return QStringLiteral("SQLITE"); }
    virtual BaseConnectionInfo *createConnection( const QString &connName, const QStringList &parameters ) const;
    virtual IDbConnectWidget *createAuthWidget( QWidget* parent = 0 ) const;

    virtual Database::IDbLoader::TopLevel supportedTopLevel() const { return Database::IDbLoader::SingleSchema; }

    virtual bool isDdlStatementTransactionSupported() const { return true; }

    virtual MetadataQuery::Ptr schemasMetadataQuery() const;
    virtual MetadataQuery::Ptr objectsMetadataQuery( const QString &schemaName, int schemaObjectType ) const;
    virtual MetadataQuery::Ptr subobjectsMetadataQuery( const QString &schemaName, int schemaObjectType, const QString &objName, int schemaObjectSubtype ) const;
    virtual MetadataQuery::Ptr objectTableNameMetadataQuery( const QString &schemaName, int schemaObjectType, const QString &objName ) const;

    virtual IDbDetailsFactory *createDbDetailsFactory( const QString &connName,
                                                       const QString &schemaName,
                                                       int schemaObjectType,
                                                       int schemaObjectSubtype,
                                                       const QString &objName ) const;
};

} // SQLITE
} // Database


#endif // __SQLITE_PROVIDER_H
