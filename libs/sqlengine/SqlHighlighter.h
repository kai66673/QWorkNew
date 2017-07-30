#ifndef SQLHIGHLIGHTER_H
#define SQLHIGHLIGHTER_H

#include "sqlengine_global.h"

#include <syntaxhighlighter.h>

namespace Sql {

class SqlParserEngine;

enum SqlFormats {
    SqlNumberFormat,
    SqlStringFormat,
    SqlKeyword1Format,
    SqlKeyword2Format,
    SqlKeyword3Format,
    SqlKeyword4Format,
    SqlKeyword5Format,
    SqlKeyword6Format,
    SqlKeyword7Format,
    SqlKeyword8Format,
    SqlOperatorFormat,
    SqlCommentFormat,
    SqlVisualWhitespace,
    NumSqlFormats
};

namespace Editor {

class SQL_EXPORT SqlHighlighter : public TextEditor::SyntaxHighlighter
{
    Q_OBJECT

public:
    SqlHighlighter( QTextDocument *document = 0 );
    virtual ~SqlHighlighter();

    virtual void highlightBlock( const QString &text );

    void onFormatSettingsChanged( bool forceRehilight = true );

private:
    void highlightWord( QStringRef word, int position, int length );
    void highlightLine( const QString &line, int position, int length, const QTextCharFormat &format );

    QTextCharFormat m_formats[Sql::NumSqlFormats];
};

} // Editor
} // Sql

#endif // SQLHIGHLIGHTER_H
