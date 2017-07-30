#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QListWidget>
#include <QInputDialog>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "Core.h"
#include "NewProjectDialog.h"
#include "ui_NewProjectDialog.h"

NewEditProjectDialog::NewEditProjectDialog( const QString &name, const QString &description, const QString &location,
                                            const QStringList &includes, const QStringList &systemIncludes, const QStringList &defines,
                                            const QString &precompiledHeader,
                                            QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::NewProjectDialog)
    , m_editMode(false)
{
    ui->setupUi(this);

    if ( !location.isEmpty() )
        m_editMode = true;

    ui->projectNameLineEdit->setText(name);
    ui->projectDescriptionLineEdit->setText(description);
    ui->locationLineEdit->setText(location);

    ui->includePathsListWidget->addItems(includes);
    ui->systemIncludePathsListWidget->addItems(systemIncludes);
    ui->definesListWidget->addItems(defines);

    ui->precompiledHeaderLineEdit->setText(precompiledHeader);

    if ( m_editMode ) {
        ui->locationLineEdit->setReadOnly(true);
        ui->locationToolButton->setEnabled(false);
        setWindowTitle(tr("Edit Project Properties"));
        ui->okPushButton->setText(tr("Ok"));
    }

    connect(ui->importIncludePathToolButton, SIGNAL(clicked()), this, SLOT(importSystemIncludePath()));
}

NewEditProjectDialog::~NewEditProjectDialog()
{
    delete ui;
}

void NewEditProjectDialog::tryAccept()
{
    QString projectName = ui->projectNameLineEdit->text().trimmed();
    if ( projectName == "" ) {
        ui->projectNameLineEdit->setFocus();
        return;
    }

    QString location = ui->locationLineEdit->text();
    if ( location == "" ) {
        ui->locationLineEdit->setFocus();
        return;
    }

    if ( !m_editMode ) {
        QFile file(location);
        if ( file.exists() ) {
            if ( Core::Storage::warning( tr("File \'%1\' Exists.\nFile content will be deleted. Continue?").arg(location),
                                         QMessageBox::Yes | QMessageBox::No, QMessageBox::No) != QMessageBox::Yes )
                return;
        }
    }

    accept();
}

void NewEditProjectDialog::choiseLocation()
{
    QString fileName = QFileDialog::getSaveFileName( Core::Storage::mainWindow()->widget(),
                                                     tr("New Project Database"), QString(), tr("ASOF Project Database (*.aspdb)"), 0,
                                                     QFileDialog::ShowDirsOnly | QFileDialog::DontConfirmOverwrite );

    if ( !fileName.isEmpty() )
        ui->locationLineEdit->setText(fileName);
}

void NewEditProjectDialog::addIncludePath()
{
    QString dirName = QFileDialog::getExistingDirectory( Core::Storage::mainWindow()->widget(),
                                                         tr("Select Path"),
                                                         "",
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );

    if ( dirName.isEmpty() )
        return;

    if ( !ui->includePathsListWidget->findItems(dirName, Qt::MatchFixedString).isEmpty() ) {
        Core::Storage::information(tr("Selected Folder Exists In List"));
        return;
    }

    ui->includePathsListWidget->addItem(dirName);
}

void NewEditProjectDialog::delIncludePath()
{
    int pathRow = ui->includePathsListWidget->currentRow();
    if ( pathRow == -1 )
        return;

    delete ui->includePathsListWidget->takeItem(pathRow);
}

void NewEditProjectDialog::addSystemIncludePath()
{
    QString dirName = QFileDialog::getExistingDirectory( Core::Storage::mainWindow()->widget(),
                                                         tr("Select Path"),
                                                         "",
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );

    if ( dirName.isEmpty() )
        return;

    if ( !ui->systemIncludePathsListWidget->findItems(dirName, Qt::MatchFixedString).isEmpty() ) {
        Core::Storage::information( tr("Selected Folder Exists In List") );
        return;
    }

    ui->systemIncludePathsListWidget->addItem(dirName);
}

void NewEditProjectDialog::delSystemIncludePath()
{
    int pathRow = ui->systemIncludePathsListWidget->currentRow();
    if ( pathRow == -1 )
        return;

    delete ui->systemIncludePathsListWidget->takeItem(pathRow);
}

void NewEditProjectDialog::importSystemIncludePath()
{
    QString filePath = QFileDialog::getOpenFileName( Core::Storage::mainWindow()->widget(),
                                                     tr("Open Project Database"),
                                                     QString(), tr("ASOF Project Database (*.aspdb)") );

    if ( filePath.isEmpty() )
        return;

    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "ASPDB_INCIMP");
        db.setDatabaseName(filePath);
        if ( db.open() ) {
            QString queryString = "select path from includes where issystem = 1";
            QSqlQuery query(db);

            if ( query.exec(queryString) ) {
                ui->systemIncludePathsListWidget->clear();
                while ( query.next() ) {
                    ui->systemIncludePathsListWidget->addItem(query.value(0).toString());
                }
            }
            else
                Core::Storage::warning(tr("Extract Data From Table INCLUDES Failed"));
        }
        else
            Core::Storage::warning(tr("ASPDB-Database File \'%1\' Cannot be Opened").arg(filePath));
    }
    QSqlDatabase::removeDatabase("ASPDB_INCIMP");
}

void NewEditProjectDialog::addDefine()
{
    bool ok;
    QString text = QInputDialog::getText( Core::Storage::mainWindow()->widget(),
                                          tr("Input Define"),
                                          tr("Define:"),
                                          QLineEdit::Normal,
                                          "",
                                          &ok );
    if ( !ok || text.isEmpty() )
        return;

    text = text.trimmed();
    QRegExp rx("(\\w+)");
    if ( !rx.exactMatch(text) ) {
        Core::Storage::information( tr("Wrong Define Name. Should be Simple Word."));
        return;
    }

    ui->definesListWidget->addItem(text);
}

void NewEditProjectDialog::delDefine()
{
    int defineRow = ui->definesListWidget->currentRow();
    if ( defineRow == -1 )
        return;

    delete ui->definesListWidget->takeItem(defineRow);
}


QString NewEditProjectDialog::projectName() const
{
    return ui->projectNameLineEdit->text().trimmed();
}

QString NewEditProjectDialog::projectDescription() const
{
    return ui->projectDescriptionLineEdit->text().trimmed();
}

QString NewEditProjectDialog::projectLocation() const
{
    return ui->locationLineEdit->text();
}

QStringList NewEditProjectDialog::includePaths() const
{
    QStringList paths;
    for (int i = 0; i < ui->includePathsListWidget->count(); i++)
        paths << ui->includePathsListWidget->item(i)->text();

    return paths;
}

QStringList NewEditProjectDialog::systemIncludePaths() const
{
    QStringList paths;
    for (int i = 0; i < ui->systemIncludePathsListWidget->count(); i++)
        paths << ui->systemIncludePathsListWidget->item(i)->text();

    return paths;
}

QStringList NewEditProjectDialog::defines() const
{
    QStringList defines;
    for (int i = 0; i < ui->definesListWidget->count(); i++)
        defines << ui->definesListWidget->item(i)->text();

    return defines;
}

QString NewEditProjectDialog::precompiledHeader() const
{
    QString header = ui->precompiledHeaderLineEdit->text();
    header = header.trimmed();
    return header;
}
