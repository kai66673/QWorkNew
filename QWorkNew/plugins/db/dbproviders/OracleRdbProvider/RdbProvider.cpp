#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QAction>
#include <QFileInfo>
#include <QDebug>

#include "Core.h"
#include "IDocument.h"
#include "RdbProvider.h"
#include "RdbParserEngine.h"
#include "RdbAuthWidget.h"
#include "DbUtils.h"
#include "DbDetails.h"
#include "sql_constants.h"
#include "rdb_sql_constants.h"
#include "SQRDB.H"

// Requires conditional expression constraint
#define RDB$K_CON_CONDITION     1
// Primary key constraint
#define RDB$K_CON_PRIMARY_KEY   2
// Referential (foreign key) constraint
#define RDB$K_CON_REFERENTIAL   3
// Unique constraint
#define RDB$K_CON_UNIQUE        4
// Not null (missing) constraint
#define RDB$K_CON_NOT_NULL      6

#if defined (Q_OS_WIN32)
#include <QtCore/qt_windows.h>
#endif

#include <sql.h>
#include <sqlucode.h>
#include <sqlext.h>

#if defined(Q_ODBC_VERSION_2)
//crude hack to get non-unicode capable driver managers to work
# undef UNICODE
# define SQLTCHAR SQLCHAR
# define SQL_C_WCHAR SQL_C_CHAR
#endif

// newer platform SDKs use SQLLEN instead of SQLINTEGER
#if defined(WIN32) && (_MSC_VER < 1300)
# define QSQLLEN SQLINTEGER
# define QSQLULEN SQLUINTEGER
#else
# define QSQLLEN SQLLEN
# define QSQLULEN SQLULEN
#endif

#ifdef UNICODE
# define QSQLSTR(queryString) (SQLWCHAR*)queryString.unicode()
#else
# define QSQLSTR(queryString) (SQLCHAR*) queryString.toLatin1().constData()
#endif

namespace Database {
namespace RDB {

struct IndexExtParamteters {
    IndexExtParamteters( long nodeSize_ = 430, const QString &typeSortedRanked_ = "SORTED", const QString &keySuffixCompression_ = "DISABLED" )
        : nodeSize(nodeSize_)
        , typeSortedRanked(typeSortedRanked_)
        , keySuffixCompression(keySuffixCompression_)
    {}

    long nodeSize;
    QString typeSortedRanked;
    QString keySuffixCompression;

};

static void parseIndexExtensionParameters( const QByteArray &baParameters, IndexExtParamteters &parameters )
{
    int  index;
    int  valSize;
    long lValue;

    index = 0;
    while (1) {
        if ( baParameters.length() <= index )
            break;

        valSize = baParameters[index + 2] + baParameters[index + 3] * 256;
        if ( valSize < 1 || valSize > 8)
            break;
        lValue = 0L;
        for ( int i = 0; i < valSize; i ++) {
            long lTmp = baParameters[index + 3 + valSize - i];
            if ( lTmp < 0 ) lTmp += 256L;
            lValue = (256L * lValue) + lTmp;
        }
        if ( baParameters[index + 0] == 0x01 && baParameters[index + 1] == 0x01 ) { // Node Size
            parameters.nodeSize = lValue;
        }
        else if ( baParameters[index + 0] == 0x01 && baParameters[index + 1] == 0x1e ) { // sorted | sorted ranked
            if ( lValue )
                parameters.typeSortedRanked = "SORTED RANKED";
        }
        else if ( baParameters[index + 0] == 0x01 && baParameters[index + 1] == 0x04 ) { // key suffix compression
            if ( lValue )
                parameters.keySuffixCompression = "ENABLED";
        }
        index += 4 + valSize;
    }
}

static QString qWarnODBCHandle(int handleType, SQLHANDLE handle, int *nativeCode = 0)
{
    SQLINTEGER nativeCode_ = 0;
    SQLSMALLINT msgLen = 0;
    SQLRETURN r = SQL_NO_DATA;
    SQLTCHAR state_[SQL_SQLSTATE_SIZE+1];
    SQLTCHAR description_[SQL_MAX_MESSAGE_LENGTH];
    QString result;
    int i = 1;

    description_[0] = 0;
    do {
        r = SQLGetDiagRec(handleType,
                            handle,
                            i,
                            (SQLTCHAR*)state_,
                            &nativeCode_,
                            (SQLTCHAR*)description_,
                            SQL_MAX_MESSAGE_LENGTH, /* in bytes, not in characters */
                            &msgLen);
        if (r == SQL_SUCCESS || r == SQL_SUCCESS_WITH_INFO) {
            if (nativeCode)
                *nativeCode = nativeCode_;
            QString tmpstore;
#ifdef UNICODE
            tmpstore = QString((const QChar*)description_, msgLen);
#else
            tmpstore = QString::fromLocal8Bit((const char*)description_, msgLen);
#endif
            if(result != tmpstore) {
                if(!result.isEmpty())
                    result += QLatin1Char(' ');
                result += tmpstore;
            }
        } else if (r == SQL_ERROR || r == SQL_INVALID_HANDLE) {
            return result;
        }
        ++i;
    } while (r != SQL_NO_DATA);
    return result;
}

static bool readBlobSegmentedList( const QString &connName, const QString &queryString, const QString &fieldName,
                                   QStringList &list, QTextCodec *codec, const QString &cursorSuffixName = QString() )
{
    QSqlDatabase db = QSqlDatabase::database(connName);

    if ( !db.isOpen() ) {
        Core::Storage::warning(QObject::tr("Database connection is not opened."));
        return false;
    }

    QVariant var = db.driver()->handle();
    SQLHANDLE hdbc = *static_cast<SQLHANDLE *>(var.data());

    if ( !hdbc ) {
        Core::Storage::warning(QObject::tr("Cannot get DB connection handle."));
        return false;
    }

    SQLHSTMT hstmtSQ;
    SQLHSTMT hstmtSL;

    SQLCHAR     szName[50];
    SQLLEN      cbName;
    SQLRETURN   retcode;

    SQLCHAR     buffer[512];
    SQLLEN      cbBuffer;

    SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmtSQ);
    SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmtSL);

    static qlonglong nCur = 0;

