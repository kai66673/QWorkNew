#include <QFileInfo>
#include <QFileDialog>
#include <QApplication>
#include <QTimer>
#include <QPlainTextEdit>
#include <QDomDocument>
#include <QMenu>
#include <QToolBar>

#include "qtcassert.h"
#include "IDocument.h"
#include "ITextDocument.h"
#include "CommandManager.h"
#include "LogManager.h"
#include "ModeManager.h"
#include "HighlighterFormatsManager.h"

#ifdef Q_OS_WIN
#define TO_UPPER_OPT(fileName) ((fileName).toUpper())
#else
#define TO_UPPER_OPT(fileName) (fileName)
#endif

IDocumentFactory::IDocumentFactory()
{ }

IDocumentFactory::~IDocumentFactory()
{
    qDeleteAll(m_documentPlugins);
    m_documentPlugins.clear();
}

IDocument *IDocumentFactory::createDocument( const QString &extension )
{
    foreach ( IDocumentPlugin *plugin, m_documentPlugins )
        if ( plugin->acceptExtension(extension) )
            return plugin->doCreateDocument();

    return 0;
}

IDocument *IDocumentFactory::createDocumentByFileName(const QString &filePath, QString &realFilePath, QString &errorMessage)
{
    QFileInfo fileInfo(filePath);
    if ( !fileInfo.exists() ) {
        errorMessage = QObject::tr("File '%1' not Exists").arg(filePath);
        return 0;
    }

    QString fName;
#ifndef Q_OS_WIN
    realFilePath = fileInfo.canonicalFilePath();
    fName = fileInfo.fileName();
#else
    realFilePath = fileInfo.canonicalPath();
    QString candidate = fileInfo.fileName();
    QDir fileDir(realFilePath);
    foreach( const QString &f, fileDir.entryList(QDir::Files) )
        if ( !candidate.compare(f, Qt::CaseInsensitive) ) {
            realFilePath += "/" + f;
            fName = f;
            break;
        }
#endif

    foreach ( IDocumentPlugin *documentPlugin, m_documentPlugins )
        if ( documentPlugin->acceptFileName(fName) )
            return documentPlugin->doCreateDocument();

    return createDocument(fileInfo.suffix());
}

void IDocumentFactory::initializeDocumentPlugins()
{
    foreach ( IDocumentPlugin *documentPlugin, m_documentPlugins ) {
        documentPlugin->initialize();
        foreach ( HighlighterFormatData *data, documentPlugin->createHighlightersFormatData() )
            HighlighterFormatsManager::registerLanguageHighlighter(data);
    }
}

QList<UiProviderInterface *> IDocumentFactory::createUiProviders( IMainWindow *mainWindow )
{
    QList<UiProviderInterface *> result;
    foreach ( IDocumentPlugin *plugin, m_documentPlugins )
        result << plugin->createUiProviders(mainWindow);
    return result;
}

IDocument::IDocument()
    : m_enterEventBlocked(false)
{ }

IDocument::~IDocument()
{ }

const QString IDocument::iconName( bool isModified ) const
{
    return ( isModified ? ":/images/document_modified.png" : ":/images/document_ok.png");
}

void IDocument::saveState( QDomDocument *document, QDomElement *element ) const
{
    Q_UNUSED(document)
    Q_UNUSED(element)
}

void IDocument::restoreState( QDomElement *element )
{
    Q_UNUSED(element)
}

void IDocument::saveContentState( QDomDocument *document, QDomElement *element ) const
{
    if ( Core::IEditor *ed = editor() )
        ed->saveContentState(document, element);
}

void IDocument::restoreContentState( QDomElement *element )
{
    if ( Core::IEditor *ed = editor() )
        ed->restoreContentState(element);
}

IDocumentCursor *IDocument::startDocumentPosition() const
{
    return new TextDocumentCursor(0, 0);
}

void IDocument::refreshDocumentCursor()
{
    TextDocumentCursor textCursor;
    setCursor(&textCursor);
}

void IDocument::addHolder(const QString &holderId, const QString &holdReason)
{
    if ( m_holders.find(holderId) != m_holders.end() )
        (m_holders[holderId].first)++;
    else
        m_holders[holderId] = qMakePair(1, holdReason);
}

void IDocument::removeHolder( const QString &holderId )
{
    if ( m_holders.find(holderId) != m_holders.end() ) {
        if ( m_holders[holderId].first > 1 )
            (m_holders[holderId].first)--;
        else
            m_holders.remove(holderId);
    }
}

QString IDocument::holdReasons( const QString &splitter ) const
{
    QStringList reasons;
    QList <QPair<int, QString> > r = m_holders.values();
    for (int i = 0; i < r.size(); i++)
        reasons << r[i].second;
    return reasons.join(splitter);
}

