#ifndef QDBCONNECTIONSMANAGER_H
#define QDBCONNECTIONSMANAGER_H

#include <QObject>
#include <QList>
#include <QDomDocument>

#include "connmanagerlib_global.h"

#include "IDatabaseLoaderPlugin.h"
#include "QIPlugin.h"
#include "QISessionClient.h"
#include "QIAppCloseReflectorClient.h"
#include "QSettingsManager.h"
#include "SqlBindHelper.h"
#include "DbConnectionsHistory.h"
#include "QCommandManager.h"

QT_BEGIN_NAMESPACE
class QAction;
class QComboBox;
class QTabWidget;
class QMainWindow;
class QSqlQueryModel;
QT_END_NAMESPACE

class QDbStructWidget;
class DbStructItem;
class QDbConnectionsManager;

class DbConnection
{
public:
    DbConnection( IDatabaseLoaderPlugin *loader_, BaseConnectionInfo *connectionInfo_ );

    BaseConnectionInfo *getConnectionInfo() const { return connectionInfo; }
    QString getConnectionName() const { return connectionName; }
    QString getConnectionTitle() const { return connectionTitle; }
    IDatabaseLoaderPlugin *getLoader() const { return loader; }
    bool transaction() const { return transactionStarted; }

    bool closeConnection( QMainWindow *mainWindow );
    bool confirmEndStartedTransaction( QMainWindow *mainWindow );

    bool startTransaction();
    bool commitTransaction();
    bool rollbackTransaction();

    QStringList *getSchemas() { return m_metaData.getSchemas(loader, connectionInfo); }
    QStringList *getTables( const QString &schemaName = "" ) { return m_metaData.getTables(loader, connectionInfo, schemaName); }
    QStringList *getViews( const QString &schemaName = "" ) { return m_metaData.getViews(loader, connectionInfo, schemaName); }
    QStringList *getIndices( const QString &schemaName = "" ) { return m_metaData.getIndices(loader, connectionInfo, schemaName); }
    QStringList *getTriggers( const QString &schemaName = "" ) { return m_metaData.getTriggers(loader, connectionInfo, schemaName); }
    void getTableFields( const QString &schemaName, const QString &tableName, QStringList *tableFields );
    void getTableConstraints( const QString &schemaName, const QString &tableName, QStringList *tableConstraints );

    void refreshMetaData() { m_metaData.clear(); }

private:
    IDatabaseLoaderPlugin *loader;
    bool transactionStarted;
    BaseConnectionInfo *connectionInfo;

    QString connectionName;
    QString connectionTitle;

    bool hasTransactionFeature;

    struct SchemaMetaData {
        SchemaMetaData( const QString &schemaName = "" )
            : m_tablesLoaded(false)
            , m_viewsLoaded(false)
            , m_indicesLoaded(false)
            , m_triggersLoaded(false)
            , m_schemaName(schemaName)
        {}
        void clear() {
            tables.clear();
            views.clear();
            indices.clear();
            triggers.clear();
            m_tablesLoaded = false;
            m_viewsLoaded = false;
            m_indicesLoaded = false;
            m_triggersLoaded = false;
        }
        QStringList tables;
        QStringList views;
        QStringList indices;
        QStringList triggers;
        bool m_tablesLoaded;
        bool m_viewsLoaded;
        bool m_indicesLoaded;
        bool m_triggersLoaded;
        QString m_schemaName;
        QStringList *getTables( IDatabaseLoaderPlugin *loader_, BaseConnectionInfo *connectionInfo_ );
        QStringList *getViews( IDatabaseLoaderPlugin *loader_, BaseConnectionInfo *connectionInfo_ );
        QStringList *getIndices( IDatabaseLoaderPlugin *loader_, BaseConnectionInfo *connectionInfo_ );
        QStringList *getTriggers( IDatabaseLoaderPlugin *loader_, BaseConnectionInfo *connectionInfo_ );
    };

    class MetaData {
    public:
        MetaData()
            : m_schemasLoaded(false)
        {}
        virtual ~MetaData() { clear(); }
        void clear() {
            m_schemaNames.clear();
            qDeleteAll(m_schemas);
            m_schemas.clear();
            m_schemasLoaded = false;
        }

        SchemaMetaData *findOrInsertSchema( BaseConnectionInfo *connectionInfo_, const QString &schemaName_ );

        QStringList *getSchemas( IDatabaseLoaderPlugin *loader_, BaseConnectionInfo *connectionInfo_ );
        QStringList *getTables( IDatabaseLoaderPlugin *loader_, BaseConnectionInfo *connectionInfo_, const QString &schemaName_ );
        QStringList *getViews( IDatabaseLoaderPlugin *loader_, BaseConnectionInfo *connectionInfo_, const QString &schemaName_ );
        QStringList *getIndices( IDatabaseLoaderPlugin *loader_, BaseConnectionInfo *connectionInfo_, const QString &schemaName_ );
        QStringList *getTriggers( IDatabaseLoaderPlugin *loader_, BaseConnectionInfo *connectionInfo_, const QString &schemaName_ );

