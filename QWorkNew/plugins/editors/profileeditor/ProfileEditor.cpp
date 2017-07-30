#include "ProfileEditor.h"
#include "ProfileEditorWidget.h"
#include "profileeditorconstants.h"
#include "texteditorconstants.h"

namespace ProEditor {

ProfileEditor::ProfileEditor(ProfileEditorWidget *editorWidget)
    : TextEditor::BaseTextEditor(editorWidget)
{
    setContext(Core::Context(Constants::C_PROFILEEDITOR_ID,
                             TextEditor::Constants::C_TEXTEDITOR));
}

ProfileEditor::~ProfileEditor()
{ }

Core::IEditor *ProfileEditor::duplicate(QWidget * /*parent*/)
{ return 0; }

QString ProfileEditor::id() const
{ return QLatin1String(Constants::C_PROFILEEDITOR_ID); }

bool ProfileEditor::open(QString *errorString, const QString &fileName, const QString &realFileName)
{
    bool status = TextEditor::BaseTextEditor::open(errorString,
                                                   fileName,
                                                   realFileName);
    return status;
}

}   // namespace Profile
