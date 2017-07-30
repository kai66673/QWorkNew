#ifndef QSQLHISTORYTABLEVIEW_H
#define QSQLHISTORYTABLEVIEW_H

#include <QTableView>
#include <QAction>
#include <QSortFilterProxyModel>
#include <QDateTime>

class QSqlHistorySortFilterProxyModel: public QSortFilterProxyModel
{
    Q_OBJECT

public:
    QSqlHistorySortFilterProxyModel( QObject *parent = 0 )
        : QSortFilterProxyModel(parent)
    {}

    enum FilterType {
        NoFilter = 0,
        StartDate,
        EndDate,
        PeriodDate
    };

    struct FilterParams {
        FilterParams()
            : filterType(NoFilter)
            , startDate(QDateTime())
            , endDate(QDateTime())
        {}

        FilterType filterType;
        QDateTime  startDate;
        QDateTime  endDate;
    };

    void setFilterParams( const FilterParams &filterParams );
    FilterParams filterParams() const { return m_filterParams; }

protected:
    bool filterAcceptsRow( int sourceRow, const QModelIndex &sourceParent ) const;

private:
    FilterParams m_filterParams;
};

class QSqlHistoryTableView : public QTableView
{
    Q_OBJECT

public:
    explicit QSqlHistoryTableView( QWidget *parent = 0 );

    void setModel( QAbstractItemModel *model );

    inline QMenu *contextMenu() const { return m_contextMenu; }

protected:
    void mouseDoubleClickEvent( QMouseEvent *event );
    void keyPressEvent( QKeyEvent *event );
    void contextMenuEvent( QContextMenuEvent *event );
    
signals:
    void sqlHistohyItemSelected( bool onoff );
    
public slots:
    void onOpen();
    void onRemove();
    void onClear();
    void onCurrentRowChanged( const QModelIndex &current, const QModelIndex &previous );
    void onFilterHistory();

    void onToggleHistoryEnabled( bool enabled );

private:
    QMenu *m_contextMenu;
};

#endif // QSQLHISTORYTABLEVIEW_H
