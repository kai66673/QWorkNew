#include <QVBoxLayout>
#include <QTextCodec>
#include <QTextBlock>
#include <QToolBar>
#include <QMenu>
#include <QApplication>

#include "GrammarEditorPlugin.h"
#include "GrammarEditorWidget.h"
#include "Core.h"
#include "ifile.h"
#include "GrammarEditorSettingsWidget.h"
#include "grammareditorconstants.h"
#include "cpphoverhandler.h"
#include "SettingsManager.h"

////////////////////////////////////////////////////////////////////////////////////////////
/// GrammarHighlighterFormatData
class GrammarHighlighterFormatData: public HighlighterFormatData
{
public:
    GrammarHighlighterFormatData() {
        m_formats.append( new HighlighterFormat( QObject::tr("Keyword"),
                                                 true,
                                                 false,
                                                 QColor(  0,   0, 255),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Rule Definition"),
                                                 true,
                                                 false,
                                                 QColor(  0, 255,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Rule Item"),
                                                 false,
                                                 false,
                                                 QColor( 64,  64,  64),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Variable"),
                                                 false,
                                                 false,
                                                 QColor(  0,   0, 128),
                                                 QColor() ) );
    }

    virtual QString languageId() const { return QObject::tr("Grammar"); }
};

SettingsCategoryGrammarEditor::SettingsCategoryGrammarEditor()
{
    valuesMap["Command"]                     = "win_bison.exe";
    valuesMap["Arguments"]                   = "-l -d %1";
    valuesMap["RunOnSave"]                   = true;
}

ISettingsTabPage *SettingsCategoryGrammarEditor::createSettingsTabPage( QWidget *parent )
{
    return new GrammarEditorSettingsWidget(valuesMap, parent);
}

void SettingsCategoryGrammarEditor::doApplySettings()
{ }

GrammarDocument::GrammarDocument( IDocumentPlugin *docPlugin )
    : TextEditor::IBaseTextDocument()
    , m_editor(0)
    , m_documentPlugin(docPlugin)
{
    m_mainWidget = new QWidget();

    m_toolBar = new DocumentToolBar;
    m_toolBar->addActions(docPlugin->toolbarActions(QString()));
    m_editor = new Grammar::EditorWidget(this, m_toolBar, m_mainWidget);

    QVBoxLayout *verticalLayout = new QVBoxLayout(m_mainWidget);
    verticalLayout->setSpacing(0);
    verticalLayout->setContentsMargins(0, 0, 0, 0);

    verticalLayout->addWidget(m_toolBar);
    verticalLayout->addWidget(m_editor);
}

GrammarDocument::~GrammarDocument()
{ }

QWidget *GrammarDocument::widget() const
{ return m_mainWidget; }

DocumentToolBar *GrammarDocument::toolBar()
{ return m_toolBar; }

bool GrammarDocument::saveDocument()
{
    bool result = TextEditor::IBaseTextDocument::saveDocument();

    if ( result ) {
        bool runGrammar = SettingsManager::instance()->value(Grammar::Constants::C_GRAMMAR_EDITOR_SETTINGS_ID, "RunOnSave").toBool();
        if ( runGrammar ) {
            if ( GrammarEditorPlugin *grammarPlugin = static_cast<GrammarEditorPlugin *>(m_documentPlugin) )
                grammarPlugin->onGrammarRun();
        }
    }

    return result;
}

bool GrammarDocument::saveAsDocument( const QString &filePath )
{
    bool modified = isModified();

    bool result = TextEditor::IBaseTextDocument::saveAsDocument(filePath);

    if ( result && modified ) {
        bool runGrammar = SettingsManager::instance()->value(Grammar::Constants::C_GRAMMAR_EDITOR_SETTINGS_ID, "RunOnSave").toBool();
        if ( runGrammar ) {
            if ( GrammarEditorPlugin *grammarPlugin = static_cast<GrammarEditorPlugin *>(m_documentPlugin) )
                grammarPlugin->onGrammarRun();
        }
    }

    return result;
}

TextEditor::TextEditorWidget *GrammarDocument::textEditorWidget() const
{ return m_editor; }

GrammarEditorPlugin::GrammarEditorPlugin()
    : PlainTextDocumentPlugin()
    , IDocumentHelper()
{
    CppEditor::Internal::CppHoverHandler *hoverHandler = CppEditor::Internal::CppHoverHandler::instance();
    connect(DocumentManager::getInstance(), SIGNAL(editorOpened(Core::IEditor*)), hoverHandler, SLOT(editorOpened(Core::IEditor*)));
}


GrammarEditorPlugin::~GrammarEditorPlugin()
{ }

bool GrammarEditorPlugin::acceptExtension( const QString &extension ) const
{
    QString ext = extension.toUpper();
    return ( ext == QLatin1String("Y") || ext == QLatin1String("YY") || ext == QLatin1String("G") );
}

IDocument *GrammarEditorPlugin::createDocument()
{
    GrammarDocument *document = new GrammarDocument(this);
    document->onFormatSettingsChanged();
    return document;
}

QStringList GrammarEditorPlugin::settingsKeyList() const
{
    return QStringList() << Grammar::Constants::C_GRAMMAR_EDITOR_SETTINGS_ID;
}

ISettings *GrammarEditorPlugin::createSettings( const QString &settingsKey ) const
{
    if ( settingsKey == Grammar::Constants::C_GRAMMAR_EDITOR_SETTINGS_ID )
        return new SettingsCategoryGrammarEditor();

    return 0;
}

bool GrammarEditorPlugin::documentEventFilter( IDocument *document, QEvent *event )
{
    if ( event->type() == QEvent::KeyPress ) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if ( keyEvent->key() == Qt::Key_Space ) {
            if ( QApplication::keyboardModifiers() & Qt::ControlModifier ) {
                if ( Grammar::EditorWidget *grammar_editor = qobject_cast<Grammar::EditorWidget *>(document->editorWidget()) ) {
                    grammar_editor->invokeAssist(TextEditor::Completion);
                    return true;
                }
            }
        }
    }
    return false;
}

