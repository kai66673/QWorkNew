#include "ConnectDialog.h"
#include "ui_ConnectDialog.h"

ConnectDialog::ConnectDialog( const QList <Database::IDbLoader *> &providers, QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);
    for (int i = 0; i < providers.size(); i++) {
        Database::IDbConnectWidget *authWidget = providers[i]->createAuthWidget(this);
        if ( authWidget ) {
            ui->connTypeComboBox->addItem(providers[i]->databaseType());
            ui->stackedAuthWidget->addWidget(authWidget);
        }
    }

    connect(ui->connTypeComboBox, SIGNAL(currentIndexChanged(int)), ui->stackedAuthWidget, SLOT(setCurrentIndex(int)));
    ui->connTypeComboBox->setFocus();
}

ConnectDialog::~ConnectDialog()
{
    delete ui;
}

void ConnectDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

int ConnectDialog::getConnectionType() const
{
    return ui->connTypeComboBox->currentIndex();
}

void ConnectDialog::tryAccept()
{
    int connTypeIndex = ui->connTypeComboBox->currentIndex();
    if ( connTypeIndex == -1 )
        reject();
    else {
        m_connParameters.clear();
        Database::IDbConnectWidget *authWidget = static_cast<Database::IDbConnectWidget *>(ui->stackedAuthWidget->widget(connTypeIndex));
        if ( authWidget->checkFillAuthParameters(m_connParameters) )
            accept();
    }
}
