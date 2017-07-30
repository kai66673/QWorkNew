#ifndef CONFIRMREMOVESCRIPTDIALOG_H
#define CONFIRMREMOVESCRIPTDIALOG_H

#include <QDialog>

namespace Ui {
class ConfirmRemoveScriptDialog;
}

class ConfirmRemoveScriptDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ConfirmRemoveScriptDialog( const QString &message, QWidget *parent = 0 );
    ~ConfirmRemoveScriptDialog();

    bool removeFromDisk() const;
    
private:
    Ui::ConfirmRemoveScriptDialog *ui;
};

#endif // CONFIRMREMOVESCRIPTDIALOG_H
