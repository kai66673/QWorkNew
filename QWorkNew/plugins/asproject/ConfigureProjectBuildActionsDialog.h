#ifndef CONFIGUREPROJECTBUILDACTIONSDIALOG_H
#define CONFIGUREPROJECTBUILDACTIONSDIALOG_H

#include <QDialog>

#include "ProjectBuildActionsModel.h"

namespace Ui {
class ConfigureProjectBuildActionsDialog;
}

class ConfigureProjectBuildActionsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ConfigureProjectBuildActionsDialog( ProjectBuildActionsModel *model, QWidget *parent = 0 );
    ~ConfigureProjectBuildActionsDialog();

public slots:
    void reject();
    void accept();
    void editEnvironment();

private slots:
    void onAdd();
    void onEdit();
    void onDelete();
    
private:
    Ui::ConfigureProjectBuildActionsDialog *ui;
    ProjectBuildActionsModel *m_model;
};

#endif // CONFIGUREPROJECTBUILDACTIONSDIALOG_H
