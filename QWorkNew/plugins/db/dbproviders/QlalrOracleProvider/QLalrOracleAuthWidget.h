#ifndef QLALR_ORACLEAUTHWIDGET_H
#define QLALR_ORACLEAUTHWIDGET_H

#include "DbLoader.h"

namespace Ui {
class QLalrOracleAuthWidget;
}

class QLalrOracleAuthWidget : public Database::IDbConnectWidget
{
    Q_OBJECT
    
public:
    explicit QLalrOracleAuthWidget( QWidget *parent = 0 );
    ~QLalrOracleAuthWidget();

    virtual bool checkFillAuthParameters( QStringList &parameters );
    
private:
    Ui::QLalrOracleAuthWidget *ui;
};

#endif // QLALR_ORACLEAUTHWIDGET_H
