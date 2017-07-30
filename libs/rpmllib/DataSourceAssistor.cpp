#include <QIcon>

#include "DataSourceAssistor.h"

namespace RPML {

IDataSourceAssistor::IDataSourceAssistor()
{
}

IDataSourceAssistor::~IDataSourceAssistor()
{
}

QMimeData *IDataSourceAssistor::mimeData( QList<DataSourceObjectItem *> &items )
{
    QStringList columnNames;
    QStringList tableName;
    foreach ( RPML::DataSourceObjectItem *item, items ) {
        QString schemaPrefix = "";
        if ( item->parent->parent )
            schemaPrefix = item->parent->name + ".";
        tableName << schemaPrefix + item->name;
        if ( item->childrens.isEmpty() )
            loadChildrens(item);
        for (int i = 0; i < item->childrens.size(); i++)
            columnNames << schemaPrefix + item->name + "." + item->childrens[i]->name;
    }
    if ( tableName.isEmpty() || columnNames.isEmpty() )
        return 0;

    QString queryText = "SELECT " + columnNames[0] + "\n";
    for (int c = 1; c < columnNames.size(); c++)
        queryText +=    "     , " + columnNames[c] + "\n";
    queryText +=        "  FROM " + tableName[0] + "\n";
    for (int t = 1; t < tableName.size(); t++)
        queryText +=    "     , " + tableName[t] + "\n";

    QMimeData *mime = new QMimeData();
    mime->setText(queryText);

    return mime;

}

DataSourceObjectModel::DataSourceObjectModel( IDataSourceAssistor *assistor, QObject *parent )
    : QAbstractItemModel(parent)
    , m_assistor(assistor)
{
    setupModel();
}

DataSourceObjectModel::~DataSourceObjectModel()
{
    delete m_rootItem;
}

QVariant DataSourceObjectModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    if ( role == Qt::DecorationRole ) {
        DataSourceObjectItem *itemIcon = static_cast<DataSourceObjectItem *>(index.internalPointer());
        return QIcon(m_assistor->iconName(itemIcon->type));
    }

    if ( role != Qt::DisplayRole )
        return QVariant();

    DataSourceObjectItem *item = static_cast<DataSourceObjectItem *>(index.internalPointer());

    return item->name;
}

Qt::ItemFlags DataSourceObjectModel::flags( const QModelIndex &index ) const
{
    if (!index.isValid())
        return 0;

    DataSourceObjectItem *item = static_cast<DataSourceObjectItem *>(index.internalPointer());
    if ( item->selectable )
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;

    return Qt::ItemIsEnabled;
}

QVariant DataSourceObjectModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    Q_UNUSED(section)

    if ( orientation == Qt::Horizontal && role == Qt::DisplayRole )
        return m_rootItem->name;

    return QVariant();
}

QModelIndex DataSourceObjectModel::index( int row, int column, const QModelIndex &parent ) const
{
    if ( !hasIndex(row, column, parent) )
        return QModelIndex();

    DataSourceObjectItem *parentItem;

    if ( !parent.isValid() )
        parentItem = m_rootItem;
    else
        parentItem = static_cast<DataSourceObjectItem *>(parent.internalPointer());

    if ( row >= 0 && row < parentItem->childrens.size() )
        return createIndex(row, column, parentItem->childrens[row]);

    return QModelIndex();
}

QModelIndex DataSourceObjectModel::parent( const QModelIndex &index ) const
{
    if ( !index.isValid() )
        return QModelIndex();

    DataSourceObjectItem *childItem = static_cast<DataSourceObjectItem *>(index.internalPointer());
    DataSourceObjectItem *parentItem = childItem->parent;

    if ( parentItem == m_rootItem )
        return QModelIndex();

    return createIndex(parentItem->parent->childrens.indexOf(parentItem), 0, parentItem);
}

int DataSourceObjectModel::rowCount( const QModelIndex &parent ) const
{
    if ( parent.column() > 0 )
        return 0;

    DataSourceObjectItem *parentItem;

    if ( !parent.isValid( ))
        parentItem = m_rootItem;
    else
        parentItem = static_cast<DataSourceObjectItem *>(parent.internalPointer());

    return parentItem->childrens.size();
}

int DataSourceObjectModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return 1;
}

bool DataSourceObjectModel::hasChildren( const QModelIndex &parent ) const
{
    if ( !parent.isValid() )
        return true;

    DataSourceObjectItem *item = static_cast<DataSourceObjectItem *>(parent.internalPointer());
    if ( item->childrens.size() > 0 )
        return true;

    return item->expandable;
}

bool DataSourceObjectModel::canFetchMore( const QModelIndex &parent ) const
{
    if ( !parent.isValid() )
        return false;

    DataSourceObjectItem *item = static_cast<DataSourceObjectItem *>(parent.internalPointer());

    if ( !item->expandable || item->childrens.size() > 0 )
        return false;

    return true;
}

void DataSourceObjectModel::fetchMore( const QModelIndex &parent )
{
    if ( !canFetchMore(parent) )
        return;

    DataSourceObjectItem *item = static_cast<DataSourceObjectItem *>(parent.internalPointer());

    m_assistor->loadChildrens(item);

    int itemCount = item->childrens.size();
    if ( itemCount > 0 ) {
        beginInsertRows(parent, 0, itemCount - 1);
        endInsertRows();
    }
}

QMimeData *DataSourceObjectModel::mimeData( const QModelIndexList &indexes ) const
{
    QList<DataSourceObjectItem *> items;

    foreach ( const QModelIndex &ind, indexes )
        if ( ind.isValid() )
            items << static_cast<DataSourceObjectItem *>(ind.internalPointer());

    return m_assistor->mimeData(items);
}

void DataSourceObjectModel::setupModel()
{
    m_rootItem = new DataSourceObjectItem(0, "<ROOT>", true, false, 0);
    m_assistor->loadChildrens(m_rootItem);
}

}   // namespace RPML
