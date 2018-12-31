#include <QtPlugin>
#include <QPluginLoader>
#include <QSqlDriver>
#include <QDomDocument>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QStackedWidget>
#include <QApplication>
#include <QComboBox>
#include <QToolBar>
#include <QLabel>
#include <QTextEdit>
#include <QHeaderView>
#include <QComboBox>
#include <QMenu>

#include "IDocument.h"
#include "DbManager.h"
#include "QSqlHistoryModel.h"
#include "QSqlHistoryTableView.h"
#include "QueryViewSettingsWidget.h"
#include "sql_constants.h"
#include "dbbrowser_constants.h"
#include "ConnectDialog.h"
#include "QDbStructWidget.h"
#include "QRdbQueryModel.h"
#include "QwTableView.h"
#include "DbQueryModel.h"
#include "DbFilterDialog.h"
#include "DbDetailsWidget.h"

#include "AST.h"
#include "SqlDocument.h"
#include "SqlParserEngineManager.h"
#include "StyleManager.h"

namespace Database {

DbManager *DbManager::connectionsManager(0);

DbTabWidget::DbTabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    setStyleSheet("QTabWidget::pane { border-top: 1px solid #C2C7CB; }");
    setTabBar(new QwTabBar(this));
}

SettingsCategoryViewQuery::SettingsCategoryViewQuery()
{
    valuesMap["MaxRowsCount"] = 4000;
    valuesMap["DefaultRowHeight"] = 14;
    valuesMap["ShowVerticalHeader"] = true;
    valuesMap["AutoResizeColumns"] = true;
    valuesMap["MaxColumnsWidth"] = 200;
    valuesMap["Font"] = QApplication::font();
    valuesMap["AlternatingRows"] = false;
    valuesMap["BackgroundColor1"] = QApplication::palette().color(QPalette::Window);
    valuesMap["BackgroundColor2"] = QApplication::palette().color(QPalette::Window);
    valuesMap["ForegroundColor"]  = QApplication::palette().color(QPalette::WindowText);
    valuesMap["SplitterSymbol"] = ", ";
    valuesMap["ShowNull"] = false;
    valuesMap["SqlHistory"] = true;
}

ISettingsTabPage *SettingsCategoryViewQuery::createSettingsTabPage( QWidget *parent )
{
    return new QueryViewSettingsWidget(valuesMap, parent);
}

void SettingsCategoryViewQuery::doApplySettings()
{
    Database::DbManager::getInstance()->reflectOnQueryViewSettingsChanged(valuesMap["SplitterSymbol"].toString());
}

DbManager::DbManager()
    : ISessionClient()
    , ICloseApplicationClient()
    , m_mainWindow(Core::Storage::mainWindow())
    , dbIndex(0)
{
    connectionsManager = this;

    m_qssDetailsWathcherIndex = Utils::StyleManager::registerQssFileWatcher(QLatin1String("DbDetailsTabBar"));

    createActions();
    createContextMenuActions();
    createSqlHistoryActions();
    createQueryTabActions();

    connect(DocumentManager::getInstance(), SIGNAL(currentDocumentChanged(IDocument*)), this, SLOT(onCurrentDocumentChanged(IDocument*)));

    loadLoaderPlugins();

    m_connectionsHistory = new DbConnectionsHistory;

    DatabaseManagerPythonModule *module = new DatabaseManagerPythonModule(this);
    Core::Storage::mainWindow()->registerModule(module, true, QStringList() << "DatabaseCursor" << "DatabaseConnection");
}

DbManager *DbManager::getInstance()
{
    return connectionsManager;
}

IDbLoader *DbManager::getCurrentLoader() const
{
    int currDbIndex = databaseComboBox->currentIndex();
    if ( currDbIndex == -1 || currDbIndex >= structWidgetList.size() )
        return 0;

    return structWidgetList[currDbIndex]->getCurrentLoader();
}

IDbCatalogItem *DbManager::getCurrentItem() const
{
    int currDbIndex = databaseComboBox->currentIndex();
    if ( currDbIndex == -1 )
        return 0;

    return structWidgetList[currDbIndex]->getCurrentItem();
}

QMenu *DbManager::menu()
{
    QMenu *menu = new QMenu(tr("&Database"));
    menu->addAction(m_cmdConnectDb->action());
    menu->addSeparator();
    menu->addAction(m_cmdRefreshDb->action());
    menu->addAction(m_cmdFilterDb->action());
    menu->addSeparator();
    menu->addAction(m_cmdConnHistory->action());
    menu->addSeparator();
    menu->addAction(m_cmdCloseDb->action());
    menu->addAction(m_cmdCloseAllDb->action());
    menu->addSeparator();
    menu->addAction(m_cmdCommit->action());
    menu->addAction(m_cmdRollback->action());
    menu->addSeparator();
    menu->addAction(m_cmdExecuteCurrent->action());
    menu->addAction(m_cmdExecuteAsScript->action());
    menu->addSeparator();
    QMenu *queryTabMenu = menu->addMenu(tr("Query Tabs"));
    queryTabMenu->addAction(m_cmdQueryTabNext->action());
    queryTabMenu->addAction(m_cmdQueryTabPrev->action());
    queryTabMenu->addSeparator();
    queryTabMenu->addAction(m_cmdQueryTabClose->action());
    queryTabMenu->addAction(m_cmdQueryTabCloseAll->action());
    return menu;
}

void DbManager::initialize()
{
    Core::CommandManager *commandManager = Core::CommandManager::getInstance();

    commandManager->registerCommands( tr("DatabaseCommands"),
                                      QList<Core::Command *>() << m_cmdConnectDb
                                                               << m_cmdCloseDb
                                                               << m_cmdCloseAllDb
                                                               << m_cmdCommit
                                                               << m_cmdRollback
                                                               << m_cmdExecuteCurrent
                                                               << m_cmdExecuteAsScript
                                                               << m_cmdConnHistory
                                                               << m_cmdRefreshDb
                                                               << m_cmdFilterDb );

    commandManager->registerCommands( tr("SqlHistoryCommands"),
                                      QList<Core::Command *>() << m_cmdSqlHistoryOpen
                                                               << m_cmdSqlHistoryRemove
                                                               << m_cmdSqlHistoryClear
                                                               << m_cmdToggleHistoryEnabled
                                                               << m_cmdSqlHistoryFilter );

    commandManager->registerCommands( tr("DatabaseObjectDetails"),
                                      QList<Core::Command *>() << m_cmdDetailsClear
                                                               << m_cmdDetailsNext
                                                               << m_cmdDetailsPrev );

    commandManager->registerCommands( tr("SqlQueryTabCommands"),
                                      QList<Core::Command *>() << m_cmdQueryTabNext
                                                               << m_cmdQueryTabPrev
                                                               << m_cmdQueryTabClose
                                                               << m_cmdQueryTabCloseAll );
}

