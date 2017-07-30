#include <QCloseEvent>

#include "basefilefind.h"
#include "searchresultwindow.h"
#include "SearchReplaceTextDialog.h"
#include "ui_SearchReplaceTextDialog.h"

SearchReplaceTextDialog::SearchReplaceTextDialog( TextEditor::SimpleBaseFileFind *find, const QString &textToReplace, Find::FindFlags findFlags, QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::SearchReplaceTextDialog)
    , m_find(find)
    , m_resultWindow(find->resultWindow())
{
    ui->setupUi(this);

    ui->replaceTextLineEdit->setText(textToReplace);

    ui->searchResultStackedWidget->addWidget(m_resultWindow->outputWidget(0));
    ui->searchResultStackedWidget->setCurrentIndex(0);

    Find::SearchResult *searchResult = m_resultWindow->startNewSearch(Find::SearchResultWindow::SearchAndReplace);

    m_find->replaceAll(textToReplace, findFlags);
    connect(searchResult, SIGNAL(activated(Find::SearchResultItem)), m_find, SLOT(openEditor(Find::SearchResultItem)));

    connect(ui->cancelPushButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->replacePushButton, SIGNAL(clicked()), this, SLOT(onReplace()));
    connect(m_find, SIGNAL(searchTextFinished()), this, SLOT(onSearchFifnished()));
}

SearchReplaceTextDialog::~SearchReplaceTextDialog()
{
    delete m_resultWindow;
    delete ui;
}

void SearchReplaceTextDialog::closeEvent( QCloseEvent *event )
{
    if ( m_find->canCancel() )
        m_find->cancel();
    event->accept();
}

void SearchReplaceTextDialog::onSearchFifnished()
{
    ui->replacePushButton->setEnabled(true);
}

void SearchReplaceTextDialog::onReplace()
{
    QString text = ui->replaceTextLineEdit->text();
    TextEditor::BaseFileFind::replaceAll(text, m_resultWindow->checkedItems());
    accept();
}
