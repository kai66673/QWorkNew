#include <QApplication>
#include <QMenu>
#include <QTextCodec>

#include "CmakeEditorPlugin.h"
#include "CmakeEditorWidget.h"
#include "Core.h"

CmakeDocument::CmakeDocument(IDocumentPlugin *docPlugin)
    : TextEditor::IBaseTextDocument()
    , m_documentPlugin(docPlugin)
{
    m_editor = new Cmake::CmakeEditorWidget(this);
}

CmakeDocument::~CmakeDocument()
{ }

QWidget *CmakeDocument::widget() const
{ return m_editor; }

TextEditor::TextEditorWidget *CmakeDocument::textEditorWidget() const
{ return m_editor; }

CmakeEditorPlugin::CmakeEditorPlugin()
    : PlainTextDocumentPlugin()
    , IDocumentHelper()
{ }

CmakeEditorPlugin::~CmakeEditorPlugin()
{ }

bool CmakeEditorPlugin::acceptExtension(const QString &extension) const
{
    QString ext = extension.toUpper();

    return (ext == QLatin1String("CMAKE"));
}

bool CmakeEditorPlugin::acceptFileName(const QString &fileName) const
{
    return (fileName == QLatin1String("CMakeLists.txt"));
}

IDocument *CmakeEditorPlugin::createDocument()
{
    CmakeDocument *document = new CmakeDocument(this);
    document->setTextCodec(QTextCodec::codecForName("UTF-8"));
    document->onFormatSettingsChanged();
    return document;
}

QStringList CmakeEditorPlugin::settingsKeyList() const
{ return QStringList(); }

ISettings *CmakeEditorPlugin::createSettings(const QString &/*settingsKey*/) const
{ return 0; }

bool CmakeEditorPlugin::documentEventFilter(IDocument *document, QEvent *event)
{
    if ( event->type() == QEvent::KeyPress ) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if ( keyEvent->key() == Qt::Key_Space && QApplication::keyboardModifiers () & Qt::ControlModifier ) {
            Cmake::CmakeEditorWidget *cmake_editor = qobject_cast<Cmake::CmakeEditorWidget *>(document->editorWidget());
            if ( cmake_editor ) {
                cmake_editor->invokeAssist(TextEditor::Completion);
                return true;
            }
        }
    }
    return false;
}

void CmakeEditorPlugin::initialize()
{
    QMainWindow *mainWindow = Core::Storage::mainWindow()->widget();

    m_cmdToggleComments = new Core::Command( tr("ToggleComments"),
                                             tr("Toggle Comments"),
                                             tr("Comment/Uncomment Selected Text"),
                                             QKeySequence(tr("Ctrl+/")),
                                             mainWindow );

    connect(m_cmdToggleComments->action(), SIGNAL(triggered()), this, SLOT(toggleComments()));

    Core::CommandManager *commandManager = Core::CommandManager::getInstance();
    commandManager->registerCommands( tr("CmakeEditorCommands"),
                                      QList<Core::Command *>() << m_cmdToggleComments );
}

QList<HighlighterFormatData *> CmakeEditorPlugin::createHighlightersFormatData() const
{
    return QList<HighlighterFormatData *>();
}

void CmakeEditorPlugin::fillEditMenu(QMenu *menu)
{
    PlainTextDocumentPlugin::fillEditMenu(menu);
    menu->addSeparator();
    menu->addAction(m_cmdToggleComments->action());
}

void CmakeEditorPlugin::toggleComments()
{
    Cmake::CmakeEditorWidget *cmake_editor = qobject_cast<Cmake::CmakeEditorWidget *>(DocumentManager::getInstance()->currentEditorWidget());
    if ( cmake_editor ) {
        cmake_editor->unCommentSelection();
        updateEditorActions();
    }
}

#ifndef HAVE_QT5
Q_EXPORT_PLUGIN2(CmakeEditorPlugin, CmakeEditorPlugin)
#endif