bool IDocument::eventFilter(QObject *obj, QEvent *event)
{
    if ( event->type() == QEvent::Enter && !m_enterEventBlocked ) {
        m_enterEventBlocked = true;
        if ( documentPlugin()->activateDocumentOnMouseEnterEvent() )
            DocumentManager::getInstance()->activateDocument(this, false);
        m_enterEventBlocked = false;
        return true;
    }
    return QObject::eventFilter(obj, event);
}

IDocumentPlugin::IDocumentPlugin()
    : m_quickFixProvider(0)
{
}

IDocumentPlugin::~IDocumentPlugin()
{
    if ( m_quickFixProvider )
        delete m_quickFixProvider;
}

IDocument *IDocumentPlugin::doCreateDocument()
{
    if ( IDocument *document = createDocument() ) {
        if ( QWidget *editor = document->editorWidget() )
            editor->installEventFilter(Core::CommandManager::getInstance());
        if ( QWidget *w = document->widget() )
            w->installEventFilter(document);
        return document;
    }

    return 0;
}

TextEditor::IAssistProvider *IDocumentPlugin::quickFixProvider()
{
    if ( !m_quickFixProvider )
        m_quickFixProvider = createQuickFixProvider();
    return m_quickFixProvider;
}

Core::IMode *IDocumentPlugin::mode() const
{
    return Core::ModeManager::editMode();
}

bool IDocumentPlugin::activateDocumentOnMouseEnterEvent() const
{
    return false;
}

IDocumentContainer::IDocumentContainer( IMainWindow *mainWindow, QObject *parent )
    : QObject(parent)
    , m_widget(0)
    , m_activeDocumentToolBar(0)
    , m_mainWindow(mainWindow)
{
    m_openEditorsWindow = new OpenEditorsWindow(m_mainWindow->widget());
    connect(this, SIGNAL(currentDocumentChanged(IDocument*)), this, SLOT(onDocumentActivated(IDocument*)));
}

IDocumentContainer::~IDocumentContainer()
{
    qDeleteAll(m_documents.values());
}

void IDocumentContainer::addDocument( IDocument *document )
{
    QWidget *w = document->widget();
    QWidget *e = document->editorWidget();
    m_documents[w] = document;
    m_editorDocuments[e] = document;
    addWidget(w);
    setCurrentWidget(w);
    e->installEventFilter(this);
}

void IDocumentContainer::closeDocument( IDocument *document )
{
    QWidget *w = document->widget();
    removeWidget(w);
    m_documents.remove(w);
    m_editorDocuments.remove(document->editorWidget());
    m_docHistory.removeAll(document);
    if ( m_activeDocumentToolBar == document->toolBar() )
        m_activeDocumentToolBar = 0;
    delete document;
}

void IDocumentContainer::setCurrentDocument( IDocument *document )
{
    setCurrentWidget(document->widget());
    emit currentDocumentChanged(document);
}

IDocument *IDocumentContainer::currentDocument() const
{
    if ( m_docHistory.isEmpty() )
        return 0;

    return m_docHistory.first();
}

IDocument *IDocumentContainer::findDocument( const QString &fileName ) const
{
    QFileInfo fi(fileName);
    QString fName = fi.canonicalFilePath();
    if ( fName.isEmpty() )      // Maybe Deleted!
        fName = fileName;

    foreach( IDocument *document, m_documents.values() ) {
#ifdef Q_OS_WIN
        if ( !document->filePath().compare(fName, Qt::CaseInsensitive) )
            return document;
#else
        if ( document->filePath() == fName )
            return document;
#endif
    }

    return 0;
}

QList<IDocument *> IDocumentContainer::documents() const
{
    return m_documents.values();
}

QList<QAction *> IDocumentContainer::createWindowsActions()
{
    QList<QAction *> actions;

    QMainWindow *mainWindow = m_mainWindow->widget();

    m_cmdNextWindow     = new Core::Command( tr("NextWindow"),
                                             QIcon(":/images/win_next.png"),
                                             tr("&Next Window"),
                                             tr("Goto/Activate Next Document"),
                                             QKeySequence("Ctrl+Tab"),
                                             mainWindow );

    m_cmdPreviousWindow = new Core::Command( tr("PreviousWindow"),
                                             QIcon(":/images/win_prev.png"),
                                             tr("&Previous Window"),
                                             tr("Goto/Activate Previous Document"),
                                             QKeySequence("Ctrl+Shift+Tab"),
                                             mainWindow );

    /* No Settings Enabled!
    Core::QCommandManager::getInstance()->registerCommands( tr("WindowsCommands"),
                                                            QList<Core::Command *>() << m_cmdNextWindow
                                                                                     << m_cmdPreviousWindow ); */

    connect(m_cmdNextWindow->action(),      SIGNAL(triggered()), this, SLOT(gotoNextWindow()));
    connect(m_cmdPreviousWindow->action(),  SIGNAL(triggered()), this, SLOT(gotoPreviousWindow()));

    actions << m_cmdNextWindow->action()
            << m_cmdPreviousWindow->action()
            << createAdditionalWindowsActions();

    return actions;
}

QList<IDocument *> IDocumentContainer::documentsByActivationHistory()
{
    return m_docHistory;
}

