#ifndef DBPOSTGRESDATASOURCECONNECTWIDGET_H
#define DBPOSTGRESDATASOURCECONNECTWIDGET_H

#include "DataSource.h"

namespace Ui {
class DbPostgresDataSourceConnectWidget;
}

class DbPostgresDataSourceConnectWidget : public RPML::IDataSourceConnectWidget
{
    Q_OBJECT
    
public:
    explicit DbPostgresDataSourceConnectWidget( QWidget *parent = 0 );
    ~DbPostgresDataSourceConnectWidget();

    virtual bool checkFillConnectParameters( QList<RPML::DataSource::ConnectParameter> &connectParameters );

private:
    Ui::DbPostgresDataSourceConnectWidget *ui;
};

#endif // DBPOSTGRESDATASOURCECONNECTWIDGET_H
