#include "Core.h"

#include "LinterAuthWidget.h"
#include "ui_LinterAuthWidget.h"

LinterAuthWidget::LinterAuthWidget( QWidget *parent )
    : Database::IDbConnectWidget(parent)
    , ui(new Ui::LinterAuthWidget)
{
    ui->setupUi(this);
}

LinterAuthWidget::~LinterAuthWidget()
{
    delete ui;
}

bool LinterAuthWidget::checkFillAuthParameters( QStringList &parameters )
{
    QString dbName = ui->databaseLineEdit->text().trimmed();
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
               << portNumber
               << userName
               << password;

    return true;
}
