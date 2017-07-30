#ifndef __LINTER_PROVIDER_H

#define __LINTER_PROVIDER_H

#include <QObject>
#include <QString>
#include <QVariant>

#include "DbLoader.h"

namespace Database {

class IDbLoader;

namespace Linter {

class Provider : public QObject, public Database::IDbLoader
{
    Q_OBJECT
    Q_INTERFACES(Database::IDbLoader)
#ifdef HAVE_QT5
    Q_PLUGIN_METADATA(IID "QWorkBase.IDbLoader/1.0")
#endif

public:
    Provider( QObject *parent = 0 );
    virtual ~Provider();

    virtual QString databaseType() const { return QStringLiteral("Linter"); }
    virtual BaseConnectionInfo *createConnection( const QString &connName, const QStringList &parameters ) const;
    virtual IDbConnectWidget *createAuthWidget( QWidget* parent = 0 ) const;

    virtual Database::IDbLoader::TopLevel supportedTopLevel() const { return Database::IDbLoader::MultiSchema; }

    virtual bool isDdlStatementTransactionSupported() const { return false; }

    virtual MetadataQuery::Ptr schemasMetadataQuery() const;
    virtual MetadataQuery::Ptr objectsMetadataQuery( const QString &schemaName, int schemaObjectType ) const;
    virtual MetadataQuery::Ptr subobjectsMetadataQuery( const QString &schemaName, int schemaObjectType, const QString &objName, int schemaObjectSubtype ) const;
    virtual MetadataQuery::Ptr objectTableNameMetadataQuery( const QString &schemaName, int schemaObjectType, const QString &objName ) const;

    virtual QStringList additionalSchemaCategories() const;
    virtual QString additionalSchemaCategoryIconName( int schemaObjectType ) const;
    virtual QString additionalSchemaObjectIconName( int schemaObjectType, ItemProperties * /*properties*/ ) const;

    virtual QStringList additionalSchemaObjectCategories( unsigned schemaObjectType ) const;
    virtual QString additionalSchemaObjectCategoryIconName(int schemaObjectType, int schemaObjectSubtype) const;
    virtual QString additionalSchemaSubobjectIconName(int schemaObjectType, int schemaObjectSubtype) const;

    virtual IDbDetailsFactory *createDbDetailsFactory( const QString &connName,
                                                       const QString &schemaName,
                                                       int schemaObjectType,
                                                       int schemaObjectSubtype,
                                                       const QString &objName ) const;
};

} // Linter
} // Database


#endif // __LINTER_PROVIDER_H