        QStringList m_schemaNames;
        bool m_schemasLoaded;

        QHash<QString, SchemaMetaData *> m_schemas;
    };

    MetaData m_metaData;
};

class QSettingsCategorySqlEditor : public QISettings
{
public:
    QSettingsCategorySqlEditor();

    virtual QString iconName() const { return ":/images/set_sql_editor.png"; }
    virtual QString title() const { return QObject::tr("SQL Editor"); }
    virtual QWidget *createSettingsWidget( QWidget *parent = 0 );
    virtual QString groupName() const { return "SQL_Editor"; }
    virtual void applySettings( QWidget *settingsWidget = 0 );
};

class QSettingsCategoryViewQuery : public QISettings
{
public:
    QSettingsCategoryViewQuery();

    virtual QString iconName() const { return ":/images/set_query_result.png"; }
    virtual QString title() const { return QObject::tr("Query View"); }
    virtual QWidget *createSettingsWidget( QWidget *parent = 0 );
    virtual QString groupName() const { return "Query_View"; }
    virtual void applySettings( QWidget *settingsWidget = 0 );
};

class QSettingsCategoryTableInfoDialog : public QISettings
{
public:
    QSettingsCategoryTableInfoDialog();

    virtual QString iconName() const { return QString(); }
    virtual QString title() const { return QString(); }
    virtual QWidget *createSettingsWidget( QWidget *parent = 0 ) { Q_UNUSED(parent); return NULL; }
    virtual QString groupName() const { return "Table_Info_Dialog"; }
    virtual void applySettings( QWidget *settingsWidget = 0 ) { Q_UNUSED(settingsWidget); }
};

class QSettingsCategorySourceViewDialog : public QISettings
{
public:
    QSettingsCategorySourceViewDialog();

    virtual QString iconName() const { return QString(); }
    virtual QString title() const { return QString(); }
    virtual QWidget *createSettingsWidget( QWidget *parent = 0 ) { Q_UNUSED(parent); return NULL; }
    virtual QString groupName() const { return "Source_View_Dialog"; }
    virtual void applySettings( QWidget *settingsWidget = 0 ) { Q_UNUSED(settingsWidget); }
};

class DbConnectionsUiProvider: public UiProviderInterface
{
public:
    DbConnectionsUiProvider( QIPlugin *plugin, QwDockWidget *dock )
        : UiProviderInterface(plugin, dock)
    {}

    virtual int     pluginUiProviderIndex() const { return 0; }
    virtual QString dockWidgetName() const { return QObject::tr("DatabaseBrowserDockWidget"); }
    virtual QString toolBarName() const { return QObject::tr("Database Browser"); }
    virtual QString toolBarObjectName() const { return QObject::tr("DatabaseBrowserToolBar"); }
    virtual QString menuBarName() const { return QObject::tr("&Database"); }
    virtual QKeySequence toggleDockKeySequence() const { return QKeySequence(QObject::tr("Ctrl+D,D")); }
};

class DbSqlHistoryUiProvider: public UiProviderInterface
{
public:
    DbSqlHistoryUiProvider( QIPlugin *plugin, QwDockWidget *dock )
        : UiProviderInterface(plugin, dock)
    {}

    virtual int     pluginUiProviderIndex() const { return 1; }
    virtual QString dockWidgetName() const { return QObject::tr("DbSqlHistoryDockWidget"); }
    virtual QString toolBarObjectName() const { return QObject::tr("DbSqlHistoryToolBar"); }
    virtual QKeySequence toggleDockKeySequence() const { return QKeySequence(QObject::tr("Ctrl+D,H")); }
};

class CONNMANAGERSHARED_EXPORT QDbConnectionsManager : public QIPlugin, public QISessionClient, public QIAppCloseReflectorClient
{
    Q_OBJECT
    Q_INTERFACES(QIPlugin)

public:
    QDbConnectionsManager();
    ~QDbConnectionsManager();

    virtual void populateMenu( int pluginUiIndex, QMenu *menu );
    virtual void populateToolBar( int pluginUiIndex, QToolBar *toolBar );

    virtual QList<QByteArray> signalList() const {
        return QList<QByteArray>()
                << "dbConnectionAdded"
                << "dbConnectionClosed";
    }

    virtual QList<QByteArray> slotList() const {
        return QList<QByteArray>()
                << "getConnectionNameList"
                << "getSchemas"
                << "getTables"
                << "getViews"
                << "getTablesAndViews"
                << "getTableFields"
                << "getIndices"
                << "getTriggers"
                << "getTableConstraints";
    }

    virtual void initialize();

    virtual QList<UiProviderInterface *> createUiProviders( QWidget *parent = 0 );

    bool confirmEndStartedTransactions();

    // QISessionClient implementation
    virtual bool sesssioAboutToClose();
    virtual void sessionClose();
    virtual void saveSession( const QString &location );
    virtual void loadSession( const QString &location );

    // QIAppCloseReflectorClient implementation
    virtual bool applicationAboutToClose();
    virtual void applicationClose();
    virtual void destroy();

protected:
    virtual QStringList settingsKeyList() const;
    virtual QISettings *createSettings( const QString &settingsKey ) const;

public:
    QString getConnectionName() const;
    BaseConnectionInfo *getConnectionInfo() const;

