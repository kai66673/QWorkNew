#include "mimedatabase.h"

#include "Core.h"
#include "IPlugin.h"
#include "stringutils.h"

#include "RpmlEditorPlugin.h"
#include "ReportEditorWidget.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>

//////////////////////////////////////////////////////////////////////////////////////
/// RpmlDocument
RpmlDocument::RpmlDocument( IDocumentPlugin *docPlugin )
    : IDocument()
    , m_file(0)
    , m_editor(0)
    , m_fileName("")
    , m_documentPlugin(docPlugin)
{
    m_editorMainWidget = new QMainWindow();
    m_editor = new RPML::ReportDesignerWidget(m_editorMainWidget);
    connect(m_editor->document(), SIGNAL(modificationChanged(bool)), this, SLOT(onModificationChanged()));

    m_editorMainWidget->setCentralWidget(m_editor);
    m_editorMainWidget->addToolBar(m_editor->editorToolBar());
    m_editorMainWidget->addToolBar(m_editor->reportToolBar());
    m_editorMainWidget->addToolBar(m_editor->textToolBar());
    m_editorMainWidget->addToolBar(m_editor->paraToolBar());
    m_editorMainWidget->addToolBar(m_editor->tableToolBar());

    QDockWidget *dataSourcesDock = m_editor->dataSourcesDock(m_editorMainWidget);
    dataSourcesDock->setObjectName("RpmlEditor_DataSources");
    dataSourcesDock->setFeatures(QDockWidget::DockWidgetMovable);
    m_editorMainWidget->addDockWidget(Qt::RightDockWidgetArea, dataSourcesDock);

    QDockWidget *inputParametersDock = m_editor->inputParametersDock(m_editorMainWidget);
    inputParametersDock->setObjectName("RpmlEditor_InputParameters");
    inputParametersDock->setFeatures(QDockWidget::DockWidgetMovable);
    m_editorMainWidget->addDockWidget(Qt::RightDockWidgetArea, inputParametersDock);
}

RpmlDocument::~RpmlDocument()
{
    if ( m_file )
        delete m_file;
}

QWidget *RpmlDocument::editorWidget() const
{
    return m_editor;
}

QWidget *RpmlDocument::widget() const
{
    return m_editorMainWidget;
}

Core::IEditor *RpmlDocument::editor() const
{ return 0; }

bool RpmlDocument::newDocument ( const QString &fileName )
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

bool RpmlDocument::newDocumentFromFile( const QString &fileName, const QString &contentFilePath )
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

bool RpmlDocument::openDocument( const QString &filePath )
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

bool RpmlDocument::saveDocument()
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

bool RpmlDocument::saveAsDocument( const QString &filePath )
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

bool RpmlDocument::saveToDocument( const QString &filePath )
{
    QString errorMessage;
    bool writed = m_editor->save(filePath, errorMessage);
    if ( !writed )
        Core::Storage::warning(errorMessage);
    return writed;
}

bool RpmlDocument::renameTo(const QString &newFilePath)
{
    if ( m_file )
        delete m_file;

    m_file = new QFile(newFilePath);

    return true;
}

QString RpmlDocument::title() const
{
    if ( m_file ) {
        QFileInfo fi(*m_file);
        return fi.fileName();
    }

    return m_fileName;
}

QString RpmlDocument::toolTip() const
{
    if ( m_file ) {
        QFileInfo fi(*m_file);
        return fi.absoluteFilePath();
    }

    return QStringLiteral("");
}

QString RpmlDocument::filePath() const
{
    return toolTip();
}

bool RpmlDocument::isModified() const
{
    return m_editor->document()->isModified();
}

void RpmlDocument::setModified(bool onoff)
{
    m_editor->document()->setModified(onoff);
}

void RpmlDocument::setCursor( IDocumentCursor *position )
{
    if ( RichEditor::RichDocumentPosition *richPosition = static_cast<RichEditor::RichDocumentPosition *>(position) ) {
        QTextCursor c(m_editor->document());
        c.setPosition(richPosition->position());
        m_editor->setTextCursor(c);
    }
    editorWidget()->setFocus();
}

