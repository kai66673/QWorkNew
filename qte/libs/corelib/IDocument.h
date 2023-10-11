#ifndef IDOCUMENT_H
#define IDOCUMENT_H

#include <QList>
#include <QAction>
#include <QTabWidget>
#include <QFileSystemWatcher>
#include <QDomDocument>
#include <QDomElement>
#include <QToolBar>

#include "corelib_global.h"
#include "iassistprovider.h"
#include "ISettingsFactory.h"
#include "IMainWindow.h"
#include "ieditor.h"
#include "CommandManager.h"
#include "OpenEditorsWindow.h"
#include "CloseApplicationHandler.h"
#include "SessionManager.h"
#include "IChangeProjectFileWatcher.h"
#include "ModeManager.h"
#include "IGuiFactory.h"
#include "DocumentToolBar.h"
#include "HighlighterFormatsManager.h"
#include "ISettingsClient.h"

class IDocument;
class IDocumentPlugin;
class DocumentManager;
class UiProviderInterface;

class CORELIB_EXPORT IDocumentFactory
{
public:
    IDocumentFactory();
    virtual ~IDocumentFactory();

    IDocument *createDocument( const QString &extension );
    IDocument *createDocumentByFileName(const QString &filePath , QString &realFilePath, QString &errorMessage);
    virtual IDocument *newDocument( QString &extension ) = 0;

    void initializeDocumentPlugins();
    virtual void loadDocumentPlugins() = 0;

    QList<UiProviderInterface *> createUiProviders( IMainWindow *mainWindow );

protected:
    QList <IDocumentPlugin *> m_documentPlugins;
};

class CORELIB_EXPORT IDocumentCursor
{
public:
    IDocumentCursor() {}
};

class CORELIB_EXPORT IDocument : public QObject
{
    Q_OBJECT

public:
    IDocument();
    virtual ~IDocument();

    virtual QWidget *editorWidget() const = 0;
    virtual QWidget *widget() const = 0;
    virtual Core::IEditor *editor() const = 0;
    virtual DocumentToolBar *toolBar() { return 0; }

    virtual bool newDocument( const QString &fileName ) = 0;
    virtual bool newDocumentFromFile( const QString &fileName, const QString &contentFilePath ) = 0;
    virtual bool openDocument( const QString &filePath ) = 0;
    virtual bool saveDocument() = 0;
    virtual bool saveAsDocument( const QString &filePath ) = 0;
    virtual bool saveToDocument( const QString &filePath ) = 0;
    virtual bool renameTo( const QString &newFilePath ) = 0;

    virtual QString title() const = 0;
    virtual QString toolTip() const = 0;
    virtual QString filePath() const = 0;

    virtual bool isModified() const = 0;
    virtual void setModified(bool onoff)  = 0;
    virtual bool isNew() const = 0;

    virtual void setCursor( IDocumentCursor *position ) = 0;

    virtual void onCommand( const QString &command ) { Q_UNUSED(command); }
    virtual const QString iconName( bool isModified = false ) const;

    virtual IDocumentPlugin *documentPlugin() const = 0;
    virtual void onFormatSettingsChanged() = 0;
    virtual void emitDocumentOpened( DocumentManager *documentManager ) { Q_UNUSED(documentManager); }
    virtual void emitDocumentClosed( DocumentManager *documentManager ) { Q_UNUSED(documentManager); }

    virtual void setTextCodec( QTextCodec *codec ) = 0;

    virtual void saveState( QDomDocument *document, QDomElement *element ) const;
    virtual void restoreState( QDomElement *element );
    virtual void saveContentState( QDomDocument *document, QDomElement *element ) const;
    virtual void restoreContentState( QDomElement *element );

    virtual IDocumentCursor *startDocumentPosition() const;
    virtual void refreshDocumentCursor();

    void addHolder( const QString &holderId, const QString &holdReason );
    void removeHolder( const QString &holderId );
    bool hasHolders() const { return m_holders.size() > 0; }
    QString holdReasons( const QString &splitter ) const;

    virtual bool fixOnSave() { return true; }

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    QMap <QString, QPair<int, QString> > m_holders;
    bool m_enterEventBlocked;
};

class CORELIB_EXPORT IDocumentPlugin : public QObject, public ISettingsFactory, public IGuiFactory
{
public:
    IDocumentPlugin();
    virtual ~IDocumentPlugin();

