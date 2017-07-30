#include "SelectProjectsDialog.h"
#include "ui_SelectProjectsDialog.h"

SelectProjectsDialog::SelectProjectsDialog(const QList<ProjectExplorer::Project *> &projects, ProjectExplorer::Project *activeProject, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SelectProjectsDialog)
    , m_projects(projects)
{
    ui->setupUi(this);

    m_model = new QStandardItemModel();

    int row = 0;
    foreach ( ProjectExplorer::Project *project, projects ) {
        QStandardItem *item = new QStandardItem();
        item->setCheckable(true);
        item->setText(project->displayName());
        item->setToolTip(project->projectFilePath().toString());
        if ( project == activeProject )
            item->setCheckState(Qt::Checked);
        m_model->setItem(row++, item);
    }

    ui->listView->setModel(m_model);
}

SelectProjectsDialog::~SelectProjectsDialog()
{
    delete ui;
}

QList<ProjectExplorer::Project *> SelectProjectsDialog::selectedProjects()
{
    QList<ProjectExplorer::Project *> result;

    for (int i = 0; i < m_model->rowCount(); i++)
        if ( m_model->item(i)->checkState() == Qt::Checked )
            result << m_projects[i];

    return result;
}

void SelectProjectsDialog::on_selectAllpushButton_clicked()
{
    setCheckedForAllItems(true);
}

void SelectProjectsDialog::on_selectAllpushButton_2_clicked()
{
    setCheckedForAllItems(false);
}

void SelectProjectsDialog::setCheckedForAllItems(bool onoff)
{
    Qt::CheckState state = (onoff ? Qt::Checked : Qt::Unchecked);
    for (int i = 0; i < m_model->rowCount(); i++)
        m_model->item(i)->setCheckState(state);
}
