#include <QMessageBox>

#include "Core.h"
#include "SearchEngine.h"

#include "FindInDialog.h"
#include "ui_FindInDialog.h"

FindInDialog::FindInDialog( const QString &txt, QWidget *parent )
    : QDialog(parent),
    ui(new Ui::FindInDialog)
{
    ui->setupUi(this);

    ui->findTextLineEdit->setText(txt);

    m_baseSearchMode = SearchEngine::getInstance()->getBaseSearchMode();

    ui->searchParamsStackedWidget->addWidget(new QWidget(ui->searchParamsStackedWidget));
    for (int i = 0; i < m_baseSearchMode->findScopeCount(); i++) {
        IFindScope *findScope = m_baseSearchMode->findScopeAt(i);
        ui->findInComboBox->addItem(findScope->name());
        ui->searchParamsStackedWidget->addWidget(findScope->createFindParamsWidget(ui->searchParamsStackedWidget));
    }

    connect(ui->closePushButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->findInComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentScopeChanged(int)));
    connect(ui->searchPushButton, SIGNAL(clicked()), this, SLOT(onSearch()));
    connect(ui->searchAndReplacePushButton, SIGNAL(clicked()), this, SLOT(onReplace()));

    ui->findInComboBox->setCurrentIndex(-1);
}

FindInDialog::~FindInDialog()
{
    delete ui;
}

void FindInDialog::onCurrentScopeChanged(int scopeIndex)
{
    if ( scopeIndex == -1 ) {
        ui->searchPushButton->setEnabled(false);
        ui->searchAndReplacePushButton->setEnabled(false);
    }

    ui->searchParamsStackedWidget->setCurrentIndex(scopeIndex + 1);

    if ( scopeIndex != -1 ) {
        IFindScope *findScope = m_baseSearchMode->findScopeAt(scopeIndex);
        ui->searchPushButton->setEnabled(findScope->isSearchEnabled());
        ui->searchAndReplacePushButton->setEnabled(findScope->isSearchAndReplaceEnabled());
    }
}

void FindInDialog::onSearch()
{
    searchOrReplace(true);
}

void FindInDialog::onReplace()
{
    searchOrReplace(false);
}

void FindInDialog::searchOrReplace( bool isSearch )
{
    QString searchText = ui->findTextLineEdit->text();
    if ( searchText.isEmpty() ) {
        Core::Storage::warning(tr("Search Text is Empty!"));
        ui->findTextLineEdit->setFocus();
        return;
    }

    int scopeIndex = ui->findInComboBox->currentIndex();
    if ( scopeIndex == -1 )
        return;

    IFindScope *findScope =  m_baseSearchMode->findScopeAt(scopeIndex);
    if ( isSearch ) {
        if ( findScope->prepareToSearch() )
            findScope->startSearch(searchText);
        else
            return;
    }
    else {
        if ( findScope->prepareToSearchAndReplace() )
            findScope->startSearchAndReplace(searchText);
        else
            return;
    }

    accept();
}
