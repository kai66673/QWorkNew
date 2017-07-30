#include "SqlTextEditorWidget.h"
#include "texteditorconstants.h"
#include "basetextdocument.h"
#include "SqlHighlighter.h"

SqlTextEditor::SqlTextEditor( SqlTextEditorWidget *editorWidget )
    : TextEditor::BaseTextEditor(editorWidget)
    , m_context(TextEditor::Constants::C_TEXTEDITOR)
{}

SqlTextEditor::~SqlTextEditor()
{}

QString SqlTextEditor::id() const
{
    return QStringLiteral("TextEditor.Connmanager.TableInfo");
}

SqlTextEditorWidget::SqlTextEditorWidget(IDocument *document, QWidget *parent)
    : TextEditor::TextEditorWidget(document, parent)
{
    setReadOnly(true);

    Sql::Editor::SqlHighlighter *sqlHighlighter = new Sql::Editor::SqlHighlighter;
    sqlHighlighter->onFormatSettingsChanged(false);
    baseTextDocument()->setSyntaxHighlighter(sqlHighlighter);

    setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    setParenthesesMatchingEnabled(true);
}

void SqlTextEditorWidget::contextMenuEvent( QContextMenuEvent * event )
{
    QPlainTextEdit::contextMenuEvent(event);
}