QList<UiProviderInterface *> DbManager::createUiProviders( IMainWindow *mainWindow )
{
    databaseComboBox = new QComboBox(mainWindow->widget());
    databaseComboBox->setMinimumSize(145, 22);
    dbTabWidget = new DbTabWidget(mainWindow->widget());

    m_dbDock = new QwCustomDockWidget( mainWindow->widget(), tr("DB Browser"), "DatabaseBrowserDockWidget",
                                       QKeySequence(tr("Ctrl+D,D")), QIcon(":/images/db_browser.png"),
                                       Qt::LeftDockWidgetArea, QwCustomDockWidget::First,
                                       QList<QAction *>() << m_cmdConnectDb->action()
                                                          << m_cmdCloseDb->action()
                                                          << m_cmdRefreshDb->action()
                                                          << m_cmdFilterDb->action() );
    m_dbDock->setWidget(dbTabWidget);

    QMenu *contextMenu = new QMenu;
    contextMenu->addAction(m_cmdConnectDb->action());
    contextMenu->addSeparator();
    contextMenu->addAction(m_cmdRefreshDb->action());
    contextMenu->addAction(m_cmdFilterDb->action());
    contextMenu->addSeparator();
    contextMenu->addAction(m_cmdConnHistory->action());
    contextMenu->addSeparator();
    contextMenu->addAction(m_cmdCloseDb->action());
    contextMenu->addAction(m_cmdCloseAllDb->action());
    m_dbDock->setContextMenu(contextMenu);

    QwCustomDockWidget *sqlDock = new QwCustomDockWidget( mainWindow->widget(), tr("SQL History"), "DbSqlHistoryDockWidget",
                                                          QKeySequence(tr("Ctrl+D,D")), QIcon(":/images/sql_history.png"),
                                                          Qt::BottomDockWidgetArea, QwCustomDockWidget::First,
                                                          QList<QAction *>() << m_cmdSqlHistoryOpen->action()
                                                                             << m_cmdSqlHistoryRemove->action()
                                                                             << m_cmdSqlHistoryClear->action()
                                                                             << m_cmdSqlHistoryFilter->action()
                                                                             << m_cmdToggleHistoryEnabled->action() );
    QString fileName = mainWindow->applicationDataLocation() + "/sqlhistory.xml";
    SqlHistoryInfoList *sqlList = new SqlHistoryInfoList(fileName, sqlDock);
    QSqlHistoryTableView *sqlTable = new QSqlHistoryTableView(sqlDock);
    QSqlHistoryModel *sqlModel = new QSqlHistoryModel(sqlList, sqlDock);
    QSqlHistorySortFilterProxyModel *sortModel = new QSqlHistorySortFilterProxyModel(this);
    sortModel->setSourceModel(sqlModel);
    sortModel->setDynamicSortFilter(true);

    QMenu *sqlHistContextMenu = sqlTable->contextMenu();
    sqlHistContextMenu->addAction(m_cmdSqlHistoryOpen->action());
    sqlHistContextMenu->addAction(m_cmdSqlHistoryRemove->action());
    sqlHistContextMenu->addAction(m_cmdSqlHistoryClear->action());
    sqlHistContextMenu->addSeparator();
    sqlHistContextMenu->addAction(m_cmdSqlHistoryFilter->action());
    sqlHistContextMenu->addSeparator();
    sqlHistContextMenu->addAction(m_cmdToggleHistoryEnabled->action());

    connect(this, SIGNAL(sqlHistoryEnabledChanged()), this, SLOT(onQueryViewSettingsChanged()));

    sqlTable->setModel(sortModel);
    sqlTable->setSortingEnabled(true);
    sqlTable->setSelectionMode(QAbstractItemView::SingleSelection);
    sqlTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    sqlTable->sortByColumn(1, Qt::DescendingOrder);
    sqlTable->resizeColumnsToContents();
    sqlTable->resizeRowsToContents();
    sqlDock->setWidget(sqlTable);
    sqlDock->setContextMenu(sqlHistContextMenu);

    connect(this, SIGNAL(sqlExecuted(QString)), sqlModel, SLOT(addSql(QString)));

    QComboBox *c = new QComboBox();
    c->setModel(databaseComboBox->model());
    connect(databaseComboBox, SIGNAL(currentIndexChanged(int)), c, SLOT(setCurrentIndex(int)));
    connect(c, SIGNAL(currentIndexChanged(int)), databaseComboBox, SLOT(setCurrentIndex(int)));

    m_detailsDock = new QwCustomDockWidget( mainWindow->widget(), tr("SQL-Object Details"), "DbObjectDetailsDockWidget",
                                            QKeySequence(tr("Ctrl+D,Y")), QIcon(":/images/dbb_details.png"),
                                            Qt::BottomDockWidgetArea, QwCustomDockWidget::First,
                                            QList<QAction *>(), c );
    m_detailsStack = new QStackedWidget(m_detailsDock);
    m_detailsStack->addWidget(new QLabel(tr("No Database Connections"), m_detailsStack));
    m_detailsDock->setWidget(m_detailsStack);

    m_queryDock = new QwTabBarDockWidget( mainWindow->widget(), tr("SQL-Query"), "DbQueryDockWidget",
                                          QKeySequence(tr("Ctrl+D,Y")), QIcon(":/images/dbb_query.png"),
                                          Qt::BottomDockWidgetArea, QwCustomDockWidget::First);

    Core::MainToolBar *toolBar = new Core::MainToolBar(tr("Database Browser"));
    toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    toolBar->setObjectName("DatabaseBrowserToolBar");
    toolBar->addAction(m_cmdConnectDb->action());
    toolBar->addWidget(databaseComboBox);
    toolBar->addSeparator();
    toolBar->addAction(m_cmdCloseDb->action());
    toolBar->addSeparator();
    toolBar->addAction(m_cmdCommit->action());
    toolBar->addAction(m_cmdRollback->action());
    toolBar->addSeparator();
    toolBar->addAction(m_cmdExecuteCurrent->action());
    toolBar->addAction(m_cmdExecuteAsScript->action());

    QList<UiProviderInterface *> providers;
    providers << new UiProviderInterface(m_dbDock, toolBar)
              << new UiProviderInterface(sqlDock)
              << new UiProviderInterface(m_detailsDock)
              << new UiProviderInterface(m_queryDock);

    connect(databaseComboBox,   SIGNAL(currentIndexChanged(int)),   dbTabWidget,        SLOT(setCurrentIndex(int)));
    connect(dbTabWidget,        SIGNAL(currentChanged(int)),        databaseComboBox,   SLOT(setCurrentIndex(int)));
    connect(dbTabWidget,        SIGNAL(currentChanged(int)),        this,               SLOT(onCurrentConnectionChanged(int)));

    connect(m_cmdQueryTabNext->action(),        SIGNAL(triggered()), m_queryDock, SLOT(nextTab()));
    connect(m_cmdQueryTabPrev->action(),        SIGNAL(triggered()), m_queryDock, SLOT(prevTab()));
    connect(m_cmdQueryTabClose->action(),       SIGNAL(triggered()), m_queryDock, SLOT(removeCurrentTab()));
    connect(m_cmdQueryTabCloseAll->action(),    SIGNAL(triggered()), m_queryDock, SLOT(removeAllTabs()));

    connect(databaseComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentDbIndexChanged(int)));

    connect(m_cmdSqlHistoryOpen->action(),    SIGNAL(triggered()), sqlTable, SLOT(onOpen()));
    connect(m_cmdSqlHistoryRemove->action(),  SIGNAL(triggered()), sqlTable, SLOT(onRemove()));
    connect(m_cmdSqlHistoryClear->action(),   SIGNAL(triggered()), sqlTable, SLOT(onClear()));
    connect(m_cmdSqlHistoryFilter->action(),  SIGNAL(triggered()), sqlTable, SLOT(onFilterHistory()));
    connect(m_cmdToggleHistoryEnabled->action(), SIGNAL(toggled(bool)), sqlTable, SLOT(onToggleHistoryEnabled(bool)));
    connect(sqlTable, SIGNAL(sqlHistohyItemSelected(bool)), this, SLOT(onSqlHistoryItemSelected(bool)));

    return providers;
}

bool DbManager::confirmEndStartedTransactions()
{
    for (int i = 0; i < connectionList.size(); i++)
        if ( !connectionList[i]->confirmEndStartedTransaction(m_mainWindow->widget()))
            return false;

    return true;
}

void DbManager::startTransaction( int currDbIndex )
{
    if ( currDbIndex == -1 ) {
        currDbIndex = databaseComboBox->currentIndex();
        if ( currDbIndex == -1 )
            return;
    }

    if ( !connectionList[currDbIndex]->startTransaction() )
        return;

    emit(transactionModeChanged(connectionList[currDbIndex]->transaction()));
}

void DbManager::commitTransaction( int currDbIndex )
{
    bool needSignal = true;

    if ( currDbIndex ==-1 ) {
        currDbIndex = databaseComboBox->currentIndex();
        if ( currDbIndex == -1 )
            return;
    }
    else {
        if ( currDbIndex != databaseComboBox->currentIndex() )
            needSignal = false;
    }

    if ( !connectionList[currDbIndex]->commitTransaction() )
        return;

    if ( needSignal )
        emit(transactionModeChanged(connectionList[currDbIndex]->transaction()));
}

void DbManager::rollbackTransaction( int currDbIndex )
{
    bool needSignal = true;

    if ( currDbIndex == -1 ) {
        currDbIndex = databaseComboBox->currentIndex();
        if ( currDbIndex == -1 )
            return;
    }
    else {
        if ( currDbIndex != databaseComboBox->currentIndex() )
            needSignal = false;
    }

    if ( !connectionList[currDbIndex]->rollbackTransaction() )
        return;

    if ( needSignal )
        emit(transactionModeChanged(connectionList[currDbIndex]->transaction()));
}

bool DbManager::sessionAboutToClose()
{
    return confirmEndStartedTransactions();
}

void DbManager::sessionClose()
{
    closeAllDbConnections();
}

