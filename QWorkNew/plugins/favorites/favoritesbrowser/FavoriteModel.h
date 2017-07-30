#ifndef FAVORITEMODEL_H
#define FAVORITEMODEL_H

#include <QAbstractItemModel>
#include <QDomDocument>
#include <QDomElement>

#include "IFavorite.h"

class FavoritesPlugin;
class FavoriteModel;

class FavoriteTreeItem
{
    friend class FavoriteModel;

public:
    FavoriteTreeItem( const QString &groupName, FavoriteTreeItem *parent );
    FavoriteTreeItem( IFavorite *favorite, FavoriteTreeItem *parent );
    virtual ~FavoriteTreeItem();

    void appendChildGroup( FavoriteTreeItem *child ) { m_childGroups.append(child); }
    void appendChildItem( FavoriteTreeItem *child ) { m_childItems.append(child); }
    void removeChildGroup( int index ) { delete m_childGroups[index]; m_childGroups.removeAt(index); }
    void removeChildItem( int index ) { delete m_childItems[index]; m_childItems.removeAt(index); }
    void removeAllChildrens();
    void removeChild( int index );

    FavoriteTreeItem *child( int row );

    int childCount() const { return m_childGroups.size() + m_childItems.size(); }
    int childGroupsCount() const { return m_childGroups.size(); }
    int childItemsCount() const { return m_childItems.size(); }

    int columnCount() const { return 1; }
    int row() const;

    FavoriteTreeItem *parent() { return m_parent; }

    bool isGroup() const { return m_favorite == 0; }

private:
    FavoriteTreeItem *m_parent;
    IFavorite *m_favorite;
    QString m_groupName;

    QList <FavoriteTreeItem *> m_childGroups;
    QList <FavoriteTreeItem*> m_childItems;
};

class FavoriteModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit FavoriteModel( FavoritesPlugin *plugin, QObject *parent = 0 );

    QVariant data( const QModelIndex &index, int role ) const;
    Qt::ItemFlags flags( const QModelIndex &index ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
    QModelIndex parent( const QModelIndex &index ) const;
    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const;

    bool isGroup( const QModelIndex &index ) const;
    void execute( const QModelIndex &index ) const;
    void insertGroup( const QModelIndex &index );
    void insertItem( const QModelIndex &index );
    void remove( const QModelIndex &index );

    void sessionClose();
    void saveSession( const QString &location );
    void loadSession( const QString &location );

    void saveApplication( const QString &location );
    void loadApplication( const QString &location );

    bool canDelete( const QModelIndex &index );

signals:

public slots:

private:
    FavoritesPlugin *m_plugin;

    void clearChildrens( FavoriteTreeItem *item );
    void saveHelper( const QString &location, FavoriteTreeItem *item );
    void saveGroup( QDomDocument &document , QDomElement *root, FavoriteTreeItem *item );
    void saveItem( QDomDocument &document , QDomElement *root, FavoriteTreeItem *favoriteItem );
    void loadHelper( const QString &location, FavoriteTreeItem *item );
    void loadInternal( QDomElement *root, FavoriteTreeItem *parentItem , const QModelIndex &parentIndex );

    FavoriteTreeItem *m_rootItem;
    FavoriteTreeItem *m_applicationRootItem;
    FavoriteTreeItem *m_sessionRootItem;

};

#endif // FAVORITEMODEL_H
