#ifndef PLAINTEXTDOCUMENTPLUGIN_H
#define PLAINTEXTDOCUMENTPLUGIN_H

#include <QObject>
#include <QFile>

#include "IDocument.h"
#include "ITextDocument.h"
#include "PlainTextEditor.h"
#include "IBaseTextDocument.h"
#include "texteditor_global.h"

class PlainTextDocumentPlugin;

class SettingsCategoryTextEditor : public ISettings
{
public:
    SettingsCategoryTextEditor();

    virtual QString iconName() const { return QStringLiteral(":/images/set_editor.png"); }
    virtual QString title() const { return QObject::tr("Text Editor"); }
    virtual ISettingsTabPage *createSettingsTabPage( QWidget *parent = 0 );
    virtual QString groupName() const { return QStringLiteral("Text_Editor"); }
    virtual void doApplySettings();
};

class TEXTEDITOR_EXPORT PlainTextDocument: public TextEditor::IBaseTextDocument
{
    Q_OBJECT

public:
    enum ChangeTrigger {
        TriggerInternal,
        TriggerExternal
    };

    enum ChangeType {
        TypeContents,
        TypePermissions,
        TypeRemoved
    };

    enum ReloadBehavior {
        BehaviorAsk,
        BehaviorSilent
    };

    enum ReloadFlag {
        FlagReload,
        FlagIgnore
    };

    PlainTextDocument( IDocumentPlugin *docPlugin );
    virtual ~PlainTextDocument();

    virtual QWidget *widget() const;

    virtual IDocumentPlugin *documentPlugin() const { return m_documentPlugin; }

protected:
    virtual TextEditor::TextEditorWidget *textEditorWidget() const;

protected:
    TextEditor::PlainTextEditorWidget *m_editor;

private:
    IDocumentPlugin *m_documentPlugin;
};

class PlainTextEditCommands: public QObject
{
public:
    PlainTextEditCommands( QObject *parent = 0 );

    void updateEditorActions();
    void fillEditMenu( QMenu *menu ) const;
    void connectActions( PlainTextDocumentPlugin *plugin );

private:
    Core::Command *m_cmdUndo;
    Core::Command *m_cmdRedo;

    Core::Command *m_cmdCopy;
    Core::Command *m_cmdPaste;
    Core::Command *m_cmdCut;
    Core::Command *m_cmdSelectAll;

    Core::Command *m_cmdLowerCase;
    Core::Command *m_cmdUpperCase;

    Core::Command *m_cmdFontSizeIncrease;
    Core::Command *m_cmdFontSizeDecrease;
    Core::Command *m_cmdFontSizeReset;

    Core::Command *m_cmdFold;
    Core::Command *m_cmdUnfold;
    Core::Command *m_cmdToggleFoldAll;

    Core::Command *m_cmdDuplicateSelectedLines;
    Core::Command *m_cmdMoveUpSelectedLines;
    Core::Command *m_cmdMoveDownSelectedLines;
};

class TEXTEDITOR_EXPORT PlainTextDocumentPlugin : public IDocumentPlugin
{
    Q_OBJECT

public:
    PlainTextDocumentPlugin();
    virtual ~PlainTextDocumentPlugin();

    virtual bool acceptExtension( const QString &extension ) const { Q_UNUSED(extension); return true; }
    virtual IDocument *createDocument();
    virtual QString defaultExtension() const { return QStringLiteral(""); }
    virtual QString description() const { return QObject::tr("Plain Text File"); }

    virtual QStringList settingsKeyList() const;
    virtual ISettings *createSettings( const QString &settingsKey ) const;

    virtual void updateEditorActions();
    virtual void fillEditMenu( QMenu *menu );

    virtual void initialize();
    virtual bool activateDocumentOnMouseEnterEvent() const;

    virtual QList<HighlighterFormatData *> createHighlightersFormatData() const;

public slots:
    void undo();
    void redo();
    void copy();
    void paste();
    void cut();
    void selectAll();
    void lowerCase();
    void upperCase();
    void fontSizeIncrease();
    void fontSizeDecrease();
    void fontSizeReset();
    void fold();
    void unfold();
    void toggleFoldAll();
    void duplicateSelectedLines();
    void moveSelectedLinesUp();
    void moveSelectedLinesDown();

protected:
    void updateEditorCommands();
    virtual TextEditor::IAssistProvider *createQuickFixProvider();
    static PlainTextEditCommands *m_commands;
};

#endif // PLAINTEXTDOCUMENTPLUGIN_H
