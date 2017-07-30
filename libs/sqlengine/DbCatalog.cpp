#include <QMenu>

#include <QDebug>

#include "DbCatalog.h"
#include "DbUtils.h"
#include "sql_constants.h"

namespace Database {

void QDbItemAction::emitItemTriggered( const QString &connName, IDbCatalogItem *item )
{
    emit itemTriggered(connName, item);
}

ItemProperties::ItemProperties()
{ }

ItemProperties::~ItemProperties()
{ }

bool ItemProperties::hasProperty( const QString &propertyName ) const
{ return m_properties.constFind(propertyName) != m_properties.constEnd(); }

QVariant ItemProperties::property( const QString &propertyName ) const
{
    QMap<QString, QVariant>::const_iterator it = m_properties.constFind(propertyName);
    if ( it == m_properties.constEnd() )
        return QVariant();
    return *it;
}

void ItemProperties::setProperty( const QString &propertyName, const QVariant &value )
{ m_properties[propertyName] = value; }

void ItemProperties::clearProperty( const QString &propertyName )
{ m_properties.remove(propertyName); }

IDbCatalogItem::IDbCatalogItem( const QString &name ,
                                IDbCatalogItem *parent,
                                const QString &schemaName,
                                const QString &tableName,
                                const QString &parentObjectName,
                                int schemaObjectType,
                                int schemaObjectSubtype )
    : m_name(name.trimmed())
    , m_parent(parent)
    , m_childrens(new QList<IDbCatalogItem *>())
    , m_childrenLoaded(false)
    , m_schemaName(schemaName)
    , m_tableName(tableName)
    , m_parentObjectName(parentObjectName)
    , m_schemaObjectType(schemaObjectType)
    , m_schemaObjectSubtype(schemaObjectSubtype)
    , m_childrensTmp(new QList<IDbCatalogItem *>())
    , m_properties(new ItemProperties())
{
}

IDbCatalogItem::~IDbCatalogItem()
{
    qDeleteAll(*m_childrens);
    delete m_childrens;
    qDeleteAll(*m_childrensTmp);
    delete m_childrensTmp;
    delete m_properties;
}

QList<IDbCatalogItem *> *IDbCatalogItem::childrens( IDbLoader *loader, BaseConnectionInfo *connectionInfo )
{
    if ( !childrensLoaded() )
        loadChildrens(loader, connectionInfo);
    return m_childrens;
}

QList<IDbCatalogItem *> *IDbCatalogItem::childrensNoLoad()
{
    return m_childrens;
}

IDbCatalogItem *IDbCatalogItem::child( int rowIndex, IDbLoader *loader, BaseConnectionInfo *connectionInfo )
{
    if ( !childrensLoaded() )
        loadChildrens(loader, connectionInfo);
    if ( rowIndex >= 0 && rowIndex < m_childrens->size() )
        return m_childrens->at(rowIndex);
    return 0;
}

IDbCatalogItem *IDbCatalogItem::childNoLoad( int rowIndex ) const
{
    if ( m_childrens ) {
        if ( rowIndex >= 0 && rowIndex < m_childrens->size() )
            return m_childrens->at(rowIndex);
    }

    return 0;
}

int IDbCatalogItem::row() const
{
    if ( m_parent )
        return m_parent->m_childrens->indexOf(const_cast<IDbCatalogItem *>(this));

    return 0;
}

IDbCatalogItem *IDbCatalogItem::childByName( const QString &name ) const
{
    for(int i = 0; i < m_childrens->size(); i++) {
        IDbCatalogItem *ch = m_childrens->at(i);
        if ( !ch->name().compare(name, Qt::CaseInsensitive) )
            return ch;
    }
    return 0;
}

IDbCatalogItem *IDbCatalogItem::childByNameWithLoad( const QString &name, IDbLoader *loader, BaseConnectionInfo *connectionInfo )
{
    if ( !childrensLoaded() )
        loadChildrens(loader, connectionInfo);

    return childByName(name);
}

bool IDbCatalogItem::loadChildrens( IDbLoader *loader, BaseConnectionInfo *connectionInfo )
{
    if ( m_childrenLoaded )
        return true;

    clearChildrens();
    m_childrenLoaded = doLoadChildrens(loader, connectionInfo);
    if ( m_childrenLoaded ) {
        for (int i = 0; i < m_childrens->size(); i++) {
            IDbCatalogItem *object = m_childrens->at(i);
            if ( object->loadChildrensImmediately() )
                object->loadChildrens(loader, connectionInfo);
        }
    }
    return m_childrenLoaded;
}

int IDbCatalogItem::fetchChildrens0( IDbLoader *loader, BaseConnectionInfo *connectionInfo )
{
    QList<IDbCatalogItem *> *t = m_childrensTmp;
    m_childrensTmp = m_childrens;
    m_childrens = t;

    if ( !loadChildrens(loader, connectionInfo) ) {
        fetchChildrens1();
        return 0;
    }

    fetchChildrens1();
    return m_childrensTmp->size();
}

void IDbCatalogItem::fetchChildrens1()
{
    QList<IDbCatalogItem *> *t = m_childrens;
    m_childrens = m_childrensTmp;
    m_childrensTmp = t;
}

void IDbCatalogItem::fetchChildrens2()
{
    QList<IDbCatalogItem *> *t = m_childrensTmp;
    m_childrensTmp = m_childrens;
    m_childrens = t;
}

void IDbCatalogItem::clearChildrens()
{
    qDeleteAll(*m_childrens);
    m_childrens->clear();
    m_childrenLoaded = false;
}

QString IDbCatalogItem::schemaName()
{
    if ( m_schemaName.isNull() )
        m_schemaName = defineSchemaName();
    return m_schemaName;
}

QString IDbCatalogItem::tableName( IDbLoader *loader, BaseConnectionInfo *connectionInfo )
{
    if ( m_tableName.isNull() )
        m_tableName = defineTableName(loader, connectionInfo);
    return m_tableName;
}

QString IDbCatalogItem::parentObjectName()
{
    if ( m_parentObjectName.isNull() )
        m_parentObjectName = defineParentObjectName();
    return m_parentObjectName;
}

DbCatalogModel::DbCatalogModel( BaseConnectionInfo *connectionInfo, IDbLoader *loader, QObject *parent )
    : QAbstractItemModel(parent)
    , m_connectionInfo(connectionInfo)
    , m_loader(loader)
{
    switch ( m_loader->supportedTopLevel() ) {
        case IDbLoader::DbMultiSchema:
            m_rootItem = new DbDatabaseCatalogRootItem();
            break;
        case IDbLoader::MultiSchema:
            m_rootItem = new DbMultiSchemaCatalogRootItem();
            break;
        case IDbLoader::SingleSchema:
            m_rootItem = new DbSingleSchemaCatalogRootItem();
            break;
    }

    m_ddlTransaction = m_loader->isDdlStatementTransactionSupported();
    m_rootItem->loadChildrens(m_loader, m_connectionInfo);
}

DbCatalogModel::~DbCatalogModel()
{
    delete m_rootItem;
}

QVariant DbCatalogModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    if ( role == Qt::DecorationRole ) {
        IDbCatalogItem *itemIcon = static_cast<IDbCatalogItem *>(index.internalPointer());
        return QIcon(itemIcon->iconName(m_loader));
    }