QList<QAction *> GrammarEditorPlugin::toolbarActions( const QString &id ) const
{
    Q_UNUSED(id)
    return QList<QAction *>() << m_cmdFindUsages->action()
                              << m_cmdRenameUsages->action()
                              << m_cmdGotoDefinition->action()
                              << m_cmdRunGrammar->action();
}

void GrammarEditorPlugin::initialize()
{
    QMainWindow *mainWindow = Core::Storage::mainWindow()->widget();

    m_cmdToggleComments = new Core::Command( tr("ToggleComments"),
                                             tr("Toggle Comments"),
                                             tr("Comment/Uncomment Selected Text"),
                                             QKeySequence(tr("Ctrl+/")),
                                             mainWindow );

    m_cmdFindUsages           = new Core::Command( tr("FindUsages"),
                                                   QIcon(":/cppeditor/images/find_usages.png"),
                                                   tr("Find Usages"),
                                                   tr("Find Usages"),
                                                   QKeySequence(tr("Ctrl+Shift+U")),
                                                   mainWindow );

    m_cmdRenameUsages         = new Core::Command( tr("RenameUsages"),
                                                   QIcon(":/cppeditor/images/rename_usages.png"),
                                                   tr("Rename Usages"),
                                                   tr("Rename Usages"),
                                                   QKeySequence(tr("Ctrl+Shift+R")),
                                                   mainWindow );

    m_cmdGotoDefinition       = new Core::Command( tr("GotoDefinition"),
                                                   QIcon(":/cppeditor/images/goto_definition.png"),
                                                   tr("Goto Definition"),
                                                   tr("Goto Definition"),
                                                   QKeySequence("F2"),
                                                   mainWindow );

    connect(m_cmdToggleComments->action(), SIGNAL(triggered()), this, SLOT(toggleComments()));
    connect(m_cmdFindUsages->action(),      SIGNAL(triggered()), this, SLOT(findUsages()));
    connect(m_cmdRenameUsages->action(),    SIGNAL(triggered()), this, SLOT(renameUsages()));
    connect(m_cmdGotoDefinition->action(),  SIGNAL(triggered()), this, SLOT(gotoDefinition()));

    Core::CommandManager *commandManager = Core::CommandManager::getInstance();
    commandManager->registerCommands( tr("GrammarEditorCommands"),
                                      QList<Core::Command *>() << m_cmdToggleComments
                                                               << m_cmdFindUsages
                                                               << m_cmdRenameUsages
                                                               << m_cmdGotoDefinition
                                                               << m_cmdRunGrammar
                                                               << m_cmdToggleWarnings
                                                               << m_cmdCancelBuild );
}

void GrammarEditorPlugin::updateEditorActions()
{
    PlainTextDocumentPlugin::updateEditorActions();
}

void GrammarEditorPlugin::fillEditMenu( QMenu *menu )
{
    PlainTextDocumentPlugin::fillEditMenu(menu);
    menu->addSeparator();
    menu->addAction(m_cmdToggleComments->action());
    menu->addAction(m_cmdGotoDefinition->action());
    menu->addAction(m_cmdFindUsages->action());
    menu->addAction(m_cmdRenameUsages->action());
    menu->addSeparator();
    menu->addAction(m_cmdRunGrammar->action());
}

