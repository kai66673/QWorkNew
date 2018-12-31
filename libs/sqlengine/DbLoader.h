#ifndef __DBLOADER_H

#define __DBLOADER_H

#include <QtPlugin>
#include <QString>
#include <QWidget>
#include <QSqlQuery>
#include <QSqlDatabase>

#include "DbFrwd.h"
#include "DbDetails.h"
#include "sqlengine_global.h"
#include "IParserEngine.h"
#include "SqlBindHelper.h"

namespace Database {

class QDbItemAction;

class SQL_EXPORT MetadataQuery
{
public:
    typedef QSharedPointer<MetadataQuery> Ptr;
    typedef QList<QPair<int, QString> > Properties;

private:
    MetadataQuery( const QString &queryText_,
                   int firstColumnIndex_ = 0,
                   int columnCount_ = 1,
                   const Properties &properties_ = Properties() )
        : queryText(queryText_)
        , firstColumnIndex(firstColumnIndex_)
        , columnCount(columnCount_)
        , properties(properties_)
    { }

public:
    QString queryText;
    int firstColumnIndex;
    int columnCount;
    Properties properties;

    static Ptr create(const QString &queryText_,
                      int firstColumnIndex_ = 0,
                      int columnCount_ = 1,
                      const Properties &properties_ = Properties() );
};

struct SQL_EXPORT ColumnInfo {
    ColumnInfo()
        : type("")
        , length(0)
        , description("")
    {}

    QString type;
    int length;
    QString description;
};

class SQL_EXPORT DbQuery
{
public:
    virtual ~DbQuery() {}

    typedef QSharedPointer<DbQuery> Ptr;
    typedef QMap<QString, Sql::SqlBindValue> Bindings;

    virtual bool exec( const QString &queryText, bool logSuccessedQuery = false, const Bindings &bindings = Bindings() ) = 0;
    virtual bool next() = 0;
    virtual QVariant value( int index ) const = 0;
    virtual QVariant value( const QString &columnName ) const = 0;
    virtual QString lastErrorText() const = 0;

    virtual int columnCount() const = 0;
    virtual QString columnName( int columnIndex ) const = 0;
    virtual int numRowsAffected() const = 0;
};

class SQL_EXPORT StandartDbQuery: public DbQuery
{
public:
    StandartDbQuery( const QSqlDatabase &database );

    virtual bool exec( const QString &queryText, bool logSuccessedQuery = false, const Bindings &bindings = Bindings() );
    virtual bool next();
    virtual QVariant value( int index ) const;
    virtual QVariant value( const QString &columnName ) const;
    virtual QString lastErrorText() const;

    virtual int columnCount() const;
    virtual QString columnName( int columnIndex ) const;
    virtual int numRowsAffected() const;

private:
    QSqlQuery m_query;
};

class SQL_EXPORT DbAccessor
{
public:
    virtual ~DbAccessor() {}

    typedef QSharedPointer<DbAccessor> Ptr;
    virtual DbQuery::Ptr query() const = 0;
};

class SQL_EXPORT StandartDbAccessor: public DbAccessor
{
public:
    StandartDbAccessor( const QString &dbName );
    virtual DbQuery::Ptr query() const;

private:
    QString m_dbName;
};

class SQL_EXPORT BaseConnectionInfo
{
public:
    BaseConnectionInfo()
        : m_connectionName("")
        , m_connectionTitle("")
        , m_connectionDescription("")
        , m_defaultSchemaName("")
        , m_caseSensetive(false)
    {}

    BaseConnectionInfo( const QString &connName, const QString &connTitle, const QString &connDescription,
                        const QString &defaultSchemaName = "", bool caseSensetive = false )
        : m_connectionName(connName)
        , m_connectionTitle(connTitle)
        , m_connectionDescription(connDescription)
        , m_defaultSchemaName(defaultSchemaName)
        , m_caseSensetive(caseSensetive)
    {}

    virtual ~BaseConnectionInfo() {}

