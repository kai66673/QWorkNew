#ifndef SEARCHREPLACETEXTDIALOG_H
#define SEARCHREPLACETEXTDIALOG_H

#include "textfindconstants.h"

#include <QDialog>

namespace Ui {
    class SearchReplaceTextDialog;
}

namespace TextEditor {
    class SimpleBaseFileFind;
}

namespace Find {
    class SearchResultWindow;
}

class SearchReplaceTextDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchReplaceTextDialog( TextEditor::SimpleBaseFileFind *find, const QString &textToReplace, Find::FindFlags findFlags, QWidget *parent = 0 );
    ~SearchReplaceTextDialog();

protected:
    virtual void closeEvent( QCloseEvent *event );

private slots:
    void onSearchFifnished();
    void onReplace();

private:
    Ui::SearchReplaceTextDialog *ui;
    TextEditor::SimpleBaseFileFind *m_find;
    Find::SearchResultWindow *m_resultWindow;
};

#endif // SEARCHREPLACETEXTDIALOG_H
