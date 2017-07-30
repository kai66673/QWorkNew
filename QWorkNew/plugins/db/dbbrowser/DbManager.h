#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "dbbrowser_global.h"

#include "IPlugin.h"
#include "ISessionClient.h"
#include "Core.h"
#include "CloseApplicationHandler.h"

#include "SqlBindHelper.h"
#include "DbConnectionsHistory.h"
#include "DbLoader.h"
#include "DbConnection.h"
#include "QDbStructWidget.h"
#include "PythonQtObjectStorage.h"

QT_BEGIN_NAMESPACE
class QStackedWidget;
class QComboBox;
QT_END_NAMESPACE

namespace Python {
class IPythonModule;
class PythonModuleFunction;
}

namespace Database {

class DbDetailsWidget;
class DatabaseManagerPythonModule;

class DbTabWidget: public QTabWidget
{
    Q_OBJECT

public:
    DbTabWidget(QWidget *parent = 0);
};

class SettingsCategoryViewQuery : public ISettings
{
public:
    SettingsCategoryViewQuery();

    virtual QString iconName() const { return QStringLiteral(":/images/set_query_result.png"); }
    virtual QString title() const { return QObject::tr("Query View"); }
    virtual ISettingsTabPage *createSettingsTabPage( QWidget *parent = 0 );
    virtual QString groupName() const { return QStringLiteral("Query_View"); }
    virtual void doApplySettings();
};

class DBBROWSER_EXPORT DbManager: public IPlugin, public ISessionClient, public ICloseApplicationClient
{
    Q_OBJECT
    Q_INTERFACES(IPlugin)
#ifdef HAVE_QT5
    Q_PLUGIN_METADATA(IID "QWorkBase.IPlugin/1.0")
#endif

    friend class DatabaseManagerPythonModule;
    friend class DatabaseConnection;

public:
    DbManager();

    static DbManager *getInstance();
    IDbLoader *getCurrentLoader() const;
    IDbCatalogItem *getCurrentItem() const;

    // IPlugin implementation
    virtual QString id() const { return QStringLiteral("DatabaseManager"); }
    virtual QMenu *menu();

    virtual QList<QByteArray> signalList() const {
        return QList<QByteArray>()
                << "dbConnectionAdded"
                << "dbConnectionClosed"
                << "currentDbConnectionChanged";
    }
    virtual void initialize();
    virtual QList<UiProviderInterface *> createUiProviders( IMainWindow *m_mainWindow );

    bool confirmEndStartedTransactions();
    void startTransaction( int currDbIndex = -1 );

    // QISessionClient implementation
    virtual bool sessionAboutToClose();
    virtual void sessionClose();
    virtual void saveSession( const QString &location );
    virtual void loadSession( const QString &location );

    // QIAppCloseReflectorClient implementation
    virtual bool applicationAboutToClose();
    virtual void applicationClose();
    virtual void destroy();

    void reflectOnQueryViewSettingsChanged( const QString &splitterSymbol );
    IDbLoader *loaderByDatabaseType( const QString &dbType );
    DbConnectionsHistory *connectionsHistory() { return m_connectionsHistory; }

    inline QAction *itemAction( int index ) const { return m_DbItemActions[index]; }

signals:
    void transactionModeChanged( bool transaction );
    void sqlExecuted( const QString &text );
    void currentDbConnectionChanged( QVariantList &valueList );

    void dbConnectionAdded( QVariantList &valuesList );
    void dbConnectionClosed( QVariantList &valuesList );
    void sqlHistoryEnabledChanged();
    void clipboardSplitterChanged( const QString &splitterSymbol );
    void connectionClosed( DbConnection * );

public slots:
    void connectToDb();
    void closeDbConnection();
    void closeAllDbConnections();
    void onConnectionsHistory();
    void executeQueryCurrent();
    void executeQueryAsScript();
    void commitTransaction( int currDbIndex = -1 );
    void rollbackTransaction( int currDbIndex = -1 );
    void onCurrentConnectionChanged( int index );
    void onCurrentDocumentChanged( IDocument *document );

    void onRefresh();
    void onFilter();

    void onCurrentDbIndexChanged( int index );

    void onStatementExecuted( const QString &queryString, bool success, bool isSelectStattement, const QString &message, QList <QList <QVariant> > *queryData );
    void onTransactionChanged( DbConnection *connection );

    QList<QAction *> dbDetailsActions() const { return QList<QAction *>() << m_cmdDetailsPrev->action() << m_cmdDetailsNext->action() << m_cmdDetailsClear->action(); }

protected:
    virtual QStringList settingsKeyList() const;
    virtual ISettings *createSettings( const QString &settingsKey ) const;

public slots:
    void onDetails();
    void onDetailsForItem(IDbCatalogItem *item , DbConnection *connection);
    void onCopyName();
    void onQueryData();
    void onSqlHistoryItemSelected( bool onoff );
    void onQueryViewSettingsChanged();

private slots:
    void onDetailsClear();
    void onDetailsNext();
    void onDetailsPrevious();
    void onDbItemActionTriggered();

private:
    bool createConnection( const DbConnectionAuthInfo &connection, bool addToHistory = true );
    bool closeConnection( int index );
    bool closeDatabaseConnection( DbConnection *connection );
    void executeQueryInternal( int executeMode );
    bool isSqlDocument( IDocument *document );
    DbDetailsWidget *currentDetailsWidget( int index = -1 );

    IMainWindow *m_mainWindow;
    QComboBox   *databaseComboBox;
    DbTabWidget *dbTabWidget;

