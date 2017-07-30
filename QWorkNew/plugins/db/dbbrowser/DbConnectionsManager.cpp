#include <QComboBox>
#include <QTabWidget>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QAction>
#include <QIcon>
#include <QDir>
#include <QPluginLoader>
#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlError>

#include "DbStructItem.h"
#include "QDbStructWidget.h"
#include "DbConnectionsManager.h"
#include "ConnectDialog.h"
#include "ChooseTableFieldsDialog.h"
#include "QWorkBase.h"
#include "QTableInfoDialog.h"
#include "QSourceViewDialog.h"

#include "QRdbQueryModel.h"
#include "QRdbTableView.h"
#include "SqlEditorSettingsWidget.h"
#include "QueryViewSettingsWidget.h"
#include "connmanager_constants.h"
#include "sql_constants.h"
#include "QSqlHistoryModel.h"
#include "QSqlHistoryTableView.h"

QDbConnectionsManager *QDbConnectionsManager::connectionsManager(0);

///////////////////////////////////////////////////////////////////////////////////////////////
/// DbConnection
DbConnection::DbConnection( IDatabaseLoaderPlugin *loader_, BaseConnectionInfo *connectionInfo_ )
    : loader(loader_)
    , transactionStarted(false)
    , connectionInfo(connectionInfo_)
{
    connectionName = connectionInfo->connectionName();
    connectionTitle = connectionInfo->connectionTitle();

    hasTransactionFeature = QSqlDatabase::database(connectionName).driver()->hasFeature(QSqlDriver::Transactions);
}

bool DbConnection::closeConnection( QMainWindow *mainWindow )
{
    if ( !confirmEndStartedTransaction(mainWindow) )
        return false;

    QSqlDatabase::removeDatabase(connectionName);
    return true;
}

bool DbConnection::confirmEndStartedTransaction( QMainWindow *mainWindow )
{
    if ( transactionStarted ) {
        QMessageBox msgBox(mainWindow);
        msgBox.setText(QObject::tr("Connection %1 Has Started Transaction.").arg(connectionTitle));
        QPushButton *commitButton   = msgBox.addButton(QObject::tr("Commit"),   QMessageBox::ActionRole);
        QPushButton *rollbackButton = msgBox.addButton(QObject::tr("Rollback"), QMessageBox::ActionRole);
        QPushButton *cancelButton   = msgBox.addButton(QObject::tr("Cancel"),   QMessageBox::RejectRole);
        msgBox.setDefaultButton(commitButton);
        msgBox.setEscapeButton(cancelButton);

        msgBox.exec();

        if ( msgBox.clickedButton() == commitButton ) {
            QSqlDatabase::database(connectionName).commit();
        }
        else if ( msgBox.clickedButton() == rollbackButton ) {
            QSqlDatabase::database(connectionName).rollback();
        }
        else {
            return false;
        }
    }

    return true;
}

bool DbConnection::startTransaction()
{
    if ( !hasTransactionFeature )
        return false;

    if ( transactionStarted )
        return false;

    transactionStarted = QSqlDatabase::database(connectionName).transaction();
    return true;
}

bool DbConnection::commitTransaction()
{
    if ( !hasTransactionFeature )
        return false;

    if ( !transactionStarted )
        return false;

    QSqlDatabase::database(connectionName).commit();
    transactionStarted = false;
    return true;
}

bool DbConnection::rollbackTransaction()
{
    if ( !hasTransactionFeature )
        return false;

    if ( !transactionStarted )
        return false;

    QSqlDatabase::database(connectionName).rollback();
    transactionStarted = false;
    return true;
}

void DbConnection::getTableFields( const QString &schemaName, const QString &tableName, QStringList *tableFields )
{
    QList <QList <QVariant> > valueList;
    if ( !schemaName.isEmpty() ) {
        QList <QVariant> item;
        item << schemaName;
        valueList << item;
    }
    if ( loader->loadFields(connectionInfo, valueList, tableName) ) {
        for (int i = 0; i < valueList.size(); i++)
            tableFields->append(valueList[i][0].toString());
    }
}

void DbConnection::getTableConstraints( const QString &schemaName, const QString &tableName, QStringList *tableConstraints )
{
    QList <QList <QVariant> > valueList;
    if ( !schemaName.isEmpty() ) {
        QList <QVariant> item;
        item << schemaName;
        valueList << item;
    }
    if ( loader->loadConstraints(connectionInfo, valueList, tableName) ) {
        for (int i = 0; i < valueList.size(); i++)
            tableConstraints->append(valueList[i][0].toString());
    }
}

/////////////////////////////////////////////////////////////////////////
// QSettingsCategorySqlEditor
QSettingsCategorySqlEditor::QSettingsCategorySqlEditor()
{
    /// Formats
    valuesMap["Number_Bold"]                = false;
    valuesMap["Number_Italic"]              = false;
    valuesMap["Number_ForeColor"]           = QColor(  0,   0, 224);
    valuesMap["Number_BackColor"]           = QColor(255, 255, 255);

    valuesMap["String_Bold"]                = false;
    valuesMap["String_Italic"]              = false;
    valuesMap["String_ForeColor"]           = QColor(  0,   0, 128);
    valuesMap["String_BackColor"]           = QColor(255, 255, 255);

    valuesMap["Keyword1_Bold"]              = true;
    valuesMap["Keyword1_Italic"]            = false;
    valuesMap["Keyword1_ForeColor"]         = QColor(  0,   0,   0);
    valuesMap["Keyword1_BackColor"]         = QColor(255, 255, 255);

    valuesMap["Keyword2_Bold"]              = true;
    valuesMap["Keyword2_Italic"]            = false;
    valuesMap["Keyword2_ForeColor"]         = QColor(  0,   0,   0);
    valuesMap["Keyword2_BackColor"]         = QColor(255, 255, 255);

    valuesMap["Keyword3_Bold"]              = true;
    valuesMap["Keyword3_Italic"]            = false;
    valuesMap["Keyword3_ForeColor"]         = QColor(  0,   0,   0);
    valuesMap["Keyword3_BackColor"]         = QColor(255, 255, 255);

    valuesMap["Keyword4_Bold"]              = true;
    valuesMap["Keyword4_Italic"]            = false;
    valuesMap["Keyword4_ForeColor"]         = QColor(  0,   0,   0);
    valuesMap["Keyword4_BackColor"]         = QColor(255, 255, 255);

    valuesMap["Keyword5_Bold"]              = true;
    valuesMap["Keyword5_Italic"]            = false;
    valuesMap["Keyword5_ForeColor"]         = QColor(  0,   0,   0);
    valuesMap["Keyword5_BackColor"]         = QColor(255, 255, 255);

    valuesMap["Keyword6_Bold"]              = true;
    valuesMap["Keyword6_Italic"]            = false;
    valuesMap["Keyword6_ForeColor"]         = QColor(  0,   0,   0);
    valuesMap["Keyword6_BackColor"]         = QColor(255, 255, 255);

    valuesMap["Keyword7_Bold"]              = true;
    valuesMap["Keyword7_Italic"]            = false;
    valuesMap["Keyword7_ForeColor"]         = QColor(  0,   0,   0);
    valuesMap["Keyword7_BackColor"]         = QColor(255, 255, 255);

    valuesMap["Keyword8_Bold"]              = true;
    valuesMap["Keyword8_Italic"]            = false;
    valuesMap["Keyword8_ForeColor"]         = QColor(  0,   0,   0);
    valuesMap["Keyword8_BackColor"]         = QColor(255, 255, 255);

    valuesMap["Operator_Bold"]              = false;
    valuesMap["Operator_Italic"]            = false;
    valuesMap["Operator_ForeColor"]         = QColor(  0,  64,  64);
    valuesMap["Operator_BackColor"]         = QColor(255, 255, 255);

    valuesMap["Comment_Bold"]               = false;
    valuesMap["Comment_Italic"]             = false;
    valuesMap["Comment_ForeColor"]          = QColor(224,   0,   0);
    valuesMap["Comment_BackColor"]          = QColor(255, 255, 255);

    valuesMap["Whitespace_Bold"]            = false;
    valuesMap["Whitespace_Italic"]          = false;
    valuesMap["Whitespace_ForeColor"]       = QColor(224,   0,   0);
    valuesMap["Whitespace_BackColor"]       = QColor(255, 255, 255);

    valuesMap["Table_Bold"]                 = true;
    valuesMap["Table_Italic"]               = false;
    valuesMap["Table_ForeColor"]            = QColor(  0, 255,   0);
    valuesMap["Table_BackColor"]            = QColor(255, 255, 255);

    valuesMap["Alias_Bold"]                 = false;
    valuesMap["Alias_Italic"]               = false;
    valuesMap["Alias_ForeColor"]            = QColor(127, 127, 127);
    valuesMap["Alias_BackColor"]            = QColor(255, 255, 255);

    valuesMap["Field_Bold"]                 = false;
    valuesMap["Field_Italic"]               = false;
    valuesMap["Field_ForeColor"]            = QColor(255,  64,   0);
    valuesMap["Field_BackColor"]            = QColor(255, 255, 255);

    valuesMap["Index_Bold"]                 = true;
    valuesMap["Index_Italic"]               = false;
    valuesMap["Index_ForeColor"]            = QColor(  0, 255,   0);
    valuesMap["Index_BackColor"]            = QColor(255, 255, 255);

    valuesMap["Trigger_Bold"]               = true;
    valuesMap["Trigger_Italic"]             = false;
    valuesMap["Trigger_ForeColor"]          = QColor(  0, 255,   0);
    valuesMap["Trigger_BackColor"]          = QColor(255, 255, 255);

    valuesMap["Constraint_Bold"]            = true;
    valuesMap["Constraint_Italic"]          = false;
    valuesMap["Constraint_ForeColor"]       = QColor(  0, 255,   0);
    valuesMap["Constraint_BackColor"]       = QColor(255, 255, 255);

    valuesMap["Bind_Bold"]                  = false;
    valuesMap["Bind_Italic"]                = false;
    valuesMap["Bind_ForeColor"]             = QColor(  0,  64, 255);
    valuesMap["Bind_BackColor"]             = QColor(255, 255, 255);

    valuesMap["Schema_Bold"]                 = true;
    valuesMap["Schema_Italic"]               = true;
    valuesMap["Schema_ForeColor"]            = QColor(  0, 192,  32);
    valuesMap["Schema_BackColor"]            = QColor(255, 255, 255);
}

