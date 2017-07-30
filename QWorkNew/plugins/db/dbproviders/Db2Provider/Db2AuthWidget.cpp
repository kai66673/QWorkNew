#include "Core.h"
#include "Db2AuthWidget.h"
#include "ui_Db2AuthWidget.h"

Db2AuthWidget::Db2AuthWidget( QWidget *parent )
    : Database::IDbConnectWidget(parent)
    , ui(new Ui::Db2AuthWidget)
{
    ui->setupUi(this);
}

Db2AuthWidget::~Db2AuthWidget()
{
    delete ui;
}

bool Db2AuthWidget::checkFillAuthParameters( QStringList &parameters )
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
