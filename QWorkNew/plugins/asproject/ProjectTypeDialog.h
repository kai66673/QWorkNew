#ifndef PROJECTTYPEDIALOG_H
#define PROJECTTYPEDIALOG_H

#include <QDialog>

namespace Ui {
class ProjectTypeDialog;
}

class ProjectTypeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ProjectTypeDialog( QWidget *parent = 0 );
    ~ProjectTypeDialog();

    enum ProjectType {
        Unknown = -1,
        Generic = 0,
        DspImport = 1
    };

    ProjectType projectType() const { return m_projectType; }

public slots:
    virtual void accept();
    
private:
    Ui::ProjectTypeDialog *ui;

    ProjectType m_projectType;
};

#endif // PROJECTTYPEDIALOG_H
