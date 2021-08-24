#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QAction>

#include <QDebug>

#include "Core.h"
#include "PostgresProvider.h"
#include "PostgresAuthWidget.h"
#include "DbUtils.h"
#include "DbCatalog.h"
#include "postgres_sql_constants.h"
#include "DbLoader.h"
#include "engine/PostgresParserEngine.h"

namespace Database {
namespace Postgres {

static QString toCamelCase(const QString &txt)
{
    QString result;
    bool need_to_upper = true;
    for (auto ch: txt) {
        if (ch == QChar('_')) {
            need_to_upper = true;
            continue;
        }
        if (need_to_upper && ch.isLetter())
            result += ch.toUpper();
        else
            result += ch;
        need_to_upper = false;
    }
    return result;
}

static QMap<QString, QString> s_py_types = {
    {"integer", "Integer"},
    {"text", "String"},
    {"boolean", "Boolean"},
    {"character varying", "String"},
    {"double precision", "Float"}
};

static QString OrmTypeString(const QString &type, const QString &length)
{
    if (type == "character varying") {
        if (length.isEmpty())
            return QString("String");
        return QString("String(%1)").arg(length);
    }

    QString py_type = s_py_types.value(type);

    return py_type.isEmpty() ? type : py_type;
}

Provider::Provider( QObject *parent )
    : QObject(parent)
    , m_engine(new ::Postgres::PostgresParserEngine())
{
}

BaseConnectionInfo *Provider::createConnection( const QString &connName, const QStringList &parameters ) const
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", connName);
    db.setDatabaseName(parameters[0]);
    db.setHostName(parameters[1]);
    db.setPort(parameters[2].toInt());
    db.setUserName(parameters[3]);
    db.setPassword(parameters[4]);

    if ( !db.open() ) {
        QMessageBox::critical( Core::Storage::mainWindow()->widget(),
                               tr("Cannot open database"),
                               tr("Unable to establish a database connection.\nClick Cancel to exit."),
                               QMessageBox::Cancel );
        return 0;
    }

    QString title = parameters[0] + "(" + parameters[3] + ")";

    QString descr;
    descr  = "Connection type: [Q]PSQL\n";
    descr += "  Database Name: " + parameters[0] + "\n";
    descr += "           Host: " + parameters[1] + "\n";
    descr += "           Port: " + parameters[2] + "\n";
    descr += "      User Name: " + parameters[3];

