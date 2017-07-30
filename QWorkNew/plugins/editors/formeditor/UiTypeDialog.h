#ifndef UITYPEDIALOG_H
#define UITYPEDIALOG_H

#include <QDialog>

namespace Ui {
class UiTypeDialog;
}

class UiTypeDialog : public QDialog
{
    Q_OBJECT

public:
    enum UiType {
        Widget,
        Dialog
    };

    explicit UiTypeDialog(QWidget *parent = 0);
    ~UiTypeDialog();

    int selctedUiType() const;

private:
    Ui::UiTypeDialog *ui;
};

#endif // UITYPEDIALOG_H