    QString cn = "CUR_TR_" + QString::number(nCur);
    if ( !cursorSuffixName.isEmpty() )
        cn.append(cursorSuffixName);
    nCur++;
    SQLSetCursorName(hstmtSQ, QSQLSTR(cn), SQL_NTS);

    retcode = SQLExecDirect(hstmtSQ, QSQLSTR(queryString), SQL_NTS);
    if ( retcode != SQL_SUCCESS )
        qDebug() << "LOB EXTR 01 " << qWarnODBCHandle(SQL_HANDLE_STMT, hstmtSQ);

    retcode = SQLBindCol(hstmtSQ, 1, SQL_C_CHAR, szName, 50, &cbName);
    if ( retcode != SQL_SUCCESS )
        qDebug() << "LOB EXTR 02 "  << qWarnODBCHandle(SQL_HANDLE_STMT, hstmtSQ);

    retcode = SQLFetch(hstmtSQ);
    if ( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO ) {

        retcode = SQLSetStmtOption(hstmtSL, SQRDB_CURSOR_TYPE, SQRDB_CURSOR_READ);
        retcode = SQLSetStmtOption(hstmtSL, SQRDB_LIST_LEN, 512);
        QString lq = "SELECT " + fieldName + " WHERE CURRENT OF " + cn;
        retcode = SQLExecDirect(hstmtSL, QSQLSTR(lq), SQL_NTS);
        if ( retcode != SQL_SUCCESS )
            qDebug() << "LOB EXTR 03 "  << qWarnODBCHandle(SQL_HANDLE_STMT, hstmtSL);
        retcode = SQLBindCol(hstmtSL, 1, SQL_C_BINARY, buffer, 512, &cbBuffer);
        if ( retcode != SQL_SUCCESS )
            qDebug() << "LOB EXTR 04 "  << qWarnODBCHandle(SQL_HANDLE_STMT, hstmtSL);
        while (1) {
            retcode = SQLFetch(hstmtSL);
            if ( retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO ) {
                list << codec->toUnicode((char *)buffer, cbBuffer);
            }
            else {
                if ( retcode != SQL_NO_DATA ) {
                    qDebug() << "LOB EXTR 05 "  << qWarnODBCHandle(SQL_HANDLE_STMT, hstmtSL);
                }
                else {
                    retcode = SQL_SUCCESS;
                }
                break;
            }
        }
    }
    else {
        Core::Storage::warning(QObject::tr("Fetching Segmented String Failed."));
        qDebug() << "LOB EXTR 02 "  << qWarnODBCHandle(SQL_HANDLE_STMT, hstmtSQ);
    }

    SQLCloseCursor(hstmtSL);
    SQLCloseCursor(hstmtSQ);
    SQLFreeHandle(SQL_HANDLE_STMT, hstmtSQ);
    SQLFreeHandle(SQL_HANDLE_STMT, hstmtSL);

    return (retcode == SQL_SUCCESS);
}

Provider::Provider( QObject *parent )
    : QObject(parent)
    , m_engine(new ::RDB::RdbParserEngine())
{
    m_codec = QTextCodec::codecForName("Windows-1251");

    m_actCreateDynamycSelect = new QDbItemAction(tr("Create Dynamic Query"), this);
    connect(m_actCreateDynamycSelect, SIGNAL(itemTriggered(QString,IDbCatalogItem*)), this, SLOT(onCreateCreateDynamycSelect(QString,IDbCatalogItem*)));

    m_actCreateCStructure = new QDbItemAction(tr("Create C Structure"), this);
    connect(m_actCreateCStructure, SIGNAL(itemTriggered(QString,IDbCatalogItem*)), this, SLOT(onCreateCStructure(QString,IDbCatalogItem*)));
}

Provider::~Provider()
{
    delete m_engine;
}

BaseConnectionInfo *Provider::createConnection( const QString &connName, const QStringList &parameters ) const
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", connName);
    db.setDatabaseName(parameters[0]);
    db.setUserName(parameters[1]);
    db.setPassword(parameters[2]);
    db.setConnectOptions("SQL_ATTR_CODEC=Windows-1251");

    if ( !db.open() ) {
        qDebug() << db.lastError().text();
        QMessageBox::critical( Core::Storage::mainWindow()->widget(),
                               tr("Cannot open database"),
                               tr("Unable to establish a database connection.\nClick Cancel to exit."),
                               QMessageBox::Cancel );
        return 0;
    }

    QString title = parameters[0] + "(" + parameters[1] + ")";

    QString descr;
    descr  = "Connection type: Oracle RDB ([Q]ODBC)\n";
    descr += "Database source: " + parameters[0] + "\n";
    descr += "      User name: " + parameters[1];

    return new BaseConnectionInfo(connName, title, descr);
}

IDbConnectWidget *Provider::createAuthWidget( QWidget *parent ) const
{
    return new RdbAuthWidget(parent);
}

MetadataQuery::Ptr Provider::schemasMetadataQuery() const
{
    return MetadataQuery::Ptr(0);
}

