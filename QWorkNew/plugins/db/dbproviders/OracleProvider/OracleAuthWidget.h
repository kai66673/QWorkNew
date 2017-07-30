#ifndef ORACLEAUTHWIDGET_H
#define ORACLEAUTHWIDGET_H

#include "DbLoader.h"

namespace Ui {
class OracleAuthWidget;
}

class OracleAuthWidget : public Database::IDbConnectWidget
{
    Q_OBJECT
    
public:
    explicit OracleAuthWidget( QWidget *parent = 0 );
    ~OracleAuthWidget();

    virtual bool checkFillAuthParameters( QStringList &parameters );
    
private:
    Ui::OracleAuthWidget *ui;
};

#endif // ORACLEAUTHWIDGET_H
