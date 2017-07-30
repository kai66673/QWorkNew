#include "KarelHoverHandler.h"
#include "KarelEditor.h"

namespace Karel {
namespace Editor {

KarelHoverHandler::KarelHoverHandler(QObject *parent)
    : TextEditor::BaseHoverHandler(parent)
{ }

KarelHoverHandler::~KarelHoverHandler()
{ }

bool KarelHoverHandler::acceptEditor(Core::IEditor *editor)
{ return qobject_cast<KarelEditor *>(editor); }

void KarelHoverHandler::identifyMatch(TextEditor::ITextEditor *editor, int pos)
{
    if ( KarelEditorWidget *karelEditor = qobject_cast<KarelEditorWidget *>(editor->widget()) ) {
        QString extraToolTip = karelEditor->extraSelectionTooltip(pos);
        if ( !extraToolTip.isEmpty() )
            setToolTip(extraToolTip);
    }
}

void KarelHoverHandler::decorateToolTip()
{ }

}   // namespace Editor
}   // namespace Karel
