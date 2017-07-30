#include "mimedatabase.h"

#include "Core.h"
#include "IPlugin.h"
#include "stringutils.h"

#include "RxmlEditorPlugin.h"
#include "ReportEditorWidget.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>

//////////////////////////////////////////////////////////////////////////////////////
/// RxmlDocument
RxmlDocument::RxmlDocument( IDocumentPlugin *docPlugin )
    : IDocument()
    , m_file(0)
    , m_editor(0)
    , m_fileName("")
    , m_documentPlugin(docPlugin)
{
    m_editorMainWidget = new QMainWindow();
    m_editor = new RPML::ReportEditorWidget(m_editorMainWidget);
    connect(m_editor->document(), SIGNAL(modificationChanged(bool)), this, SLOT(onModificationChanged()));

    m_editorMainWidget->setCentralWidget(m_editor);
    m_editorMainWidget->addToolBar(m_editor->editorToolBar());
    m_editorMainWidget->addToolBar(m_editor->printToolBar());
    m_editorMainWidget->addToolBar(m_editor->textToolBar());
    m_editorMainWidget->addToolBar(m_editor->paraToolBar());
    m_editorMainWidget->addToolBar(m_editor->tableToolBar());
}

RxmlDocument::~RxmlDocument()
{
    if ( m_file )
        delete m_file;
}

QWidget *RxmlDocument::editorWidget() const
{
    return m_editor;
}

QWidget *RxmlDocument::widget() const
{
    return m_editorMainWidget;
}

Core::IEditor *RxmlDocument::editor() const
{ return 0; }

bool RxmlDocument::newDocument ( const QString &fileName )
{
    if ( m_file ) {
        delete m_file;
        m_file = NULL;
    }

    m_editor->clearReport();

    m_fileName = fileName;
    m_editor->setFocus();
    m_editor->document()->setModified(false);

    return true;
}

bool RxmlDocument::newDocumentFromFile( const QString &fileName, const QString &contentFilePath )
{
    if ( m_file ) {
        delete m_file;
        m_file = NULL;
    }

    QString errorString;
    if ( !m_editor->load(contentFilePath, errorString) )
        return false;

    m_fileName = fileName;
    m_editor->setFocus();
    m_editor->document()->setModified(true);

    return true;
}

bool RxmlDocument::openDocument( const QString &filePath )
{
    QString errorMessage;
    bool readed = m_editor->load(filePath, errorMessage);

    if ( readed ) {
        if ( m_file )
            delete m_file;
        m_file = new QFile(filePath);
    }

    m_editor->document()->setModified(false);
    m_editor->setFocus();

    return readed;
}

bool RxmlDocument::saveDocument()
{
    if ( !m_file )
        return false;

    QString errorMessage;
    bool writed = m_editor->save(m_file->fileName(), errorMessage);

    if ( writed )
        m_editor->document()->setModified(false);
    else
        Core::Storage::warning(errorMessage);

    m_editor->setFocus();

    return writed;
}

bool RxmlDocument::saveAsDocument( const QString &filePath )
{
    QString errorMessage;
    bool writed = m_editor->save(filePath, errorMessage);

    if ( writed ) {
        if ( m_file )
            delete m_file;
        m_file = new QFile(filePath);

        m_editor->document()->setModified(false);
    }
    else
        Core::Storage::warning(errorMessage);

    m_editor->setFocus();

    return true;
}

bool RxmlDocument::saveToDocument( const QString &filePath )
{
    QString errorMessage;
    bool writed = m_editor->save(filePath, errorMessage);
    if ( !writed )
        Core::Storage::warning(errorMessage);
    return writed;
}

bool RxmlDocument::renameTo(const QString &newFilePath)
{
    if ( m_file )
        delete m_file;

    m_file = new QFile(newFilePath);

    return true;
}

QString RxmlDocument::title() const
{
    if ( m_file ) {
        QFileInfo fi(*m_file);
        return fi.fileName();
    }

    return m_fileName;
}

QString RxmlDocument::toolTip() const
{
    if ( m_file ) {
        QFileInfo fi(*m_file);
        return fi.absoluteFilePath();
    }

    return QStringLiteral("");
}

QString RxmlDocument::filePath() const
{
    return toolTip();
}

bool RxmlDocument::isModified() const
{
    return m_editor->document()->isModified();
}

void RxmlDocument::setModified(bool onoff)
{
    m_editor->document()->setModified(onoff);
}

