#ifndef DBSQLITEDATASOURCECONNECTWIDGET_H
#define DBSQLITEDATASOURCECONNECTWIDGET_H

#include "DataSource.h"

namespace Ui {
class DbSqliteDataSourceConnectWidget;
}

class DbSqliteDataSourceConnectWidget : public RPML::IDataSourceConnectWidget
{
    Q_OBJECT
    
public:
    explicit DbSqliteDataSourceConnectWidget( QWidget *parent = 0 );
    ~DbSqliteDataSourceConnectWidget();
    
    virtual bool checkFillConnectParameters( QList<RPML::DataSource::ConnectParameter> &connectParameters );

private slots:
    void on_browsePathButton_clicked();

private:
    Ui::DbSqliteDataSourceConnectWidget *ui;
};

#endif // DBSQLITEDATASOURCECONNECTWIDGET_H