MetadataQuery::Ptr Provider::objectsMetadataQuery( const QString &schemaName, int schemaObjectType ) const
{
    Q_UNUSED(schemaName)

    switch ( schemaObjectType ) {

        case Sql::Constants::DbSchemaObjectType::C_TABLE:
            return MetadataQuery::create("SELECT TRIM(C1.RDB$RELATION_NAME) FROM RDB$RELATIONS C1 WHERE RDB$SYSTEM_FLAG = 0 AND RDB$VIEW_SOURCE IS NULL ORDER BY 1");

        case Sql::Constants::DbSchemaObjectType::C_VIEW:
            return MetadataQuery::create("SELECT TRIM(C1.RDB$RELATION_NAME) FROM RDB$RELATIONS C1 WHERE RDB$SYSTEM_FLAG = 0 AND RDB$VIEW_SOURCE IS NOT NULL ORDER BY 1");

        case Sql::Constants::DbSchemaObjectType::C_INDEX:
            return MetadataQuery::create("SELECT TRIM(RDB$INDEX_NAME), TRIM(RDB$RELATION_NAME) FROM RDB$INDICES ORDER BY 1", 0, 2);

        case Sql::Constants::DbSchemaObjectType::C_TRIGGER:
            return MetadataQuery::create("SELECT TRIM(RDB$TRIGGER_NAME), TRIM(RDB$RELATION_NAME) FROM RDB$TRIGGERS ORDER BY 1", 0, 2);

        case Sql::Constants::DbSchemaObjectType::C_CONSTRAINT:
            return MetadataQuery::create("SELECT TRIM(RDB$CONSTRAINT_NAME), TRIM(RDB$RELATION_NAME) FROM RDB$RELATION_CONSTRAINTS ORDER BY 1", 0, 2);

        case Sql::Constants::DbSchemaObjectType::C_MODULE:
            return MetadataQuery::create("SELECT TRIM(RDB$MODULE_NAME) FROM RDB$MODULES ORDER BY RDB$MODULE_NAME");

        case Sql::Constants::DbSchemaObjectType::C_FUNCTION:
            return MetadataQuery::create("SELECT TRIM(RDB$ROUTINE_NAME) FROM RDB$ROUTINES WHERE BITSTRING(RDB$FLAGS FROM 0 FOR 1) = 1 ORDER BY RDB$ROUTINE_NAME");

        case Sql::Constants::DbSchemaObjectType::C_PROCEDURE:
            return MetadataQuery::create("SELECT TRIM(RDB$ROUTINE_NAME) FROM RDB$ROUTINES WHERE BITSTRING(RDB$FLAGS FROM 0 FOR 1) = 0 ORDER BY RDB$ROUTINE_NAME");

        case Sql::Constants::DbSchemaObjectType::C_AREA:
            return MetadataQuery::create("SELECT DISTINCT TRIM(RDB$AREA_NAME) FROM RDB$STORAGE_MAP_AREAS ORDER BY 1");
    }

    return MetadataQuery::Ptr(0);
}

MetadataQuery::Ptr Provider::subobjectsMetadataQuery( const QString &schemaName, int schemaObjectType, const QString &objName, int schemaObjectSubtype ) const
{
    Q_UNUSED(schemaName)

    switch ( schemaObjectType ) {

        case Sql::Constants::DbSchemaObjectType::C_TABLE:
            switch ( schemaObjectSubtype ) {
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_COLUMN:
                    return MetadataQuery::create( " SELECT TRIM(RF.RDB$FIELD_NAME), F.RDB$FIELD_TYPE FROM RDB$RELATIONS R, RDB$RELATION_FIELDS RF,RDB$FIELDS F WHERE R.RDB$RELATION_NAME = \'" + objName.toUpper() +
                                                  "\' AND RF.RDB$RELATION_NAME = R.RDB$RELATION_NAME AND RF.RDB$FIELD_SOURCE = F.RDB$FIELD_NAME AND R.RDB$SYSTEM_FLAG = 0 ORDER BY RF.RDB$FIELD_POSITION ASC");
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_INDEX:
                    return MetadataQuery::create("SELECT TRIM(RDB$INDEX_NAME) FROM RDB$INDICES WHERE RDB$RELATION_NAME = \'" + objName.toUpper() + "\' ORDER BY 1");
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_TRIGGER:
                    return MetadataQuery::create("SELECT TRIM(RDB$TRIGGER_NAME) FROM RDB$TRIGGERS WHERE RDB$RELATION_NAME = \'" + objName.toUpper() + "\' ORDER BY 1");
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_CONSTRAINT:
                    return MetadataQuery::create("SELECT TRIM(RDB$CONSTRAINT_NAME) FROM RDB$RELATION_CONSTRAINTS WHERE RDB$RELATION_NAME = \'" + objName.toUpper() + "\' ORDER BY 1");
                default:
                    return MetadataQuery::Ptr(0);
            }

        case Sql::Constants::DbSchemaObjectType::C_VIEW:
            switch ( schemaObjectSubtype ) {
                case Sql::Constants::DbSchemaObjectSubtype::C_VIEW_COLUMN:
                    return MetadataQuery::create( " SELECT TRIM(RF.RDB$FIELD_NAME), F.RDB$FIELD_TYPE FROM RDB$RELATIONS R, RDB$RELATION_FIELDS RF,RDB$FIELDS F WHERE R.RDB$RELATION_NAME = \'" + objName.toUpper() +
                                                  "\' AND RF.RDB$RELATION_NAME = R.RDB$RELATION_NAME AND RF.RDB$FIELD_SOURCE = F.RDB$FIELD_NAME AND R.RDB$SYSTEM_FLAG = 0 ORDER BY RF.RDB$FIELD_POSITION ASC" );
                default:
                    return MetadataQuery::Ptr(0);
            }

        case Sql::Constants::DbSchemaObjectType::C_MODULE:
            switch ( schemaObjectSubtype ) {
                case Sql::Constants::DbSchemaObjectSubtype::C_MODULE_FUNCTION:
                    return MetadataQuery::create("SELECT R.RDB$ROUTINE_NAME, M.RDB$MODULE_NAME FROM RDB$ROUTINES R, RDB$MODULES M WHERE R.RDB$MODULE_ID = M.RDB$MODULE_ID AND M.RDB$MODULE_NAME = \'" + objName.toUpper() + "\' AND BITSTRING(R.RDB$FLAGS FROM 0 FOR 1) = 1 ORDER BY R.RDB$ROUTINE_NAME", 0, 2);
                case Sql::Constants::DbSchemaObjectSubtype::C_MODULE_PROCEDURE:
                    return MetadataQuery::create("SELECT R.RDB$ROUTINE_NAME, M.RDB$MODULE_NAME FROM RDB$ROUTINES R, RDB$MODULES M WHERE R.RDB$MODULE_ID = M.RDB$MODULE_ID AND M.RDB$MODULE_NAME = \'" + objName.toUpper() + "\' AND BITSTRING(R.RDB$FLAGS FROM 0 FOR 1) = 0 ORDER BY R.RDB$ROUTINE_NAME", 0, 2);
            }
    }

    return MetadataQuery::Ptr(0);
}