void DbManager::saveSession(const QString &location)
{
    QString fileName = location;
    fileName += "/dbconnections.xml";

    QFile file(fileName);
    if ( !file.open(QFile::WriteOnly | QFile::Text) ) {
        m_mainWindow->showWarning( tr("Error On Save Session Db-Connections\nFile \"%1\" Cannot be Opened for Writing:\n%2.")
                                   .arg(fileName).arg(file.errorString()) );
        return;
    }

    QDomDocument domDocument("dbconnections");

    domDocument.appendChild(domDocument.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"windows-1251\""));

    QDomElement root = domDocument.createElement("dbconnections");
    root.setAttribute("version", "1.0");
    domDocument.appendChild(root);

    for (int i = 0; i < connectionInfoList.size(); i++) {
        QDomElement itemElement = domDocument.createElement("item");
        itemElement.setAttribute("dbType", connectionList[i]->loader()->databaseType());
        itemElement.setAttribute("description", connectionInfoList[i].description);
        for (int j = 0; j < connectionInfoList[i].parameters.size(); j++) {
            QDomElement parElement = domDocument.createElement("parameter");
            parElement.setAttribute("value", connectionInfoList[i].parameters[j]);
            itemElement.appendChild(parElement);
        }
        root.appendChild(itemElement);
    }

    const int IndentSize = 4;
    QTextStream out(&file);
    domDocument.save(out, IndentSize);

    file.close();
}

void DbManager::loadSession( const QString &location )
{
    QString fileName = location;
    fileName += "/dbconnections.xml";

    QFile file(fileName);
    if ( !file.exists() )
        return;

    QDomDocument domDocument;

    QString errorStr;
    int errorLine;
    int errorColumn;

    if ( !domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn) ) {
        m_mainWindow->showWarning( tr("Error On Load Session Db-Connections\nCommon Error in xml-file:\nLine %1, Column %2: %3")
                                   .arg(errorLine).arg(errorColumn).arg(errorStr) );
        return;
    }

    QDomElement root = domDocument.documentElement();
    if ( root.tagName() != "dbconnections" ) {
        m_mainWindow->showWarning( tr("Error On Load Session Db-Connections\nInvalid Format of Sessions xml-file") );
        return;
    }
    if ( !root.hasAttribute("version") ) {
        m_mainWindow->showWarning( tr("Error On Load Session Db-Connections\nInvalid Format of Sessions xml-file (version expected)") );
        return;
    }
    if ( root.attribute("version") != "1.0" ) {
        m_mainWindow->showWarning( tr("Error On Load Session Db-Connections\nInvalid Format of Sessions xml-file (wrong version)") );
        return;
    }

    QDomElement itemElement = root.firstChildElement();
    while ( !itemElement.isNull() ) {
        DbConnectionAuthInfo connInfo;
        connInfo.databaseType = itemElement.attribute("dbType");
        connInfo.description = itemElement.attribute("description");
        QDomElement parElement = itemElement.firstChildElement();
        while ( !parElement.isNull() ) {
            connInfo.parameters << parElement.attribute("value");
            parElement = parElement.nextSiblingElement();
        }
        itemElement = itemElement.nextSiblingElement();

        createConnection(connInfo, false);
    }

    file.close();
}

bool DbManager::applicationAboutToClose()
{
    return confirmEndStartedTransactions();
}

void DbManager::applicationClose()
{
    closeAllDbConnections();
}

void DbManager::destroy()
{
    delete m_connectionsHistory;
}

void DbManager::reflectOnQueryViewSettingsChanged( const QString &splitterSymbol )
{
    emit sqlHistoryEnabledChanged();
    emit clipboardSplitterChanged(splitterSymbol);
}

IDbLoader *DbManager::loaderByDatabaseType( const QString &dbType )
{
    for (int i = 0; loaderTypeList.size(); i++) {
        if ( dbType == loaderTypeList[i]->databaseType() )
            return loaderTypeList[i];
    }

    return 0;
}

QStringList DbManager::settingsKeyList() const
{
    return QStringList() << SqlBrowser::C_SQL_QUERY_SETTINGS_ID
                         << Sql::Constants::C_SQL_EDITOR_SETTINGS_ID;
}

ISettings *DbManager::createSettings( const QString &settingsKey ) const
{
    if ( settingsKey == SqlBrowser::C_SQL_QUERY_SETTINGS_ID )
        return new SettingsCategoryViewQuery();

    return 0;
}

void DbManager::onDetailsForItem( IDbCatalogItem *item, DbConnection *connection )
{
    DbDetailsWidget *dw  = currentDetailsWidget();
    if ( !dw )
        return;

    dw->addDbDetails(item->referencedItem(connection->loader(), connection->connectionInfo()));
    m_detailsDock->showAndSetCurrentIfTabified();
}

void DbManager::onDetails()
{
    int currDbIndex = databaseComboBox->currentIndex();
    if ( currDbIndex == -1 )
        return;

    IDbCatalogItem *item = structWidgetList[currDbIndex]->getCurrentItem();
    if ( !item )
        return;

    DbDetailsWidget *dw  = currentDetailsWidget(currDbIndex);
    if ( !dw )
        return;

    DbCatalogModel *objectsModel = structWidgetList[currDbIndex]->objectsModel();

    dw->addDbDetails(item->referencedItem(objectsModel->loader(), objectsModel->connectionInfo()));
    m_detailsDock->showAndSetCurrentIfTabified();
}

void DbManager::onCopyName()
{
    if ( Database::IDbCatalogItem *item = getCurrentItem() ) {
        QString name = item->name();
        if ( !name.isEmpty() ) {
            QClipboard *clipboard = QApplication::clipboard();
            clipboard->setText(name);
        }
    }
}

void DbManager::onQueryData()
{
    if ( Database::IDbCatalogItem *item = getCurrentItem() ) {
        QString name = item->name();
        if ( !name.isEmpty() ) {
            int currDbIndex = databaseComboBox->currentIndex();
            if ( currDbIndex == -1 )
                return;
            connectionList[currDbIndex]->startQueryDataNewJob(item->schemaName(), name);
        }
    }
}

void DbManager::onSqlHistoryItemSelected( bool onoff )
{
    m_cmdSqlHistoryOpen->action()->setEnabled(onoff);
    m_cmdSqlHistoryRemove->action()->setEnabled(onoff);
}

void DbManager::onQueryViewSettingsChanged()
{
    m_cmdToggleHistoryEnabled->action()->setChecked(SettingsManager::instance()->value(SqlBrowser::C_SQL_QUERY_SETTINGS_ID, "SqlHistory").toBool());
}

void DbManager::onDetailsClear()
{
    DbDetailsWidget *dw = currentDetailsWidget();
    if ( dw )
        dw->clear();
}

void DbManager::onDetailsNext()
{
    DbDetailsWidget *dw = currentDetailsWidget();
    if ( dw )
        dw->next();
}

void DbManager::onDetailsPrevious()
{
    DbDetailsWidget *dw = currentDetailsWidget();
    if ( dw )
        dw->previous();
}

void DbManager::dbItemActionTriggered( QDbItemAction *action )
{
    if ( IDbCatalogItem *item = getCurrentItem() ) {
        int currDbIndex = databaseComboBox->currentIndex();
        action->emitItemTriggered(connectionList[currDbIndex]->connectionName(), item);
    }
}

void DbManager::registerLoaderItemActions( IDbLoader *loader )
{
    foreach ( QDbItemAction *action, loader->allActions() )
        connect(action, SIGNAL(triggered()), this, SLOT(onDbItemActionTriggered()));
}

void DbManager::onDbItemActionTriggered()
{
    if ( QDbItemAction *action = qobject_cast<QDbItemAction *>(sender()) )
        dbItemActionTriggered(action);
}

