#ifndef KARELREPLACEUSAGESDIALOG_H
#define KARELREPLACEUSAGESDIALOG_H

#include <QDialog>
#include <QTextDocument>

#include "searchresultwindow.h"
#include "KarelToken.h"

namespace Ui {
class KarelReplaceUsagesDialog;
}

class KarelReplaceUsagesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KarelReplaceUsagesDialog(QTextDocument *document, const QString &filePath, const QString &replacement, const QList<const Karel::Token *> tokens, QWidget *parent = 0);
    ~KarelReplaceUsagesDialog();

private Q_SLOTS:
    void openEditor(const Find::SearchResultItem &item);
    void onReplacementTextChanged(const QString &txt);
    void onReplace();

private:
    Ui::KarelReplaceUsagesDialog *ui;
    Find::SearchResultWindow *m_resultWindow;
};

#endif // KARELREPLACEUSAGESDIALOG_H