QList<QAction *> IDocumentContainer::createAdditionalWindowsActions()
{
    return QList<QAction *>();
}

void IDocumentContainer::onDocumentActivated( IDocument *document )
{
    if ( document ) {
        Core::ModeManager::switchToMode(document->documentPlugin()->mode());
        m_docHistory.removeAll(document);
        m_docHistory.prepend(document);
    } else {
        Core::ModeManager::switchToMode(0);
    }
}

void IDocumentContainer::gotoPreviousWindow()
{
    if ( m_documents.size() < 2 )
        return;

    if (m_openEditorsWindow->isVisible()) {
        m_openEditorsWindow->selectNextEditor();
    } else {
        m_openEditorsWindow->setEditors(documentsByActivationHistory());
        m_openEditorsWindow->selectNextEditor();
        showPopupOrSelectDocument();
    }
}

bool IDocumentContainer::eventFilter(QObject *obj, QEvent *ev)
{
    if ( ev->type() == QEvent::FocusIn ) {
        if ( QWidget *w = qobject_cast<QWidget *>(obj) ) {
            QHash <QWidget *, IDocument *>::const_iterator it = m_editorDocuments.find(w);
            if ( it != m_editorDocuments.end() ) {
                IDocument *document = it.value();
                if ( m_activeDocumentToolBar != document->toolBar() ) {
                    if ( m_activeDocumentToolBar )
                        m_activeDocumentToolBar->setEnabled(false);
                    m_activeDocumentToolBar = document->toolBar();
                    if ( m_activeDocumentToolBar )
                        m_activeDocumentToolBar->setEnabled(true);
                }
                onDocumentActivated(document);
            }
        }
    }

    return QObject::eventFilter(obj, ev);
}

void IDocumentContainer::gotoNextWindow()
{
    if ( m_documents.size() < 2 )
        return;

    if (m_openEditorsWindow->isVisible()) {
        m_openEditorsWindow->selectPreviousEditor();
    } else {
        m_openEditorsWindow->setEditors(documentsByActivationHistory());
        m_openEditorsWindow->selectPreviousEditor();
        showPopupOrSelectDocument();
    }
}

void IDocumentContainer::showPopupOrSelectDocument() const
{
    if (QApplication::keyboardModifiers() == Qt::NoModifier) {
        m_openEditorsWindow->selectAndHide();
    } else {
        QMainWindow *mainWindow = m_mainWindow->widget();
        const QPoint p = mainWindow->mapToGlobal(QPoint(0, 0));
        m_openEditorsWindow->move((mainWindow->width() - m_openEditorsWindow->width()) / 2 + p.x(),
                            (mainWindow->height() - m_openEditorsWindow->height()) / 2 + p.y());
        m_openEditorsWindow->setVisible(true);
    }
}

DocumentManager *DocumentManager::documentManager = 0;

DocumentManager::DocumentManager( IMainWindow *mainWindow, IDocumentFactory *documentFactory, IDocumentContainer *documentContainer )
    : QObject(0)
    , ISessionClient()
    , ICloseApplicationClient()
    , ISettingsClient()
    , m_mainWindow(mainWindow)
    , m_documentFactory(documentFactory)
    , m_container(documentContainer)
    , m_watcher(new QFileSystemWatcher())
    , m_blockActivatedSignal(false)
    , m_lastMenuDocumentPlugin(0)
    , m_untitledCounter(0)
{
    documentManager = this;

    connect(m_mainWindow->widget(), SIGNAL(windowActivated()), this, SLOT(onMainWindowActivated()));
    connect(m_watcher, SIGNAL(fileChanged(QString)), this, SLOT(onFileChangedOutside(QString)));

    connect(m_container, SIGNAL(currentDocumentChanged(IDocument*)), this, SLOT(onCurrentEditorChanged(IDocument*)));
    connect(m_container, SIGNAL(closeDocumentRequested(IDocument*)), this, SLOT(onCloseDocumentRequested(IDocument*)));

    connect(this, SIGNAL(documentSaved(QString)), this, SLOT(onDocumentSaved(QString)));
}

DocumentManager::~DocumentManager()
{
    delete m_documentFactory;
    delete m_container;
    delete m_watcher;
}

void DocumentManager::loadDocumentPlugins()
{
    m_documentFactory->loadDocumentPlugins();
}

IDocument *DocumentManager::currentDocument()
{
    return m_container->currentDocument();
}

IDocumentPlugin *DocumentManager::currentDocumentPlugin()
{
    if ( IDocument *document = m_container->currentDocument() )
        return document->documentPlugin();

    return 0;
}

QString DocumentManager::selectedText()
{
    if ( IDocument *document = currentDocument() )
        if ( ITextDocument *textDocument = dynamic_cast<ITextDocument *>(document) )
            return textDocument->selectedText();

    return QStringLiteral("");
}

int DocumentManager::documentsCount()
{
    return m_container->documents().size();
}

