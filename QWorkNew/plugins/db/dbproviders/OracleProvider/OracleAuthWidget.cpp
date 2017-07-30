#include "Core.h"
#include "OracleAuthWidget.h"
#include "ui_OracleAuthWidget.h"

OracleAuthWidget::OracleAuthWidget( QWidget *parent )
    : Database::IDbConnectWidget(parent)
    , ui(new Ui::OracleAuthWidget)
{
    ui->setupUi(this);
}

OracleAuthWidget::~OracleAuthWidget()
{
    delete ui;
}

bool OracleAuthWidget::checkFillAuthParameters( QStringList &parameters )
{
    QString dbName = ui->databaseLineEdit->text().trimmed();
    ui->databaseLineEdit->setText(dbName);
    if ( dbName.isEmpty() ) {
        Core::Storage::warning(tr("Database name cannot be empty"));
        ui->databaseLineEdit->setFocus();
        return false;
    }

    QString userName = ui->userLineEdit->text().trimmed();
    ui->userLineEdit->setText(userName);
    if ( userName.isEmpty() ) {
        Core::Storage::warning(tr("User name cannot be empty"));
        ui->userLineEdit->setFocus();
        return false;
    }

    QString password = ui->passwordLineEdit->text();

    parameters << dbName
               << userName
               << password;

    return true;
}
