#ifndef IBASETEXTDOCUMENT_H
#define IBASETEXTDOCUMENT_H

#include "ITextDocument.h"
#include "IFixOnSaveHandler.h"
#include "texteditor.h"

namespace TextEditor {

class TEXTEDITOR_EXPORT IBaseTextDocument: public IDocument, public ITextDocument
{
public:
    IBaseTextDocument();

    virtual ~IBaseTextDocument();

    virtual QWidget *editorWidget() const;
    virtual Core::IEditor *editor() const;

    virtual bool newDocument (const QString &fileName);
    virtual bool newDocumentFromFile(const QString &fileName, const QString &contentFilePath);
    virtual bool openDocument(const QString &filePath);
    virtual bool saveDocument();
    virtual bool saveAsDocument(const QString &filePath);
    virtual bool saveToDocument(const QString &filePath);
    virtual bool renameTo(const QString &newFilePath);

    virtual QString title() const;
    virtual QString toolTip() const;
    virtual QString filePath() const;

    virtual bool isModified() const;
    virtual void setModified(bool onoff);
    virtual bool isNew() const;

    virtual void insertText(const QString &text);
    virtual void appendText(const QString &text);
    virtual QString currentWord() const;
    virtual void replaceCurrentWord(const QString &text);
    virtual int selectionStart() const;
    virtual int selectionEnd() const;
    virtual QString text() const;
    virtual QString selectedText() const;
    virtual int line() const;
    virtual int column() const;
    virtual int lineCount() const;

    virtual void convertPosition(int pos, unsigned *line, unsigned *column) const;
    virtual void setCursor(IDocumentCursor *cursor);
    virtual void refreshDocumentCursor();

    virtual QString getText(unsigned startLine, unsigned startPos, unsigned endLine, unsigned endPos) const;

    virtual void onFormatSettingsChanged();
    virtual void emitDocumentOpened(DocumentManager *documentManager);
    virtual void emitDocumentClosed(DocumentManager *documentManager);

    virtual void setTextCodec(QTextCodec *codec);

    void setFixOnSaveHandler(IFixOnSaveHandler *fixOnSaveHandler);

protected:
    virtual TextEditor::TextEditorWidget *textEditorWidget() const = 0;

    QFile       *m_file;
    QString      m_fileName;
    IFixOnSaveHandler *m_fixOnSaveHandler;
};

}   // namespace TextEditor

#endif // IBASETEXTDOCUMENT_H
