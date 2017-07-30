#ifndef RDBAUTHWIDGET_H
#define RDBAUTHWIDGET_H

#include "DbLoader.h"

namespace Ui {
class RdbAuthWidget;
}

class RdbAuthWidget : public Database::IDbConnectWidget
{
    Q_OBJECT
    
public:
    explicit RdbAuthWidget( QWidget *parent = 0 );
    ~RdbAuthWidget();

    virtual bool checkFillAuthParameters( QStringList &parameters );
    
public slots:
    void onFocusLoginField();

private:
    Ui::RdbAuthWidget *ui;
};

#endif // RDBAUTHWIDGET_H
