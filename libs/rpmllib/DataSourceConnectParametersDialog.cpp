#include "DataSourceConnectParametersDialog.h"
#include "ui_DataSourceConnectParametersDialog.h"

DataSourceConnectParametersDialog::DataSourceConnectParametersDialog( QList<RPML::IDataSourceProvider *> *providers, QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::DataSourceConnectParametersDialog)
    , m_providers(providers)
{
    ui->setupUi(this);

    for (int i = 0; i < m_providers->size(); i++) {
        RPML::IDataSourceProvider *provider = m_providers->at(i);
        ui->dataSourceTypeComboBox->addItem(QString("%1. %2").arg(provider->typeId()).arg(provider->typeDescription()));
        RPML::IDataSourceConnectWidget *connectWidget = provider->createConnectWidget(this);
        ui->stackedWidget->addWidget(connectWidget);
        m_connectWidgets.push_back(connectWidget);
    }

    connect(ui->dataSourceTypeComboBox, SIGNAL(currentIndexChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
    ui->dataSourceTypeComboBox->setFocus();
    ui->dataSourceTypeComboBox->setCurrentIndex(0);
}

DataSourceConnectParametersDialog::~DataSourceConnectParametersDialog()
{
    delete ui;
}

RPML::IDataSourceProvider *DataSourceConnectParametersDialog::selectedProvider() const
{
    return m_providers->at(ui->dataSourceTypeComboBox->currentIndex());
}

QList<RPML::DataSource::ConnectParameter> DataSourceConnectParametersDialog::dataSourceConnectParameters() const
{
    return m_connectParameters;
}

void DataSourceConnectParametersDialog::on_okButton_clicked()
{
    int dsTypeIndex = ui->dataSourceTypeComboBox->currentIndex();
    if ( dsTypeIndex == -1 ) {
        ui->dataSourceTypeComboBox->setFocus();
        return;
    }

    RPML::IDataSourceConnectWidget *connectWidget = m_connectWidgets[dsTypeIndex];
    m_connectParameters.clear();
    if ( connectWidget->checkFillConnectParameters(m_connectParameters) )
        accept();
}
