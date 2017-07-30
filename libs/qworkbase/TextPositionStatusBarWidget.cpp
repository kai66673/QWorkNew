#include "TextPositionStatusBarWidget.h"
#include "ui_TextPositionStatusBarWidget.h"

#include "GotoLineDialog.h"

TextPositionStatusBarWidget::TextPositionStatusBarWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TextPositionStatusBarWidget)
{
    ui->setupUi(this);

    connect(ui->gotoLineToolButton, SIGNAL(clicked()), this, SLOT(onGotoLineNumber()));
}

TextPositionStatusBarWidget::~TextPositionStatusBarWidget()
{
    delete ui;
}

void TextPositionStatusBarWidget::onCurrentDocumentChanged( IDocument *document )
{
    if ( document ) {
        if ( ITextDocument *text = dynamic_cast<ITextDocument *>(document) ) {
            // Need update document position
            ui->gotoLineToolButton->setEnabled(true);
            ui->lineLineEdit->setText(QString::number(text->line() + 1));
            ui->columnLineEdit->setText(QString::number(text->column()));
            ui->selectedLineEdit->setText(QString::number(text->selectedLength()));
            return;
        }
    }

    // Need Hide document position
    ui->gotoLineToolButton->setEnabled(false);
    ui->lineLineEdit->setText("");
    ui->columnLineEdit->setText("");
    ui->selectedLineEdit->setText("");
}

void TextPositionStatusBarWidget::onGotoLineNumber()
{
    IDocument *document = DocumentManager::getInstance()->currentDocument();
    if ( document ) {
        if ( ITextDocument *textDocument = dynamic_cast<ITextDocument *>(document) ) {
            GotoLineDialog gotoLineDialog(textDocument->lineCount(), textDocument->line());
            if ( gotoLineDialog.exec() == QDialog::Accepted ) {
                int newLineNumber = gotoLineDialog.lineNumber() - 1;
                TextDocumentCursor position(newLineNumber, 0);
                document->setCursor(&position);
            }
        }
    }
}