IDocument *DocumentManager::findDocument(const QString &filePath)
{
    return m_container->findDocument(filePath);
}

bool DocumentManager::closeDocument( IDocument *document, bool confirmSaving )
{
    return tryCloseFile(document, !confirmSaving);
}

bool DocumentManager::newDocument( const QString &extension )
{
    IDocument *document = m_documentFactory->createDocument(extension);
    if ( !document )
        return false;

    QString fileName = createUntitledBaseName();
    if ( extension != "" )
        fileName += "." + extension;

    if ( !document->newDocument(fileName) ) {
        delete document;
        return false;
    }

    document->emitDocumentOpened(this);

    addDocumentInternal(document);

    return true;
}

QMap<QString, QString> DocumentManager::openedEditorsContents()
{
    QMap<QString, QString> result;

    QList<IDocument *> documents = m_container->documents();
    foreach ( IDocument *document, documents ) {
        if ( !document->isNew() && document->isModified() ) {
           if ( QPlainTextEdit *editorWidget = qobject_cast <QPlainTextEdit *> (document->editorWidget()) ) {
               result[document->filePath()] = editorWidget->toPlainText();
           }
        }
    }

    return result;
}

QList<QAction *> DocumentManager::createWindowsActions()
{
    return m_container->createWindowsActions();
}

QList<UiProviderInterface *> DocumentManager::createUiProviders( IMainWindow *mainWindow )
{
    return m_documentFactory->createUiProviders(mainWindow);
}

void DocumentManager::expectFileChanges(const QString &fileName)
{
    m_watcher->removePath(fileName);
}

void DocumentManager::unexpectFileChanges(const QString &fileName, bool reload)
{
    m_watcher->addPath(fileName);

    if ( reload ) {
        if ( IDocument *document = m_container->findDocument(fileName) ) {
            document->openDocument(fileName);
            m_container->documentChanged(document);
            m_container->updateDocumentIcon(document);
        }
    }
}

void DocumentManager::registerProjectFileWatcher(const QString &fileName, IChangeProjectFileWatcher *watcher)
{
    m_projectFiles[TO_UPPER_OPT(fileName)] = watcher;
    m_watcher->addPath(fileName);
}

void DocumentManager::deregisterProjectFileWatcher(const QString &fileName)
{
    m_projectFiles.remove(TO_UPPER_OPT(fileName));
    if ( !findDocument(fileName) )
        m_watcher->removePath(fileName);
}

void DocumentManager::newFile()
{
    QString extension;
    IDocument *document = m_documentFactory->newDocument(extension);

    if ( !document )
        return;

    QString fileName = createUntitledBaseName();
    if ( extension != "" )
        fileName += "." + extension;

    if ( !document->newDocument(fileName) ) {
        delete document;
        return;
    }

    document->emitDocumentOpened(this);

    addDocumentInternal(document);
}

bool DocumentManager::activateDocument(IDocument *document , bool moveMousePosition)
{
    m_container->setCurrentDocument(document);
    if ( moveMousePosition ) {
        if ( QWidget *w = document->editorWidget() ) {
            QCursor c = w->cursor();
            c.setPos(w->mapToGlobal(QPoint(w->width()/2, w->height()/2)));
            w->setCursor(c);
        }
    }
    document->refreshDocumentCursor();
    return true;
}

QList<IDocument *> DocumentManager::documentsOrdered() const
{
    return m_container->documentsOrdered();
}

IDocument *DocumentManager::openDocument( const QString &filePath, IDocumentCursor *position, QString *errorMessage, bool addRecentFile )
{
    QString errorText;
    IDocument *document = openDocumentInternal(filePath, errorText, position, addRecentFile);

    if ( position )
        delete position;

    if ( !document ) {
        if ( errorMessage )
            *errorMessage = errorText;
        else
            QMessageBox::warning( m_mainWindow->widget(), m_mainWindow->appName(), errorText);
    }

    return document;
}

IDocument *DocumentManager::openDocumentInternal( const QString &filePath, QString &errorMessage, IDocumentCursor *position, bool addRecentFile )
{
    if ( IDocument *document = m_container->findDocument(filePath) ) {
        m_container->setCurrentDocument(document);
        document->editorWidget()->setFocus();
        if ( position )
            document->setCursor(position);
        else
            document->refreshDocumentCursor();
        return document;
    }

    QString realFilePath;
    IDocument *document = m_documentFactory->createDocumentByFileName(filePath, realFilePath, errorMessage);

    if ( !document ) {
        errorMessage = tr("Document Factory Internal Error on Creating Document for File '%1'").arg(filePath);
        return 0;
    }

    if ( !document->openDocument(realFilePath) ) {
        delete document;
        errorMessage = tr("Internal Error on Loading Content of File '%1'").arg(filePath);
        return 0;
    }

    document->emitDocumentOpened(this);

    addDocumentInternal(document, position);

    if ( document->editor() ) {
        m_mainWindow->cutForwardNavigationHistory();
        m_mainWindow->addCurrentPositionToNavigationHistory();
    }

    m_watcher->addPath(realFilePath);

    if ( addRecentFile )
        emit addToRecentFiles(realFilePath);

    return document;
}

