#include "Core.h"

#include "SelectFavoriteFactoryDialog.h"
#include "ui_SelectFavoriteFactoryDialog.h"

SelectFavoriteFactoryDialog::SelectFavoriteFactoryDialog( QList<IFavoriteFactory *> *factories, QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::SelectFavoriteFactoryDialog)
    , m_factories(factories)
    , m_selectedFactory(0)
{
    ui->setupUi(this);

    for (int i = 0; i < m_factories->size(); i++)
        ui->factoriesListWidget->insertItem(i, m_factories->at(i)->description());
}

SelectFavoriteFactoryDialog::~SelectFavoriteFactoryDialog()
{
    delete ui;
}

void SelectFavoriteFactoryDialog::accept()
{
    int rowIndex = ui->factoriesListWidget->currentRow();
    if ( rowIndex == -1 ) {
        Core::Storage::warning(tr("No Favorite Factory Selected"));
        ui->factoriesListWidget->setFocus();
        return;
    }

    m_selectedFactory = m_factories->at(rowIndex);
    QDialog::accept();
}
