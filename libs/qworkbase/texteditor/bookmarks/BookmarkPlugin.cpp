#include <QAction>
#include <QIcon>
#include <QFile>
#include <QToolBar>
#include <QMenu>

#include "BookmarkPlugin.h"
#include "QWorkBase.h"

BookmarkPlugin::BookmarkPlugin()
    : ISessionClient()
    , m_manager(new Bookmarks::Internal::BookmarkManager)
{
    createActions();
}

BookmarkPlugin::~BookmarkPlugin()
{
    delete m_manager;
}

void BookmarkPlugin::createActions()
{
    QWorkMainWindow *mainWindow = QWorkMainWindow::getInstance();

    m_cmdToggle     = new Core::Command( tr("ToggleBookmark"),
                                         QIcon(":/texteditor/images/bookmark.png"),
                                         tr("Toggle Bookmark"),
                                         tr("Toggle Bookmark"),
                                         QKeySequence(tr("Ctrl+M")),
                                         mainWindow );

    m_cmdNext       = new Core::Command( tr("GotoNextBookmark"),
                                         QIcon(":/texteditor/images/bookmark_next.png"),
                                         tr("Goto Next Bookmark"),
                                         tr("Goto Next Bookmark"),
                                         QKeySequence(tr("Ctrl+.")),
                                         mainWindow );

    m_cmdPrev       = new Core::Command( tr("GotoPreviousBookmark"),
                                         QIcon(":/texteditor/images/bookmark_prev.png"),
                                         tr("Goto Previous Bookmark"),
                                         tr("Goto Previous Bookmark"),
                                         QKeySequence(tr("Ctrl+,")),
                                         mainWindow );

    m_cmdNextInDoc  = new Core::Command( tr("GotoNextDocumentBookmark"),
                                         QIcon(":/texteditor/images/bookmark_next_indoc.png"),
                                         tr("Goto Next Boookmark in Current Document"),
                                         tr("Goto Next Boookmark in Current Document"),
                                         QKeySequence(tr("Ctrl+Alt+.")),
                                         mainWindow );

    m_cmdPrevInDoc  = new Core::Command( tr("GotoPreviousDocumentBookmark"),
                                         QIcon(":/texteditor/images/bookmark_prev_indoc.png"),
                                         tr("Goto Previous Bookmark in Current Document"),
                                         tr("Goto Previous Bookmark in Current Document"),
                                         QKeySequence(tr("Ctrl+Alt+,")),
                                         mainWindow );

    connect(m_cmdToggle->action(),          SIGNAL(triggered()), m_manager, SLOT(toggleBookmark()));
    connect(m_cmdToggle->action(),          SIGNAL(triggered()), this,      SLOT(onToggleBookmark()));
    connect(m_cmdNext->action(),            SIGNAL(triggered()), m_manager, SLOT(next()));
    connect(m_cmdNextInDoc->action(),       SIGNAL(triggered()), m_manager, SLOT(nextInDocument()));
    connect(m_cmdPrev->action(),            SIGNAL(triggered()), m_manager, SLOT(prev()));
    connect(m_cmdPrevInDoc->action(),       SIGNAL(triggered()), m_manager, SLOT(prevInDocument()));
}

QList <QAction *> BookmarkPlugin::actions()
{
    return QList <QAction *>() << m_cmdToggle->action()
                               << m_cmdNext->action()
                               << m_cmdPrev->action()
                               << m_cmdNextInDoc->action()
                               << m_cmdPrevInDoc->action();
}

void BookmarkPlugin::initialize()
{
    Core::CommandManager::getInstance()->registerCommands( tr("BookmarksCommands"),
                                                            QList<Core::Command *>() << m_cmdToggle
                                                                                     << m_cmdNext
                                                                                     << m_cmdPrev
                                                                                     << m_cmdNextInDoc
                                                                                     << m_cmdPrevInDoc );
}