void DocumentManager::addDocumentInternal( IDocument *document, IDocumentCursor *position )
{
    bool needDeletePosition = false;
    if ( !position ) {
        position = document->startDocumentPosition();
        needDeletePosition = true;
    }

    m_container->addDocument(document);
    m_container->updateDocumentIcon(document);
    document->setCursor(position);

    if ( needDeletePosition )
        delete position;
}

QMenu *DocumentManager::editMenu()
{
    return m_mainWindow->editMenu();
}

void DocumentManager::fontSizeIncrease()
{
    SettingsManager *settingsManager = SettingsManager::instance();
    int zoom = settingsManager->value("000_TextEditor", "Zoom").toInt();
    zoom += 10;
    if ( zoom > 400 )
        zoom = 400;
    settingsManager->value("000_TextEditor", "Zoom") = zoom;

    onFormatSettingsChanged();
}

void DocumentManager::fontSizeDecrease()
{
    SettingsManager *settingsManager = SettingsManager::instance();
    int zoom = settingsManager->value("000_TextEditor", "Zoom").toInt();
    zoom -= 10;
    if ( zoom < 20 )
        zoom = 20;
    settingsManager->value("000_TextEditor", "Zoom") = zoom;

    onFormatSettingsChanged();
}

bool DocumentManager::isCurrentDocumentUntitled()
{
    IDocument *document = currentDocument();
    if ( !document )
        return false;

    return document->isNew();
}

QList <Core::IEditor *> DocumentManager::openedEditors()
{
    QList <Core::IEditor *> editors;

    foreach ( IDocument *document, m_container->documents() )
        if ( Core::IEditor *editor = document->editor() )
            editors << editor;

    return editors;
}

void DocumentManager::emitEditorOpened( Core::IEditor *editor )
{
    emit(editorOpened(editor));
}

void DocumentManager::emitEditorClosed( Core::IEditor *editor )
{
    emit(editorClosed(editor));
}

void DocumentManager::openFile()
{
    QString filePath = QFileDialog::getOpenFileName(m_mainWindow->widget());
    if ( filePath.isEmpty() )
        return;

    openDocument(filePath);
}

Core::IEditor *DocumentManager::editorForFileName( const QString &fileName )
{
    foreach( IDocument *doc, m_container->documents() ) {
        if ( doc->filePath() == fileName )
            return doc->editor();
    }

    return 0;
}

Core::IEditor *DocumentManager::currentEditor()
{
    if ( IDocument *document = m_container->currentDocument() )
        return document->editor();

    return 0;
}

QWidget *DocumentManager::currentEditorWidget()
{
    if ( IDocument *document = m_container->currentDocument() )
        return document->editorWidget();

    return 0;
}

bool DocumentManager::hasUnsavedDocuments() const
{
    foreach ( IDocument *document, m_container->documents() )
        if ( document->isModified() || document->isNew() )
            return true;

    return false;
}

void DocumentManager::saveFile()
{
    trySaveDocument();
}

void DocumentManager::saveAsFile()
{
    IDocument *document = m_container->currentDocument();
    if ( !document )
        return;

    QString oldFileName = "";
    if ( !document->isNew() )
        oldFileName = document->filePath();

    QString newFileName = QFileDialog::getSaveFileName(m_mainWindow->widget());
    if ( newFileName.isEmpty() )
        return;
    if ( !document->saveAsDocument(newFileName) )
        return;
    m_container->documentChanged(document);
    m_container->updateDocumentIcon(document);

    emit(documentSaved(newFileName));

    if ( oldFileName != "" )
        m_watcher->removePath(oldFileName);

    m_watcher->addPath(newFileName);

    emit addToRecentFiles(newFileName);
}

void DocumentManager::saveAllFiles()
{
    foreach ( IDocument *document, m_container->documents() )
        if ( !trySaveDocument(document) )
            break;
}

void DocumentManager::closeFile()
{
    tryCloseFile();
    if ( IDocument *document = currentDocument() )
        document->refreshDocumentCursor();
}

void DocumentManager::closeAllFiles()
{
    tryCloseAllFiles();
    if ( IDocument *document = currentDocument() )
        document->refreshDocumentCursor();
}

void DocumentManager::insertText( const QString &text, const QString &extension )
{
    IDocument *document = m_container->currentDocument();
    bool bNeedNewDocument = false;
    if ( !document ) {
        document = m_documentFactory->createDocument(extension);
        bNeedNewDocument = true;
    }
    else {
        if ( !(document->title().endsWith("." + extension, Qt::CaseInsensitive)) ) {
            document = m_documentFactory->createDocument(extension);
            bNeedNewDocument = true;
        }
    }

    if ( !document )
        return;

    if ( bNeedNewDocument ) {
        QString fileName = createUntitledBaseName();
        if ( extension != "" )
            fileName += "." + extension;

        if ( !document->newDocument(fileName) ) {
            delete document;
            return;
        }

        m_container->addDocument(document);
    }

    if ( ITextDocument *textDocument = dynamic_cast<ITextDocument *>(document) )
        textDocument->insertText(text);

    if ( bNeedNewDocument )
        document->emitDocumentOpened(this);

    m_container->updateDocumentIcon(document);
}