QList<UiProviderInterface *> GrammarEditorPlugin::createUiProviders( IMainWindow *mainWindow )
{
    m_cmdToggleWarnings     = new Core::Command( tr("ToggleShowBuildWarnings"),
                                                 QIcon(":/grammareditor/images/yy_warnings.png"),
                                                 tr("Show/Hide Warnings"),
                                                 tr("Show/Hide Warnings"),
                                                 QKeySequence(),
                                                 mainWindow->widget() );
    m_cmdToggleWarnings->action()->setCheckable(true);

    m_cmdCancelBuild        = new Core::Command( tr("CancelGrammarRun"),
                                                 QIcon(":/grammareditor/images/yy_cancel.png"),
                                                 tr("Cancel Grammar Run"),
                                                 tr("Cancel Grammar Run"),
                                                 QKeySequence(),
                                                 mainWindow->widget() );
    m_cmdCancelBuild->action()->setEnabled(false);


    QwTabBar *buildTabBar = new QwTabBar;
    buildTabBar->setDrawBase(false);
    buildTabBar->addTab(QIcon(":/grammareditor/images/yy_run_output.png"), tr("Output"));
    buildTabBar->addTab(QIcon(":/grammareditor/images/yy_run_issues.png"), tr("Issues"));
    m_buildDock = new QwCustomDockWidget( mainWindow->widget(), tr("Grammar Run"), "GrammarRunDockWidget",
                                          QKeySequence(tr("Ctrl+D,G")), QIcon(":/grammareditor/images/yy_run.png"),
                                          Qt::BottomDockWidgetArea, QwCustomDockWidget::First,
                                          QList<QAction *>() << m_cmdToggleWarnings->action() << m_cmdCancelBuild->action(),
                                          buildTabBar );

    m_runManager = new GrammarRunManager(buildTabBar);
    m_buildDock->setWidget(m_runManager);

    m_cmdRunGrammar         = new Core::Command( tr("RunGrammar"),
                                                 QIcon(":/grammareditor/images/yy_run.png"),
                                                 tr("Run"),
                                                 tr("Run Grammar Code Generation"),
                                                 QKeySequence(),
                                                 mainWindow->widget() );

    connect(buildTabBar, SIGNAL(currentChanged(int)), m_runManager, SLOT(setCurrentIndex(int)));
    connect(m_cmdRunGrammar->action(), SIGNAL(triggered()), this, SLOT(onGrammarRun()));
    connect(m_cmdToggleWarnings->action(), SIGNAL(toggled(bool)), this, SLOT(toggleHideWarnings(bool)));
    buildTabBar->setCurrentIndex(0);

    QMenu *contextMenu = new QMenu;
    contextMenu->addAction(m_cmdToggleWarnings->action());
    contextMenu->addAction(m_cmdCancelBuild->action());
    m_buildDock->setContextMenu(contextMenu);

    connect(m_runManager, SIGNAL(buildActionStarted()), this, SLOT(onBuildActionStarted()));
    connect(m_runManager, SIGNAL(buildActionFinished()), this, SLOT(onBuildActionFifnished()));

    return QList<UiProviderInterface *>() << new UiProviderInterface(m_buildDock);
}

bool GrammarEditorPlugin::applicationAboutToClose()
{
    onCancelGrammarRun();
    return true;
}

void GrammarEditorPlugin::applicationClose()
{ }

QList<HighlighterFormatData *> GrammarEditorPlugin::createHighlightersFormatData() const
{
    return QList<HighlighterFormatData *>() << new GrammarHighlighterFormatData();
}

void GrammarEditorPlugin::onBuildActionStarted()
{
    m_cmdCancelBuild->action()->setEnabled(true);
}

void GrammarEditorPlugin::onBuildActionFifnished()
{
    m_cmdCancelBuild->action()->setEnabled(false);
}

void GrammarEditorPlugin::toggleComments()
{
    Grammar::EditorWidget *grammar_editor = qobject_cast<Grammar::EditorWidget *>(DocumentManager::getInstance()->currentEditorWidget());
    if ( grammar_editor ) {
        grammar_editor->unCommentSelection();
        updateEditorActions();
    }
}

void GrammarEditorPlugin::findUsages()
{
    if ( Grammar::EditorWidget *editor = qobject_cast<Grammar::EditorWidget *>(DocumentManager::getInstance()->currentEditorWidget()) )
        editor->findUsages();
}

void GrammarEditorPlugin::renameUsages()
{
    if ( Grammar::EditorWidget *editor = qobject_cast<Grammar::EditorWidget *>(DocumentManager::getInstance()->currentEditorWidget()) )
        editor->renameUsages();
}

void GrammarEditorPlugin::gotoDefinition()
{
    if ( Grammar::EditorWidget *editor = qobject_cast<Grammar::EditorWidget *>(DocumentManager::getInstance()->currentEditorWidget()) )
        editor->jumpToDefinition();
}

void GrammarEditorPlugin::onGrammarRun()
{
    m_buildDock->showAndSetCurrentIfTabified();
    m_runManager->execute();
}

void GrammarEditorPlugin::onCancelGrammarRun()
{
    m_runManager->cancelBuildAction();
}

void GrammarEditorPlugin::toggleHideWarnings( bool onoff )
{
    m_runManager->toggleHideWarnings(onoff);
}

#ifndef HAVE_QT5
Q_EXPORT_PLUGIN2(GrammarEditorPlugin, GrammarEditorPlugin)
#endif
