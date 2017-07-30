#include "CmakeEditor.h"
#include "CmakeEditorWidget.h"
#include "cmakeeditorconstants.h"
#include "texteditorconstants.h"

namespace Cmake {

CmakeEditor::CmakeEditor(CmakeEditorWidget *editorWidget)
    : TextEditor::PlainTextEditor(editorWidget)
{
    setContext(Core::Context(Constants::C_CMAKEEDITOR_ID,
                             TextEditor::Constants::C_TEXTEDITOR));
}

CmakeEditor::~CmakeEditor()
{ }

QString CmakeEditor::id() const
{ return Constants::C_CMAKEEDITOR_ID; }

}   // namespace CmakeEditor