QList<UiProviderInterface *> BookmarkPlugin::createUiProviders( IMainWindow *mainWindow )
{
    m_bookmarksDock = new QwCustomDockWidget( mainWindow->widget(), tr("Bookmarks"), "BookmarksDockWidget",
                                              QKeySequence(), QIcon(":/texteditor/images/bookmarks.png"),
                                              Qt::LeftDockWidgetArea, QwCustomDockWidget::Second,
                                              QList<QAction *>() << m_cmdNext->action()
                                                                 << m_cmdPrev->action()
                                                                 << m_cmdNextInDoc->action()
                                                                 << m_cmdPrevInDoc->action() );

    Bookmarks::Internal::BookmarkView *bookmarksView = new Bookmarks::Internal::BookmarkView(m_bookmarksDock);
    bookmarksView->setModel(m_manager);
    m_bookmarksDock->setWidget(bookmarksView);

    QMenu *contextMenu = new QMenu;
    contextMenu->addAction(m_cmdNext->action());
    contextMenu->addAction(m_cmdPrev->action());
    contextMenu->addAction(m_cmdNextInDoc->action());
    contextMenu->addAction(m_cmdPrevInDoc->action());
    m_bookmarksDock->setContextMenu(contextMenu);

    Core::MainToolBar *toolBar = new Core::MainToolBar(tr("Bookmarks"));
    toolBar->setObjectName("BookmarksToolBar");
    toolBar->addActions(actions());

    QList<UiProviderInterface *> providers;
    providers << new UiProviderInterface(m_bookmarksDock, toolBar);

    return providers;
}

bool BookmarkPlugin::sessionAboutToClose()
{
    return true;
}

void BookmarkPlugin::sessionClose()
{
    m_manager->removeAllBookmarks();
}

void BookmarkPlugin::saveSession( const QString &location )
{
    QString fileName = location;
    fileName += "/bookmarks.xml";

    QFile file(fileName);
    if ( !file.open(QFile::WriteOnly | QFile::Text) ) {
        QWorkMainWindow::warning( tr("Error On Save Session Bookmarks\nFile \"%1\" Cannot be Opened for Writing:\n%2.")
                                  .arg(fileName).arg(file.errorString()) );
        return;
    }

    QDomDocument domDocument("bookmarks");

    domDocument.appendChild(domDocument.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"windows-1251\""));

    QDomElement root = domDocument.createElement("bookmarks");
    root.setAttribute("version", "1.0");
    domDocument.appendChild(root);

    m_manager->saveBookmarks(domDocument, &root);

    const int IndentSize = 4;
    QTextStream out(&file);
    domDocument.save(out, IndentSize);

    file.close();
}

void BookmarkPlugin::loadSession( const QString &location )
{
    QString fileName = location;
    fileName += "/bookmarks.xml";

    QFile file(fileName);
    if ( !file.exists() )
        return;

    QDomDocument domDocument;

    QString errorStr;
    int errorLine;
    int errorColumn;

    // Загружаем всю структуру
    if (!domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn)) {
        QWorkMainWindow::warning( tr("Error On Load Session Bookmarks\nCommon Error in xml-file:\nLine %1, Column %2: %3")
                                  .arg(errorLine).arg(errorColumn).arg(errorStr) );
        return;
    }

    // Первичная проверка (тип и версия документа)
    QDomElement root = domDocument.documentElement();
    if ( root.tagName() != "bookmarks" ) {
        QWorkMainWindow::warning( tr("Error On Load Session Bookmarks\nInvalid Format of Sessions xml-file") );
        return;
    }
    if ( !root.hasAttribute("version") ) {
        QWorkMainWindow::warning( tr("Error On Load Session Bookmarks\nInvalid Format of Sessions xml-file (version expected)") );
        return;
    }
    if ( root.attribute("version") != "1.0" ) {
        QWorkMainWindow::warning( tr("Error On Load Session Bookmarks\nInvalid Format of Sessions xml-file (wrong version)") );
        return;
    }

    m_manager->loadBookmarks(&root);

    file.close();
}

void BookmarkPlugin::onToggleBookmark()
{
    if ( m_bookmarksDock->isVisible() )
        m_bookmarksDock->showAndSetCurrentIfTabified();
}

QMenu *BookmarkPlugin::menu()
{
    QMenu *menu = new QMenu(tr("Bookmarks"));
    menu->addActions(actions());
    return menu;
}
