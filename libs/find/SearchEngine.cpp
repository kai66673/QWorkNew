#include <QDockWidget>
#include <QTabWidget>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QAction>
#include <QTableView>
#include <QLineEdit>
#include <QMenu>
#include <QLabel>
#include <QToolBar>
#include <QToolButton>
#include <QMessageBox>
#include <QPushButton>
#include <QTextCodec>
#include <QLineEdit>
#include <QFileDialog>
#include <QString>

#include <QDebug>

#include "Core.h"
#include "IDocument.h"
#include "ITextDocument.h"
#include <textfindconstants.h>
#include <ifindsupport.h>
#include <itexteditor.h>
#include <searchresultwindow.h>
#include <basefilefind.h>
#include <textfindconstants.h>
#include <searchresultwindow.h>
#include <ifile.h>
#include <qtcassert.h>

#include "SearchResultTabWidget.h"
#include "BaseTextSearchParamsWidget.h"
#include "FindReplaceWidget.h"
#include "SearchEngine.h"
#include "SearchReplaceTextDialog.h"
#include "LocatorSearchModePlugin.h"
#include "ModeManager.h"
#include "StyleManager.h"

///////////////////////////////////////////////////////////////////////////////////
/// SearchModeAction
SearchModeAction::SearchModeAction( const QString &text, int searchModeIndex, ISearchModePlugin *searchModePlugin, QObject *parent )
    : QAction(text, parent)
    , m_searchModeIndex(searchModeIndex)
    , m_searchModePlugin(searchModePlugin)
{
}

///////////////////////////////////////////////////////////////////////////////////
/// QBaseSearchModeSettings
BaseSearchModeSettings::BaseSearchModeSettings( FindReplaceWidget *searchWidget )
    : m_searchWidget(searchWidget)
{
    valuesMap["CaseSensetive"]          = false;
    valuesMap["WholeWordsOnly"]         = false;
    valuesMap["UseRegularExpressions"]  = false;
}

void BaseSearchModeSettings::doApplySettings()
{
    m_searchWidget->applySettings();
}

///////////////////////////////////////////////////////////////////////////////////
/// QBaseSearchModePlugin
BaseSearchModePlugin::BaseSearchModePlugin()
    : m_searchWidget(new FindReplaceWidget())
{
    registerFindScope(new CurrentTextDocumentScope());
    registerFindScope(new OpenedTextDocumentsScope());
    registerFindScope(new SystemDirDocumentsScope());
}

BaseSearchModePlugin::~BaseSearchModePlugin()
{
    qDeleteAll(m_findScopeList);
}

QWidget *BaseSearchModePlugin::createWidget( QWidget *parent )
{
    m_searchWidget->setParent(parent);
    return m_searchWidget;
}

void BaseSearchModePlugin::startSearchText( const QString &searchText )
{
    if ( !m_searchWidget )
        return;

    if ( searchText != "" )
       m_searchWidget->setFindText(searchText);

    m_searchWidget->startSearch();
}

bool BaseSearchModePlugin::isSearchChanged( const QString &searchText )
{
    if ( searchText.isEmpty() )
        return false;

    if ( searchText == m_searchWidget->findText() )
        return false;

    return true;
}

QList<QAction *> BaseSearchModePlugin::actions() const
{
    return m_searchWidget->searchActions();
}

///////////////////////////////////////////////////////////////////////////////////
/// QTestSearchModePlugin
QWidget *QTestSearchModePlugin::createWidget( QWidget *parent )
{
    return new QLabel(tr("!! ANY TEST MODE !!"), parent);
}

///////////////////////////////////////////////////////////////////////////////////
/// SearchEngine
SearchEngine *SearchEngine::searchEngine = 0;

