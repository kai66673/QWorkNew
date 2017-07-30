#include <QMessageBox>

#include "ReportDataTableDialog.h"
#include "ui_ReportDataTableDialog.h"
#include "Core.h"

namespace RPML {

DataSourceProvider::DataSourceProvider( RPML::DataSource *source )
    : m_source(source)
{
}

void DataSourceProvider::setDataSource( RPML::DataSource *source )
{
    m_source = source;
}

RPML::DataSource *DataSourceProvider::dataSource()
{
    return m_source;
}

DataTableGroupingDelegeate::DataTableGroupingDelegeate( DataSourceProvider *sourceProvider, QObject *parent )
    : QItemDelegate(parent)
    , m_sourceProvider(sourceProvider)
{
}

QWidget *DataTableGroupingDelegeate::createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    Q_UNUSED(option)

    switch ( index.column() ) {
        case 0: {
            QComboBox *combo = new QComboBox(parent);
            RPML::DataSource *source = m_sourceProvider->dataSource();
            if ( QStringList *fields = source->queryFields() )
                for (int i = 0; i < fields->size(); i++)
                    combo->addItem(QString("%1. [%2]").arg(i + 1).arg(fields->at(i)));
            return combo;
        }
        case 1: {
            QSpinBox *spin = new QSpinBox(parent);
            spin->setMinimum(1);
            spin->setMaximum(8);
            return spin;
        }
    }
    return new QWidget(parent);
}

void DataTableGroupingDelegeate::setEditorData( QWidget *editor, const QModelIndex &index ) const
{
    switch ( index.column() ) {
        case 0: {
            QComboBox *combo = qobject_cast<QComboBox *>(editor);
            combo->setCurrentIndex(index.model()->data(index, Qt::EditRole).toInt());
            break;
        }
        case 1: {
            QSpinBox *spin = qobject_cast<QSpinBox *>(editor);
            spin->setValue(index.model()->data(index, Qt::EditRole).toInt());
            break;
        }
    }
}

void DataTableGroupingDelegeate::setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
    switch ( index.column() ) {
        case 0: {
            QComboBox *combo = qobject_cast<QComboBox *>(editor);
            model->setData(index, combo->currentIndex(), Qt::EditRole);
            break;
        }
        case 1: {
            QSpinBox *spin = qobject_cast<QSpinBox *>(editor);
            model->setData(index, spin->value(), Qt::EditRole);
            break;
        }
    }
}

void DataTableGroupingDelegeate::updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}

DataTableGroupingModel::DataTableGroupingModel( DataSourceProvider *sourceProvider,
                                                QList<RPML::DataTableGroupingInfo *> *groups,
                                                QObject *parent )
    : QAbstractTableModel(parent)
    , m_sourceProvider(sourceProvider)
    , m_groups(groups)
{
}

int DataTableGroupingModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return m_groups->size();
}

int DataTableGroupingModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return 2;
}

QVariant DataTableGroupingModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    int row = index.row();

    if ( row >= 0 && row < rowCount() && (role == Qt::EditRole || role == Qt::DisplayRole) ) {
        int columnNumber = m_groups->at(row)->columnNumber;
        if ( role == Qt::EditRole ) {
            switch ( index.column() ) {
                case 0: return columnNumber;
                case 1: return m_groups->at(row)->rowsCount;
            }
        } else if ( role == Qt::DisplayRole ) {
            switch ( index.column() ) {
                case 0:
                    if ( QStringList *fields = m_sourceProvider->dataSource()->queryFields() )
                        return QString("%1. [%2]").arg(columnNumber + 1).arg(fields->at(columnNumber));
                    return QVariant();
                case 1: return m_groups->at(row)->rowsCount;
            }
        }
    }

    return QVariant();
}

QVariant DataTableGroupingModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( role == Qt::DisplayRole && orientation == Qt::Horizontal ) {
        switch ( section ) {
            case 0: return tr("Column");
            case 1: return tr("Rows Count");
        }
    }

    return QVariant();
}

Qt::ItemFlags DataTableGroupingModel::flags( const QModelIndex &index ) const
{
    if ( !index.isValid() )
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool DataTableGroupingModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
    if ( role != Qt::EditRole || !index.isValid() )
        return false;

    int row = index.row();
    int column = index.column();
    if ( row >= 0 && row < rowCount() && column >= 0 && column < columnCount() ) {
        switch ( column ) {
            case 0:
                if ( checkColumnUsed(value.toInt(), row) )
                    return false;
                m_groups->at(row)->columnNumber = value.toInt();
                break;
            case 1:
                m_groups->at(row)->rowsCount = value.toInt();
                emit groupingRowsCountChanged();
                break;
        }
    }

    emit dataChanged(index, index);
    return true;
}

void DataTableGroupingModel::clearGrouping()
{
    beginResetModel();
    qDeleteAll(*m_groups);
    m_groups->clear();
    endResetModel();

    emit groupingRowsCountChanged();
}

