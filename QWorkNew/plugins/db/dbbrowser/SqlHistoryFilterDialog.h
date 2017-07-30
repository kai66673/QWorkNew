#ifndef SQLHISTORYFILTERDIALOG_H
#define SQLHISTORYFILTERDIALOG_H

#include <QDialog>

#include "QSqlHistoryTableView.h"

namespace Ui {
class SqlHistoryFilterDialog;
}

class SqlHistoryFilterDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SqlHistoryFilterDialog( QSqlHistorySortFilterProxyModel *model, QWidget *parent = 0 );
    ~SqlHistoryFilterDialog();

private slots:
    void onFilterTypeChanged( int index );
    void tryAccept();
    
private:
    void initFilterData();

    Ui::SqlHistoryFilterDialog *ui;
    QSqlHistorySortFilterProxyModel *m_model;
};

#endif // SQLHISTORYFILTERDIALOG_H
