#ifndef DOCKCORNERSETTINGSDIALOG_H
#define DOCKCORNERSETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class DockCornerSettingsDialog;
}

class DockCornerSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DockCornerSettingsDialog(QWidget *parent = 0);
    ~DockCornerSettingsDialog();

public slots:
    virtual void accept();

private:
    Ui::DockCornerSettingsDialog *ui;
};

#endif // DOCKCORNERSETTINGSDIALOG_H
