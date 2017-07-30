#ifndef NEWURLDIALOG_H
#define NEWURLDIALOG_H

#include <QDialog>

#include "IFavorite.h"

namespace Ui {
class NewUrlDialog;
}

class NewUrlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewUrlDialog(QWidget *parent = 0);
    ~NewUrlDialog();

    FavoriteData *favoriteData( const QString &id ) const;

public slots:
    virtual void accept();

private:
    Ui::NewUrlDialog *ui;
};

#endif // NEWURLDIALOG_H