SearchEngine::SearchEngine( QMenu *searchMenu, QObject *parent )
    : QObject(parent)
    , m_toggleSearchActionsBlocked(false)
    , m_searchMenu(searchMenu)
    , m_lastSearchModePlugin(0)
{
    searchEngine = this;
    IMainWindow *mainWindow = Core::Storage::mainWindow();

    loadSearchModePlugins();

    QList<QAction *> actions;
    foreach ( SearchModeAction *action, searchModeActions )
        actions.append(action);
    dockWidget = new QwCustomDockWidget( mainWindow->widget(), tr("Search"), "SearchEngineDockWidget",
                                         QKeySequence(tr("Ctrl+D,S")), QIcon(":/images/find_dock.png"),
                                         Qt::BottomDockWidgetArea, QwCustomDockWidget::First,
                                         actions );
    dockWidget->setContentsMargins(0, 0, 0, 0);

    m_cornerToolBar = new QwTabCornerToolBar();

    m_cmdPrev       = new Core::Command( tr("GotoPreviousItem"),
                                         QIcon(":/images/sr_prev.png"),
                                         tr("Goto Previous Item"),
                                         tr("Goto Previous Item"),
                                         QKeySequence(tr("Shift+F6")),
                                         this );

    m_cmdNext       = new Core::Command( tr("GotoNextItem"),
                                         QIcon(":/images/sr_next.png"),
                                         tr("Goto Next Item"),
                                         tr("Goto Next Item"),
                                         QKeySequence(tr("F6")),
                                         this );

    m_cmdExpand     = new Core::Command( tr("ExpandAll"),
                                         QIcon(":/images/sr_expand.png"),
                                         tr("Expand All"), tr("Expand/Collapse All Items"),
                                         QKeySequence(),
                                         this );

    m_cmdCloseAll   = new Core::Command( tr("ClearAll"),
                                         QIcon(":/images/sr_clear.png"),
                                         tr("Clear"),
                                         tr("Clear (Remove) All Search Tabs"),
                                         QKeySequence(),
                                         this );

    m_cmdExpand->action()->setCheckable(true);

    Core::CommandManager::getInstance()->registerCommands( tr("FindPanelCommands"),
                                                            QList<Core::Command *>() << m_cmdPrev
                                                                                     << m_cmdNext
                                                                                     << m_cmdExpand
                                                                                     << m_cmdCloseAll );

    m_cornerToolBar->addActions(QList<QAction *>() << m_cmdPrev->action()
                                                   << m_cmdNext->action()
                                                   << m_cmdExpand->action()
                                                   << m_cmdCloseAll->action());

    int qssWatcherIndex = Utils::StyleManager::registerQssFileWatcher(QLatin1String("SearchTabWidget"));
    searchResultTabWidget = new SearchResultTabWidget(qssWatcherIndex, m_cornerToolBar, dockWidget);
    QSizePolicy sp = searchResultTabWidget->sizePolicy();
    sp.setVerticalStretch(1);
    searchResultTabWidget->setSizePolicy(sp);
    searchResultTabWidget->setStyleSheet("SearchResultTabWidget::pane { border-top: 1px solid #C2C7CB; }");

    QWidget *widget = new QWidget(dockWidget);
    m_mainLayout = new QVBoxLayout(widget);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->addWidget(searchResultTabWidget);

    dockWidget->setWidget(widget);
    dockWidget->setObjectName("SearchEngineDockWidget");
    mainWindow->addDockWidgetHandler(new Core::DockWidgetHandler(dockWidget,
                                                                 new Core::EditModeDockGuiClient(dockWidget)));
    mainWindow->addToggleAction(dockWidget->toggleViewAction());

    connect(searchResultTabWidget, SIGNAL(tabCloseRequested(int)),  this, SLOT(onTabClose(int)));

    if ( !searchModeActions.isEmpty() ) {
        QMenu *contextMenu = new QMenu;
        foreach ( SearchModeAction *action, searchModeActions )
            contextMenu->addAction(action);
        dockWidget->setContextMenu(contextMenu);
    }

    baseSearchModeInit();

    connect(m_cmdPrev->action(),        SIGNAL(triggered()), this, SLOT(onPrevSearchResult()));
    connect(m_cmdNext->action(),        SIGNAL(triggered()), this, SLOT(onNextSearchResult()));
    connect(m_cmdExpand->action(),      SIGNAL(triggered(bool)), this, SLOT(onExpandSearchResults(bool)));
    connect(m_cmdCloseAll->action(),    SIGNAL(triggered()), this, SLOT(onAllTabClose()));
}

SearchEngine::~SearchEngine()
{
    qDeleteAll(searchModePlugins);
    qDeleteAll(m_searchWindows);
}

Find::SearchResultWindow *SearchEngine::newSearchResultWindow( const QString &title, const QIcon &icon, ISearchProcess *search )
{
    Find::SearchResultWindow *searchWindow = new Find::SearchResultWindow(search);
    m_searchWindows.append(searchWindow);
    searchResultTabWidget->addWidget(icon, title, searchWindow->outputWidget(0));
    dockWidget->showAndSetCurrentIfTabified();
    return searchWindow;
}

void SearchEngine::setCurrentSearchProgressWidget( QWidget *progressWidget )
{
    searchResultTabWidget->setCurrentSearchProgressWidget(progressWidget);
}

