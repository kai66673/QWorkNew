#include "ReportSelectDataSourceColumnDialog.h"
#include "ui_ReportSelectDataSourceColumnDialog.h"

namespace RPML {

ReportSelectDataSourceColumnDialog::ReportSelectDataSourceColumnDialog( QList<RPML::DataSource *> *sources,
                                                                        int sourceIdForced, QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::ReportSelectDataSourceColumnDialog)
    , m_sources(sources)
    , m_sourceIdForced(sourceIdForced)
    , m_id(0)
    , m_columnNumber(0)
{
    ui->setupUi(this);

    if ( m_sourceIdForced )
        ui->dataSourceComboBox->setEnabled(false);

    int index = 0;
    foreach ( RPML::DataSource *source, *m_sources ) {
        ui->dataSourceComboBox->addItem(tr("%1. %2").arg(source->m_id).arg(source->m_description));
        if ( m_sourceIdForced == source->m_id )
            ui->dataSourceComboBox->setCurrentIndex(index);
        index++;
    }

    connect(ui->dataSourceComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onDataSourceIndexChanaged(int)));
    onDataSourceIndexChanaged(ui->dataSourceComboBox->currentIndex());
}

ReportSelectDataSourceColumnDialog::~ReportSelectDataSourceColumnDialog()
{
    delete ui;
}

void ReportSelectDataSourceColumnDialog::onDataSourceIndexChanaged( int index )
{
    ui->fieldComboBox->clear();
    if ( index == -1 )
        return;

    if ( RPML::IDataSourceQuery *query = m_sources->at(index)->m_query )
        ui->fieldComboBox->addItems(*query->fields());
}

void ReportSelectDataSourceColumnDialog::on_okPushButton_clicked()
{
    int index = ui->dataSourceComboBox->currentIndex();
    if ( index == -1 )
        return;

    m_id = m_sources->at(index)->m_id;
    m_columnNumber = ui->fieldComboBox->currentIndex();
    if ( m_columnNumber == -1 ) {
        ui->fieldComboBox->setFocus();
        return;
    }

    accept();
}

}   // namespace RPML
