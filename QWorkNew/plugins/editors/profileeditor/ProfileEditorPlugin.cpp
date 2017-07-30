#include <QTextCodec>
#include <QMenu>
#include <QApplication>

#include "ProfileEditorPlugin.h"
#include "ProfileEditorWidget.h"
#include "profileeditorconstants.h"
#include "Core.h"
#include "ifile.h"

////////////////////////////////////////////////////////////////////////////////////////////
/// QmakeHighlighterFormatData
class QmakeHighlighterFormatData: public HighlighterFormatData
{
public:
    QmakeHighlighterFormatData() {
        m_formats.append( new HighlighterFormat( QObject::tr("Variable"),
                                                 false,
                                                 false,
                                                 QColor(224,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Function"),
                                                 true,
                                                 false,
                                                 QColor(165,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Comment"),
                                                 true,
                                                 false,
                                                 QColor(165,   0,   0),
                                                 QColor() ) );

        m_formats.append( new HighlighterFormat( QObject::tr("Whitespace"),
                                                 true,
                                                 false,
                                                 QColor(  0,   0, 165),
                                                 QColor() ) );
    }

    virtual QString languageId() const { return QObject::tr("qmake"); }
};

ProFileDocument::ProFileDocument(IDocumentPlugin *docPlugin)
    : TextEditor::IBaseTextDocument()
    , m_documentPlugin(docPlugin)
{
    m_editor = new ProEditor::ProfileEditorWidget(this);
}

ProFileDocument::~ProFileDocument()
{ }

QWidget *ProFileDocument::widget() const
{ return m_editor; }

bool ProFileDocument::saveDocument()
{
    if ( !m_file )
        return false;

    QString errorString;
    if ( !textEditorWidget()->file()->save(&errorString, m_file->fileName(), false, true) )
        return false;

    textEditorWidget()->setFocus();

    return true;
}

bool ProFileDocument::saveAsDocument(const QString &filePath)
{
    QString errorString;
    if ( !textEditorWidget()->file()->save(&errorString, filePath, false, true) )
        return false;

    if ( m_file )
        delete m_file;
    m_file = new QFile(filePath);

    textEditorWidget()->setFocus();

    return true;
}

bool ProFileDocument::saveToDocument(const QString &filePath)
{
    QString errorString;
    if ( !textEditorWidget()->file()->save(&errorString, filePath, false, true) )
        return false;

    return true;
}

TextEditor::TextEditorWidget *ProFileDocument::textEditorWidget() const
{ return m_editor; }

ProfileEditorPlugin::ProfileEditorPlugin()
    : PlainTextDocumentPlugin()
    , IDocumentHelper()
{ }

ProfileEditorPlugin::~ProfileEditorPlugin()
{ }

bool ProfileEditorPlugin::acceptExtension(const QString &extension) const
{
    QString ext = extension.toUpper();

    return ( ext[0] == 'P' && ext[1] == 'R' &&
            (ext[2] == 'O' || ext[2] == 'I' || ext[2] == 'F') );
}

IDocument *ProfileEditorPlugin::createDocument()
{
    ProFileDocument *document = new ProFileDocument(this);
    document->setTextCodec(QTextCodec::codecForName("UTF-8"));
    document->onFormatSettingsChanged();
    return document;
}

QStringList ProfileEditorPlugin::settingsKeyList() const
{ return QStringList(); }

ISettings *ProfileEditorPlugin::createSettings(const QString &/*settingsKey*/) const
{ return 0; }

bool ProfileEditorPlugin::documentEventFilter(IDocument *document, QEvent *event)
{
    if ( event->type() == QEvent::KeyPress ) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if ( keyEvent->key() == Qt::Key_Space && QApplication::keyboardModifiers () & Qt::ControlModifier ) {
            ProEditor::ProfileEditorWidget *profile_editor = qobject_cast<ProEditor::ProfileEditorWidget *>(document->editorWidget());
            if ( profile_editor ) {
                profile_editor->invokeAssist(TextEditor::Completion);
                return true;
            }
        }
    }
    return false;
}

void ProfileEditorPlugin::initialize()
{
    QMainWindow *mainWindow = Core::Storage::mainWindow()->widget();

    m_cmdToggleComments = new Core::Command( tr("ToggleComments"),
                                             tr("Toggle Comments"),
                                             tr("Comment/Uncomment Selected Text"),
                                             QKeySequence(tr("Ctrl+/")),
                                             mainWindow );

    connect(m_cmdToggleComments->action(), SIGNAL(triggered()), this, SLOT(toggleComments()));

    Core::CommandManager *commandManager = Core::CommandManager::getInstance();
    commandManager->registerCommands( tr("ProFileEditorCommands"),
                                      QList<Core::Command *>() << m_cmdToggleComments );
}

QList<HighlighterFormatData *> ProfileEditorPlugin::createHighlightersFormatData() const
{
    return QList<HighlighterFormatData *>() << new QmakeHighlighterFormatData();
}

void ProfileEditorPlugin::fillEditMenu(QMenu *menu)
{
    PlainTextDocumentPlugin::fillEditMenu(menu);
    menu->addSeparator();
    menu->addAction(m_cmdToggleComments->action());
}

void ProfileEditorPlugin::toggleComments()
{
    ProEditor::ProfileEditorWidget *profile_editor = qobject_cast<ProEditor::ProfileEditorWidget *>(DocumentManager::getInstance()->currentEditorWidget());
    if ( profile_editor ) {
        profile_editor->unCommentSelection();
        updateEditorActions();
    }
}

#ifndef HAVE_QT5
Q_EXPORT_PLUGIN2(ProfileEditorPlugin, ProfileEditorPlugin)
#endif
