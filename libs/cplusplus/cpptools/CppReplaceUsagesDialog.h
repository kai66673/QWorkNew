#ifndef CPPREPLACEUSAGESDIALOG_H
#define CPPREPLACEUSAGESDIALOG_H

#include <QDialog>
#include "Symbol.h"
#include "searchresultwindow.h"
#include "cppfindreferences.h"

#include "cpptools_global.h"

namespace Ui {
    class CppReplaceUsagesDialog;
}

class CPPTOOLS_EXPORT CppReplaceUsagesDialog : public QDialog
{
    Q_OBJECT

public:
    CppReplaceUsagesDialog( CPlusPlus::Symbol *symbol, const CPlusPlus::LookupContext &context,
                            const QString &replacement, CppTools::Internal::CppFindReferences *cppFindReferences,
                            QWidget *parent = 0 );

    CppReplaceUsagesDialog( const QList<CPlusPlus::Usage> &usages, const QString &replacement, QWidget *parent = 0 );

    ~CppReplaceUsagesDialog();

protected:
    virtual void closeEvent( QCloseEvent *event );

private slots:
    void onSearchFinished();
    void onReplace();
    void onCancel();

signals:
    void replaceButtonClicked(const QString &text, const QList<Find::SearchResultItem> &items);
    void searchCanceled();
    void openEditor(const Find::SearchResultItem &item);

private:
    Ui::CppReplaceUsagesDialog *ui;
    Find::SearchResultWindow *m_resultWindow;
};

#endif // CPPREPLACEUSAGESDIALOG_H
