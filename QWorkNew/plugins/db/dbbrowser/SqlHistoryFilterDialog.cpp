#include "SqlHistoryFilterDialog.h"
#include "ui_SqlHistoryFilterDialog.h"

SqlHistoryFilterDialog::SqlHistoryFilterDialog( QSqlHistorySortFilterProxyModel *model, QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::SqlHistoryFilterDialog)
    , m_model(model)
{
    ui->setupUi(this);
    initFilterData();
}

SqlHistoryFilterDialog::~SqlHistoryFilterDialog()
{
    delete ui;
}

void SqlHistoryFilterDialog::onFilterTypeChanged( int index )
{
    switch ( index ) {
        case 0:
            ui->startDateTimeEdit->setEnabled(false);
            ui->endDateTimeEdit->setEnabled(false);
            break;
        case 1:
            ui->startDateTimeEdit->setEnabled(true);
            ui->endDateTimeEdit->setEnabled(false);
            break;
        case 2:
            ui->startDateTimeEdit->setEnabled(false);
            ui->endDateTimeEdit->setEnabled(true);
            break;
        case 3:
            ui->startDateTimeEdit->setEnabled(true);
            ui->endDateTimeEdit->setEnabled(true);
            break;
    }
}

void SqlHistoryFilterDialog::tryAccept()
{
    switch ( ui->filterTypeComboBox->currentIndex() ) {
        case 0: {
            QSqlHistorySortFilterProxyModel::FilterParams params;
            m_model->setFilterParams(params);
            break;
        }
        case 1: {
            QSqlHistorySortFilterProxyModel::FilterParams params;
            params.filterType = QSqlHistorySortFilterProxyModel::StartDate;
            params.startDate = ui->startDateTimeEdit->dateTime();
            m_model->setFilterParams(params);
            break;
        }
        case 2: {
            QSqlHistorySortFilterProxyModel::FilterParams params;
            params.filterType = QSqlHistorySortFilterProxyModel::EndDate;
            params.endDate = ui->endDateTimeEdit->dateTime();
            m_model->setFilterParams(params);
            break;
        }
        case 3: {
            QSqlHistorySortFilterProxyModel::FilterParams params;
            params.filterType = QSqlHistorySortFilterProxyModel::PeriodDate;
            params.startDate = ui->startDateTimeEdit->dateTime();
            params.endDate = ui->endDateTimeEdit->dateTime();
            m_model->setFilterParams(params);
            break;
        }
        default:
            // WTF!
            return;
    }

    accept();
}


void SqlHistoryFilterDialog::initFilterData()
{
    QSqlHistorySortFilterProxyModel::FilterParams params = m_model->filterParams();

    onFilterTypeChanged(params.filterType);
    ui->filterTypeComboBox->setCurrentIndex(params.filterType);

    if ( params.startDate.isValid() )
        ui->startDateTimeEdit->setDateTime(params.startDate);
    else
        ui->startDateTimeEdit->setDateTime(QDateTime::currentDateTime());

    if ( params.endDate.isValid() )
        ui->endDateTimeEdit->setDateTime(params.endDate);
    else
        ui->endDateTimeEdit->setDateTime(QDateTime::currentDateTime());

    connect(ui->filterTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterTypeChanged(int)));
    connect(ui->applyPushButton, SIGNAL(clicked()), this, SLOT(tryAccept()));
}
