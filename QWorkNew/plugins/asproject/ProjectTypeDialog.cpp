#include "ProjectTypeDialog.h"
#include "ui_ProjectTypeDialog.h"

ProjectTypeDialog::ProjectTypeDialog( QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::ProjectTypeDialog)
{
    ui->setupUi(this);
}

ProjectTypeDialog::~ProjectTypeDialog()
{
    delete ui;
}

void ProjectTypeDialog::accept()
{
    switch ( ui->ppListWidget->currentIndex().row() ) {
        case 0:  m_projectType = Generic;   break;
        case 1:  m_projectType = DspImport; break;
        default: m_projectType = Unknown;   break;
    }

    QDialog::accept();
}
