#ifndef REPORTSELECTDATASOURCECOLUMNDIALOG_H
#define REPORTSELECTDATASOURCECOLUMNDIALOG_H

#include <QDialog>

#include "DataSource.h"

namespace Ui {
class ReportSelectDataSourceColumnDialog;
}

namespace RPML {

class ReportSelectDataSourceColumnDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReportSelectDataSourceColumnDialog( QList<RPML::DataSource *> *sources,
                                                 int sourceIdForced, QWidget *parent = 0 );
    ~ReportSelectDataSourceColumnDialog();

    inline int dataSourceId() const { return m_id; }
    inline int dataSourceColumnNumber() const { return m_columnNumber; }

private slots:
    void onDataSourceIndexChanaged( int index );

    void on_okPushButton_clicked();

private:
    Ui::ReportSelectDataSourceColumnDialog *ui;
    QList<RPML::DataSource *> *m_sources;
    int m_sourceIdForced;
    int m_id;
    int m_columnNumber;
};

}   // namespace RPML

#endif // REPORTSELECTDATASOURCECOLUMNDIALOG_H
