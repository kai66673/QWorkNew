#include "mimedatabase.h"

#include "Core.h"
#include "IPlugin.h"
#include "stringutils.h"

#include "RichEditorPlugin.h"
#include "RichEditorWidget.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>

//////////////////////////////////////////////////////////////////////////////////////
/// RichDocument
RichDocument::RichDocument( IDocumentPlugin *docPlugin )
    : IDocument()
    , m_file(0)
    , m_editor(0)
    , m_fileName("")
    , m_documentPlugin(docPlugin)
{
    m_editorMainWidget = new QMainWindow();

    m_editor = new RichEditor::RichEditorWidget(m_editorMainWidget);
    connect(m_editor->document(), SIGNAL(modificationChanged(bool)), this, SLOT(onModificationChanged()));

    m_editorMainWidget->setCentralWidget(m_editor);
    m_editorMainWidget->addToolBar(m_editor->textToolBar());
    m_editorMainWidget->addToolBar(m_editor->paraToolBar());
    m_editorMainWidget->addToolBar(m_editor->tableToolBar());
}

RichDocument::~RichDocument()
{
    if ( m_file )
        delete m_file;
}

QWidget *RichDocument::editorWidget() const
{
    return m_editor;
}

QWidget *RichDocument::widget() const
{
    return m_editorMainWidget;
}

Core::IEditor *RichDocument::editor() const
{ return 0; }

bool RichDocument::newDocument ( const QString &fileName )
{
    if ( m_file ) {
        delete m_file;
        m_file = NULL;
    }

    m_editor->setContent("");

    m_fileName = fileName;
    m_editor->setFocus();
    m_editor->document()->setModified(false);

    return true;
}

bool RichDocument::newDocumentFromFile( const QString &fileName, const QString &contentFilePath )
{
    if ( m_file ) {
        delete m_file;
        m_file = NULL;
    }

    QString errorString;
    if ( !m_editor->setFileContent(contentFilePath, errorString) )
        return false;

    m_fileName = fileName;
    m_editor->setFocus();
    m_editor->document()->setModified(true);

    return true;
}

bool RichDocument::openDocument( const QString &filePath )
{
    QString errorString;
    if ( !m_editor->setFileContent(filePath, errorString) )
        return false;

    if ( m_file )
        delete m_file;
    m_file = new QFile(filePath);

    m_editor->setFocus();
    m_editor->document()->setModified(false);
    return true;
}

bool RichDocument::saveDocument()
{
    if ( !m_file )
        return false;

    QTextDocumentWriter writer(m_file->fileName());
    bool writed = writer.write(m_editor->document());

    m_editor->setFocus();
    m_editor->document()->setModified(false);

    return writed;
}

bool RichDocument::saveAsDocument( const QString &filePath )
{
    QTextDocumentWriter writer(filePath);
    if ( !writer.write(m_editor->document()) )
        return false;

    if ( m_file )
        delete m_file;
    m_file = new QFile(filePath);

    m_editor->setFocus();
    m_editor->document()->setModified(false);

    return true;
}

bool RichDocument::saveToDocument( const QString &filePath )
{
    QTextDocumentWriter writer(filePath);
    if ( !writer.write(m_editor->document()) )
        return false;

    return true;
}

bool RichDocument::renameTo(const QString &newFilePath)
{
    if ( m_file )
        delete m_file;

    m_file = new QFile(newFilePath);

    return true;
}

QString RichDocument::title() const
{
    if ( m_file ) {
        QFileInfo fi(*m_file);
        return fi.fileName();
    }

    return m_fileName;
}

QString RichDocument::toolTip() const
{
    if ( m_file ) {
        QFileInfo fi(*m_file);
        return fi.absoluteFilePath();
    }

    return QStringLiteral("");
}

QString RichDocument::filePath() const
{
    return toolTip();
}

bool RichDocument::isModified() const
{
    return m_editor->document()->isModified();
}

void RichDocument::setModified(bool onoff)
{
    m_editor->document()->setModified(onoff);
}

void RichDocument::setCursor( IDocumentCursor *position )
{
    if ( RichEditor::RichDocumentPosition *richPosition = static_cast<RichEditor::RichDocumentPosition *>(position) ) {
        QTextCursor c(m_editor->document());
        c.setPosition(richPosition->position());
        m_editor->setTextCursor(c);
    }
    editorWidget()->setFocus();
}

void RichDocument::onFormatSettingsChanged()
{ }

void RichDocument::emitDocumentOpened( DocumentManager *documentManager )
{
    Q_UNUSED(documentManager)
//    RichEditor::RichEditorWidget *rich_editor = qobject_cast<RichEditor::RichEditorWidget *>(editorWidget());
//    if ( rich_editor )
//        documentManager->emitEditorOpened(rich_editor->editor());
}

void RichDocument::emitDocumentClosed( DocumentManager *documentManager )
{
    Q_UNUSED(documentManager)
//    RichEditor::RichEditorWidget *rich_editor = qobject_cast<RichEditor::RichEditorWidget *>(editorWidget());
//    if ( rich_editor )
//        documentManager->emitEditorClosed(rich_editor->editor());
}

void RichDocument::setTextCodec( QTextCodec *codec )
{
    Q_UNUSED(codec)
}

void RichDocument::saveState( QDomDocument *document, QDomElement *element ) const
{
    Q_UNUSED(document)
    element->setAttribute("WindowState", Utils::Hex::toHexString<QByteArray>(m_editorMainWidget->saveState()));
}

void RichDocument::restoreState( QDomElement *element )
{
    QString stateString = element->attribute("WindowState", "");
    if ( !stateString.isEmpty() )
        m_editorMainWidget->restoreState(Utils::Hex::fromHexString<QByteArray>(stateString));
}

void RichDocument::onModificationChanged()
{ DocumentManager::getInstance()->onDocumentModificationChanged(this); }

//////////////////////////////////////////////////////////////////////////////////////
/// RichEditorPlugin
RichEditorPlugin::RichEditorPlugin()
    : IDocumentPlugin()
{
    Core::MimeDatabase *mimeDatabase = Core::Storage::mainWindow()->mimeDatabase();
    QString errorMessage;
    mimeDatabase->addMimeTypes(QLatin1String(":/richeditor/richeditor.mimetypes.xml"), &errorMessage);
}

void RichEditorPlugin::initialize()
{
}

void RichEditorPlugin::updateEditorActions()
{
}

void RichEditorPlugin::fillEditMenu( QMenu *menu )
{
    Q_UNUSED(menu)
}

TextEditor::IAssistProvider *RichEditorPlugin::createQuickFixProvider()
{
    return 0;
}

QStringList RichEditorPlugin::settingsKeyList() const
{
    return QStringList();
}

ISettings *RichEditorPlugin::createSettings( const QString &settingsKey ) const
{
    Q_UNUSED(settingsKey)
    return 0;
}

RichEditorPlugin::~RichEditorPlugin()
{
}

bool RichEditorPlugin::acceptExtension( const QString &extension ) const
{
    QString ext = extension.toUpper();

    return ( ext == QLatin1String("HTM") || ext == QLatin1String("HTML") );
}

IDocument *RichEditorPlugin::createDocument()
{
    RichDocument *document = new RichDocument(this);
    document->onFormatSettingsChanged();
    return document;
}

#ifndef HAVE_QT5
Q_EXPORT_PLUGIN2(RichEditorPlugin, RichEditorPlugin)
#endif