MetadataQuery::Ptr Provider::objectTableNameMetadataQuery( const QString &schemaName, int schemaObjectType, const QString &objName ) const
{
    Q_UNUSED(schemaName)

    switch ( schemaObjectType ) {

        case Sql::Constants::DbSchemaObjectType::C_INDEX:
            return MetadataQuery::create("SELECT TRIM(RDB$RELATION_NAME) FROM RDB$INDICES WHERE RDB$INDEX_NAME = \'" + objName.toUpper() + "\'");

        case Sql::Constants::DbSchemaObjectType::C_TRIGGER:
            return MetadataQuery::create("SELECT TRIM(RDB$RELATION_NAME) FROM RDB$TRIGGERS WHERE RDB$TRIGGER_NAME = \'" + objName.toUpper() + "\'");

        case Sql::Constants::DbSchemaObjectType::C_CONSTRAINT:
            return MetadataQuery::create("SELECT TRIM(RDB$RELATION_NAME) FROM RDB$RELATION_CONSTRAINTS WHERE RDB$CONSTRAINT_NAME = \'" + objName.toUpper() + "\'");
    }

    return MetadataQuery::Ptr(0);
}

QStringList Provider::additionalSchemaCategories() const
{
    return QStringList() << QObject::tr("Modules")
                         << QObject::tr("Functions")
                         << QObject::tr("Procedures")
                         << QObject::tr("Areas");
}

QStringList Provider::additionalSchemaObjectCategories( unsigned schemaObjectType ) const
{
    if ( schemaObjectType == Sql::Constants::DbSchemaObjectType::C_MODULE )
        return QStringList() << QObject::tr("Functions") << QObject::tr("Procedures");
    return QStringList();
}

QString Provider::additionalSchemaCategoryIconName( int schemaObjectType ) const
{
    switch ( schemaObjectType ) {
        case Sql::Constants::DbSchemaObjectType::C_MODULE:          return QStringLiteral(":/images/dbb_modules.png");
        case Sql::Constants::DbSchemaObjectType::C_FUNCTION:        return QStringLiteral(":/images/dbb_funcproc_list.png");
        case Sql::Constants::DbSchemaObjectType::C_PROCEDURE:       return QStringLiteral(":/images/dbb_funcproc_list.png");
        case Sql::Constants::DbSchemaObjectType::C_AREA:            return QStringLiteral(":/images/dbb_storages.png");
    }

    return QStringLiteral("");
}

QString Provider::additionalSchemaObjectIconName( int schemaObjectType , ItemProperties * ) const
{
    switch ( schemaObjectType ) {
        case Sql::Constants::DbSchemaObjectType::C_MODULE:          return QStringLiteral(":/images/dbb_module.png");
        case Sql::Constants::DbSchemaObjectType::C_FUNCTION:        return QStringLiteral(":/images/dbb_function.png");
        case Sql::Constants::DbSchemaObjectType::C_PROCEDURE:       return QStringLiteral(":/images/dbb_procedure.png");
        case Sql::Constants::DbSchemaObjectType::C_AREA:            return QStringLiteral(":/images/dbb_storage.png");
    }

    return QStringLiteral("");
}

QString Provider::additionalSchemaObjectCategoryIconName (int schemaObjectType, int schemaObjectSubtype ) const
{
    if ( schemaObjectType == Sql::Constants::DbSchemaObjectType::C_MODULE ) {
        switch ( schemaObjectSubtype ) {
            case Sql::Constants::DbSchemaObjectSubtype::C_MODULE_FUNCTION:  return QStringLiteral(":/images/dbb_funcproc_list.png");
            case Sql::Constants::DbSchemaObjectSubtype::C_MODULE_PROCEDURE: return QStringLiteral(":/images/dbb_funcproc_list.png");
        }
    }

    return QStringLiteral("");
}

QString Provider::additionalSchemaSubobjectIconName( int schemaObjectType, int schemaObjectSubtype ) const
{
    if ( schemaObjectType == Sql::Constants::DbSchemaObjectType::C_MODULE ) {
        switch ( schemaObjectSubtype ) {
            case Sql::Constants::DbSchemaObjectSubtype::C_MODULE_FUNCTION:  return QStringLiteral(":/images/dbb_function.png");
            case Sql::Constants::DbSchemaObjectSubtype::C_MODULE_PROCEDURE: return QStringLiteral(":/images/dbb_procedure.png");
        }
    }

    return QStringLiteral("");
}

