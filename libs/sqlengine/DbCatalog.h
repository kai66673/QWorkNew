#ifndef DBCATALOG_H
#define DBCATALOG_H

#include <QPair>
#include <QList>
#include <QAbstractItemModel>
#include <QMainWindow>
#include <QAction>

#include "sqlengine_global.h"
#include "sql_constants.h"
#include "DbLoader.h"

namespace Database {

class IDbCatalogItem;

class SQL_EXPORT QDbItemAction: public QAction
{
    Q_OBJECT

public:
    QDbItemAction( const QString &text, QObject *parent = 0 )
        : QAction(text, parent)
    {}

    void emitItemTriggered( const QString &connName, IDbCatalogItem *item );

signals:
    void itemTriggered( const QString &connName, IDbCatalogItem *item );
};

class SQL_EXPORT ItemProperties
{
public:
    ItemProperties();
    virtual ~ItemProperties();

    bool hasProperty( const QString &propertyName ) const;
    QVariant property( const QString &propertyName ) const;
    void setProperty( const QString &propertyName, const QVariant &value );
    void clearProperty( const QString &propertyName );

private:
    QMap<QString, QVariant> m_properties;
};

class SQL_EXPORT IDbCatalogItem
{
public:
    IDbCatalogItem( const QString &name,
                    IDbCatalogItem *parent = 0,
                    const QString &schemaName = QString(),
                    const QString &tableName = QString(),
                    const QString &parentObjectName = QString(),
                    int schemaObjectType = Sql::Constants::DbSchemaObjectType::C_UNDEFINED,
                    int schemaObjectSubtype = Sql::Constants::DbSchemaObjectSubtype::C_UNDEFINED );
    virtual ~IDbCatalogItem();

    inline QString name() const { return m_name; }
    inline IDbCatalogItem *parent() const { return m_parent; }
    QList<IDbCatalogItem *> *childrens( IDbLoader *loader, BaseConnectionInfo *connectionInfo );
    QList<IDbCatalogItem *> *childrensNoLoad();

    IDbCatalogItem *child( int rowIndex, IDbLoader *loader, BaseConnectionInfo *connectionInfo );
    inline IDbCatalogItem *childNoLoad( int rowIndex ) const;
    inline int childCount() const { return m_childrens->size(); }
    int row() const;

    IDbCatalogItem *childByName( const QString &name ) const;
    IDbCatalogItem *childByNameWithLoad( const QString &name, IDbLoader *loader, BaseConnectionInfo *connectionInfo );

    bool loadChildrens( IDbLoader *loader, BaseConnectionInfo *connectionInfo );
    int  fetchChildrens0( IDbLoader *loader, BaseConnectionInfo *connectionInfo );
    void fetchChildrens1();
    void fetchChildrens2();

    inline bool childrensLoaded() const { return m_childrenLoaded; }
    virtual bool hasChildrens( IDbLoader * /*loader*/, BaseConnectionInfo * /*connectionInfo*/ ) const { return true; }
    virtual bool loadChildrensImmediately() const = 0;
    virtual bool doLoadChildrens( IDbLoader *loader, BaseConnectionInfo *connectionInfo ) = 0;

    void clearChildrens();

    QString schemaName();
    QString tableName( IDbLoader *loader, BaseConnectionInfo *connectionInfo );
    QString parentObjectName();

    int schemaObjectType() const { return m_schemaObjectType; }
    int schemaObjectSubtype() const { return m_schemaObjectSubtype; }

    virtual QString defineSchemaName() const = 0;
    virtual QString defineTableName( IDbLoader *loader, BaseConnectionInfo *connectionInfo ) = 0;
    virtual QString defineParentObjectName() const = 0;

    virtual QString iconName( IDbLoader * /*loader*/ ) const { return QStringLiteral(""); }
    virtual bool isNameDetailed() const { return false; }
    virtual bool isNameFiltered() const { return false; }

    virtual IDbCatalogItem *dbDetailItem() { return 0; }
    virtual IDbCatalogItem *referencedItem (IDbLoader * /*loader*/, BaseConnectionInfo * /*connectionInfo*/) { return this; }

    virtual QList<int> actions() const { return QList<int>(); }

    bool hasProperty( const QString &propertyName ) const
    { return m_properties->hasProperty(propertyName); }
    QVariant property( const QString &propertyName ) const
    { return m_properties->property(propertyName); }
    void setProperty( const QString &propertyName, const QVariant &value )
    { m_properties->setProperty(propertyName, value); }
    void clearProperty( const QString &propertyName )
    { m_properties->clearProperty(propertyName); }

