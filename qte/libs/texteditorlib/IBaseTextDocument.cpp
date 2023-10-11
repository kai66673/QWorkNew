#include <QTextCodec>
#include <QTextBlock>

#include "IBaseTextDocument.h"
#include "ifile.h"

namespace TextEditor {

IBaseTextDocument::IBaseTextDocument()
    : IDocument()
    , ITextDocument()
    , m_file(0)
    , m_fixOnSaveHandler(0)
{ }

IBaseTextDocument::~IBaseTextDocument()
{
    if ( m_file )
        delete m_file;
}

QWidget *IBaseTextDocument::editorWidget() const
{ return textEditorWidget(); }

Core::IEditor *IBaseTextDocument::editor() const
{ return textEditorWidget()->editor(); }

bool IBaseTextDocument::newDocument(const QString &fileName)
{
    if ( m_file ) {
        delete m_file;
        m_file = NULL;
    }

    m_fileName = fileName;
    textEditorWidget()->setTextCodec(QTextCodec::codecForLocale());
    textEditorWidget()->setFocus();

    return true;
}

bool IBaseTextDocument::newDocumentFromFile(const QString &fileName, const QString &contentFilePath)
{
    if ( m_file ) {
        delete m_file;
        m_file = NULL;
    }

    QString errorString;
    if ( !textEditorWidget()->editor()->open(&errorString, fileName, contentFilePath) )
        return false;

    m_fileName = fileName;
    textEditorWidget()->setTextCodec(QTextCodec::codecForLocale());
    textEditorWidget()->setFocus();
    textEditorWidget()->document()->setModified(true);

    return true;
}

bool IBaseTextDocument::openDocument(const QString &filePath)
{
    QString errorString;
    if ( !textEditorWidget()->editor()->open(&errorString, filePath, filePath) )
        return false;

    if ( m_file )
        delete m_file;
    m_file = new QFile(filePath);

    textEditorWidget()->setFocus();
    return true;
}

bool IBaseTextDocument::saveDocument()
{
    if ( !m_file )
        return false;

    if ( m_fixOnSaveHandler )
        if ( !m_fixOnSaveHandler->fixOnSave(textEditorWidget()->document()) )
            return false;

    QString errorString;
    if ( !textEditorWidget()->file()->save(&errorString, m_file->fileName()) )
        return false;

    textEditorWidget()->setFocus();

    return true;
}

bool IBaseTextDocument::saveAsDocument(const QString &filePath)
{
    QString errorString;
    if ( !textEditorWidget()->file()->save(&errorString, filePath) )
        return false;

    if ( m_file )
        delete m_file;
    m_file = new QFile(filePath);

    if ( m_fixOnSaveHandler )
        if ( !m_fixOnSaveHandler->fixOnSave(textEditorWidget()->document()) )
            return false;

    textEditorWidget()->setFocus();

    return true;
}

bool IBaseTextDocument::saveToDocument(const QString &filePath)
{
    if ( m_fixOnSaveHandler )
        if ( !m_fixOnSaveHandler->fixOnSave(textEditorWidget()->document()) )
            return false;

    QString errorString;
    if ( !textEditorWidget()->file()->save(&errorString, filePath) )
        return false;

    return true;
}

bool IBaseTextDocument::renameTo(const QString &newFilePath)
{
    if ( m_file )
        delete m_file;

    m_file = new QFile(newFilePath);

    return true;
}

QString IBaseTextDocument::title() const
{
    if ( m_file ) {
        QFileInfo fi(*m_file);
        return fi.fileName();
    }

    return m_fileName;
}

QString IBaseTextDocument::toolTip() const
{
    if ( m_file ) {
        QFileInfo fi(*m_file);
        return fi.absoluteFilePath();
    }

    return QStringLiteral("");
}

QString IBaseTextDocument::filePath() const
{ return toolTip(); }

bool IBaseTextDocument::isModified() const
{ return textEditorWidget()->document()->isModified(); }

void IBaseTextDocument::setModified(bool onoff)
{ textEditorWidget()->document()->setModified(onoff); }

bool IBaseTextDocument::isNew() const
{ return (m_file == NULL); }

void IBaseTextDocument::insertText(const QString &text)
{
    if ( text.isEmpty() )
        return;

    textEditorWidget()->insertText(text);
    textEditorWidget()->document()->setModified(true);
}

void IBaseTextDocument::appendText(const QString &text)
{
    if ( text.isEmpty() )
        return;

    textEditorWidget()->appendText(text);
    textEditorWidget()->document()->setModified(true);
}

QString IBaseTextDocument::currentWord() const
{
    QTextCursor cursor = textEditorWidget()->textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    QString key = cursor.selectedText();
    if ( !key.isEmpty() && !key[0].isLetterOrNumber() && key[0] != '_' ) {
        cursor = textEditorWidget()->textCursor();
        cursor.movePosition(QTextCursor::Left);
        cursor.select(QTextCursor::WordUnderCursor);
        key = cursor.selectedText();
        if ( !key.isEmpty() && !key[0].isLetterOrNumber() && key[0] != '_' )
            key = "";
    }
    return key;
}

void IBaseTextDocument::replaceCurrentWord(const QString &text)
{
    QTextCursor cursor = textEditorWidget()->textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    QString key = cursor.selectedText();
    if ( !key.isEmpty() && !key[0].isLetterOrNumber() && key[0] != '_' ) {
        cursor = textEditorWidget()->textCursor();
        cursor.movePosition(QTextCursor::Left);
        cursor.select(QTextCursor::WordUnderCursor);
        key = cursor.selectedText();
        if ( !key.isEmpty() && !key[0].isLetterOrNumber() && key[0] != '_' ) {
            key = "";
            cursor = textEditorWidget()->textCursor();
        }
    }
    textEditorWidget()->setTextCursor(cursor);
    textEditorWidget()->textCursor().insertText(text);
}

int IBaseTextDocument::selectionStart() const
{ return textEditorWidget()->textCursor().selectionStart(); }

int IBaseTextDocument::selectionEnd() const
{ return textEditorWidget()->textCursor().selectionEnd(); }

QString IBaseTextDocument::text() const
{ return textEditorWidget()->toPlainText(); }

QString IBaseTextDocument::selectedText() const
{ return textEditorWidget()->textCursor().selectedText(); }

int IBaseTextDocument::line() const
{ return textEditorWidget()->textCursor().blockNumber(); }

int IBaseTextDocument::column() const
{ return textEditorWidget()->textCursor().columnNumber(); }

int IBaseTextDocument::lineCount() const
{ return textEditorWidget()->blockCount(); }

void IBaseTextDocument::convertPosition(int pos, unsigned *line, unsigned *column) const
{ return textEditorWidget()->convertPosition(pos, line, column); }

void IBaseTextDocument::setCursor(IDocumentCursor *cursor)
{
    if ( TextDocumentCursor *textCursor = static_cast<TextDocumentCursor *>(cursor) )
        textEditorWidget()->gotoLine(textCursor->line() + 1, textCursor->column());
    textEditorWidget()->setFocus();
}

void IBaseTextDocument::refreshDocumentCursor()
{
    textEditorWidget()->refreshDocumentCursor();
}

QString IBaseTextDocument::getText(unsigned startLine, unsigned startPos, unsigned endLine, unsigned endPos) const
{
    QTextDocument *doc = textEditorWidget()->document();
    unsigned startOffset = doc->findBlockByLineNumber(startLine).position() + startPos - 1;
    unsigned endOffset   = doc->findBlockByLineNumber(endLine).position() + endPos - 1;
    QString str = doc->toPlainText().mid(startOffset, endOffset - startOffset);
    return str;
}

void IBaseTextDocument::onFormatSettingsChanged()
{ textEditorWidget()->onSettingsChanged(); }

void IBaseTextDocument::emitDocumentOpened(DocumentManager *documentManager)
{ documentManager->emitEditorOpened(textEditorWidget()->editor()); }

void IBaseTextDocument::emitDocumentClosed(DocumentManager *documentManager)
{ documentManager->emitEditorClosed(textEditorWidget()->editor()); }

void IBaseTextDocument::setTextCodec(QTextCodec *codec)
{ textEditorWidget()->setTextCodec(codec); }

void IBaseTextDocument::setFixOnSaveHandler(IFixOnSaveHandler *fixOnSaveHandler)
{ m_fixOnSaveHandler = fixOnSaveHandler; }

}   // namespace TextEditor
