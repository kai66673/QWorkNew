#ifndef PYTHONSNIPPETPROVIDER_H
#define PYTHONSNIPPETPROVIDER_H

#include "snippets/isnippetprovider.h"

namespace PythonEditor {

class PythonSnippetProvider : public TextEditor::ISnippetProvider
{
public:
    PythonSnippetProvider();
    virtual ~PythonSnippetProvider();

public:
    virtual QString groupId() const;
    virtual QString displayName() const;
    virtual void decorateEditor(TextEditor::SnippetEditorWidget *editor) const;
};

}   // namespace PythonEditor

#endif // PYTHONSNIPPETPROVIDER_H
