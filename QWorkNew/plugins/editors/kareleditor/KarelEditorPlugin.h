#ifndef KARELEDITORPLUGIN_H
#define KARELEDITORPLUGIN_H

#include "PlainTextDocumentPlugin.h"
#include "KarelDocumentCaseFixHandler.h"

namespace Karel {
namespace Editor {
class KarelEditorWidget;
}
}

class SettingsCategoryKarelEditor : public ISettings
{
public:
    SettingsCategoryKarelEditor();

    virtual QString iconName() const { return QStringLiteral(":/kareleditor/images/set_karel.png"); }
    virtual QString title() const { return QObject::tr("Karel Editor"); }
    virtual ISettingsTabPage *createSettingsTabPage( QWidget *parent = 0 );
    virtual QString groupName() const { return QStringLiteral("Karel_Editor"); }
    virtual void doApplySettings();
};

class KarelDocument: public TextEditor::IBaseTextDocument
{
    Q_OBJECT

public:
    KarelDocument(IDocumentPlugin *docPlugin);
    virtual ~KarelDocument();

    virtual QWidget *widget() const;
    virtual DocumentToolBar *toolBar();

    virtual IDocumentPlugin *documentPlugin() const { return m_documentPlugin; }

protected:
    virtual TextEditor::TextEditorWidget *textEditorWidget() const;

protected:
    Karel::Editor::KarelEditorWidget *m_editor;
    DocumentToolBar *m_toolBar;

private:
    IDocumentPlugin *m_documentPlugin;
    QWidget *m_mainWidget;
};

class KarelEditorPlugin: public PlainTextDocumentPlugin, public IDocumentHelper
{
    Q_OBJECT
    Q_INTERFACES(IDocumentPlugin)
#ifdef HAVE_QT5
    Q_PLUGIN_METADATA(IID "QWorkBase.IDocumentPlugin/1.0")
#endif

public:
    KarelEditorPlugin();
    virtual ~KarelEditorPlugin();

    virtual bool acceptExtension( const QString &extension ) const;
    virtual IDocument *createDocument();
    virtual QString defaultExtension() const { return QStringLiteral("kl"); }
    virtual QString description() const { return QObject::tr("KAREL Source File"); }

    virtual QStringList settingsKeyList() const;
    virtual ISettings *createSettings( const QString &settingsKey ) const;

    // IDocumentHelper implementation
    virtual bool documentEventFilter( IDocument *document, QEvent *event );

    virtual QList<QAction *> toolbarActions( const QString &id ) const;

    virtual void initialize();
    virtual QList<HighlighterFormatData *> createHighlightersFormatData() const;

    virtual void updateEditorActions();
    virtual void fillEditMenu( QMenu *menu );

private slots:
    void toggleComments();
    void gotoDefinition();
    void findUsages();
    void replaceUsages();

private:
    Karel::KarelDocumentCaseFixHandler m_caseFixer;

    void createActions();

    Core::Command *m_cmdToggleComments;
    Core::Command *m_cmdGotoDefinition;
    Core::Command *m_cmdFindUsages;
    Core::Command *m_cmdRenameUsages;
};

#endif // KARELEDITORPLUGIN_H