    if ( role == Qt::FontRole ) {
        IDbCatalogItem *item = static_cast<IDbCatalogItem *>(index.internalPointer());
        if ( item->isNameDetailed() ) {
            QFont font;
            font.setBold(true);
            return font;
        }
        return QVariant();
    }

    if ( role != Qt::DisplayRole )
        return QVariant();

    IDbCatalogItem *item = static_cast<IDbCatalogItem *>(index.internalPointer());

    return item->name();
}

Qt::ItemFlags DbCatalogModel::flags( const QModelIndex &index ) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
}

QVariant DbCatalogModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    Q_UNUSED(section)

    if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
        return m_rootItem->name();

    return QVariant();
}

QModelIndex DbCatalogModel::index( int row, int column, const QModelIndex &parent ) const
{
    if ( !hasIndex(row, column, parent) )
        return QModelIndex();

    IDbCatalogItem *parentItem;

    if ( !parent.isValid() )
        parentItem = m_rootItem;
    else
        parentItem = static_cast<IDbCatalogItem *>(parent.internalPointer());

    IDbCatalogItem *childItem = parentItem->childNoLoad(row);
    if ( childItem )
        return createIndex(row, column, childItem);

    return QModelIndex();
}

QModelIndex DbCatalogModel::parent( const QModelIndex &index ) const
{
    if ( !index.isValid() )
        return QModelIndex();

    IDbCatalogItem *childItem = static_cast<IDbCatalogItem *>(index.internalPointer());
    IDbCatalogItem *parentItem = childItem->parent();

    if ( parentItem == m_rootItem )
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int DbCatalogModel::rowCount( const QModelIndex &parent ) const
{
    IDbCatalogItem *parentItem;
    if ( parent.column() > 0 )
        return 0;

    if ( !parent.isValid( ))
        parentItem = m_rootItem;
    else
        parentItem = static_cast<IDbCatalogItem *>(parent.internalPointer());

    return parentItem->childCount();
}

int DbCatalogModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return 1;
}

