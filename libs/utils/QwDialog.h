#ifndef QWDIALOG_H
#define QWDIALOG_H

#include <QDialog>

#include "utils_global.h"

class QTCREATOR_UTILS_EXPORT QwDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QwDialog( const QString &className, const QString &orgName, const QString &appName,
                       QWidget *parent = 0, Qt::WindowFlags f = 0 );
    virtual ~QwDialog();

signals:

public slots:
    int exec();
    virtual void accept();
    virtual void reject();

protected:
    virtual void closeEvent( QCloseEvent *event );

    virtual void saveDialogGeomtry();
    virtual void restoreDialogGeomtry();

    QString m_className;
    QString m_orgName;
    QString m_appName;
};

#endif // QWDIALOG_H
