#include "Core.h"
#include "QLalrOracleAuthWidget.h"
#include "ui_QLalrOracleAuthWidget.h"

QLalrOracleAuthWidget::QLalrOracleAuthWidget( QWidget *parent )
    : Database::IDbConnectWidget(parent)
    , ui(new Ui::QLalrOracleAuthWidget)
{
    ui->setupUi(this);
}

QLalrOracleAuthWidget::~QLalrOracleAuthWidget()
{
    delete ui;
}

bool QLalrOracleAuthWidget::checkFillAuthParameters( QStringList &parameters )
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
