#ifndef NEWFOLDERDIALOG_H
#define NEWFOLDERDIALOG_H

#include <QDialog>

#include "ProjectData.h"

namespace Ui {
    class NewFolderDialog;
}

class NewFolderDialog : public QDialog
{
    Q_OBJECT

public:
    NewFolderDialog(QWidget *parent = 0);
    ~NewFolderDialog();

    QString folderName() const;
    QString folderDescription() const;
    QString defaultSourcePath() const;
    FolderNode::FolderType folderType() const;

    void setFolderName( const QString &name );
    void setFolderDescription( const QString &description );
    void setDefaultSourcePath( const QString &path );
    void setFolderType( FolderNode::FolderType folderType );

private slots:
    void tryAccept();
    void onChangeDefaultSourcePath();

private:
    Ui::NewFolderDialog *ui;
};

#endif // NEWFOLDERDIALOG_H
