#ifndef GRAMMAREDITORPLUGIN_H
#define GRAMMAREDITORPLUGIN_H

#include <QtPlugin>
#include <QToolBar>

#include "PlainTextDocumentPlugin.h"
#include "IPlugin.h"
#include "GrammarEditorWidget.h"
#include "GrammarRunManager.h"
#include "IBaseTextDocument.h"

class SettingsCategoryGrammarEditor : public ISettings
{
public:
    SettingsCategoryGrammarEditor();

    virtual QString iconName() const { return QStringLiteral(":/grammareditor/images/set_editor.png"); }
    virtual QString title() const { return QObject::tr("Grammar Editor"); }
    virtual ISettingsTabPage *createSettingsTabPage( QWidget *parent = 0 );
    virtual QString groupName() const { return QStringLiteral("Grammar_Editor"); }
    virtual void doApplySettings();
};

class GrammarDocument: public TextEditor::IBaseTextDocument
{
    Q_OBJECT

public:
    GrammarDocument( IDocumentPlugin *docPlugin );
    virtual ~GrammarDocument();

    virtual QWidget *widget() const;
    virtual DocumentToolBar *toolBar();

    bool saveDocument();
    bool saveAsDocument(const QString &filePath);

    virtual IDocumentPlugin *documentPlugin() const { return m_documentPlugin; }

protected:
    virtual TextEditor::TextEditorWidget *textEditorWidget() const;

protected:
    Grammar::EditorWidget *m_editor;
    DocumentToolBar *m_toolBar;

private:
    IDocumentPlugin *m_documentPlugin;
    QWidget *m_mainWidget;
};

class GrammarEditorPlugin : public PlainTextDocumentPlugin, public IDocumentHelper, public ICloseApplicationClient
{
    Q_OBJECT
    Q_INTERFACES(IDocumentPlugin)
#ifdef HAVE_QT5
    Q_PLUGIN_METADATA(IID "QWorkBase.IDocumentPlugin/1.0")
#endif

public:
    GrammarEditorPlugin();
    virtual ~GrammarEditorPlugin();

    virtual bool acceptExtension( const QString &extension ) const;
    virtual IDocument *createDocument();
    virtual QString defaultExtension() const { return QStringLiteral("yy"); }
    virtual QString description() const { return QObject::tr("Yacc(Bison)-grammar File"); }

    virtual QStringList settingsKeyList() const;
    virtual ISettings *createSettings( const QString &settingsKey ) const;

    virtual bool documentEventFilter( IDocument *document, QEvent *event );

    virtual QList<QAction *> toolbarActions( const QString &id ) const;

    virtual void initialize();

    virtual void updateEditorActions();
    virtual void fillEditMenu(  QMenu *menu );

    virtual QList<UiProviderInterface *> createUiProviders( IMainWindow *mainWindow );

    virtual bool applicationAboutToClose();
    virtual void applicationClose();

    virtual QList<HighlighterFormatData *> createHighlightersFormatData() const;

public slots:
    void onBuildActionStarted();
    void onBuildActionFifnished();
    void onGrammarRun();

private slots:
    void toggleComments();
    void findUsages();
    void renameUsages();
    void gotoDefinition();
    void onCancelGrammarRun();
    void toggleHideWarnings( bool onoff );

private:
    Core::Command *m_cmdFindUsages;
    Core::Command *m_cmdRenameUsages;
    Core::Command *m_cmdGotoDefinition;
    Core::Command *m_cmdToggleComments;

    Core::Command *m_cmdRunGrammar;
    Core::Command *m_cmdCancelBuild;
    Core::Command *m_cmdToggleWarnings;

    QwCustomDockWidget *m_buildDock;
    GrammarRunManager *m_runManager;
};

#endif // GRAMMAREDITORPLUGIN_H
