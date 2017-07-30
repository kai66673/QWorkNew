#include "ConfirmRemoveScriptDialog.h"
#include "ui_ConfirmRemoveScriptDialog.h"

ConfirmRemoveScriptDialog::ConfirmRemoveScriptDialog( const QString &message, QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::ConfirmRemoveScriptDialog)
{
    ui->setupUi(this);
    ui->messageLabel->setText(message);
}

ConfirmRemoveScriptDialog::~ConfirmRemoveScriptDialog()
{
    delete ui;
}

bool ConfirmRemoveScriptDialog::removeFromDisk() const
{
    return ui->removeFromDiskCheckBox->isChecked();
}
