#ifndef KARELAUTOCOMPLETER_H
#define KARELAUTOCOMPLETER_H

#include <autocompleter.h>

namespace Karel {

class AutoCompleter : public TextEditor::AutoCompleter
{
public:
    AutoCompleter();
    virtual ~AutoCompleter();

    virtual bool contextAllowsAutoParentheses(const QTextCursor &cursor,
                                              const QString &textToInsert = QString()) const;
    virtual bool isInComment(const QTextCursor &cursor) const;
    virtual QString insertMatchingBrace(const QTextCursor &cursor,
                                        const QString &text,
                                        QChar la,
                                        int *skippedChars) const;

private:
    bool isInCommentStrict(const QTextCursor &cursor) const;
};

}   // namespace Karel

#endif // KARELAUTOCOMPLETER_H
