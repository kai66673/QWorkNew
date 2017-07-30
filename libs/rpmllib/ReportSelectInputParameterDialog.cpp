#include <QStandardItemModel>
#include <QMessageBox>

#include "ReportSelectInputParameterDialog.h"
#include "ui_ReportSelectInputParameterDialog.h"
#include "Core.h"

ReportSelectInputParameterDialog::ReportSelectInputParameterDialog( QList<RPML::ReportInputParameter *> *parameters, QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::ReportSelectInputParameterDialog)
    , m_parameters(parameters)
    , m_index(-1)
{
    ui->setupUi(this);

    QStandardItemModel *model = new QStandardItemModel(m_parameters->size(), 2, this);
    model->setHorizontalHeaderLabels( QStringList() << tr("ID") << tr("Description") );

    for (int i = 0; i < m_parameters->size(); i++) {
        QStandardItem *itemId = new QStandardItem(QString("%1").arg(m_parameters->at(i)->id));
        model->setItem(i, 0, itemId);
        QStandardItem *itemDescription = new QStandardItem(m_parameters->at(i)->description);
        model->setItem(i, 1, itemDescription);
    }

    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setModel(model);
}

ReportSelectInputParameterDialog::~ReportSelectInputParameterDialog()
{
    delete ui;
}

void ReportSelectInputParameterDialog::on_okButton_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if ( !index.isValid() ) {
        Core::Storage::warning(tr("No Parameter Selected"));
        ui->tableView->setFocus();
        return;
    }

    m_index = index.row();
    QDialog::accept();
}
