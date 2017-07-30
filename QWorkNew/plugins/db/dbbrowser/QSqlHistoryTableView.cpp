#include <QKeyEvent>
#include <QMenu>

#include "Core.h"
#include "IDocument.h"
#include "QSqlHistoryTableView.h"
#include "SqlHistoryFilterDialog.h"
#include "dbbrowser_constants.h"

void QSqlHistorySortFilterProxyModel::setFilterParams( const QSqlHistorySortFilterProxyModel::FilterParams &filterParams )
{
    m_filterParams = filterParams;
    invalidateFilter();
}

bool QSqlHistorySortFilterProxyModel::filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const
{
    if ( m_filterParams.filterType == NoFilter )
        return true;

    QModelIndex index = sourceModel()->index(sourceRow, 1, sourceParent);
    QDateTime rowDateTime = sourceModel()->data(index).toDateTime();

    if ( m_filterParams.filterType == StartDate || m_filterParams.filterType == PeriodDate ) {
        if ( rowDateTime < m_filterParams.startDate )
            return false;
    }

    if ( m_filterParams.filterType == EndDate || m_filterParams.filterType == PeriodDate ) {
        if ( rowDateTime > m_filterParams.endDate )
            return false;
    }

    return true;
}

QSqlHistoryTableView::QSqlHistoryTableView( QWidget *parent )
    : QTableView(parent)
    , m_contextMenu(new QMenu(this))
{
}

void QSqlHistoryTableView::setModel( QAbstractItemModel *model )
{
    QTableView::setModel(model);
    connect(selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(onCurrentRowChanged(QModelIndex,QModelIndex)));
}

void QSqlHistoryTableView::mouseDoubleClickEvent( QMouseEvent *event )
{
    Q_UNUSED(event);

    QModelIndex index = selectionModel()->currentIndex();
    if ( !index.isValid() )
        return;

    onOpen();
}

void QSqlHistoryTableView::keyPressEvent( QKeyEvent *event )
{
    switch ( event->key() ) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
            onOpen();
            break;
        case Qt::Key_Delete:
            if ( event->modifiers() & Qt::ShiftModifier )
                onClear();
            else
                onRemove();
            break;
        default :
            QTableView::keyPressEvent(event);
            break;
    }
}

void QSqlHistoryTableView::contextMenuEvent( QContextMenuEvent *event )
{
    if (!event)
        return;

    m_contextMenu->exec(event->globalPos());

    event->accept();
}

void QSqlHistoryTableView::onOpen()
{
    int rowIndex = currentIndex().row();
    if ( rowIndex == -1 )
        return;

    QString text = model()->data(model()->index(rowIndex, 0)).toString();
    DocumentManager::getInstance()->insertText(text, "SQL");
}

void QSqlHistoryTableView::onRemove()
{
    int index = currentIndex().row();
    if ( index == -1 )
        return;

    model()->removeRows(index, 1);
}

void QSqlHistoryTableView::onClear()
{
    int rowCount = model()->rowCount();
    model()->removeRows(0, rowCount);
}

void QSqlHistoryTableView::onCurrentRowChanged( const QModelIndex &current, const QModelIndex &previous )
{
    if ( current.row() == previous.row() )
        return;

    bool enabled = ( current.row() != -1 );
    emit sqlHistohyItemSelected(enabled);
}

void QSqlHistoryTableView::onFilterHistory()
{
    QSqlHistorySortFilterProxyModel *m = qobject_cast<QSqlHistorySortFilterProxyModel *>(model());
    SqlHistoryFilterDialog filterDialog(m, Core::Storage::mainWindow()->widget());
    filterDialog.exec();
}

void QSqlHistoryTableView::onToggleHistoryEnabled( bool enabled )
{
    SettingsManager::instance()->value(SqlBrowser::C_SQL_QUERY_SETTINGS_ID, "SqlHistory") = enabled;
}
