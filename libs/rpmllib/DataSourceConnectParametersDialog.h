#ifndef DATASOURCECONNECTPARAMETERSDIALOG_H
#define DATASOURCECONNECTPARAMETERSDIALOG_H

#include <QDialog>

#include "DataSource.h"

namespace Ui {
class DataSourceConnectParametersDialog;
}

class DataSourceConnectParametersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataSourceConnectParametersDialog( QList<RPML::IDataSourceProvider *> *providers, QWidget *parent = 0 );
    ~DataSourceConnectParametersDialog();

    RPML::IDataSourceProvider *selectedProvider() const;
    QList<RPML::DataSource::ConnectParameter> dataSourceConnectParameters() const;

private slots:
    void on_okButton_clicked();

private:
    Ui::DataSourceConnectParametersDialog *ui;
    QList<RPML::IDataSourceProvider *> *m_providers;

    QList<RPML::IDataSourceConnectWidget *> m_connectWidgets;
    QList<RPML::DataSource::ConnectParameter> m_connectParameters;
};

#endif // DATASOURCECONNECTPARAMETERSDIALOG_H
