#ifndef SELECTPROJECTSDIALOG_H
#define SELECTPROJECTSDIALOG_H

#include <QDialog>
#include <QStandardItemModel>

#include "project.h"

namespace Ui {
class SelectProjectsDialog;
}

class SelectProjectsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectProjectsDialog(const QList<ProjectExplorer::Project *> &projects, ProjectExplorer::Project *activeProject, QWidget *parent = 0);
    ~SelectProjectsDialog();

    QList<ProjectExplorer::Project *> selectedProjects();

private slots:
    void on_selectAllpushButton_clicked();
    void on_selectAllpushButton_2_clicked();

private:
    void setCheckedForAllItems(bool onoff);

    Ui::SelectProjectsDialog *ui;
    QStandardItemModel *m_model;
    const QList<ProjectExplorer::Project *> &m_projects;
};

#endif // SELECTPROJECTSDIALOG_H
