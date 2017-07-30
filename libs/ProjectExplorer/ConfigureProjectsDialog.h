#ifndef CONFIGUREPROJECTSDIALOG_H
#define CONFIGUREPROJECTSDIALOG_H

#include "projectwindow.h"
#include "QwDialog.h"

namespace Ui {
class ConfigureProjectsDialog;
}

class ConfigureProjectsDialog : public QwDialog
{
    Q_OBJECT

public:
    explicit ConfigureProjectsDialog(QWidget *parent = 0);
    ~ConfigureProjectsDialog();

    ProjectExplorer::Internal::ProjectWindow *projectWindow();

private:
    Ui::ConfigureProjectsDialog *ui;
    ProjectExplorer::Internal::ProjectWindow *m_projectWindow;
};

#endif // CONFIGUREPROJECTSDIALOG_H
