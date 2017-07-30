#ifndef LINTERAUTHWIDGET_H
#define LINTERAUTHWIDGET_H

#include "DbLoader.h"

namespace Ui {
class LinterAuthWidget;
}

class LinterAuthWidget : public Database::IDbConnectWidget
{
    Q_OBJECT

public:
    explicit LinterAuthWidget( QWidget *parent = 0 );
    ~LinterAuthWidget();

    virtual bool checkFillAuthParameters( QStringList &parameters );

private:
    Ui::LinterAuthWidget *ui;
};

#endif // LINTERAUTHWIDGET_H
