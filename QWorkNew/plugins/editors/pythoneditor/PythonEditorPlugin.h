#ifndef PYTHONEDITORPLUGIN_H
#define PYTHONEDITORPLUGIN_H

#include "PlainTextDocumentPlugin.h"
#include "IBaseTextDocument.h"

namespace PythonEditor {
class EditorWidget;
}

class PythonDocument : public TextEditor::IBaseTextDocument
{
    Q_OBJECT

public:
    PythonDocument( IDocumentPlugin *docPlugin );
    virtual ~PythonDocument();

    virtual QWidget *widget() const;

    virtual IDocumentPlugin *documentPlugin() const { return m_documentPlugin; }

protected:
    virtual TextEditor::TextEditorWidget *textEditorWidget() const;

protected:
    PythonEditor::EditorWidget *m_editor;

private:
    IDocumentPlugin *m_documentPlugin;
    QWidget *m_mainWidget;
};

class PythonEditorPlugin : public PlainTextDocumentPlugin, public IDocumentHelper
{
    Q_OBJECT
    Q_INTERFACES(IDocumentPlugin)
#ifdef HAVE_QT5
    Q_PLUGIN_METADATA(IID "QWorkBase.IDocumentPlugin/1.0")
#endif

public:
    PythonEditorPlugin();
    virtual ~PythonEditorPlugin();

    virtual bool acceptExtension( const QString &extension ) const;
    virtual IDocument *createDocument();
    virtual QString defaultExtension() const { return QStringLiteral("py"); }
    virtual QString description() const { return QObject::tr("Python-Script File"); }

    virtual QStringList settingsKeyList() const;
    virtual ISettings *createSettings( const QString &settingsKey ) const;

    virtual bool documentEventFilter( IDocument *document, QEvent *event );

    virtual void initialize();

    virtual void updateEditorActions();
    virtual void fillEditMenu(  QMenu *menu );

    virtual QList<HighlighterFormatData *> createHighlightersFormatData() const;

private slots:
    void toggleComments();

private:
    Core::Command *m_cmdToggleComments;
};

#endif // PYTHONEDITORPLUGIN_H
