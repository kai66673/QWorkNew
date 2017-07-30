#ifndef ADDPYTHONSCRIPTDIALOG_H
#define ADDPYTHONSCRIPTDIALOG_H

#include <QDialog>
#include <QMap>

#include "PythonScriptManager.h"

namespace Ui {
class AddPythonScriptDialog;
}

class AddPythonScriptDialog: public QDialog
{
    Q_OBJECT
    
public:
    explicit AddPythonScriptDialog( QMap<QString, Python::PythonScript *> *scripts, const QString &scriptsDir, QWidget *parent = 0 );
    ~AddPythonScriptDialog();

    QString scriptName() { return m_scriptName; }
    QString scriptDescription() { return m_scriptDescription; }

public slots:
    virtual void accept();
    
private:
    void fillNamesCombo();

    Ui::AddPythonScriptDialog *ui;

    QMap<QString, Python::PythonScript *> *m_scripts;
    QString m_scriptsDir;
    QString m_scriptName;
    QString m_scriptDescription;
};

#endif // ADDPYTHONSCRIPTDIALOG_H
