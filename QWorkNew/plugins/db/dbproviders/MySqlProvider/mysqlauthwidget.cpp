#include "Core.h"

#include "mysqlauthwidget.h"
#include "ui_mysqlauthwidget.h"

MySqlAuthWidget::MySqlAuthWidget(QWidget *parent)
    : Database::IDbConnectWidget(parent)
    , ui(new Ui::MySqlAuthWidget)
{
    ui->setupUi(this);
}

MySqlAuthWidget::~MySqlAuthWidget()
{
    delete ui;
}

bool MySqlAuthWidget::checkFillAuthParameters(QStringList &parameters)
{
    QString dbName = ui->databaseLineEdit->text().trimmed();
    if (dbName.isEmpty()) {
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
