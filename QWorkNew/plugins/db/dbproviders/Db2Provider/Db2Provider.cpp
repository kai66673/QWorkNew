#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QAction>
#include <QFileInfo>
#include <QDebug>

#include "Core.h"
#include "Db2Provider.h"
#include "Db2AuthWidget.h"
#include "DbUtils.h"
#include "sql_constants.h"
#include "db2_sql_constants.h"

namespace Database {
namespace Db2 {


Provider::Provider( QObject *parent )
    : QObject(parent)
{
}

BaseConnectionInfo *Provider::createConnection( const QString &connName, const QStringList &parameters ) const
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QDB2", connName);
    db.setHostName(parameters[1]);
    if ( !parameters[2].isEmpty() )
        db.setPort(parameters[2].toInt());
    db.setDatabaseName(parameters[0]);
    db.setUserName(parameters[3]);
    db.setPassword(parameters[4]);

    if ( !db.open() ) {
        QMessageBox::critical( Core::Storage::mainWindow()->widget(),
                               tr("Cannot open database:\n"),
                               tr("Unable to establish a database connection.\nClick Cancel to exit.\n\n%1\n(%2)")
                               .arg(db.lastError().text()).arg(QSqlDatabase::drivers().join(", ")),
                               QMessageBox::Cancel );
        return 0;
    }

    QString title = parameters[0] + "(" + parameters[3] + ")";

    QString descr;
    descr  = "Connection type: DB2 ([Q]ODBC)\n";
    descr += "Database source: " + parameters[0] + "\n";
    descr += "           Host: " + parameters[1] + "\n";
    descr += "           Port: " + parameters[2] + "\n";
    descr += "      User name: " + parameters[3];