void DocumentManager::onDocumentModificationChanged( IDocument *document )
{
    m_container->updateDocumentIcon(document);
}

void DocumentManager::onDocumentSaved(const QString &fileName)
{
    QMap<QString, IChangeProjectFileWatcher *>::const_iterator it = m_projectFiles.find(TO_UPPER_OPT(fileName));
    if ( it != m_projectFiles.end() )
        it.value()->handleChanges();
}

void DocumentManager::onFormatSettingsChanged()
{
    foreach ( IDocument *document, m_container->documents() )
        document->onFormatSettingsChanged();
}

void DocumentManager::onMainWindowActivated()
{
    QTimer::singleShot(0, this, SLOT(checkForReload()));
}

void DocumentManager::checkForReload()
{
    if ( m_blockActivatedSignal )
        return;

    m_blockActivatedSignal = true; {

        QMainWindow *mainWidget = m_mainWindow->widget();

        foreach ( const QString &chFile, m_changedFiles ) {
            if ( IDocument *document = m_container->findDocument(chFile) ) {
                if ( QMessageBox::warning( mainWidget,
                                           m_mainWindow->appName(),
                                           tr("File \'\'%1 has changed outside. Reload?").arg(chFile),
                                           QMessageBox::Yes | QMessageBox::No ) == QMessageBox::Yes ) {
                    document->openDocument(chFile);
                    m_container->documentChanged(document);
                } else {
                    document->setModified(true);
                }
                m_container->updateDocumentIcon(document);
            }
            emit(documentSaved(chFile));
        }

        foreach ( const QString &delFile, m_deletedFiles ) {
            if ( IDocument *document = m_container->findDocument(delFile) ) {
                if ( QMessageBox::warning( mainWidget,
                                           m_mainWindow->appName(),
                                           tr("File \'%1\' has deleted outside. Close (Yes) / Keep (No)").arg(delFile),
                                           QMessageBox::Yes | QMessageBox::No ) == QMessageBox::Yes ) {
                    tryCloseFile(document, true);
                }
            }
        }

        m_changedFiles.clear();
        m_deletedFiles.clear();

    } m_blockActivatedSignal = false;
}

void DocumentManager::onFileChangedOutside( const QString &filePath )
{
    QFileInfo fi(filePath);

    if ( fi.exists() )
        m_changedFiles.insert(filePath);
    else {
        m_deletedFiles.insert(filePath);
        m_changedFiles.remove(filePath);
    }
}

void DocumentManager::onCurrentEditorChanged( IDocument *document )
{
    emit currentEditorChanged(document ? document->editorWidget() : 0);

    emit currentDocumentChanged(document);

    if ( document )
        document->editorWidget()->setFocus();
}

void DocumentManager::onCursorPositionChanged()
{
    if ( IDocument *document = currentDocument() )
        emit(currentDocumentPositionChanged(document));
}

void DocumentManager::onCloseDocumentRequested( IDocument *document )
{
    tryCloseFile(document);
}

void DocumentManager::updateEditMenu()
{
    QMenu *editMenu = m_mainWindow->editMenu();

    IDocumentPlugin *plugin = currentDocumentPlugin();
    if ( plugin != m_lastMenuDocumentPlugin ) {
        m_lastMenuDocumentPlugin = plugin;
        editMenu->clear();
        if ( m_lastMenuDocumentPlugin )
            m_lastMenuDocumentPlugin->fillEditMenu(editMenu);
    }

    if ( m_lastMenuDocumentPlugin )
        m_lastMenuDocumentPlugin->updateEditorActions();
}

void DocumentManager::onDocumentRenamed( const QString &oldFileName, const QString &newFileName )
{
#ifdef Q_OS_WIN
    Qt::CaseSensitivity cs = Qt::CaseInsensitive;
#else
    Qt::CaseSensitivity cs = Qt::CaseSensitive;
#endif

    if ( !oldFileName.compare(newFileName, cs) )
        return;

    if ( IDocument *document = findDocument(oldFileName) ) {
        if ( document->renameTo(newFileName) ) {
            m_container->documentChanged(document);
            m_container->updateDocumentIcon(document);

            if ( oldFileName != "" )
                m_watcher->removePath(oldFileName);

            m_watcher->addPath(newFileName);
        }
    }
}