    return new BaseConnectionInfo(connName, title, descr, "public");
}

IDbConnectWidget *Provider::createAuthWidget( QWidget *parent ) const
{
    return new PostgresAuthWidget(parent);
}

MetadataQuery::Ptr Provider::dbObjectsMetadataQuery(int dbObjectType) const
{
    switch ( dbObjectType ) {
        case Sql::Constants::DbDatabaseObjectType::C_TABLESPACE:
            return MetadataQuery::create("SELECT spcname FROM pg_catalog.pg_tablespace ORDER BY 1");
        case Sql::Constants::DbDatabaseObjectType::C_GROUP_ROLE:
            return MetadataQuery::create("SELECT groname FROM pg_catalog.pg_group ORDER BY 1");
        case Sql::Constants::DbDatabaseObjectType::C_USER:
            return MetadataQuery::create("SELECT rolname FROM pg_catalog.pg_roles ORDER BY 1");
    }

    return MetadataQuery::Ptr(0);
}

MetadataQuery::Ptr Provider::schemasMetadataQuery() const
{
    return MetadataQuery::create("SELECT schemata.schema_name FROM information_schema.schemata "
                                 " WHERE schemata.schema_name NOT IN ('pg_toast', 'pg_temp_1', 'pg_toast_temp_1', 'pg_catalog', 'information_schema')");
}

MetadataQuery::Ptr Provider::objectsMetadataQuery( const QString &schemaName, int schemaObjectType ) const
{
    switch ( schemaObjectType ) {

        case Sql::Constants::DbSchemaObjectType::C_TABLE:
            return MetadataQuery::create("SELECT pg_tables.tablename FROM pg_catalog.pg_tables WHERE pg_tables.schemaname = '" + schemaName + "' ORDER BY pg_tables.tablename");

        case Sql::Constants::DbSchemaObjectType::C_VIEW:
            return MetadataQuery::create("SELECT pg_views.viewname FROM pg_catalog.pg_views WHERE pg_views.schemaname = '" + schemaName + "' ORDER BY pg_views.viewname");

        case Sql::Constants::DbSchemaObjectType::C_INDEX:
            return MetadataQuery::create("SELECT pg_indexes.indexname, pg_indexes.tablename FROM pg_catalog.pg_indexes WHERE pg_indexes.schemaname = '" +
                                         schemaName + "' ORDER BY pg_indexes.indexname", 0, 2);

        case Sql::Constants::DbSchemaObjectType::C_TRIGGER:
            return MetadataQuery::create("SELECT DISTINCT triggers.trigger_name, triggers.event_object_table FROM information_schema.triggers WHERE triggers.trigger_schema = '" + schemaName + "' ORDER BY triggers.trigger_name", 0, 2);

        case Sql::Constants::DbSchemaObjectType::C_CONSTRAINT:
            return MetadataQuery::create("SELECT constraint_table_usage.constraint_name, constraint_table_usage.table_name FROM information_schema.constraint_table_usage "
                                         "WHERE constraint_table_usage.constraint_schema = '"
                                         + schemaName + "' ORDER BY constraint_table_usage.constraint_name", 0, 2);

        case Sql::Constants::DbSchemaObjectType::C_FUNCTION:
            return MetadataQuery::create("SELECT DISTINCT p.proname FROM pg_proc p "
                                         "  JOIN pg_namespace n ON n.oid = p.pronamespace AND nspname = '" + schemaName + "'"
                                         " WHERE pg_catalog.format_type(prorettype, NULL) != 'trigger'");

        case Sql::Constants::DbSchemaObjectType::C_TRIGGER_FUNCTION:
            return MetadataQuery::create("SELECT DISTINCT p.proname FROM pg_proc p "
                                         "  JOIN pg_namespace n ON n.oid = p.pronamespace AND nspname = '" + schemaName + "'"
                                         " WHERE pg_catalog.format_type(prorettype, NULL) = 'trigger'");

        case Sql::Constants::DbSchemaObjectType::C_SEQUENCE:
            return MetadataQuery::create("SELECT sequence_name FROM information_schema.sequences WHERE sequence_schema = '" + schemaName + "'");

        case Sql::Constants::DbSchemaObjectType::C_TYPE:
            return MetadataQuery::create("SELECT t.typname, t.typtype FROM pg_catalog.pg_type t "
                                         "  LEFT JOIN pg_catalog.pg_namespace n ON n.oid = t.typnamespace "
                                         " WHERE (t.typrelid = 0 OR EXISTS (SELECT 1 FROM pg_catalog.pg_class c WHERE c.oid = t.typrelid AND c.relkind = 'c')) "
                                         "   AND NOT EXISTS(SELECT 1 FROM pg_catalog.pg_type el WHERE el.oid = t.typelem AND el.typarray = t.oid) "
                                         "   AND n.nspname = '" + schemaName + "' ORDER BY 1, 2", 0, 1, MetadataQuery::Properties() << qMakePair(1, QString("TypeKind")));
    }

    return MetadataQuery::Ptr(0);
}

MetadataQuery::Ptr Provider::subobjectsMetadataQuery( const QString &schemaName, int schemaObjectType, const QString &objName, int schemaObjectSubtype ) const
{
    switch ( schemaObjectType ) {

        case Sql::Constants::DbSchemaObjectType::C_TABLE:
            switch ( schemaObjectSubtype ) {
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_COLUMN:
                    return MetadataQuery::create( "SELECT columns.column_name, columns.data_type FROM information_schema.columns WHERE columns.table_schema = '" + schemaName +
                                                  "' AND columns.table_name = '" + objName + "' ORDER BY columns.ordinal_position", 0, 2 );
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_INDEX:
                    return MetadataQuery::create( "SELECT pg_indexes.indexname FROM pg_catalog.pg_indexes WHERE pg_indexes.schemaname = '" + schemaName +
                                                  "' AND pg_indexes.tablename = '" + objName + "'" );
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_TRIGGER:
                    return MetadataQuery::create( "SELECT DISTINCT triggers.trigger_name FROM information_schema.triggers WHERE triggers.trigger_schema = '" + schemaName +
                                                  "' AND triggers.event_object_table = '" + objName + "'" );
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_CONSTRAINT:
                    return MetadataQuery::create( "SELECT constraint_table_usage.constraint_name FROM information_schema.constraint_table_usage WHERE constraint_table_usage.constraint_schema = '"
                                                  + schemaName + "' AND constraint_table_usage.table_name = '" + objName + "'" );
                default:
                    return MetadataQuery::Ptr(0);
            }

        case Sql::Constants::DbSchemaObjectType::C_VIEW:
            switch ( schemaObjectSubtype ) {
                case Sql::Constants::DbSchemaObjectSubtype::C_VIEW_COLUMN:
                    return MetadataQuery::create( "SELECT view_column_usage.column_name FROM information_schema.view_column_usage WHERE view_column_usage.view_schema = '" + schemaName +
                                                  "' AND view_column_usage.view_name = '" + objName + "'", 0, 2 );
                default:
                    return MetadataQuery::Ptr(0);
            }
    }

    return MetadataQuery::Ptr(0);
}

MetadataQuery::Ptr Provider::objectTableNameMetadataQuery( const QString &schemaName, int schemaObjectType, const QString &objName ) const
{
    switch ( schemaObjectType ) {

        case Sql::Constants::DbSchemaObjectType::C_INDEX:
            return MetadataQuery::create("SELECT pg_indexes.tablename FROM pg_catalog.pg_indexes WHERE pg_indexes.schemaname = '" + schemaName + "' AND pg_indexes.indexname = '" + objName + "'");

        case Sql::Constants::DbSchemaObjectType::C_TRIGGER:
            return MetadataQuery::create("SELECT DISTINCT triggers.event_object_table FROM information_schema.triggers WHERE triggers.trigger_schema = '" + schemaName + "' AND triggers.trigger_name = '" + objName + "'");

        case Sql::Constants::DbSchemaObjectType::C_CONSTRAINT:
            return MetadataQuery::create( "SELECT constraint_table_usage.table_name FROM information_schema.constraint_table_usage WHERE constraint_table_usage.constraint_schema = '"
                                          + schemaName + "' AND constraint_table_usage.constraint_name = '" + objName + "'" );
    }

    return MetadataQuery::Ptr(0);
}

QStringList Provider::additionalDbCategories() const
{
    return QStringList() << QObject::tr("Tablespaces") << QObject::tr("Groups") << QObject::tr("Users");
}

QString Provider::additionalDbCategoryIconName(int dbObjectType) const
{
    switch ( dbObjectType ) {
        case Sql::Constants::DbDatabaseObjectType::C_TABLESPACE:
            return QStringLiteral(":/images/dbb_tablespaces.png");
        case Sql::Constants::DbDatabaseObjectType::C_GROUP_ROLE:
            return QStringLiteral(":/images/dbb_groups.png");
        case Sql::Constants::DbDatabaseObjectType::C_USER:
            return QStringLiteral(":/images/dbb_users.png");
    }

    return QStringLiteral("");
}

QString Provider::additionalDbObjectIconName(int dbObjectType) const
{
    switch ( dbObjectType ) {
        case Sql::Constants::DbDatabaseObjectType::C_TABLESPACE:
            return QStringLiteral(":/images/dbb_tablespace.png");
        case Sql::Constants::DbDatabaseObjectType::C_GROUP_ROLE:
            return QStringLiteral(":/images/dbb_group.png");
        case Sql::Constants::DbDatabaseObjectType::C_USER:
            return QStringLiteral(":/images/dbb_user.png");
    }

    return QStringLiteral("");
}

QStringList Provider::additionalSchemaCategories() const
{
    return QStringList() << QObject::tr("Functions")
                         << QObject::tr("Trigger Functions")
                         << QObject::tr("Sequences")
                         << QObject::tr("Types");
}

QString Provider::additionalSchemaCategoryIconName( int schemaObjectType ) const
{
    switch ( schemaObjectType ) {
        case Sql::Constants::DbSchemaObjectType::C_FUNCTION:            return QStringLiteral(":/images/dbb_funcproc_list.png");
        case Sql::Constants::DbSchemaObjectType::C_TRIGGER_FUNCTION:    return QStringLiteral(":/images/dbb_funcproc_list.png");
        case Sql::Constants::DbSchemaObjectType::C_SEQUENCE:            return QStringLiteral(":/images/dbb_sequences.png");
        case Sql::Constants::DbSchemaObjectType::C_TYPE:                return QStringLiteral(":/images/pgsql_dbb_types.png");
    }

    return QStringLiteral("");
}

QString Provider::additionalSchemaObjectIconName( int schemaObjectType , ItemProperties * properties ) const
{
    switch ( schemaObjectType ) {
        case Sql::Constants::DbSchemaObjectType::C_FUNCTION:            return QStringLiteral(":/images/dbb_function.png");
        case Sql::Constants::DbSchemaObjectType::C_TRIGGER_FUNCTION:    return QStringLiteral(":/images/dbb_procedure.png");
        case Sql::Constants::DbSchemaObjectType::C_SEQUENCE:            return QStringLiteral(":/images/dbb_sequence.png");
        case Sql::Constants::DbSchemaObjectType::C_TYPE: {
            QVariant typeKind = properties->property("TypeKind");
            if ( typeKind.isValid() ) {
                QChar ch = typeKind.toString()[0];
                switch ( ch.toLatin1() ) {
                    case 'c': return QStringLiteral(":/images/pgsql_dbb_class");
                    case 'e': return QStringLiteral(":/images/pgsql_dbb_enum");
                    case 'r': return QStringLiteral(":/images/pgsql_dbb_range");
                }
            }
            return QStringLiteral(":/images/pgsql_dbb_class.png");
        }
    }

    return QStringLiteral("");
}

static IDbDetailsFactory *createTableDbDetailsFactory( const QString &connName, const QString &schemaName, const QString &tableName )
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
        QString   queryString;
        queryString = QString( "SELECT d.description "
                               "  FROM pg_description d "
                               "  JOIN pg_class c ON d.objoid = c.oid "
                               "  JOIN pg_namespace n ON c.relnamespace = n.oid "
                               " WHERE n.nspname = '%1' "
                               "   AND c.relname = '%2' "
                               "   AND d.objsubid = 0").arg(schemaName).arg(tableName);
        QString description = "";
        if ( Database::Utils::executeQueryWithLog(&query, queryString) )
            if ( query.next() )
                description = query.value(0).toString();
        t->addItem(Database::DbDetailsFormViewFactory::FormItem(QObject::tr("Description"), description));
    }

    // Table Columns Info
    Database::DbDetailsTableFactory *c = new Database::DbDetailsTableFactory( QStringList() << QObject::tr("Name")
                                                                                            << QObject::tr("Description")
                                                                                            << QObject::tr("Data Type")
                                                                                            << QObject::tr("Nullable") );
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = QString( "SELECT col.column_name, d.description, col.data_type, col.is_nullable "
                               "  FROM pg_class c "
                               "  JOIN pg_namespace n ON c.relnamespace = n.oid "
                               "  JOIN information_schema.columns col "
                               "    ON col.table_schema = n.nspname "
                               "   AND col.table_name = c.relname "
                               "  LEFT "
                               "  JOIN pg_description d "
                               "    ON d.objoid = c.oid "
                               "   AND d.objsubid = col.ordinal_position "
                               " WHERE n.nspname = '%1' "
                               "   AND c.relname = '%2'" ).arg(schemaName).arg(tableName);
        if ( Database::Utils::executeQueryWithLog(&query, queryString) )
            while ( query.next() ) {
                QStringList row;
                row << query.value(0).toString();
                row << query.value(1).toString();
                row << query.value(2).toString();
                row << query.value(3).toString();
                c->addRow(row);
            }
    }

    // Table Indexes Info
    Database::DbDetailsTableWithSourceFactory *in = new Database::DbDetailsTableWithSourceFactory( QStringList() << QObject::tr("Name") );
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = QString( "SELECT indexname, indexdef FROM pg_indexes "
                               " WHERE schemaname = '%1' "
                               "   AND tablename = '%2'"
                               " ORDER BY indexname" ).arg(schemaName).arg(tableName);
        if ( Database::Utils::executeQueryWithLog(&query, queryString) ) {
            while ( query.next() )
                in->addRow( QStringList() << query.value(0).toString(), query.value(1).toString() );
        }
    }

    // Table Triggers Info
    Database::DbDetailsTableWithSourceFactory *tr = new Database::DbDetailsTableWithSourceFactory( QStringList() << QObject::tr("Name") );
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = QString( "SELECT t.tgname, pg_get_triggerdef(t.oid) "
                               "  FROM pg_trigger t "
                               "  JOIN pg_class c ON t.tgrelid = c.oid "
                               "  JOIN pg_namespace n ON c.relnamespace = n.oid "
                               " WHERE n.nspname = '%1' "
                               "   AND c.relname = '%2'" ).arg(schemaName).arg(tableName);
        if ( Database::Utils::executeQueryWithLog(&query, queryString) ) {
            while ( query.next() )
                tr->addRow( QStringList() << query.value(0).toString(), query.value(1).toString() );
        }
    }

    // Table Constraints Info
    Database::DbDetailsTableWithSourceFactory *cn = new Database::DbDetailsTableWithSourceFactory( QStringList() << QObject::tr("Name")
                                                                                                                 << QObject::tr("Deferrable")
                                                                                                                 << QObject::tr("Deferred") );
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = QString( "SELECT c.conname, c.condeferrable, c.condeferred, pg_get_constraintdef(c.oid) "
                               "  FROM pg_constraint c "
                               "  JOIN pg_class cl ON c.conrelid = cl.oid "
                               "  JOIN pg_namespace n ON c.connamespace = n.oid "
                               " WHERE n.nspname = '%1' "
                               "   AND cl.relname = '%2'" ).arg(schemaName).arg(tableName);
        if ( Database::Utils::executeQueryWithLog(&query, queryString) ) {
            while ( query.next() )
                cn->addRow( QStringList() << query.value(0).toString()
                                          << query.value(1).toString()
                                          << query.value(2).toString(),
                            query.value(3).toString() );
        }
    }

    // ORM class
    QString cls_src = QString("class %1(Base):\n").arg(toCamelCase(tableName));
    cls_src += QString("    __tablename__ = '%1'\n\n").arg(tableName);
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = QString("SELECT columns.column_name, columns.data_type, columns.character_maximum_length,"
                              "  case when columns.column_name in ("
                              "         select ccu.column_name "
                              "           FROM information_schema.table_constraints tc"
                              "              , information_schema.constraint_column_usage ccu "
                              "          where ccu.constraint_schema = tc.constraint_schema "
                              "            and ccu.constraint_name = tc.constraint_name "
                              "            and tc.constraint_type = 'PRIMARY KEY' and tc.table_name = '%2' "
                              "            and tc.table_schema = '%1') "
                              "       then true "
                              "       else false "
                              "   end as is_pk "
                              "  FROM information_schema.columns "
                              " WHERE columns.table_schema = '%1' "
                              "   AND columns.table_name = '%2' "
                              " ORDER BY columns.ordinal_position;")
                .arg(schemaName)
                .arg(tableName);
        if (Database::Utils::executeQueryWithLog(&query, queryString)) {
            while ( query.next() ) {
                bool is_pk = query.value(3).toBool();
                cls_src += QString("    %1 = Column(%2%3)\n")
                        .arg(query.value(0).toString())
                        .arg(OrmTypeString(query.value(1).toString(),
                                           query.value(2).toString()))
                        .arg(is_pk ? ", primary_key=True" : "");
            }
        }
    }

    Database::DbDetailsPySourceFactory *ps = new Database::DbDetailsPySourceFactory(cls_src);

    Database::DbDetailsTabbedFactory *tab = new Database::DbDetailsTabbedFactory;
    tab->addTab(QObject::tr("Table Info"), t);
    tab->addTab(QObject::tr("Columns Info"), c);
    tab->addTab(QObject::tr("Indexes"), in);
    tab->addTab(QObject::tr("Triggers"), tr);
    tab->addTab(QObject::tr("Constraints"), cn);
    tab->addTab(QObject::tr("Sqlalchemy Table Class"), ps);

    return tab;
}