    IDocument *doCreateDocument();

    virtual bool acceptExtension( const QString &extension ) const = 0;
    virtual bool acceptFileName( const QString &/*fileName*/ ) const { return false; }
    virtual QString defaultExtension() const = 0;
    virtual QString description() const = 0;
    virtual bool isAvailableInNewDocumentDialog() const { return true; }

    virtual QList<QAction *> toolbarActions( const QString &id ) const { Q_UNUSED(id); return QList<QAction *>(); }
    TextEditor::IAssistProvider *quickFixProvider();

    virtual void updateEditorActions() =  0;
    virtual void fillEditMenu( QMenu *menu ) = 0;

    virtual void initialize() = 0;

    virtual QList<HighlighterFormatData *> createHighlightersFormatData() const { return QList<HighlighterFormatData *>(); }

    virtual QList<UiProviderInterface *> createUiProviders( IMainWindow *mainWindow ) { Q_UNUSED(mainWindow); return QList<UiProviderInterface *>(); }
    virtual Core::IMode *mode() const;
    virtual bool activateDocumentOnMouseEnterEvent() const;

protected:
    virtual IDocument *createDocument() = 0;
    virtual TextEditor::IAssistProvider *createQuickFixProvider() = 0;

private:
    TextEditor::IAssistProvider *m_quickFixProvider;
};

class CORELIB_EXPORT IDocumentContainer: public QObject
{
    Q_OBJECT

public:
    IDocumentContainer( IMainWindow *mainWindow, QObject *parent = 0 );
    virtual ~IDocumentContainer();

    void addDocument( IDocument *document );
    void closeDocument( IDocument *document );
    void setCurrentDocument( IDocument *document );
    IDocument *currentDocument() const;
    IDocument *findDocument( const QString &fileName ) const;
    QList <IDocument *> documents() const;
    QList<QAction *> createWindowsActions();
    QList<IDocument *> documentsByActivationHistory();

    virtual QList<QAction *> createAdditionalWindowsActions();

    virtual QWidget *createWidget() = 0;
    virtual void addWidget( QWidget *widget ) = 0;
    virtual void removeWidget( QWidget *widget ) = 0;
    virtual void setCurrentWidget( QWidget *widget ) = 0;
    virtual void documentChanged( IDocument *document ) = 0;
    virtual void updateDocumentIcon( IDocument *document ) = 0;
    virtual QList <IDocument *> documentsOrdered() const = 0;

    virtual void restoreState() = 0;

    QWidget *widget() {
        if ( !m_widget ) {
            m_widget = createWidget();
            if ( QFrame *frame = dynamic_cast<QFrame *>(widget()) ) {
                frame->setFrameShape(QFrame::NoFrame);
                frame->setLineWidth(0);
            }
        }
        return m_widget;
    }

private slots:
    void onDocumentActivated( IDocument *document );
    void gotoNextWindow();
    void gotoPreviousWindow();

signals:
    void currentDocumentChanged( IDocument * );
    void closeDocumentRequested( IDocument *document );

protected:
    bool eventFilter(QObject *obj, QEvent *ev);

    QWidget *m_widget;
    QHash <QWidget *, IDocument *> m_documents;
    QHash <QWidget *, IDocument *> m_editorDocuments;
    QToolBar *m_activeDocumentToolBar;
    QList<IDocument *> m_docHistory;

    IMainWindow *m_mainWindow;

private:
    Core::Command *m_cmdNextWindow;
    Core::Command *m_cmdPreviousWindow;

    void showPopupOrSelectDocument() const;
    OpenEditorsWindow *m_openEditorsWindow;
};

class CORELIB_EXPORT DocumentManager : public QObject, public ISessionClient, public ICloseApplicationClient, public ISettingsClient
{
    Q_OBJECT

public:
    DocumentManager( IMainWindow *mainWindow, IDocumentFactory *documentFactory, IDocumentContainer *documentContainer );
    ~DocumentManager();

    void loadDocumentPlugins();

    static DocumentManager *getInstance();

    QWidget *centralWidget() { return m_container->widget(); }
    IDocument *currentDocument();
    IDocumentPlugin *currentDocumentPlugin();
    QString selectedText();

    int documentsCount();

    IDocument *findDocument( const QString &filePath );
    bool closeDocument( IDocument *document, bool confirmSaving = true );
    bool tryCloseAllFiles( bool forceClose = false );
    void sendCommandToDocuments( const QString &command );

