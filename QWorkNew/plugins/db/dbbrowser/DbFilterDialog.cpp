#include "DbConnection.h"
#include "DbFilterDialog.h"
#include "ui_DbFilterDialog.h"

DbFilterDialog::DbFilterDialog( Database::DbFilter *filter, QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::DbFilterDialog)
    , m_filter(filter)
{
    ui->setupUi(this);

    switch ( m_filter->filterType ) {
        case Database::DbFilter::NoFilter:      ui->filterTypeComboBox->setCurrentIndex(0); break;
        case Database::DbFilter::StartsWith:    ui->filterTypeComboBox->setCurrentIndex(1); break;
        case Database::DbFilter::EndsWith:      ui->filterTypeComboBox->setCurrentIndex(2); break;
        case Database::DbFilter::Contains:      ui->filterTypeComboBox->setCurrentIndex(3); break;
        default: ui->filterTypeComboBox->setCurrentIndex(-1);
    }

    ui->filterTextLineEdit->setText(m_filter->filterText);
    if ( ui->filterTypeComboBox->currentIndex() < 1 )
        ui->filterTextLineEdit->setEnabled(false);

    connect(ui->filterTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFilterTypeChanged(int)));
    connect(ui->okPushButton, SIGNAL(clicked()), this, SLOT(onAccept()));
}

DbFilterDialog::~DbFilterDialog()
{
    delete ui;
}

void DbFilterDialog::onFilterTypeChanged( int index )
{
    ui->filterTextLineEdit->setEnabled( index > 0 );
}

void DbFilterDialog::onAccept()
{
    m_filter->filterText = ui->filterTextLineEdit->text().trimmed();

    switch ( ui->filterTypeComboBox->currentIndex() ) {
        case 0: m_filter->filterType = Database::DbFilter::NoFilter;    break;
        case 1: m_filter->filterType = Database::DbFilter::StartsWith;  break;
        case 2: m_filter->filterType = Database::DbFilter::EndsWith;    break;
        case 3: m_filter->filterType = Database::DbFilter::Contains;    break;
        default: return;
    }

    accept();
}