void SearchEngine::onTabClose( int tabIndex )
{
    if ( !m_searchWindows[tabIndex]->cancelSearch() ) {
        Core::Storage::warning(tr("Search Tab Cannot be Closed - Search in Progress."));
        return;
    }

    searchResultTabWidget->removeWidget(tabIndex);
    delete m_searchWindows[tabIndex];
    m_searchWindows.removeAt(tabIndex);
}

void SearchEngine::onAllTabClose()
{
    bool hasUnfinished = false;

    for ( int i = m_searchWindows.size() - 1; i >= 0; i-- ) {
        if ( !m_searchWindows[i]->cancelSearch() ) {
            hasUnfinished = true;
            continue;
        }

        searchResultTabWidget->removeWidget(i);
        delete m_searchWindows[i];
        m_searchWindows.removeAt(i);
    }

    if ( hasUnfinished ) {
        Core::Storage::information( tr("There are Search Tabs which cannot be closed - Search in Progress") );
    }
}

void SearchEngine::onSearch( bool bShow )
{
    if ( m_toggleSearchActionsBlocked )
        return;

    m_toggleSearchActionsBlocked = true;

    SearchModeAction *searchAction = dynamic_cast<SearchModeAction *>(sender());
    QTC_ASSERT(searchAction, return);

    ISearchModePlugin *actionSearchMode = searchAction->searchModePlugin();
    QTC_ASSERT(actionSearchMode, return);

    if ( actionSearchMode != m_lastSearchModePlugin ) {
        m_searchActionsMenu->clear();
        m_searchActionsMenu->addActions(actionSearchMode->actions());
        m_lastSearchModePlugin = actionSearchMode;
    }

    bool indexChanged;
    dockWidget->showAndSetCurrentIfTabified(&indexChanged);
    if ( indexChanged )
        bShow = true;

    if ( actionSearchMode->isSearchChanged(DocumentManager::getInstance()->selectedText()) )
        bShow = true;

    if ( m_mainLayout->count() > 1 ) {
        QWidget *w = m_mainLayout->itemAt(0)->widget();
        w->hide();
        m_mainLayout->removeWidget(w);
    }

    if ( bShow ) {
        int index = 0;
        for (int i = 0; i < searchModeActions.size(); i++) {
            bool checked = (searchModePlugins[i] == actionSearchMode);
            if ( checked )
                index = i;
            searchModeActions[i]->setChecked(checked);
        }
        m_mainLayout->insertWidget(0, m_searchModeWidgets[index]);
        m_searchModeWidgets[index]->show();
        actionSearchMode->startSearchText(DocumentManager::getInstance()->selectedText());
    }

    m_toggleSearchActionsBlocked = false;
}

void SearchEngine::baseSearchModeInit()
{
    int index = 0;

    m_toggleSearchActionsBlocked = true;

    foreach ( ISearchModePlugin *searchPlugin, searchModePlugins ) {
        QWidget *smw = searchPlugin->createWidget(0);
        smw->hide();
        m_searchModeWidgets.append(smw);
        searchModeActions[index]->setChecked(false);
        index++;
    }

    m_toggleSearchActionsBlocked = false;

    m_searchMenu->addSeparator();
    m_searchActionsMenu = m_searchMenu->addMenu(tr("Search Actions"));
}

void SearchEngine::loadSearchModePlugins()
{
    BaseSearchModePlugin *baseSearchPlugin = new BaseSearchModePlugin();
    baseSearchPlugin->registerSettings();
    addSearchMode(baseSearchPlugin);

    LocatorSearchModePlugin *locatorPlugin = new LocatorSearchModePlugin();
    addSearchMode(locatorPlugin);
}

void SearchEngine::addSearchMode( ISearchModePlugin *searchModePlugin )
{
    int searchModeIndex = searchModePlugins.size();

    SearchModeAction *searchAction = new SearchModeAction(searchModePlugin->menuText(), searchModeIndex, searchModePlugin, Core::Storage::mainWindow()->widget());
    searchAction->setShortcut(searchModePlugin->shortcut());
    searchAction->setToolTip(searchModePlugin->toolTip());
    searchAction->setCheckable(true);
    searchAction->setIcon(QIcon(searchModePlugin->iconName()));
    m_searchMenu->addAction(searchAction);

//    searchModeStackedWidget->addWidget(searchModePlugin->createWidget(searchModeStackedWidget));

    searchModePlugins.append(searchModePlugin);
    searchModeActions.append(searchAction);

    connect(searchAction, SIGNAL(toggled(bool)), this, SLOT(onSearch(bool)));
}

