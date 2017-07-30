#include "Core.h"

#include "PostgresAuthWidget.h"
#include "ui_PostgresAuthWidget.h"

PostgresAuthWidget::PostgresAuthWidget( QWidget *parent )
    : Database::IDbConnectWidget(parent)
    , ui(new Ui::PostgresAuthWidget)
{
    ui->setupUi(this);
}

PostgresAuthWidget::~PostgresAuthWidget()
{
    delete ui;
}

bool PostgresAuthWidget::checkFillAuthParameters( QStringList &parameters )
{
    QString dbName = ui->databaseLineEdit->text().trimmed();
    ui->databaseLineEdit->setText(dbName);
    if ( dbName.isEmpty() ) {
        Core::Storage::warning(tr("Database name cannot be empty"));
        ui->databaseLineEdit->setFocus();
        return false;
    }

    QString hostName = ui->hostLineEdit->text().trimmed();
    QString portNumber = ui->portLineEdit->text().trimmed();

    QString userName = ui->userLineEdit->text().trimmed();
    ui->userLineEdit->setText(userName);
    if ( userName.isEmpty() ) {
        Core::Storage::warning(tr("User name cannot be empty"));
        ui->userLineEdit->setFocus();
        return false;
    }

    QString password = ui->passwordLineEdit->text();

    parameters << dbName
               << hostName
               << portNumber
               << userName
               << password;

    return true;
}
