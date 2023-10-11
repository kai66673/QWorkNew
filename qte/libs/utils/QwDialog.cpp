#include <QSettings>

#include "QwDialog.h"

QwDialog::QwDialog( const QString &className, const QString &orgName, const QString &appName,
                    QWidget *parent, Qt::WindowFlags f )
    : QDialog(parent, f)
    , m_className(className)
    , m_orgName(orgName)
    , m_appName(appName)
{
}

QwDialog::~QwDialog()
{
}

int QwDialog::exec()
{
    restoreDialogGeomtry();
    return QDialog::exec();
}

void QwDialog::accept()
{
    saveDialogGeomtry();
    QDialog::accept();
}

void QwDialog::reject()
{
    saveDialogGeomtry();
    QDialog::reject();
}

void QwDialog::closeEvent( QCloseEvent *event )
{
    saveDialogGeomtry();
    QDialog::closeEvent(event);
}

void QwDialog::saveDialogGeomtry()
{
    QSettings settings(m_orgName, m_appName);
    settings.beginGroup(m_className);
    settings.setValue("x", pos().x());
    settings.setValue("y", pos().y());
    settings.setValue("width", width());
    settings.setValue("height", height());
    settings.endGroup();
}

void QwDialog::restoreDialogGeomtry()
{
    QSettings settings(m_orgName, m_appName);
    settings.beginGroup(m_className);
    int x = settings.value("x", 200).toInt();
    int y = settings.value("y", 200).toInt();
    int w = settings.value("width", 640).toInt();
    int h = settings.value("height", 480).toInt();
    settings.endGroup();
    move(x, y);
    resize(w, h);
}
