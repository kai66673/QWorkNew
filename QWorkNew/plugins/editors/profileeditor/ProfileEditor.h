#ifndef PROFILEEDITOR_H
#define PROFILEEDITOR_H

#include "texteditor.h"

#include "profileeditor_global.h"

namespace ProEditor {

class ProfileEditorWidget;

class PROFILEEDITORSHARED_EXPORT ProfileEditor: public TextEditor::BaseTextEditor
{
    Q_OBJECT

public:
    explicit ProfileEditor(ProfileEditorWidget *editorWidget);
    virtual ~ProfileEditor();

    bool duplicateSupported() const { return false; }
    Core::IEditor *duplicate(QWidget *);

    QString id() const;
    bool isTemporary() const { return false; }

    /**
      Opens file for editing, actual work performed by base class
      */
    bool open(QString *errorString,
              const QString &fileName,
              const QString &realFileName);
};

}   // namespace Profile

#endif // PROFILEEDITOR_H
