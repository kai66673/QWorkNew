#ifndef QFINDINDIALOG_H
#define QFINDINDIALOG_H

#include <QDialog>

namespace Ui {
    class FindInDialog;
}

class BaseSearchModePlugin;

class FindInDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindInDialog( const QString &txt, QWidget *parent = 0 );
    ~FindInDialog();

private slots:
    void onCurrentScopeChanged( int scopeIndex );
    void onSearch();
    void onReplace();

private:
    void searchOrReplace( bool isSearch );
    Ui::FindInDialog *ui;
    BaseSearchModePlugin *m_baseSearchMode;
};

#endif // QFINDINDIALOG_H
