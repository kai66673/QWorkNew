#ifndef QDBSTRUCTWIDGET_H
#define QDBSTRUCTWIDGET_H

#include <QWidget>
#include <QSortFilterProxyModel>

#include "DbStructTreeView.h"

QT_BEGIN_NAMESPACE
class QTreeView;
class QVBoxLayout;
QT_END_NAMESPACE

namespace Database {
    class BaseConnectionInfo;
    class IDbLoader;
    class DbCatalogModel;
    class IDbCatalogRootItem;
    struct DbFilter;
}

class DbFilterModel: public QSortFilterProxyModel
{
    Q_OBJECT

public:
    DbFilterModel( Database::DbFilter *filter = 0, QObject *parent = 0 );

    void updateFilterModel( Database::DbFilter *filter );

protected:
    bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const;

private:
    Database::DbFilter *m_filter;
};

class QDbStructWidget : public QWidget
{
    Q_OBJECT

public:
    QDbStructWidget( Database::BaseConnectionInfo *connectionInfo,
                     Database::IDbLoader *aLoader,
                     QMainWindow *mainWindow_,
                     QWidget *parent = 0 );
    ~QDbStructWidget();

    Database::IDbCatalogItem *getCurrentItem( int childIndex = -1 ) const;
    Database::IDbLoader *getCurrentLoader() const;

    inline Database::IDbCatalogRootItem *rootItem() const { return m_rootItem; }
    inline Database::DbCatalogModel *objectsModel() const { return m_objectsModel; }

    void updateFilterModel( Database::DbFilter *filter );

    void refreshDbModel( Database::DbFilter *filter );

private:
    Database::BaseConnectionInfo *m_connectionInfo;

    Database::IDbLoader *loader;
    QMainWindow *mainWindow;

    QVBoxLayout *verticalLayout;

    DbStructTreeView   *structTreeView;

    Database::IDbCatalogRootItem *m_rootItem;
    Database::DbCatalogModel *m_objectsModel;
    DbFilterModel *m_filterModel;
};

#endif // QDBSTRUCTWIDGET_H
