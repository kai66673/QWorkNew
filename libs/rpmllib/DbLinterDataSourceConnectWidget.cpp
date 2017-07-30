#include "DbLinterDataSourceConnectWidget.h"
#include "ui_DbLinterDataSourceConnectWidget.h"

DbLinterDataSourceConnectWidget::DbLinterDataSourceConnectWidget( QWidget *parent )
    : RPML::IDataSourceConnectWidget(parent)
    , ui(new Ui::DbLinterDataSourceConnectWidget)
{
    ui->setupUi(this);
}

DbLinterDataSourceConnectWidget::~DbLinterDataSourceConnectWidget()
{
    delete ui;
}

bool DbLinterDataSourceConnectWidget::checkFillConnectParameters( QList<RPML::DataSource::ConnectParameter> &connectParameters )
{
    connectParameters.clear();

    QString strValue;

    connectParameters.push_back(RPML::DataSource::ConnectParameter("database", ui->databaseLineEdit->text().trimmed()));

    strValue = ui->portLineEdit->text().trimmed();
    if ( !strValue.isEmpty() ) {
        bool ok;
        strValue.toInt(&ok);
        if ( !ok ) {
            ui->portLineEdit->setFocus();
            return false;
        }
    }
    connectParameters.push_back(RPML::DataSource::ConnectParameter("port", strValue));

    strValue = ui->userLineEdit->text().trimmed();
    if ( strValue.isEmpty() ) {
        ui->userLineEdit->setFocus();
        return false;
    }
    connectParameters.push_back(RPML::DataSource::ConnectParameter("user", strValue));

    connectParameters.push_back(RPML::DataSource::ConnectParameter("password", ui->passwordLineEdit->text()));

    return true;
}
