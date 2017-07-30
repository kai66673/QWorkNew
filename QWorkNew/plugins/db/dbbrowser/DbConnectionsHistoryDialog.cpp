#include "DbConnectionsHistoryDialog.h"
#include "ui_DbConnectionsHistoryDialog.h"

#include "DbManager.h"
#include "DbLoader.h"

DbConnectionsHistoryDialog::DbConnectionsHistoryDialog( QList<DbConnectionAuthInfo> *connections_, QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::DbConnectionsHistoryDialog)
    , connections(connections_)
    , m_selectedIndex(-1)
{
    ui->setupUi(this);
    setupConnections();
}

DbConnectionsHistoryDialog::~DbConnectionsHistoryDialog()
{
    delete ui;
}

void DbConnectionsHistoryDialog::setupConnections()
{
    Database::IDbLoader *loader;
    Database::DbManager *manager = Database::DbManager::getInstance();
    for (int i = 0; i < connections->size(); i++) {
        loader = manager->loaderByDatabaseType(connections->at(i).databaseType);
        if ( !loader ) {
            connections->removeAt(i);
            i--;
            continue;
        }
//        QString connInfo = connections->at(i).databaseType;
//        for (int j = 0; j < connections->at(i).parameters.size() - 1; j++) {
//            connInfo += ": ";
//            connInfo += connections->at(i).parameters[j];
//        }
//        ui->connectionsListWidget->addItem(connInfo);
        ui->connectionsListWidget->addItem(connections->at(i).description);
    }

    if ( connections->size() > 0 )
        ui->connectionsListWidget->setCurrentRow(0);
    updateStates();

    connect(ui->connectionsListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(updateStates()));

    connect(ui->connectButton,  SIGNAL(clicked()), this, SLOT(selectConnection()));
    connect(ui->removeButton,   SIGNAL(clicked()), this, SLOT(removeConnection()));
    connect(ui->clearButton,    SIGNAL(clicked()), this, SLOT(clearConnections()));
    connect(ui->closeButton,    SIGNAL(clicked()), this, SLOT(reject()));
}

void DbConnectionsHistoryDialog::updateStates()
{
    ui->clearButton->setEnabled(connections->size() > 0);
    ui->removeButton->setEnabled(ui->connectionsListWidget->currentRow() != -1);
    ui->connectButton->setEnabled(ui->connectionsListWidget->currentRow() != -1);
}

void DbConnectionsHistoryDialog::selectConnection()
{
    m_selectedIndex = ui->connectionsListWidget->currentRow();
    accept();
}

void DbConnectionsHistoryDialog::removeConnection()
{
    int rowIndex = ui->connectionsListWidget->currentRow();
    if ( rowIndex == -1 )
        return;

    delete ui->connectionsListWidget->takeItem(rowIndex);
    connections->removeAt(rowIndex);

    updateStates();
}

void DbConnectionsHistoryDialog::clearConnections()
{
    ui->connectionsListWidget->clear();
    connections->clear();

    updateStates();
}
