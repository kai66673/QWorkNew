#include <QCoreApplication>

#include "PythonSnippetProvider.h"
#include "pythoneditorconstants.h"
#include "tools/pythonhighlighter.h"
#include "tools/pythonindenter.h"

#include <snippets/snippeteditor.h>

namespace PythonEditor {

PythonSnippetProvider::PythonSnippetProvider()
{ }

PythonSnippetProvider::~PythonSnippetProvider()
{ }

QString PythonSnippetProvider::groupId() const
{
    return QLatin1String(Constants::PYTHON_SNIPPETS_GROUP_ID);
}

QString PythonSnippetProvider::displayName() const
{
    return QCoreApplication::translate("PythonEditor::PythonSnippetProvider", "Python");
}

void PythonSnippetProvider::decorateEditor(TextEditor::SnippetEditorWidget *editor) const
{
    PythonHighlighter *highlighter = new PythonHighlighter();
    editor->setSyntaxHighlighter(highlighter);
    editor->setIndenter(new PythonIndenter());
    editor->onSettingsChanged();
    highlighter->onFormatSettingsChanged();
}

}   // namespace PythonEditor
