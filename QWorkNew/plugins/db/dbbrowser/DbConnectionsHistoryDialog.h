#ifndef DBCONNECTIONSHISTORYDIALOG_H
#define DBCONNECTIONSHISTORYDIALOG_H

#include <QDialog>

#include "DbConnectionsHistory.h"

namespace Ui {
class DbConnectionsHistoryDialog;
}

class DbConnectionsHistoryDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit DbConnectionsHistoryDialog( QList<DbConnectionAuthInfo> *connections_, QWidget *parent = 0 );
    ~DbConnectionsHistoryDialog();

    int selectedIndex() const { return m_selectedIndex; }
    
private slots:
    void updateStates();
    void selectConnection();
    void removeConnection();
    void clearConnections();

private:
    void setupConnections();

    Ui::DbConnectionsHistoryDialog *ui;
    QList<DbConnectionAuthInfo> *connections;

    int m_selectedIndex;
};

#endif // DBCONNECTIONSHISTORYDIALOG_H
