#ifndef CASSANDRAAUTHWIDGET_H
#define CASSANDRAAUTHWIDGET_H

#include "DbLoader.h"

namespace Ui {
class Form;
}

class CassandraAuthWidget : public Database::IDbConnectWidget
{
    Q_OBJECT

public:
    explicit CassandraAuthWidget( QWidget *parent = 0 );
    ~CassandraAuthWidget();

    virtual bool checkFillAuthParameters( QStringList &parameters );

private:
    Ui::Form *ui;
};

#endif // CASSANDRAAUTHWIDGET_H