bool DbManager::createConnection( const DbConnectionAuthInfo &connection, bool addToHistory )
{
    IDbLoader *loader = loaderByDatabaseType(connection.databaseType);
    if ( !loader )
        return false;

    QString dbName = QString("DB_%1").arg(dbIndex);
    dbIndex++;

    BaseConnectionInfo *connectionInfo = loader->createConnection(dbName, connection.parameters);
    if ( !connectionInfo )
        return false;
    QString dbTitle = connectionInfo->connectionTitle();

    QDbStructWidget *structWidget = new QDbStructWidget(connectionInfo, loader, m_mainWindow->widget());

    DbConnection *c = new DbConnection( loader,
                                        connectionInfo,
                                        structWidget->rootItem(),
                                        structWidget->objectsModel(),
                                        &m_sqlHelper );
    connect( c, SIGNAL(statementExecuted(QString,bool,bool,QString,QList<QList<QVariant> >*)),
             this, SLOT(onStatementExecuted(QString,bool,bool,QString,QList<QList<QVariant> >*)) );
    connect( c, SIGNAL(transactionChanged(DbConnection*)), this, SLOT(onTransactionChanged(DbConnection*)));
    connect(c, SIGNAL(connectionClosed(DbConnection*)), this, SIGNAL(connectionClosed(DbConnection*)));
    connect(c, SIGNAL(detailsRequested(IDbCatalogItem*,DbConnection*)), this, SLOT(onDetailsForItem(IDbCatalogItem*,DbConnection*)));

    connectionList.append(c);
    databaseComboBox->addItem(dbTitle);
    QString dbDescr = "<pre>" + connectionInfo->connectionDescription() + "</pre>";
    structWidgetList.append(structWidget);
    dbTabWidget->addTab(structWidget, dbTitle);
    dbTabWidget->setTabToolTip(dbTabWidget->count() - 1, dbDescr);
    DbDetailsWidget *dw = new DbDetailsWidget(c, connectionInfo, loader, dbDescr, m_qssDetailsWathcherIndex, m_detailsStack);
    m_detailsStack->addWidget(dw);
    m_detailsStack->setCurrentIndex(m_detailsStack->count() - 1);

    databaseComboBox->setCurrentIndex(databaseComboBox->count() - 1);

    emit(dbConnectionAdded(QVariantList() << dbTitle));

    m_cmdCloseAllDb->action()->setEnabled(true);
    m_cmdCloseDb->action()->setEnabled(true);
    m_cmdRefreshDb->action()->setEnabled(true);
    m_cmdFilterDb->action()->setEnabled(true);

    DbConnectionAuthInfo connInfo;
    connInfo.databaseType = loader->databaseType();
    connInfo.description = connectionInfo->connectionDescription();
    connInfo.parameters << connection.parameters;

    if ( addToHistory )
        m_connectionsHistory->addConnection(connInfo);

    connectionInfoList.append(connInfo);

    return true;
}

void DbManager::onStatementExecuted( const QString &queryString, bool success, bool isSelectStattement, const QString &message, QList <QList <QVariant> > *queryData )
{
    QWidget *tabWidget = 0;

    if ( !isSelectStattement || !success ) {
        QTextEdit *textEdit = new QTextEdit();
        textEdit->setReadOnly(true);
        textEdit->setText(message);
        tabWidget = textEdit;
    }
    else {
        QString splitter = SettingsManager::instance()->value(SqlBrowser::C_SQL_QUERY_SETTINGS_ID, "SplitterSymbol").toString();
        Utils::QwTableView *queryView = new Utils::QwTableView(splitter);
        DbQueryModel *queryModel = new DbQueryModel(queryData);
        connect(this, SIGNAL(clipboardSplitterChanged(QString)), queryView, SLOT(clipboardSplitterChanged(QString)));
        queryView->setModel(queryModel);
        const int rowsHeight = SettingsManager::instance()->value(SqlBrowser::C_SQL_QUERY_SETTINGS_ID, "DefaultRowHeight").toInt();
        queryView->verticalHeader()->setDefaultSectionSize(rowsHeight);
        if ( !SettingsManager::instance()->value(SqlBrowser::C_SQL_QUERY_SETTINGS_ID, "ShowVerticalHeader").toBool() )
            queryView->verticalHeader()->hide();
        QFont font = qvariant_cast<QFont>(SettingsManager::instance()->value(SqlBrowser::C_SQL_QUERY_SETTINGS_ID, "Font"));
        queryView->setFont(font);
        if ( SettingsManager::instance()->value(SqlBrowser::C_SQL_QUERY_SETTINGS_ID, "AutoResizeColumns").toBool() ) {
            queryView->resizeColumnsToContents();
            const int maxColWidth = SettingsManager::instance()->value(SqlBrowser::C_SQL_QUERY_SETTINGS_ID, "MaxColumnsWidth").toInt();
            for (int i = 0; i < queryModel->columnCount(QModelIndex()); i++ )
                if ( queryView->columnWidth(i) > maxColWidth )
                    queryView->setColumnWidth(i, maxColWidth);
        }
        tabWidget = queryView;
    }

    m_queryDock->addTab( success ? QIcon(":/images/qr_ok.png") : QIcon(":/images/qr_error.png"),
                         queryString.left(18).replace('\n', ' '),
                         "<pre>" + queryString.trimmed().replace("<", "&lt;") + "</pre>",
                         tabWidget );
    m_queryDock->showAndSetCurrentIfTabified();

    bool allowBadStatementsHistory = false; /// TODO: from settings
    if ( success || allowBadStatementsHistory )
        emit sqlExecuted(queryString);
}

void DbManager::onTransactionChanged( DbConnection *connection )
{
    int currDbIndex = databaseComboBox->currentIndex();
    if ( currDbIndex == -1 )
        return;

    if ( connectionList[currDbIndex] == connection )
        emit transactionModeChanged(connection->transaction());
}

void DbManager::executeQueryInternal( int executeMode )
{
    int index = databaseComboBox->currentIndex();
    if ( index != -1 )
        connectionList[index]->startNewJob( executeMode == 1 ? Database::DocumentSelectedText : Database::AllDocument);
}

bool DbManager::isSqlDocument( IDocument *document )
{
    return ( document && document->documentPlugin()->defaultExtension() == QLatin1String("SQL") );
}

DbDetailsWidget *DbManager::currentDetailsWidget( int index )
{
    if ( index == -1 )
        index = databaseComboBox->currentIndex();
    if ( index == -1 )
        return 0;

    return (static_cast<DbDetailsWidget *>(m_detailsStack->widget(index + 1)));
}

void DbManager::loadLoaderPlugins()
{
    loaderTypeList.clear();

    QDir pluginsDir;
    pluginsDir = QDir(qApp->applicationDirPath());
    pluginsDir.cd("dbproviders");

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if ( plugin ) {
            IDbLoader *iLoader = qobject_cast<IDbLoader *>(plugin);
            if ( iLoader ) {
                loaderTypeList.append(iLoader);
                registerLoaderItemActions(iLoader);
            }
        }
    }
}

