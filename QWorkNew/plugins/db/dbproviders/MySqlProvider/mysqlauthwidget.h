#ifndef MYSQLAUTHWIDGET_H
#define MYSQLAUTHWIDGET_H

#include "DbLoader.h"

namespace Ui {
class MySqlAuthWidget;
}

class MySqlAuthWidget: public Database::IDbConnectWidget
{
    Q_OBJECT

public:
    explicit MySqlAuthWidget(QWidget *parent = nullptr);
    ~MySqlAuthWidget();

    virtual bool checkFillAuthParameters(QStringList &parameters);

private:
    Ui::MySqlAuthWidget *ui;
};

#endif // MYSQLAUTHWIDGET_H
