#include "Core.h"
#include "SelectProjectImporterDialog.h"
#include "ui_SelectProjectImporterDialog.h"

SelectProjectImporterDialog::SelectProjectImporterDialog( AsofProjectImportManager *importManager, QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::SelectProjectImporterDialog)
    , m_importManager(importManager)
    , m_importer(0)
{
    ui->setupUi(this);

    for (int i = 0; i < m_importManager->importersCount(); i++)
        ui->importersListWidget->addItem((*m_importManager)[i]->description());

    ui->importersListWidget->setCurrentRow(0);
}

SelectProjectImporterDialog::~SelectProjectImporterDialog()
{
    delete ui;
}

void SelectProjectImporterDialog::accept()
{
    int index = ui->importersListWidget->currentRow();

    if ( index == -1 ) {
        Core::Storage::warning(tr("No Project Type Selected."));
        ui->importersListWidget->setFocus();
        return;
    }
    m_importer = (*m_importManager)[index];
    QDialog::accept();
}