bool DbCatalogModel::hasChildren( const QModelIndex &parent ) const
{
    if ( !parent.isValid() )
        return true;

    IDbCatalogItem *item = static_cast<IDbCatalogItem *>(parent.internalPointer());
    if ( item->childCount() ) return true;

    if ( !item->hasChildrens(m_loader, m_connectionInfo) )
        return false;

    return true;
}

bool DbCatalogModel::canFetchMore( const QModelIndex &parent ) const
{
    if ( !parent.isValid() )
        return false;

    IDbCatalogItem *item = static_cast<IDbCatalogItem *>(parent.internalPointer());

    if ( !item->hasChildrens(m_loader, m_connectionInfo) )
        return false;

    if ( item->childrensLoaded() )
        return false;

    return true;
}

void DbCatalogModel::fetchMore( const QModelIndex &parent )
{
    if ( !canFetchMore(parent) )
        return;

    IDbCatalogItem *item = static_cast<IDbCatalogItem *>(parent.internalPointer());

//    item->loadChildrens(m_loader, m_connectionInfo);
//    int itemCount = item->childCount() - 1;
//    if ( itemCount == -1 )
//        return;

//    beginInsertRows(parent, 0, itemCount);
//    endInsertRows();

    int itemCount = item->fetchChildrens0(m_loader, m_connectionInfo) - 1;
    if ( itemCount == -1 )
        return;

    beginInsertRows(parent, 0, itemCount);
    item->fetchChildrens2();
    endInsertRows();
}

IDbCatalogItem *DbCatalogModel::getItem( const QModelIndex &index )
{
    if ( canFetchMore(index) )
        fetchMore(index);

    return static_cast<IDbCatalogItem *>(index.internalPointer());
}

IDbCatalogItem *DbCatalogModel::getItemChild( int rowIndex, const QModelIndex &index )
{
    if ( canFetchMore(index) )
        fetchMore(index);

    QModelIndex childIndex = this->index(rowIndex, 0, index);
    if ( !childIndex.isValid() )
        return NULL;

    return getItem(childIndex);
}

QList<IDbCatalogItem *> *DbMultiSchemaCatalogRootItem::schemaObjects( unsigned schemaObjectType, const QString &schemaName, IDbLoader *loader, BaseConnectionInfo *connectionInfo )
{
    QString schName = schemaName;
    if ( schName.isEmpty() )
        schName = connectionInfo->defaultSchemaName();
    if ( schName.isEmpty() )
        return 0;

    if ( IDbCatalogItem *schemaItem = childByName(schName) ) {
        if ( IDbCatalogItem *categoryItem = schemaItem->child(schemaObjectType, loader, connectionInfo) ) {
            return categoryItem->childrens(loader, connectionInfo);
        }
    }

    return 0;
}

IDbCatalogItem *DbMultiSchemaCatalogRootItem::schemaObjectList(unsigned schemaObjectType, const QString &schemaName, IDbLoader *loader, BaseConnectionInfo *connectionInfo )
{
    QString schName = schemaName;
    if ( schName.isEmpty() )
        schName = connectionInfo->defaultSchemaName();
    if ( schName.isEmpty() )
        return 0;

    if ( IDbCatalogItem *schemaItem = childByName(schName) )
        return schemaItem->child(schemaObjectType, loader, connectionInfo);

    return 0;
}

