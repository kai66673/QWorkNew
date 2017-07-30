#ifndef SQLITEAUTHWIDGET_H
#define SQLITEAUTHWIDGET_H

#include "DbLoader.h"

namespace Ui {
class SqliteAuthWidget;
}

class SqliteAuthWidget : public Database::IDbConnectWidget
{
    Q_OBJECT
    
public:
    explicit SqliteAuthWidget(QWidget *parent = 0);
    ~SqliteAuthWidget();

    virtual bool checkFillAuthParameters( QStringList &parameters );

private slots:
    void onChooseFileName();
    
private:
    Ui::SqliteAuthWidget *ui;
};

#endif // SQLITEAUTHWIDGET_H
