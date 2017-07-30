#ifndef GRAMMARHIGHLIGHTER_H
#define GRAMMARHIGHLIGHTER_H

#include <syntaxhighlighter.h>

namespace Grammar {

class GrammarHighlighter : public TextEditor::SyntaxHighlighter
{
    Q_OBJECT

public:
    enum CppFormats {
        CppNumberFormat = 0,
        CppStringFormat,
        CppTypeFormat,
        CppKeywordFormat,
        CppOperatorFormat,
        CppPreprocessorFormat,
        CppLabelFormat,
        CppCommentFormat,
        CppDoxygenCommentFormat,
        CppDoxygenTagFormat,
        CppVisualWhitespace,
        SqlKeywordFormat,
        NumCppFormats
    };

    enum GrammarFormats {
        GrammarKeyword = 0,
        GrammarRuleDefinition,
        GrammarRuleItem,
        GrammarVariable,
        GrammarFormatsCount
    };

    explicit GrammarHighlighter( QTextDocument *document = 0 );

    // Set formats from a sequence of type QTextCharFormat
    template <class InputIterator>
    void setFormats(InputIterator begin, InputIterator end) {
        qCopy(begin, end, m_cppFormats);
    }

    QTextCharFormat labelFormat() const { return m_cppFormats[CppLabelFormat]; }

    virtual void highlightBlock(const QString &text);
    void onFormatSettingsChanged( bool forceRehilight = true );

private:
    void highlightWord(QStringRef word, int position, int length);
    void highlightLine(const QString &text, int position, int length,
                       const QTextCharFormat &format);

    void highlightDoxygenComment(const QString &text, int position,
                                 int length);

    bool isPPKeyword(const QStringRef &text) const;

    QTextCharFormat m_cppFormats[NumCppFormats];
    QTextCharFormat m_grammarFormats[GrammarFormatsCount];
};

}   // namespace Grammar

#endif // GRAMMARHIGHLIGHTER_H
