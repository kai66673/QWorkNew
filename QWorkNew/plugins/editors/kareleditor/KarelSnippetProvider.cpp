#include <QCoreApplication>

#include "KarelSnippetProvider.h"
#include "karel_constants.h"

#include "KarelHighlighter.h"
#include "KarelIndenter.h"
#include "KarelAutoCompleter.h"

#include <snippets/snippeteditor.h>

namespace Karel {
namespace Editor {

KarelSnippetProvider::KarelSnippetProvider()
{ }

KarelSnippetProvider::~KarelSnippetProvider()
{ }

QString KarelSnippetProvider::groupId() const
{
    return QLatin1String(Constants::KAREL_SNIPPETS_GROUP_ID);
}

QString KarelSnippetProvider::displayName() const
{
    return QCoreApplication::translate("KarelEditor::KarelSnippetProvider", "Karel");
}

void KarelSnippetProvider::decorateEditor(TextEditor::SnippetEditorWidget *editor) const
{
    KarelHighlighter *highlighter = new KarelHighlighter();
    editor->setSyntaxHighlighter(highlighter);
    editor->setIndenter(new Indenter);
    editor->setAutoCompleter(new AutoCompleter);
    editor->onSettingsChanged();;
    highlighter->onFormatSettingsChanged();
}

}   // namespace Editor
}   // namespace Karel