bool DbMultiSchemaCatalogRootItem::doLoadChildrens( IDbLoader *loader, BaseConnectionInfo *connectionInfo )
{
    return Utils::loadMetadata1<DbSchemaItem>(connectionInfo, m_childrens, this, loader->schemasMetadataQuery().data());
}

QList<IDbCatalogItem *> *DbSingleSchemaCatalogRootItem::schemaObjects( unsigned schemaObjectType, const QString &schemaName, IDbLoader *loader, BaseConnectionInfo *connectionInfo )
{
    if ( schemaName.isEmpty() ) {
        if ( IDbCatalogItem *categoryItem = child(schemaObjectType, loader, connectionInfo) ) {
            return categoryItem->childrens(loader, connectionInfo);
        }
    }

    return 0;
}

IDbCatalogItem *DbSingleSchemaCatalogRootItem::schemaObjectList( unsigned schemaObjectType, const QString &schemaName, IDbLoader *loader, BaseConnectionInfo *connectionInfo )
{
    if ( schemaName.isEmpty() )
        return child(schemaObjectType, loader, connectionInfo);

    return 0;
}

bool DbSingleSchemaCatalogRootItem::doLoadChildrens( IDbLoader *loader, BaseConnectionInfo *connectionInfo )
{
    Q_UNUSED(connectionInfo)

    QStringList categories;
    loader->fillSchemaCategories(categories);
    for (int i = 0; i < categories.size(); i++)
        m_childrens->append(new DbSchemaCategoryItem(categories[i], this, i));
    return true;
}

QList<IDbCatalogItem *> *DbDatabaseCatalogRootItem::schemas() const
{
    return m_childrens->at(0)->childrensNoLoad();
}

QList<IDbCatalogItem *> *DbDatabaseCatalogRootItem::schemaObjects(unsigned schemaObjectType, const QString &schemaName, IDbLoader *loader, BaseConnectionInfo *connectionInfo)
{
    QString schName = schemaName;
    if ( schName.isEmpty() )
        schName = connectionInfo->defaultSchemaName();
    if ( schName.isEmpty() )
        return 0;

    if ( m_childrens && m_childrens->size() > 0) {
        if ( IDbCatalogItem *schemaItem = m_childrens->at(0)->childByName(schName) ) {
            if ( IDbCatalogItem *categoryItem = schemaItem->child(schemaObjectType, loader, connectionInfo) ) {
                return categoryItem->childrens(loader, connectionInfo);
            }
        }
    }

    return 0;
}

IDbCatalogItem *DbDatabaseCatalogRootItem::schemaObjectList(unsigned schemaObjectType, const QString &schemaName, IDbLoader *loader, BaseConnectionInfo *connectionInfo)
{
    QString schName = schemaName;
    if ( schName.isEmpty() )
        schName = connectionInfo->defaultSchemaName();
    if ( schName.isEmpty() )
        return 0;

    if ( m_childrens && m_childrens->size() > 0) {
        if ( IDbCatalogItem *schemaItem = m_childrens->at(0)->childByName(schName) )
            return schemaItem->child(schemaObjectType, loader, connectionInfo);
    }

    return 0;
}

bool DbDatabaseCatalogRootItem::doLoadChildrens(IDbLoader *loader, BaseConnectionInfo *connectionInfo)
{
    Q_UNUSED(connectionInfo);

    QStringList categories;
    loader->fillDbCategories(categories);
    for (int i = 0; i < categories.size(); i++)
        m_childrens->append(new DbDatabaseCategoryItem(categories[i], this, i));
    return true;
}

bool DbSchemaCategoryItem::doLoadChildrens( IDbLoader *loader, BaseConnectionInfo *connectionInfo )
{
    return Utils::loadMetadata<DbSchemaObjectItem>( connectionInfo, m_childrens, this,
                                                    loader->objectsMetadataQuery( schemaName(),
                                                                                  schemaObjectType() ).data() );
}