void DbManager::createActions()
{
    QMainWindow *mainWindow = m_mainWindow->widget();

    m_cmdConnectDb      = new Core::Command( tr("Connect"),
                                             QIcon(":/images/db_connect.png"),
                                             tr("Connect To DB"),
                                             tr("Connect To Database"),
                                             QKeySequence(tr("Ctrl+Alt+O")),
                                             mainWindow );

    m_cmdCloseDb        = new Core::Command( tr("Close"),
                                             QIcon(":/images/db_close.png"),
                                             tr("Close Connection"),
                                             tr("Close Current Database Connection"),
                                             QKeySequence(tr("Ctrl+Alt+W")),
                                             mainWindow );

    m_cmdCloseAllDb     = new Core::Command( tr("CloseAll"),
                                             tr("Close All Connections"),
                                             tr("Close All Database Connections"),
                                             QKeySequence(tr("Ctrl+Alt+Shift+W")),
                                             mainWindow );

    m_cmdCommit         = new Core::Command( tr("Commit"),
                                             QIcon(":/images/db_commit.png"),
                                             tr("Commit"),
                                             tr("Commit Started Transaction"),
                                             QKeySequence(tr("Alt+F8")),
                                             mainWindow );

    m_cmdRollback       = new Core::Command( tr("Rollback"),
                                             QIcon(":/images/db_rollback.png"),
                                             tr("Rollback"),
                                             tr("Rollback Started Transaction"),
                                             QKeySequence(tr("Alt+Shift+F8")),
                                             mainWindow );

    m_cmdExecuteCurrent = new Core::Command( tr("Execute"),
                                             QIcon(":/images/act_execute_curr.png"),
                                             tr("Execute"),
                                             tr("Execute Selected Queries"),
                                             QKeySequence(tr("F8")),
                                             mainWindow );

    m_cmdExecuteAsScript= new Core::Command( tr("ExecuteAsScript"),
                                             QIcon(":/images/act_execute.png"),
                                             tr("Execute As Script"),
                                             tr("Execute All Document Queries"),
                                             QKeySequence(tr("Ctrl+F8")),
                                             mainWindow );

    m_cmdConnHistory    = new Core::Command( tr("ConnectionsHistory"),
                                             tr("Connections History"),
                                             tr("Connections History"),
                                             QKeySequence(tr("Alt+Shift+O")),
                                             mainWindow );

    m_cmdRefreshDb      = new Core::Command( tr("RefreshDatabase"),
                                             QIcon(":/images/dbb_refresh.png"),
                                             tr("Refresh"),
                                             tr("Refresh"),
                                             QKeySequence(tr("Alt+Shift+R")),
                                             mainWindow );

    m_cmdFilterDb       = new Core::Command( tr("FilterDatabase"),
                                             QIcon(":/images/sql_his_filter.png"),
                                             tr("Filter"),
                                             tr("Filter"),
                                             QKeySequence(tr("Alt+Shift+F")),
                                             mainWindow );

    m_cmdDetailsClear   = new Core::Command( tr("DetailsClear"),
                                             QIcon(":/images/qr_tab_close_all.png"),
                                             tr("Clear"),
                                             tr("Clear All Details Tabs"),
                                             QKeySequence(),
                                             mainWindow );

    m_cmdDetailsNext    = new Core::Command( tr("DetailsNext"),
                                             QIcon(":/images/qr_tab_next.png"),
                                             tr("Next"),
                                             tr("Select Next Datails Tab"),
                                             QKeySequence(),
                                             mainWindow );

    m_cmdDetailsPrev    = new Core::Command( tr("DetailsPrev"),
                                             QIcon(":/images/qr_tab_prev.png"),
                                             tr("Previous"),
                                             tr("Select Previous Datails Tab"),
                                             QKeySequence(),
                                             mainWindow );

    m_cmdCloseDb->action()->setEnabled(false);
    m_cmdCloseAllDb->action()->setEnabled(false);
    m_cmdCommit->action()->setEnabled(false);
    m_cmdRollback->action()->setEnabled(false);
    m_cmdExecuteCurrent->action()->setEnabled(false);
    m_cmdExecuteAsScript->action()->setEnabled(false);
    m_cmdRefreshDb->action()->setEnabled(false);
    m_cmdFilterDb->action()->setEnabled(false);

    connect(m_cmdConnectDb->action(),   SIGNAL(triggered()), this, SLOT(connectToDb()));
    connect(m_cmdCloseDb->action(),     SIGNAL(triggered()), this, SLOT(closeDbConnection()));
    connect(m_cmdCloseAllDb->action(),  SIGNAL(triggered()), this, SLOT(closeAllDbConnections()));

    connect(m_cmdCommit->action(),      SIGNAL(triggered()), this, SLOT(commitTransaction()));
    connect(m_cmdRollback->action(),    SIGNAL(triggered()), this, SLOT(rollbackTransaction()));

    connect(m_cmdExecuteCurrent->action(),    SIGNAL(triggered()), this, SLOT(executeQueryCurrent()));
    connect(m_cmdExecuteAsScript->action(),   SIGNAL(triggered()), this, SLOT(executeQueryAsScript()));

    connect(m_cmdConnHistory->action(),       SIGNAL(triggered()), this, SLOT(onConnectionsHistory()));

    connect(this,           SIGNAL(transactionModeChanged(bool)), m_cmdCommit->action(),    SLOT(setEnabled(bool)));
    connect(this,           SIGNAL(transactionModeChanged(bool)), m_cmdRollback->action(),  SLOT(setEnabled(bool)));

    connect(m_cmdRefreshDb->action(),   SIGNAL(triggered()), this, SLOT(onRefresh()));
    connect(m_cmdFilterDb->action(),    SIGNAL(triggered()), this, SLOT(onFilter()));

    connect(m_cmdDetailsClear->action(),    SIGNAL(triggered()), this, SLOT(onDetailsClear()));
    connect(m_cmdDetailsNext->action(),     SIGNAL(triggered()), this, SLOT(onDetailsNext()));
    connect(m_cmdDetailsPrev->action(),     SIGNAL(triggered()), this, SLOT(onDetailsPrevious()));
}

void DbManager::createContextMenuActions()
{
    QMainWindow *mainWindow = m_mainWindow->widget();

    m_actDetails = new QAction(tr("Details"), mainWindow);
    QFont font = m_actDetails->font();
    font.setBold(true);
    m_actDetails->setFont(font);

    m_actCopyName = new QAction(tr("Copy Name to Clipboard"), mainWindow);
    m_actQueryData = new QAction(tr("Query Data"), mainWindow);

    connect(m_actDetails,   SIGNAL(triggered()), this, SLOT(onDetails()));
    connect(m_actCopyName,  SIGNAL(triggered()), this, SLOT(onCopyName()));
    connect(m_actQueryData, SIGNAL(triggered()), this, SLOT(onQueryData()));

    m_DbItemActions << m_actDetails << m_actCopyName << m_actQueryData;
}

void DbManager::createSqlHistoryActions()
{
    QMainWindow *mainWindow = m_mainWindow->widget();

    m_cmdSqlHistoryOpen         = new Core::Command( tr("Insert"),
                                                     QIcon(":/images/sql_his_open.png"),
                                                     tr("Insert Into Editor"),
                                                     tr("Insert Selected Query Into Dcoument"),
                                                     QKeySequence(),
                                                     mainWindow );

    m_cmdSqlHistoryRemove       = new Core::Command( tr("Remove"),
                                                     QIcon(":/images/sql_his_remove.png"),
                                                     tr("Remove"),
                                                     tr("Remove Selected Query From History"),
                                                     QKeySequence(),
                                                     mainWindow );

    m_cmdSqlHistoryClear        = new Core::Command( tr("Clear"),
                                                     QIcon(":/images/sql_his_clear.png"),
                                                     tr("Clear"),
                                                     tr("Remove All Queries From History"),
                                                     QKeySequence(),
                                                     mainWindow );

    m_cmdToggleHistoryEnabled   = new Core::Command( tr("Enabled"),
                                                     QIcon(":/images/cre_select_all.png"),
                                                     tr("Enabled History"),
                                                     tr("Toggle Enabled SQL-History"),
                                                     QKeySequence(),
                                                     mainWindow );

    m_cmdSqlHistoryFilter       = new Core::Command( tr("Filter"),
                                                     QIcon(":/images/sql_his_filter.png"),
                                                     tr("Filter"),
                                                     tr("Change Filter Parameters"),
                                                     QKeySequence(),
                                                     mainWindow );

    m_cmdToggleHistoryEnabled->action()->setCheckable(true);

    m_cmdSqlHistoryOpen->action()->setEnabled(false);
    m_cmdSqlHistoryRemove->action()->setEnabled(false);
}

void DbManager::createQueryTabActions()
{
    QMainWindow *mainWindow = m_mainWindow->widget();

    m_cmdQueryTabNext           = new Core::Command( tr("QueryTabNext"),
                                                     QIcon(":/images/qr_tab_next.png"),
                                                     tr("Next Tab"),
                                                     tr("Show Next Query Tab"),
                                                     QKeySequence(),
                                                     mainWindow );

    m_cmdQueryTabPrev           = new Core::Command( tr("QueryTabPrev"),
                                                     QIcon(":/images/qr_tab_prev.png"),
                                                     tr("Previous Tab"),
                                                     tr("Show Previous Query Tab"),
                                                     QKeySequence(),
                                                     mainWindow );

    m_cmdQueryTabClose          = new Core::Command( tr("QueryTabClose"),
                                                     QIcon(":/images/qr_tab_close.png"),
                                                     tr("Close"),
                                                     tr("Close Current Query Tab"),
                                                     QKeySequence(),
                                                     mainWindow );

    m_cmdQueryTabCloseAll       = new Core::Command( tr("QueryTabCloseAll"),
                                                     QIcon(":/images/qr_tab_close_all.png"),
                                                     tr("Close All"),
                                                     tr("Close All Query Tabs"),
                                                     QKeySequence(),
                                                     mainWindow );
}

