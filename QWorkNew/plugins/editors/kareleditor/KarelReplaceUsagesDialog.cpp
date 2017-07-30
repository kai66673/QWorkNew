#include <QTextBlock>

#include "ITextDocument.h"
#include "basefilefind.h"

#include "KarelReplaceUsagesDialog.h"
#include "ui_KarelReplaceUsagesDialog.h"

KarelReplaceUsagesDialog::KarelReplaceUsagesDialog(QTextDocument *document, const QString &filePath, const QString &replacement, const QList<const Karel::Token *> tokens, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::KarelReplaceUsagesDialog)
    , m_resultWindow(new Find::SearchResultWindow(0))
{
    ui->setupUi(this);

    ui->stackedWidget->addWidget(m_resultWindow->outputWidget(0));
    ui->stackedWidget->setCurrentIndex(0);
    ui->lineEdit->setText(replacement);

    Find::SearchResult *search = m_resultWindow->startNewSearch(Find::SearchResultWindow::SearchAndReplace);
    connect(search, SIGNAL(activated(Find::SearchResultItem)), this, SLOT(openEditor(Find::SearchResultItem)));

    foreach (const Karel::Token *tk, tokens) {
        QTextCursor tc(document);
        tc.setPosition(tk->begin());
        m_resultWindow->addResult(filePath,
                                 tc.blockNumber() + 1,
                                 tc.block().text(),
                                 tk->begin() - tc.block().position(),
                                 tk->length());
    }

    m_resultWindow->finishSearch();

    connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(onReplacementTextChanged(QString)));
    onReplacementTextChanged(ui->lineEdit->text());

    connect(ui->replaceButton, SIGNAL(clicked()), this, SLOT(onReplace()));
}

KarelReplaceUsagesDialog::~KarelReplaceUsagesDialog()
{
    delete ui;
}

void KarelReplaceUsagesDialog::openEditor(const Find::SearchResultItem &item)
{
    if (item.path.size() > 0) {
        DocumentManager::getInstance()->openDocument( item.path.first(),
                                                      new TextDocumentCursor(item.lineNumber - 1, item.textMarkPos) );
    } else {
        DocumentManager::getInstance()->openDocument(item.text);
    }
}

void KarelReplaceUsagesDialog::onReplacementTextChanged(const QString &txt)
{
    ui->replaceButton->setEnabled(!txt.trimmed().isEmpty());
}

void KarelReplaceUsagesDialog::onReplace()
{
    TextEditor::BaseFileFind::replaceAll(ui->lineEdit->text().trimmed(), m_resultWindow->checkedItems());
    accept();
}