QString DbSchemaCategoryItem::iconName( IDbLoader *loader ) const
{
    switch ( m_schemaObjectType ) {
        case Sql::Constants::DbSchemaObjectType::C_TABLE:       return QStringLiteral(":/images/dbb_tables.png");
        case Sql::Constants::DbSchemaObjectType::C_VIEW:        return QStringLiteral(":/images/dbb_views.png");
        case Sql::Constants::DbSchemaObjectType::C_INDEX:       return QStringLiteral(":/images/dbb_indexes.png");
        case Sql::Constants::DbSchemaObjectType::C_TRIGGER:     return QStringLiteral(":/images/dbb_triggers.png");
        case Sql::Constants::DbSchemaObjectType::C_CONSTRAINT:  return QStringLiteral(":/images/dbb_constraints.png");
    }

    return loader->additionalSchemaCategoryIconName(m_schemaObjectType);
}

bool DbDatabaseCategoryItem::doLoadChildrens(IDbLoader *loader, BaseConnectionInfo *connectionInfo)
{
    switch ( m_dbObjectType ) {
        case Sql::Constants::DbDatabaseObjectType::C_SCHEMA:
            return Utils::loadMetadata1<DbSchemaItem>(connectionInfo, m_childrens, this, loader->schemasMetadataQuery().data());
    }

    return Utils::loadDatabaseMetadata<DbDatabaseObjectItem>(connectionInfo, m_childrens, this, loader->dbObjectsMetadataQuery(m_dbObjectType).data());
}

QString DbDatabaseCategoryItem::iconName(IDbLoader *loader) const
{
    switch ( m_dbObjectType ) {
    case Sql::Constants::DbDatabaseObjectType::C_SCHEMA:
        return QStringLiteral(":/images/dbb_storages.png");
    }

    return loader->additionalDbCategoryIconName(m_dbObjectType);
}

int DbDatabaseCategoryItem::databaseObjectType() const
{ return m_dbObjectType; }

bool DbSchemaItem::doLoadChildrens( IDbLoader *loader, BaseConnectionInfo *connectionInfo )
{
    Q_UNUSED(connectionInfo)

    QStringList categories;
    loader->fillSchemaCategories(categories);
    for (int i = 0; i < categories.size(); i++)
        m_childrens->append(new DbSchemaCategoryItem(categories[i], this, i));
    return true;
}

bool DbSchemaObjectItem::hasChildrens( IDbLoader *loader, BaseConnectionInfo *connectionInfo ) const
{
    Q_UNUSED(connectionInfo)

    QStringList categories;
    loader->fillSchemaObjectCategories(schemaObjectType(), categories);
    return categories.size() > 0;
}

bool DbSchemaObjectItem::doLoadChildrens( IDbLoader *loader, BaseConnectionInfo *connectionInfo )
{
    Q_UNUSED(connectionInfo)

    QStringList categories;
    loader->fillSchemaObjectCategories(schemaObjectType(), categories);

    for (int i = 0; i < categories.size(); i++)
        m_childrens->append(new DbSchemaObjectCategoryItem(categories[i], this, i));

    return true;
}

QString DbSchemaObjectItem::defineTableName( IDbLoader *loader, BaseConnectionInfo *connectionInfo )
{
    return Utils::loadString( connectionInfo,
                              loader->objectTableNameMetadataQuery( schemaName(),
                                                                    schemaObjectType(),
                                                                    parentObjectName() ).data() );
}

QString DbSchemaObjectItem::iconName( IDbLoader *loader ) const
{
    switch ( m_schemaObjectType ) {
        case Sql::Constants::DbSchemaObjectType::C_TABLE:       return QStringLiteral(":/images/dbb_table.png");
        case Sql::Constants::DbSchemaObjectType::C_VIEW:        return QStringLiteral(":/images/dbb_view.png");
        case Sql::Constants::DbSchemaObjectType::C_INDEX:       return QStringLiteral(":/images/dbb_index.png");
        case Sql::Constants::DbSchemaObjectType::C_TRIGGER:     return QStringLiteral(":/images/dbb_trigger.png");
        case Sql::Constants::DbSchemaObjectType::C_CONSTRAINT:  return QStringLiteral(":/images/dbb_constraint.png");
    }

    return loader->additionalSchemaObjectIconName( m_schemaObjectType, m_properties );
}

