#include <QApplication>
#include <QVBoxLayout>
#include <QMenu>

#include "KarelEditorPlugin.h"
#include "KarelEditor.h"
#include "KarelHoverHandler.h"
#include "Core.h"
#include "snippets/snippetscollection.h"
#include "KarelSnippetProvider.h"
#include "KarelCompletionAssist.h"
#include "KarelEditorSettingsWidget.h"
#include "KarelSymbolIcons.h"
#include "karel_constants.h"

////////////////////////////////////////////////////////////////////////////////////////////
/// SettingsCategoryKarelEditor
SettingsCategoryKarelEditor::SettingsCategoryKarelEditor()
{
    valuesMap["ApplyOnSave"] = true;

    valuesMap["ApplyToBuiltings"] = true;
    valuesMap["ApplyToKeywords"] = true;
    valuesMap["ApplyToTypes"] = true;
    valuesMap["ApplyToDirectives"] = true;
    valuesMap["ApplyToSystems"] = true;

    valuesMap["BuiltingsCaseType"] = 0;
    valuesMap["KeywordsCaseType"] = 0;
    valuesMap["TypesCaseType"] = 0;
    valuesMap["DirectivesCaseType"] = 0;
    valuesMap["SystemsCaseType"] = 0;
}

ISettingsTabPage *SettingsCategoryKarelEditor::createSettingsTabPage(QWidget *parent)
{
    return new KarelEditorSettingsWidget(valuesMap, parent);
}

void SettingsCategoryKarelEditor::doApplySettings()
{ }

