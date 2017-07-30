#include <QFileDialog>

#include "Core.h"
#include "DbLoader.h"
#include "SqliteAuthWidget.h"
#include "ui_SqliteAuthWidget.h"

SqliteAuthWidget::SqliteAuthWidget(QWidget *parent)
    : Database::IDbConnectWidget(parent)
    , ui(new Ui::SqliteAuthWidget)
{
    ui->setupUi(this);

    connect(ui->chooseFileToolButton, SIGNAL(clicked()), this, SLOT(onChooseFileName()));
}

SqliteAuthWidget::~SqliteAuthWidget()
{
    delete ui;
}

bool SqliteAuthWidget::checkFillAuthParameters(QStringList &parameters)
{
    QString fileName = ui->fileNameLineEdit->text().trimmed();
    ui->fileNameLineEdit->setText(fileName);
    if ( fileName.isEmpty() ) {
        Core::Storage::warning(tr("Database file name cannot be empty"));
        ui->fileNameLineEdit->setFocus();
        return false;
    }

    parameters << fileName;
    return true;
}

void SqliteAuthWidget::onChooseFileName()
{
    QString filePath = QFileDialog::getOpenFileName(Core::Storage::mainWindow()->widget());
    if ( !filePath.isEmpty() )
        ui->fileNameLineEdit->setText(filePath);
}
