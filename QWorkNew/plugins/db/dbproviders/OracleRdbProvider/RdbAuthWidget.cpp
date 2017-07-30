#include "Core.h"
#include "RdbAuthWidget.h"
#include "ui_RdbAuthWidget.h"

RdbAuthWidget::RdbAuthWidget( QWidget *parent )
    : Database::IDbConnectWidget(parent)
    , ui(new Ui::RdbAuthWidget)
{
    ui->setupUi(this);

    connect(ui->databaseLineEdit, SIGNAL(editingFinished()), this, SLOT(onFocusLoginField()));
}

RdbAuthWidget::~RdbAuthWidget()
{
    delete ui;
}

bool RdbAuthWidget::checkFillAuthParameters( QStringList &parameters )
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

void RdbAuthWidget::onFocusLoginField()
{
    if ( ui->userLineEdit->text() == QLatin1String("") ) {
        ui->userLineEdit->setText(ui->databaseLineEdit->text());
        ui->userLineEdit->selectAll();
    }
}
