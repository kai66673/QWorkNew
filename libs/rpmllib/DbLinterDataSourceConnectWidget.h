#ifndef DBLINTERDATASOURCECONNECTWIDGET_H
#define DBLINTERDATASOURCECONNECTWIDGET_H

#include "DataSource.h"

namespace Ui {
class DbLinterDataSourceConnectWidget;
}

class DbLinterDataSourceConnectWidget : public RPML::IDataSourceConnectWidget
{
    Q_OBJECT
    
public:
    explicit DbLinterDataSourceConnectWidget( QWidget *parent = 0 );
    ~DbLinterDataSourceConnectWidget();

    virtual bool checkFillConnectParameters( QList<RPML::DataSource::ConnectParameter> &connectParameters );

private:
    Ui::DbLinterDataSourceConnectWidget *ui;
};

#endif // DBLINTERDATASOURCECONNECTWIDGET_H
