#include "ListSelectorDialog.h"
#include "ui_ListSelectorDialog.h"

ListSelectorDialog::ListSelectorDialog( const QString &title, const QStringList &strings, QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::ListSelectorDialog)
    , m_wizardsChoosen(false)
{
    ui->setupUi(this);

    setWindowTitle(title);
    ui->listWidget->addItems(strings);
    ui->listWidget->setCurrentRow(0);
}

ListSelectorDialog::~ListSelectorDialog()
{
    delete ui;
}

int ListSelectorDialog::getCurrentIndex()
{
    return m_wizardsChoosen ? -2 : ui->listWidget->currentRow();
}

void ListSelectorDialog::on_wizardsButton_clicked()
{
    m_wizardsChoosen = true;
    accept();
}
