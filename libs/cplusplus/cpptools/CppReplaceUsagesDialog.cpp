#include <QMessageBox>
#include <QCloseEvent>

#include "CppReplaceUsagesDialog.h"
#include "ui_CppReplaceUsagesDialog.h"

CppReplaceUsagesDialog::CppReplaceUsagesDialog( CPlusPlus::Symbol *symbol, const CPlusPlus::LookupContext &context,
                                                const QString &replacement, CppTools::Internal::CppFindReferences *cppFindReferences,
                                                QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::CppReplaceUsagesDialog)
    , m_resultWindow(new Find::SearchResultWindow(cppFindReferences))
{
    ui->setupUi(this);

    ui->searchResultStackedWidget->addWidget(m_resultWindow->outputWidget(0));
    ui->searchResultStackedWidget->setCurrentIndex(0);
    ui->lineEdit->setText(replacement);

    cppFindReferences->renameUsages(symbol, context, replacement, m_resultWindow);

    connect(cppFindReferences,    SIGNAL(finished()), this, SLOT(onSearchFinished()));
    connect(ui->replacePushButton,  SIGNAL(clicked()),  this, SLOT(onReplace()));
    connect(ui->cancelPushButton,   SIGNAL(clicked()),  this, SLOT(onCancel()));
}

CppReplaceUsagesDialog::CppReplaceUsagesDialog( const QList<CPlusPlus::Usage> &usages, const QString &replacement, QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::CppReplaceUsagesDialog)
    , m_resultWindow(new Find::SearchResultWindow(0))
{
    ui->setupUi(this);

    ui->searchResultStackedWidget->addWidget(m_resultWindow->outputWidget(0));
    ui->searchResultStackedWidget->setCurrentIndex(0);
    ui->lineEdit->setText(replacement);

    connect(ui->replacePushButton,  SIGNAL(clicked()),  this, SLOT(onReplace()));
    connect(ui->cancelPushButton,   SIGNAL(clicked()),  this, SLOT(onCancel()));

    Find::SearchResult *search = m_resultWindow->startNewSearch(Find::SearchResultWindow::SearchAndReplace);
    connect(search, SIGNAL(activated(Find::SearchResultItem)), this, SIGNAL(openEditor(Find::SearchResultItem)));

    foreach ( const CPlusPlus::Usage &result, usages ) {
        m_resultWindow->addResult( result.path,
                                   result.line,
                                   result.lineText,
                                   result.col,
                                   result.len );
    }

    m_resultWindow->finishSearch();

    onSearchFinished();
}

CppReplaceUsagesDialog::~CppReplaceUsagesDialog()
{
    delete ui;
}

void CppReplaceUsagesDialog::onSearchFinished()
{
    ui->replacePushButton->setEnabled(true);
}

void CppReplaceUsagesDialog::onReplace()
{
    QString replaceText = ui->lineEdit->text();
    if ( replaceText == QLatin1String("") ) {
        QMessageBox::warning(parentWidget(), tr("Replace Usages"), tr("Replace Text Cannot be Empty!"));
        ui->lineEdit->setFocus();
        return;
    }

    emit replaceButtonClicked(replaceText, m_resultWindow->checkedItems());
    QDialog::accept();
}

void CppReplaceUsagesDialog::onCancel()
{
    emit searchCanceled();
    QDialog::reject();
}

void CppReplaceUsagesDialog::closeEvent( QCloseEvent *event )
{
    emit searchCanceled();
    event->accept();
}
