#include <QMainWindow>

#include "DockCornerSettingsDialog.h"
#include "ui_DockCornerSettingsDialog.h"
#include "Core.h"

DockCornerSettingsDialog::DockCornerSettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DockCornerSettingsDialog)
{
    ui->setupUi(this);

    QMainWindow *mainWindow = Core::Storage::mainWindow()->widget();

    mainWindow->corner(Qt::TopLeftCorner) == Qt::TopDockWidgetArea
            ? ui->topLeftTopButton->setChecked(true)
            : ui->topLeftLeftButton->setChecked(true);

    mainWindow->corner(Qt::TopRightCorner) == Qt::TopDockWidgetArea
            ? ui->topRightTopButton->setChecked(true)
            : ui->topRightRightButton->setChecked(true);

    mainWindow->corner(Qt::BottomLeftCorner) == Qt::BottomDockWidgetArea
            ? ui->bottomLeftBottomButton->setChecked(true)
            : ui->bottomLeftLeftButton->setChecked(true);

    mainWindow->corner(Qt::BottomRightCorner) == Qt::BottomDockWidgetArea
            ? ui->bottomRightBottomButton->setChecked(true)
            : ui->bottomRightRightButton->setChecked(true);
}

DockCornerSettingsDialog::~DockCornerSettingsDialog()
{
    delete ui;
}

void DockCornerSettingsDialog::accept()
{
    QMainWindow *mainWindow = Core::Storage::mainWindow()->widget();

    mainWindow->setCorner( Qt::TopLeftCorner, ui->topLeftTopButton->isChecked() ? Qt::TopDockWidgetArea : Qt::LeftDockWidgetArea );
    mainWindow->setCorner( Qt::TopRightCorner, ui->topRightTopButton->isChecked() ? Qt::TopDockWidgetArea : Qt::RightDockWidgetArea );
    mainWindow->setCorner( Qt::BottomLeftCorner, ui->bottomLeftBottomButton->isChecked() ? Qt::BottomDockWidgetArea : Qt::LeftDockWidgetArea );
    mainWindow->setCorner( Qt::BottomRightCorner, ui->bottomRightBottomButton->isChecked() ? Qt::BottomDockWidgetArea : Qt::RightDockWidgetArea );

    QDialog::accept();
}