static IDbDetailsFactory *createViewDbDetailsFactory( const QString &connName, const QString &schemaName, const QString &viewName )
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
        QString   queryString;
        queryString = "SELECT d.description, pg_get_viewdef(c.oid) FROM pg_class c JOIN pg_namespace n on c.relnamespace = n.oid LEFT JOIN pg_description d ON d.objoid = c.oid WHERE n.nspname = '"
                + schemaName + "' AND c.relname = '" + viewName + "'";
        if ( Database::Utils::executeQueryWithLog(&query, queryString) ) {
            if ( query.next() ) {
                t->addItem(Database::DbDetailsFormViewFactory::FormItem(QObject::tr("Description"), query.value(0).toString()));
                t->addItem(Database::DbDetailsFormViewFactory::FormItem( QObject::tr("Source"), query.value(1).toString(), Database::DbDetailsFormViewFactory::SourceText ));
            }
        }
    }

    // Table Columns Info
    Database::DbDetailsTableFactory *c = new Database::DbDetailsTableFactory( QStringList() << QObject::tr("Column")
                                                                                            << QObject::tr("Table")
                                                                                            << QObject::tr("Data Type")
                                                                                            << QObject::tr("Nullable") );
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = QString( "SELECT c.column_name, c.table_name, c.data_type, c.is_nullable "
                               "  FROM information_schema.view_column_usage vc "
                               "  JOIN information_schema.columns c "
                               "    ON c.column_name = vc.column_name "
                               "   AND c.table_name = vc.table_name "
                               " WHERE vc.view_schema = '%1' "
                               "   AND vc.view_name = '%2' "
                               " ORDER BY c.table_name, c.ordinal_position" ).arg(schemaName).arg(viewName);
        if ( Database::Utils::executeQueryWithLog(&query, queryString) )
            while ( query.next() ) {
                QStringList row;
                row << query.value(0).toString();
                row << query.value(1).toString();
                row << query.value(2).toString();
                row << query.value(3).toString();
                c->addRow(row);
            }
    }

    Database::DbDetailsTabbedFactory *tab = new Database::DbDetailsTabbedFactory;
    tab->addTab(QObject::tr("View Info"), t);
    tab->addTab(QObject::tr("Used Columns"), c);

    return tab;
}

