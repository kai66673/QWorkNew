#include <QIcon>
#include <QFile>
#include <QInputDialog>

#include "Core.h"
#include "FavoritesPlugin.h"
#include "FavoriteModel.h"

FavoriteTreeItem::FavoriteTreeItem( const QString &groupName, FavoriteTreeItem *parent )
    : m_parent(parent)
    , m_favorite(0)
    , m_groupName(groupName)
{
}

FavoriteTreeItem::FavoriteTreeItem( IFavorite *favorite, FavoriteTreeItem *parent )
    : m_parent(parent)
    , m_favorite(favorite)
    , m_groupName("")
{
}

FavoriteTreeItem::~FavoriteTreeItem()
{
    if ( m_favorite )
        delete m_favorite;

    qDeleteAll(m_childGroups);
    qDeleteAll(m_childItems);
}

void FavoriteTreeItem::removeAllChildrens()
{
    qDeleteAll(m_childItems);
    m_childItems.clear();

    qDeleteAll(m_childGroups);
    m_childGroups.clear();
}

void FavoriteTreeItem::removeChild( int index )
{
    if ( index < 0 )
        return;
    if ( index < m_childGroups.size() ) {
        delete m_childGroups[index];
        m_childGroups.removeAt(index);
        return;
    }
    index -= m_childGroups.size();
    if ( index < m_childItems.size() ) {
        delete m_childItems[index];
        m_childItems.removeAt(index);
    }
}

FavoriteTreeItem *FavoriteTreeItem::child( int row )
{
    if ( row < 0 )
        return 0;

    if ( row < m_childGroups.size() )
        return m_childGroups[row];

    if ( row < m_childGroups.size() + m_childItems.size() )
        return m_childItems[row - m_childGroups.size()];

    return 0;
}

int FavoriteTreeItem::row() const
{
    if ( !m_parent )
        return 0;

    int index = m_parent->m_childGroups.indexOf(const_cast<FavoriteTreeItem *>(this));
    if ( index == -1 )
        index = m_parent->m_childItems.indexOf(const_cast<FavoriteTreeItem *>(this)) + m_parent->m_childGroups.size();

    return index;
}

FavoriteModel::FavoriteModel( FavoritesPlugin *plugin, QObject *parent )
    : QAbstractItemModel(parent)
    , m_plugin(plugin)
{
    m_rootItem = new FavoriteTreeItem("<ROOT>", 0);
    m_applicationRootItem = new FavoriteTreeItem(tr("Application"), m_rootItem);
    m_sessionRootItem = new FavoriteTreeItem(tr("Session"), m_rootItem);
    m_rootItem->appendChildGroup(m_applicationRootItem);
    m_rootItem->appendChildGroup(m_sessionRootItem);
}

QVariant FavoriteModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    if ( index.column() != 0 )
        return QVariant();

    FavoriteTreeItem *item = static_cast<FavoriteTreeItem *>(index.internalPointer());

    switch ( role ) {
        case Qt::DisplayRole:
            if ( item->m_favorite )
                return item->m_favorite->data()->name();
            return item->m_groupName;
        case Qt::ToolTipRole:
            if ( item->m_favorite )
                return item->m_favorite->data()->description();
            return item->m_groupName;
        case Qt::DecorationRole:
            if ( item->m_favorite )
                return QIcon(item->m_favorite->iconName());
            if ( item == m_sessionRootItem )
                return QIcon(":/images/fav_ses_folder.png");
            if ( item == m_applicationRootItem )
                return QIcon(":/images/fav_app_folder.png");
            return QIcon(":/images/fav_folder.png");
    }

    return QVariant();
}

