#include <QFileDialog>

#include "Core.h"
#include "AddEditProjectBuildActionDialog.h"
#include "ui_AddEditProjectBuildActionDialog.h"

AddEditProjectBuildActionDialog::AddEditProjectBuildActionDialog( ProjectBuildAction *buildAction, QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::AddEditProjectBuildActionDialog)
    , m_buildAction(buildAction)
{
    ui->setupUi(this);
    ui->actionNameLineEdit->setText(m_buildAction->m_commandName);
    ui->exeFilePathLineEdit->setText(m_buildAction->m_command);
    ui->argumentsLineEdit->setText(m_buildAction->m_commandArgs);
    ui->workingDirLineEdit->setText(m_buildAction->m_commandWorkDir);
    switch ( m_buildAction->m_priorAction ) {
        case ProjectBuildAction::AskSaveDocuments:
            ui->priorActionComboBox->setCurrentIndex(1);
            break;
        case ProjectBuildAction::AutoSaveDocuments:
            ui->priorActionComboBox->setCurrentIndex(2);
            break;
        default:
            ui->priorActionComboBox->setCurrentIndex(0);
    }

    connect(ui->workingDirToolButton,   SIGNAL(clicked()), this, SLOT(onWorkingDirectory()));
}

AddEditProjectBuildActionDialog::~AddEditProjectBuildActionDialog()
{
    delete ui;
}

void AddEditProjectBuildActionDialog::accept()
{
    QString commandName = ui->actionNameLineEdit->text().trimmed();
    if ( commandName.isEmpty() ) {
        Core::Storage::warning(tr("Action Name Cannot be Empty"));
        ui->actionNameLineEdit->setText(commandName);
        ui->actionNameLineEdit->setFocus();
        return;
    }

    QString filePath = ui->exeFilePathLineEdit->text();
    if ( filePath.isEmpty() ) {
        Core::Storage::warning(tr("Executable File not Defined"));
        ui->exeFilePathLineEdit->setFocus();
        return;
    }

    QString workDir = ui->workingDirLineEdit->text();

    m_buildAction->m_commandName = commandName;
    m_buildAction->m_command = filePath;
    m_buildAction->m_commandArgs = ui->argumentsLineEdit->text().trimmed();
    m_buildAction->m_commandWorkDir = workDir;
    switch ( ui->priorActionComboBox->currentIndex() ) {
        case 1:
            m_buildAction->m_priorAction = ProjectBuildAction::AskSaveDocuments;
            break;
        case 2:
            m_buildAction->m_priorAction = ProjectBuildAction::AutoSaveDocuments;
            break;
        default:
            m_buildAction->m_priorAction = ProjectBuildAction::NoActions;
    }

    QDialog::accept();
}

void AddEditProjectBuildActionDialog::onWorkingDirectory()
{
    QString dirName = QFileDialog::getExistingDirectory( Core::Storage::mainWindow()->widget(),
                                                         tr("Select Working Directory"),
                                                         ui->workingDirLineEdit->text(),
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );

    if ( dirName.isEmpty() )
        return;

    ui->workingDirLineEdit->setText(dirName);
}
