#include "mimedatabase.h"

#include "Core.h"
#include "CommandManager.h"
#include "IPlugin.h"
#include "snippets/snippetscollection.h"

#include "pythoneditorwidget.h"
#include "basetextdocument.h"
#include "PythonEditorPlugin.h"
#include "pythoneditorconstants.h"
#include "PythonCompletiotionAssist.h"
#include "PythonSnippetProvider.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QMenu>

////////////////////////////////////////////////////////////////////////////////////////////
/// PythonHighlighterFormatData
class PythonHighlighterFormatData: public HighlighterFormatData
{
public:
    PythonHighlighterFormatData() {
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

        m_formats.append( new HighlighterFormat( QObject::tr("Keyword"),
                                                 true,
                                                 false,
                                                 QColor(  0,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Type"),
                                                 true,
                                                 false,
                                                 QColor(224,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Class Field"),
                                                 true,
                                                 false,
                                                 QColor(224,  96,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Magic Attribute"),
                                                 true,
                                                 true,
                                                 QColor(  0, 224,  24),
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

        m_formats.append( new HighlighterFormat( QObject::tr("Doxygen"),
                                                 true,
                                                 false,
                                                 QColor(165,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Identifier"),
                                                 true,
                                                 false,
                                                 QColor(165,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Whitespace"),
                                                 true,
                                                 false,
                                                 QColor(  0,   0, 165),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Imported Module"),
                                                 true,
                                                 false,
                                                 QColor(  0, 165, 165),
                                                 QColor() ) );
    }

    virtual QString languageId() const { return QObject::tr("Python"); }
};

//////////////////////////////////////////////////////////////////////////////////////
/// PythonDocument
PythonDocument::PythonDocument(IDocumentPlugin *docPlugin)
    : TextEditor::IBaseTextDocument()
    , m_editor(0)
    , m_documentPlugin(docPlugin)
{
    m_mainWidget = new QWidget();

    m_editor = new PythonEditor::EditorWidget(this, m_mainWidget);

    m_editor->setCompletionProvider(new PythonCompletionProvider());


    QVBoxLayout *verticalLayout = new QVBoxLayout(m_mainWidget);
    verticalLayout->setSpacing(0);
    verticalLayout->setContentsMargins(0, 0, 0, 0);

    verticalLayout->addWidget(m_editor);
}

PythonDocument::~PythonDocument()
{ }

QWidget *PythonDocument::widget() const
{ return m_mainWidget; }

TextEditor::TextEditorWidget *PythonDocument::textEditorWidget() const
{ return m_editor; }

//////////////////////////////////////////////////////////////////////////////////////
/// PythonEditorPlugin
PythonEditorPlugin::PythonEditorPlugin()
    : PlainTextDocumentPlugin()
    , IDocumentHelper()
{
    Core::MimeDatabase *mimeDatabase = Core::Storage::mainWindow()->mimeDatabase();
    QString errorMessage;
    mimeDatabase->addMimeTypes(QLatin1String(":/pythoneditor/pythoneditor.mimetypes.xml"), &errorMessage);
}

void PythonEditorPlugin::initialize()
{
    QMainWindow *mainWindow = Core::Storage::mainWindow()->widget();

    m_cmdToggleComments = new Core::Command( tr("ToggleComments"),
                                             tr("Toggle Comments"),
                                             tr("Comment/Uncomment Selected Text"),
                                             QKeySequence(tr("Ctrl+/")),
                                             mainWindow );

    connect(m_cmdToggleComments->action(), SIGNAL(triggered()), this, SLOT(toggleComments()));

    Core::CommandManager *commandManager = Core::CommandManager::getInstance();
    commandManager->registerCommands( tr("PythonEditorCommands"),
                                      QList<Core::Command *>() << m_cmdToggleComments );

    TextEditor::Internal::SnippetsCollection::instance()->registerSnipperProvider(new PythonEditor::PythonSnippetProvider());
}

void PythonEditorPlugin::updateEditorActions()
{
    PlainTextDocumentPlugin::updateEditorActions();
}

void PythonEditorPlugin::fillEditMenu( QMenu *menu )
{
    PlainTextDocumentPlugin::fillEditMenu(menu);
    menu->addSeparator();
    menu->addAction(m_cmdToggleComments->action());
}

QList<HighlighterFormatData *> PythonEditorPlugin::createHighlightersFormatData() const
{
    return QList<HighlighterFormatData *>() << new PythonHighlighterFormatData();
}

PythonEditorPlugin::~PythonEditorPlugin()
{
}

bool PythonEditorPlugin::acceptExtension( const QString &extension ) const
{
    QString ext = extension.toUpper();

    return ( ext == QLatin1String("PY") || ext == QLatin1String("QWPY") );
}

IDocument *PythonEditorPlugin::createDocument()
{
    PythonDocument *document = new PythonDocument(this);
    document->setTextCodec(QTextCodec::codecForName("UTF-8"));
    document->onFormatSettingsChanged();
    return document;
}

QStringList PythonEditorPlugin::settingsKeyList() const
{ return QStringList(); }

ISettings *PythonEditorPlugin::createSettings( const QString &/*settingsKey*/ ) const
{ return 0; }

bool PythonEditorPlugin::documentEventFilter(IDocument *document, QEvent *event)
{
    if ( event->type() == QEvent::KeyPress ) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if ( keyEvent->key() == Qt::Key_Space && QApplication::keyboardModifiers () & Qt::ControlModifier ) {
            PythonEditor::EditorWidget *python_editor = qobject_cast<PythonEditor::EditorWidget *>(document->editorWidget());
            if ( python_editor ) {
                python_editor->invokeAssist(TextEditor::Completion);
//                TextEditor::CompletionSupport::instance()->complete(cpp_editor->editor(), TextEditor::SemanticCompletion, true);
                return true;
            }
        }
    }
    return false;
}

void PythonEditorPlugin::toggleComments()
{
    PythonEditor::EditorWidget *python_editor = qobject_cast<PythonEditor::EditorWidget *>(DocumentManager::getInstance()->currentEditorWidget());
    if ( python_editor ) {
        python_editor->unCommentSelection();
        updateEditorActions();
    }
}

#ifndef HAVE_QT5
Q_EXPORT_PLUGIN2(PythonEditorPlugin, PythonEditorPlugin)
#endif
