#ifndef ADDEDITPROJECTBUILDACTIONDIALOG_H
#define ADDEDITPROJECTBUILDACTIONDIALOG_H

#include <QDialog>

#include "ProjectBuildActionsModel.h"

namespace Ui {
class AddEditProjectBuildActionDialog;
}

class AddEditProjectBuildActionDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddEditProjectBuildActionDialog( ProjectBuildAction *buildAction, QWidget *parent = 0 );
    ~AddEditProjectBuildActionDialog();

public slots:
    void accept();

private slots:
    void onWorkingDirectory();
    
private:
    Ui::AddEditProjectBuildActionDialog *ui;
    ProjectBuildAction *m_buildAction;
};

#endif // ADDEDITPROJECTBUILDACTIONDIALOG_H
