#include <QFileDialog>

#include "Core.h"
#include "NewFolderDialog.h"
#include "ui_NewFolderDialog.h"

NewFolderDialog::NewFolderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewFolderDialog)
{
    ui->setupUi(this);
    ui->folderTypeComboBox->addItems( QStringList() << tr("Not Assigned") << tr("Default Source") << tr("Default Header") );
    connect(ui->defSourcePathToolButton, SIGNAL(clicked()), this, SLOT(onChangeDefaultSourcePath()));
}

NewFolderDialog::~NewFolderDialog()
{
    delete ui;
}

QString NewFolderDialog::folderName() const
{
    return ui->nameLineEdit->text().trimmed();
}

QString NewFolderDialog::folderDescription() const
{
    return ui->descriptionLineEdit->text().trimmed();
}

QString NewFolderDialog::defaultSourcePath() const
{
    return ui->defSourcePathLineEdit->text();
}

FolderNode::FolderType NewFolderDialog::folderType() const
{
    switch ( ui->folderTypeComboBox->currentIndex() ) {
        case 1: return FolderNode::DefaultSource;
        case 2: return FolderNode::DefaultHeader;
    }

    return FolderNode::NotAssigned;
}

void NewFolderDialog::setFolderName( const QString &name )
{
    ui->nameLineEdit->setText(name);
}

void NewFolderDialog::setFolderDescription( const QString &description )
{
    ui->descriptionLineEdit->setText(description);
}

void NewFolderDialog::setDefaultSourcePath( const QString &path )
{
    ui->defSourcePathLineEdit->setText(path);
}

void NewFolderDialog::setFolderType( FolderNode::FolderType folderType )
{
    qDebug() << "NewFolderDialog::setFolderType" << folderType;

    switch ( folderType ) {
        case FolderNode::NotAssigned:      ui->folderTypeComboBox->setCurrentIndex(0); break;
        case FolderNode::DefaultSource:    ui->folderTypeComboBox->setCurrentIndex(1); break;
        case FolderNode::DefaultHeader:    ui->folderTypeComboBox->setCurrentIndex(2); break;
    }
}

void NewFolderDialog::tryAccept()
{
    if ( ui->nameLineEdit->text().trimmed() == "" ) {
        Core::Storage::warning(tr("Folder Name Cannot be Empty"));
        ui->nameLineEdit->setFocus();
        return;
    }

    accept();
}

void NewFolderDialog::onChangeDefaultSourcePath()
{
    QString dirName = QFileDialog::getExistingDirectory( Core::Storage::mainWindow()->widget(),
                                                         tr("Select Path"),
                                                         ui->defSourcePathLineEdit->text(),
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );

    if ( dirName.isEmpty() )
        return;

    ui->defSourcePathLineEdit->setText(dirName);
}