void DbManager::connectToDb()
{
    ConnectDialog connDialog(loaderTypeList, m_mainWindow->widget());
    if ( connDialog.exec() != QDialog::Accepted )
        return;

    int connType = connDialog.getConnectionType();
    if ( connType == -1 )
        return;

    QString dbName = QString("DB_%1").arg(dbIndex);
    dbIndex++;
    QStringList parameters = connDialog.getConnectionParameters();

    BaseConnectionInfo *connectionInfo = loaderTypeList[connType]->createConnection(dbName, parameters);
    if ( !connectionInfo )
        return;
    QString dbTitle = connectionInfo->connectionTitle();

    QDbStructWidget *structWidget = new QDbStructWidget(connectionInfo, loaderTypeList[connType], m_mainWindow->widget());

    DbConnection *c = new DbConnection( loaderTypeList[connType],
                                        connectionInfo,
                                        structWidget->rootItem(),
                                        structWidget->objectsModel(),
                                        &m_sqlHelper );
    connect( c, SIGNAL(statementExecuted(QString,bool,bool,QString,QList<QList<QVariant> >*)),
             this, SLOT(onStatementExecuted(QString,bool,bool,QString,QList<QList<QVariant> >*)) );
    connect( c, SIGNAL(transactionChanged(DbConnection*)), this, SLOT(onTransactionChanged(DbConnection*)));
    connect(c, SIGNAL(connectionClosed(DbConnection*)), this, SIGNAL(connectionClosed(DbConnection*)));
    connect(c, SIGNAL(detailsRequested(IDbCatalogItem*,DbConnection*)), this, SLOT(onDetailsForItem(IDbCatalogItem*,DbConnection*)));

    connectionList.append(c);
    databaseComboBox->addItem(dbTitle);
    QString dbDescr = "<pre>" + connectionInfo->connectionDescription() + "</pre>";
    structWidgetList.append(structWidget);
    dbTabWidget->addTab(structWidget, dbTitle);
    dbTabWidget->setTabToolTip(dbTabWidget->count() - 1, dbDescr);
    DbDetailsWidget *dw = new DbDetailsWidget(c, connectionInfo, loaderTypeList[connType], dbDescr, m_qssDetailsWathcherIndex, m_detailsStack);
    m_detailsStack->addWidget(dw);
    m_detailsStack->setCurrentIndex(m_detailsStack->count() - 1);
    connect(structWidget, SIGNAL(currentRowChanged(QModelIndex)), dw, SLOT(onCurrentIndexChanged(QModelIndex)));

    databaseComboBox->setCurrentIndex(databaseComboBox->count() - 1);

    emit(dbConnectionAdded(QVariantList() << dbTitle));

    m_cmdCloseAllDb->action()->setEnabled(true);
    m_cmdCloseDb->action()->setEnabled(true);
    m_cmdRefreshDb->action()->setEnabled(true);
    m_cmdFilterDb->action()->setEnabled(true);

    DbConnectionAuthInfo connInfo;
    connInfo.databaseType = loaderTypeList[connType]->databaseType();
    connInfo.description = connectionInfo->connectionDescription();
    connInfo.parameters << parameters;
    m_connectionsHistory->addConnection(connInfo);

    connectionInfoList.append(connInfo);

    m_dbDock->showAndSetCurrentIfTabified();
}

bool DbManager::closeConnection( int index )
{
    if ( !connectionList[index]->closeConnection(m_mainWindow->widget()) )
        return false;

    DbDetailsWidget *dw = currentDetailsWidget(index);
    if ( dw )
        dw->clear();

    databaseComboBox->removeItem(index);
    structWidgetList.removeAt(index);
    dbTabWidget->removeTab(index);

    DbConnection *connection = connectionList[index];
    delete connection;
    connectionList.removeAt(index);
    connectionInfoList.removeAt(index);

    QWidget *delWidget = m_detailsStack->widget(index + 1);
    m_detailsStack->removeWidget(delWidget);
    delWidget->deleteLater();

    emit(dbConnectionClosed(QVariantList() << index));

    if ( !databaseComboBox->count() ) {
        m_cmdCloseDb->action()->setEnabled(false);
        m_cmdCloseAllDb->action()->setEnabled(false);
        m_cmdRefreshDb->action()->setEnabled(false);
        m_cmdFilterDb->action()->setEnabled(false);
    }

    return true;
}

bool DbManager::closeDatabaseConnection( DbConnection *connection )
{
    for (int i = 0; i < connectionList.size(); i++)
        if ( connection == connectionList[i] )
            return closeConnection(i);

    return false;
}

void DbManager::closeDbConnection()
{
    int index = databaseComboBox->currentIndex();
    if ( index == -1 )
        return;

    closeConnection(index);
}

void DbManager::closeAllDbConnections()
{
    int index = databaseComboBox->count() - 1;

    while ( index >= 0 ) {
        if ( !connectionList[index]->closeConnection(m_mainWindow->widget()) )
            return;
        databaseComboBox->removeItem(index);
        dbTabWidget->removeTab(index);

        DbConnection *connection = connectionList[index];
        delete connection;
        connectionList.removeAt(index);
        connectionInfoList.removeAt(index);
        structWidgetList.removeAt(index);

        QWidget *delWidget = m_detailsStack->widget(index + 1);
        m_detailsStack->removeWidget(delWidget);
        delWidget->deleteLater();

        emit(dbConnectionClosed(QVariantList() << index));

        index--;
    }

    dbIndex = 0;

    m_cmdCloseDb->action()->setEnabled(false);
    m_cmdCloseAllDb->action()->setEnabled(false);
    m_cmdRefreshDb->action()->setEnabled(false);
    m_cmdFilterDb->action()->setEnabled(false);
}

void DbManager::onConnectionsHistory()
{
    DbConnectionAuthInfo connection;
    if ( !m_connectionsHistory->selectOrEditConnection(connection) )
        return;

    if ( createConnection(connection) ) {
        if ( m_dbDock->isVisible() )
            m_dbDock->showAndSetCurrentIfTabified();
    }
}

void DbManager::executeQueryCurrent()
{
    executeQueryInternal(1);
}

void DbManager::executeQueryAsScript()
{
    executeQueryInternal(0);
}

void DbManager::onCurrentConnectionChanged( int index )
{
    bool execEnabled = true;

    if ( index == -1 ) {
        emit(transactionModeChanged(false));
        QVariantList tmp;
        emit(currentDbConnectionChanged(tmp));
        execEnabled = false;
    }
    else {
        emit(transactionModeChanged(connectionList[index]->transaction()));
        emit( currentDbConnectionChanged( QVariantList() << qVariantFromValue((void *) connectionList[index]) ) );
        IDocument *document = DocumentManager::getInstance()->currentDocument();
        execEnabled = isSqlDocument(document);
    }

    IDbLoader *loader = 0;
    if ( index != -1 )
        loader = structWidgetList[index]->getCurrentLoader();

    SqlParserEngineManager::instance()->onCurrentEngineChanged(loader ? loader->parserEngine() : 0);
    SqlParserEngineManager::instance()->onCurrentConnectionChanged(index == -1 ? 0 : connectionList[index]);

    m_cmdExecuteCurrent->action()->setEnabled(execEnabled);
    m_cmdExecuteAsScript->action()->setEnabled(execEnabled);
}

void DbManager::onCurrentDocumentChanged( IDocument *document )
{
    bool execEnabled = ( isSqlDocument(document) && databaseComboBox->currentIndex() != -1 );
    m_cmdExecuteCurrent->action()->setEnabled(execEnabled);
    m_cmdExecuteAsScript->action()->setEnabled(execEnabled);
}

void DbManager::onRefresh()
{
    int index = databaseComboBox->currentIndex();
    if ( index == -1 )
        return;

    connectionList[index]->waitDetailsForFinished();

    if ( !connectionList[index]->noJobs() ) {
        m_mainWindow->showWarning(tr("There are executed Jobs."));
        return;
    }

    QDbStructWidget *refreshedStructWidget = structWidgetList[index];
    DbConnection *refreshedConnection = connectionList[index];

    DbDetailsWidget *dw  = currentDetailsWidget(index);
    if ( dw )
        dw->clear();

    refreshedStructWidget->refreshDbModel(refreshedConnection->filter());

    refreshedConnection->setDbModel(refreshedStructWidget->objectsModel());
    refreshedConnection->setDbRootItem(refreshedStructWidget->rootItem());
}

void DbManager::onFilter()
{
    int index = databaseComboBox->currentIndex();
    if ( index == -1 )
        return;

    Database::DbFilter *filter = connectionList[index]->filter();

    DbFilterDialog filterDialog(filter, m_mainWindow->widget());
    if ( filterDialog.exec() != QDialog::Accepted )
        return;

    structWidgetList[index]->updateFilterModel(filter);
}

void DbManager::onCurrentDbIndexChanged(int index)
{
    m_detailsStack->setCurrentIndex(index + 1);
}

