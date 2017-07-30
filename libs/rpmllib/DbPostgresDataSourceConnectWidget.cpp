#include "DbPostgresDataSourceConnectWidget.h"
#include "ui_DbPostgresDataSourceConnectWidget.h"

DbPostgresDataSourceConnectWidget::DbPostgresDataSourceConnectWidget( QWidget *parent )
    : RPML::IDataSourceConnectWidget(parent)
    , ui(new Ui::DbPostgresDataSourceConnectWidget)
{
    ui->setupUi(this);
}

DbPostgresDataSourceConnectWidget::~DbPostgresDataSourceConnectWidget()
{
    delete ui;
}

bool DbPostgresDataSourceConnectWidget::checkFillConnectParameters( QList<RPML::DataSource::ConnectParameter> &connectParameters )
{
    connectParameters.clear();

    QString strValue;

    strValue = ui->databaseLineEdit->text().trimmed();
    if ( strValue.isEmpty() ) {
        ui->databaseLineEdit->setFocus();
        return false;
    }
    connectParameters.push_back(RPML::DataSource::ConnectParameter("database", strValue));

    strValue = ui->hostLineEdit->text().trimmed();
    if ( strValue.isEmpty() ) {
        ui->hostLineEdit->setFocus();
        return false;
    }
    connectParameters.push_back(RPML::DataSource::ConnectParameter("host", strValue));

    bool ok;
    strValue = ui->portLineEdit->text().trimmed();
    strValue.toInt(&ok);
    if ( strValue.isEmpty() || !ok ) {
        ui->portLineEdit->setFocus();
        return false;
    }
    connectParameters.push_back(RPML::DataSource::ConnectParameter("port", strValue));

    strValue = ui->schemaLineEdit->text().trimmed();
    if ( strValue.isEmpty() ) {
        ui->schemaLineEdit->setFocus();
        return false;
    }
    connectParameters.push_back(RPML::DataSource::ConnectParameter("schema", strValue));

    strValue = ui->userLineEdit->text().trimmed();
    if ( strValue.isEmpty() ) {
        ui->userLineEdit->setFocus();
        return false;
    }
    connectParameters.push_back(RPML::DataSource::ConnectParameter("user", strValue));

    connectParameters.push_back(RPML::DataSource::ConnectParameter("password", ui->passwordLineEdit->text()));

    return true;
}
