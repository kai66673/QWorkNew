#ifndef POSTGRESAUTHWIDGET_H
#define POSTGRESAUTHWIDGET_H

#include "DbLoader.h"

namespace Ui {
class PostgresAuthWidget;
}

class PostgresAuthWidget : public Database::IDbConnectWidget
{
    Q_OBJECT

public:
    explicit PostgresAuthWidget( QWidget *parent = 0 );
    ~PostgresAuthWidget();

    virtual bool checkFillAuthParameters( QStringList &parameters );

private:
    Ui::PostgresAuthWidget *ui;
};

#endif // POSTGRESAUTHWIDGET_H