static IDbDetailsFactory *createTableDbDetailsFactory( const QString &connName, const QString &tableName, QTextCodec *codec )
{
    QSqlDatabase db = QSqlDatabase::database(connName);

    if ( !db.isOpen() ) {
        Core::Storage::warning(QObject::tr("Database connection is not opened."));
        return 0;
    }

    // Table Info
    Database::DbDetailsFormViewFactory *t = new Database::DbDetailsFormViewFactory;
    {
        t->addItem(Database::DbDetailsFormViewFactory::FormItem(QObject::tr("Name"), tableName));
        QSqlQuery query(db);
        QString   queryString = "SELECT RDB$DESCRIPTION FROM RDB$RELATIONS WHERE RDB$RELATION_NAME = :tableName";
        query.prepare(queryString);
        query.bindValue(":tableName", tableName.toUpper());
        QString remark = "";
        if ( Database::Utils::executeQueryWithLog(&query) )
            if ( query.next() )
                remark = codec->toUnicode(query.value(0).toByteArray());
        t->addItem(Database::DbDetailsFormViewFactory::FormItem(QObject::tr("description"), remark));
    }

    // Table Columns Info
    Database::DbDetailsTableFactory *c = new Database::DbDetailsTableFactory( QStringList() << QObject::tr("Name")
                                                                                            << QObject::tr("Type")
                                                                                            << QObject::tr("Length")
                                                                                            << QObject::tr("Description") );
    {
        QSqlQuery query(db);
        QString   queryString;
        int       fieldType;
        queryString = " SELECT TRIM(RF.RDB$FIELD_NAME), F.RDB$FIELD_TYPE, F.RDB$FIELD_LENGTH, RF.RDB$DESCRIPTION"
                      " FROM RDB$RELATIONS R, RDB$RELATION_FIELDS RF,RDB$FIELDS F"
                      " WHERE R.RDB$RELATION_NAME = :tableName AND"
                      "   RF.RDB$RELATION_NAME = R.RDB$RELATION_NAME AND"
                      "   RF.RDB$FIELD_SOURCE = F.RDB$FIELD_NAME AND"
                      "   R.RDB$SYSTEM_FLAG = 0"
                      "ORDER BY RF.RDB$FIELD_POSITION ASC";
        query.prepare(queryString);
        query.bindValue(":tableName", tableName.toUpper());
        if ( Database::Utils::executeQueryWithLog(&query) )
            while ( query.next() ) {
                QStringList row;
                row << query.value(0).toString();
                fieldType = query.value(1).toInt();
                switch (fieldType) {
                    case 7:     row << "short (SMALLINT)"; break;
                    case 8:     row << "long (INTEGER)";   break;
                    case 14:    row << "char (CHAR)";      break;
                    case 27:    row << "double (REAL)";    break;
                    case 37:    row << "char (VARCHAR)";   break;
                    case 261:   row << "string (LOB)";     break;
                    default:    row << "<<UNKNOWN>>";      break;
                }
                row << query.value(2).toString();
                row << codec->toUnicode(query.value(3).toByteArray());
                c->addRow(row);
            }
    }

    // Table Indexes Info
    Database::DbDetailsTableWithTableFactory *in = new Database::DbDetailsTableWithTableFactory( QStringList() << QObject::tr("Name")
                                                                                                               << QObject::tr("Unique")
                                                                                                               << QObject::tr("Type")
                                                                                                               << QObject::tr("Key suffix compression")
                                                                                                               << QObject::tr("Node size")
                                                                                                               << QObject::tr("Storage Area"),
                                                                                                 QStringList() << QObject::tr("Column")
                                                                                                               << QObject::tr("Order") );
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = "SELECT I.RDB$INDEX_NAME, I.RDB$UNIQUE_FLAG, I.RDB$EXTENSION_PARAMETERS, M.RDB$AREA_NAME "
                      "  FROM RDB$INDICES I LEFT JOIN RDB$STORAGE_MAP_AREAS M ON I.RDB$INDEX_ID = M.RDB$INDEX_ID "
                      " WHERE RDB$RELATION_NAME = :tableName ORDER BY RDB$INDEX_NAME";
        query.prepare(queryString);
        query.bindValue(":tableName", tableName.toUpper());
        if ( Database::Utils::executeQueryWithLog(&query) ) {
            QList<QStringList> data;
            while ( query.next() ) {
                QStringList item;
                item << query.value(0).toString().trimmed();
                item << (query.value(1).toInt() == 0 ? QObject::tr("No") : QObject::tr("Yes"));
                QByteArray ba = query.value(2).toByteArray();
                IndexExtParamteters par;
                parseIndexExtensionParameters(ba, par);
                item << par.typeSortedRanked << par.keySuffixCompression << QString::number(par.nodeSize);
                item << query.value(3).toString();
                data << item;
            }
            for (int i = 0; i < data.size(); i++) {
                QList<QStringList> indColumns;
                queryString = "SELECT RDB$FIELD_NAME, RDB$FLAGS FROM RDB$INDEX_SEGMENTS WHERE RDB$INDEX_NAME = :indexName ORDER BY RDB$FIELD_POSITION;";
                query.prepare(queryString);
                query.bindValue(":indexName", data[i][0].toUpper());
                query.exec();
                while ( query.next() ) {
                    QStringList item;
                    item << query.value(0).toString();
                    item << (query.value(1).toInt() == 0 ? QObject::tr("Asc") : QObject::tr("Desc"));
                    indColumns << item;
                }
                in->addRow(data[i], indColumns);
            }
        }
    }

    // Table Triggers Info
    Database::DbDetailsTableWithSourceFactory *tr = new Database::DbDetailsTableWithSourceFactory( QStringList() << QObject::tr("Name")
                                                                                                                 << QObject::tr("Type") );
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = "SELECT RDB$TRIGGER_NAME, RDB$TRIGGER_TYPE FROM RDB$TRIGGERS WHERE RDB$RELATION_NAME = :tableName ORDER BY RDB$TRIGGER_NAME";
        query.prepare(queryString);
        query.bindValue(":tableName", tableName.toUpper());
        if ( Database::Utils::executeQueryWithLog(&query) ) {
            QList<QStringList> data;
            while ( query.next() ) {
                QStringList item;
                item << query.value(0).toString().trimmed();
                int triggerType = query.value(1).toInt();
                switch ( triggerType ) {
                    case 1: item << "before insert";    break;
                    case 2: item << "before delete";    break;
                    case 3: item << "before update";    break;
                    case 4: item << "after insert";     break;
                    case 5: item << "after delete";     break;
                    case 6: item << "after update";     break;
                    default: item << "<UNKNOWN>";       break;
                }
                data << item;
            }
            for (int i = 0; i < data.size(); i++) {
                queryString = "SELECT RDBVMS$TRIGGER_SOURCE FROM RDBVMS$TRIGGERS WHERE RDBVMS$TRIGGER_NAME = \'" + data[i][0].toUpper() + "\'";
                QString fieldTable = "RDBVMS$TRIGGER_SOURCE";

                QStringList strList;
                QString triggerSource = "";
                if ( readBlobSegmentedList(connName, queryString, fieldTable, strList, codec, tableName.toUpper()) )
                    triggerSource = strList.join("\n");
                tr->addRow(data[i], triggerSource);
            }
        }
    }

    // Table Constraints Info
    Database::DbDetailsTableWithSourceFactory *cn = new Database::DbDetailsTableWithSourceFactory( QStringList() << QObject::tr("Name")
                                                                                                                 << QObject::tr("Type") );
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = "SELECT RDB$CONSTRAINT_NAME, RDB$CONSTRAINT_TYPE FROM RDB$RELATION_CONSTRAINTS WHERE RDB$RELATION_NAME = :tableName ORDER BY RDB$CONSTRAINT_NAME";
        query.prepare(queryString);
        query.bindValue(":tableName", tableName.toUpper());
        if ( Database::Utils::executeQueryWithLog(&query) ) {
            QList<QStringList> data;
            while ( query.next() ) {
                QStringList item;
                item << query.value(0).toString().trimmed();
                int constraintType = query.value(1).toInt();
                switch ( constraintType ) {
                    case RDB$K_CON_CONDITION:   item << QObject::tr("Conditional");      break;
                    case RDB$K_CON_PRIMARY_KEY: item << QObject::tr("Primary Key");      break;
                    case RDB$K_CON_REFERENTIAL: item << QObject::tr("Referential");      break;
                    case RDB$K_CON_UNIQUE:      item << QObject::tr("Unique");           break;
                    case RDB$K_CON_NOT_NULL:    item << QObject::tr("Not Null");         break;
                    default: item << "";
                }
                data << item;
            }
            for (int i = 0; i < data.size(); i++) {
                queryString = "SELECT RDB$CONSTRAINT_SOURCE FROM RDB$RELATION_CONSTRAINTS WHERE RDB$CONSTRAINT_NAME = \'" + data[i][0].toUpper() + "\'";
                QString fieldTable = "RDB$CONSTRAINT_SOURCE";
                QString constraintSource = "";
                QStringList strList;
                if ( readBlobSegmentedList(connName, queryString, fieldTable, strList, codec, tableName.toUpper()) )
                    constraintSource = strList.join("\n");
                cn->addRow(data[i], constraintSource);
            }
        }
    }

    Database::DbDetailsTabbedFactory *tab = new Database::DbDetailsTabbedFactory;
    tab->addTab(QObject::tr("Table Info"), t);
    tab->addTab(QObject::tr("Columns"), c);
    tab->addTab(QObject::tr("Indexes"), in);
    tab->addTab(QObject::tr("Triggers"), tr);
    tab->addTab(QObject::tr("Constraints"), cn);

    return tab;
}

