#ifndef ITEXTDOCUMENT_H
#define ITEXTDOCUMENT_H

#include <QString>

#include "corelib_global.h"
#include "IDocument.h"

class CORELIB_EXPORT TextDocumentCursor : public IDocumentCursor
{
public:
    TextDocumentCursor( int line = -1, int column = 0 )
        : m_line(line)
        , m_column(column)
    {}

    inline int line() const { return m_line; }
    inline int column() const { return m_column; }

private:
    int m_line;
    int m_column;
};

class CORELIB_EXPORT ITextDocument
{
public:
    virtual void insertText( const QString &text ) = 0;
    virtual void appendText( const QString &text ) = 0;
    virtual QString currentWord() const = 0;
    virtual void replaceCurrentWord( const QString &text ) = 0;
    virtual int selectionStart() const = 0;
    virtual int selectionEnd() const = 0;
    virtual QString text() const = 0;
    virtual QString selectedText() const = 0;
    virtual int line() const = 0;
    virtual int column() const = 0;
    virtual int lineCount() const = 0;

    virtual void convertPosition( int pos, unsigned *line, unsigned *column ) const = 0;
    virtual QString getText( unsigned startLine, unsigned startPos, unsigned endLine, unsigned endPos ) const = 0;

    int selectedLength() const { return selectionEnd() - selectionStart(); }
};

#endif // ITEXTDOCUMENT_H