DatabaseCursor::DatabaseCursor( const QString &dbName, const QString &queryText, const QMap<QString, QVariant> &bindings )
    : QObject(0)
    , m_dbName(dbName)
    , m_queryText(queryText)
    , m_bindings(bindings)
    , m_valid(false)
    , m_query(0)
    , m_lastError("")
{
    QSqlDatabase db = QSqlDatabase::database(dbName);
    if ( db.isOpen() ) {
        m_query = new QSqlQuery(db);
        m_query->setForwardOnly(true);
        if ( m_query->prepare(queryText) ) {
            for ( QMap<QString, QVariant>::iterator it = m_bindings.begin();
                  it != m_bindings.end(); ++it ) {
                m_query->bindValue(it.key(), it.value());
            }
            if ( m_queryText.startsWith("SELECT", Qt::CaseInsensitive) ) {
                if ( m_query->exec() ) {
                    m_valid = true;
                }
                else
                    m_lastError = m_query->lastError().text();
            }
            else
                m_lastError = tr("Cursor Statement is Not Select Statement");
        }
        else
            m_lastError = m_query->lastError().text();
    }
    else
        m_lastError = tr("Database connection \"%1\" is not opened").arg(dbName);
}

DatabaseCursor::~DatabaseCursor()
{
    if ( m_query ) {
        delete m_query;
    }
}

bool DatabaseCursor::isValid() const
{
    return m_valid;
}

QString DatabaseCursor::lastError() const
{
    if ( m_lastError.isEmpty() && m_query )
        return m_query->lastError().text();

    return m_lastError;
}

bool DatabaseCursor::next()
{
    if ( !m_valid )
        return false;

    return m_query->next();
}

QVariant DatabaseCursor::value( int column )
{
    if ( !m_valid )
        return QVariant();

    return m_query->value(column);
}

int DatabaseCursor::columnCount()
{
    if ( m_valid )
        return m_query->record().count();

    return 0;
}

QString DatabaseCursor::columnNameAt( int index )
{
    if ( index >= 0 && index < columnCount() )
        return m_query->record().fieldName(index);

    return QString();
}

void DatabaseCursor::close()
{
    if ( m_query ) {
        m_query->clear();
        delete m_query;
        m_query = 0;
        m_valid = false;
        m_lastError = tr("Cursor Closed");
    }
}

DatabaseConnection::DatabaseConnection(Core::PythonQtStorageDestroyDelegate *delegate, DbConnection *connection )
    : Core::PythonQtObject(delegate, connection)
    , m_manager(DbManager::getInstance())
    , m_connection(connection)
{ }

DatabaseConnection::~DatabaseConnection()
{
    qDeleteAll(m_cursors);
    m_cursors.clear();
}

//bool DatabaseConnection::isValid( bool showMessage )
//{
//    if ( !m_connection ) {
//        if ( showMessage )
//            Core::Storage::warning(tr("Database Connection No Longer Valid"));
//        return false;
//    }

//    return true;
//}

bool DatabaseConnection::close()
{
    if ( !isValid() )
        return false;

    return m_manager->closeDatabaseConnection(m_connection);
}

QStringList DatabaseConnection::schemas()
{
    if ( !isValid() )
        return QStringList();

    QList<IDbCatalogItem *> *items = m_connection->schemas();
    if ( !items )
        return QStringList();

    QStringList result;
    foreach ( IDbCatalogItem *item, *items )
        result << item->name();

    return result;
}

QStringList DatabaseConnection::tables( const QString &schemaName )
{
    if ( !isValid() )
        return QStringList();

    return schemaObjectNames(schemaName, Sql::Constants::DbSchemaObjectType::C_TABLE);
}

QStringList DatabaseConnection::views( const QString &schemaName )
{
    if ( !isValid() )
        return QStringList();

    return schemaObjectNames(schemaName, Sql::Constants::DbSchemaObjectType::C_VIEW);
}

QStringList DatabaseConnection::indexes( const QString &schemaName )
{
    if ( !isValid() )
        return QStringList();

    return schemaObjectNames(schemaName, Sql::Constants::DbSchemaObjectType::C_INDEX);
}

QStringList DatabaseConnection::triggers( const QString &schemaName )
{
    if ( !isValid() )
        return QStringList();

    return schemaObjectNames(schemaName, Sql::Constants::DbSchemaObjectType::C_TRIGGER);
}

QStringList DatabaseConnection::constraints( const QString &schemaName )
{
    if ( !isValid() )
        return QStringList();

    return schemaObjectNames(schemaName, Sql::Constants::DbSchemaObjectType::C_CONSTRAINT);
}

QStringList DatabaseConnection::schemaObjectNames( const QString &schemaName, unsigned objectType )
{
    if ( !isValid() )
        return QStringList();

    QList<IDbCatalogItem *> *items = m_connection->schemaObjects(schemaName, objectType);
    if ( !items )
        return QStringList();

    QStringList result;
    foreach ( IDbCatalogItem *item, *items )
        result << item->name();

    return result;
}

QStringList DatabaseConnection::tableColumns( const QString &tableName, const QString &schemaName )
{
    if ( !isValid() )
        return QStringList();

    return schemaObjectObjectNames( schemaName, tableName,
                                    Sql::Constants::DbSchemaObjectType::C_TABLE,
                                    Sql::Constants::DbSchemaObjectSubtype::C_TABLE_COLUMN );
}

QStringList DatabaseConnection::viewColumns( const QString &viewName, const QString &schemaName )
{
    if ( !isValid() )
        return QStringList();

    return schemaObjectObjectNames( schemaName, viewName,
                                    Sql::Constants::DbSchemaObjectType::C_VIEW,
                                    Sql::Constants::DbSchemaObjectSubtype::C_VIEW_COLUMN );
}

QStringList DatabaseConnection::tableIndexes( const QString &tableName, const QString &schemaName )
{
    if ( !isValid() )
        return QStringList();

    return schemaObjectObjectNames( schemaName, tableName,
                                    Sql::Constants::DbSchemaObjectType::C_TABLE,
                                    Sql::Constants::DbSchemaObjectSubtype::C_TABLE_INDEX );
}

QStringList DatabaseConnection::tableTriggers( const QString &tableName, const QString &schemaName )
{
    if ( !isValid() )
        return QStringList();

    return schemaObjectObjectNames( schemaName, tableName,
                                    Sql::Constants::DbSchemaObjectType::C_TABLE,
                                    Sql::Constants::DbSchemaObjectSubtype::C_TABLE_TRIGGER );
}

QStringList DatabaseConnection::tableConstraints( const QString &tableName, const QString &schemaName )
{
    if ( !isValid() )
        return QStringList();

    return schemaObjectObjectNames( schemaName, tableName,
                                    Sql::Constants::DbSchemaObjectType::C_TABLE,
                                    Sql::Constants::DbSchemaObjectSubtype::C_TABLE_CONSTRAINT );
}

QStringList DatabaseConnection::schemaObjectObjectNames( const QString &schemaName, const QString &objectName, unsigned objectType, unsigned objectSubtype )
{
    if ( !isValid() )
        return QStringList();

    QList<IDbCatalogItem *> *items = m_connection->schemaObjectObjects(schemaName, objectName, objectType, objectSubtype);
    if ( !items )
        return QStringList();

    QStringList result;
    foreach ( IDbCatalogItem *item, *items )
        result << item->name();

    return result;
}

DatabaseCursor *DatabaseConnection::openCursor( const QString &queryText, const QMap<QString, QVariant> &bindings )
{
    if ( !isValid() )
        return 0;

    DatabaseCursor *cursor = new DatabaseCursor(m_connection->connectionName(), queryText, bindings);
    m_cursors.append(cursor);

    if ( !cursor->isValid() )
        Core::Storage::mainWindow()->pythonReport(tr("Cursor Cannot be Opened or Executed:\n%1").arg(cursor->lastError()));

    return cursor;
}

void DatabaseConnection::clear()
{
    m_connection = 0;

    qDeleteAll(m_cursors);
    m_cursors.clear();
}

DatabaseManagerPythonModule::DatabaseManagerPythonModule( DbManager *manager )
    : IPythonModule("QwDatabaseManager", "databaseManager", "Database Manager public methods")
    , m_manager(manager)
{
    m_functions << new Python::PythonModuleFunction("databaseTypesSupported", "Get List of All Database Types Supported")
                << new Python::PythonModuleFunction("createConnection", "Create New Database Connection")
                << new Python::PythonModuleFunction("newConnection", "Create New Database Connection (GUI-Version)")
                << new Python::PythonModuleFunction("newConnectionFromHistory", "Create New Database Connection From History")
                << new Python::PythonModuleFunction("closeConnection", "Close Database Connection")
                << new Python::PythonModuleFunction("closeAllConnections", "Close All Opened Database Connections")
                << new Python::PythonModuleFunction("currentConnectionIndex", "Get Index (-1 if no Connections Exists) of Current Connection")
                << new Python::PythonModuleFunction("setCurrentConnectionIndex", "Set Index of Current Database Connection")
                << new Python::PythonModuleFunction("connectionsCount", "Get Database Connections Count")
                << new Python::PythonModuleFunction("currentConnection", "Get Connection Object for Current Database Connection")
                << new Python::PythonModuleFunction("connectionAt", "Get Connection Object for Database Connection with Index-Argument")
                << new Python::PythonModuleFunction("openCursor", "Create and Open Cursor for Select Statement");
}