QWidget *QSettingsCategorySqlEditor::createSettingsWidget( QWidget *parent )
{
    return new SqlEditorSettingsWidget(valuesMap, parent);
}

void QSettingsCategorySqlEditor::applySettings( QWidget *settingsWidget )
{
    if ( settingsWidget ) {
        SqlEditorSettingsWidget *widget = qobject_cast<SqlEditorSettingsWidget *>(settingsWidget);
        if ( !widget )
            return;
        widget->saveSettings(valuesMap);
    }

    /// Sql colors settings
    QDocumentManager::getInstance()->onExtraSettingsChanged();
}

/////////////////////////////////////////////////////////////////////////
// QSettingsCategoryViewQuery
QSettingsCategoryViewQuery::QSettingsCategoryViewQuery()
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

QWidget *QSettingsCategoryViewQuery::createSettingsWidget( QWidget *parent )
{
    return new QueryViewSettingsWidget(valuesMap, parent);
}

void QSettingsCategoryViewQuery::applySettings( QWidget *settingsWidget )
{
    if ( settingsWidget ) {
        QueryViewSettingsWidget *widget = qobject_cast <QueryViewSettingsWidget *>(settingsWidget);
        if ( !widget )
            return;

        widget->saveSettings(valuesMap);
    }

    QDbConnectionsManager::getInstance()->reflectOnQueryViewSettingsChanged();
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// QSettingsCategoryTableInfoDialog
QSettingsCategoryTableInfoDialog::QSettingsCategoryTableInfoDialog()
{
    valuesMap["x"] = 120;
    valuesMap["y"] = 100;
    valuesMap["width"] = 480;
    valuesMap["height"] = 320;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// QSettingsCategorySourceViewDialog
QSettingsCategorySourceViewDialog::QSettingsCategorySourceViewDialog()
{
    valuesMap["x"] = 120;
    valuesMap["y"] = 100;
    valuesMap["width"] = 480;
    valuesMap["height"] = 320;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/// QDbConnectionsManager
QDbConnectionsManager::QDbConnectionsManager()
    : QISessionClient()
    , QIAppCloseReflectorClient()
    , dbIndex(0)
{
    connectionsManager = this;

    mainWindow = QWorkMainWindow::getInstance();
    databaseComboBox = new QComboBox(mainWindow);
    databaseComboBox->setMinimumSize(145, 22);
    dbTabWidget = new QTabWidget(mainWindow);

    createActions();
    createContextMenuActions();

    connect(databaseComboBox,   SIGNAL(currentIndexChanged(int)),   dbTabWidget,        SLOT(setCurrentIndex(int)));
    connect(dbTabWidget,        SIGNAL(currentChanged(int)),        databaseComboBox,   SLOT(setCurrentIndex(int)));
    connect(dbTabWidget,        SIGNAL(currentChanged(int)),        this,               SLOT(onCurrentConnectionChanged(int)));

    connect(QDocumentManager::getInstance(), SIGNAL(currentDocumentChanged(QIDocument*)), this, SLOT(onCurrentDocumentChanged(QIDocument*)));

    loadLoaderPlugins();

    m_connectionsHistory = new DbConnectionsHistory;
}

QDbConnectionsManager::~QDbConnectionsManager()
{
    connectionList.clear();
    connectionInfoList.clear();
    structWidgetList.clear();
    loaderTypeList.clear();
}

QStringList QDbConnectionsManager::settingsKeyList() const
{
    return QStringList() << SqlBrowser::C_SQL_QUERY_SETTINGS_ID
                         << SqlBrowser::C_SQL_TABLEINFO_SETTINGS_ID
                         << SqlBrowser::C_SQL_SOURCEVIEW_SETTINGS_ID
                         << Sql::Constants::C_SQL_EDITOR_SETTINGS_ID;
}

QISettings *QDbConnectionsManager::createSettings( const QString &settingsKey ) const
{
    if ( settingsKey == SqlBrowser::C_SQL_QUERY_SETTINGS_ID )
        return new QSettingsCategoryViewQuery();

    if ( settingsKey == SqlBrowser::C_SQL_TABLEINFO_SETTINGS_ID )
        return new QSettingsCategoryTableInfoDialog();

    if ( settingsKey == SqlBrowser::C_SQL_SOURCEVIEW_SETTINGS_ID )
        return new QSettingsCategorySourceViewDialog();

    if ( settingsKey == Sql::Constants::C_SQL_EDITOR_SETTINGS_ID )
        return new QSettingsCategorySqlEditor();

    return NULL;
}

QDbConnectionsManager* QDbConnectionsManager::getInstance()
{
    return connectionsManager;
}

static bool prepareStatement( IDatabaseLoaderPlugin *loader, QSqlStatement &statement, QString &text, const char splitSymbol )
{
    int pos = statement.startPosition;
    int endpos = statement.endPosition;
    int line = statement.startLine;
    int mode = 0;   // 0 - normal
                    // 1 - строковый комментарий
                    // 2 - блоковый комментарий

    bool typeDefined = false;

    for (int i = pos; i < endpos; i++ ) {
        if ( text[pos] == '\n' )
            line++;
        switch ( mode ) {
        case 0:
            if ( text[pos] == ' ' || text[pos] == '\t' || text[pos] == '\n' ) {
                pos++;
                break;
            }
            if ( text[pos] == '-' && text[pos+1] == '-' ) {
                mode = 1;
                pos += 2;
                break;
            }
            if ( text[pos] == '/' && text[pos+1] == '*' ) {
                mode = 2;
                pos += 2;
                break;
            }
            if ( !typeDefined ) {
                if ( pos == endpos ) return false;
                statement.isBeginTransaction = true;
                statement.startPosition = pos;
                statement.startLine = line;
                if ( text.mid(pos).startsWith("commit", Qt::CaseInsensitive) ) {
                    statement.isCommitTransaction = true;
                }
                else if ( text.mid(pos).startsWith("rollback", Qt::CaseInsensitive) ) {
                    statement.isRollbackTransaction = true;
                }
                else {
                    statement.isBeginTransaction = loader->isTransactionStatement(text.mid(pos));
                }
                typeDefined = true;
            }
            if  ( text[pos] == ':' ) {
                int pos0 = pos;
                pos++;
                while ( pos < endpos && (text[pos] == '_' ||
                        (text[pos] >= '0' && text[pos] <= '9') ||
                        (text[pos] >= 'a' && text[pos] <= 'z') ||
                        (text[pos] >= 'A' && text[pos] <= 'Z') ) )
                    pos++;
                if ( pos != endpos || text[pos] == splitSymbol )
                    pos--;
                if ( pos > pos0 + 1 ) {
                    QString bindName = text.mid(pos0, pos - pos0 + 1).toUpper();
                    statement.bindNames.append(bindName);
                    qDebug() << "    >>> BINDING" << bindName;
                }
            }
        case 1:
            if ( text[pos] == '\n' )
                mode = 0;
            pos++;
            break;
        case 2:
            if (text[pos] == '*' && text[pos+1] == '/' ) {
                mode = 0;
                pos++;
            }
            pos++;
            break;
        }
    }

    return typeDefined;
}

void QDbConnectionsManager::parseStatements(QString &text, QList<QSqlStatement> &statements , char splitSymbol )
{
    IDatabaseLoaderPlugin *loader = QDbConnectionsManager::getInstance()->getCurrentLoader();
    if ( !loader )
        return;

    int currPos = 0;

    int mode = 0;       // 0 - нормальный
                        // 1 - строка
                        // 2 - символ
                        // 3 - строчный комментарий
                        // 4 - блоковый комментарий

    int length = text.length();

    // 1. Удалили 'splitSymbol' из комментариев и строк
    while ( currPos < length ) {
        switch ( mode ) {
        case 0:
            if ( text[currPos] == '\"' ) {
                mode = 1;
                currPos++;
                break;
            }
            if ( text[currPos] == '\'' ) {
                mode = 2;
                currPos++;
                break;
            }
            if ( text[currPos] == '-' ) {
                currPos++;
                if ( currPos < length ) {
                    if ( text[currPos] == '-' ) {
                        mode = 3;
                        currPos++;
                        break;
                    }
                }
                break;
            }
            if ( text[currPos] == '/' ) {
                currPos++;
                if ( currPos < length ) {
                    if ( text[currPos] == '*' ) {
                        mode = 4;
                        currPos++;
                        break;
                    }
                }
                break;
            }
            currPos++;
            break;
        case 1:
            if ( text[currPos] == '\"' ) {
                mode = 0;
                currPos++;
                break;
            }
            if ( text[currPos] == splitSymbol )
                text[currPos] = ' ';
            currPos++;
            break;
        case 2:
            if ( text[currPos] == '\'' ) {
                mode = 0;
                currPos++;
                break;
            }
            if ( text[currPos] == splitSymbol )
                text[currPos] = ' ';
            currPos++;
            break;
        case 3:
            if ( text[currPos] == '\n' ) {
                mode = 0;
                currPos++;
                break;
            }
            if ( text[currPos] == splitSymbol )
                text[currPos] = ' ';
            currPos++;
            break;
        case 4:
            if ( text[currPos] == '*' ) {
                currPos++;
                if ( currPos < length ) {
                    if ( text[currPos] == '/' ) {
                        mode = 0;
                        currPos++;
                        break;
                    }
                }
                break;
            }
            if ( text[currPos] == splitSymbol )
                text[currPos] = ' ';
            currPos++;
            break;
        }
    }

    // 2. Выполняем разбивку по 'splitSymbol'
    int currLine = 0;
    int pos = 0;
    int line = 0;

    currPos = 0;
    QString rr = "\n|";
    rr.append(splitSymbol);
    while ( (currPos = text.indexOf(QRegExp(rr), currPos)) != -1 ) {
        if ( text[currPos] == '\n' ) {
            currLine++;
            currPos++;
        }
        else {
            QSqlStatement statement(line, pos, currLine, currPos);
            currPos++;
            pos = currPos;
            line = currLine;
            if ( prepareStatement(loader, statement, text, splitSymbol) )
                statements.push_back(statement);
        }
    }
    if ( pos < length - 1 ) {
        QSqlStatement statement(line, pos, currLine, length - 1);
        if ( prepareStatement(loader, statement, text, splitSymbol) )
            statements.push_back(statement);
    }
}

void QDbConnectionsManager::createActions()
{
    /// Actions For Database Menu
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

    m_cmdCloseDb->action()->setEnabled(false);
    m_cmdCloseAllDb->action()->setEnabled(false);
    m_cmdCommit->action()->setEnabled(false);
    m_cmdRollback->action()->setEnabled(false);
    m_cmdExecuteCurrent->action()->setEnabled(false);
    m_cmdExecuteAsScript->action()->setEnabled(false);

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
}

void QDbConnectionsManager::populateMenu( int pluginUiIndex, QMenu *menu )
{
    if ( pluginUiIndex == 0 ) {
        menu->addAction(m_cmdConnectDb->action());
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
    }
}

void QDbConnectionsManager::populateToolBar( int pluginUiIndex, QToolBar *toolBar )
{
    if ( pluginUiIndex == 0 ) {
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
    }
}

void QDbConnectionsManager::initialize()
{
    Core::QCommandManager::getInstance()->registerCommands( tr("DatabaseCommands"),
                                                            QList<Core::Command *>() << m_cmdConnectDb
                                                                                     << m_cmdCloseDb
                                                                                     << m_cmdCloseAllDb
                                                                                     << m_cmdCommit
                                                                                     << m_cmdRollback
                                                                                     << m_cmdExecuteCurrent
                                                                                     << m_cmdExecuteAsScript
                                                                                     << m_cmdConnHistory );
}

void QDbConnectionsManager::destroy()
{
    delete m_connectionsHistory;
}

QList<UiProviderInterface *> QDbConnectionsManager::createUiProviders(QWidget *parent)
{
    QwDockWidget *dbDock = new QwDockWidget(tr("Database Browser"), parent);
    dbDock->toggleViewAction()->setIcon(QIcon(":/images/db_browser.png"));
    dbDock->setWidget(dbTabWidget);

    QMenu *contextMenu = new QMenu;
    contextMenu->addAction(m_cmdConnectDb->action());
    contextMenu->addSeparator();
    contextMenu->addAction(m_cmdConnHistory->action());
    contextMenu->addSeparator();
    contextMenu->addAction(m_cmdCloseDb->action());
    contextMenu->addAction(m_cmdCloseAllDb->action());
    dbDock->setContextMenu(contextMenu);

    QwDockWidget *sqlDock = new QwDockWidget(tr("SQL History"), parent);
    sqlDock->toggleViewAction()->setIcon(QIcon(":/images/sql_history.png"));
    QString fileName = QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    fileName += "/";
    fileName += QWorkMainWindow::getInstance()->getAppName();
    fileName += "/sqlhistory.xml";
    SqlHistoryInfoList *sqlList = new SqlHistoryInfoList(fileName, sqlDock);
    QSqlHistoryTableView *sqlTable = new QSqlHistoryTableView(sqlDock);
    QSqlHistoryModel *sqlModel = new QSqlHistoryModel(sqlList, sqlDock);
    QSqlHistorySortFilterProxyModel *sortModel = new QSqlHistorySortFilterProxyModel(this);
    sortModel->setSourceModel(sqlModel);
    sortModel->setDynamicSortFilter(true);

    connect(this, SIGNAL(sqlHistoryEnabledChanged()), sqlTable, SLOT(onQueryViewSettingsChanged()));

    sqlTable->setModel(sortModel);
    sqlTable->setSortingEnabled(true);
    sqlTable->setSelectionMode(QAbstractItemView::SingleSelection);
    sqlTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    sqlTable->sortByColumn(1, Qt::DescendingOrder);
    sqlTable->resizeColumnsToContents();
    sqlTable->resizeRowsToContents();
    sqlDock->setWidget(sqlTable);
    sqlDock->setContextMenu(sqlTable->contextMenu());

    connect(this, SIGNAL(sqlExecuted(QString)), sqlModel, SLOT(addSql(QString)));

    QList<UiProviderInterface *> providers;
    providers << new DbConnectionsUiProvider(this, dbDock)
              << new DbSqlHistoryUiProvider(this, sqlDock);

    return providers;
}

bool QDbConnectionsManager::confirmEndStartedTransactions()
{
    for (int i = 0; i < connectionList.size(); i++)
        if ( !connectionList[i].confirmEndStartedTransaction(mainWindow))
            return false;

    return true;
}

bool QDbConnectionsManager::sesssioAboutToClose()
{
    return confirmEndStartedTransactions();
}

void QDbConnectionsManager::sessionClose()
{
    closeAllDbConnections();
}

void QDbConnectionsManager::saveSession( const QString &location )
{
    QString fileName = location;
    fileName += "/dbconnections.xml";

    QFile file(fileName);
    if ( !file.open(QFile::WriteOnly | QFile::Text) ) {
        QWorkMainWindow::warning( tr("Error On Save Session Db-Connections\nFile \"%1\" Cannot be Opened for Writing:\n%2.")
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
        itemElement.setAttribute("dbType", connectionList[i].getLoader()->databaseType());
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

void QDbConnectionsManager::loadSession( const QString &location )
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

    // Загружаем всю структуру
    if (!domDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn)) {
        QWorkMainWindow::warning( tr("Error On Load Session Db-Connections\nCommon Error in xml-file:\nLine %1, Column %2: %3")
                                  .arg(errorLine).arg(errorColumn).arg(errorStr) );
        return;
    }

    // Первичная проверка (тип и версия документа)
    QDomElement root = domDocument.documentElement();
    if ( root.tagName() != "dbconnections" ) {
        QWorkMainWindow::warning( tr("Error On Load Session Db-Connections\nInvalid Format of Sessions xml-file") );
        return;
    }
    if ( !root.hasAttribute("version") ) {
        QWorkMainWindow::warning( tr("Error On Load Session Db-Connections\nInvalid Format of Sessions xml-file (version expected)") );
        return;
    }
    if ( root.attribute("version") != "1.0" ) {
        QWorkMainWindow::warning( tr("Error On Load Session Db-Connections\nInvalid Format of Sessions xml-file (wrong version)") );
        return;
    }

    QDomElement itemElement = root.firstChildElement();
    while ( !itemElement.isNull() ) {
        DbConnectionInfo connInfo;
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

bool QDbConnectionsManager::applicationAboutToClose()
{
    return confirmEndStartedTransactions();
}

void QDbConnectionsManager::applicationClose()
{
    closeAllDbConnections();
}

QString QDbConnectionsManager::getConnectionName() const
{
    int currDbIndex = databaseComboBox->currentIndex();
    if ( currDbIndex == -1 )
        return "";

    return connectionList[currDbIndex].getConnectionName();
}

BaseConnectionInfo *QDbConnectionsManager::getConnectionInfo() const
{
    int currDbIndex = databaseComboBox->currentIndex();
    if ( currDbIndex == -1 )
        return 0;

    return connectionList[currDbIndex].getConnectionInfo();
}

void QDbConnectionsManager::loadLoaderPlugins()
{
    loaderTypeList.clear();

    QDir pluginsDir;
    pluginsDir = QDir(qApp->applicationDirPath());
    pluginsDir.cd("dbloaders");

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        // QFileInfo fileInfo(fileName);
        // if ( fileInfo.suffix().toUpper() == "DLL" ) {
            QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
            QObject *plugin = loader.instance();
            if ( plugin ) {
                IDatabaseLoaderPlugin *iLoader = qobject_cast<IDatabaseLoaderPlugin *>(plugin);
                if ( iLoader ) {
                    loaderTypeList.append(iLoader);
                }
            }
        // }
    }
}

void QDbConnectionsManager::emitInsertText( const QString &text, const QString &extension )
{
    QDocumentManager::getInstance()->insertText(text, extension);
}

DbStructItem *QDbConnectionsManager::getCurrentItem( int childIndex ) const
{
    int currDbIndex = databaseComboBox->currentIndex();
    if ( currDbIndex == -1 )
        return NULL;

    return structWidgetList[currDbIndex]->getCurrentItem(childIndex);
}

bool QDbConnectionsManager::getCurrentSchemaName( QString &schemaName ) const
{
    schemaName = "";
    IDatabaseLoaderPlugin *loader = getCurrentLoader();
    if ( !loader->multiSchemaSupported() )
        return true;

    DbStructItem *currentItem = getCurrentItem();
    DbStructItem *parentItem = currentItem;
    while ( parentItem ) {
        if ( DbStructSchemaItem *schemaItem = dynamic_cast<DbStructSchemaItem *>(parentItem) ) {
            schemaName = schemaItem->data(0).toString().trimmed();
            return true;
        }
        parentItem = parentItem->parent();
    }
    return false;
}

bool QDbConnectionsManager::getCurrentSchemaName( IDatabaseLoaderPlugin *loader, DbStructItem *item , QString &schemaName )
{
    schemaName = "";
    if ( !loader->multiSchemaSupported() )
        return true;

    DbStructItem *parentItem = item;
    while ( parentItem ) {
        if ( DbStructSchemaItem *schemaItem = dynamic_cast<DbStructSchemaItem *>(parentItem) ) {
            schemaName = schemaItem->data(0).toString();
            return true;
        }
        parentItem = parentItem->parent();
    }
    return false;
}

IDatabaseLoaderPlugin *QDbConnectionsManager::getCurrentLoader() const
{
    int currDbIndex = databaseComboBox->currentIndex();
    if ( currDbIndex == -1 )
        return NULL;

    return structWidgetList[currDbIndex]->getCurrentLoader();
}

void QDbConnectionsManager::createContextMenuActions()
{
    actCreateQuerySelectStar    = new QAction(QIcon(":/images/cre_select_star.png"),    tr("Create Select * Query"), mainWindow);
    actCreateQuerySelectAll     = new QAction(QIcon(":/images/cre_select_all.png"),     tr("Create Select All Query"), mainWindow);
    actCreateQuerySelectChoosen = new QAction(QIcon(":/images/cre_select_choosen.png"), tr("Create Select ... Query"), mainWindow);

    actViewTriggerSource        = new QAction(tr("View Trigger Source"), mainWindow);
    actViewFuncProcSource       = new QAction(tr("View Function/Procedure Source"), mainWindow);
    actViewModuleSource         = new QAction(tr("View Module Source"), mainWindow);

    connect(actCreateQuerySelectStar,       SIGNAL(triggered()), this, SLOT(onCreateQuerySelectStar()));
    connect(actCreateQuerySelectAll,        SIGNAL(triggered()), this, SLOT(onCreateQuerySelectAll()));
    connect(actCreateQuerySelectChoosen,    SIGNAL(triggered()), this, SLOT(onCreateQuerySelectChoosen()));

    connect(actViewTriggerSource,           SIGNAL(triggered()), this, SLOT(onViewTriggerSource()));
    connect(actViewFuncProcSource,          SIGNAL(triggered()), this, SLOT(onViewFuncProcSource()));
    connect(actViewModuleSource,            SIGNAL(triggered()), this, SLOT(onViewModuleSource()));

    actTableDetails             = new QAction(tr("Details..."), mainWindow);
    actViewDetails              = new QAction(tr("Details..."), mainWindow);

    connect(actTableDetails,            SIGNAL(triggered()), this, SLOT(onTableDetails()));
    connect(actViewDetails,             SIGNAL(triggered()), this, SLOT(onViewDetails()));
}

void QDbConnectionsManager::connectToDb()
{
    ConnectDialog connDialog(loaderTypeList, mainWindow);
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

    QDbStructWidget *structWidget = new QDbStructWidget(connectionInfo, loaderTypeList[connType], mainWindow);

    connectionList.append(DbConnection(loaderTypeList[connType], connectionInfo));
    databaseComboBox->addItem(dbTitle);
    dbTabWidget->addTab(structWidget, dbTitle);
    dbTabWidget->setTabToolTip(dbTabWidget->count() - 1, "<pre>" + connectionInfo->connectionDescription() + "</pre>");
    structWidgetList.append(structWidget);

    databaseComboBox->setCurrentIndex(databaseComboBox->count() - 1);

    emit(dbConnectionAdded(QVariantList() << dbTitle));

    m_cmdCloseAllDb->action()->setEnabled(true);
    m_cmdCloseDb->action()->setEnabled(true);

    DbConnectionInfo connInfo;
    connInfo.databaseType = loaderTypeList[connType]->databaseType();
    connInfo.description = connectionInfo->connectionDescription();
    connInfo.parameters << parameters;
    m_connectionsHistory->addConnection(connInfo);

    connectionInfoList.append(connInfo);
}

void QDbConnectionsManager::closeDbConnection()
{
    int index = databaseComboBox->currentIndex();
    if ( index == -1 )
        return;

    if ( !connectionList[index].closeConnection(mainWindow) )
        return;

    databaseComboBox->removeItem(index);
    dbTabWidget->removeTab(index);

    connectionList.removeAt(index);
    connectionInfoList.removeAt(index);
    structWidgetList.removeAt(index);

    emit(dbConnectionClosed(QVariantList() << index));

    if ( !databaseComboBox->count() ) {
        m_cmdCloseDb->action()->setEnabled(false);
        m_cmdCloseAllDb->action()->setEnabled(false);
    }
}

void QDbConnectionsManager::closeAllDbConnections()
{
    int index = databaseComboBox->count() - 1;

    while ( index >= 0 ) {
        if ( !connectionList[index].closeConnection(mainWindow) )
            return;
        databaseComboBox->removeItem(index);
        dbTabWidget->removeTab(index);

        connectionList.removeAt(index);
        connectionInfoList.removeAt(index);
        structWidgetList.removeAt(index);

        emit(dbConnectionClosed(QVariantList() << index));

        index--;
    }

    dbIndex = 0;

    m_cmdCloseDb->action()->setEnabled(false);
    m_cmdCloseAllDb->action()->setEnabled(false);
}

void QDbConnectionsManager::onCreateQuerySelectStar()
{
    QString text = getSelectStarQueryString();
    if ( text == "" )
        return;

    emitInsertText(text, "SQL");
}

void QDbConnectionsManager::onCreateQuerySelectAll()
{
    QString text = getSelectAllQueryString();
    if ( text == "" )
        return;

    emitInsertText(text, "SQL");
}

void QDbConnectionsManager::onCreateQuerySelectChoosen()
{
    QString text = getSelectChoosenQueryString();
    if ( text == "" )
        return;

    emitInsertText(text, "SQL");
}

void QDbConnectionsManager::onViewTriggerSource()
{
    DbStructItem *triggerItem = getCurrentItem();
    if ( !triggerItem )
        return;

    IDatabaseLoaderPlugin *loader = getCurrentLoader();
    if ( !loader )
        return;

    QString schemaName;
    if ( !getCurrentSchemaName(schemaName) )
        return;

    QString triggerName = triggerItem->data(0).toString();
    QString triggerSource;
    loader->getTriggerSource(getConnectionInfo(), schemaName, triggerName, triggerSource);

    showSourceViewDialog(tr("Trigger Source"), "SQL", triggerSource);
}

void QDbConnectionsManager::onViewFuncProcSource()
{
    DbStructItem *funcProcItem = getCurrentItem();
    if ( !funcProcItem )
        return;

    IDatabaseLoaderPlugin *loader = getCurrentLoader();
    if ( !loader )
        return;

    QString funcProcName = funcProcItem->data(0).toString();
    QString schemaName;
    if ( !getCurrentSchemaName(loader, funcProcItem, schemaName) )
        return;
    QString funcProcSource;
    loader->getFuncProcSource(getConnectionInfo(), schemaName, funcProcName, funcProcSource);

    showSourceViewDialog(tr("Function/Procedure Source"), "SQL", funcProcSource);
}

void QDbConnectionsManager::onViewModuleSource()
{
    DbStructItem *moduleItem = getCurrentItem();
    if ( !moduleItem )
        return;

    IDatabaseLoaderPlugin *loader = getCurrentLoader();
    if ( !loader )
        return;

    QString moduleName = moduleItem->data(0).toString();
    QString schemaName;
    if ( !getCurrentSchemaName(loader, moduleItem, schemaName) )
        return;
    QString moduleSource;
    loader->getModuleSource(getConnectionInfo(), schemaName, moduleName, moduleSource);

    showSourceViewDialog(tr("Module (Package) Source"), "SQL", moduleSource);
}

void QDbConnectionsManager::onTableDetails()
{
    DbStructItem *currentItem = getCurrentItem();
    if ( !currentItem )
        return;

    IDatabaseLoaderPlugin *loader = getCurrentLoader();
    if ( !loader )
        return;

    QString schemaName;
    if ( !getCurrentSchemaName(loader, currentItem, schemaName) )
        return;
    showTableInfoDialog( getConnectionInfo(), loader, schemaName, false, /* View Mode */
                         currentItem->getTableName(loader, schemaName),
                         currentItem->getObjectType(),
                         currentItem->getObjectName());
}

void QDbConnectionsManager::onViewDetails()
{
    DbStructItem *currentItem = getCurrentItem();
    if ( !currentItem )
        return;

    IDatabaseLoaderPlugin *loader = getCurrentLoader();
    if ( !loader )
        return;

    QString schemaName;
    if ( !getCurrentSchemaName(loader, currentItem, schemaName) )
        return;
    showTableInfoDialog( getConnectionInfo(), loader, schemaName, true, /* View Mode */
                         currentItem->getTableName(loader, schemaName),
                         currentItem->getObjectType(),
                         currentItem->getObjectName());
}

QString QDbConnectionsManager::getSelectStarQueryString() const
{
    DbStructItem *tableItem = getCurrentItem();
    if ( !tableItem )
        return "";

    IDatabaseLoaderPlugin *loader = getCurrentLoader();
    if ( !loader )
        return "";

    QString schemaName;
    if ( !getCurrentSchemaName(schemaName) )
        return "";

    QString queryString = "SELECT * FROM ";
    if ( !schemaName.isEmpty() )
        queryString += schemaName + ".";
    queryString += tableItem->data(0).toString();
    queryString += ";";

    return queryString;
}

QString QDbConnectionsManager::getSelectAllQueryString() const
{
    DbStructItem *columnsItem = getCurrentItem(0);
    if ( !columnsItem )
        return "";

    QString schemaName;
    if ( !getCurrentSchemaName(schemaName) )
        return "";

    QString queryString = "";

    int i = 0;
    for (;i < columnsItem->childCount(); i++) {
        QString fieldName = columnsItem->child(i)->data(0).toString();
        if ( !i )
            queryString += "SELECT t." + fieldName + "\n";
        else
            queryString += "     , t." + fieldName + "\n";
    }
    if ( !i )
        return "";

    queryString += "  FROM ";
    if ( !schemaName.isEmpty() )
        queryString += schemaName + ".";
    queryString += columnsItem->data(1).toString() + " t;";

    return queryString;
}

QString QDbConnectionsManager::getSelectChoosenQueryString() const
{
    DbStructItem *tableItem = getCurrentItem(0);
    if ( !tableItem )
        return "";

    QString schemaName;
    if ( !getCurrentSchemaName(schemaName) )
        return "";

    ChooseTableFieldsDialog fieldsDialog(schemaName, tableItem, true, mainWindow);
    if ( fieldsDialog.exec() != QDialog::Accepted )
        return "";

    return fieldsDialog.getSelectQuery();
}

bool QDbConnectionsManager::getChoosenTableFieldItems( QList <DbStructItem *> &fieldItems ) const
{
    DbStructItem *tableItem = getCurrentItem(0);
    if ( !tableItem )
        return false;

    ChooseTableFieldsDialog fieldsDialog("", tableItem, false, mainWindow);
    if ( fieldsDialog.exec() != QDialog::Accepted )
        return false;

    return fieldsDialog.getChoosenTableFieldItems(fieldItems);
}

void QDbConnectionsManager::startTransaction( int currDbIndex )
{
    if ( currDbIndex == -1 ) {
        currDbIndex = databaseComboBox->currentIndex();
        if ( currDbIndex == -1 )
            return;
    }

    if ( !connectionList[currDbIndex].startTransaction() )
        return;

    emit(transactionModeChanged(connectionList[currDbIndex].transaction()));
}

QStringList *QDbConnectionsManager::getCurrentSchemas()
{
    int currDbIndex = databaseComboBox->currentIndex();
    if ( currDbIndex == -1 )
        return 0;

    return connectionList[currDbIndex].getSchemas();
}

QStringList *QDbConnectionsManager::getCurrentTables( const QString &schemaName )
{
    int currDbIndex = databaseComboBox->currentIndex();
    if ( currDbIndex == -1 )
        return 0;

    return connectionList[currDbIndex].getTables(schemaName);
}

QStringList *QDbConnectionsManager::getCurrentViews( const QString &schemaName )
{
    int currDbIndex = databaseComboBox->currentIndex();
    if ( currDbIndex == -1 )
        return 0;

    return connectionList[currDbIndex].getViews(schemaName);
}

QStringList *QDbConnectionsManager::getCurrentIndices( const QString &schemaName )
{
    int currDbIndex = databaseComboBox->currentIndex();
    if ( currDbIndex == -1 )
        return 0;

    return connectionList[currDbIndex].getIndices(schemaName);
}

QStringList *QDbConnectionsManager::getCurrentTriggers( const QString &schemaName )
{
    int currDbIndex = databaseComboBox->currentIndex();
    if ( currDbIndex == -1 )
        return 0;

    return connectionList[currDbIndex].getTriggers(schemaName);
}

void QDbConnectionsManager::getCurrentFields( const QString &schemaName, const QString &tableName, QStringList *tableFields )
{
    int currDbIndex = databaseComboBox->currentIndex();
    if ( currDbIndex == -1 )
        return;

    connectionList[currDbIndex].getTableFields(schemaName, tableName, tableFields);
}

void QDbConnectionsManager::getCurrentConstraints( const QString &schemaName, const QString &tableName, QStringList *tableConstraints )
{
    int currDbIndex = databaseComboBox->currentIndex();
    if ( currDbIndex == -1 )
        return;

    connectionList[currDbIndex].getTableConstraints(schemaName, tableName, tableConstraints);
}

void QDbConnectionsManager::reflectOnQueryViewSettingsChanged()
{
    emit sqlHistoryEnabledChanged();
}

void QDbConnectionsManager::commitTransaction( int currDbIndex )
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

    if ( !connectionList[currDbIndex].commitTransaction() )
        return;

    if ( needSignal )
        emit(transactionModeChanged(connectionList[currDbIndex].transaction()));
}

void QDbConnectionsManager::rollbackTransaction( int currDbIndex )
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

    if ( !connectionList[currDbIndex].rollbackTransaction() )
        return;

    if ( needSignal )
        emit(transactionModeChanged(connectionList[currDbIndex].transaction()));
}

void QDbConnectionsManager::refreshMetaData()
{
    int currDbIndex = databaseComboBox->currentIndex();
    if ( currDbIndex == -1 )
        return;

    connectionList[currDbIndex].refreshMetaData();

}

void QDbConnectionsManager::onCurrentConnectionChanged( int index )
{
    bool execEnabled = true;

    if ( index == -1 ) {
        emit(transactionModeChanged(false));
        execEnabled = false;
    }
    else {
        emit(transactionModeChanged(connectionList[index].transaction()));
        QIDocument *document = QDocumentManager::getInstance()->currentDocument();
        execEnabled = isSqlDocument(document);
    }

    m_cmdExecuteCurrent->action()->setEnabled(execEnabled);
    m_cmdExecuteAsScript->action()->setEnabled(execEnabled);
}

bool QDbConnectionsManager::isSqlDocument( QIDocument *document )
{
    return ( document && document->documentPlugin()->defaultExtension() == "SQL" );
}

void QDbConnectionsManager::onCurrentDocumentChanged( QIDocument *document )
{
    bool execEnabled = ( isSqlDocument(document) && databaseComboBox->currentIndex() != -1 );
    m_cmdExecuteCurrent->action()->setEnabled(execEnabled);
    m_cmdExecuteAsScript->action()->setEnabled(execEnabled);
}

void QDbConnectionsManager::getConnectionNameList( QVariantList &valueList )
{
    QStringList nameList;

    for (int i = 0; i < connectionList.size(); i++)
        nameList << connectionList[i].getConnectionTitle();

    valueList << nameList;
}

void QDbConnectionsManager::getSchemas( QVariantList &valueList )
{
    valueList << qVariantFromValue((void *) getCurrentSchemas());
}

void QDbConnectionsManager::getTables( QVariantList &valueList )
{
    if ( valueList.size() != 1 )
        return;

    QString schemaName = valueList[0].toString();
    valueList.clear();

    valueList << qVariantFromValue((void *) getCurrentTables(schemaName));
}

void QDbConnectionsManager::getViews( QVariantList &valueList )
{
    if ( valueList.size() != 1 )
        return;

    QString schemaName = valueList[0].toString();
    valueList.clear();

    valueList << qVariantFromValue((void *) getCurrentViews(schemaName));
}

void QDbConnectionsManager::getTablesAndViews( QVariantList &valueList )
{
    if ( valueList.size() != 1 )
        return;

    QString schemaName = valueList[0].toString();
    valueList.clear();

    valueList << qVariantFromValue((void *) getCurrentTables(schemaName))
              << qVariantFromValue((void *) getCurrentViews(schemaName));
}

void QDbConnectionsManager::getIndices( QVariantList &valueList )
{
    if ( valueList.size() != 1 )
        return;

    QString schemaName = valueList[0].toString();
    valueList.clear();

    valueList << qVariantFromValue((void *) getCurrentIndices(schemaName));
}

void QDbConnectionsManager::getTriggers( QVariantList &valueList )
{
    if ( valueList.size() != 1 )
        return;

    QString schemaName = valueList[0].toString();
    valueList.clear();

    valueList << qVariantFromValue((void *) getCurrentTriggers(schemaName));
}

void QDbConnectionsManager::getTableFields( QVariantList &valueList )
{
    if ( valueList.size() != 2 )
        return;

    const QStringList &tableNames = valueList[0].toStringList();
    const QStringList &tableSchemas = valueList[1].toStringList();

    int count = tableNames.size();
    if ( count != tableSchemas.size() )
        return;

    QStringList fields;

    for (int i = 0; i < count; i++)
        getCurrentFields(tableSchemas[i], tableNames[i], &fields);

    fields.removeDuplicates();
    valueList << fields;
}

void QDbConnectionsManager::getTableConstraints( QVariantList &valueList )
{
    if ( valueList.size() != 2 )
        return;

    QString tableName = valueList[0].toString();
    QString schemaName = valueList[1].toString();

    QStringList fields;
    getCurrentConstraints(schemaName, tableName, &fields);

    valueList << fields;
}

void QDbConnectionsManager::executeQueryCurrent()
{
    executeQueryInternal(1);
}

void QDbConnectionsManager::executeQueryAsScript()
{
    executeQueryInternal(0);
}

void QDbConnectionsManager::createConnection(const DbConnectionInfo &connection , bool addToHistory )
{
    IDatabaseLoaderPlugin *loader = loaderByDatabaseType(connection.databaseType);
    if ( !loader )
        return;

    QString dbName = QString("DB_%1").arg(dbIndex);
    dbIndex++;

    BaseConnectionInfo *connectionInfo = loader->createConnection(dbName, connection.parameters);
    if ( !connectionInfo )
        return;
    QString dbTitle = connectionInfo->connectionTitle();

    QDbStructWidget *structWidget = new QDbStructWidget(connectionInfo, loader, mainWindow);

    connectionList.append(DbConnection(loader, connectionInfo));
    databaseComboBox->addItem(dbTitle);
    dbTabWidget->addTab(structWidget, dbTitle);
    dbTabWidget->setTabToolTip(dbTabWidget->count() - 1, "<pre>" + connectionInfo->connectionDescription() + "</pre>");
    structWidgetList.append(structWidget);

    databaseComboBox->setCurrentIndex(databaseComboBox->count() - 1);

    emit(dbConnectionAdded(QVariantList() << dbTitle));

    m_cmdCloseAllDb->action()->setEnabled(true);
    m_cmdCloseDb->action()->setEnabled(true);

    DbConnectionInfo connInfo;
    connInfo.databaseType = loader->databaseType();
    connInfo.description = connectionInfo->connectionDescription();
    connInfo.parameters << connection.parameters;

    if ( addToHistory )
        m_connectionsHistory->addConnection(connInfo);

    connectionInfoList.append(connInfo);
}

void QDbConnectionsManager::onConnectionsHistory()
{
    DbConnectionInfo connection;
    if ( !m_connectionsHistory->selectOrEditConnection(connection) )
        return;

    createConnection(connection);
}

IDatabaseLoaderPlugin *QDbConnectionsManager::loaderByDatabaseType( const QString &dbType )
{
    for (int i = 0; loaderTypeList.size(); i++) {
        if ( dbType == loaderTypeList[i]->databaseType() )
            return loaderTypeList[i];
    }

    return 0;
}

void QDbConnectionsManager::executeQueryInternal( int executeMode )
{
    if ( !connectionList.size() )
        return;

    QIDocument *document = QDocumentManager::getInstance()->currentDocument();
    if ( !document )
        return;

    QIText *textDocument = dynamic_cast<QIText *>(document);
    if ( !textDocument )
        return;

    int connIndex = databaseComboBox->currentIndex();
    if ( connIndex == -1 )
        return;

    QTabWidget *toolsTabWidget = document->toolsTabWidget();
    if ( !toolsTabWidget ) {
        /// TODO: Create Common Tools TabWidget
        return;
    }

    toolsTabWidget->clear();
    toolsTabWidget->show();

    QString connectionName = connectionList[connIndex].getConnectionName();
    QSqlDatabase db = QSqlDatabase::database(connectionName);
    QString queryString;

    if ( executeMode == 2 ) {
        // Execute Script As One Block
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

        bool transactionStarted = connectionList[connIndex].transaction();
        if ( !transactionStarted )
            startTransaction(connIndex);
        QSqlQuery query(db);
        query.setForwardOnly(true);
        queryString = textDocument->text();
        bool queryResult = query.exec(queryString);
        QTextEdit *textEdit = new QTextEdit();
        textEdit->setReadOnly(true);
        if ( queryResult ) {
            textEdit->setText(tr("Executed successfully..."));
            int currentIndex = toolsTabWidget->addTab(textEdit, QIcon(":/images/qr_ok.png"), queryString.left(18).replace('\n', ' '));
            toolsTabWidget->setTabToolTip(currentIndex, "<pre>" + queryString.trimmed() + "</pre>");
            toolsTabWidget->setCurrentIndex(currentIndex);
        }
        else {
            textEdit->setText(tr("Failed: %1").arg(query.lastError().text()));
            int currentIndex = toolsTabWidget->addTab(textEdit, QIcon(":/images/qr_error.png"), queryString.left(18).replace('\n', ' '));
            toolsTabWidget->setCurrentIndex(currentIndex);
            toolsTabWidget->setTabToolTip(currentIndex, "<pre>" + queryString.trimmed() + "</pre>");
            if ( !transactionStarted )
                rollbackTransaction(connIndex);
        }
        emit sqlExecuted(queryString);

        QApplication::restoreOverrideCursor();
        return;
    }

    QString nText = textDocument->text().replace("\r\n", "\n").replace('\r', '\n');
    nText.append(";");
    QString qText = nText;

    QList <QSqlStatement> statementList;
    connectionList[connIndex].getLoader()->parseStatements(nText, statementList);

    if ( !statementList.size() )
        return;

    int startPos = textDocument->selectionStart();
    int endPos   = textDocument->selectionEnd();

    QSet <QString> bindingsSet;
    foreach ( QSqlStatement statement, statementList ) {
        if ( executeMode == 1 ) {
            // Execute Current Selection
            if ( statement.endPosition < startPos )
                continue;

            if ( statement.startPosition > endPos )
                break;
        }
        for (int i = 0; i < statement.bindNames.size(); i++)
            bindingsSet << statement.bindNames[i];
    }

    QMap<QString, SqlBindValue> bindings;
    if ( !m_sqlHelper.defineBindValues(bindingsSet, bindings) )
        return;

    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));

    foreach ( QSqlStatement statement, statementList ) {
        if ( executeMode == 1 ) {
            // Execute Current Selection
            if ( statement.endPosition < startPos )
                continue;

            if ( statement.startPosition > endPos )
                break;
        }
        queryString = qText.mid(statement.startPosition, statement.endPosition - statement.startPosition);
        if ( queryString.trimmed().isEmpty() )
            continue;
        if ( !queryString.startsWith("SELECT", Qt::CaseInsensitive) ) {
            bool transactionStarted = connectionList[connIndex].transaction();
            if ( statement.isBeginTransaction )
                if ( !transactionStarted )
                    startTransaction(connIndex);
            QSqlQuery query(db);
            bool queryResult = m_sqlHelper.prepareBindExecute(&query, queryString, &statement.bindNames, &bindings);
            QTextEdit *textEdit = new QTextEdit();
            textEdit->setReadOnly(true);
            if ( queryResult ) {
                int rowsCount = query.numRowsAffected();
                if ( !queryString.startsWith("INSERT", Qt::CaseInsensitive) &&
                     !queryString.startsWith("UPDATE", Qt::CaseInsensitive) &&
                     !queryString.startsWith("DELETE", Qt::CaseInsensitive) )
                    rowsCount = -1;
                if ( rowsCount == -1 )
                    textEdit->setText(tr("Executed successfully..."));
                else
                    textEdit->setText(tr("Executed successfully... %1 Rows Affected").arg(QString::number(rowsCount)));
                int currentIndex = toolsTabWidget->addTab(textEdit, QIcon(":/images/qr_ok.png"), queryString.left(18).replace('\n', ' '));
                toolsTabWidget->setTabToolTip(currentIndex, "<pre>" + queryString.trimmed() + "</pre>");
                toolsTabWidget->setCurrentIndex(currentIndex);
                if ( transactionStarted ) {
                    if ( statement.isCommitTransaction )
                        commitTransaction(connIndex);
                    if ( statement.isRollbackTransaction )
                        rollbackTransaction();
                }
            }
            else {
                textEdit->setText(tr("Failed: %1").arg(query.lastError().text()));
                int currentIndex = toolsTabWidget->addTab(textEdit, QIcon(":/images/qr_error.png"), queryString.left(18).replace('\n', ' '));
                toolsTabWidget->setTabToolTip(currentIndex, "<pre>" + queryString.trimmed() + "</pre>");
                toolsTabWidget->setCurrentIndex(currentIndex);
                if ( !transactionStarted )
                    rollbackTransaction(connIndex);
            }
        }
        else {
            QRdbQueryModel *queryModel = new QRdbQueryModel(connectionName, queryString, &statement.bindNames, &bindings);
            if ( queryModel->isHasError() ) {
                QTextEdit *textEdit = new QTextEdit(queryModel->getLastError());
                textEdit->setReadOnly(true);
                int currentIndex = toolsTabWidget->addTab(textEdit, QIcon(":/images/qr_error.png"), queryString.left(18).replace('\n', ' '));
                toolsTabWidget->setTabToolTip(currentIndex, "<pre>" + queryString.trimmed() + "</pre>");
                toolsTabWidget->setCurrentIndex(currentIndex);
                delete queryModel;
            }
            else {
                QRdbTableView *queryView = new QRdbTableView();
                queryView->setModel(queryModel);
                const int rowsHeight = QSettingsManager::getInstance()->value(SqlBrowser::C_SQL_QUERY_SETTINGS_ID, "DefaultRowHeight").toInt();
                queryView->verticalHeader()->setDefaultSectionSize(rowsHeight);
                if ( !QSettingsManager::getInstance()->value(SqlBrowser::C_SQL_QUERY_SETTINGS_ID, "ShowVerticalHeader").toBool() )
                    queryView->verticalHeader()->hide();
                QFont font = qvariant_cast<QFont>(QSettingsManager::getInstance()->value(SqlBrowser::C_SQL_QUERY_SETTINGS_ID, "Font"));
                queryView->setFont(font);
                if ( QSettingsManager::getInstance()->value(SqlBrowser::C_SQL_QUERY_SETTINGS_ID, "AutoResizeColumns").toBool() ) {
                    queryView->resizeColumnsToContents();
                    const int maxColWidth = QSettingsManager::getInstance()->value(SqlBrowser::C_SQL_QUERY_SETTINGS_ID, "MaxColumnsWidth").toInt();
                    for (int i = 0; i < queryModel->columnCount(QModelIndex()); i++ )
                        if ( queryView->columnWidth(i) > maxColWidth )
                            queryView->setColumnWidth(i, maxColWidth);
                }
                int currentIndex = toolsTabWidget->addTab(queryView, QIcon(":/images/qr_ok.png"), queryString.left(18).replace('\n', ' '));
                toolsTabWidget->setTabToolTip(currentIndex, "<pre>" + queryString.trimmed() + "</pre>");
                toolsTabWidget->setCurrentIndex(currentIndex);
            }
        }
        emit sqlExecuted(queryString);
    }

    QApplication::restoreOverrideCursor();
}

void QDbConnectionsManager::showTableInfoDialog( BaseConnectionInfo *connInfo, IDatabaseLoaderPlugin *loader,
                                                 const QString &schemaName, bool viewMode, const QString &tableName,
                                                 int tableObjectType, const QString &tableObjectName )
{
    if ( tableName.isEmpty() )
        return;

    QTableInfoDialog tableInfoDialog(connInfo, loader, schemaName, viewMode, tableName, tableObjectType, tableObjectName, mainWindow);

    QSettingsManager *settingsManager = QSettingsManager::getInstance();

    int x       = settingsManager->value(SqlBrowser::C_SQL_TABLEINFO_SETTINGS_ID, "x").toInt();
    int y       = settingsManager->value(SqlBrowser::C_SQL_TABLEINFO_SETTINGS_ID, "y").toInt();
    int width   = settingsManager->value(SqlBrowser::C_SQL_TABLEINFO_SETTINGS_ID, "width").toInt();
    int height  = settingsManager->value(SqlBrowser::C_SQL_TABLEINFO_SETTINGS_ID, "height").toInt();
    tableInfoDialog.setGeometry(x, y, width, height);

    tableInfoDialog.exec();

    QRect rect= tableInfoDialog.geometry();
    settingsManager->value(SqlBrowser::C_SQL_TABLEINFO_SETTINGS_ID, "x")      = rect.x();
    settingsManager->value(SqlBrowser::C_SQL_TABLEINFO_SETTINGS_ID, "y")      = rect.y();
    settingsManager->value(SqlBrowser::C_SQL_TABLEINFO_SETTINGS_ID, "width")  = rect.width();
    settingsManager->value(SqlBrowser::C_SQL_TABLEINFO_SETTINGS_ID, "height") = rect.height();
}

void QDbConnectionsManager::showSourceViewDialog( const QString &title, const QString &defaultExt, const QString &sourceText )
{
    QSourceViewDialog sourceViewDialog(title, defaultExt, sourceText, mainWindow);

    QSettingsManager *settingsManager = QSettingsManager::getInstance();

    int x       = settingsManager->value(SqlBrowser::C_SQL_SOURCEVIEW_SETTINGS_ID, "x").toInt();
    int y       = settingsManager->value(SqlBrowser::C_SQL_SOURCEVIEW_SETTINGS_ID, "y").toInt();
    int width   = settingsManager->value(SqlBrowser::C_SQL_SOURCEVIEW_SETTINGS_ID, "width").toInt();
    int height  = settingsManager->value(SqlBrowser::C_SQL_SOURCEVIEW_SETTINGS_ID, "height").toInt();
    sourceViewDialog.setGeometry(x, y, width, height);

    sourceViewDialog.exec();

    QRect rect = sourceViewDialog.geometry();
    settingsManager->value(SqlBrowser::C_SQL_SOURCEVIEW_SETTINGS_ID, "x")      = rect.x();
    settingsManager->value(SqlBrowser::C_SQL_SOURCEVIEW_SETTINGS_ID, "y")      = rect.y();
    settingsManager->value(SqlBrowser::C_SQL_SOURCEVIEW_SETTINGS_ID, "width")  = rect.width();
    settingsManager->value(SqlBrowser::C_SQL_SOURCEVIEW_SETTINGS_ID, "height") = rect.height();
}

QStringList *DbConnection::SchemaMetaData::getTables( IDatabaseLoaderPlugin *loader_, BaseConnectionInfo *connectionInfo_ )
{
    if ( !m_tablesLoaded ) {
        QList <QList <QVariant> >items;
        if ( !m_schemaName.isEmpty() ) {
            QList <QVariant> item;
            item << m_schemaName;
            items << item;
        }
        m_tablesLoaded = loader_->loadTables(connectionInfo_, items);
        if ( m_tablesLoaded ) {
            for (int i = 0; i < items.size(); i++)
                tables << items[i][0].toString();
        }
    }
    return &tables;
}

QStringList *DbConnection::SchemaMetaData::getViews( IDatabaseLoaderPlugin *loader_, BaseConnectionInfo *connectionInfo_ )
{
    if ( !m_viewsLoaded ) {
        QList <QList <QVariant> >items;
        if ( !m_schemaName.isEmpty() ) {
            QList <QVariant> item;
            item << m_schemaName;
            items << item;
        }
        m_viewsLoaded = loader_->loadViews(connectionInfo_, items);
        if ( m_viewsLoaded ) {
            for (int i = 0; i < items.size(); i++)
                views << items[i][0].toString();
        }
    }
    return &views;
}

QStringList *DbConnection::SchemaMetaData::getIndices( IDatabaseLoaderPlugin *loader_, BaseConnectionInfo *connectionInfo_ )
{
    if ( !m_indicesLoaded ) {
        QList <QList <QVariant> >items;
        if ( !m_schemaName.isEmpty() ) {
            QList <QVariant> item;
            item << m_schemaName;
            items << item;
        }
        m_indicesLoaded = loader_->loadIndices(connectionInfo_, items);
        if ( m_indicesLoaded ) {
            for (int i = 0; i < items.size(); i++)
                indices << items[i][0].toString();
        }
    }
    return &indices;
}

QStringList *DbConnection::SchemaMetaData::getTriggers( IDatabaseLoaderPlugin *loader_, BaseConnectionInfo *connectionInfo_ )
{
    if ( !m_triggersLoaded ) {
        QList <QList <QVariant> >items;
        if ( !m_schemaName.isEmpty() ) {
            QList <QVariant> item;
            item << m_schemaName;
            items << item;
        }
        m_triggersLoaded = loader_->loadTriggers(connectionInfo_, items);
        if ( m_triggersLoaded ) {
            for (int i = 0; i < items.size(); i++)
                triggers << items[i][0].toString();
        }
    }
    return &triggers;
}

DbConnection::SchemaMetaData *DbConnection::MetaData::findOrInsertSchema( BaseConnectionInfo *connectionInfo_, const QString &schemaName_ )
{
    QString schemaName = schemaName_;
    if ( schemaName.isEmpty() )
        schemaName = connectionInfo_->defaultSchemaName();

    QHash<QString, DbConnection::SchemaMetaData *>::const_iterator it = m_schemas.find(schemaName);
    if ( it != m_schemas.end() )
        return it.value();

    DbConnection::SchemaMetaData *newSchema = new DbConnection::SchemaMetaData(schemaName);
    m_schemas[schemaName] = newSchema;
    return newSchema;
}

QStringList *DbConnection::MetaData::getSchemas( IDatabaseLoaderPlugin *loader_, BaseConnectionInfo *connectionInfo_ )
{
    if ( !m_schemasLoaded && loader_->multiSchemaSupported() ) {
        QList <QList <QVariant> >items;
        m_schemasLoaded = loader_->loadSchemaNames(connectionInfo_, items);
        if ( m_schemasLoaded )
            for (int i = 0; i < items.size(); i++)
                m_schemaNames << items[i][0].toString().trimmed();
    }
    return &m_schemaNames;
}

QStringList *DbConnection::MetaData::getTables(IDatabaseLoaderPlugin *loader_, BaseConnectionInfo *connectionInfo_ , const QString &schemaName_ )
{
    QString schemaName = schemaName_;
    if ( schemaName.isEmpty() )
        schemaName = connectionInfo_->defaultSchemaName();
    return findOrInsertSchema(connectionInfo_, schemaName)->getTables(loader_, connectionInfo_);
}

QStringList *DbConnection::MetaData::getViews( IDatabaseLoaderPlugin *loader_, BaseConnectionInfo *connectionInfo_, const QString &schemaName_ )
{
    QString schemaName = schemaName_;
    if ( schemaName.isEmpty() )
        schemaName = connectionInfo_->defaultSchemaName();
    return findOrInsertSchema(connectionInfo_, schemaName)->getViews(loader_, connectionInfo_);
}

QStringList *DbConnection::MetaData::getIndices(IDatabaseLoaderPlugin *loader_, BaseConnectionInfo *connectionInfo_ , const QString &schemaName_ )
{
    return findOrInsertSchema(connectionInfo_, schemaName_)->getIndices(loader_, connectionInfo_);
}

QStringList *DbConnection::MetaData::getTriggers(IDatabaseLoaderPlugin *loader_, BaseConnectionInfo *connectionInfo_ , const QString &schemaName_ )
{
    return findOrInsertSchema(connectionInfo_, schemaName_)->getTriggers(loader_, connectionInfo_);
}

Q_EXPORT_PLUGIN2(QDbConnectionsManager, QDbConnectionsManager)