static bool getCreateViewQuery(const QString &connName, const QString &viewName, QString &viewSource, QTextCodec *codec )
{
    viewSource = "";

    QSqlDatabase db = QSqlDatabase::database(connName);

    if ( !db.isOpen() ) {
        Core::Storage::warning(QObject::tr("Database connection is not opened."));
        return false;
    }

    QString queryString = "SELECT RDB$VIEW_SOURCE FROM RDB$RELATIONS WHERE RDB$RELATION_NAME = \'" + viewName + "\'";
    QString fieldTable = "RDB$VIEW_SOURCE";

    QStringList strList;
    if ( readBlobSegmentedList(connName, queryString, fieldTable, strList, codec) )
        viewSource = strList.join("\n");

    return true;
}

static IDbDetailsFactory *createViewDbDetailsFactory( const QString &connName, const QString &viewName, QTextCodec *codec )
{
    QSqlDatabase db = QSqlDatabase::database(connName);

    if ( !db.isOpen() ) {
        Core::Storage::warning(QObject::tr("Database connection is not opened."));
        return 0;
    }

    // View Info
    Database::DbDetailsFormViewFactory *t = new Database::DbDetailsFormViewFactory;
    {
        t->addItem(Database::DbDetailsFormViewFactory::FormItem(QObject::tr("Name"), viewName));
        QSqlQuery query(db);
        QString   queryString = "SELECT RDB$DESCRIPTION FROM RDB$RELATIONS WHERE RDB$RELATION_NAME = :tableName";
        query.prepare(queryString);
        query.bindValue(":tableName", viewName.toUpper());
        QString remark = "";
        if ( Database::Utils::executeQueryWithLog(&query) )
            if ( query.next() )
                remark = query.value(0).toString();
        t->addItem(Database::DbDetailsFormViewFactory::FormItem(QObject::tr("description"), remark));
        QString source;
        getCreateViewQuery(connName, viewName.toUpper(), source, codec);
        t->addItem(Database::DbDetailsFormViewFactory::FormItem( QObject::tr("Source"), source, Database::DbDetailsFormViewFactory::SourceText ));
    }

    // Table Columns Info
    Database::DbDetailsTableFactory *c = new Database::DbDetailsTableFactory( QStringList() << QObject::tr("Name")
                                                                                            << QObject::tr("Type")
                                                                                            << QObject::tr("Length")
                                                                                            << QObject::tr("Description") );
    {
        QSqlQuery query(db);
        QString   queryString;
        int       fieldType;
        queryString = " SELECT TRIM(RF.RDB$FIELD_NAME), F.RDB$FIELD_TYPE, F.RDB$FIELD_LENGTH, RF.RDB$DESCRIPTION"
                      " FROM RDB$RELATIONS R, RDB$RELATION_FIELDS RF,RDB$FIELDS F"
                      " WHERE R.RDB$RELATION_NAME = :tableName AND"
                      "   RF.RDB$RELATION_NAME = R.RDB$RELATION_NAME AND"
                      "   RF.RDB$FIELD_SOURCE = F.RDB$FIELD_NAME AND"
                      "   R.RDB$SYSTEM_FLAG = 0"
                      "ORDER BY RF.RDB$FIELD_POSITION ASC";
        query.prepare(queryString);
        query.bindValue(":tableName", viewName.toUpper());
        if ( Database::Utils::executeQueryWithLog(&query) )
            while ( query.next() ) {
                QStringList row;
                row << query.value(0).toString();
                fieldType = query.value(1).toInt();
                switch (fieldType) {
                    case 7:     row << "short (SMALLINT)"; break;
                    case 8:     row << "long (INTEGER)";   break;
                    case 14:    row << "char (CHAR)";      break;
                    case 27:    row << "double (REAL)";    break;
                    case 37:    row << "char (VARCHAR)";   break;
                    case 261:   row << "string (LOB)";     break;
                    default:    row << "<<UNKNOWN>>";      break;
                }
                row << query.value(2).toString();
                row << query.value(3).toString();
                c->addRow(row);
            }
    }

    Database::DbDetailsTabbedFactory *tab = new Database::DbDetailsTabbedFactory;
    tab->addTab(QObject::tr("View Info"), t);
    tab->addTab(QObject::tr("Columns Info"), c);

    return tab;
}

static void getFuncProcSource( const QString &connName, const QString &funcName, QString &funcSource, QTextCodec *codec )
{
    funcSource = "";

    QString queryString = "SELECT RDB$ROUTINE_SOURCE FROM RDB$ROUTINES WHERE RDB$ROUTINE_NAME = \'" + funcName + "\'";
    QString fieldTable = "RDB$ROUTINE_SOURCE";

    QStringList strList;
    if ( readBlobSegmentedList(connName, queryString, fieldTable, strList, codec) )
        funcSource = strList.join("\n");
}