bool DataTableGroupingModel::appendGrouping()
{
    if ( QStringList *fields = m_sourceProvider->dataSource()->queryFields() ) {
        int maxColumnNumber = fields->size();
        int colNumberCandidate = 0;
        while ( colNumberCandidate < maxColumnNumber ) {
            bool found = false;
            for (int i = 0; i < m_groups->size(); i++)
                if ( m_groups->at(i)->columnNumber == colNumberCandidate ) {
                    found = true;
                    break;
                }
            if ( !found ) {
                if ( colNumberCandidate >= maxColumnNumber )
                    return false;
                int pos = m_groups->size();
                beginInsertRows(QModelIndex(), pos, pos);
                m_groups->push_back(new RPML::DataTableGroupingInfo(colNumberCandidate));
                endInsertRows();
                emit groupingRowsCountChanged();
                return true;
            }
            colNumberCandidate++;
        }
    }

    return false;
}

void DataTableGroupingModel::removeGrouping()
{
    int pos = m_groups->size() - 1;
    if ( pos >= 0 ) {
        RPML::DataTableGroupingInfo *delGroup = m_groups->at(pos);
        beginRemoveRows(QModelIndex(), pos, pos);
        m_groups->removeAt(pos);
        endRemoveRows();
        delete delGroup;
        emit groupingRowsCountChanged();
    }
}

bool DataTableGroupingModel::checkColumnUsed( int columnNumber, int currentRow )
{
    for (int i = 0; i < m_groups->size(); i++)
        if ( i != currentRow && m_groups->at(i)->columnNumber == columnNumber )
            return true;
    return false;
}

ReportDataTableDialog::ReportDataTableDialog( QList<RPML::DataSource *> *sources, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ReportDataTableDialog)
    , m_sources(sources)
{
    ui->setupUi(this);

    foreach ( RPML::DataSource *src, *m_sources )
        ui->dataSourcesComboBox->addItem(tr("%1. %2").arg(src->m_id).arg(src->m_description));

    connect(ui->dataSourcesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onDataSourceChanged(int)));

    m_sourceProvider = new DataSourceProvider( m_sources->at(ui->dataSourcesComboBox->currentIndex()) );
    m_groupingModel = new DataTableGroupingModel(m_sourceProvider, &m_groupings);
    ui->groupingTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->groupingTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->groupingTableView->setModel(m_groupingModel);
    ui->groupingTableView->setItemDelegate(new DataTableGroupingDelegeate(m_sourceProvider));
    ui->groupingTableView->setEditTriggers(QAbstractItemView::AllEditTriggers);

    connect(m_groupingModel, SIGNAL(groupingRowsCountChanged()), this, SLOT(recalculateRowsCount()));
}

ReportDataTableDialog::~ReportDataTableDialog()
{
    delete ui;
    qDeleteAll(m_groupings);
    delete m_sourceProvider;
}

void ReportDataTableDialog::fillDataTableParameters( RPML::ReportDataTable *dataTable, int *columnCount,
                                                     qreal *padding, qreal *spacing ) const
{
    dataTable->setSourceId(m_sources->at(ui->dataSourcesComboBox->currentIndex())->m_id);
    dataTable->setHeaderRowsCount(ui->headerRowsCountSpinBox->value());
    dataTable->setDetailRowsCount(ui->detailsRowsCountSpinBox->value());
    dataTable->setFooterRowsCount(ui->footerRowsCountSpinBox->value());
    *columnCount = ui->columnsCountSpinBox->value();
    *padding = ui->padiingSpinBox->value();
    *spacing = ui->spacingSpinBox->value();

    foreach ( RPML::DataTableGroupingInfo *grouping, m_groupings )
        dataTable->addGrouping( new RPML::DataTableGroupingInfo(grouping->columnNumber, grouping->rowsCount) );
}

void ReportDataTableDialog::onDataSourceChanged( int index )
{
    m_groupingModel->clearGrouping();
    m_sourceProvider->setDataSource(m_sources->at(index));
    recalculateRowsCount();
}

void ReportDataTableDialog::on_okButton_clicked()
{
    QDialog::accept();
}

void ReportDataTableDialog::on_addGroupingToolButton_clicked()
{
    if ( !m_groupingModel->appendGrouping() )
        Core::Storage::warning( tr("Not Found Unused Query Columns") );

}

void ReportDataTableDialog::on_delGroupingToolButton_clicked()
{
    m_groupingModel->removeGrouping();
}

void ReportDataTableDialog::on_clearGroupingToolButton_clicked()
{
    m_groupingModel->clearGrouping();
}

void ReportDataTableDialog::on_headerRowsCountSpinBox_valueChanged( int arg1 )
{
    Q_UNUSED(arg1)
    recalculateRowsCount();
}

void ReportDataTableDialog::on_detailsRowsCountSpinBox_valueChanged( int arg1 )
{
    Q_UNUSED(arg1)
    recalculateRowsCount();
}

void ReportDataTableDialog::on_footerRowsCountSpinBox_valueChanged( int arg1 )
{
    Q_UNUSED(arg1)
    recalculateRowsCount();
}

void ReportDataTableDialog::recalculateRowsCount()
{
    int rowsCount = 0;

    rowsCount += ui->headerRowsCountSpinBox->value();
    for (int i = 0; i < m_groupings.size(); i++)
        rowsCount += m_groupings[i]->rowsCount;
    rowsCount += ui->detailsRowsCountSpinBox->value();
    rowsCount += ui->footerRowsCountSpinBox->value();

    ui->totalRowsCountSpinBox->setValue(rowsCount);
}

}   // namespace RPML