void RpmlDocument::onFormatSettingsChanged()
{ }

void RpmlDocument::emitDocumentOpened( DocumentManager *documentManager )
{
    Q_UNUSED(documentManager)
//    RichEditor::RichEditorWidget *rich_editor = qobject_cast<RichEditor::RichEditorWidget *>(editorWidget());
//    if ( rich_editor )
//        documentManager->emitEditorOpened(rich_editor->editor());
}

void RpmlDocument::emitDocumentClosed( DocumentManager *documentManager )
{
    Q_UNUSED(documentManager)
//    RichEditor::RichEditorWidget *rich_editor = qobject_cast<RichEditor::RichEditorWidget *>(editorWidget());
//    if ( rich_editor )
//        documentManager->emitEditorClosed(rich_editor->editor());
}

void RpmlDocument::setTextCodec( QTextCodec *codec )
{
    Q_UNUSED(codec)
}

void RpmlDocument::saveState( QDomDocument *document, QDomElement *element ) const
{
    Q_UNUSED(document)
    element->setAttribute("WindowGeometry", Utils::Hex::toHexString<QByteArray>(m_editorMainWidget->saveGeometry()));
    element->setAttribute("WindowState", Utils::Hex::toHexString<QByteArray>(m_editorMainWidget->saveState()));
}

void RpmlDocument::restoreState( QDomElement *element )
{
    QString geometryString = element->attribute("WindowGeometry", "");
    if ( !geometryString.isEmpty() )
        m_editorMainWidget->restoreGeometry(Utils::Hex::fromHexString<QByteArray>(geometryString));

    QString stateString = element->attribute("WindowState", "");
    if ( !stateString.isEmpty() )
        m_editorMainWidget->restoreState(Utils::Hex::fromHexString<QByteArray>(stateString));
}

void RpmlDocument::saveContentState( QDomDocument *document, QDomElement *element ) const
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

void RpmlDocument::restoreContentState( QDomElement *element )
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

void RpmlDocument::onModificationChanged()
{ DocumentManager::getInstance()->onDocumentModificationChanged(this); }

//////////////////////////////////////////////////////////////////////////////////////
/// RpmlEditorPlugin
RpmlEditorPlugin::RpmlEditorPlugin()
    : IDocumentPlugin()
{
    Core::MimeDatabase *mimeDatabase = Core::Storage::mainWindow()->mimeDatabase();
    QString errorMessage;
    mimeDatabase->addMimeTypes(QLatin1String(":/rpmleditor/rpmleditor.mimetypes.xml"), &errorMessage);
}

void RpmlEditorPlugin::initialize()
{
}

void RpmlEditorPlugin::updateEditorActions()
{
}

void RpmlEditorPlugin::fillEditMenu( QMenu *menu )
{
    Q_UNUSED(menu)
}

TextEditor::IAssistProvider *RpmlEditorPlugin::createQuickFixProvider()
{
    return 0;
}

QStringList RpmlEditorPlugin::settingsKeyList() const
{
    return QStringList();
}

ISettings *RpmlEditorPlugin::createSettings( const QString &settingsKey ) const
{
    Q_UNUSED(settingsKey)
    return 0;
}

RpmlEditorPlugin::~RpmlEditorPlugin()
{
}

bool RpmlEditorPlugin::acceptExtension( const QString &extension ) const
{
    QString ext = extension.toUpper();

    return ( ext == QLatin1String("RPML") );
}

IDocument *RpmlEditorPlugin::createDocument()
{
    RpmlDocument *document = new RpmlDocument(this);
    document->onFormatSettingsChanged();
    return document;
}

#ifndef HAVE_QT5
Q_EXPORT_PLUGIN2(RpmlEditorPlugin, RpmlEditorPlugin)
#endif
