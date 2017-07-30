#ifndef SQLPARSERENGINEMANAGER_H
#define SQLPARSERENGINEMANAGER_H

#include <QObject>

#include "IParserEngine.h"
#include "DbConnection.h"
#include "sqlengine_global.h"

class SQL_EXPORT SqlParserEngineManager: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(SqlParserEngineManager)

    explicit SqlParserEngineManager( QObject *parent = 0 );

public:
    static SqlParserEngineManager *instance();
    virtual ~SqlParserEngineManager();

    Sql::IParserEngine *currentEngine();
    Database::DbConnection *currentConnection();
    
signals:
    void currentEngineChanged( Sql::IParserEngine *engine );
    
public slots:
    void onCurrentEngineChanged( Sql::IParserEngine *engine );
    void onCurrentConnectionChanged( Database::DbConnection *connection );

private:
    static SqlParserEngineManager *m_instance;
    Sql::IParserEngine *m_defaultParserEngine;
    Sql::IParserEngine *m_currentParserEngine;
    Database::DbConnection *m_currentConnection;
    
};

#endif // SQLPARSERENGINEMANAGER_H
