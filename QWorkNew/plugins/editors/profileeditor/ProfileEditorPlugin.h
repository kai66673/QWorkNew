#ifndef PROFILEEDITORPLUGIN_H
#define PROFILEEDITORPLUGIN_H

#include "PlainTextDocumentPlugin.h"

#include "profileeditor_global.h"

namespace ProEditor {
class ProfileEditorWidget;
}

class PROFILEEDITORSHARED_EXPORT ProFileDocument: public TextEditor::IBaseTextDocument
{
    Q_OBJECT

public:
    ProFileDocument(IDocumentPlugin *docPlugin);
    virtual ~ProFileDocument();

    virtual QWidget *widget() const;

    virtual bool saveDocument();
    virtual bool saveAsDocument(const QString &filePath);
    virtual bool saveToDocument(const QString &filePath);

    virtual IDocumentPlugin *documentPlugin() const { return m_documentPlugin; }

protected:
    virtual TextEditor::TextEditorWidget *textEditorWidget() const;

protected:
    ProEditor::ProfileEditorWidget *m_editor;

private:
    IDocumentPlugin *m_documentPlugin;
};

class PROFILEEDITORSHARED_EXPORT ProfileEditorPlugin: public PlainTextDocumentPlugin, public IDocumentHelper
{
    Q_OBJECT
    Q_INTERFACES(IDocumentPlugin)
#ifdef HAVE_QT5
    Q_PLUGIN_METADATA(IID "QWorkBase.IDocumentPlugin/1.0")
#endif

public:
    ProfileEditorPlugin();
    virtual ~ProfileEditorPlugin();

    virtual bool acceptExtension(const QString &extension) const;
    virtual IDocument *createDocument();
    virtual QString defaultExtension() const { return QStringLiteral("pro"); }
    virtual QString description() const { return QObject::tr("qmake Project File"); }

    virtual QStringList settingsKeyList() const;
    virtual ISettings *createSettings(const QString &settingsKey) const;

    virtual bool isAvailableInNewDocumentDialog() const { return false; }

    virtual bool documentEventFilter(IDocument *document, QEvent *event);

    virtual void initialize();
    virtual QList<HighlighterFormatData *> createHighlightersFormatData() const;
    virtual void fillEditMenu(QMenu *menu);

private slots:
    void toggleComments();

private:
    Core::Command *m_cmdToggleComments;
};

#endif // PROFILEEDITORPLUGIN_H