    inline ItemProperties *properties() { return m_properties; }

protected:
    QString m_name;
    IDbCatalogItem *m_parent;
    QList<IDbCatalogItem *> *m_childrens;
    bool m_childrenLoaded;

    QString m_schemaName;
    QString m_tableName;
    QString m_parentObjectName;

    int m_schemaObjectType;
    int m_schemaObjectSubtype;

    QList<IDbCatalogItem *> *m_childrensTmp;

    ItemProperties *m_properties;
};

class IDbCatalogRootItem: public IDbCatalogItem
{
public:
    IDbCatalogRootItem()
        : IDbCatalogItem("<ROOT>")
    {}

    virtual QList<IDbCatalogItem *> *schemas() const = 0;
    virtual QList<IDbCatalogItem *> *schemaObjects( unsigned schemaObjectType, const QString &schemaName, IDbLoader *loader, BaseConnectionInfo *connectionInfo ) = 0;
    virtual IDbCatalogItem *schemaObjectList( unsigned schemaObjectType, const QString &schemaName, IDbLoader *loader, BaseConnectionInfo *connectionInfo ) = 0;
};

class DbMultiSchemaCatalogRootItem: public IDbCatalogRootItem
{
public:
    DbMultiSchemaCatalogRootItem()
        : IDbCatalogRootItem()
    {}

    virtual QList<IDbCatalogItem *> *schemas() const { return m_childrens;}
    virtual QList<IDbCatalogItem *> *schemaObjects( unsigned schemaObjectType, const QString &schemaName, IDbLoader *loader, BaseConnectionInfo *connectionInfo );
    virtual IDbCatalogItem *schemaObjectList( unsigned schemaObjectType, const QString &schemaName, IDbLoader *loader, BaseConnectionInfo *connectionInfo );

    virtual bool loadChildrensImmediately() const { return true; }
    virtual bool doLoadChildrens( IDbLoader *loader, BaseConnectionInfo *connectionInfo );

    virtual QString defineSchemaName() const { return QStringLiteral(""); }
    virtual QString defineTableName( IDbLoader * /*loader*/, BaseConnectionInfo * /*connectionInfo*/ ) { return QStringLiteral(""); }
    virtual QString defineParentObjectName() const { return QStringLiteral(""); }
};

class DbSingleSchemaCatalogRootItem: public IDbCatalogRootItem
{
public:
    DbSingleSchemaCatalogRootItem()
        : IDbCatalogRootItem()
    {}

    virtual QList<IDbCatalogItem *> *schemas() const { return 0;}
    virtual QList<IDbCatalogItem *> *schemaObjects( unsigned schemaObjectType, const QString &schemaName, IDbLoader *loader, BaseConnectionInfo *connectionInfo );
    virtual IDbCatalogItem *schemaObjectList( unsigned schemaObjectType, const QString &schemaName, IDbLoader *loader, BaseConnectionInfo *connectionInfo );

    virtual bool loadChildrensImmediately() const { return true; }
    virtual bool doLoadChildrens( IDbLoader *loader, BaseConnectionInfo *connectionInfo );

    virtual QString defineSchemaName() const { return QStringLiteral(""); }
    virtual QString defineTableName( IDbLoader * /*loader*/, BaseConnectionInfo * /*connectionInfo*/ ) { return QStringLiteral(""); }
    virtual QString defineParentObjectName() const { return QStringLiteral(""); }
};

class DbDatabaseCatalogRootItem: public IDbCatalogRootItem
{
public:
    DbDatabaseCatalogRootItem()
        : IDbCatalogRootItem()
    {}

    virtual QList<IDbCatalogItem *> *schemas() const;
    virtual QList<IDbCatalogItem *> *schemaObjects( unsigned schemaObjectType, const QString &schemaName, IDbLoader *loader, BaseConnectionInfo *connectionInfo );
    virtual IDbCatalogItem *schemaObjectList( unsigned schemaObjectType, const QString &schemaName, IDbLoader *loader, BaseConnectionInfo *connectionInfo );

    virtual bool loadChildrensImmediately() const { return true; }
    virtual bool doLoadChildrens( IDbLoader *loader, BaseConnectionInfo *connectionInfo );