bool DocumentManager::tryCloseFile( IDocument *document, bool forceClose )
{
    if ( !document )
        document = m_container->currentDocument();
    if ( !document )
        return true;

    if ( document->hasHolders() ) {
        m_mainWindow->showWarning( tr("Dociment cannot be Closed:\n%1")
                                   .arg(document->holdReasons("\n")) );
        return false;
    }

    if ( !forceClose && document->isModified() ) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning( m_mainWindow->widget(),
                                    m_mainWindow->appName(),
                                    tr("File '%1' has been modified.\nDo you want to save changes").arg(document->title()),
                                    QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if ( ret != QMessageBox::Discard ) {
            if ( ret == QMessageBox::Save ) {
                if ( !trySaveDocument(document) )
                    return false;
            }
            else
                return false;
        }
    }

    if ( !document->filePath().isEmpty() )
        m_watcher->removePath(document->filePath());

    document->emitDocumentClosed(this);
    emit(documentClosed(document));

    m_container->closeDocument(document);

    return true;
}

bool DocumentManager::tryCloseAllFiles( bool forceClose )
{
    foreach ( IDocument *document, m_container->documents() )
        if ( !tryCloseFile(document, forceClose) )
            return false;

    return true;
}

DocumentManager *DocumentManager::getInstance()
{
    return documentManager;
}

bool DocumentManager::trySaveDocument( IDocument *document )
{
    if ( !document )
        document = m_container->currentDocument();
    if ( !document )
        return true;

    if ( !document->fixOnSave() )
        return false;

    if ( document->isModified() ) {
        if ( !document->isNew() ) {
            m_watcher->removePath(document->filePath());
            bool bSaved = document->saveDocument();
            m_watcher->addPath(document->filePath());
            if ( !bSaved )
                return false;
        }
        else {
            // Document is new - file name should be specified
            QString newFileName = QFileDialog::getSaveFileName(m_mainWindow->widget());
            if ( newFileName.isEmpty() )
                return false;
            if ( !document->saveAsDocument(newFileName) )
                return false;
            m_watcher->addPath(newFileName);
            m_container->documentChanged(document);
            emit addToRecentFiles(newFileName);
        }
    }
    m_container->updateDocumentIcon(document);
    emit(documentSaved(document->filePath()));

    return true;
}

QString DocumentManager::createUntitledBaseName()
{
    return QString("UNTITLED%1").arg(++m_untitledCounter);
}

void DocumentManager::saveTemporaryNewDocument( IDocument *document, const QString &location )
{
    QTC_ASSERT( document && document->isNew(), return );
    QString dirName = location + "/.newfiles";

    QDir dir(dirName);
    if ( !dir.exists() ) {
        if ( !dir.mkpath(dirName) ) {
            Core::LogManager::addMessage( Core::LogMessage(tr("Directory %1 Cannot Be Created").arg(dirName)) );
            return;
        }
    }

    if ( !document->saveToDocument(dirName + "/" + document->title()) )
        Core::LogManager::addMessage( Core::LogMessage(tr("Tempary New File %1 Cannot be Saved").arg(dirName + "/" + document->title())) );
}

bool DocumentManager::loadTemporaryNewDocument( const QString &fileName, const QString &location )
{
    QString filePath = location + "/.newfiles/" + fileName;

    QString extension = "";
    for (int j = fileName.size() - 1; j >= 0; j--) {
        if ( fileName[j] == '.' )
            break;
        extension.prepend(fileName[j]);
    }

    IDocument *document = m_documentFactory->createDocument(extension);
    if ( !document )
        return false;

    if ( !document->newDocumentFromFile(fileName, filePath) ) {
        delete document;
        return false;
    }

    document->emitDocumentOpened(this);

    addDocumentInternal(document);

    {
        int counter = 0;
        for (int i = 8; i < fileName.size(); i++) {
            char ch = fileName[i].toLatin1();
            if ( ch < '0' || ch > '9' )
                break;
            counter = 10 * counter + ch - '0';
        }
        if ( m_untitledCounter < counter )
            m_untitledCounter = counter;
    }

    return true;
}

bool DocumentManager::tryConfirmSaveAllDocuments( bool bSessionClose )
{
    foreach ( IDocument *document, m_container->documents() ) {
        if ( bSessionClose && document->isNew() )
            continue;
        if ( document->isModified() ) {
            QMessageBox::StandardButton ret;
            ret = QMessageBox::warning( m_mainWindow->widget(),
                                        m_mainWindow->appName(),
                                        tr("File '%1' has been modified.\nDo you want to save changes").arg(document->title()),
                                        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel );

            if ( ret != QMessageBox::Discard ) {
                if ( ret == QMessageBox::Save ) {
                    if ( !trySaveDocument(document) )
                        return false;
                }
                else
                    return false;
            }
        }
    }

    return true;
}

bool DocumentManager::trySaveAllDocuments()
{
    foreach ( IDocument *document, m_container->documents() )
        if ( document->isModified() )
            if ( !trySaveDocument(document) )
                return false;

    return true;
}

bool DocumentManager::silentSaveAllDocuments()
{
    foreach ( IDocument *document, m_container->documents() ) {
        if ( document->isModified() && !document->isNew() ) {
            m_watcher->removePath(document->filePath());
            bool bSaved = document->saveDocument();
            m_watcher->addPath(document->filePath());
            if ( !bSaved )
                return false;
        }
    }

    return true;
}