Qt::ItemFlags FavoriteModel::flags( const QModelIndex &index ) const
{
    if ( !index.isValid() )
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant FavoriteModel::headerData( int /*section*/, Qt::Orientation /*orientation*/, int /*role*/ ) const
{
    return QVariant();
}

QModelIndex FavoriteModel::index( int row, int column, const QModelIndex &parent ) const
{
    if ( !hasIndex(row, column, parent) )
        return QModelIndex();

    FavoriteTreeItem *parentItem;

    if ( !parent.isValid() )
        parentItem = m_rootItem;
    else
        parentItem = static_cast<FavoriteTreeItem *>(parent.internalPointer());

    FavoriteTreeItem *childItem = parentItem->child(row);
    if ( childItem )
        return createIndex(row, column, childItem);

    return QModelIndex();
}

QModelIndex FavoriteModel::parent( const QModelIndex &index ) const
{
    if ( !index.isValid() )
        return QModelIndex();

    FavoriteTreeItem *childItem = static_cast<FavoriteTreeItem *>(index.internalPointer());
    FavoriteTreeItem *parentItem = childItem->parent();

    if ( parentItem == m_rootItem )
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int FavoriteModel::rowCount( const QModelIndex &parent ) const
{
    if ( parent.column() > 0 )
        return 0;

    FavoriteTreeItem *parentItem;
    if ( !parent.isValid() )
        parentItem = m_rootItem;
    else
        parentItem = static_cast<FavoriteTreeItem *>(parent.internalPointer());

    return parentItem->childCount();
}

int FavoriteModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return 1;
}

bool FavoriteModel::isGroup( const QModelIndex &index ) const
{
    FavoriteTreeItem *item = static_cast<FavoriteTreeItem *>(index.internalPointer());
    return item->isGroup();
}

void FavoriteModel::execute( const QModelIndex &index ) const
{
    FavoriteTreeItem *item = static_cast<FavoriteTreeItem *>(index.internalPointer());
    if ( IFavorite *favorite = item->m_favorite )
        favorite->execute();
}

void FavoriteModel::insertGroup( const QModelIndex &index )
{
    FavoriteTreeItem *item = static_cast<FavoriteTreeItem *>(index.internalPointer());
    if ( !item )
        return;

    bool ok;
    QString groupName = QInputDialog::getText( Core::Storage::mainWindow()->widget(),
                                               tr("Input Group Name"),
                                               tr("Group Name:"), QLineEdit::Normal, "", &ok )
            .trimmed();
    if (ok && !groupName.isEmpty()) {
        QModelIndex ind = index;
        if ( !item->isGroup() ) {
            item = item->parent();
            ind = index.parent();
        }
        int pos = item->m_childGroups.size();
        beginInsertRows(ind, pos, pos);
        FavoriteTreeItem *group = new FavoriteTreeItem(groupName, item);
        item->appendChildGroup(group);
        endInsertRows();
    }
}

void FavoriteModel::insertItem( const QModelIndex &index )
{
    if ( FavoriteTreeItem *item = static_cast<FavoriteTreeItem *>(index.internalPointer()) ) {
        if ( IFavoriteFactory *factory = m_plugin->selectFavoriteFactory() ) {
            if ( IFavorite *favorite = factory->createFavorite() ) {
                QModelIndex ind = index;
                if ( !item->isGroup() ) {
                    item = item->parent();
                    ind = index.parent();
                }
                int pos = item->childCount();
                beginInsertRows(ind, pos, pos);
                FavoriteTreeItem *favItem = new FavoriteTreeItem(favorite, item);
                item->appendChildItem(favItem);
                endInsertRows();
            }
        }
    }
}

void FavoriteModel::remove( const QModelIndex &index )
{
    if ( FavoriteTreeItem *item = static_cast<FavoriteTreeItem *>(index.internalPointer()) ) {
        if ( item != m_sessionRootItem && item != m_applicationRootItem ) {
            int pos = item->row();
            beginRemoveRows(index.parent(), pos, pos);
            item->parent()->removeChild(pos);
            endRemoveRows();
        }
    }

}

void FavoriteModel::sessionClose()
{
    clearChildrens(m_sessionRootItem);
}

void FavoriteModel::saveSession( const QString &location )
{
    saveHelper(location, m_sessionRootItem);
}

void FavoriteModel::loadSession( const QString &location )
{
    clearChildrens(m_sessionRootItem);
    loadHelper(location, m_sessionRootItem);
}

void FavoriteModel::saveApplication( const QString &location )
{
    saveHelper(location, m_applicationRootItem);
}

void FavoriteModel::loadApplication( const QString &location )
{
    clearChildrens(m_applicationRootItem);
    loadHelper(location, m_applicationRootItem);
}

bool FavoriteModel::canDelete( const QModelIndex &index )
{
    if ( index.isValid() ) {
        if ( FavoriteTreeItem *item = static_cast<FavoriteTreeItem *>(index.internalPointer()) ) {
            return ( item != m_applicationRootItem && item != m_sessionRootItem );
        }
    }

    return false;
}

void FavoriteModel::clearChildrens( FavoriteTreeItem *item )
{
    beginRemoveRows(index(item->row(), 0), 0, item->childCount());
    item->removeAllChildrens();
    endRemoveRows();
}

void FavoriteModel::saveHelper( const QString &location, FavoriteTreeItem *item )
{
    QString fileName = location;
    fileName += "/favorites.xml";

    QFile file(fileName);
    if ( !file.open(QFile::WriteOnly | QFile::Text) ) {
        Core::Storage::warning( tr("Error On Save Session Favorites\nFile \"%1\" Cannot be Opened for Writing:\n%2.")
                                .arg(fileName).arg(file.errorString()) );
        return;
    }

    QDomDocument domDocument("favorites");

    domDocument.appendChild(domDocument.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"windows-1251\""));

    QDomElement root = domDocument.createElement("favorites");
    root.setAttribute("version", "1.0");
    domDocument.appendChild(root);

    foreach ( FavoriteTreeItem *groupItem, item->m_childGroups )
        saveGroup(domDocument, &root, groupItem);
    foreach ( FavoriteTreeItem *favoriteItem, item->m_childItems )
        saveItem(domDocument, &root, favoriteItem);

    const int IndentSize = 4;
    QTextStream out(&file);
    domDocument.save(out, IndentSize);

    file.close();
}

