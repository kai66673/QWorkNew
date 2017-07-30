#include "CassandraAuthWidget.h"
#include "ui_CassandraAuthWidget.h"

#include "Core.h"

CassandraAuthWidget::CassandraAuthWidget(QWidget *parent)
    : Database::IDbConnectWidget(parent)
    , ui(new Ui::Form)
{
    ui->setupUi(this);
}

CassandraAuthWidget::~CassandraAuthWidget()
{
    delete ui;
}

bool CassandraAuthWidget::checkFillAuthParameters( QStringList &parameters )
{
    QString hosts = ui->hostsLineEdit->text().trimmed();
    if ( hosts.isEmpty() ) {
        Core::Storage::warning(tr("Hosts list cannot be empty"));
        ui->hostsLineEdit->setFocus();
        return false;
    }

    QString portNumber = ui->portLineEdit->text().trimmed();
    if (  portNumber.isEmpty() ) {
        Core::Storage::warning(tr("Port Number cannot be empty"));
        ui->portLineEdit->setFocus();
        return false;
    }

    parameters << hosts << portNumber;
    return true;
}
