#ifndef PROFILEEDITORWIDGET_H
#define PROFILEEDITORWIDGET_H

#include <QObject>

#include "texteditor.h"
#include "uncommentselection.h"
#include "fontsettings.h"
#include "profilehighlighter.h"

#include "profileeditor_global.h"

namespace ProEditor {

class PROFILEEDITORSHARED_EXPORT ProfileEditorWidget: public TextEditor::TextEditorWidget
{
    Q_OBJECT

public:
    ProfileEditorWidget(IDocument *document, QWidget *parent = 0);
    virtual ~ProfileEditorWidget();

    virtual void unCommentSelection();
    virtual void onSettingsChanged();

protected:
    TextEditor::BaseTextEditor *createEditor();
    virtual Core::ILink *findLinkAt(const QTextCursor &cursor, bool resolveTarget = true);

private:
    Utils::CommentDefinition m_commentDefinition;
    ProfileHighlighter *m_highlighter;
};

}   // namespace Profile

#endif // PROFILEEDITORWIDGET_H
