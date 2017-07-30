#include "DspConfigDialog.h"
#include "ui_DspConfigDialog.h"

#include "Core.h"

DspConfigDialog::DspConfigDialog( const QSet<QString> &cfgSet, QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::DspConfigDialog)
{
    ui->setupUi(this);
    foreach ( const QString &cfg, cfgSet )
        ui->listWidget->addItem(cfg);
}

DspConfigDialog::~DspConfigDialog()
{
    delete ui;
}

void DspConfigDialog::accept()
{
    if ( QListWidgetItem *item = ui->listWidget->currentItem() ) {
        m_cfgName = item->text();
        QDialog::accept();
    }
    else {
        Core::Storage::warning(tr("No Configuration Item Selected"));
    }
}
