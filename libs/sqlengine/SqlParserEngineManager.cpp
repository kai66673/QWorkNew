#include <QDebug>

#include "SqlParserEngineManager.h"
#include "SqlParserEngine.h"

SqlParserEngineManager *SqlParserEngineManager::m_instance(0);

SqlParserEngineManager::SqlParserEngineManager( QObject *parent )
    : QObject(parent)
    , m_defaultParserEngine(new Sql::SqlParserEngine())
    , m_currentParserEngine(m_defaultParserEngine)
    , m_currentConnection(0)
{
}

SqlParserEngineManager *SqlParserEngineManager::instance()
{
    if ( !m_instance )
        m_instance = new SqlParserEngineManager();
    return m_instance;
}

SqlParserEngineManager::~SqlParserEngineManager()
{
    delete m_defaultParserEngine;
}

Sql::IParserEngine *SqlParserEngineManager::currentEngine()
{
    return m_currentParserEngine ? m_currentParserEngine : m_defaultParserEngine;
}

Database::DbConnection *SqlParserEngineManager::currentConnection()
{
    return m_currentConnection;
}

void SqlParserEngineManager::onCurrentEngineChanged( Sql::IParserEngine *engine )
{
    if ( m_currentParserEngine == engine )
        return;

    if ( !engine ) {
        if ( m_currentParserEngine == m_defaultParserEngine )
            return;
        m_currentParserEngine = m_defaultParserEngine;
    }
    else
        m_currentParserEngine = engine;

    emit currentEngineChanged(m_currentParserEngine);
}

void SqlParserEngineManager::onCurrentConnectionChanged( Database::DbConnection *connection )
{
    m_currentConnection = connection;
}
