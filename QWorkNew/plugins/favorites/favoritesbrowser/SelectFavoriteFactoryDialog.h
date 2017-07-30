#ifndef SELECTFAVORITEFACTORYDIALOG_H
#define SELECTFAVORITEFACTORYDIALOG_H

#include <QDialog>

#include "IFavorite.h"

namespace Ui {
class SelectFavoriteFactoryDialog;
}

class SelectFavoriteFactoryDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SelectFavoriteFactoryDialog( QList<IFavoriteFactory *> *factories, QWidget *parent = 0 );
    ~SelectFavoriteFactoryDialog();

    IFavoriteFactory *selectedFactory() const { return m_selectedFactory; }

public slots:
    virtual void accept();
    
private:
    Ui::SelectFavoriteFactoryDialog *ui;
    QList<IFavoriteFactory *> *m_factories;
    IFavoriteFactory *m_selectedFactory;
};

#endif // SELECTFAVORITEFACTORYDIALOG_H