static IDbDetailsFactory *createModuleDbDetailsFactory( const QString &connName, const QString &moduleName, QTextCodec *codec )
{
    QSqlDatabase db = QSqlDatabase::database(connName);

    if ( !db.isOpen() ) {
        Core::Storage::warning(QObject::tr("Database connection is not opened."));
        return 0;
    }

    QSqlQuery query(db);
    QString   queryString;

    queryString = "SELECT RDB$MODULE_ID FROM RDB$MODULES WHERE RDB$MODULE_NAME = :moduleName";
    query.prepare(queryString);
    query.bindValue(":moduleName", moduleName);
    QString moduleSource = "";

    if ( Database::Utils::executeQueryWithLog(&query) ) {
        if ( query.next() ) {
            qlonglong moduleId = query.value(0).toLongLong();
            query.clear();
            queryString = "SELECT RDB$MODULE_HDR_SOURCE FROM RDB$MODULES WHERE RDB$MODULE_NAME = \'" + moduleName.toUpper() + "\'";
            QString moduleHeaderSource = "RDB$MODULE_HDR_SOURCE";
            QStringList strList;
            if ( readBlobSegmentedList(connName, queryString, moduleHeaderSource, strList, codec, moduleName.toUpper()) ) {
                moduleSource = strList.join("\n");
                moduleSource.prepend("MODULE");
                moduleSource.append("\n");
                queryString = "SELECT RDB$ROUTINE_NAME, BITSTRING(RDB$FLAGS FROM 0 FOR 1) FROM RDB$ROUTINES where RDB$MODULE_ID = :moduleId";
                query.prepare(queryString);
                query.bindValue(":moduleId", moduleId);
                query.exec();
                QStringList subrNameList;
                QList <int> subrTypeList;
                while ( query.next() ) {
                    QString procFuncName = query.value(0).toString().trimmed();
                    int isFunc = query.value(1).toInt();
                    subrNameList << procFuncName;
                    subrTypeList << isFunc;
                }
                query.clear();
                for (int i = 0; i < subrNameList.size(); i++){
                    QString procFuncSource;
                    getFuncProcSource(connName, subrNameList[i], procFuncSource, codec);
                    if ( procFuncSource != "" ) {
                        moduleSource.append("\n");
                        if ( subrTypeList[i] )
                            moduleSource.append("FUNCTION");
                        else
                            moduleSource.append("PROCEDURE");
                        moduleSource.append(procFuncSource);
                        moduleSource.append(";\n");
                    }
                }
                moduleSource.append("\nEND MODULE;");
            }
        }
    }

    Database::DbDetailsSourceFactory *s = new Database::DbDetailsSourceFactory(moduleSource);
    return s;
}

static IDbDetailsFactory *createFuncProcDbDetailsFactory( const QString &connName, const QString &funcName, bool isFunc, QTextCodec *codec )
{
    QSqlDatabase db = QSqlDatabase::database(connName);

    if ( !db.isOpen() ) {
        Core::Storage::warning(QObject::tr("Database connection is not opened."));
        return 0;
    }

    QString queryString = "SELECT RDB$ROUTINE_SOURCE FROM RDB$ROUTINES WHERE RDB$ROUTINE_NAME = \'" + funcName.toUpper() + "\'";
    QString fieldTable = "RDB$ROUTINE_SOURCE";

    QString funcSource = "";
    QStringList strList;
    if ( readBlobSegmentedList(connName, queryString, fieldTable, strList, codec, funcName.toUpper()) ) {
        funcSource = strList.join("\n");
        if ( isFunc )
            funcSource.prepend("FUNCTION ");
        else
            funcSource.prepend("PROCEDURE ");
        funcSource.append(";\n");
    }

    Database::DbDetailsSourceFactory *s = new Database::DbDetailsSourceFactory(funcSource);
    return s;
}

IDbDetailsFactory *Provider::createDbDetailsFactory( const QString &connName, const QString &schemaName, int schemaObjectType,
                                                     int schemaObjectSubtype, const QString &objName ) const
{
    Q_UNUSED(schemaName)
    Q_UNUSED(schemaObjectSubtype)

    switch ( schemaObjectType ) {
        case Sql::Constants::DbSchemaObjectType::C_TABLE:       return createTableDbDetailsFactory(connName, objName, m_codec );
        case Sql::Constants::DbSchemaObjectType::C_VIEW:        return createViewDbDetailsFactory(connName, objName, m_codec);
        case Sql::Constants::DbSchemaObjectType::C_MODULE:      return createModuleDbDetailsFactory(connName, objName, m_codec);
        case Sql::Constants::DbSchemaObjectType::C_FUNCTION:    return createFuncProcDbDetailsFactory(connName, objName, true, m_codec);
        case Sql::Constants::DbSchemaObjectType::C_PROCEDURE:   return createFuncProcDbDetailsFactory(connName, objName, false, m_codec);
    }

    return 0;
}

QList<QAction *> Provider::actionsForItem( IDbCatalogItem *item ) const
{
    if ( item->schemaObjectSubtype() == Sql::Constants::DbSchemaObjectSubtype::C_UNDEFINED &&
         item->isNameDetailed() ) {
        switch ( item->schemaObjectType() ) {
            case Sql::Constants::DbSchemaObjectType::C_TABLE:
            case Sql::Constants::DbSchemaObjectType::C_VIEW:
                return QList<QAction *>() << m_actCreateDynamycSelect  << m_actCreateCStructure;
        }
    }

    return QList<QAction *>();
}

QList<QDbItemAction *> Provider::allActions() const
{
    return QList<QDbItemAction *>() << m_actCreateDynamycSelect << m_actCreateCStructure;
}