    int dbIndex;
    QList <DbConnection *> connectionList;
    QList <DbConnectionAuthInfo> connectionInfoList;
    QList <QDbStructWidget *> structWidgetList;

    static DbManager *connectionsManager;

    void dbItemActionTriggered( QDbItemAction *action );
    void registerLoaderItemActions( IDbLoader *loader );
    void loadLoaderPlugins();
    QList <IDbLoader *> loaderTypeList;

    void createActions();
    Core::Command *m_cmdConnectDb;
    Core::Command *m_cmdCloseDb;
    Core::Command *m_cmdCloseAllDb;
    Core::Command *m_cmdCommit;
    Core::Command *m_cmdRollback;
    Core::Command *m_cmdExecuteCurrent;
    Core::Command *m_cmdExecuteAsScript;
    Core::Command *m_cmdConnHistory;
    Core::Command *m_cmdRefreshDb;
    Core::Command *m_cmdFilterDb;

    Core::Command *m_cmdDetailsClear;
    Core::Command *m_cmdDetailsNext;
    Core::Command *m_cmdDetailsPrev;

    void createContextMenuActions();
    QAction *m_actDetails;
    QAction *m_actCopyName;
    QAction *m_actQueryData;
    QList<QAction *> m_DbItemActions;

    Sql::SqlBindHelper m_sqlHelper;
    DbConnectionsHistory *m_connectionsHistory;

    QStackedWidget      *m_detailsStack;
    QwCustomDockWidget  *m_dbDock;
    QwCustomDockWidget  *m_detailsDock;
    QwTabBarDockWidget  *m_queryDock;

    void createSqlHistoryActions();
    Core::Command *m_cmdSqlHistoryOpen;
    Core::Command *m_cmdSqlHistoryRemove;
    Core::Command *m_cmdSqlHistoryClear;
    Core::Command *m_cmdToggleHistoryEnabled;
    Core::Command *m_cmdSqlHistoryFilter;

    void createQueryTabActions();
    Core::Command *m_cmdQueryTabNext;
    Core::Command *m_cmdQueryTabPrev;
    Core::Command *m_cmdQueryTabClose;
    Core::Command *m_cmdQueryTabCloseAll;

    int m_qssDetailsWathcherIndex;
};

class DatabaseCursor: public QObject
{
    Q_OBJECT

public:
    DatabaseCursor( const QString &dbName ,const QString &queryText, const QMap<QString, QVariant> &bindings );
    ~DatabaseCursor();

    bool isValid() const;

public slots:
    QString lastError() const;
    bool next();
    QVariant value( int column );
    int columnCount();
    QString columnNameAt( int index );
    void close();

private:
    QString m_dbName;
    QString m_queryText;
    QMap<QString, QVariant> m_bindings;
    bool m_valid;

    QSqlQuery *m_query;
    QString m_lastError;
};

class DatabaseConnection: public Core::PythonQtObject
{
    Q_OBJECT

    friend class DatabaseManagerPythonModule;

public:
    DatabaseConnection(Core::PythonQtStorageDestroyDelegate *delegate, DbConnection *connection);
    ~DatabaseConnection();

public slots:
    bool close();

    QStringList schemas();

    QStringList tables( const QString &schemaName = QString() );
    QStringList views( const QString &schemaName = QString() );
    QStringList indexes( const QString &schemaName = QString() );
    QStringList triggers( const QString &schemaName = QString() );
    QStringList constraints( const QString &schemaName = QString() );
    QStringList schemaObjectNames( const QString &schemaName, unsigned objectType );

    QStringList tableColumns( const QString &tableName, const QString &schemaName = QString() );
    QStringList viewColumns( const QString &viewName, const QString &schemaName = QString() );
    QStringList tableIndexes( const QString &tableName, const QString &schemaName = QString() );
    QStringList tableTriggers( const QString &tableName, const QString &schemaName = QString() );
    QStringList tableConstraints( const QString &tableName, const QString &schemaName = QString() );
    QStringList schemaObjectObjectNames( const QString &schemaName, const QString &objectName,
                                         unsigned objectType, unsigned objectSubtype );

    DatabaseCursor *openCursor( const QString &queryText, const QMap<QString, QVariant> &bindings = QMap<QString, QVariant>() );

protected:
    virtual void clear();

private:
    DbManager *m_manager;
    DbConnection *m_connection;

    QList<DatabaseCursor *> m_cursors;
};

class DatabaseManagerPythonModule: public Python::IPythonModule
{
    Q_OBJECT

public:
    DatabaseManagerPythonModule( DbManager *manager );
    virtual ~DatabaseManagerPythonModule();

    virtual QList<Python::PythonModuleFunction *> functions() const;
    virtual QString sourceCodeForAssist() const;

    virtual QMetaObject *metaObjectForFunction( const QString &functionName ) const;

public slots:
    QStringList databaseTypesSupported();
    DatabaseConnection *createConnection( const QString &dbType, const QStringList &parameters , bool addToConnectionHistory );
    DatabaseConnection *newConnection();
    DatabaseConnection *newConnectionFromHistory();
    bool closeConnection( int index );
    bool closeAllConnections();
    int  currentConnectionIndex();
    bool setCurrentConnectionIndex( int index );
    int  connectionsCount();

    DatabaseConnection *currentConnection();
    DatabaseConnection *connectionAt( int connectionIndex );

private:
    DatabaseConnection *pythonConnection( int connectionIndex );

    DbManager *m_manager;
    QList<Python::PythonModuleFunction *> m_functions;

    Core::PythonQtObjectStorage<DbConnection, DatabaseConnection> m_pythonConnectionsStorage;
};

} // Database

#endif // DBMANAGER_H
