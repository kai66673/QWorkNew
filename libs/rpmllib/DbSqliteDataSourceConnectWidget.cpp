#include <QFile>
#include <QFileDialog>

#include "DbSqliteDataSourceConnectWidget.h"
#include "ui_DbSqliteDataSourceConnectWidget.h"
#include "Core.h"

DbSqliteDataSourceConnectWidget::DbSqliteDataSourceConnectWidget( QWidget *parent )
    : RPML::IDataSourceConnectWidget(parent)
    , ui(new Ui::DbSqliteDataSourceConnectWidget)
{
    ui->setupUi(this);
}

DbSqliteDataSourceConnectWidget::~DbSqliteDataSourceConnectWidget()
{
    delete ui;
}

bool DbSqliteDataSourceConnectWidget::checkFillConnectParameters( QList<RPML::DataSource::ConnectParameter> &connectParameters )
{
    QString filePath = ui->pathLineEdit->text().trimmed();
    if ( !filePath.isEmpty() ) {
        QFile f(filePath);
        if ( f.exists() ) {
            connectParameters.push_back(RPML::DataSource::ConnectParameter("database", filePath));
            return true;
        }
    }

    ui->pathLineEdit->setFocus();
    return false;
}

void DbSqliteDataSourceConnectWidget::on_browsePathButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(Core::Storage::mainWindow()->widget());
    if ( !filePath.isEmpty() )
        ui->pathLineEdit->setText(filePath);
}