    return new BaseConnectionInfo(connName, title, descr, parameters[3].toUpper());
}

IDbConnectWidget *Provider::createAuthWidget( QWidget *parent ) const
{
    return new Db2AuthWidget(parent);
}

MetadataQuery::Ptr Provider::schemasMetadataQuery() const
{
    return MetadataQuery::create("SELECT SCHEMANAME FROM SYSCAT.SCHEMATA ORDER BY SCHEMANAME");
}

MetadataQuery::Ptr Provider::objectsMetadataQuery( const QString &schemaName, int schemaObjectType ) const
{
    switch ( schemaObjectType ) {
        case Sql::Constants::DbSchemaObjectType::C_TABLE:
            return MetadataQuery::create("SELECT TABNAME FROM SYSCAT.TABLES WHERE TABSCHEMA = \'" + schemaName.toUpper() + "\' AND TYPE = \'T\' ORDER BY TABNAME");

        case Sql::Constants::DbSchemaObjectType::C_VIEW:
            return MetadataQuery::create("SELECT TABNAME FROM SYSCAT.TABLES WHERE TABSCHEMA = \'" + schemaName.toUpper() + "\' AND TYPE = \'V\' ORDER BY TABNAME");

        case Sql::Constants::DbSchemaObjectType::C_INDEX:
            return MetadataQuery::create("SELECT INDNAME, TABNAME FROM SYSCAT.INDEXES WHERE TABSCHEMA = \'" + schemaName.toUpper() + "\' ORDER BY INDNAME", 0, 2);

        case Sql::Constants::DbSchemaObjectType::C_TRIGGER:
            return MetadataQuery::create("SELECT TRIGNAME, TABNAME FROM SYSCAT.TRIGGERS WHERE TABSCHEMA = \'" + schemaName.toUpper() + "\' ORDER BY TRIGNAME", 0, 2);

        case Sql::Constants::DbSchemaObjectType::C_CONSTRAINT:
            return MetadataQuery::create("SELECT CONSTNAME, TABNAME FROM SYSCAT.TABCONST WHERE TABSCHEMA = \'" + schemaName.toUpper() + "\' ORDER BY CONSTNAME", 0, 2);

        case Sql::Constants::DbSchemaObjectType::C_MODULE:
            return MetadataQuery::create("SELECT MODULENAME FROM SYSCAT.MODULES WHERE MODULESCHEMA = \'" + schemaName.toUpper() + "\'");

        case Sql::Constants::DbSchemaObjectType::C_FUNCTION:
            return MetadataQuery::create( "SELECT ROUTINENAME FROM SYSCAT.ROUTINES WHERE ROUTINESCHEMA = \'" + schemaName.toUpper() + "\'"
                                          " AND ROUTINETYPE = \'F\' "
                                          " AND ROUTINEMODULEID IS NULL "
                                          " ORDER BY ROUTINENAME" );

        case Sql::Constants::DbSchemaObjectType::C_PROCEDURE:
            return MetadataQuery::create( "SELECT ROUTINENAME FROM SYSCAT.ROUTINES WHERE ROUTINESCHEMA = \'" + schemaName.toUpper() + "\'"
                                          " AND ROUTINETYPE = \'P\' "
                                          " AND ROUTINEMODULEID IS NULL "
                                          " ORDER BY ROUTINENAME" );
    }

    return MetadataQuery::Ptr(0);
}

MetadataQuery::Ptr Provider::subobjectsMetadataQuery( const QString &schemaName, int schemaObjectType, const QString &objName, int schemaObjectSubtype ) const
{
    switch ( schemaObjectType ) {

        case Sql::Constants::DbSchemaObjectType::C_TABLE:
            switch ( schemaObjectSubtype ) {
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_COLUMN:
                    return MetadataQuery::create("SELECT COLNAME, TYPENAME FROM SYSCAT.COLUMNS WHERE TABSCHEMA = \'" + schemaName.toUpper() +
                                                 "\' AND TABNAME = \'" + objName.toUpper() + "\' ORDER BY COLNO", 0, 2);
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_INDEX:
                    return MetadataQuery::create("SELECT INDNAME FROM SYSCAT.INDEXES WHERE TABSCHEMA = \'" + schemaName.toUpper() +
                                                 "\' AND TABNAME = \'" + objName.toUpper() + "\' ORDER BY INDNAME");
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_TRIGGER:
                    return MetadataQuery::create("SELECT TRIGNAME FROM SYSCAT.TRIGGERS WHERE TABSCHEMA = \'" + schemaName.toUpper() +
                                                 "\' AND TABNAME = \'" + objName.toUpper() + "\' ORDER BY TRIGNAME");
                case Sql::Constants::DbSchemaObjectSubtype::C_TABLE_CONSTRAINT:
                    return MetadataQuery::create("SELECT CONSTNAME FROM SYSCAT.TABCONST WHERE TABSCHEMA = \'" + schemaName.toUpper() +
                                                 "\' AND TABNAME = \'" + objName.toUpper() + "\' ORDER BY CONSTNAME");
                default:
                    return MetadataQuery::Ptr(0);
            }

        case Sql::Constants::DbSchemaObjectType::C_VIEW:
            switch ( schemaObjectSubtype ) {
                case Sql::Constants::DbSchemaObjectSubtype::C_VIEW_COLUMN:
                    return MetadataQuery::create("SELECT COLNAME, TYPENAME FROM SYSCAT.COLUMNS WHERE TABSCHEMA = \'" + schemaName + "\' AND TABNAME = \'" + objName.toUpper() + "\' ORDER BY COLNO", 0, 2);
                default:
                    return MetadataQuery::Ptr(0);
            }

        case Sql::Constants::DbSchemaObjectType::C_MODULE:
            switch ( schemaObjectType ) {
                case Sql::Constants::DbSchemaObjectSubtype::C_MODULE_FUNCTION:
                    return MetadataQuery::create( "SELECT R.ROUTINENAME, M.MODULENAME FROM SYSCAT.ROUTINES R, SYSCAT.MODULES M "
                                                  " WHERE R.ROUTINEMODULEID = M.MODULEID "
                                                  "   AND M.MODULENAME = \'" + objName.toUpper() + "\' "
                                                  "   AND R.ROUTINETYPE = \'F\' "
                                                  " ORDER BY R.ROUTINENAME", 0, 2 );
                case Sql::Constants::DbSchemaObjectSubtype::C_MODULE_PROCEDURE:
                    return MetadataQuery::create( "SELECT R.ROUTINENAME, M.MODULENAME FROM SYSCAT.ROUTINES R, SYSCAT.MODULES M "
                                                  " WHERE R.ROUTINEMODULEID = M.MODULEID "
                                                  "   AND M.MODULENAME = \'" + objName.toUpper() + "\' "
                                                  "   AND R.ROUTINETYPE = \'P\' "
                                                  " ORDER BY R.ROUTINENAME", 0, 2 );
            }
    }

    return MetadataQuery::Ptr(0);
}

MetadataQuery::Ptr Provider::objectTableNameMetadataQuery( const QString &schemaName, int schemaObjectType, const QString &objName ) const
{
    switch ( schemaObjectType ) {

        case Sql::Constants::DbSchemaObjectType::C_INDEX:
            return MetadataQuery::create("SELECT TABNAME FROM SYSCAT.INDEXES WHERE TABSCHEMA = \'" + schemaName.toUpper() + "\' AND INDNAME = \'" + objName.toUpper() + "\'");

        case Sql::Constants::DbSchemaObjectType::C_TRIGGER:
            return MetadataQuery::create("SELECT TABNAME FROM SYSCAT.TRIGGERS WHERE TABSCHEMA = \'" + schemaName.toUpper() + "\' AND TRIGNAME = \'" + objName.toUpper() + "\'");

        case Sql::Constants::DbSchemaObjectType::C_CONSTRAINT:
            return MetadataQuery::create("SELECT TABNAME FROM SYSCAT.TABCONST WHERE TABSCHEMA = \'" + schemaName.toUpper() + "\' AND CONSTNAME = \'" + objName.toUpper() + "\'");
    }

    return MetadataQuery::Ptr(0);
}

QStringList Provider::additionalSchemaCategories() const
{
    return QStringList() << QObject::tr("Modules")
                         << QObject::tr("Functions")
                         << QObject::tr("Procedures");
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
    }

