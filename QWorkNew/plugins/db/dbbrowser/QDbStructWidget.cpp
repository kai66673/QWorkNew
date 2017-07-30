#include <QtGui>
#include <QVBoxLayout>

#include "DbLoader.h"
#include "DbCatalog.h"
#include "DbConnection.h"
#include "QDbStructWidget.h"
#include "DbManager.h"

DbFilterModel::DbFilterModel( Database::DbFilter *filter, QObject *parent )
    : QSortFilterProxyModel(parent)
    , m_filter(filter)
{
}

void DbFilterModel::updateFilterModel( Database::DbFilter *filter )
{
    m_filter = filter;
    invalidateFilter();
}

bool DbFilterModel::filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
{
    if ( m_filter ) {
        if ( m_filter->filterType != Database::DbFilter::NoFilter && !m_filter->filterText.isEmpty() ) {
            QModelIndex childIndex = sourceModel()->index(sourceRow, 0, sourceParent);
            if ( childIndex.isValid() ) {
                if ( Database::IDbCatalogItem *item = static_cast<Database::IDbCatalogItem *>(childIndex.internalPointer()) ) {
                    if ( item->isNameFiltered() ) {
                        switch ( m_filter->filterType ) {
                            case Database::DbFilter::StartsWith:    return item->name().startsWith(m_filter->filterText, Qt::CaseInsensitive);
                            case Database::DbFilter::EndsWith:      return item->name().endsWith(m_filter->filterText, Qt::CaseInsensitive);
                            case Database::DbFilter::Contains:      return item->name().contains(m_filter->filterText, Qt::CaseInsensitive);
                        }
                    }
                }
            }
        }
    }

    return true;
}

QDbStructWidget::QDbStructWidget( Database::BaseConnectionInfo *connectionInfo,
                                  Database::IDbLoader *aLoader,
                                  QMainWindow *mainWindow_,
                                  QWidget *parent )
    : QWidget(parent)
    , m_connectionInfo(connectionInfo)
    , loader(aLoader)
    , mainWindow(mainWindow_)
{
    verticalLayout = new QVBoxLayout(this);

    verticalLayout->setSpacing(0);
    verticalLayout->setMargin(0);

    structTreeView = new DbStructTreeView(mainWindow, this);
    structTreeView->setHeaderHidden(true);
    m_objectsModel = new Database::DbCatalogModel(m_connectionInfo, loader);
    m_filterModel = new DbFilterModel();
    m_filterModel->setSourceModel(m_objectsModel);
    structTreeView->setModel(m_filterModel);
    m_rootItem = m_objectsModel->rootItem();

    verticalLayout->addWidget(structTreeView);
    structTreeView->setFrameShape(QFrame::NoFrame);
    structTreeView->setLineWidth(0);
}

QDbStructWidget::~QDbStructWidget()
{
    if ( m_connectionInfo )
        delete m_connectionInfo;
}

Database::IDbCatalogItem *QDbStructWidget::getCurrentItem( int childIndex ) const
{
    return structTreeView->getCurrentItem( childIndex );
}

Database::IDbLoader *QDbStructWidget::getCurrentLoader() const
{
    return structTreeView->getCurrentLoader();
}

void QDbStructWidget::updateFilterModel( Database::DbFilter *filter )
{
    m_filterModel->updateFilterModel(filter);
}

void QDbStructWidget::refreshDbModel( Database::DbFilter *filter )
{
    Database::DbCatalogModel *oldModel = m_objectsModel;
    DbFilterModel *oldFilterModel = m_filterModel;

    m_objectsModel = new Database::DbCatalogModel(m_connectionInfo, loader);
    m_filterModel = new DbFilterModel(filter);
    m_filterModel->setSourceModel(m_objectsModel);
    structTreeView->setModel(m_filterModel);
    m_rootItem = m_objectsModel->rootItem();

    oldFilterModel->deleteLater();
    oldModel->deleteLater();
}

