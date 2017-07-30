#include <QFileDialog>

#include "GenericProjectLocationDialog.h"
#include "ui_GenericProjectLocationDialog.h"

GenericProjectLocationDialog::GenericProjectLocationDialog( QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::GenericProjectLocationDialog)
{
    ui->setupUi(this);
    connect(ui->toolButton, SIGNAL(clicked()), this, SLOT(onSpecifyLocation()));
}

GenericProjectLocationDialog::~GenericProjectLocationDialog()
{
    delete ui;
}

void GenericProjectLocationDialog::accept()
{
    m_sourceLocation = ui->locationLineEdit->text().trimmed();
    if ( !m_sourceLocation.isEmpty() )
        QDialog::accept();
}


void GenericProjectLocationDialog::onSpecifyLocation()
{
    QString fileName = QFileDialog::getSaveFileName( this, tr("Save File"), "",
                                                     tr("ASOF Poject Database Files (*.aspdb)") );

    if ( !fileName.isEmpty() )
        ui->locationLineEdit->setText(fileName);
}