    inline QString connectionName() const { return m_connectionName; }
    inline QString connectionTitle() const { return m_connectionTitle; }
    inline QString connectionDescription() const { return m_connectionDescription; }
    inline QString defaultSchemaName() const { return m_defaultSchemaName; }
    inline bool caseSensetive() const { return m_caseSensetive; }

    virtual DbAccessor::Ptr accessor() const;

private:
    QString m_connectionName;
    QString m_connectionTitle;
    QString m_connectionDescription;
    QString m_defaultSchemaName;
    bool m_caseSensetive;
};

class SQL_EXPORT IDbConnectWidget: public QWidget
{
public:
    IDbConnectWidget( QWidget *parent = 0 ) : QWidget(parent) {}
    virtual bool checkFillAuthParameters( QStringList &parameters ) = 0;
};

class SQL_EXPORT IDbLoader
{
public:
    virtual QString databaseType() const = 0;
    virtual BaseConnectionInfo *createConnection( const QString &connName, const QStringList &parameters ) const = 0;
    virtual IDbConnectWidget *createAuthWidget( QWidget* parent = 0 ) const = 0;

    enum TopLevel {
        SingleSchema,
        MultiSchema,
        DbMultiSchema
    };

    virtual TopLevel supportedTopLevel() const = 0;

    virtual bool isDdlStatementTransactionSupported() const = 0;

    void fillDbCategories( QStringList &categories ) const;
    void fillSchemaCategories( QStringList &categories ) const;
    void fillSchemaObjectCategories( unsigned schemaObjectType, QStringList &categories ) const;

    virtual MetadataQuery::Ptr dbObjectsMetadataQuery( int /*dbObjectType*/ ) const { return MetadataQuery::Ptr(0); }
    virtual MetadataQuery::Ptr schemasMetadataQuery() const = 0;
    virtual MetadataQuery::Ptr objectsMetadataQuery( const QString &schemaName, int schemaObjectType ) const = 0;
    virtual MetadataQuery::Ptr subobjectsMetadataQuery( const QString &schemaName, int schemaObjectType, const QString &objName, int schemaObjectSubtype ) const = 0;
    virtual MetadataQuery::Ptr objectTableNameMetadataQuery( const QString &schemaName, int schemaObjectType, const QString &objName ) const = 0;

    QStringList standartDbCategories() const;
    virtual QStringList additionalDbCategories() const;
    virtual QString additionalDbCategoryIconName( int /*dbObjectType*/ ) const { return QStringLiteral(""); }
    virtual QString additionalDbObjectIconName( int /*dbObjectType*/ ) const { return QStringLiteral(""); }

    QStringList standartSchemaCategories() const;
    virtual QStringList additionalSchemaCategories() const;
    virtual QString additionalSchemaCategoryIconName( int /*schemaObjectType*/ ) const { return QStringLiteral(""); }
    virtual QString additionalSchemaObjectIconName( int /*schemaObjectType*/, ItemProperties * /*properties*/ ) const { return QStringLiteral(""); }

    QStringList standartSchemaObjectCategories( unsigned schemaObjectType ) const;
    virtual QStringList additionalSchemaObjectCategories( unsigned schemaObjectType ) const;
    virtual QString additionalSchemaObjectCategoryIconName( int /*schemaObjectType*/, int /*schemaObjectSubtype*/ ) const { return QStringLiteral(""); }
    virtual QString additionalSchemaSubobjectIconName( int /*schemaObjectType*/, int /*schemaObjectSubtype*/ ) const { return QStringLiteral(""); }

    virtual IDbDetailsFactory *createDbDetailsFactory( const QString &connName,
                                                       const QString &schemaName,
                                                       int schemaObjectType,
                                                       int schemaObjectSubtype,
                                                       const QString &objName ) const = 0;

    virtual QList<QAction *> actionsForItem( IDbCatalogItem * /*item*/ ) const { return QList<QAction *>(); }
    virtual QList<QDbItemAction *> allActions() const { return QList<QDbItemAction *>(); }

    virtual Sql::IParserEngine *parserEngine() { return 0; }
};

} // Database

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(Database::IDbLoader, "QWorkBase.IDbLoader/1.0")
QT_END_NAMESPACE

#endif // __DBLOADER_H
