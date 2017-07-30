#include "SqlHoverHandler.h"
#include "SQLEditor.h"

namespace Sql {
namespace Editor {

SqlHoverHandler::SqlHoverHandler( QObject *parent )
    : TextEditor::BaseHoverHandler(parent)
{
}

SqlHoverHandler::~SqlHoverHandler()
{
}

bool SqlHoverHandler::acceptEditor( Core::IEditor *editor )
{
    return qobject_cast<SQLEditor *>(editor);
}

void SqlHoverHandler::identifyMatch( TextEditor::ITextEditor *editor, int pos )
{
    if ( SQLEditorWidget *sqlEditor = qobject_cast<SQLEditorWidget *>(editor->widget()) ) {
        QString extraToolTip = sqlEditor->extraSelectionTooltip(pos);
        if ( !extraToolTip.isEmpty() )
            setToolTip(extraToolTip);
    }
}

void SqlHoverHandler::decorateToolTip()
{
}

} // namespace Editor
} // namespace Sql
