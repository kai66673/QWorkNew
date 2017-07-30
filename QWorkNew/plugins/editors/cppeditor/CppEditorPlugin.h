#ifndef CPPEDITORPLUGIN_H
#define CPPEDITORPLUGIN_H

#include <QToolBar>

#include "cpptools/cppmodelmanager.h"
#include "cpplocatordata.h"

#include "quickfix.h"
#include "PlainTextDocumentPlugin.h"
#include "cppeditor.h"
#include "IBaseTextDocument.h"

class SettingsCategoryCppEditor : public ISettings
{
public:
    SettingsCategoryCppEditor();

    virtual QString iconName() const { return QStringLiteral(":/cppeditor/images/set_editor.png"); }
    virtual QString title() const { return QObject::tr("Cpp Editor"); }
    virtual ISettingsTabPage *createSettingsTabPage( QWidget *parent = 0 );
    virtual QString groupName() const { return QStringLiteral("Cpp_Editor"); }
    virtual void doApplySettings();
};

class CppDocument : public TextEditor::IBaseTextDocument
{
    Q_OBJECT

public:
    CppDocument( IDocumentPlugin *docPlugin );
    virtual ~CppDocument();

    virtual QWidget *widget() const;
    virtual DocumentToolBar *toolBar();

    virtual IDocumentPlugin *documentPlugin() const { return m_documentPlugin; }

protected:
    virtual TextEditor::TextEditorWidget *textEditorWidget() const;

protected:
    CppEditor::Internal::CPPEditorWidget *m_editor;
    DocumentToolBar *m_toolBar;

private:
    IDocumentPlugin *m_documentPlugin;
    QWidget *m_mainWidget;
};

class CppEditorPlugin : public PlainTextDocumentPlugin, public IDocumentHelper
{
    Q_OBJECT
    Q_INTERFACES(IDocumentPlugin)
#ifdef HAVE_QT5
    Q_PLUGIN_METADATA(IID "QWorkBase.IDocumentPlugin/1.0")
#endif

public:
    CppEditorPlugin();
    virtual ~CppEditorPlugin();

    virtual bool acceptExtension( const QString &extension ) const;
    virtual IDocument *createDocument();
    virtual QString defaultExtension() const { return QStringLiteral("CPP"); }
    virtual QString description() const { return QObject::tr("CPP-Source File"); }

    virtual QStringList settingsKeyList() const;
    virtual ISettings *createSettings( const QString &settingsKey ) const;

    virtual bool documentEventFilter( IDocument *document, QEvent *event );

    virtual QList<QAction *> toolbarActions( const QString &id ) const;

    virtual void initialize();
    virtual QList<HighlighterFormatData *> createHighlightersFormatData() const;

    virtual void updateEditorActions();
    virtual void fillEditMenu(  QMenu *menu );

protected:
    virtual TextEditor::IAssistProvider *createQuickFixProvider();

private slots:
    void findUsages();
    void renameUsages();
    void gotoDefinition();
    void toggleComments();
    void switchSourceHeader();
    void refactor();
    void cppHelper();

private:
    CppTools::Internal::CppModelManager *m_modelManager;

    void createActions();

    Core::Command *m_cmdOutlineSortToggle;
    Core::Command *m_cmdFindUsages;
    Core::Command *m_cmdRenameUsages;
    Core::Command *m_cmdGotoDefinition;

    Core::Command *m_cmdToggleComments;
    Core::Command *m_cmdSwitchSourceHeaderFile;
    Core::Command *m_cmdRefactor;
    Core::Command *m_cmdCppHelper;

    void createFixFactories();
    QList<TextEditor::QuickFixFactory *> m_fixFactories;

    CppTools::CppLocatorData *m_locatorData;
};

#endif // CPPEDITORPLUGIN_H
