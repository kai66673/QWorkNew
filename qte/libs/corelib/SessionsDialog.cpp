#include <QInputDialog>
#include <QDir>
#include <QMessageBox>

#include "SessionsDialog.h"
#include "ui_SessionsDialog.h"
#include "SessionManager.h"

SessionsDialog::SessionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SessionsDialog)
{
    ui->setupUi(this);
    updateUi();

    connect(ui->restoreCheckBox,    SIGNAL(toggled(bool)),          this, SLOT(onRestoreSessionCheckToggled(bool)));
    connect(ui->listWidget,         SIGNAL(currentRowChanged(int)), this, SLOT(onCurrentSessionChanged(int)));
    connect(ui->deleteButton,       SIGNAL(clicked()),              this, SLOT(onSessionDelete()));
    connect(ui->newButton,          SIGNAL(clicked()),              this, SLOT(onSessionNew()));
    connect(ui->switchToButton,     SIGNAL(clicked()),              this, SLOT(onSwitchToSession()));

    ui->listWidget->setCurrentRow(-1);
}

SessionsDialog::~SessionsDialog()
{
    delete ui;
}

void SessionsDialog::onRestoreSessionCheckToggled( bool onoff )
{
    SessionManager *sessionManager = SessionManager::getInstance();
    sessionManager->m_restoreLastSession = onoff;
}

void SessionsDialog::onCurrentSessionChanged( int row )
{
    if ( row == -1 ) {
        ui->switchToButton->setEnabled(false);
        ui->deleteButton->setEnabled(false);
        return;
    }

    SessionManager *sessionManager = SessionManager::getInstance();
    ui->switchToButton->setEnabled(row != sessionManager->m_currentSessionIndex);
    ui->deleteButton->setEnabled(row != 0);
}

void SessionsDialog::onSessionDelete()
{
    int row = ui->listWidget->currentRow();
    if ( row > 0 ) {
        SessionManager::getInstance()->deleteSession(row);
        updateUi();
    }
}

void SessionsDialog::onSessionNew()
{
    bool ok;
    QString sessionName = QInputDialog::getText( this, tr("Input New Session Name"),
                                                 tr("Session Name:"), QLineEdit::Normal,
                                                 QDir::home().dirName(), &ok );
    if ( ok ) {
        SessionManager *sessionManager = SessionManager::getInstance();
        sessionName = sessionName.trimmed();
        if ( sessionName.isEmpty() ) {
            QMessageBox::warning(this, tr("Wrong Session Name"), tr("Session Name Cannot be Empty"));
            return;
        }
        for (int i = 0; i < sessionManager->m_sessions.size(); i++) {
            if ( !sessionManager->m_sessions[i].compare(sessionName, Qt::CaseInsensitive) ) {
                QMessageBox::warning(this, tr("Wrong Session Name"), tr("Exists Session with Same Name"));
                return;
            }
        }
        sessionManager->m_sessions.append(sessionName);
        sessionManager->m_currentSessionIndex = sessionManager->m_sessions.size() - 1;
        accept();
    }
}

void SessionsDialog::onSwitchToSession()
{
    SessionManager::getInstance()->switchToSession(ui->listWidget->currentRow(), true);
    accept();
}

void SessionsDialog::updateUi()
{
    SessionManager *sessionManager = SessionManager::getInstance();

    ui->restoreCheckBox->setChecked(sessionManager->m_restoreLastSession);

    ui->listWidget->clear();

    {
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setText(sessionManager->m_sessions[0]);
        QFont font = newItem->font();
        font.setItalic(true);
        if ( sessionManager->m_currentSessionIndex == 0 )
            font.setBold(true);
        newItem->setFont(font);
        ui->listWidget->addItem(newItem);
    }

    for (int i = 1; i < sessionManager->m_sessions.size(); i++){
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setText(sessionManager->m_sessions[i]);
        if ( i == sessionManager->m_currentSessionIndex ) {
            QFont font = newItem->font();
            font.setBold(true);
            newItem->setFont(font);
        }
        ui->listWidget->addItem(newItem);
    }

    ui->listWidget->setCurrentRow(-1);
}
