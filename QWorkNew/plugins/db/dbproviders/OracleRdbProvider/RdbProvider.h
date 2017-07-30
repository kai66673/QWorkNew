#ifndef __RDB_PROVIDER_H

#define __RDB_PROVIDER_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QTextCodec>

#include "DbLoader.h"

namespace Database {

class IDbLoader;

namespace RDB {

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

    virtual QString databaseType() const { return QStringLiteral("RDB ODBC"); }
    virtual BaseConnectionInfo *createConnection( const QString &connName, const QStringList &parameters ) const;
    virtual IDbConnectWidget *createAuthWidget( QWidget* parent = 0 ) const;

    virtual Database::IDbLoader::TopLevel supportedTopLevel() const { return Database::IDbLoader::SingleSchema; }

    virtual bool isDdlStatementTransactionSupported() const { return true; }

    virtual MetadataQuery::Ptr schemasMetadataQuery() const;
    virtual MetadataQuery::Ptr objectsMetadataQuery( const QString &schemaName, int schemaObjectType ) const;
    virtual MetadataQuery::Ptr subobjectsMetadataQuery( const QString &schemaName, int schemaObjectType, const QString &objName, int schemaObjectSubtype ) const;
    virtual MetadataQuery::Ptr objectTableNameMetadataQuery( const QString &schemaName, int schemaObjectType, const QString &objName ) const;

    virtual QStringList additionalSchemaCategories() const;
    virtual QStringList additionalSchemaObjectCategories( unsigned schemaObjectType ) const;
    virtual QString additionalSchemaCategoryIconName( int schemaObjectType ) const;
    virtual QString additionalSchemaObjectIconName( int schemaObjectType, ItemProperties * /*properties*/ ) const;
    virtual QString additionalSchemaObjectCategoryIconName( int schemaObjectType, int schemaObjectSubtype ) const;
    virtual QString additionalSchemaSubobjectIconName( int schemaObjectType, int schemaObjectSubtype ) const;

    virtual IDbDetailsFactory *createDbDetailsFactory( const QString &connName,
                                                       const QString &schemaName,
                                                       int schemaObjectType,
                                                       int schemaObjectSubtype,
                                                       const QString &objName ) const;

    virtual QList<QAction *> actionsForItem( IDbCatalogItem *item ) const;
    virtual QList<QDbItemAction *> allActions() const;

    virtual Sql::IParserEngine *parserEngine() { return m_engine; }

private slots:
    void onCreateCreateDynamycSelect( const QString &connName, IDbCatalogItem *item );
    void onCreateCStructure( const QString &connName, IDbCatalogItem *item );

private:
    QDbItemAction *m_actCreateDynamycSelect;
    QDbItemAction *m_actCreateCStructure;

    Sql::IParserEngine *m_engine;

    QTextCodec *m_codec;
};

} // RDB
} // Database


#endif // __RDB_PROVIDER_H