    return QStringLiteral("");
}

QString Provider::additionalSchemaObjectIconName( int schemaObjectType , ItemProperties * ) const
{
    switch ( schemaObjectType ) {
        case Sql::Constants::DbSchemaObjectType::C_MODULE:          return QStringLiteral(":/images/dbb_module.png");
        case Sql::Constants::DbSchemaObjectType::C_FUNCTION:        return QStringLiteral(":/images/dbb_function.png");
        case Sql::Constants::DbSchemaObjectType::C_PROCEDURE:       return QStringLiteral(":/images/dbb_procedure.png");
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
        QString   queryString = "SELECT REMARKS FROM SYSCAT.TABLES WHERE TABSCHEMA = \'" + schemaName.toUpper() + "\' AND TABNAME = \'" + tableName.toUpper() + "\'";
        QString remark = "";
        if ( Database::Utils::executeQueryWithLog(&query, queryString) )
            if ( query.next() )
                remark = query.value(0).toString();
        t->addItem(Database::DbDetailsFormViewFactory::FormItem( QObject::tr("Description"), remark ));
    }

    // Table Columns Info
    Database::DbDetailsTableFactory *c = new Database::DbDetailsTableFactory( QStringList() << QObject::tr("Name")
                                                                                            << QObject::tr("Type")
                                                                                            << QObject::tr("Nullable")
                                                                                            << QObject::tr("Default Value")
                                                                                            << QObject::tr("PK Sequence")
                                                                                            << QObject::tr("Description") );
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = "SELECT COLNAME, TYPENAME, NULLS, DEFAULT, KEYSEQ, REMARKS FROM SYSCAT.COLUMNS WHERE TABSCHEMA = \'" + schemaName + "\' AND TABNAME = \'" + tableName + "\' ORDER BY COLNO";
        if ( Database::Utils::executeQueryWithLog(&query, queryString) )
            while ( query.next() ) {
                QStringList row;
                row << query.value(0).toString();
                row << query.value(1).toString();
                row << (query.value(2).toString() == QLatin1String("Y") ? QObject::tr("Yes") : QObject::tr("No"));
                row << query.value(3).toString();
                if ( query.value(4).isNull() )
                    row << "";
                else
                    row << query.value(4).toString();
                row << query.value(5).toString();
                c->addRow(row);
            }
    }

    // Table Indexes Info
    Database::DbDetailsTableWithTableFactory *in = new Database::DbDetailsTableWithTableFactory( QStringList() << QObject::tr("Name")
                                                                                                               << QObject::tr("Unique"),
                                                                                                 QStringList() << QObject::tr("Column")
                                                                                                               << QObject::tr("Order") );
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = "SELECT INDNAME, UNIQUERULE FROM SYSCAT.INDEXES WHERE TABSCHEMA = \'" + schemaName.toUpper() +
                      "\' AND TABNAME = \'" + tableName.toUpper() + "\' ORDER BY INDNAME";
        if ( Database::Utils::executeQueryWithLog(&query, queryString) ) {
            QList<QStringList> data;
            while ( query.next() ) {
                QStringList item;
                item << query.value(0).toString();
                QString rule = query.value(1).toString();
                if ( rule == QLatin1String("P") )
                    item << QObject::tr("Yes (PK)");
                else if ( rule == QLatin1String("U") )
                    item << QObject::tr("Yes");
                else
                    item << QObject::tr("No");
                data << item;
            }
            for (int i = 0; i < data.size(); i++) {
                queryString = "SELECT COLNAMES, COLCOUNT FROM SYSCAT.INDEXES WHERE TABSCHEMA = \'" + schemaName.toUpper() +
                              "\' AND INDNAME = \'" + data[i][0].toUpper() + "\'";
                query.exec(queryString);
                if ( query.next() ) {
                    QList<QStringList> indColumns;
                    QString columns = query.value(0).toString();
                    int colCount = query.value(1).toInt();
                    int currIndex = 0;
                    for (int j = 0; j < colCount && currIndex < columns.length(); j++) {
                        QStringList item;
                        QString sortOrder;
                        QString colName = "";
                        if ( columns[currIndex] == '-' )
                            sortOrder = QObject::tr("DESC");
                        else if ( columns[currIndex] == '+' )
                            sortOrder = QObject::tr("ASC");
                        else
                            continue;
                        currIndex++;
                        while ( currIndex < columns.length() && columns[currIndex] != '+' ) {
                            colName += columns[currIndex];
                            currIndex++;
                        }
                        item << colName << sortOrder;
                        indColumns << item;
                    }
                    in->addRow(data[i], indColumns);
                }
            }
        }
    }

    // Table Triggers Info
    Database::DbDetailsTableWithSourceFactory *tr = new Database::DbDetailsTableWithSourceFactory( QStringList() << QObject::tr("Name")
                                                                                                                 << QObject::tr("Type") );
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = "SELECT TRIGNAME, TRIGTIME, TRIGEVENT FROM SYSCAT.TRIGGERS WHERE TABSCHEMA = \'" + schemaName.toUpper() + "\' AND TABNAME = \'" + tableName.toUpper() + "\' ORDER BY TRIGNAME";
        if ( Database::Utils::executeQueryWithLog(&query, queryString) ) {
            QList<QStringList> data;
            while ( query.next() ) {
                QStringList item;
                item << query.value(0).toString();
                QString triggerTime = query.value(1).toString();
                QString triggerEvent = query.value(2).toString();
                QString triggerType = "AFTER";
                if ( triggerTime == QLatin1String("B") )
                    triggerType = "BEFORE";
                if ( triggerEvent == QLatin1String("I") )
                    triggerType += " INSERT";
                else if ( triggerEvent == QLatin1String("D") )
                    triggerType += " DELETE";
                else
                    triggerType += " UPDATE";
                item << triggerType;
                data << item;
            }
            for (int i = 0; i < data.size(); i++) {
                queryString = "SELECT TEXT FROM SYSCAT.TRIGGERS WHERE TABSCHEMA = \'" + schemaName + "\' AND TRIGNAME = \'" + data[i][0].toUpper() + "\'";
                query.exec(queryString);
                QString triggerSource = "";
                if ( query.next() ) {
                    triggerSource = query.value(0).toString();
                }
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
        queryString  = "SELECT \'[\' || COLNAME || \']\' ";
        queryString += "     , \'NOT NULL\' ";
        queryString += "  FROM SYSCAT.COLUMNS ";
        queryString += " WHERE TABSCHEMA = \'" + schemaName + "\' ";
        queryString += "   AND TABNAME = \'" + tableName + "\' ";
        queryString += "   AND NULLS = \'N\' ";
        queryString += " UNION ";
        queryString += "SELECT CONSTNAME ";
        queryString += "     , CASE TYPE ";
        queryString += "           WHEN \'P\' THEN \'PRIMARY KEY\' ";
        queryString += "           WHEN \'F\' THEN \'FOREIGN KEY\' ";
        queryString += "           WHEN \'U\' THEN \'UNIQUE\' ";
        queryString += "           WHEN \'K\' THEN \'CHECK\' ";
        queryString += "           ELSE \'\' ";
        queryString += "       END ";
        queryString += "  FROM SYSCAT.TABCONST ";
        queryString += " WHERE TABSCHEMA = \'" + schemaName + "\' ";
        queryString += "   AND TABNAME = \'" + tableName + "\' ";
        queryString += " ORDER BY 1";
        if ( Database::Utils::executeQueryWithLog(&query, queryString) ) {
            QList<QStringList> data;
            QString constraintSource;
            while ( query.next() ) {
                QStringList item;
                item << query.value(0).toString()
                     << query.value(1).toString();
                data << item;
            }
            for (int i = 0; i < data.size(); i++) {
                if ( data[i][0].startsWith("[") ) {
                    cn->addRow(data[i], "");
                } else if ( data[i][1] == QLatin1String("CHECK") ) {
                    queryString = "SELECT TEXT FROM SYSCAT.CHECKS WHERE TABSCHEMA = \'" + schemaName + "\' AND CONSTNAME = \'" + data[i][0].toUpper() + "\'";
                    query.exec(queryString);
                    constraintSource = "";
                    if ( query.next() )
                        constraintSource = query.value(0).toString();
                    cn->addRow(data[i], constraintSource);
                } else if ( data[i][1] == QLatin1String("FOREIGN KEY") ) {
                    queryString = "SELECT REFTABNAME, DELETERULE, UPDATERULE, FK_COLNAMES, PK_COLNAMES FROM SYSCAT.REFERENCES WHERE TABSCHEMA = \'" +
                                  schemaName + "\' AND CONSTNAME = \'" + data[i][0].toUpper() + "\'";
                    query.exec(queryString);
                    constraintSource = "";
                    if ( query.next() ) {
                        QString refTableName = query.value(0).toString();
                        QString deleteRule   = query.value(1).toString();
                        QString updateRule   = query.value(2).toString();
                        QString colNames     = query.value(3).toString();
                        QString refColNames  = query.value(4).toString();
                        constraintSource  = "FOREIGN KEY (" + colNames.split(QRegExp("\\s+"), QString::SkipEmptyParts).join(", ") + ")\n";
                        constraintSource += " REFERENCES " + refTableName + "(" + refColNames.split(QRegExp("\\s+"), QString::SkipEmptyParts).join(", ") + ")\n";
                        constraintSource += "  ON DELETE ";
                        QChar rule = deleteRule[0];
                        switch ( rule.toLatin1() ) {
                            case 'R': constraintSource += "RESTRICT";       break;
                            case 'C': constraintSource += "CASCADE";        break;
                            case 'N': constraintSource += "SET NULL";       break;
                            case 'D': constraintSource += "SET DEFAULT";    break;
                            default:  constraintSource += "NO ACTION";      break;
                        }
                        constraintSource += "\n";
                        constraintSource += "  ON UPDATE ";
                        rule = updateRule[0];
                        switch ( rule.toLatin1() ) {
                            case 'R': constraintSource += "RESTRICT";       break;
                            default:  constraintSource += "NO ACTION";      break;
                        }
                    }
                    cn->addRow(data[i], constraintSource);
                } else {
                    cn->addRow(data[i], "");
                }
            }
        }
    }

    Database::DbDetailsTabbedFactory *tab = new Database::DbDetailsTabbedFactory;
    tab->addTab(QObject::tr("Table Info"), t);
    tab->addTab(QObject::tr("Columns Info"), c);
    tab->addTab(QObject::tr("Indexes"), in);
    tab->addTab(QObject::tr("Triggers"), tr);
    tab->addTab(QObject::tr("Constraints"), cn);

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
        QString   queryString = "SELECT REMARKS FROM SYSCAT.TABLES WHERE TABSCHEMA = \'" + schemaName.toUpper() + "\' AND TABNAME = \'" + viewName.toUpper() + "\'";
        QString remark = "";
        if ( Database::Utils::executeQueryWithLog(&query, queryString) )
            if ( query.next() )
                remark = query.value(0).toString();
        t->addItem(Database::DbDetailsFormViewFactory::FormItem( QObject::tr("Description"), remark ));
        queryString = "SELECT TEXT FROM SYSCAT.VIEWS WHERE VIEWSCHEMA = \'" + schemaName.toUpper() + "\' AND VIEWNAME = \'" + viewName.toUpper() + "\'";
        QString source = "";
        if ( Database::Utils::executeQueryWithLog(&query, queryString) )
            if ( query.next() )
                source = query.value(0).toString();
        t->addItem(Database::DbDetailsFormViewFactory::FormItem( QObject::tr("Source"), source,
                                                                 Database::DbDetailsFormViewFactory::SourceText ));
    }

    // View Columns Info
    Database::DbDetailsTableFactory *c = new Database::DbDetailsTableFactory( QStringList() << QObject::tr("Name")
                                                                                            << QObject::tr("Type")
                                                                                            << QObject::tr("Nullable")
                                                                                            << QObject::tr("Default Value")
                                                                                            << QObject::tr("PK Sequence")
                                                                                            << QObject::tr("Description") );
    {
        QSqlQuery query(db);
        QString   queryString;
        queryString = "SELECT COLNAME, TYPENAME, NULLS, DEFAULT, KEYSEQ, REMARKS FROM SYSCAT.COLUMNS WHERE TABSCHEMA = \'" +
                schemaName.toUpper() + "\' AND TABNAME = \'" + viewName.toUpper() + "\' ORDER BY COLNO";
        if ( Database::Utils::executeQueryWithLog(&query, queryString) )
            while ( query.next() ) {
                QStringList row;
                row << query.value(0).toString();
                row << query.value(1).toString();
                row << (query.value(2).toString() == QLatin1String("Y") ? QObject::tr("Yes") : QObject::tr("No"));
                row << query.value(3).toString();
                if ( query.value(4).isNull() )
                    row << "";
                else
                    row << query.value(4).toString();
                row << query.value(5).toString();
                c->addRow(row);
            }
    }

    Database::DbDetailsTabbedFactory *tab = new Database::DbDetailsTabbedFactory;
    tab->addTab(QObject::tr("View Info"), t);
    tab->addTab(QObject::tr("Columns Info"), c);

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

    queryString = "SELECT TEXT FROM SYSCAT.ROUTINES WHERE ROUTINESCHEMA = \'" + schemaName.toUpper()
            + "\' AND ROUTINENAME = \'" + funcName.toUpper() +"\'";
    if ( Database::Utils::executeQueryWithLog(&query, queryString) )
        if ( query.next() )
            source = query.value(0).toString();

    Database::DbDetailsSourceFactory *s = new Database::DbDetailsSourceFactory(source);
    return s;
}

IDbDetailsFactory *Provider::createDbDetailsFactory( const QString &connName, const QString &schemaName, int schemaObjectType,
                                                     int schemaObjectSubtype, const QString &objName ) const
{
    Q_UNUSED(schemaObjectSubtype)

    switch ( schemaObjectType ) {
        case Sql::Constants::DbSchemaObjectType::C_TABLE:       return createTableDbDetailsFactory(connName, schemaName, objName);
        case Sql::Constants::DbSchemaObjectType::C_VIEW:        return createViewDbDetailsFactory(connName, schemaName, objName);
        case Sql::Constants::DbSchemaObjectType::C_FUNCTION:    return createSourceDbDetailsFactory(connName, schemaName, objName);
        case Sql::Constants::DbSchemaObjectType::C_PROCEDURE:   return createSourceDbDetailsFactory(connName, schemaName, objName);
    }

    return 0;
}

#ifndef HAVE_QT5
Q_EXPORT_PLUGIN2(Provider, Provider)
#endif

} // Db2
} // Database