static IDbDetailsFactory *createSourceDbDetailsFactory( const QString &connName, const QString &schemaName, const QString &funcName )
{
    QSqlDatabase db = QSqlDatabase::database(connName);

    if ( !db.isOpen() ) {
        Core::Storage::warning(QObject::tr("Database connection is not opened."));
        return 0;
    }

    QString source = "";
    QSqlQuery query(db);
    QString   queryString;

    queryString = QString( "SELECT pg_get_functiondef(oid) FROM pg_proc "
                           " WHERE proowner IN (SELECT nspowner FROM pg_namespace WHERE nspname = '%1') "
                           "   AND proname = '%2'" ).arg(schemaName).arg(funcName);
    if ( Database::Utils::executeQueryWithLog(&query, queryString) )
        if ( query.next() )
            source = query.value(0).toString();

    Database::DbDetailsSourceFactory *s = new Database::DbDetailsSourceFactory(source);
    return s;
}

static IDbDetailsFactory *createSequenceDbDetailsFactory( const QString &connName, const QString &schemaName, const QString &seqName )
{
    QSqlDatabase db = QSqlDatabase::database(connName);

    if ( !db.isOpen() ) {
        Core::Storage::warning(QObject::tr("Database connection is not opened."));
        return 0;
    }

    Database::DbDetailsFormViewFactory *t = new Database::DbDetailsFormViewFactory;

    QString data_type = "";
    QString increment = "";
    QString minimum_value = "";
    QString maximum_value = "";
    QString start_value = "";
    QString cycle_option = "";

    QSqlQuery query(db);
    QString   queryString;
    queryString = QString( "SELECT data_type, increment, minimum_value, maximum_value, start_value, cycle_option "
                           "  FROM information_schema.sequences "
                           " WHERE sequence_schema = '%1' "
                           "   AND sequence_name = '%2'" ).arg(schemaName).arg(seqName);
    if ( Database::Utils::executeQueryWithLog(&query, queryString) ) {
        if ( query.next() ) {
            data_type       = query.value(0).toString();
            increment       = query.value(1).toString();
            minimum_value   = query.value(2).toString();
            maximum_value   = query.value(3).toString();
            start_value     = query.value(4).toString();
            cycle_option    = query.value(5).toString();
        }
    }

    t->addItem( Database::DbDetailsFormViewFactory::FormItem(QObject::tr("Name"), seqName) );
    t->addItem( Database::DbDetailsFormViewFactory::FormItem(QObject::tr("Data Type"), data_type) );
    t->addItem( Database::DbDetailsFormViewFactory::FormItem(QObject::tr("Increment"), increment) );
    t->addItem( Database::DbDetailsFormViewFactory::FormItem(QObject::tr("Minimum"), minimum_value) );
    t->addItem( Database::DbDetailsFormViewFactory::FormItem(QObject::tr("Maximum"), maximum_value) );
    t->addItem( Database::DbDetailsFormViewFactory::FormItem(QObject::tr("Start"), start_value) );
    t->addItem( Database::DbDetailsFormViewFactory::FormItem(QObject::tr("Sycled"), cycle_option) );

    return t;
}

IDbDetailsFactory *Provider::createDbDetailsFactory( const QString &connName, const QString &schemaName, int schemaObjectType,
                                                     int schemaObjectSubtype, const QString &objName ) const
{
    Q_UNUSED(schemaName)
    Q_UNUSED(schemaObjectSubtype)

    switch ( schemaObjectType ) {
        case Sql::Constants::DbSchemaObjectType::C_TABLE:               return createTableDbDetailsFactory(connName, schemaName, objName );
        case Sql::Constants::DbSchemaObjectType::C_VIEW:                return createViewDbDetailsFactory(connName, schemaName, objName);
        case Sql::Constants::DbSchemaObjectType::C_FUNCTION:            return createSourceDbDetailsFactory(connName, schemaName, objName);
        case Sql::Constants::DbSchemaObjectType::C_TRIGGER_FUNCTION:    return createSourceDbDetailsFactory(connName, schemaName, objName);
        case Sql::Constants::DbSchemaObjectType::C_SEQUENCE:            return createSequenceDbDetailsFactory(connName, schemaName, objName);
    }

    return 0;
}

#ifndef HAVE_QT5
Q_EXPORT_PLUGIN2(Provider, Provider)
#endif

} // Postgres
} // Database