void FavoriteModel::saveGroup( QDomDocument &document, QDomElement *root, FavoriteTreeItem *item )
{
    QDomElement itemElement = document.createElement("group");
    itemElement.setAttribute("name", item->m_groupName);
    root->appendChild(itemElement);

    foreach ( FavoriteTreeItem *groupItem, item->m_childGroups )
        saveGroup(document, &itemElement, groupItem);
    foreach ( FavoriteTreeItem *favoriteItem, item->m_childItems )
        saveItem(document, &itemElement, favoriteItem);
}

void FavoriteModel::saveItem( QDomDocument &document, QDomElement *root, FavoriteTreeItem *favoriteItem )
{
    if ( !favoriteItem->m_favorite )
        return;

    const FavoriteData *data = favoriteItem->m_favorite->data();
    if ( !data )
        return;

    QDomElement itemElement = document.createElement("item");
    itemElement.setAttribute("id",          data->id());
    itemElement.setAttribute("name",        data->name());
    itemElement.setAttribute("description", data->description());
    QString strData = "";
    if ( data->data() )
        strData = QString(data->data()->toHex());
    itemElement.setAttribute("data",        strData);
    root->appendChild(itemElement);
}

void FavoriteModel::loadHelper( const QString &location, FavoriteTreeItem *item )
{
    QString fileName = location;
    fileName += "/favorites.xml";

    QFile file(fileName);
    if ( !file.exists() )
        return;

    QDomDocument domDocument;

    QString errorStr;
    int errorLine;
    int errorColumn;

    // Загружаем всю структуру
    if (!domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn)) {
        Core::Storage::warning( tr("Error On Load Session Favorites\nCommon Error in xml-file:\nLine %1, Column %2: %3")
                                .arg(errorLine).arg(errorColumn).arg(errorStr) );
        return;
    }

    // Первичная проверка (тип и версия документа)
    QDomElement root = domDocument.documentElement();
    if ( root.tagName() != "favorites" ) {
        Core::Storage::warning( tr("Error On Load Session Favorites\nInvalid Format of Sessions xml-file") );
        return;
    }
    if ( !root.hasAttribute("version") ) {
        Core::Storage::warning( tr("Error On Load Session Favorites\nInvalid Format of Sessions xml-file (version expected)") );
        return;
    }
    if ( root.attribute("version") != "1.0" ) {
        Core::Storage::warning( tr("Error On Load Session Favorites\nInvalid Format of Sessions xml-file (wrong version)") );
        return;
    }

    loadInternal(&root, item, index(item->row(), 0));

    file.close();
}

void FavoriteModel::loadInternal( QDomElement *root, FavoriteTreeItem *parentItem, const QModelIndex &parentIndex )
{
    QDomElement itemElement = root->firstChildElement();
    int pos;

    while ( !itemElement.isNull() ) {
        if ( itemElement.tagName() == QLatin1String("item") ) {
            QString id = itemElement.attribute("id", "");
            if ( id.isEmpty() ) {
                Core::Storage::warning( tr("Error On Load Session Favorites\nInvalid Format of Sessions xml-file (expected attribute \"id\" for tag <%1> at line %2)")
                                        .arg(itemElement.tagName()).arg(itemElement.lineNumber()) );
                return;
            }
            if ( IFavoriteFactory *factory = m_plugin->favoriteFactoryForId(id) ) {
                if ( IFavorite *favorite = factory->restoreFavorite( new FavoriteData( id,
                                                                                       itemElement.attribute("name"),
                                                                                       itemElement.attribute("description"),
                                                                                       new QByteArray(QByteArray::fromHex(itemElement.attribute("data").toLocal8Bit())) ) ) ) {
                    pos = parentItem->childCount();
                    beginInsertRows(parentIndex, pos, pos);
                    FavoriteTreeItem *favItem = new FavoriteTreeItem(favorite, parentItem);
                    parentItem->appendChildItem(favItem);
                    endInsertRows();
                }
            }
            else {
                pos = parentItem->childCount();
                beginInsertRows(parentIndex, pos, pos);
                FavoriteTreeItem *favItem = new FavoriteTreeItem(
                            new NullFavorite( tr("Not Found Favorite Factory for %1").arg(id),
                                              new FavoriteData( id,
                                                                itemElement.attribute("name"),
                                                                itemElement.attribute("description"),
                                                                new QByteArray(QByteArray::fromHex(itemElement.attribute("data").toLocal8Bit())) ) ),
                            parentItem );
                parentItem->appendChildItem(favItem);
                endInsertRows();
            }
        } else if ( itemElement.tagName() == QLatin1String("group") ) {
            pos = parentItem->m_childGroups.count();
            beginInsertRows(parentIndex, pos, pos);
            FavoriteTreeItem *groupItem = new FavoriteTreeItem(itemElement.attribute("name", "<UNKNOWN>"), parentItem);
            parentItem->appendChildGroup(groupItem);
            endInsertRows();
            loadInternal(&itemElement, groupItem, index(pos, 0, parentIndex));
        } else {
            Core::Storage::warning( tr("Error On Load Session Favorites\nInvalid Format of Sessions xml-file (unknown tag <%1> at line %2)")
                                    .arg(itemElement.tagName()).arg(itemElement.lineNumber()) );
            return;
        }
        itemElement = itemElement.nextSiblingElement();
    }
}