    virtual QString defineSchemaName() const { return QStringLiteral(""); }
    virtual QString defineTableName( IDbLoader * /*loader*/, BaseConnectionInfo * /*connectionInfo*/ ) { return QStringLiteral(""); }
    virtual QString defineParentObjectName() const { return QStringLiteral(""); }
};

class DbSchemaItem: public IDbCatalogItem
{
public:
    DbSchemaItem( const QString &name,
                  IDbCatalogItem *parent )
        : IDbCatalogItem(name, parent)
    {}

    virtual bool loadChildrensImmediately() const { return true; }
    virtual bool doLoadChildrens( IDbLoader *loader, BaseConnectionInfo *connectionInfo );

    virtual QString defineSchemaName() const { return m_name; }
    virtual QString defineTableName( IDbLoader * /*loader*/, BaseConnectionInfo * /*connectionInfo*/ ) { return QStringLiteral(""); }
    virtual QString defineParentObjectName() const { return QStringLiteral(""); }

    virtual QString iconName( IDbLoader * /*loader*/ ) const { return QStringLiteral(":/images/dbb_schema.png"); }
    virtual bool isNameDetailed() const { return true; }
};

class DbSchemaCategoryItem: public IDbCatalogItem
{
public:
    DbSchemaCategoryItem( const QString &name,
                          IDbCatalogItem *parent,
                          int schemaObjectType )
        : IDbCatalogItem( name,
                          parent,
                          QString(),
                          QString(),
                          QString(),
                          schemaObjectType )
    {}

    virtual bool loadChildrensImmediately() const { return false; }
    virtual bool doLoadChildrens( IDbLoader *loader, BaseConnectionInfo *connectionInfo );

    virtual QString defineSchemaName() const { return parent()->schemaName(); }
    virtual QString defineTableName( IDbLoader * /*loader*/, BaseConnectionInfo * /*connectionInfo*/ ) { return QStringLiteral(""); }
    virtual QString defineParentObjectName() const { return QStringLiteral(""); }

    virtual QString iconName( IDbLoader *loader ) const;
};

class DbDatabaseCategoryItem: public IDbCatalogItem
{
public:
    DbDatabaseCategoryItem( const QString &name,
                            IDbCatalogItem *parent,
                            int dbObjectType )
        : IDbCatalogItem(name, parent)
        , m_dbObjectType(dbObjectType)
    {}

    virtual bool loadChildrensImmediately() const { return true; }
    virtual bool doLoadChildrens( IDbLoader *loader, BaseConnectionInfo *connectionInfo );

    virtual QString defineSchemaName() const { return QStringLiteral(""); }
    virtual QString defineTableName( IDbLoader * /*loader*/, BaseConnectionInfo * /*connectionInfo*/ ) { return QStringLiteral(""); }
    virtual QString defineParentObjectName() const { return QStringLiteral(""); }

    virtual QString iconName( IDbLoader *loader ) const;

    int databaseObjectType() const;

private:
    int m_dbObjectType;
};

class DbSchemaObjectItem: public IDbCatalogItem
{
public:
    DbSchemaObjectItem( const QString &name,
                        IDbCatalogItem *parent,
                        const QString &tableName = QString() )
        : IDbCatalogItem(name, parent, QString(), tableName, QString(), parent->schemaObjectType())
    {}

    virtual bool hasChildrens( IDbLoader *loader, BaseConnectionInfo *connectionInfo ) const;

    virtual bool loadChildrensImmediately() const { return true; }
    virtual bool doLoadChildrens( IDbLoader *loader, BaseConnectionInfo *connectionInfo );

    virtual QString defineSchemaName() const { return parent()->schemaName(); }
    virtual QString defineTableName( IDbLoader *loader, BaseConnectionInfo *connectionInfo );
    virtual QString defineParentObjectName() const { return m_name; }

    virtual QString iconName( IDbLoader *loader ) const;
    virtual bool isNameDetailed() const { return true; }
    virtual bool isNameFiltered() const { return true; }

    virtual IDbCatalogItem *dbDetailItem() { return this; }
    virtual IDbCatalogItem *referencedItem( IDbLoader *loader, BaseConnectionInfo *connectionInfo );

    virtual QList<int> actions() const;
};

class DbSchemaObjectCategoryItem: public IDbCatalogItem
{
public:
    DbSchemaObjectCategoryItem( const QString &name,
                                IDbCatalogItem *parent,
                                int schemaObjectSubtype )
        : IDbCatalogItem( name,
                          parent,
                          QString(),
                          QString(),
                          QString(),
                          parent->schemaObjectType(),
                          schemaObjectSubtype )
    {}

