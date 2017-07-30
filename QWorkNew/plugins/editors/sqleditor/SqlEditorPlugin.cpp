#include <QVBoxLayout>
#include <QToolBar>
#include <QFileInfo>
#include <QTableWidget>
#include <QComboBox>
#include <QAction>
#include <QSplitter>
#include <QApplication>
#include <QMenu>

#include "Core.h"
#include "IPlugin.h"
#include "SQLEditor.h"
#include "SqlEditorPlugin.h"
#include "ifile.h"
#include "CommandManager.h"
#include "SqlHoverHandler.h"
#include "sql_constants.h"
#include "SqlCompletiotionAssist.h"

////////////////////////////////////////////////////////////////////////////////////////////
/// SqlHighlighterFormatData
class SqlHighlighterFormatData: public HighlighterFormatData
{
public:
    SqlHighlighterFormatData() {
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

        m_formats.append( new HighlighterFormat( QObject::tr("Keyword1"),
                                                 true,
                                                 false,
                                                 QColor(  0,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Keyword2"),
                                                 true,
                                                 false,
                                                 QColor(  0,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Keyword3"),
                                                 true,
                                                 false,
                                                 QColor(  0,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Keyword4"),
                                                 true,
                                                 false,
                                                 QColor(  0,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Keyword5"),
                                                 true,
                                                 false,
                                                 QColor(  0,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Keyword6"),
                                                 true,
                                                 false,
                                                 QColor(  0,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Keyword7"),
                                                 true,
                                                 false,
                                                 QColor(  0,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Keyword8"),
                                                 true,
                                                 false,
                                                 QColor(  0,   0,   0),
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

        m_formats.append( new HighlighterFormat( QObject::tr("Table"),
                                                 true,
                                                 false,
                                                 QColor(  0, 255,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Alias"),
                                                 false,
                                                 false,
                                                 QColor(127, 127, 127),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Field"),
                                                 false,
                                                 false,
                                                 QColor(255,  64,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Index"),
                                                 true,
                                                 false,
                                                 QColor(  0, 255,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Trigger"),
                                                 true,
                                                 false,
                                                 QColor(  0, 255,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Constraint"),
                                                 true,
                                                 false,
                                                 QColor(  0, 255,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Bind"),
                                                 false,
                                                 false,
                                                 QColor(  0,  64, 255),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Schema"),
                                                 true,
                                                 true,
                                                 QColor(  0, 192,  32),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Identifier"),
                                                 false,
                                                 false,
                                                 QColor(  0,  64, 255),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Label"),
                                                 false,
                                                 false,
                                                 QColor(  0,  64, 255),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Function"),
                                                 false,
                                                 false,
                                                 QColor(  0,  64, 255),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Cursor"),
                                                 true,
                                                 false,
                                                 QColor(  0,  96, 255),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Map"),
                                                 false,
                                                 false,
                                                 QColor(  0,  96, 255),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Area"),
                                                 false,
                                                 false,
                                                 QColor(  0,  96, 255),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Module"),
                                                 false,
                                                 false,
                                                 QColor(  0,  64, 255),
                                                 QColor() ) );
    }

    virtual QString languageId() const { return QObject::tr("SQL"); }
};

//////////////////////////////////////////////////////////////////////////////////////
/// SqlDocument
SqlDocument::SqlDocument( IDocumentPlugin *docPlugin )
    : TextEditor::IBaseTextDocument()
    , m_editor(0)
    , m_documentPlugin(docPlugin)
    , m_sqlDocument(0)
{
    m_mainWidget = new QWidget();

    m_editor = new Sql::Editor::SQLEditorWidget(this, m_mainWidget);

    QVBoxLayout *verticalLayout = new QVBoxLayout(m_mainWidget);
    verticalLayout->setSpacing(0);
    verticalLayout->setContentsMargins(0, 0, 0, 0);

    verticalLayout->addWidget(m_editor);
}

SqlDocument::~SqlDocument()
{ }

QWidget *SqlDocument::widget() const
{ return m_mainWidget; }

Sql::Document::Ptr SqlDocument::sqlDocument()
{
    if ( !m_sqlDocument )
        m_sqlDocument = Sql::Document::create(m_fileName);

    return m_sqlDocument;
}

QFuture<void> SqlDocument::refresh()
{
    sqlDocument()->setSource(text().toLatin1());
    return QFuture<void>();
}

TextEditor::TextEditorWidget *SqlDocument::textEditorWidget() const
{ return m_editor; }

//////////////////////////////////////////////////////////////////////////////////////
/// SqlEditorPlugin
SqlEditorPlugin::SqlEditorPlugin()
    : PlainTextDocumentPlugin()
    , IDocumentHelper()
{
    Sql::Editor::SqlHoverHandler *hoverHandler = new Sql::Editor::SqlHoverHandler(this);
    connect(DocumentManager::getInstance(), SIGNAL(editorOpened(Core::IEditor *)),
            hoverHandler, SLOT(editorOpened(Core::IEditor *)));

    Sql::SqlCompletionAssistProvider *sqlCompletion = new Sql::SqlCompletionAssistProvider();
    Core::Storage::mainWindow()->addCompletionAssistProvider(sqlCompletion);
}

bool SqlEditorPlugin::acceptExtension( const QString &extension ) const
{
    QString ext = extension.toUpper();

    return ( ext == QLatin1String("SQL") );
}

IDocument * SqlEditorPlugin::createDocument()
{
    SqlDocument *document = new SqlDocument(this);
    document->onFormatSettingsChanged();
    return document;
}

QStringList SqlEditorPlugin::settingsKeyList() const
{
    return QStringList();
}

ISettings *SqlEditorPlugin::createSettings( const QString &settingsKey ) const
{
    Q_UNUSED(settingsKey);
    return 0;
}

bool SqlEditorPlugin::documentEventFilter( IDocument *document, QEvent *event )
{
    if ( event->type() == QEvent::KeyPress ) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if ( keyEvent->key() == Qt::Key_Space && QApplication::keyboardModifiers () & Qt::ControlModifier ) {
            Sql::Editor::SQLEditorWidget *sql_editor = qobject_cast<Sql::Editor::SQLEditorWidget *>(document->editorWidget());
            if ( sql_editor ) {
                sql_editor->invokeAssist(TextEditor::Completion);
                //TextEditor::CompletionSupport::instance()->complete(sql_editor->editor(), TextEditor::SemanticCompletion, true);
                return true;
            }
        }
    }
    return false;
}

void SqlEditorPlugin::initialize()
{
    QMainWindow *mainWindow = Core::Storage::mainWindow()->widget();

    m_cmdToggleComments         = new Core::Command( tr("ToggleComments"),
                                                     tr("Toggle Comments"),
                                                     tr("Comment/Uncomment Selected Text"),
                                                     QKeySequence(tr("Ctrl+/")),
                                                     mainWindow );

    connect(m_cmdToggleComments->action(),      SIGNAL(triggered()), this, SLOT(toggleComments()));

    m_metadataProvider = new Sql::DbMetadataProvider;

    Core::CommandManager *commandManager = Core::CommandManager::getInstance();
    commandManager->registerCommands( tr("SqlEditorCommands"),
                                      QList<Core::Command *>() << m_cmdToggleComments );
}

QList<HighlighterFormatData *> SqlEditorPlugin::createHighlightersFormatData() const
{
    return QList<HighlighterFormatData *>() << new SqlHighlighterFormatData();
}

void SqlEditorPlugin::updateEditorActions()
{
    PlainTextDocumentPlugin::updateEditorActions();
}

void SqlEditorPlugin::fillEditMenu( QMenu *menu )
{
    PlainTextDocumentPlugin::fillEditMenu(menu);
    menu->addSeparator();
    menu->addAction(m_cmdToggleComments->action());
}

void SqlEditorPlugin::toggleComments()
{
    Sql::Editor::SQLEditorWidget *editor = qobject_cast<Sql::Editor::SQLEditorWidget *>(DocumentManager::getInstance()->currentEditorWidget());
    if ( editor ) {
        editor->unCommentSelection();
        updateEditorActions();
    }
}

#ifndef HAVE_QT5
Q_EXPORT_PLUGIN2(SqlEditorPlugin, SqlEditorPlugin)
#endif
