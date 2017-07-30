#include "GotoLineDialog.h"
#include "ui_GotoLineDialog.h"

GotoLineDialog::GotoLineDialog( int lineCount, int currentLine, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GotoLineDialog)
{
    ui->setupUi(this);

    ui->lineNumberHorizontalSlider->setMinimum(1);
    ui->lineNumberHorizontalSlider->setMaximum(lineCount);
    ui->lineNumberSpinBox->setMinimum(1);
    ui->lineNumberSpinBox->setMaximum(lineCount);

    ui->lineNumberHorizontalSlider->setValue(currentLine + 1);
    ui->lineNumberSpinBox->setValue(currentLine + 1);

    connect(ui->lineNumberHorizontalSlider, SIGNAL(valueChanged(int)), ui->lineNumberSpinBox, SLOT(setValue(int)));
    connect(ui->lineNumberSpinBox, SIGNAL(valueChanged(int)), ui->lineNumberHorizontalSlider, SLOT(setValue(int)));
}

GotoLineDialog::~GotoLineDialog()
{
    delete ui;
}

int GotoLineDialog::lineNumber() const
{
    return ui->lineNumberSpinBox->value();
}
