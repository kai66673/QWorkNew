#ifndef SESSIONSDIALOG_H
#define SESSIONSDIALOG_H

#include <QDialog>

#include "corelib_global.h"

namespace Ui {
class SessionsDialog;
}

class CORELIB_EXPORT SessionsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SessionsDialog(QWidget *parent = 0);
    ~SessionsDialog();

private slots:
    void onRestoreSessionCheckToggled( bool onoff );
    void onCurrentSessionChanged( int row );
    void onSessionDelete();
    void onSessionNew();
    void onSwitchToSession();
    
private:
    Ui::SessionsDialog *ui;

    void updateUi();
};

#endif // SESSIONSDIALOG_H
