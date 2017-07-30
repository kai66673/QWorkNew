#include <QStackedWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QApplication>
#include <QToolBar>

#include <QDebug>

#ifdef HAVE_QT5
#include <QtConcurrent/QtConcurrentRun>
#else
#include <QtCore/qtconcurrentrun.h>
#endif

#include "DbDetailsWidget.h"
#include "DbDetails.h"
#include "DbManager.h"

namespace Database {

static IDbDetailsFactory *getDbItemDetails( IDbCatalogItem *item, IDbLoader *loader, const QString &connName )
{
    IDbDetailsFactory *f = loader->createDbDetailsFactory( connName,
                                                           item->schemaName(),
                                                           item->schemaObjectType(),
                                                           item->schemaObjectSubtype(),
                                                           item->name() );
    return f;
}

DbDetailsItem::DbDetailsItem( IDbCatalogItem *detailsItem, IDbCatalogItem *selectedItem )
    : QObject(0)
    , m_detailsItem(detailsItem)
    , m_selectedItem(selectedItem)
{ }

DbDetailsTabBar::DbDetailsTabBar(int qssDetailsWathcherIndex, QWidget *parent)
    : QTabBar(parent)
    , Utils::StyleManagedWidget(this, qssDetailsWathcherIndex)
{ }

DbDetailsTabWidget::DbDetailsTabWidget(int qssDetailsWathcherIndex, QWidget *parent)
    : QTabWidget(parent)
{
    setStyleSheet("QTabWidget::pane { border-top: 1px solid #C2C7CB; }");
    setTabsClosable(true);
    setTabBar(new DbDetailsTabBar(qssDetailsWathcherIndex, this));
    QwTabCornerToolBar *toolBar = new QwTabCornerToolBar();
    toolBar->addActions(DbManager::getInstance()->dbDetailsActions());
    setCornerWidget(toolBar, Qt::TopLeftCorner);
}

DbDetailsWidget::DbDetailsWidget(DbConnection *connection, BaseConnectionInfo *connectionInfo, IDbLoader *loader,
                                 const QString &dbDescription, int qssDetailsWathcherIndex, QWidget *parent )
    : QWidget(parent)
    , m_dbDescription(dbDescription)
    , m_connectionInfo(connectionInfo)
    , m_loader(loader)
    , m_connection(connection)
{
    m_stackedWidget = new QStackedWidget(this);
    QLabel *dbDescrLabel = new QLabel(dbDescription, m_stackedWidget);
    m_detailsTabWidget = new DbDetailsTabWidget(qssDetailsWathcherIndex, m_stackedWidget);
    m_stackedWidget->addWidget(dbDescrLabel);
    m_stackedWidget->addWidget(m_detailsTabWidget);
    m_stackedWidget->setCurrentIndex(0);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_stackedWidget);
    setLayout(layout);
    layout->setMargin(0);
    layout->setSpacing(0);

    connect(m_detailsTabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(onDetailsTabClose(int)));
}

DbDetailsWidget::~DbDetailsWidget()
{
    qDeleteAll(m_details);
}

void DbDetailsWidget::clear()
{
    qDeleteAll(m_details);
    m_details.clear();
    m_detailsTabWidget->clear();
    m_stackedWidget->setCurrentIndex(0);
}

void DbDetailsWidget::next()
{
    int detailsCount = m_detailsTabWidget->count();
    if ( detailsCount > 1 ) {
        int currentIndex = m_detailsTabWidget->currentIndex();
        currentIndex++;
        if ( currentIndex == detailsCount )
            currentIndex = 0;
        m_detailsTabWidget->setCurrentIndex(currentIndex);
    }
}

void DbDetailsWidget::previous()
{
    int detailsCount = m_detailsTabWidget->count();
    if ( detailsCount > 1 ) {
        int currentIndex = m_detailsTabWidget->currentIndex();
        currentIndex--;
        if ( currentIndex == -1 )
            currentIndex = detailsCount - 1;
        m_detailsTabWidget->setCurrentIndex(currentIndex);
    }
}

void DbDetailsWidget::addDbDetails( IDbCatalogItem *selectedItem )
{
    if ( !selectedItem )
        return;

    IDbCatalogItem *detailsItem = selectedItem->dbDetailItem();
    if ( !detailsItem )
        return;

    int i = 0;
    foreach ( DbDetailsItem *d, m_details ) {
        if ( d->detailsItem() == detailsItem ) {
            m_detailsTabWidget->setCurrentIndex(i);
            m_stackedWidget->setCurrentIndex(1);
            navigateToSelectedItem(selectedItem, i);
            return;
        }
        i++;
    }

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    DbDetailsItem *newItem = new DbDetailsItem(detailsItem, selectedItem);
    IDbDetailsFactory *detailsFactory = getDbItemDetails(detailsItem, m_loader, m_connection->connectionName());
    onItemDetailsJobExecuted(detailsFactory, newItem);

    QApplication::restoreOverrideCursor();
}

void DbDetailsWidget::onItemDetailsJobExecuted(IDbDetailsFactory *detailsFactory, DbDetailsItem *details )
{
    QWidget *dw = 0;
    if ( detailsFactory ) {
        dw = detailsFactory->createDetailsWidget(m_detailsTabWidget);
        delete detailsFactory;
    }
    if ( !dw )
        dw = new QLabel(tr("Not Implemented!(?)"), m_detailsTabWidget);

    m_detailsTabWidget->addTab( dw, QIcon(details->detailsItem()->iconName(m_loader)),
                                details->detailsItem()->name() );
    m_detailsTabWidget->setCurrentIndex(m_detailsTabWidget->count() - 1);
    m_stackedWidget->setCurrentIndex(1);

    navigateToSelectedItem(details->selectedItem(), m_detailsTabWidget->count() - 1);
    m_details.append(details);
}

void DbDetailsWidget::onDetailsTabClose( int index )
{
    if ( index == -1 )
        return;

    QWidget *delWidget = m_detailsTabWidget->widget(index);
    m_detailsTabWidget->removeTab(index);
    delWidget->deleteLater();
    delete m_details[index];
    m_details.removeAt(index);

    if ( !m_detailsTabWidget->count() )
        m_stackedWidget->setCurrentIndex(0);
}

void DbDetailsWidget::navigateToSelectedItem( IDbCatalogItem *item, int index )
{
    if ( item ) {
        if ( IDetailsNavigator *navigator = dynamic_cast<IDetailsNavigator *>(m_detailsTabWidget->widget(index)) ) {
            navigator->navigateToItem(item);
        }
    }
}

} // Database
