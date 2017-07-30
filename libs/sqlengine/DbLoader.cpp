#include <QSqlError>
#include <QSqlRecord>

#include "DbLoader.h"
#include "LogManager.h"
#include "sql_constants.h"

namespace Database {

void IDbLoader::fillDbCategories(QStringList &categories) const
{
    categories << standartDbCategories() << additionalDbCategories();
}

void IDbLoader::fillSchemaCategories( QStringList &categories ) const
{
    categories << standartSchemaCategories() << additionalSchemaCategories();
}

QStringList IDbLoader::additionalSchemaCategories() const
{
    return QStringList();
}

QStringList IDbLoader::standartSchemaObjectCategories( unsigned schemaObjectType ) const
{
    QStringList categories;

    switch ( schemaObjectType ) {

        case Sql::Constants::DbSchemaObjectType::C_TABLE:
            categories << QObject::tr("Columns")
                       << QObject::tr("Indexes")
                       << QObject::tr("Triggers")
                       << QObject::tr("Constraints");
            break;

        case Sql::Constants::DbSchemaObjectType::C_VIEW:
            categories << QObject::tr("Columns");
            break;
    }

    return categories;
}

void IDbLoader::fillSchemaObjectCategories( unsigned schemaObjectType, QStringList &categories ) const
{
    categories << standartSchemaObjectCategories(schemaObjectType) << additionalSchemaObjectCategories(schemaObjectType);
}

QStringList IDbLoader::standartDbCategories() const
{
    return QStringList() << QObject::tr("Schemas");
}

QStringList IDbLoader::additionalDbCategories() const
{
    return QStringList();
}

QStringList IDbLoader::standartSchemaCategories() const
{
    QStringList categories;
    categories << QObject::tr("Tables")
               << QObject::tr("Views")
               << QObject::tr("Indexes")
               << QObject::tr("Triggers")
               << QObject::tr("Constraints");

    return categories;
}

QStringList IDbLoader::additionalSchemaObjectCategories( unsigned schemaObjectType ) const
{
    Q_UNUSED(schemaObjectType)
    return QStringList();
}

MetadataQuery::Ptr MetadataQuery::create( const QString &queryText_, int firstColumnIndex_, int columnCount_, const Properties &properties_ )
{
    return Ptr(new MetadataQuery(queryText_, firstColumnIndex_, columnCount_, properties_));
}

StandartDbQuery::StandartDbQuery( const QSqlDatabase &database )
    : DbQuery()
    , m_query(database)
{
    m_query.setForwardOnly(true);
}

QVariant StandartDbQuery::value( const QString &columnName ) const
{
    return m_query.value(columnName);
}

QString StandartDbQuery::lastErrorText() const
{
    return m_query.lastError().text();
}

int StandartDbQuery::columnCount() const
{
    return m_query.record().count();
}

QString StandartDbQuery::columnName( int columnIndex ) const
{
    return m_query.record().fieldName(columnIndex);
}

int StandartDbQuery::numRowsAffected() const
{
    return m_query.numRowsAffected();
}

QVariant StandartDbQuery::value( int index ) const
{
    return m_query.value(index);
}

bool StandartDbQuery::next()
{
    return m_query.next();
}

bool StandartDbQuery::exec( const QString &queryText, bool logSuccessedQuery, const Bindings &bindings )
{
    bool result = false;

    if ( bindings.isEmpty() )
        result = m_query.exec(queryText);
    else {
        m_query.prepare(queryText);
        for ( Bindings::const_iterator it = bindings.constBegin();
              it != bindings.constEnd(); ++it )
            if ( it.value().type == 0 )
                m_query.bindValue(":" + it.key(), it.value().value.toInt());
            else
                m_query.bindValue(":" + it.key(), it.value().value);
        result = m_query.exec();
    }

    if ( !result )
        Core::LogManager::addMessage( Core::LogMessage( QObject::tr("Query:\n%1\nExecuted with Error:\n%2").arg(queryText).arg(m_query.lastError().text()),
                                                        QObject::tr("Sql.Spy"), Core::LogMessage::Information, QColor(192, 31, 31) ) );
    else if ( logSuccessedQuery )
        Core::LogManager::addMessage( Core::LogMessage( QObject::tr("Query executed:\n%1").arg(queryText),
                                                        QObject::tr("Sql.Spy"), Core::LogMessage::Information, QColor(192, 255, 255) ) );
    return result;
}

StandartDbAccessor::StandartDbAccessor( const QString &dbName )
    : DbAccessor()
    , m_dbName(dbName)
{ }

DbQuery::Ptr StandartDbAccessor::query() const
{
    if ( !m_dbName.isEmpty() ) {
        QSqlDatabase db = QSqlDatabase::database(m_dbName);
        if ( !db.isValid() ) {
            Core::LogManager::addMessage( Core::LogMessage( QObject::tr("Database connection is not valid!"),
                                                            QObject::tr("DbU.Err"), Core::LogMessage::Information, QColor(64, 255, 255) ) );
        } else if ( !db.isOpen() ) {
            Core::LogManager::addMessage( Core::LogMessage( QObject::tr("Database connection is not opened!"),
                                                            QObject::tr("DbU.Err"), Core::LogMessage::Information, QColor(64, 255, 255) ) );
        } else
            return DbQuery::Ptr(new StandartDbQuery(db));
    }

    return DbQuery::Ptr(0);
}

DbAccessor::Ptr BaseConnectionInfo::accessor() const
{
    return DbAccessor::Ptr(new StandartDbAccessor(connectionName()));
}

} // Database
