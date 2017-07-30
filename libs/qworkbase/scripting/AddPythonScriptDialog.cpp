#include <QDir>
#include <QFileInfo>

#include "AddPythonScriptDialog.h"
#include "ui_AddPythonScriptDialog.h"
#include "QWorkBase.h"

static bool checkIdentifierName( const QString &name )
{
    if ( name.isEmpty() )
        return false;

    if ( !name[0].isLetter() && name[0] != QChar('_') )
        return false;

    for (int i = 1; i < name.length(); i++)
        if ( !name[i].isLetterOrNumber() && name[i] != '_' )
            return false;

    return true;
}

AddPythonScriptDialog::AddPythonScriptDialog( QMap<QString, Python::PythonScript *> *scripts, const QString &scriptsDir, QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::AddPythonScriptDialog)
    , m_scripts(scripts)
    , m_scriptsDir(scriptsDir)
{
    ui->setupUi(this);
    fillNamesCombo();
}

AddPythonScriptDialog::~AddPythonScriptDialog()
{
    delete ui;
}

void AddPythonScriptDialog::accept()
{
    m_scriptName = ui->nameComboBox->currentText().trimmed();

    if ( !checkIdentifierName(m_scriptName) ) {
        QWorkMainWindow::warning(tr("Invalid Identifier \"%1\"").arg(m_scriptName));
        ui->nameComboBox->setFocus();
        return;
    }

    if ( m_scripts->find(m_scriptName) != m_scripts->end() ) {
        QWorkMainWindow::warning(tr("Script with name \"%1\" Exists").arg(m_scriptName));
        ui->nameComboBox->setFocus();
        return;
    }

    m_scriptDescription = ui->descriptionLineEdit->text().trimmed();
    QDialog::accept();
}

void AddPythonScriptDialog::fillNamesCombo()
{
    QDir dir(m_scriptsDir);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Name);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        if ( fileInfo.completeSuffix().toUpper() == QLatin1String("PY") )
            ui->nameComboBox->addItem(fileInfo.baseName());
    }
    ui->nameComboBox->setCurrentIndex(-1);
}
