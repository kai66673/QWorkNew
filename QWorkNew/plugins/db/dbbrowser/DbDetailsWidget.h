#ifndef DBDETAILSWIDGET_H
#define DBDETAILSWIDGET_H

#include <QWidget>
#include <QModelIndex>
#include <QTabBar>

#include "DbCatalog.h"
#include "DbLoader.h"
#include "DbConnection.h"
#include "StyleManager.h"

QT_BEGIN_NAMESPACE
class QStackedWidget;
QT_END_NAMESPACE

namespace Database {

class DbDetailsItem: public QObject
{
    Q_OBJECT

public:
    DbDetailsItem( IDbCatalogItem *detailsItem, IDbCatalogItem *selectedItem );

    inline IDbCatalogItem *detailsItem() const { return m_detailsItem; }
    inline IDbCatalogItem *selectedItem() const { return m_selectedItem; }

private:
    IDbCatalogItem *m_detailsItem;
    IDbCatalogItem *m_selectedItem;
};

class DbDetailsTabBar: public QTabBar, public Utils::StyleManagedWidget
{
    Q_OBJECT

public:
    DbDetailsTabBar(int qssDetailsWathcherIndex, QWidget *parent = 0);
};

class DbDetailsTabWidget: public QTabWidget
{
    Q_OBJECT

public:
    DbDetailsTabWidget(int qssDetailsWathcherIndex, QWidget *parent = 0);
};

class DbDetailsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DbDetailsWidget(DbConnection *connection, BaseConnectionInfo *connectionInfo, IDbLoader *loader,
                             const QString &dbDescription, int qssDetailsWathcherIndex, QWidget *parent = 0);
    ~DbDetailsWidget();

    void clear();
    void next();
    void previous();

    void addDbDetails( IDbCatalogItem *selectedItem );
    
public slots:
    void onItemDetailsJobExecuted( IDbDetailsFactory *detailsFactory, DbDetailsItem *details );

private slots:
    void onDetailsTabClose( int index );

private:
    void navigateToSelectedItem( IDbCatalogItem *item, int index );

private:
    QString m_dbDescription;
    BaseConnectionInfo *m_connectionInfo;
    IDbLoader *m_loader;
    DbConnection *m_connection;

    QList<DbDetailsItem *> m_details;

    QStackedWidget *m_stackedWidget;
    DbDetailsTabWidget *m_detailsTabWidget;
};

} // Database

#endif // DBDETAILSWIDGET_H
