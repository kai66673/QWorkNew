#include "ConfigureProjectsDialog.h"
#include "ui_ConfigureProjectsDialog.h"

#include "Core.h"

ConfigureProjectsDialog::ConfigureProjectsDialog(QWidget *parent) :
    QwDialog("ConfigureProjectsDialog",
             Core::Storage::mainWindow()->orgName(),
             Core::Storage::mainWindow()->appName(),
             parent),
    ui(new Ui::ConfigureProjectsDialog)
{
    ui->setupUi(this);

    m_projectWindow = new ProjectExplorer::Internal::ProjectWindow(this);
    ui->stackedWidget->addWidget(m_projectWindow);

    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(accept()));
}

ConfigureProjectsDialog::~ConfigureProjectsDialog()
{
    delete ui;
}

ProjectExplorer::Internal::ProjectWindow *ConfigureProjectsDialog::projectWindow()
{
    return m_projectWindow;
}
