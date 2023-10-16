#pragma once

#include "texteditor_global.h"
#include "completionassistprovider.h"
#include "assistenums.h"
#include "iassistinterface.h"


namespace TextEditor {

enum PositionOperation {
    Current = 1,
    EndOfLine = 2,
    StartOfLine = 3,
    Anchor = 4,
    EndOfDoc = 5
};

class TEXTEDITOR_EXPORT BaseTextEditor
{
public:
    virtual QWidget *editorWidget() = 0;
    virtual TextEditor::CompletionAssistProvider *completionProvider() = 0;
    virtual IAssistInterface *createAssistInterface(AssistKind assistKind,
                                                    AssistReason assistReason) const = 0;
    virtual bool hasBlockSelection() const = 0;
    virtual int position(PositionOperation posOp = Current, int at = -1) const = 0;
    virtual QRect cursorRect(int pos) const = 0;
    virtual QString textAt(int pos, int length) const = 0;
    virtual void replace(int length, const QString &string) = 0;
    virtual void setCursorPosition(int pos) = 0;

};

}   // namespace TextEditor

