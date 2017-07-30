#include "UiTypeDialog.h"
#include "ui_UiTypeDialog.h"

UiTypeDialog::UiTypeDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UiTypeDialog)
{
    ui->setupUi(this);
}

UiTypeDialog::~UiTypeDialog()
{
    delete ui;
}

int UiTypeDialog::selctedUiType() const
{
    return ui->uiTypesComboBox->currentIndex();
}