void Provider::onCreateCreateDynamycSelect( const QString &connName, IDbCatalogItem *item )
{
    QSqlDatabase db = QSqlDatabase::database(connName);

    if ( !db.isOpen() ) {
        Core::Storage::warning(tr("Database connection is not opened."));
        return;
    }

    QString tableName = item->name().toUpper();

    QSqlQuery query(db);
    QString   queryString;
    QList <QList <QVariant> > items;

    queryString = " SELECT TRIM(RF.RDB$FIELD_NAME), F.RDB$FIELD_TYPE, F.RDB$FIELD_LENGTH"
                  " FROM RDB$RELATIONS R, RDB$RELATION_FIELDS RF,RDB$FIELDS F"
                  " WHERE R.RDB$RELATION_NAME = :tableName AND"
                  "   RF.RDB$RELATION_NAME = R.RDB$RELATION_NAME AND"
                  "   RF.RDB$FIELD_SOURCE = F.RDB$FIELD_NAME AND"
                  "   R.RDB$SYSTEM_FLAG = 0"
                  "ORDER BY RF.RDB$FIELD_POSITION ASC";
    query.prepare(queryString);
    query.bindValue(":tableName", tableName);
    query.exec();
    while ( query.next() ) {
        QList <QVariant> item2;
        item2 << query.value(0).toString();
        item2 << query.value(1).toInt();
        items << item2;
    }

    QString queryText = "SELECT * FROM " + tableName;
    QString text = "{\n";
    QString varName = "e_" + tableName.toLower();

    text += "  " + tableName + " " + varName + ";\n\n";

    text += "  char szQuery[512];\n";
    text += "  char szErrorMessage[256];\n\n";

    text += "  long lRet;\n\n";

    text += "  sprintf(szQuery, \"" + queryText + "\");\n\n";

    text += "  opr_start();\n";
    text += "  opr_o_c(szQuery);\n";
    text += "  opr_send(OP_DYN_SMPL);\n\n";

    text += "  while ( opr_i() ) {\n";
    text += "    opr_i_l(&lRet);\n\n";

    text += "    if ( lRet == RET_NORMAL ) {\n";
    text += "      memset(&" + varName + ", 0, sizeof(" + tableName + "));\n\n";

    for (int i = 0; i < items.size(); i++) {
        QString fieldName = items[i][0].toString();
        int fieldType = items[i][1].toInt();
        switch ( fieldType ) {
        case 8:  text += "      opr_i_l(&" + varName + "." + fieldName + ");\n"; break;   // long
        case 14: text += "      opr_i_c( " + varName + "." + fieldName + ");\n"; break;   // char
        case 27: text += "      opr_i_d(&" + varName + "." + fieldName + ");\n"; break;   // double
        case 7: // VVV // short
        default: text += "      opr_i_s(&" + varName + "." + fieldName + ");\n"; break;
        }
    }
    for (int j = 0; j < items.size(); j++) {
        int fieldType = items[j][1].toInt();
        if ( fieldType == 14 ) {
            QString fieldName = items[j][0].toString();
            text += "      trim(" + varName + "." + fieldName + ");\n";
        }
    }
    text += "\n      /// TODO: Save Fetch Result Here.\n\n";
    text += "    }\n\n";

    text += "    if ( lRet != RET_NORMAL && lRet != RET_NO_DATA ) {\n";
    text += "      memset(szErrorMessage, 0, sizeof(szErrorMessage));\n";
    text += "      opr_i_c(szErrorMessage);\n";
    text += "    }\n";
    text += "  }\n\n";

    text += "  my_opr_end();\n\n";

    text += "  if ( lRet != RET_NORMAL && lRet != RET_NO_DATA ) {\n";
    text += "    ServerErrorMessage(lRet, trim(szErrorMessage));\n";
    text += "    return false;\n";
    text += "  }\n\n";

    text += "  return true;\n";
    text += "}\n";

    DocumentManager::getInstance()->insertText(text, "CPP");
}

void Provider::onCreateCStructure( const QString &connName, IDbCatalogItem *item )
{
    QString tableName = item->name();
    QString tableComment = "";
    QString text = "typedef struct {                                    /* ";
    int     fieldType;
    QString fieldName;
    QString structRow;

    QSqlDatabase db = QSqlDatabase::database(connName);

    if ( !db.isOpen() ) {
        Core::Storage::warning(tr("Database connection is not opened."));
        return;
    }

    QSqlQuery query(db);

    QString   queryString = "SELECT RDB$DESCRIPTION FROM RDB$RELATIONS WHERE RDB$RELATION_NAME = :tableName";

    query.prepare(queryString);
    query.bindValue(":tableName", tableName.toUpper());
    query.exec();
    if ( query.next() ) {
        tableComment = query.value(0).toString();
    }
    text += tableComment + "*/";

    queryString = " SELECT TRIM(RF.RDB$FIELD_NAME), F.RDB$FIELD_TYPE, F.RDB$FIELD_LENGTH, RF.RDB$DESCRIPTION"
                  " FROM RDB$RELATIONS R, RDB$RELATION_FIELDS RF,RDB$FIELDS F"
                  " WHERE R.RDB$RELATION_NAME = :tableName AND"
                  "   RF.RDB$RELATION_NAME = R.RDB$RELATION_NAME AND"
                  "   RF.RDB$FIELD_SOURCE = F.RDB$FIELD_NAME AND"
                  "   R.RDB$SYSTEM_FLAG = 0"
                  "ORDER BY RF.RDB$FIELD_POSITION ASC";
    query.prepare(queryString);
    query.bindValue(":tableName", tableName.toUpper());
    query.exec();
    while ( query.next() ) {
        text += "\n  ";
        fieldName = query.value(0).toString();
        fieldType = query.value(1).toInt();
        switch (fieldType) {
            case 7:     structRow = "short  "; break;
            case 8:     structRow = "long   "; break;
            case 14:    structRow = "char   "; break;
            case 27:    structRow = "double "; break;
            case 261:   structRow = "char   "; break;
            default:    structRow = "char   "; break;
        }
        for (int c = 0; c < 32 - fieldName.length(); c++)
            structRow += " ";
        structRow += fieldName;
        if ( fieldType == 14 || fieldType == 261 ) {
            structRow += "[";
            structRow += query.value(2).toString();
            structRow += "]";
        }
        structRow += ";";
        while (structRow.length() < 50)
            structRow += " ";
        structRow += "/* ";
        structRow += query.value(3).toString();
        structRow += " */";
        text += structRow;
    }

    text += "\n} " + tableName + ";\n";

    DocumentManager::getInstance()->insertText(text, "CPP");
}

#ifndef HAVE_QT5
Q_EXPORT_PLUGIN2(Provider, Provider)
#endif

} // RDB
} // Database