void SearchEngine::onPrevSearchResult()
{
    int index = searchResultTabWidget->currentIndex();
    if ( index == -1 )
        return;

    m_searchWindows[index]->goToPrev();
}

void SearchEngine::onNextSearchResult()
{
    int index = searchResultTabWidget->currentIndex();
    if ( index == -1 )
        return;

    m_searchWindows[index]->goToNext();
}

void SearchEngine::onExpandSearchResults( bool checked )
{
    foreach ( Find::SearchResultWindow *searchWindow, m_searchWindows )
        searchWindow->onToggleExpandCollapseAll(checked);

    if ( checked )
        m_cmdExpand->action()->setText(tr("Collapse All"));
    else
        m_cmdExpand->action()->setText(tr("Expand All"));
}

void SearchEngine::onAnySearchProgressFinished()
{
    searchResultTabWidget->updateTabsGemetry();
}

BaseTextFindScope::BaseTextFindScope()
{}

BaseTextFindScope::~BaseTextFindScope()
{
}

QWidget *BaseTextFindScope::createFindParamsWidget( QWidget *parent )
{
    m_searchParamsWidget = new BaseTextSearchParamsWidget(parent);
    return m_searchParamsWidget;
}

void BaseTextFindScope::startSearch( const QString &searchText )
{
    TextEditor::SimpleBaseFileFind *m_find = new TextEditor::SimpleBaseFileFind( SearchEngine::getInstance()->newSearchResultWindow(searchText, QIcon(), 0), files() );
    m_find->resultWindow()->setSearchProcess(m_find);

    Find::FindFlags flags;
    if ( m_searchParamsWidget->caseSensetive() )
        flags |= Find::FindCaseSensitively;
    if ( m_searchParamsWidget->wholeWordsOnly() )
        flags |= Find::FindWholeWords;
    if ( m_searchParamsWidget->useRegularExpressions() )
        flags |= Find::FindRegularExpression;

    Find::SearchResult *searchResult = m_find->findAll(searchText, flags);
    connect(searchResult, SIGNAL(activated(Find::SearchResultItem)), this, SLOT(openEditor(Find::SearchResultItem)));
}

void BaseTextFindScope::startSearchAndReplace( const QString &searchText )
{
    TextEditor::SimpleBaseFileFind *m_find = new TextEditor::SimpleBaseFileFind( new Find::SearchResultWindow(), files() );
    m_find->resultWindow()->setSearchProcess(m_find);

    Find::FindFlags flags;
    if ( m_searchParamsWidget->caseSensetive() )
        flags |= Find::FindCaseSensitively;
    if ( m_searchParamsWidget->wholeWordsOnly() )
        flags |= Find::FindWholeWords;
    if ( m_searchParamsWidget->useRegularExpressions() )
        flags |= Find::FindRegularExpression;

    SearchReplaceTextDialog replaceDialog(m_find, searchText, flags, Core::Storage::mainWindow()->widget());
    replaceDialog.exec();
}

void BaseTextFindScope::openEditor( const Find::SearchResultItem &item )
{
    if (item.path.size() > 0) {
        DocumentManager::getInstance()->openDocument( QDir::fromNativeSeparators(item.path.first()),
                                                      new TextDocumentCursor(item.lineNumber - 1, item.textMarkPos) );
    } else {
        DocumentManager::getInstance()->openDocument(item.text);
    }
}

bool CurrentTextDocumentScope::isSearchEnabled() const
{
    Core::IEditor *editor = DocumentManager::getInstance()->currentEditor();
    if ( !editor )
        return false;

    return (qobject_cast<TextEditor::ITextEditor *>(editor) != 0);
}

