#include "Core.h"
#include "ConfigureProjectBuildActionsDialog.h"
#include "AddEditProjectBuildActionDialog.h"
#include "ui_ConfigureProjectBuildActionsDialog.h"

#include "ProjectBuildEnvironmentDialog.h"

ConfigureProjectBuildActionsDialog::ConfigureProjectBuildActionsDialog( ProjectBuildActionsModel *model, QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::ConfigureProjectBuildActionsDialog)
    , m_model(model)
{
    ui->setupUi(this);
    ui->buidActionsTableView->setModel(m_model);
    ui->buidActionsTableView->resizeColumnsToContents();
    ui->buidActionsTableView->resizeRowsToContents();
    ui->buidActionsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->buidActionsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(ui->addBuildActionButton,       SIGNAL(clicked()), this, SLOT(onAdd()));
    connect(ui->editBuildActionButton,      SIGNAL(clicked()), this, SLOT(onEdit()));
    connect(ui->deleteBuildActionButton,    SIGNAL(clicked()), this, SLOT(onDelete()));
    connect(ui->envPushButton,              SIGNAL(clicked()), this, SLOT(editEnvironment()));
}

ConfigureProjectBuildActionsDialog::~ConfigureProjectBuildActionsDialog()
{
    delete ui;
}

void ConfigureProjectBuildActionsDialog::reject()
{
    m_model->readFromDb();
    QDialog::reject();
}

void ConfigureProjectBuildActionsDialog::accept()
{
    m_model->writeToDb();
    QDialog::accept();
}

void ConfigureProjectBuildActionsDialog::editEnvironment()
{
    ProjectBuildEnvironmentDialog envDialog( m_model->buildManager()->environment(),
                                             Core::Storage::mainWindow()->widget() );
    envDialog.exec();
}

void ConfigureProjectBuildActionsDialog::onAdd()
{
    ProjectBuildAction buildAction;
    AddEditProjectBuildActionDialog addDialog(&buildAction, Core::Storage::mainWindow()->widget());
    if ( addDialog.exec() == QDialog::Accepted ) {
        int addIndex = m_model->rowCount(QModelIndex());
        m_model->insertRows(addIndex, 1);

        m_model->m_projectBuildActions[addIndex].m_commandName      = buildAction.m_commandName;
        m_model->m_projectBuildActions[addIndex].m_command          = buildAction.m_command;
        m_model->m_projectBuildActions[addIndex].m_commandArgs      = buildAction.m_commandArgs;
        m_model->m_projectBuildActions[addIndex].m_commandWorkDir   = buildAction.m_commandWorkDir;
        m_model->m_projectBuildActions[addIndex].m_priorAction      = buildAction.m_priorAction;

        m_model->emitRowChanged(addIndex);
    }
}

void ConfigureProjectBuildActionsDialog::onEdit()
{
    int index = ui->buidActionsTableView->currentIndex().row();
    if ( index == -1 )
        return;

    ProjectBuildAction buildAction( m_model->m_projectBuildActions[index].m_commandName,
                                    m_model->m_projectBuildActions[index].m_command,
                                    m_model->m_projectBuildActions[index].m_commandArgs,
                                    m_model->m_projectBuildActions[index].m_commandWorkDir,
                                    m_model->m_projectBuildActions[index].m_priorAction );

    AddEditProjectBuildActionDialog editDialog(&buildAction, Core::Storage::mainWindow()->widget());
    if ( editDialog.exec() == QDialog::Accepted ) {
        m_model->m_projectBuildActions[index].m_commandName     = buildAction.m_commandName;
        m_model->m_projectBuildActions[index].m_command         = buildAction.m_command;
        m_model->m_projectBuildActions[index].m_commandArgs     = buildAction.m_commandArgs;
        m_model->m_projectBuildActions[index].m_commandWorkDir  = buildAction.m_commandWorkDir;
        m_model->m_projectBuildActions[index].m_priorAction     = buildAction.m_priorAction;

        m_model->emitRowChanged(index);
    }
}

void ConfigureProjectBuildActionsDialog::onDelete()
{
    int index = ui->buidActionsTableView->currentIndex().row();
    if ( index == -1 )
        return;

    m_model->removeRows(index, 1);
}