IDbCatalogItem *DbSchemaObjectItem::referencedItem( IDbLoader *loader, BaseConnectionInfo *connectionInfo )
{
    switch ( m_schemaObjectType ) {
        case Sql::Constants::DbSchemaObjectType::C_INDEX:
        case Sql::Constants::DbSchemaObjectType::C_TRIGGER:
        case Sql::Constants::DbSchemaObjectType::C_CONSTRAINT:
            if ( IDbCatalogItem *resultItem = parent()->parent() )
                if ( resultItem = resultItem->child(0, loader, connectionInfo) )
                    if ( resultItem = resultItem->childByNameWithLoad(tableName(loader, connectionInfo), loader, connectionInfo) )
                        if ( resultItem = resultItem->child(m_schemaObjectType - 1, loader, connectionInfo) )
                            return resultItem = resultItem->childByNameWithLoad(name(), loader, connectionInfo);
            return this;
    }

    return this;
}

QList<int> DbSchemaObjectItem::actions() const
{
    switch ( schemaObjectType() ) {
        case Sql::Constants::DbSchemaObjectType::C_TABLE:
        case Sql::Constants::DbSchemaObjectType::C_VIEW:
            return QList<int>() << Sql::Constants::DbItemAction::C_QUERY_DATA_ACTION;
    }

    return QList<int>();
}

bool DbSchemaObjectCategoryItem::doLoadChildrens( IDbLoader *loader, BaseConnectionInfo *connectionInfo )
{
    return Utils::loadMetadata<DbSchemaSubObjectItem>( connectionInfo, m_childrens, this,
                                                       loader->subobjectsMetadataQuery( schemaName(),
                                                                                        schemaObjectType(),
                                                                                        parentObjectName(),
                                                                                        schemaObjectSubtype() ).data() );
}

QString DbSchemaObjectCategoryItem::iconName( IDbLoader *loader ) const
{
    Q_UNUSED(loader)

    switch ( m_schemaObjectType ) {

        case Sql::Constants::DbSchemaObjectType::C_TABLE:
            switch ( m_schemaObjectSubtype ) {
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_COLUMN:     return QStringLiteral(":/images/dbb_columns.png");
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_INDEX:      return QStringLiteral(":/images/dbb_indexes.png");
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_TRIGGER:    return QStringLiteral(":/images/dbb_triggers.png");
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_CONSTRAINT: return QStringLiteral(":/images/dbb_constraints.png");
                default: return QStringLiteral("");
            }

        case Sql::Constants::DbSchemaObjectType::C_VIEW:
            switch ( m_schemaObjectSubtype ) {
                case Sql::Constants::DbSchemaObjectSubtype::C_VIEW_COLUMN: return QStringLiteral(":/images/dbb_columns.png");
                default: return QStringLiteral("");
            }
    }

    return loader->additionalSchemaObjectCategoryIconName(m_schemaObjectType, m_schemaObjectSubtype);
}

QString DbSchemaSubObjectItem::iconName( IDbLoader *loader ) const
{
    Q_UNUSED(loader)

    switch ( m_schemaObjectType ) {

        case Sql::Constants::DbSchemaObjectType::C_TABLE:
            switch ( m_schemaObjectSubtype ) {
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_COLUMN:     return QStringLiteral(":/images/dbb_field_text.png");
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_INDEX:      return QStringLiteral(":/images/dbb_index.png");
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_TRIGGER:    return QStringLiteral(":/images/dbb_trigger.png");
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_CONSTRAINT: return QStringLiteral(":/images/dbb_constraint.png");
                default: return QStringLiteral("");
            }

        case Sql::Constants::DbSchemaObjectType::C_VIEW:
            switch ( m_schemaObjectSubtype ) {
                case Sql::Constants::DbSchemaObjectSubtype::C_VIEW_COLUMN: return QStringLiteral(":/images/dbb_field_text.png");
                default: return QStringLiteral("");
            }
    }

    return loader->additionalSchemaSubobjectIconName(m_schemaObjectType, m_schemaObjectSubtype);
}

QString DbDatabaseObjectItem::iconName(IDbLoader *loader) const
{
    return loader->additionalDbObjectIconName(m_dbObectType);
}

} // Database
