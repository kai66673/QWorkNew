#include <QUrl>
#include <QApplication>
#include <QClipboard>

#include "Core.h"
#include "NewUrlDialog.h"
#include "ui_NewUrlDialog.h"
#include "IDocument.h"
#include "itexteditor.h"

NewUrlDialog::NewUrlDialog( QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::NewUrlDialog)
{
    ui->setupUi(this);

    if ( Core::IEditor *editor = DocumentManager::getInstance()->currentEditor() ) {
        if ( TextEditor::ITextEditor *textEditor = qobject_cast<TextEditor::ITextEditor *>(editor) ) {
            QString selectedText = textEditor->selectedText().trimmed();
            QUrl url(selectedText);
            if ( url.isValid() ) {
                ui->urlEdit->setText(selectedText);
                return;
            }
        }
    }

    QString clipbordText = QApplication::clipboard()->text().trimmed();
    QUrl url(clipbordText);
    if ( url.isValid() )
        ui->urlEdit->setText(clipbordText);
}

NewUrlDialog::~NewUrlDialog()
{
    delete ui;
}

FavoriteData *NewUrlDialog::favoriteData( const QString &id ) const
{
    return new FavoriteData(id, ui->nameEdit->text().trimmed(), ui->urlEdit->text().trimmed(), 0);
}

void NewUrlDialog::accept()
{
    if ( ui->nameEdit->text().trimmed().isEmpty() ) {
        Core::Storage::warning(tr("No URL Name Specified"));
        ui->nameEdit->setFocus();
        return;
    }

    QString urlString = ui->urlEdit->text().trimmed();
    QUrl url(urlString);
    if ( !url.isValid() ) {
        Core::Storage::warning(tr("No Valid URL Specified"));
        ui->urlEdit->setFocus();
        return;
    }

    QDialog::accept();
}
