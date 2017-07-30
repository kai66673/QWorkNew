#ifndef KARELHIGHLIGHTER_H
#define KARELHIGHLIGHTER_H

#include <syntaxhighlighter.h>

namespace Karel {
namespace Editor {

enum KarelFormats {
    KarelKeyword1Format,
    KarelKeyword2Format,
    KarelKeyword3Format,
    KarelKeyword4Format,
    KarelKeyword5Format,
    KarelNumberFormar,
    KarelStringFormar,
    KarelOperatorFormat,
    KarelCommentFormat,
    KarelVisualWhitespace,
    NumKarelFormats
};

class KarelHighlighter : public TextEditor::SyntaxHighlighter
{
    Q_OBJECT

public:
    KarelHighlighter(QTextDocument *document = 0);
    virtual ~KarelHighlighter();

    virtual void highlightBlock( const QString &text );

    void onFormatSettingsChanged( bool forceRehilight = true );

private:
    void highlightLine(const QString &text, int position, int length, const QTextCharFormat &format );

    QTextCharFormat m_formats[NumKarelFormats];
};

}   // namespace Editor
}   // namespace Karel

#endif // KARELHIGHLIGHTER_H
