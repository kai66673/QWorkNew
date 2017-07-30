#ifndef KARELSNIPPETPROVIDER_H
#define KARELSNIPPETPROVIDER_H

#include "snippets/isnippetprovider.h"

namespace Karel {
namespace Editor {

class KarelSnippetProvider: public TextEditor::ISnippetProvider
{
public:
    KarelSnippetProvider();
    virtual ~KarelSnippetProvider();

public:
    virtual QString groupId() const;
    virtual QString displayName() const;
    virtual void decorateEditor(TextEditor::SnippetEditorWidget *editor) const;
};

}   // namespace Editor
}   // namespace Karel

#endif // KARELSNIPPETPROVIDER_H
