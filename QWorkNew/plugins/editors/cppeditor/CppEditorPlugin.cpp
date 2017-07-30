#include <Symbols.h>
#include "cppquickfix.h"
#include "cppquickfixassistant.h"
#include "mimedatabase.h"

#include "Core.h"
#include "CommandManager.h"
#include "CppEditorSettingsWidget.h"
#include "IPlugin.h"

#include "cppeditor.h"
#include "basetextdocument.h"
#include "snippets/snippetscollection.h"
#include "cppsnippetprovider.h"
#include "CppEditorPlugin.h"
//#include "cpptools/cppcodecompletion.h"
#include "cpptools/cppcompletionassist.h"
#include "cpptools/cpptoolssettings.h"
#include "cpphoverhandler.h"
#include "cppeditorconstants.h"
#include "cppquickfixassistant.h"
#include "cpplocatorfilter.h"
#include "cppclassesfilter.h"
#include "cppfunctionsfilter.h"
#include "LocatorManager.h"

#include <QApplication>
#include <QSharedPointer>
#include <QVBoxLayout>
#include <QMenu>

////////////////////////////////////////////////////////////////////////////////////////////
/// CppHighlighterFormatData
class CppHighlighterFormatData: public HighlighterFormatData
{
public:
    CppHighlighterFormatData() {
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

        m_formats.append( new HighlighterFormat( QObject::tr("Builtin Type"),
                                                 true,
                                                 false,
                                                 QColor(224,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Keyword"),
                                                 true,
                                                 false,
                                                 QColor(  0,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Operator"),
                                                 false,
                                                 false,
                                                 QColor(  0,  64,  64),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Preprocessor"),
                                                 false,
                                                 false,
                                                 QColor(  0, 128,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Label"),
                                                 false,
                                                 false,
                                                 QColor( 32,  32,  32),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Comment"),
                                                 false,
                                                 false,
                                                 QColor(224,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Doxygen Comment"),
                                                 false,
                                                 false,
                                                 QColor(165,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Doxygen Tag"),
                                                 true,
                                                 false,
                                                 QColor(165,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Whitespace"),
                                                 true,
                                                 false,
                                                 QColor(224,  96,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("SQL-Embedded Keyword"),
                                                 true,
                                                 false,
                                                 QColor(  0,   0, 165),
                                                 QColor() ) );

        // Semantic formats
        m_formats.append( new HighlighterFormat( QObject::tr("Occurrences"),
                                                 true,
                                                 false,
                                                 QColor(),
                                                 QColor(222, 222, 222) ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Unused Occurrences"),
                                                 true,
                                                 false,
                                                 QColor(),
                                                 QColor(127, 127, 127) ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Type"),
                                                 true,
                                                 false,
                                                 QColor(  0, 255,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Local"),
                                                 false,
                                                 false,
                                                 QColor(127, 127, 127),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Field"),
                                                 false,
                                                 false,
                                                 QColor(255,  64,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Static"),
                                                 false,
                                                 false,
                                                 QColor(  0,  64, 255),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Virtual Method"),
                                                 true,
                                                 true,
                                                 QColor(  0, 255,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Function"),
                                                 true,
                                                 false,
                                                 QColor(  0, 255,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("SQL Table Alias"),
                                                 true,
                                                 false,
                                                 QColor(  0,  96,  96),
                                                 QColor() ) );
    }

    virtual QString languageId() const { return QObject::tr("Cpp"); }
};

////////////////////////////////////////////////////////////////////////////////////////////
/// SettingsCategoryCppEditor
SettingsCategoryCppEditor::SettingsCategoryCppEditor()
{
    valuesMap["ApplyToAll"]                     = true;
    valuesMap["ApplyToLessSize"]                = 600;
}

ISettingsTabPage *SettingsCategoryCppEditor::createSettingsTabPage( QWidget *parent )
{
    return new CppEditorSettingsWidget(valuesMap, parent);
}

void SettingsCategoryCppEditor::doApplySettings()
{ }

//////////////////////////////////////////////////////////////////////////////////////
/// QCppDocument
CppDocument::CppDocument( IDocumentPlugin *docPlugin )
    : TextEditor::IBaseTextDocument()
    , m_editor(0)
    , m_documentPlugin(docPlugin)
{
    m_mainWidget = new QWidget();

    m_toolBar = new DocumentToolBar;
    m_editor = new CppEditor::Internal::CPPEditorWidget(this, m_documentPlugin, m_toolBar, m_mainWidget);

    QVBoxLayout *verticalLayout = new QVBoxLayout(m_mainWidget);
    verticalLayout->setSpacing(0);
    verticalLayout->setContentsMargins(0, 0, 0, 0);

    verticalLayout->addWidget(m_toolBar);
    verticalLayout->addWidget(m_editor);
}

CppDocument::~CppDocument()
{ }

QWidget *CppDocument::widget() const
{ return m_mainWidget; }

DocumentToolBar *CppDocument::toolBar()
{ return m_toolBar; }

TextEditor::TextEditorWidget *CppDocument::textEditorWidget() const
{ return m_editor; }

//////////////////////////////////////////////////////////////////////////////////////
/// CppEditorPlugin
CppEditorPlugin::CppEditorPlugin()
    : PlainTextDocumentPlugin()
    , IDocumentHelper()
    , m_locatorData(new CppTools::CppLocatorData())
{
    Core::MimeDatabase *mimeDatabase = Core::Storage::mainWindow()->mimeDatabase();
    QString errorMessage;
    mimeDatabase->addMimeTypes(QLatin1String(":/cppeditor/CppEditor.mimetypes.xml"), &errorMessage);
    m_modelManager = CppTools::Internal::CppModelManager::instance();
    m_modelManager->reinitializeExtensions();

    CppTools::Internal::CppCompletionAssistProvider *cppCompletion = new CppTools::Internal::CppCompletionAssistProvider();
    Core::Storage::mainWindow()->addCompletionAssistProvider(cppCompletion);
    CppEditor::Internal::CppHoverHandler *hoverHandler = CppEditor::Internal::CppHoverHandler::instance();
    connect(DocumentManager::getInstance(), SIGNAL(editorOpened(Core::IEditor*)), hoverHandler, SLOT(editorOpened(Core::IEditor*)));

    new CppTools::CppToolsSettings(this);

    Core::Storage::mainWindow()->registerBuiltinScript("Cpp.SwitchHeaderSources", ":/cppeditor/scripting/switchHeaderSource.py");

    createActions();
    createFixFactories();
}

void CppEditorPlugin::initialize()
{
    Core::CommandManager *commandManager = Core::CommandManager::getInstance();

    commandManager->registerCommands( tr("CppEditorCommands"),
                                      QList<Core::Command *>() << m_cmdCppHelper
                                                               << m_cmdOutlineSortToggle
                                                               << m_cmdFindUsages
                                                               << m_cmdToggleComments
                                                               << m_cmdSwitchSourceHeaderFile
                                                               << m_cmdRefactor );

    connect(m_modelManager, &CppTools::Internal::CppModelManager::documentUpdated,
            m_locatorData, &CppTools::CppLocatorData::onDocumentUpdated);

    connect(m_modelManager, &CppTools::Internal::CppModelManager::aboutToRemoveFiles,
            m_locatorData, &CppTools::CppLocatorData::onAboutToRemoveFiles);


    LocatorManager::instance()->registerFilter(new CppTools::Internal::CppLocatorFilter(m_locatorData));
    LocatorManager::instance()->registerFilter(new CppTools::CppClassesFilter(m_locatorData));
    LocatorManager::instance()->registerFilter(new CppTools::Internal::CppFunctionsFilter(m_locatorData));

    TextEditor::Internal::SnippetsCollection::instance()->registerSnipperProvider(new CppEditor::Internal::CppSnippetProvider());
}

QList<HighlighterFormatData *> CppEditorPlugin::createHighlightersFormatData() const
{
    return QList<HighlighterFormatData *>() << new CppHighlighterFormatData();
}

void CppEditorPlugin::updateEditorActions()
{
    PlainTextDocumentPlugin::updateEditorActions();

    CppEditor::Internal::CPPEditorWidget *editor = qobject_cast<CppEditor::Internal::CPPEditorWidget *>(DocumentManager::getInstance()->currentEditorWidget());
    m_cmdRefactor->action()->setEnabled( editor ? !editor->isOutdated() : false );
//    if ( editor )
//        editor->fillQuickFixesMenu(m_quickFixMenu);
}

void CppEditorPlugin::fillEditMenu( QMenu *menu )
{
    PlainTextDocumentPlugin::fillEditMenu(menu);
    menu->addSeparator();
    menu->addAction(m_cmdCppHelper->action());
    menu->addSeparator();
    menu->addAction(m_cmdToggleComments->action());
    menu->addAction(m_cmdSwitchSourceHeaderFile->action());
    menu->addAction(m_cmdGotoDefinition->action());
    menu->addAction(m_cmdFindUsages->action());
    menu->addAction(m_cmdRenameUsages->action());
    menu->addSeparator();
    menu->addAction(m_cmdRefactor->action());

//    m_quickFixMenu = menu->addMenu(tr("&Refactor"));
}

TextEditor::IAssistProvider *CppEditorPlugin::createQuickFixProvider()
{
    return new CppEditor::Internal::CppQuickFixAssistProvider(&m_fixFactories);
}

CppEditorPlugin::~CppEditorPlugin()
{
    qDeleteAll(m_fixFactories);
    delete m_locatorData;
}

bool CppEditorPlugin::acceptExtension( const QString &extension ) const
{
    QString ext = extension.toUpper();				

    return ( ext == QLatin1String("CPP") ||
             ext == QLatin1String("C") ||
             ext == QLatin1String("CC") ||
             ext == QLatin1String("CXX") ||
             ext == QLatin1String("SC") ||
             ext == QLatin1String("H") ||
             ext == QLatin1String("HH") ||
             ext == QLatin1String("HPP") );
}

IDocument *CppEditorPlugin::createDocument()
{
    CppDocument *document = new CppDocument(this);
    document->onFormatSettingsChanged();
    return document;
}

QStringList CppEditorPlugin::settingsKeyList() const
{
    return QStringList() << CppEditor::Constants::C_CPP_EDITOR_SETTINGS_ID;
}

ISettings *CppEditorPlugin::createSettings( const QString &settingsKey ) const
{
     if ( settingsKey == CppEditor::Constants::C_CPP_EDITOR_SETTINGS_ID )
         return new SettingsCategoryCppEditor();

    return 0;
}

bool CppEditorPlugin::documentEventFilter(IDocument *document, QEvent *event)
{
    if ( event->type() == QEvent::KeyPress ) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if ( keyEvent->key() == Qt::Key_Space ) {
            if ( QApplication::keyboardModifiers() & Qt::ControlModifier ) {
                if ( CppEditor::Internal::CPPEditorWidget *cpp_editor = qobject_cast<CppEditor::Internal::CPPEditorWidget *>(document->editorWidget()) ) {
                    cpp_editor->invokeAssist(TextEditor::Completion);
                    return true;
                }
            }
        }
    }
    return false;
}

void CppEditorPlugin::createActions()
{
    QMainWindow *mainWindow = Core::Storage::mainWindow()->widget();

    m_cmdOutlineSortToggle    = new Core::Command( tr("OutlineSortToggle"),
                                                   QIcon(":/cppeditor/images/outline_sort.png"),
                                                   tr("Toggle Outline Sorting"),
                                                   tr("Toggle Outline Sorting"),
                                                   QKeySequence(),
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

    m_cmdOutlineSortToggle->action()->setCheckable(true);

    connect(m_cmdFindUsages->action(),      SIGNAL(triggered()), this, SLOT(findUsages()));
    connect(m_cmdRenameUsages->action(),    SIGNAL(triggered()), this, SLOT(renameUsages()));
    connect(m_cmdGotoDefinition->action(),  SIGNAL(triggered()), this, SLOT(gotoDefinition()));

    m_cmdToggleComments = new Core::Command( tr("ToggleComments"),
                                             tr("Toggle Comments"),
                                             tr("Comment/Uncomment Selected Text"),
                                             QKeySequence(tr("Ctrl+/")),
                                             mainWindow );

    m_cmdSwitchSourceHeaderFile = new Core::Command( tr("SwitchSourceHeaderFile"),
                                                     tr("Switch Source/Header"),
                                                     tr("Switch Between Source/Header Files"),
                                                     QKeySequence(tr("F4")),
                                                     mainWindow );

    m_cmdRefactor = new Core::Command( tr("Refactor"),
                                       tr("Refactor"),
                                       tr("Apply Refactoring Changes Available In Current Context"),
                                       QKeySequence(tr("Alt+Shift+Return")),
                                       mainWindow );

    m_cmdCppHelper = new Core::Command( tr("CppContextHelper"),
                                        tr("Cpp Context Helper"),
                                        tr("Apply Cpp Context Helper"),
                                        QKeySequence(tr("Alt+Return")),
                                        mainWindow );

    connect(m_cmdToggleComments->action(), SIGNAL(triggered()), this, SLOT(toggleComments()));
    connect(m_cmdSwitchSourceHeaderFile->action(), SIGNAL(triggered()), this, SLOT(switchSourceHeader()));
    connect(m_cmdRefactor->action(), SIGNAL(triggered()), this, SLOT(refactor()));
    connect(m_cmdCppHelper->action(), SIGNAL(triggered()), this, SLOT(cppHelper()));

    m_modelManager->addAdvancedCompletitionAction(m_cmdCppHelper->action());
}

void CppEditorPlugin::createFixFactories()
{
    registerQuickFixes(m_fixFactories);
}

QList<QAction *> CppEditorPlugin::toolbarActions( const QString &id ) const
{
    if ( id == QLatin1String("CPP_AG_TOGGLE_SORT_OUTLINE") ) {
        return QList<QAction *>() << m_cmdOutlineSortToggle->action();
    }
    else if ( id == QLatin1String("CPP_AG_USAGES_SUPPORT") ) {
        return QList<QAction *>() << m_cmdFindUsages->action()
                                  << m_cmdRenameUsages->action()
                                  << m_cmdGotoDefinition->action();
    }

    return QList<QAction *>();
}

void CppEditorPlugin::findUsages()
{
    if ( CppEditor::Internal::CPPEditorWidget *editor = qobject_cast<CppEditor::Internal::CPPEditorWidget *>(DocumentManager::getInstance()->currentEditorWidget()) )
        editor->findUsages();
}

void CppEditorPlugin::renameUsages()
{
    if ( CppEditor::Internal::CPPEditorWidget *editor = qobject_cast<CppEditor::Internal::CPPEditorWidget *>(DocumentManager::getInstance()->currentEditorWidget()) )
        editor->renameSymbolUnderCursor();
}

void CppEditorPlugin::gotoDefinition()
{
    if (CppEditor::Internal::CPPEditorWidget *editor = qobject_cast<CppEditor::Internal::CPPEditorWidget *>(DocumentManager::getInstance()->currentEditorWidget()) )
        editor->jumpToDefinition();
}

void CppEditorPlugin::toggleComments()
{
    CppEditor::Internal::CPPEditorWidget *editor = qobject_cast<CppEditor::Internal::CPPEditorWidget *>(DocumentManager::getInstance()->currentEditorWidget());
    if ( editor ) {
        editor->unCommentSelection();
        updateEditorActions();
    }
}

void CppEditorPlugin::switchSourceHeader()
{
    Core::Storage::mainWindow()->executeBuiltinScript("Cpp.SwitchHeaderSources");
}

void CppEditorPlugin::refactor()
{
    CppEditor::Internal::CPPEditorWidget *editor = qobject_cast<CppEditor::Internal::CPPEditorWidget *>(DocumentManager::getInstance()->currentEditorWidget());
    QMenu *quickFixMenu = new QMenu(Core::Storage::mainWindow()->widget());
    if ( editor->fillQuickFixesMenu(quickFixMenu) ) {
        quickFixMenu->setActiveAction(quickFixMenu->actions()[0]);
        quickFixMenu->exec(editor->viewport()->mapToGlobal( editor->cursorRect().bottomRight()) - QPoint(16, 16));
    }
    delete quickFixMenu;
}

void CppEditorPlugin::cppHelper()
{
    DocumentManager *documentManager = DocumentManager::getInstance();
    if ( ITextDocument *textDocument = dynamic_cast<ITextDocument *>(documentManager->currentDocument()) ) {
        QString key = textDocument->currentWord();
        if ( !key.isEmpty() )
            if ( CppEditor::Internal::CPPEditorWidget *cppEditorWidget = qobject_cast<CppEditor::Internal::CPPEditorWidget *>(documentManager->currentEditorWidget()) )
                cppEditorWidget->cppHelper(key, textDocument);
    }
}

#ifndef HAVE_QT5
Q_EXPORT_PLUGIN2(CppEditorPlugin, CppEditorPlugin)
#endif
