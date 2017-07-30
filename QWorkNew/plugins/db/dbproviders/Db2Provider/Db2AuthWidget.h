#ifndef DB2AUTHWIDGET_H
#define DB2AUTHWIDGET_H

#include "DbLoader.h"

namespace Ui {
class Db2AuthWidget;
}

class Db2AuthWidget : public Database::IDbConnectWidget
{
    Q_OBJECT
    
public:
    explicit Db2AuthWidget( QWidget *parent = 0 );
    ~Db2AuthWidget();

    virtual bool checkFillAuthParameters( QStringList &parameters );
    
private:
    Ui::Db2AuthWidget *ui;
};

#endif // DB2AUTHWIDGET_H