bool DocumentManager::sessionAboutToClose()
{
    return tryConfirmSaveAllDocuments(true);
}

void DocumentManager::sessionClose()
{
     tryCloseAllFiles(true);
}

void DocumentManager::sendCommandToDocuments( const QString &command )
{
    foreach( IDocument *document, m_container->documents() )
        document->onCommand(command);
}

void DocumentManager::initializeDocumentPlugins()
{
    m_documentFactory->initializeDocumentPlugins();
}

void DocumentManager::saveSession( const QString &location )
{
    QString fileName = location;
    fileName += "/documents.xml";

    QFile file(fileName);
    if ( !file.open(QFile::WriteOnly | QFile::Text) ) {
        m_mainWindow->showWarning( tr("Error On Save Session Documents\nFile \"%1\" Cannot be Opened for Writing:\n%2.")
                                   .arg(fileName).arg(file.errorString()) );
        return;
    }

    QDomDocument domDocument("documents");

    domDocument.appendChild(domDocument.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"windows-1251\""));

    QDomElement root = domDocument.createElement("documents");
    root.setAttribute("version", "1.0");
    domDocument.appendChild(root);

    IDocument *currDoc = m_container->currentDocument();
    QList<IDocument *> documents = m_container->documentsOrdered();
    foreach ( IDocument *document, documents ) {
        bool isNew = document->isNew();
        if ( isNew && !document->isModified() )
            continue;
        QString fileName = ( isNew ? document->title() : document->filePath() );
        QDomElement itemElement = domDocument.createElement("item");
        itemElement.setAttribute("fileName", fileName);
        itemElement.setAttribute("active", document == currDoc ? "1" : "0");
        itemElement.setAttribute("new", isNew);
        document->saveState(&domDocument, &itemElement);
        document->saveContentState(&domDocument, &itemElement);
        root.appendChild(itemElement);
        if ( isNew )
            saveTemporaryNewDocument(document, location);
    }

    const int IndentSize = 4;
    QTextStream out(&file);
    domDocument.save(out, IndentSize);

    file.close();
}

void DocumentManager::loadSession( const QString &location )
{
    QString fileName = location;
    fileName += "/documents.xml";

    QFile file(fileName);
    if ( !file.exists() )
        return;

    QDomDocument domDocument;

    QString errorStr;
    int errorLine;
    int errorColumn;

    // Load all
    if (!domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn)) {
        m_mainWindow->showWarning( tr("Error On Load Session Documents\nCommon Error in xml-file:\nLine %1, Column %2: %3")
                                   .arg(errorLine).arg(errorColumn).arg(errorStr) );
        return;
    }

    // Primary check (Document Type and Version)
    QDomElement root = domDocument.documentElement();
    if ( root.tagName() != "documents" ) {
        m_mainWindow->showWarning( tr("Error On Load Session Documents\nInvalid Format of Sessions xml-file") );
        return;
    }
    if ( !root.hasAttribute("version") ) {
        m_mainWindow->showWarning( tr("Error On Load Session Documents\nInvalid Format of Sessions xml-file (version expected)") );
        return;
    }
    if ( root.attribute("version") != "1.0" ) {
        m_mainWindow->showWarning( tr("Error On Load Session Documents\nInvalid Format of Sessions xml-file (wrong version)") );
        return;
    }

    IDocument *activeDocument = 0;
    m_untitledCounter = 0;

    m_mainWindow->setNavigationHistoryBlocked();

    QDomElement itemElement = root.firstChildElement();
    while ( !itemElement.isNull() ) {
        if ( itemElement.tagName() != "item" ) {
            m_mainWindow->showWarning( tr("Error On Load Session Documents\nInvalid Format of Sessions xml-file (unknown tag <%1> at line %2)")
                                       .arg(itemElement.tagName()).arg(itemElement.lineNumber()) );
            break;
        }
        QString fileName = itemElement.attribute("fileName");
        bool isActive = itemElement.attribute("active").toInt();
        bool isNew = itemElement.attribute("new", "0").toInt();
        bool isOpened = ( isNew ? loadTemporaryNewDocument(fileName, location) : openDocument(fileName) != 0 );
        if ( isOpened ) {
            IDocument *curDocument = currentDocument();
            if ( isActive )
                activeDocument = curDocument;
            curDocument->restoreState(&itemElement);
            curDocument->restoreContentState(&itemElement);
        }
        itemElement = itemElement.nextSiblingElement();
    }

    if ( activeDocument )
        activateDocument(activeDocument);

    m_mainWindow->setNavigationHistoryBlocked(false);
}

bool DocumentManager::applicationAboutToClose()
{
    return tryConfirmSaveAllDocuments();
}

void DocumentManager::applicationClose()
{
    tryCloseAllFiles(true);
}

void DocumentManager::onSettingsChanged()
{
    onFormatSettingsChanged();
}
