#include <QTextCodec>
#include <QFileInfo>
#include <QTextBlock>
#include <QMenu>

#include "Core.h"
#include "ifile.h"
#include "PlainTextDocumentPlugin.h"
#include "TextEditorSettingsWidget.h"
#include "manager.h"
#include "snippetscollection.h"
#include "plaintextsnippetprovider.h"

static bool s_activateDocOnMouseEnter = false;

////////////////////////////////////////////////////////////////////////////////////////////
/// GenericHighlighterFormatData
class GenericHighlighterFormatData: public HighlighterFormatData
{
public:
    GenericHighlighterFormatData() {
        m_formats.append( new HighlighterFormat( QObject::tr("Whitespace"),
                                                 false,
                                                 false,
                                                 QColor(127,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Keyword"),
                                                 true,
                                                 false,
                                                 QColor(  0,   0, 255),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Data Type"),
                                                 false,
                                                 false,
                                                 QColor( 96,   0,  48),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Decimal"),
                                                 false,
                                                 false,
                                                 QColor( 48,   0,  64),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("BaseN"),
                                                 false,
                                                 false,
                                                 QColor( 48,   0,  64),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Float"),
                                                 false,
                                                 false,
                                                 QColor( 48,   24,  96),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Char"),
                                                 false,
                                                 false,
                                                 QColor(192,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("String"),
                                                 false,
                                                 false,
                                                 QColor(255,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Comment"),
                                                 false,
                                                 false,
                                                 QColor(  0, 255,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Alert"),
                                                 false,
                                                 false,
                                                 QColor(  0,   0, 255),
                                                 QColor(255, 255,   0) ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Error"),
                                                 true,
                                                 false,
                                                 QColor(  0, 255,   0),
                                                 QColor(255,   0,   0) ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Function"),
                                                 true,
                                                 false,
                                                 QColor( 92,  92,  92),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Region Marker"),
                                                 false,
                                                 false,
                                                 QColor(255,  64,   0),
                                                 QColor(  0, 255, 255) ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Others"),
                                                 true,
                                                 false,
                                                 QColor(  0,  92,   0),
                                                 QColor() ) );
    }

    virtual QString languageId() const { return QObject::tr("Generic"); }
    virtual int     prioritet() const { return 90; }
};

////////////////////////////////////////////////////////////////////////////////////////////
/// SettingsCategoryTextEditor
SettingsCategoryTextEditor::SettingsCategoryTextEditor()
{
    /// Font & COlors
    QFont font("Monospace", 9);
    font.setStyleHint(QFont::Courier);

    valuesMap["Font"]                       = font;
    valuesMap["Zoom"]                       = 100;
    valuesMap["Antialias"]                  = true;

    /// Tabs & Indentation
    valuesMap["TabToSpaces"]                = true;
    valuesMap["AutoIndentation"]            = true;
    valuesMap["BackspaceIndentation"]       = true;
    valuesMap["TabSize"]                    = 4;
    valuesMap["IndentSize"]                 = 4;

    /// Behavior
    valuesMap["MouseNavigation"]            = true;
    valuesMap["ScrollWheelZooming"]         = true;
    valuesMap["ActivateDocOnMouseEnter"]    = false;

    /// Text Wrapping
    valuesMap["Wrapping"]                   = false;
    valuesMap["ShowRightMargin"]            = false;
    valuesMap["RightMarginColumn"]          = 120;

    /// Display Settings
    valuesMap["ShowLineNumbers"]            = true;
    valuesMap["ShowFoldingMarkers"]         = true;
    valuesMap["MarkTextChanges"]            = true;
    valuesMap["ShowWhitespaces"]            = true;
    valuesMap["CenterCursorOnScroll"]       = false;
    valuesMap["HighlightCurrentLine"]       = true;
    valuesMap["HighlightBlocks"]            = false;
    valuesMap["AnimateMatchingParentheses"] = true;
}

ISettingsTabPage *SettingsCategoryTextEditor::createSettingsTabPage( QWidget *parent )
{
    return new TextEditorSettingsWidget(valuesMap, parent);
}

void SettingsCategoryTextEditor::doApplySettings()
{
    s_activateDocOnMouseEnter = valuesMap["ActivateDocOnMouseEnter"].toBool();
}


//////////////////////////////////////////////////////////////////////////////////////
/// PlainDocument
PlainTextDocument::PlainTextDocument( IDocumentPlugin *docPlugin )
    : TextEditor::IBaseTextDocument()
    , m_editor(0)
    , m_documentPlugin(docPlugin)
{
    m_editor = new TextEditor::PlainTextEditorWidget(nullptr);
}

PlainTextDocument::~PlainTextDocument()
{ }

QWidget *PlainTextDocument::widget() const
{
    return m_editor;
}

TextEditor::TextEditorWidget *PlainTextDocument::textEditorWidget() const
{ return m_editor; }

//////////////////////////////////////////////////////////////////////////////////////
/// PlainTextEditCommands
PlainTextEditCommands::PlainTextEditCommands( QObject *parent )
    : QObject(parent)
{
    QMainWindow *mainWindow = Core::Storage::mainWindow()->widget();

    m_cmdUndo               = new Core::Command( tr("Undo"),
                                                 QIcon(":/images/txt_undo.png"),
                                                 tr("Undo"),
                                                 tr("Undo Last Action/Modification"),
                                                 QKeySequence(tr("Ctrl+Z")),
                                                 mainWindow );

    m_cmdRedo               = new Core::Command( tr("Redo"),
                                                 QIcon(":/images/txt_redo.png"),
                                                 tr("Redo"),
                                                 tr("Redo Last Action/Modification"),
                                                 QKeySequence(tr("Ctrl+Y")),
                                                 mainWindow );

    m_cmdCopy               = new Core::Command( tr("Copy"),
                                                 QIcon(":/images/txt_copy.png"),
                                                 tr("&Copy"),
                                                 tr("Copy Selected Text Into Clipboard"),
                                                 QKeySequence(QKeySequence::Copy),
                                                 mainWindow );

    m_cmdPaste              = new Core::Command( tr("Paste"),
                                                 QIcon(":/images/txt_paste.png"),
                                                 tr("&Paste"),
                                                 tr("Paste Text From Clipboard"),
                                                 QKeySequence(QKeySequence::Paste),
                                                 mainWindow );

    m_cmdCut                = new Core::Command( tr("Cut"),
                                                 QIcon(":/images/txt_cut.png"),
                                                 tr("C&ut"),
                                                 tr("Cut Selected Text Into Clipboard"),
                                                 QKeySequence(QKeySequence::Cut),
                                                 mainWindow );

    m_cmdSelectAll          = new Core::Command( tr("SelectAll"),
                                                 QIcon(":/images/txt_select_all.png"),
                                                 tr("Select &All"),
                                                 tr("Select All Editor Contents"),
                                                 QKeySequence(QKeySequence::SelectAll),
                                                 mainWindow );

    m_cmdLowerCase          = new Core::Command( tr("LowerCase"),
                                                 QIcon(":/images/txt_lowercase.png"),
                                                 tr("&lower Case"),
                                                 tr("Switch Selection to Lower Case"),
                                                 QKeySequence(tr("Ctrl+Alt+[")),
                                                 mainWindow );

    m_cmdUpperCase          = new Core::Command( tr("UpperCase"),
                                                 QIcon(":/images/txt_uppercase.png"),
                                                 tr("&UPPER Case"),
                                                 tr("Switch Selection to Upper Case"),
                                                 QKeySequence(tr("Ctrl+Alt+]")),
                                                 mainWindow );

    m_cmdFontSizeIncrease   = new Core::Command( tr("IncreaseFontSize"),
                                                 tr("&Increase"),
                                                 tr("Increase Font Size"),
                                                 QKeySequence(tr("Ctrl++")),
                                                 mainWindow );

    m_cmdFontSizeDecrease   = new Core::Command( tr("DecreaseFontSize"),
                                                 tr("&Decrease"),
                                                 tr("Decrease Font Size"),
                                                 QKeySequence(tr("Ctrl+-")),
                                                 mainWindow );

    m_cmdFontSizeReset      = new Core::Command( tr("ResetFontSize"),
                                                 tr("&Reset"),
                                                 tr("Reset Font Size to Default Value"),
                                                 QKeySequence(tr("Ctrl+0")),
                                                 mainWindow );

    m_cmdFold               = new Core::Command( tr("Fold"),
                                                 tr("Fold"),
                                                 tr("Fold Current Text Block"),
                                                 QKeySequence(tr("Ctrl+<")),
                                                 mainWindow );

    m_cmdUnfold             = new Core::Command( tr("Unfold"),
                                                 tr("Unfold"),
                                                 tr("Unfold Current Text Block"),
                                                 QKeySequence(tr("Ctrl+>")),
                                                 mainWindow );

    m_cmdToggleFoldAll      = new Core::Command( tr("ToggleFoldAll"),
                                                 tr("Toggle Fold All"),
                                                 tr("Fold/Unfold All Text Blocks of Current Document"),
                                                 QKeySequence(tr("Ctrl+Alt+9")),
                                                 mainWindow );

    m_cmdDuplicateSelectedLines
                            = new Core::Command( tr("DuplicateSelectedLines"),
                                                 tr("Duplicate Selected Lines"),
                                                 tr("Duplicate Selected Lines of Current Document"),
                                                 QKeySequence(tr("Ctrl+D")),
                                                 mainWindow );

    m_cmdMoveUpSelectedLines
                            = new Core::Command( tr("MoveUpSelectedLines"),
                                                 tr("Move Up Selected Lines"),
                                                 tr("Move Up Selected Lines of Current Document"),
                                                 QKeySequence(tr("Alt+Up")),
                                                 mainWindow );

    m_cmdMoveDownSelectedLines
                            = new Core::Command( tr("MoveDownSelectedLines"),
                                                 tr("Move Down Selected Lines"),
                                                 tr("Move Down Selected Lines of Current Document"),
                                                 QKeySequence(tr("Alt+Down")),
                                                 mainWindow );

    Core::CommandManager *commandManager = Core::CommandManager::getInstance();
    commandManager->registerCommands( tr("EditCommands"),
                                      QList<Core::Command *>() << m_cmdUndo
                                                               << m_cmdRedo
                                                               << m_cmdCopy
                                                               << m_cmdPaste
                                                               << m_cmdCut
                                                               << m_cmdSelectAll
                                                               << m_cmdLowerCase
                                                               << m_cmdUpperCase
                                                               << m_cmdFontSizeIncrease
                                                               << m_cmdFontSizeDecrease
                                                               << m_cmdFontSizeReset
                                                               << m_cmdFold
                                                               << m_cmdUnfold
                                                               << m_cmdToggleFoldAll
                                                               << m_cmdDuplicateSelectedLines
                                                               << m_cmdMoveUpSelectedLines
                                                               << m_cmdMoveDownSelectedLines );
}

void PlainTextEditCommands::updateEditorActions()
{
    TextEditor::ITextEditor *editor = qobject_cast<TextEditor::ITextEditor *>(DocumentManager::getInstance()->currentEditor());

    if ( !editor ) {
        m_cmdUndo->action()->setEnabled(false);
        m_cmdRedo->action()->setEnabled(false);
        m_cmdCopy->action()->setEnabled(false);
        m_cmdPaste->action()->setEnabled(false);
        m_cmdCut->action()->setEnabled(false);
        m_cmdSelectAll->action()->setEnabled(false);
        m_cmdLowerCase->action()->setEnabled(false);
        m_cmdUpperCase->action()->setEnabled(false);
        m_cmdFontSizeIncrease->action()->setEnabled(false);
        m_cmdFontSizeDecrease->action()->setEnabled(false);
        m_cmdFontSizeReset->action()->setEnabled(false);
        m_cmdFold->action()->setEnabled(false);
        m_cmdUnfold->action()->setEnabled(false);
        m_cmdToggleFoldAll->action()->setEnabled(false);
        m_cmdDuplicateSelectedLines->action()->setEnabled(false);
        m_cmdMoveUpSelectedLines->action()->setEnabled(false);
        m_cmdMoveDownSelectedLines->action()->setEnabled(false);
    }
    else {
        TextEditor::TextEditorWidget *textEditor = qobject_cast<TextEditor::TextEditorWidget *>(DocumentManager::getInstance()->currentEditorWidget());
        if ( textEditor ) {
            m_cmdUndo->action()->setEnabled(textEditor->document()->isUndoAvailable());
            m_cmdRedo->action()->setEnabled(textEditor->document()->isRedoAvailable());
        }
        else {
            m_cmdUndo->action()->setEnabled(false);
            m_cmdRedo->action()->setEnabled(false);
        }
        if ( editor->selectedText().isEmpty() ) {
            m_cmdCopy->action()->setEnabled(false);
            m_cmdCut->action()->setEnabled(false);
        }
        else {
            m_cmdCopy->action()->setEnabled(true);
            m_cmdCut->action()->setEnabled(true);
        }
        m_cmdPaste->action()->setEnabled(editor->canPaste());
        m_cmdSelectAll->action()->setEnabled(true);
        m_cmdLowerCase->action()->setEnabled(true);
        m_cmdUpperCase->action()->setEnabled(true);
        m_cmdFontSizeIncrease->action()->setEnabled(true);
        m_cmdFontSizeDecrease->action()->setEnabled(true);
        m_cmdFontSizeReset->action()->setEnabled(true);
        m_cmdFold->action()->setEnabled(true);
        m_cmdUnfold->action()->setEnabled(true);
        m_cmdToggleFoldAll->action()->setEnabled(true);
        m_cmdDuplicateSelectedLines->action()->setEnabled(true);
        m_cmdMoveUpSelectedLines->action()->setEnabled(true);
        m_cmdMoveDownSelectedLines->action()->setEnabled(true);
    }
}

void PlainTextEditCommands::fillEditMenu( QMenu *menu ) const
{
    menu->addAction(m_cmdUndo->action());
    menu->addAction(m_cmdRedo->action());

    menu->addSeparator();
    menu->addAction(m_cmdCopy->action());
    menu->addAction(m_cmdPaste->action());
    menu->addAction(m_cmdCut->action());
    menu->addSeparator();
    menu->addAction(m_cmdSelectAll->action());

    menu->addSeparator();
    QMenu *caseMenu = menu->addMenu(tr("&Switch Case"));
    caseMenu->addAction(m_cmdLowerCase->action());
    caseMenu->addAction(m_cmdUpperCase->action());

    menu->addSeparator();
    QMenu *fontSizeMenu = menu->addMenu(tr("&Font Size"));
    fontSizeMenu->addAction(m_cmdFontSizeIncrease->action());
    fontSizeMenu->addAction(m_cmdFontSizeDecrease->action());
    fontSizeMenu->addAction(m_cmdFontSizeReset->action());

    menu->addSeparator();
    QMenu *foldMenu = menu->addMenu(tr("&Folding"));
    foldMenu->addAction(m_cmdFold->action());
    foldMenu->addAction(m_cmdUnfold->action());
    foldMenu->addAction(m_cmdToggleFoldAll->action());

    menu->addSeparator();
    QMenu *advancedMenu = menu->addMenu(tr("&Advanced"));
    advancedMenu->addAction(m_cmdDuplicateSelectedLines->action());
    advancedMenu->addAction(m_cmdMoveUpSelectedLines->action());
    advancedMenu->addAction(m_cmdMoveDownSelectedLines->action());
}

void PlainTextEditCommands::connectActions( PlainTextDocumentPlugin *plugin )
{
    connect(m_cmdUndo->action(),                    SIGNAL(triggered()),    plugin, SLOT(undo()));
    connect(m_cmdRedo->action(),                    SIGNAL(triggered()),    plugin, SLOT(redo()));
    connect(m_cmdCopy->action(),                    SIGNAL(triggered()),    plugin, SLOT(copy()));
    connect(m_cmdPaste->action(),                   SIGNAL(triggered()),    plugin, SLOT(paste()));
    connect(m_cmdCut->action(),                     SIGNAL(triggered()),    plugin, SLOT(cut()));
    connect(m_cmdSelectAll->action(),               SIGNAL(triggered()),    plugin, SLOT(selectAll()));
    connect(m_cmdLowerCase->action(),               SIGNAL(triggered()),    plugin, SLOT(lowerCase()));
    connect(m_cmdUpperCase->action(),               SIGNAL(triggered()),    plugin, SLOT(upperCase()));
    connect(m_cmdFontSizeIncrease->action(),        SIGNAL(triggered()),    plugin, SLOT(fontSizeIncrease()));
    connect(m_cmdFontSizeDecrease->action(),        SIGNAL(triggered()),    plugin, SLOT(fontSizeDecrease()));
    connect(m_cmdFontSizeReset->action(),           SIGNAL(triggered()),    plugin, SLOT(fontSizeReset()));
    connect(m_cmdFold->action(),                    SIGNAL(triggered()),    plugin, SLOT(fold()));
    connect(m_cmdUnfold->action(),                  SIGNAL(triggered()),    plugin, SLOT(unfold()));
    connect(m_cmdToggleFoldAll->action(),           SIGNAL(triggered()),    plugin, SLOT(toggleFoldAll()));
    connect(m_cmdDuplicateSelectedLines->action(),  SIGNAL(triggered()),    plugin, SLOT(duplicateSelectedLines()));
    connect(m_cmdMoveUpSelectedLines->action(),     SIGNAL(triggered()),    plugin, SLOT(moveSelectedLinesUp()));
    connect(m_cmdMoveDownSelectedLines->action(),   SIGNAL(triggered()),    plugin, SLOT(moveSelectedLinesDown()));
}

//////////////////////////////////////////////////////////////////////////////////////
/// QPlainDocumentPlugin
PlainTextEditCommands *PlainTextDocumentPlugin::m_commands = 0;

PlainTextDocumentPlugin::PlainTextDocumentPlugin()
    : IDocumentPlugin()
{ }

PlainTextDocumentPlugin::~PlainTextDocumentPlugin()
{
    if ( m_commands ) {
        delete m_commands;
        m_commands = 0;
    }
}

IDocument *PlainTextDocumentPlugin::createDocument()
{
    PlainTextDocument *document = new PlainTextDocument(this);
    document->onFormatSettingsChanged();
    return document;
}

QStringList PlainTextDocumentPlugin::settingsKeyList() const
{
    return QStringList() << "000_TextEditor";
}

ISettings *PlainTextDocumentPlugin::createSettings( const QString &settingsKey ) const
{
    if ( settingsKey == QLatin1String("000_TextEditor") )
        return new SettingsCategoryTextEditor();

    return NULL;
}

void PlainTextDocumentPlugin::updateEditorActions()
{
    updateEditorCommands();
    m_commands->updateEditorActions();
}

void PlainTextDocumentPlugin::fillEditMenu( QMenu * menu )
{
    updateEditorCommands();
    m_commands->fillEditMenu(menu);
}

void PlainTextDocumentPlugin::initialize()
{
    TextEditor::Internal::Manager::instance()->registerMimeTypes();
    updateEditorCommands();

    TextEditor::Internal::SnippetsCollection::instance()->registerSnipperProvider(new TextEditor::Internal::PlainTextSnippetProvider());
}

bool PlainTextDocumentPlugin::activateDocumentOnMouseEnterEvent() const
{
    return s_activateDocOnMouseEnter;
}

QList<HighlighterFormatData *> PlainTextDocumentPlugin::createHighlightersFormatData() const
{
    return QList<HighlighterFormatData *>() << new GenericHighlighterFormatData();
}

void PlainTextDocumentPlugin::undo()
{
    TextEditor::TextEditorWidget *textEditor = qobject_cast<TextEditor::TextEditorWidget *>(DocumentManager::getInstance()->currentEditorWidget());
    if ( textEditor ) {
        textEditor->undo();
        updateEditorActions();
    }
}

void PlainTextDocumentPlugin::redo()
{
    TextEditor::TextEditorWidget *textEditor = qobject_cast<TextEditor::TextEditorWidget *>(DocumentManager::getInstance()->currentEditorWidget());
    if ( textEditor ) {
        textEditor->redo();
        updateEditorActions();
    }
}

void PlainTextDocumentPlugin::copy()
{
    TextEditor::ITextEditor *editor = qobject_cast<TextEditor::ITextEditor *>(DocumentManager::getInstance()->currentEditor());
    if ( editor )
        editor->copy();
}

void PlainTextDocumentPlugin::paste()
{
    TextEditor::ITextEditor *editor = qobject_cast<TextEditor::ITextEditor *>(DocumentManager::getInstance()->currentEditor());
    if ( editor ) {
        editor->paste();
        updateEditorActions();
    }
}

void PlainTextDocumentPlugin::cut()
{
    TextEditor::ITextEditor *editor = qobject_cast<TextEditor::ITextEditor *>(DocumentManager::getInstance()->currentEditor());
    if ( editor ) {
        editor->cut();
        updateEditorActions();
    }
}

void PlainTextDocumentPlugin::selectAll()
{
    TextEditor::ITextEditor *editor = qobject_cast<TextEditor::ITextEditor *>(DocumentManager::getInstance()->currentEditor());
    if ( editor ) {
        editor->selectAll();
        updateEditorActions();
    }
}

void PlainTextDocumentPlugin::lowerCase()
{
    TextEditor::ITextEditor *editor = qobject_cast<TextEditor::ITextEditor *>(DocumentManager::getInstance()->currentEditor());
    if ( editor ) {
        editor->switchCase(TextEditor::ITextEditor::Lower);
        updateEditorActions();
    }
}

void PlainTextDocumentPlugin::upperCase()
{
    TextEditor::ITextEditor *editor = qobject_cast<TextEditor::ITextEditor *>(DocumentManager::getInstance()->currentEditor());
    if ( editor ) {
        editor->switchCase(TextEditor::ITextEditor::Upper);
        updateEditorActions();
    }
}

void PlainTextDocumentPlugin::fontSizeIncrease()
{
    DocumentManager::getInstance()->fontSizeIncrease();
}

void PlainTextDocumentPlugin::fontSizeDecrease()
{
    DocumentManager::getInstance()->fontSizeDecrease();
}

void PlainTextDocumentPlugin::fontSizeReset()
{
    SettingsManager *settingsManager = SettingsManager::instance();
    settingsManager->value("000_TextEditor", "Zoom") = 100;

    DocumentManager *docManager = DocumentManager::getInstance();
    docManager->onFormatSettingsChanged();
}

void PlainTextDocumentPlugin::fold()
{
    TextEditor::TextEditorWidget *editorWidget = qobject_cast<TextEditor::TextEditorWidget *>(DocumentManager::getInstance()->currentEditorWidget());
    if ( editorWidget )
        editorWidget->fold();
}

void PlainTextDocumentPlugin::unfold()
{
    TextEditor::TextEditorWidget *editorWidget = qobject_cast<TextEditor::TextEditorWidget *>(DocumentManager::getInstance()->currentEditorWidget());
    if ( editorWidget )
        editorWidget->unfold();
}

void PlainTextDocumentPlugin::toggleFoldAll()
{
    TextEditor::TextEditorWidget *editorWidget = qobject_cast<TextEditor::TextEditorWidget *>(DocumentManager::getInstance()->currentEditorWidget());
    if ( editorWidget )
        editorWidget->unfoldAll();
}

void PlainTextDocumentPlugin::duplicateSelectedLines()
{
    TextEditor::TextEditorWidget *editorWidget = qobject_cast<TextEditor::TextEditorWidget *>(DocumentManager::getInstance()->currentEditorWidget());
    if ( editorWidget )
        editorWidget->duplicateSelectedLines();
}

void PlainTextDocumentPlugin::moveSelectedLinesUp()
{
    TextEditor::TextEditorWidget *editorWidget = qobject_cast<TextEditor::TextEditorWidget *>(DocumentManager::getInstance()->currentEditorWidget());
    if ( editorWidget )
        editorWidget->moveSelectedLinesUp();
}

void PlainTextDocumentPlugin::moveSelectedLinesDown()
{
    TextEditor::TextEditorWidget *editorWidget = qobject_cast<TextEditor::TextEditorWidget *>(DocumentManager::getInstance()->currentEditorWidget());
    if ( editorWidget )
        editorWidget->moveSelectedLinesDown();
}

void PlainTextDocumentPlugin::updateEditorCommands()
{
    if ( !m_commands ) {
        m_commands = new PlainTextEditCommands;
        m_commands->connectActions(this);
    }
}

TextEditor::IAssistProvider *PlainTextDocumentPlugin::createQuickFixProvider()
{
    return 0;
}
