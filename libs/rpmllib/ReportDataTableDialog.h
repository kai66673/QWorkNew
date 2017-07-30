#ifndef REPORTDATATABLEDIALOG_H
#define REPORTDATATABLEDIALOG_H

#include <QDialog>
#include <QAbstractTableModel>
#include <QItemDelegate>

//#include "QxUiEditReportDialog.h"
#include "DataSource.h"
#include "ReportData.h"

namespace Ui {
class ReportDataTableDialog;
}

namespace RPML {

class DataSourceProvider
{
public:
    DataSourceProvider( RPML::DataSource *source );

    void setDataSource( RPML::DataSource *source );
    inline RPML::DataSource *dataSource();

private:
    RPML::DataSource *m_source;
};

class DataTableGroupingDelegeate: public QItemDelegate
{
public:
    DataTableGroupingDelegeate( DataSourceProvider *sourceProvider, QObject *parent = 0 );

    QWidget *createEditor( QWidget *parent, const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;

    void setEditorData( QWidget *editor, const QModelIndex &index) const;
    void setModelData( QWidget *editor, QAbstractItemModel *model,
                       const QModelIndex &index) const;

    void updateEditorGeometry( QWidget *editor,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index) const;

private:
    DataSourceProvider *m_sourceProvider;
};

class DataTableGroupingModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    DataTableGroupingModel( DataSourceProvider *sourceProvider,
                            QList<RPML::DataTableGroupingInfo *> *groups,
                            QObject *parent = 0 );

    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const;
    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex &index ) const;

    bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole );

    void clearGrouping();
    bool appendGrouping();
    void removeGrouping();

signals:
    void groupingRowsCountChanged();

private:
    bool checkColumnUsed( int columnNumber, int currentRow );
    DataSourceProvider *m_sourceProvider;
    QList<RPML::DataTableGroupingInfo *> *m_groups;
};

class ReportDataTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReportDataTableDialog( QList<RPML::DataSource *> *sources, QWidget *parent = 0 );
    virtual ~ReportDataTableDialog();

    void fillDataTableParameters( RPML::ReportDataTable *dataTable, int *columnCount,
                                  qreal *padding, qreal *spacing ) const;

private slots:
    void onDataSourceChanged( int index );

    void on_okButton_clicked();
    void on_addGroupingToolButton_clicked();
    void on_delGroupingToolButton_clicked();
    void on_clearGroupingToolButton_clicked();
    void on_headerRowsCountSpinBox_valueChanged( int arg1 );
    void on_detailsRowsCountSpinBox_valueChanged( int arg1 );
    void on_footerRowsCountSpinBox_valueChanged( int arg1 );

    void recalculateRowsCount();

private:
    Ui::ReportDataTableDialog *ui;
    QList<RPML::DataSource *> *m_sources;
    DataTableGroupingModel *m_groupingModel;
    QList<RPML::DataTableGroupingInfo *> m_groupings;
    DataSourceProvider *m_sourceProvider;
};

}   // namespace RPML

#endif // REPORTDATATABLEDIALOG_H