bool CurrentTextDocumentScope::prepareToSearch() const
{
    IDocument *document = DocumentManager::getInstance()->currentDocument();
    if ( !document )
        return false;

    if ( document->isNew() || document->isModified() ) {
        if ( Core::Storage::information( tr("Current document should be saved before search!\nSave It?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) != QMessageBox::Yes )
            return false;
        DocumentManager::getInstance()->saveFile();
        if ( document->isNew() || document->isModified() )
            return false;
    }

    return true;
}

bool CurrentTextDocumentScope::isSearchAndReplaceEnabled() const
{
    return isSearchEnabled();
}


Utils::FileIterator * CurrentTextDocumentScope::files()
{
    return new Utils::FileIterator( QStringList() << DocumentManager::getInstance()->currentDocument()->filePath(),
                                    QList<QTextCodec *>() << QTextCodec::codecForName("Windows-1251") );
}

Utils::FileIterator * OpenedTextDocumentsScope::files()
{
    QList <Core::IEditor *> editors = DocumentManager::getInstance()->openedEditors();

    QStringList fileNames;
    foreach ( Core::IEditor *editor, editors )
        fileNames << editor->file()->fileName();

    return new Utils::FileIterator( fileNames, QList<QTextCodec *>() << QTextCodec::codecForName("Windows-1251") );
}

bool OpenedTextDocumentsScope::isSearchEnabled() const
{
    return DocumentManager::getInstance()->count() > 0;
}

bool OpenedTextDocumentsScope::isSearchAndReplaceEnabled() const
{
    return isSearchEnabled();
}

bool OpenedTextDocumentsScope::prepareToSearch() const
{
    DocumentManager *documentManager = DocumentManager::getInstance();

    if ( documentManager->hasUnsavedDocuments() ) {
        if ( Core::Storage::information(tr("All Documents should be Saved.\nSave It?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) != QMessageBox::Yes )
            return false;

        documentManager->saveAllFiles();
        if ( documentManager->hasUnsavedDocuments() )
            return false;
    }

    return true;
}

bool SystemDirDocumentsScope::prepareToSearch() const
{
    if ( m_rootDirLineEdit->text().isEmpty() ) {
        Core::Storage::warning("Root directory should be select");
        m_selectRootDirPushButton->setFocus();
        return false;
    }

    return true;
}

QWidget *SystemDirDocumentsScope::createFindParamsWidget(QWidget *parent)
{
    QWidget *widget = BaseTextFindScope::createFindParamsWidget(parent);

    QHBoxLayout *rootDirLayout = new QHBoxLayout;
    rootDirLayout->setContentsMargins(0, 0, 0, 0);
    rootDirLayout->setSpacing(0);
    m_rootDirLineEdit = new QLineEdit;
    m_rootDirLineEdit->setReadOnly(true);
    m_selectRootDirPushButton = new QPushButton(tr("..."));
    m_selectRootDirPushButton->setMaximumWidth(26);
    rootDirLayout->addWidget(m_rootDirLineEdit);
    rootDirLayout->addWidget(m_selectRootDirPushButton);
    QWidget *rootDirWidget = new QWidget;
    rootDirWidget->setLayout(rootDirLayout);

    QHBoxLayout *patternLayout = new QHBoxLayout;
    patternLayout->setContentsMargins(0, 0, 0, 0);
    patternLayout->setSpacing(2);
    m_filePatternLineEdit = new QLineEdit;
    QLabel *patternLabel = new QLabel(tr("Pattern Files"));
    patternLayout->addWidget(patternLabel);
    patternLayout->addWidget(m_filePatternLineEdit);
    QWidget *patternWidget = new QWidget;
    patternWidget->setLayout(patternLayout);

    QVBoxLayout *layout = qobject_cast<QVBoxLayout *> (widget->layout());
    layout->removeItem(widget->layout()->itemAt(3));
    layout->addWidget(rootDirWidget);
    layout->addWidget(patternWidget);
    layout->addStretch();

    connect(m_selectRootDirPushButton, SIGNAL(clicked()), this, SLOT(onSelectRootDir()));

    return widget;
}

void SystemDirDocumentsScope::onSelectRootDir()
{
    QString dirName = QFileDialog::getExistingDirectory( Core::Storage::mainWindow()->widget(),
                                                         tr("Select Path"),
                                                         "",
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );

    if ( dirName.isEmpty() )
        return;

    m_rootDirLineEdit->setText(dirName);
}

QStringList SystemDirDocumentsScope::fileNameFilters() const
{
    QStringList filters;
    if ( !m_filePatternLineEdit->text().isEmpty() ) {
        const QStringList parts = m_filePatternLineEdit->text().split(QLatin1Char(','));
        foreach (const QString &part, parts) {
            const QString filter = part.trimmed();
            if (!filter.isEmpty()) {
                filters << filter;
            }
        }
    }
    return filters;
}

Utils::FileIterator * SystemDirDocumentsScope::files()
{
    return new Utils::SubDirFileIterator( QStringList() << m_rootDirLineEdit->text(),
                                          fileNameFilters(),
                                          QTextCodec::codecForName("Windows-1251") );
}
