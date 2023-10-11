#include "DesignerDockTitle.h"
#include "ui_DesignerDockTitle.h"

DesignerDockTitle::DesignerDockTitle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DesignerDockTitle)
{
    ui->setupUi(this);
    ui->label->setMinimumHeight(5);
    ui->label->setMaximumHeight(5);
    setMinimumHeight(5);
    setMaximumHeight(5);
}

DesignerDockTitle::~DesignerDockTitle()
{
    delete ui;
}