////////////////////////////////////////////////////////////////////////////////////////////
/// KarelHighlighterFormatData
class KarelHighlighterFormatData: public HighlighterFormatData
{
public:
    KarelHighlighterFormatData() {
        m_formats.append( new HighlighterFormat( QObject::tr("Builtin Function"),
                                                 false,
                                                 false,
                                                 QColor(  0,   0, 128),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Keyword"),
                                                 true,
                                                 false,
                                                 QColor(  0,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Builtin Type"),
                                                 true,
                                                 false,
                                                 QColor(  0,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Directive"),
                                                 true,
                                                 false,
                                                 QColor(  0,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("System Vars/Constants"),
                                                 true,
                                                 false,
                                                 QColor(  0,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Number"),
                                                 false,
                                                 false,
                                                 QColor(  0,   0, 224),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("String"),
                                                 false,
                                                 false,
                                                 QColor(  0,   0, 128),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Operator"),
                                                 false,
                                                 false,
                                                 QColor(  0,  64,  64),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Comment"),
                                                 false,
                                                 false,
                                                 QColor(224,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Whitespace"),
                                                 false,
                                                 false,
                                                 QColor(224,   0,   0),
                                                 QColor() ) );

        /// Semantic
        m_formats.append( new HighlighterFormat( QObject::tr("User Type"),
                                                 true,
                                                 false,
                                                 QColor(  0, 255,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Constant"),
                                                 false,
                                                 false,
                                                 QColor( 64, 248,  12),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Variable"),
                                                 false,
                                                 false,
                                                 QColor(248,  64,  64),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Type Field"),
                                                 false,
                                                 false,
                                                 QColor(255,  64,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Routine"),
                                                 false,
                                                 true,
                                                 QColor(127,  64, 248),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Program Name"),
                                                 false,
                                                 true,
                                                 QColor( 12,  64, 248),
                                                 QColor() ) );
    }

    virtual QString languageId() const { return QObject::tr("KAREL"); }
};

KarelDocument::KarelDocument(IDocumentPlugin *docPlugin)
    : TextEditor::IBaseTextDocument()
    , m_documentPlugin(docPlugin)
{
    m_toolBar = new DocumentToolBar;
    m_mainWidget = new QWidget();
    m_editor = new Karel::Editor::KarelEditorWidget(this, docPlugin, m_toolBar, m_mainWidget);

    QVBoxLayout *verticalLayout = new QVBoxLayout(m_mainWidget);
    verticalLayout->setSpacing(0);
    verticalLayout->setContentsMargins(0, 0, 0, 0);

    verticalLayout->addWidget(m_toolBar);
    verticalLayout->addWidget(m_editor);
}

KarelDocument::~KarelDocument()
{ }

QWidget *KarelDocument::widget() const
{ return m_mainWidget; }

DocumentToolBar *KarelDocument::toolBar()
{ return m_toolBar; }

TextEditor::TextEditorWidget *KarelDocument::textEditorWidget() const
{ return m_editor; }

KarelEditorPlugin::KarelEditorPlugin()
    : PlainTextDocumentPlugin()
    , IDocumentHelper()
{
    Karel::KarelSymbolIcons::init();

    Karel::KarelCompletionAssistProvider *karelCompletion = new Karel::KarelCompletionAssistProvider();
    Core::Storage::mainWindow()->addCompletionAssistProvider(karelCompletion);
    Karel::Editor::KarelHoverHandler *hoverHandler = new Karel::Editor::KarelHoverHandler(this);
    connect(DocumentManager::getInstance(), SIGNAL(editorOpened(Core::IEditor *)),
            hoverHandler, SLOT(editorOpened(Core::IEditor *)));

    createActions();
}

KarelEditorPlugin::~KarelEditorPlugin()
{ }

bool KarelEditorPlugin::acceptExtension(const QString &extension) const
{
    return !extension.compare(QStringLiteral("kl"), Qt::CaseInsensitive);
}

IDocument *KarelEditorPlugin::createDocument()
{
    KarelDocument *document = new KarelDocument(this);
    document->setFixOnSaveHandler(&m_caseFixer);
    document->onFormatSettingsChanged();
    return document;
}

QStringList KarelEditorPlugin::settingsKeyList() const
{
    return QStringList() << Karel::Constants::C_KAREL_EDITOR_SETTINGS_ID;
}

ISettings *KarelEditorPlugin::createSettings(const QString &settingsKey) const
{
    if ( settingsKey == Karel::Constants::C_KAREL_EDITOR_SETTINGS_ID )
        return new SettingsCategoryKarelEditor();

    return 0;
}

bool KarelEditorPlugin::documentEventFilter(IDocument *document, QEvent *event)
{
    if ( event->type() == QEvent::KeyPress ) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if ( keyEvent->key() == Qt::Key_Space && QApplication::keyboardModifiers () & Qt::ControlModifier ) {
            Karel::Editor::KarelEditorWidget *karel_editor = qobject_cast<Karel::Editor::KarelEditorWidget *>(document->editorWidget());
            if ( karel_editor ) {
                karel_editor->invokeAssist(TextEditor::Completion);
                return true;
            }
        }
    }
    return false;
}

QList<QAction *> KarelEditorPlugin::toolbarActions(const QString &id) const
{
    if ( id == QLatin1String("KAREL_AG_USAGES_SUPPORT") )
        return QList<QAction *>() << m_cmdGotoDefinition->action()
                                  << m_cmdFindUsages->action()
                                  << m_cmdRenameUsages->action();

    return QList<QAction *>();
}

void KarelEditorPlugin::initialize()
{
    Core::CommandManager *commandManager = Core::CommandManager::getInstance();
    commandManager->registerCommands( tr("KarelEditorCommands"),
                                      QList<Core::Command *>() << m_cmdToggleComments
                                                               << m_cmdGotoDefinition
                                                               << m_cmdFindUsages
                                                               << m_cmdRenameUsages );

    TextEditor::Internal::SnippetsCollection::instance()->registerSnipperProvider(new Karel::Editor::KarelSnippetProvider());
}

QList<HighlighterFormatData *> KarelEditorPlugin::createHighlightersFormatData() const
{
    return QList<HighlighterFormatData *>() << new KarelHighlighterFormatData();
}

void KarelEditorPlugin::updateEditorActions()
{
    PlainTextDocumentPlugin::updateEditorActions();
}

void KarelEditorPlugin::fillEditMenu(QMenu *menu)
{
    PlainTextDocumentPlugin::fillEditMenu(menu);
    menu->addSeparator();
    menu->addAction(m_cmdToggleComments->action());
    menu->addAction(m_cmdGotoDefinition->action());
    menu->addAction(m_cmdFindUsages->action());
    menu->addAction(m_cmdRenameUsages->action());
}

void KarelEditorPlugin::toggleComments()
{
    if ( Karel::Editor::KarelEditorWidget *editor = qobject_cast<Karel::Editor::KarelEditorWidget *>(DocumentManager::getInstance()->currentEditorWidget()) ) {
        editor->unCommentSelection();
        updateEditorActions();
    }
}

void KarelEditorPlugin::gotoDefinition()
{
    if ( Karel::Editor::KarelEditorWidget *editor = qobject_cast<Karel::Editor::KarelEditorWidget *>(DocumentManager::getInstance()->currentEditorWidget()) )
        editor->jumpToDefinition();
}

void KarelEditorPlugin::findUsages()
{
    if ( Karel::Editor::KarelEditorWidget *editor = qobject_cast<Karel::Editor::KarelEditorWidget *>(DocumentManager::getInstance()->currentEditorWidget()) )
        editor->findUsages();
}

void KarelEditorPlugin::replaceUsages()
{
    if ( Karel::Editor::KarelEditorWidget *editor = qobject_cast<Karel::Editor::KarelEditorWidget *>(DocumentManager::getInstance()->currentEditorWidget()) )
        editor->replaceUsages();
}

void KarelEditorPlugin::createActions()
{
    QMainWindow *mainWindow = Core::Storage::mainWindow()->widget();

    m_cmdToggleComments         = new Core::Command( tr("ToggleComments"),
                                                     tr("Toggle Comments"),
                                                     tr("Comment/Uncomment Selected Text"),
                                                     QKeySequence(tr("Ctrl+/")),
                                                     mainWindow );

    m_cmdGotoDefinition       = new Core::Command( tr("GotoDefinition"),
                                                   QIcon(":/kareleditor/images/goto_definition.png"),
                                                   tr("Goto Definition"),
                                                   tr("Goto Definition"),
                                                   QKeySequence("F2"),
                                                   mainWindow );

    m_cmdFindUsages           = new Core::Command( tr("FindUsages"),
                                                   QIcon(":/kareleditor/images/find_usages.png"),
                                                   tr("Find Usages"),
                                                   tr("Find Usages"),
                                                   QKeySequence("Ctrl+Shift+U"),
                                                   mainWindow );

    m_cmdRenameUsages         = new Core::Command( tr("RenameUsages"),
                                                   QIcon(":/kareleditor/images/rename_usages.png"),
                                                   tr("Rename Usages"),
                                                   tr("Rename Usages"),
                                                   QKeySequence("Ctrl+Shift+R"),
                                                   mainWindow );

    connect(m_cmdToggleComments->action(), SIGNAL(triggered()), this, SLOT(toggleComments()));
    connect(m_cmdGotoDefinition->action(),  SIGNAL(triggered()), this, SLOT(gotoDefinition()));
    connect(m_cmdFindUsages->action(),  SIGNAL(triggered()), this, SLOT(findUsages()));
    connect(m_cmdRenameUsages->action(),  SIGNAL(triggered()), this, SLOT(replaceUsages()));
}

#ifndef HAVE_QT5
Q_EXPORT_PLUGIN2(KarelEditorPlugin, KarelEditorPlugin)
#endif
