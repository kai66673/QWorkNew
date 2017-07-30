#ifndef QLISTSELECTORDIALOG_H
#define QLISTSELECTORDIALOG_H

#include <QDialog>

namespace Ui {
    class ListSelectorDialog;
}

class ListSelectorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ListSelectorDialog( const QString &title, const QStringList &strings, QWidget *parent = 0 );
    ~ListSelectorDialog();

    int getCurrentIndex();

private slots:
    void on_wizardsButton_clicked();

private:
    Ui::ListSelectorDialog *ui;
    bool m_wizardsChoosen;
};

#endif // QLISTSELECTORDIALOG_H
