#ifndef CMAKEEDITORPLUGIN_H
#define CMAKEEDITORPLUGIN_H

#include "PlainTextDocumentPlugin.h"

#include "cmakeeditor_global.h"

namespace Cmake {
class CmakeEditorWidget;
}

class CMAKEEDITORSHARED_EXPORT CmakeDocument: public TextEditor::IBaseTextDocument
{
    Q_OBJECT

public:
    CmakeDocument(IDocumentPlugin *docPlugin);
    virtual ~CmakeDocument();

    virtual QWidget *widget() const;

    virtual IDocumentPlugin *documentPlugin() const { return m_documentPlugin; }

protected:
    virtual TextEditor::TextEditorWidget *textEditorWidget() const;

protected:
    Cmake::CmakeEditorWidget *m_editor;

private:
    IDocumentPlugin *m_documentPlugin;
};

class CMAKEEDITORSHARED_EXPORT CmakeEditorPlugin: public PlainTextDocumentPlugin, public IDocumentHelper
{
    Q_OBJECT
    Q_INTERFACES(IDocumentPlugin)
#ifdef HAVE_QT5
    Q_PLUGIN_METADATA(IID "QWorkBase.IDocumentPlugin/1.0")
#endif

public:
    CmakeEditorPlugin();
    virtual ~CmakeEditorPlugin();

    virtual bool acceptExtension(const QString &extension) const;
    virtual bool acceptFileName(const QString &fileName) const;
    virtual IDocument *createDocument();
    virtual QString defaultExtension() const { return QStringLiteral("cmake"); }
    virtual QString description() const { return QObject::tr("cmake Project File"); }

    virtual QStringList settingsKeyList() const;
    virtual ISettings *createSettings(const QString &) const;

    virtual bool documentEventFilter(IDocument *document, QEvent *event);

    virtual bool isAvailableInNewDocumentDialog() const { return false; }

    virtual void initialize();
    virtual QList<HighlighterFormatData *> createHighlightersFormatData() const;
    virtual void fillEditMenu(QMenu *menu);

private slots:
    void toggleComments();

private:
    Core::Command *m_cmdToggleComments;
};

#endif // CMAKEEDITORPLUGIN_H