    bool activateDocument( IDocument *document, bool moveMousePosition = true );
    QList <IDocument *> documentsOrdered() const;

    IDocument *openDocument( const QString &filePath, IDocumentCursor *position = 0, QString *errorMessage = 0, bool addRecentFile = true );
    QMenu *editMenu();

    virtual void fontSizeIncrease();
    virtual void fontSizeDecrease();

    bool isCurrentDocumentUntitled();

    QList <Core::IEditor *> openedEditors();

    void emitEditorOpened( Core::IEditor *editor );
    void emitEditorClosed( Core::IEditor *editor );

    Core::IEditor *editorForFileName( const QString &fileName );
    Core::IEditor *currentEditor();
    QWidget *currentEditorWidget();

    int count() const { return m_container->documents().size(); }
    bool hasUnsavedDocuments() const;

    void initializeDocumentPlugins();

    bool tryConfirmSaveAllDocuments( bool bSessionClose = false );
    bool trySaveDocument( IDocument *document = 0 );
    bool trySaveAllDocuments();
    bool silentSaveAllDocuments();

    // ISessionClient implementaion
    virtual bool sessionAboutToClose();
    virtual void sessionClose();
    virtual void saveSession( const QString &location );
    virtual void loadSession( const QString &location );

    // ICloseApplicationClient implementation
    virtual bool applicationAboutToClose();
    virtual void applicationClose();

    // ISettingsClient implementation
    virtual void onSettingsChanged();

    bool newDocument( const QString &extension );

    QMap<QString, QString> openedEditorsContents();

    QList<QAction *> createWindowsActions();
    QList<UiProviderInterface *> createUiProviders( IMainWindow *mainWindow );

    void expectFileChanges( const QString &fileName );
    void unexpectFileChanges( const QString &fileName, bool reload = true );

    void registerProjectFileWatcher(const QString &fileName, IChangeProjectFileWatcher *watcher);
    void deregisterProjectFileWatcher(const QString &fileName);

public slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveAsFile();
    void saveAllFiles();
    void closeFile();
    void closeAllFiles();
    void insertText( const QString &text, const QString &extension );
    void onDocumentModificationChanged( IDocument *document );
    void onDocumentSaved(const QString &fileName);

    void onFormatSettingsChanged();

    void onMainWindowActivated();
    void checkForReload();
    void onFileChangedOutside( const QString &filePath );
    void onCurrentEditorChanged( IDocument *document );
    void onCursorPositionChanged();
    void onCloseDocumentRequested( IDocument *document );

    void updateEditMenu();
    void onDocumentRenamed( const QString &oldFileName, const QString &newFileName );

signals:
    void documentSaved( const QString &filePath );

    void editorOpened( Core::IEditor *editor );
    void editorClosed( Core::IEditor *editor );
    void currentEditorChanged( QWidget *editor );
    void currentDocumentChanged( IDocument *document );
    void currentDocumentPositionChanged( IDocument *document );
    void documentClosed( IDocument *document );

    void addToRecentFiles( const QString &filePath );
    void updateRecentFile( const QString &oldFileName, const QString &newFileName );

private:
    static DocumentManager *documentManager;

    bool tryCloseFile( IDocument *document = 0, bool forceClose = false );
    IDocument *openDocumentInternal( const QString &filePath, QString &errorMessage, IDocumentCursor *position = 0, bool addRecentFile = true );

    void addDocumentInternal( IDocument *document , IDocumentCursor *position = 0 );

    IMainWindow *m_mainWindow;
    IDocumentFactory *m_documentFactory;
    IDocumentContainer *m_container;

    QFileSystemWatcher *m_watcher;
    bool m_blockActivatedSignal;
    QSet<QString> m_changedFiles;
    QSet<QString> m_deletedFiles;

    IDocumentPlugin *m_lastMenuDocumentPlugin;

    QString createUntitledBaseName();
    void saveTemporaryNewDocument( IDocument *document, const QString &location );
    bool loadTemporaryNewDocument( const QString &fileName, const QString &location );
    int m_untitledCounter;

    QMap<QString, IChangeProjectFileWatcher *> m_projectFiles;
};

Q_DECLARE_METATYPE(IDocument *)

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IDocumentPlugin, "QWorkBase.IDocumentPlugin/1.0")
QT_END_NAMESPACE

#endif // IDOCUMENT_H