    static QDbConnectionsManager *getInstance();

    static void parseStatements( QString &text, QList <QSqlStatement> &statements, char splitSymbol = ';' );

    void emitInsertText( const QString &text, const QString &extension );

    bool hasConnections() const { return (connectionList.size() > 0); }

    DbStructItem *getCurrentItem( int childIndex = -1 ) const;
    bool getCurrentSchemaName( QString &schemaName ) const;
    bool getCurrentSchemaName( IDatabaseLoaderPlugin *loader, DbStructItem *item, QString &schemaName );
    IDatabaseLoaderPlugin *getCurrentLoader() const;
    QString getSelectStarQueryString() const;
    QString getSelectAllQueryString() const;
    QString getSelectChoosenQueryString() const;

    bool getChoosenTableFieldItems( QList <DbStructItem *> &fieldItems ) const;

    void startTransaction( int currDbIndex = -1 );

    QStringList *getCurrentSchemas();
    QStringList *getCurrentTables( const QString &schemaName );
    QStringList *getCurrentViews( const QString &schemaName );
    QStringList *getCurrentIndices( const QString &schemaName );
    QStringList *getCurrentTriggers( const QString &schemaName );
    void getCurrentFields( const QString &schemaName, const QString &tableName, QStringList *tableFields );
    void getCurrentConstraints( const QString &schemaName, const QString &tableName, QStringList *tableConstraints );

    void reflectOnQueryViewSettingsChanged();
    IDatabaseLoaderPlugin *loaderByDatabaseType( const QString &dbType );
    DbConnectionsHistory *connectionsHistory() { return m_connectionsHistory; }

signals:
    void transactionModeChanged( bool transaction );
    void sqlExecuted( const QString &text );

    void dbConnectionAdded( QVariantList &valuesList );
    void dbConnectionClosed( QVariantList &valuesList );
    void sqlHistoryEnabledChanged();

public slots:
    void connectToDb();
    void closeDbConnection();
    void closeAllDbConnections();

    void onCreateQuerySelectStar();
    void onCreateQuerySelectAll();
    void onCreateQuerySelectChoosen();
    void onViewTriggerSource();
    void onViewFuncProcSource();
    void onViewModuleSource();

    void onTableDetails();
    void onViewDetails();

    void commitTransaction( int currDbIndex = -1 );
    void rollbackTransaction( int currDbIndex = -1 );

    void refreshMetaData();

    void onCurrentConnectionChanged(int);
    void onCurrentDocumentChanged( QIDocument *document );

    void getConnectionNameList( QVariantList &valueList );
    void getSchemas( QVariantList &valueList );
    void getTables( QVariantList &valueList );
    void getViews( QVariantList &valueList );
    void getTablesAndViews( QVariantList &valueList );
    void getIndices( QVariantList &valueList );
    void getTriggers( QVariantList &valueList );
    void getTableFields( QVariantList &valueList );
    void getTableConstraints( QVariantList &valueList );

    void executeQueryCurrent();
    void executeQueryAsScript();

    void onConnectionsHistory();

private:
    void createContextMenuActions();
    void executeQueryInternal( int executeMode );
    bool isSqlDocument( QIDocument *document );

    void createConnection( const DbConnectionInfo &connection, bool addToHistory = true );

    QComboBox   *databaseComboBox;
    QTabWidget  *dbTabWidget;
    QMainWindow *mainWindow;

    int dbIndex;
    QList <DbConnection> connectionList;
    QList <DbConnectionInfo> connectionInfoList;
    QList <QDbStructWidget *> structWidgetList;

    static QDbConnectionsManager *connectionsManager;

    void loadLoaderPlugins();
    QList <IDatabaseLoaderPlugin *> loaderTypeList;

public:
    QAction *actCreateQuerySelectStar;
    QAction *actCreateQuerySelectAll;
    QAction *actCreateQuerySelectChoosen;
    QAction *actViewTriggerSource;
    QAction *actViewFuncProcSource;
    QAction *actViewModuleSource;

    QAction *actTableDetails;
    QAction *actViewDetails;

private:
    void createActions();
    Core::Command *m_cmdConnectDb;
    Core::Command *m_cmdCloseDb;
    Core::Command *m_cmdCloseAllDb;
    Core::Command *m_cmdCommit;
    Core::Command *m_cmdRollback;
    Core::Command *m_cmdExecuteCurrent;
    Core::Command *m_cmdExecuteAsScript;
    Core::Command *m_cmdConnHistory;

    void showSourceViewDialog( const QString &title, const QString &extension, const QString &sourceText );
    void showTableInfoDialog( BaseConnectionInfo *connInfo, IDatabaseLoaderPlugin *loader,
                              const QString &schemaName, bool viewMode, const QString &tableName,
                              int tableObjectType, const QString &tableObjectName );

    SqlBindHelper m_sqlHelper;
    DbConnectionsHistory *m_connectionsHistory;
};

#endif // QDBCONNECTIONSMANAGER_H
