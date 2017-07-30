#ifndef SQLTEXTEDITORWIDGET_H
#define SQLTEXTEDITORWIDGET_H

#include <texteditor.h>

class SqlTextEditorWidget;

class SqlTextEditor : public TextEditor::BaseTextEditor
{
    Q_OBJECT

public:
    SqlTextEditor( SqlTextEditorWidget *editorWidget );
    virtual ~SqlTextEditor();

    Core::Context context() const { return m_context; }

    bool duplicateSupported() const { return false; }
    Core::IEditor *duplicate(QWidget * /* parent */ ) { return 0; }
    bool isTemporary() const { return false; }
    virtual QString id() const;

private:
    const Core::Context m_context;
};

class SqlTextEditorWidget : public TextEditor::TextEditorWidget
{
    Q_OBJECT

public:
    explicit SqlTextEditorWidget(IDocument *document, QWidget *parent = 0 );

protected:
    virtual TextEditor::BaseTextEditor *createEditor() { return new SqlTextEditor(this); }

    virtual void contextMenuEvent( QContextMenuEvent *event );
};

#endif // SQLTEXTEDITORWIDGET_H