    virtual bool loadChildrensImmediately() const { return false; }
    virtual bool doLoadChildrens( IDbLoader *loader, BaseConnectionInfo *connectionInfo );

    virtual QString defineSchemaName() const { return parent()->schemaName(); }
    virtual QString defineTableName( IDbLoader *loader, BaseConnectionInfo *connectionInfo ) { return parent()->tableName(loader, connectionInfo); }
    virtual QString defineParentObjectName() const { return parent()->parentObjectName(); }

    virtual QString iconName( IDbLoader *loader ) const;

    virtual IDbCatalogItem *dbDetailItem() { return parent()->dbDetailItem(); }
};

class DbSchemaSubObjectItem: public IDbCatalogItem
{
public:
    DbSchemaSubObjectItem( const QString &name,
                           IDbCatalogItem *parent,
                           QString additionalInfo = QString() )
        : IDbCatalogItem( name,
                          parent,
                          QString(),
                          QString(),
                          QString(),
                          parent->schemaObjectType(),
                          parent->schemaObjectSubtype() )
        , m_additionalInfo(additionalInfo)
    {}

    virtual bool hasChildrens( IDbLoader * /*loader*/, BaseConnectionInfo * /*connectionInfo*/ ) const { return false; }
    virtual bool loadChildrensImmediately() const { return false; }
    virtual bool doLoadChildrens( IDbLoader * /*loader*/, BaseConnectionInfo * /*connectionInfo*/ ) { return true; }

    virtual QString defineSchemaName() const { return parent()->schemaName(); }
    virtual QString defineTableName( IDbLoader *loader, BaseConnectionInfo *connectionInfo ) { return parent()->tableName(loader, connectionInfo); }
    virtual QString defineParentObjectName() const { return parent()->parentObjectName(); }

    virtual QString iconName( IDbLoader *loader ) const;
    virtual bool isNameDetailed() const { return true; }

    virtual IDbCatalogItem *dbDetailItem() { return parent()->dbDetailItem(); }

private:
    QString m_additionalInfo;
};

class SQL_EXPORT DbDatabaseObjectItem: public IDbCatalogItem
{
public:
    DbDatabaseObjectItem( const QString &name,
                          DbDatabaseCategoryItem *parent )
        : IDbCatalogItem(name, parent)
        , m_dbObectType(parent->databaseObjectType())
    {}

    virtual bool hasChildrens( IDbLoader * /*loader*/, BaseConnectionInfo * /*connectionInfo*/ ) const { return false; }
    virtual bool loadChildrensImmediately() const { return false; }
    virtual bool doLoadChildrens( IDbLoader * /*loader*/, BaseConnectionInfo * /*connectionInfo*/ ) { return true; }

    virtual QString defineSchemaName() const { return QStringLiteral(""); }
    virtual QString defineTableName( IDbLoader * /*loader*/, BaseConnectionInfo * /*connectionInfo*/ ) { return QStringLiteral(""); }
    virtual QString defineParentObjectName() const { return QStringLiteral(""); }

    virtual QString iconName( IDbLoader *loader ) const;
    virtual bool isNameDetailed() const { return true; }

    virtual IDbCatalogItem *dbDetailItem() { return parent()->dbDetailItem(); }

private:
    int m_dbObectType;
};

class SQL_EXPORT DbCatalogModel: public QAbstractItemModel
{
    Q_OBJECT

public:
    DbCatalogModel( BaseConnectionInfo *connectionInfo, IDbLoader *loader, QObject *parent = 0 );
    virtual ~DbCatalogModel();

    QVariant data( const QModelIndex &index, int role) const;
    Qt::ItemFlags flags( const QModelIndex &index ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
    QModelIndex parent( const QModelIndex &index ) const;
    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const;
    bool hasChildren ( const QModelIndex & parent ) const;
    bool canFetchMore( const QModelIndex & parent ) const;
    void fetchMore( const QModelIndex & parent );

    IDbCatalogItem *getItem( const QModelIndex &index );
    IDbCatalogItem *getItemChild( int rowIndex, const QModelIndex &index );

    inline IDbLoader *loader() const { return m_loader; }
    inline BaseConnectionInfo *connectionInfo() const { return m_connectionInfo; }
    inline IDbCatalogRootItem *rootItem() const { return m_rootItem; }

private:
    BaseConnectionInfo *m_connectionInfo;
    IDbLoader *m_loader;
    IDbCatalogRootItem *m_rootItem;
    bool m_ddlTransaction;
};

} // Database

#endif // DBCATALOG_H