DatabaseManagerPythonModule::~DatabaseManagerPythonModule()
{
    qDeleteAll(m_functions);
}

QList<Python::PythonModuleFunction *> DatabaseManagerPythonModule::functions() const
{
    return m_functions;
}

QString DatabaseManagerPythonModule::sourceCodeForAssist() const
{
    return QString("class DatabaseCursor():\n"
                   "    def lastError(self):\n"
                   "        return 'Ok'\n"
                   "    def next(self):\n"
                   "        return True\n"
                   "    def value(self, column):\n"
                   "        return 'v'\n"
                   "    def close(self):\n"
                   "        return None\n"
                   "    def columnCount(self):\n"
                   "        return 0\n"
                   "    def columnNameAt(self, column):\n"
                   "        return ''\n"
                   "class DatabaseConnection():\n"
                   "    def close(self):\n"
                   "        return True\n"
                   "    def tables(self, schemaName = ''):\n"
                   "        return ['t1', 't2', 't3']\n"
                   "    def views(self, schemaName = ''):\n"
                   "        return ['t1', 't2', 't3']\n"
                   "    def indexes(self, schemaName = ''):\n"
                   "        return ['t1', 't2', 't3']\n"
                   "    def triggers(self, schemaName = ''):\n"
                   "        return ['t1', 't2', 't3']\n"
                   "    def constraints(self, schemaName = ''):\n"
                   "        return ['t1', 't2', 't3']\n"
                   "    def schemaObjectNames(self, schemaName, objectType):\n"
                   "        return ['t1', 't2', 't3']\n"
                   "    def tableColumns(self, tableName, schemaName = ''):\n"
                   "        return ['t1', 't2', 't3']\n"
                   "    def viewColumns(self, viewName, schemaName = ''):\n"
                   "        return ['t1', 't2', 't3']\n"
                   "    def tableIndexes(self, tableName, schemaName = ''):\n"
                   "        return ['t1', 't2', 't3']\n"
                   "    def tableTriggers(self, tableName, schemaName = ''):\n"
                   "        return ['t1', 't2', 't3']\n"
                   "    def tableConstraints(self, tableName, schemaName = ''):\n"
                   "        return ['t1', 't2', 't3']\n"
                   "    def schemaObjectObjectNames(self, schemaName, objectName, objectType, objectSubtype):\n"
                   "        return ['t1', 't2', 't3']\n"
                   "    def tableColumnInfo(self, tableName, columnName , schemaName = ''):\n"
                   "        return ['SMALLINT', 2, 'A FIELD']\n"
                   "    def openCursor(self, queryText, bindings):\n"
                   "        return DatabaseCursor()\n"
                   "class QwDatabaseManager():\n"
                   "    def databaseTypesSupported(self):\n"
                   "        return ['a', 'b', 'c']\n"
                   "    def createConnection(self, dbType, parameters, addToConnectionHistory):\n"
                   "        return DatabaseConnection()\n"
                   "    def newConnection(self):\n"
                   "        return DatabaseConnection()\n"
                   "    def newConnectionFromHistory(self):\n"
                   "        return DatabaseConnection()\n"
                   "    def closeConnection(self, index):\n"
                   "        return True\n"
                   "    def closeAllConnections(self):\n"
                   "        return True\n"
                   "    def currentConnectionIndex(self):\n"
                   "        return 0\n"
                   "    def setCurrentConnectionIndex(self, index):\n"
                   "        return True\n"
                   "    def connectionsCount(self):\n"
                   "        return 0\n"
                   "    def currentConnection(self):\n"
                   "        return DatabaseConnection()\n"
                   "    def connectionAt(self, index):\n"
                   "        return DatabaseConnection()\n");
}

QMetaObject *DatabaseManagerPythonModule::metaObjectForFunction( const QString &functionName ) const
{
    if ( functionName == QLatin1String("currentConnection") ||
         functionName == QLatin1String("connectionAt") ||
         functionName == QLatin1String("newConnection") ||
         functionName == QLatin1String("newConnectionFromHistory") ||
         functionName == QLatin1String("createConnection") )
        return const_cast<QMetaObject *>(&(DatabaseConnection::staticMetaObject));

    return 0;
}

QStringList DatabaseManagerPythonModule::databaseTypesSupported()
{
    QStringList result;
    foreach ( IDbLoader *loader, m_manager->loaderTypeList )
        result << loader->databaseType();
    return result;
}

DatabaseConnection *DatabaseManagerPythonModule::createConnection( const QString &dbType, const QStringList &parameters, bool addToConnectionHistory )
{
    if ( !m_manager->createConnection(DbConnectionAuthInfo(dbType, parameters), addToConnectionHistory) ) {
        Core::Storage::mainWindow()->pythonReport("Database Connection Cannot be Created");
        return 0;
    }

    DbConnection *connection = m_manager->connectionList[m_manager->connectionList.size() - 1];
    return m_pythonConnectionsStorage.findOrCreatePythonQtObject(connection);
}

DatabaseConnection *DatabaseManagerPythonModule::newConnection()
{
    int connectionCount = m_manager->connectionList.size();
    m_manager->connectToDb();
    if ( !(m_manager->connectionList.size() > connectionCount) ) {
        Core::Storage::mainWindow()->pythonReport("Database Connection Cannot be Created");
        return 0;
    }

    DbConnection *connection = m_manager->connectionList[connectionCount];
    return m_pythonConnectionsStorage.findOrCreatePythonQtObject(connection);
}

DatabaseConnection *DatabaseManagerPythonModule::newConnectionFromHistory()
{
    int connectionCount = m_manager->connectionList.size();
    m_manager->onConnectionsHistory();
    if ( !(m_manager->connectionList.size() > connectionCount) ) {
        Core::Storage::mainWindow()->pythonReport("Database Connection Cannot be Created");
        return 0;
    }

    DbConnection *connection = m_manager->connectionList[connectionCount];
    return m_pythonConnectionsStorage.findOrCreatePythonQtObject(connection);
}

bool DatabaseManagerPythonModule::closeConnection( int index )
{
    if ( index < 0 || index >= connectionsCount() ) {
        Core::Storage::mainWindow()->pythonReport(tr("Out of Range of Database Connection Index"));
        return false;
    }

    return m_manager->closeConnection(index);
}

bool DatabaseManagerPythonModule::closeAllConnections()
{
    m_manager->closeAllDbConnections();
    return (connectionsCount() == 0);
}

int DatabaseManagerPythonModule::currentConnectionIndex()
{
    return m_manager->databaseComboBox->currentIndex();
}

bool DatabaseManagerPythonModule::setCurrentConnectionIndex( int index )
{
    if ( index < 0 || index >= connectionsCount() ) {
        Core::Storage::mainWindow()->pythonReport(tr("Out of Range of Database Connection Index"));
        return false;
    }

    m_manager->databaseComboBox->setCurrentIndex(index);
    return true;
}

int DatabaseManagerPythonModule::connectionsCount()
{
    return m_manager->databaseComboBox->count();
}

DatabaseConnection *DatabaseManagerPythonModule::pythonConnection( int connectionIndex )
{
    DbConnection *connection = m_manager->connectionList[connectionIndex];
    return m_pythonConnectionsStorage.findOrCreatePythonQtObject(connection);
}

DatabaseConnection *DatabaseManagerPythonModule::currentConnection()
{
    int connectionIndex = m_manager->databaseComboBox->currentIndex();
    if ( connectionIndex == -1 ) {
        Core::Storage::mainWindow()->pythonReport(tr("No Database Connections"));
        return 0;
    }

    return pythonConnection(connectionIndex);
}

DatabaseConnection *DatabaseManagerPythonModule::connectionAt( int connectionIndex )
{
    if ( connectionIndex < 0 || connectionIndex >= connectionsCount() ) {
        Core::Storage::mainWindow()->pythonReport(tr("Out of Range of Database Connection Index"));
        return 0;
    }

    return pythonConnection(connectionIndex);
}

#ifndef HAVE_QT5
Q_EXPORT_PLUGIN2(DbManager, DbManager)
#endif

} // Database
