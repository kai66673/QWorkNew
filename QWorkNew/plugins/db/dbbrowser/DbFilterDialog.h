#ifndef DBFILTERDIALOG_H
#define DBFILTERDIALOG_H

#include <QDialog>

namespace Ui {
class DbFilterDialog;
}

namespace Database {
    struct DbFilter;
}

class DbFilterDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DbFilterDialog( Database::DbFilter *filter, QWidget *parent = 0 );
    ~DbFilterDialog();

private slots:
    void onFilterTypeChanged( int index );
    void onAccept();
    
private:
    Ui::DbFilterDialog *ui;
    Database::DbFilter *m_filter;
};

#endif // DBFILTERDIALOG_H