void RxmlDocument::setCursor( IDocumentCursor *position )
{
    if ( RichEditor::RichDocumentPosition *richPosition = static_cast<RichEditor::RichDocumentPosition *>(position) ) {
        QTextCursor c(m_editor->document());
        c.setPosition(richPosition->position());
        m_editor->setTextCursor(c);
    }
    editorWidget()->setFocus();
}

void RxmlDocument::onFormatSettingsChanged()
{ }

void RxmlDocument::emitDocumentOpened( DocumentManager *documentManager )
{
    Q_UNUSED(documentManager)
//    RichEditor::RichEditorWidget *rich_editor = qobject_cast<RichEditor::RichEditorWidget *>(editorWidget());
//    if ( rich_editor )
//        documentManager->emitEditorOpened(rich_editor->editor());
}

void RxmlDocument::emitDocumentClosed( DocumentManager *documentManager )
{
    Q_UNUSED(documentManager)
//    RichEditor::RichEditorWidget *rich_editor = qobject_cast<RichEditor::RichEditorWidget *>(editorWidget());
//    if ( rich_editor )
//        documentManager->emitEditorClosed(rich_editor->editor());
}

void RxmlDocument::setTextCodec( QTextCodec *codec )
{
    Q_UNUSED(codec)
}

void RxmlDocument::saveState( QDomDocument *document, QDomElement *element ) const
{
    Q_UNUSED(document)
    element->setAttribute("WindowState", Utils::Hex::toHexString<QByteArray>(m_editorMainWidget->saveState()));
}

void RxmlDocument::restoreState( QDomElement *element )
{
    QString stateString = element->attribute("WindowState", "");
    if ( !stateString.isEmpty() )
        m_editorMainWidget->restoreState(Utils::Hex::fromHexString<QByteArray>(stateString));
}

void RxmlDocument::saveContentState( QDomDocument *document, QDomElement *element ) const
{
    Q_UNUSED(document)

    int pos = m_editor->textCursor().position();
    bool pageMode = m_editor->pageMode();
    QByteArray state;
    QDataStream stream(&state, QIODevice::WriteOnly);
    stream << 1;        // Version
    stream << pos;
    stream << pageMode;

    element->setAttribute("ContentState", Utils::Hex::toHexString<QByteArray>(state));
}

void RxmlDocument::restoreContentState( QDomElement *element )
{
    QString stateString = element->attribute("ContentState", "");
    if ( !stateString.isEmpty() ) {
        QByteArray state = Utils::Hex::fromHexString<QByteArray>(stateString);
        QDataStream stream(&state, QIODevice::ReadOnly);
        int version;
        int pos;
        bool pageMode;
        stream >> version;
        stream >> pos;
        stream >> pageMode;
        if ( version >= 1 ) {
            QTextCursor c = m_editor->textCursor();
            c.setPosition(pos);
            m_editor->setTextCursor(c);
            m_editor->setPageMode(pageMode);
        }
    }
}

void RxmlDocument::onModificationChanged()
{ DocumentManager::getInstance()->onDocumentModificationChanged(this); }

//////////////////////////////////////////////////////////////////////////////////////
/// RxmlEditorPlugin
RxmlEditorPlugin::RxmlEditorPlugin()
    : IDocumentPlugin()
{
    Core::MimeDatabase *mimeDatabase = Core::Storage::mainWindow()->mimeDatabase();
    QString errorMessage;
    mimeDatabase->addMimeTypes(QLatin1String(":/rxmleditor/rxmleditor.mimetypes.xml"), &errorMessage);
}

void RxmlEditorPlugin::initialize()
{
}

void RxmlEditorPlugin::updateEditorActions()
{
}

void RxmlEditorPlugin::fillEditMenu( QMenu *menu )
{
    Q_UNUSED(menu)
}

TextEditor::IAssistProvider *RxmlEditorPlugin::createQuickFixProvider()
{
    return 0;
}

QStringList RxmlEditorPlugin::settingsKeyList() const
{
    return QStringList();
}

ISettings *RxmlEditorPlugin::createSettings( const QString &settingsKey ) const
{
    Q_UNUSED(settingsKey)
    return 0;
}

RxmlEditorPlugin::~RxmlEditorPlugin()
{
}

bool RxmlEditorPlugin::acceptExtension( const QString &extension ) const
{
    QString ext = extension.toUpper();

    return ( ext == QLatin1String("RXML") );
}

IDocument *RxmlEditorPlugin::createDocument()
{
    RxmlDocument *document = new RxmlDocument(this);
    document->onFormatSettingsChanged();
    return document;
}

#ifndef HAVE_QT5
Q_EXPORT_PLUGIN2(RxmlEditorPlugin, RxmlEditorPlugin)
#endif
