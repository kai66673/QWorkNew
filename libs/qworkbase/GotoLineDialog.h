#ifndef QGOTOLINEDIALOG_H
#define QGOTOLINEDIALOG_H

#include <QDialog>

namespace Ui {
    class GotoLineDialog;
}

class GotoLineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GotoLineDialog( int lineCount, int currentLine, QWidget *parent = 0 );
    ~GotoLineDialog();

    int lineNumber() const;

private:
    Ui::GotoLineDialog *ui;
};

#endif // QGOTOLINEDIALOG_H
