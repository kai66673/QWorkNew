#include <QSqlDatabase>
#include <QSqlDriver>
#include <QMessageBox>
#include <QPushButton>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QApplication>

#include "SqlDocument.h"
#include "AST.h"
#ifndef HAVE_QT5
#include "QtCore/qtconcurrentrun.h"
#else
#include "QtConcurrent/qtconcurrentrun.h"
#endif
#include <progressmanager.h>

#include "texteditor.h"
#include "DbConnection.h"
#include "SqlBindHelper.h"
#include "SqlStatement.h"
#include "SqlParserEngineManager.h"
#include "sql_constants.h"

#include "IDocument.h"
#include "ITextDocument.h"
#include "Core.h"

namespace Database {

struct DbJob {
    DbJob( const QString &source )
        : sqlDocument(Sql::Document::create("<EXECUTE>"))
        , itemsExecuted(0)
    {
        sqlDocument->setSource(source.toLatin1());
        sqlDocument->fillStatementFromText( &statements,
                                            SqlParserEngineManager::instance()->currentEngine(),
                                            source );
    }

    DbJob( const QString &source, ExecuteType executeType, ITextDocument *textDocument )
        : sqlDocument(Sql::Document::create("<EXECUTE>"))
        , itemsExecuted(0)
    {
        sqlDocument->setSource(source.toLatin1());
        sqlDocument->fillSqlStatements( &statements,
                                        SqlParserEngineManager::instance()->currentEngine(),
                                        executeType,
                                        textDocument );
    }

    virtual ~DbJob() {
        qDeleteAll(statements);
    }

    Sql::Document::Ptr sqlDocument;
    QList<Sql::SqlStatement *> statements;
    int itemsExecuted;
    QMap<QString, Sql::SqlBindValue> bindings;
    QFutureInterface<void> futureInterface;
};

struct DbJobItem {
    DbJobItem( DbQuery::Ptr query_,
               bool isSelectStatement_,
               bool isBlockStatement_,
               bool isCommitStatement_,
               bool isRollbackStatement_,
               const QString &statementText_ )
        : query(query_)
        , isSelectStatement(isSelectStatement_)
        , isBlockStatement(isBlockStatement_)
        , isCommitStatement(isCommitStatement_)
        , isRollbackStatenent(isRollbackStatement_)
        , statementText(statementText_)
        , successfullyExecuted(false)
        , queryData(0)
        , resultMessage("")
    {
        if ( isSelectStatement_ )
            queryData = new QList <QList <QVariant> >();
    }

    void clear() {
        resultMessage = "";
        if ( queryData ) {
            if ( !isSelectStatement || !successfullyExecuted )
                delete queryData;
        }
        queryData = 0;
        successfullyExecuted = false;
    }

    DbQuery::Ptr query;
    bool isSelectStatement;
    bool isBlockStatement;
    bool isCommitStatement;
    bool isRollbackStatenent;
    QString statementText;
    bool successfullyExecuted;
    QList <QList <QVariant> > *queryData;       /// TODO: use QSharedPointer
    QString resultMessage;
};

static DbJobItem *executeJobItem( DbJobItem *job, const QMap<QString, Sql::SqlBindValue> &bindings )
{
    if ( job->statementText.isEmpty() ) {
        job->resultMessage = QObject::tr("Empty statement text");
        job->successfullyExecuted = false;
        return job;
    }

    if ( !job->query ) {
        job->resultMessage = QObject::tr("Empty job query");
        job->successfullyExecuted = false;
        return job;
    }

    job->successfullyExecuted = job->query->exec(job->statementText, false, bindings);
    if ( !job->successfullyExecuted ) {
        job->resultMessage = job->query->lastErrorText();
        return job;
    }

    if ( job->isSelectStatement ) {
        int rowIndex = 0;
        int colIndex = 0;
        int colCount = job->query->columnCount();
        QList <QVariant> headers;
        for (; colIndex < colCount; colIndex++) {
            headers << job->query->columnName(colIndex);
        }
        job->queryData->append(headers);
        while ( job->query->next() && rowIndex < 2000 ) {
            QList <QVariant> record;
            for (colIndex = 0; colIndex < colCount; colIndex++) {
                record << job->query->value(colIndex);
            }
            job->queryData->append(record);
            rowIndex++;
        }
    }
    else {
        int rowsCount = job->query->numRowsAffected();
        if ( !job->statementText.startsWith("INSERT", Qt::CaseInsensitive) &&
             !job->statementText.startsWith("UPDATE", Qt::CaseInsensitive) &&
             !job->statementText.startsWith("DELETE", Qt::CaseInsensitive) )
            rowsCount = -1;
        if ( rowsCount == -1 )
            job->resultMessage = QObject::tr("Executed successfully...");
        else
            job->resultMessage = QObject::tr("Executed successfully... %1 Rows Affected").arg(QString::number(rowsCount));
    }

    return job;
}

DbConnection::DbConnection( IDbLoader *loader_, BaseConnectionInfo *connectionInfo_,
                            IDbCatalogRootItem *rootItem , DbCatalogModel *model, Sql::SqlBindHelper *sqlHelper )
    : QObject(0)
    , m_loader(loader_)
    , m_transaction(false)
    , m_connectionInfo(connectionInfo_)
    , m_rootItem(rootItem)
    , m_model(model)
    , m_failSaveTransaction(false)
    , m_sqlHelper(sqlHelper)
{
    m_databaseName = m_connectionInfo->connectionName();
    m_connectionTitle = m_connectionInfo->connectionTitle();

    hasTransactionFeature = QSqlDatabase::database(m_databaseName).driver()->hasFeature(QSqlDriver::Transactions);

    connect(&m_jobItemFutureWatcher, SIGNAL(finished()), this, SLOT(onJobItemExecuted()));
}

bool DbConnection::closeConnection( QMainWindow *mainWindow )
{
    waitDetailsForFinished();

    if ( !noJobs() ) {
        Core::Storage::warning(QObject::tr("Connection cannot be Closed.\nThere are any executed Jobs!"));
        return false;
    }

    if ( !confirmEndStartedTransaction(mainWindow) )
        return false;

    emit connectionClosed(this);
    QSqlDatabase::removeDatabase(m_databaseName);
    return true;
}

bool DbConnection::confirmEndStartedTransaction( QMainWindow *mainWindow )
{
    if ( m_transaction ) {
        QMessageBox msgBox(mainWindow);
        msgBox.setText(QObject::tr("Connection %1 Has Started Transaction.").arg(m_connectionTitle));
        QPushButton *commitButton   = msgBox.addButton(QObject::tr("Commit"),   QMessageBox::ActionRole);
        QPushButton *rollbackButton = msgBox.addButton(QObject::tr("Rollback"), QMessageBox::ActionRole);
        QPushButton *cancelButton   = msgBox.addButton(QObject::tr("Cancel"),   QMessageBox::RejectRole);
        msgBox.setDefaultButton(commitButton);
        msgBox.setEscapeButton(cancelButton);

        msgBox.exec();

        if ( msgBox.clickedButton() == commitButton ) {
            if ( !QSqlDatabase::database(m_databaseName).commit() ) {
                Core::Storage::warning( tr("Error On Commit Transaction:\n%1")
                                        .arg(QSqlDatabase::database(m_databaseName).lastError().text()) );
                return true;
            }
        }
        else if ( msgBox.clickedButton() == rollbackButton ) {
            if ( !QSqlDatabase::database(m_databaseName).rollback() ) {
                Core::Storage::warning( tr("Error On Rollback Transaction:\n%1")
                                        .arg(QSqlDatabase::database(m_databaseName).lastError().text()) );
                return true;
            }
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

    if ( m_transaction )
        return false;

    m_transaction = QSqlDatabase::database(m_databaseName).transaction();
    return true;
}

bool DbConnection::commitTransaction()
{
    if ( !hasTransactionFeature )
        return false;

    if ( !m_transaction )
        return false;

    if ( QSqlDatabase::database(m_databaseName).commit() ) {
        m_transaction = false;
        return true;
    }

    Core::Storage::warning( tr("Error On Commit Transaction:\n%1")
                            .arg(QSqlDatabase::database(m_databaseName).lastError().text()) );
    return false;
}

bool DbConnection::rollbackTransaction()
{
    if ( !hasTransactionFeature )
        return false;

    if ( !m_transaction )
        return false;

    if ( QSqlDatabase::database(m_databaseName).rollback() ) {
        m_transaction = false;
        return true;
    }

    Core::Storage::warning( tr("Error On Rollback Transaction:\n%1")
                            .arg(QSqlDatabase::database(m_databaseName).lastError().text()) );
    return false;
}

QList<IDbCatalogItem *> *DbConnection::schemaObjects( const QString &schemaName, unsigned objectType )
{
    return m_rootItem->schemaObjects(objectType, schemaName, m_loader, m_connectionInfo);
}

QList<IDbCatalogItem *> *DbConnection::schemaObjectObjects( const QString &schemaName, const QString &objectName, unsigned objectType, unsigned objectSubtype )
{
    if ( IDbCatalogItem *objectList = m_rootItem->schemaObjectList(objectType, schemaName, m_loader, m_connectionInfo) ) {
        if ( IDbCatalogItem *table = objectList->childByNameWithLoad(objectName, m_loader, m_connectionInfo) ) {
            if ( IDbCatalogItem *subobjectList = table->child(objectSubtype, m_loader, m_connectionInfo) )
                return subobjectList->childrens(m_loader, m_connectionInfo);
        }
    }
    return 0;
}

QList<IDbCatalogItem *> *DbConnection::tableColumns( const QString &schemaName, const QString &tableName )
{
    return schemaObjectObjects( schemaName, tableName,
                                Sql::Constants::DbSchemaObjectType::C_TABLE,
                                Sql::Constants::DbSchemaObjectSubtype::C_TABLE_COLUMN );
}

QList<IDbCatalogItem *> *DbConnection::viewColumns( const QString &schemaName, const QString &tableName )
{
    return schemaObjectObjects( schemaName, tableName,
                                Sql::Constants::DbSchemaObjectType::C_VIEW,
                                Sql::Constants::DbSchemaObjectSubtype::C_VIEW_COLUMN );
}

QList<IDbCatalogItem *> *DbConnection::tableConstraints( const QString &schemaName, const QString &tableName )
{
    return schemaObjectObjects( schemaName, tableName,
                                Sql::Constants::DbSchemaObjectType::C_TABLE,
                                Sql::Constants::DbSchemaObjectSubtype::C_TABLE_CONSTRAINT );
}

QList<IDbCatalogItem *> *DbConnection::tableTriggers( const QString &schemaName, const QString &tableName )
{
    return schemaObjectObjects( schemaName, tableName,
                                Sql::Constants::DbSchemaObjectType::C_TABLE,
                                Sql::Constants::DbSchemaObjectSubtype::C_TABLE_TRIGGER );
}

QList<IDbCatalogItem *> *DbConnection::tableIndexes( const QString &schemaName, const QString &tableName )
{
    return schemaObjectObjects( schemaName, tableName,
                                Sql::Constants::DbSchemaObjectType::C_TABLE,
                                Sql::Constants::DbSchemaObjectSubtype::C_TABLE_INDEX );
}

bool DbConnection::noJobs() const
{
    return m_jobs.isEmpty();
}

void DbConnection::startNewJob( ExecuteType executeType )
{
    if ( m_databaseName.isEmpty() )
        return;

    IDocument *document = DocumentManager::getInstance()->currentDocument();
    if ( !document )
        return;

    ITextDocument *textDocument = dynamic_cast<ITextDocument *>(document);
    if ( !textDocument )
        return;

    TextEditor::TextEditorWidget *editor = qobject_cast<TextEditor::TextEditorWidget *>(DocumentManager::getInstance()->currentEditorWidget());
    if ( !editor ) // Is Not Text Editor
        return;

    DbJob *newJob = new DbJob(editor->document()->toPlainText(), executeType, textDocument);

    if ( newJob->statements.size() > 0 ) {
        QSet <QString> bindingsSet;
        for (int i = 0; i < newJob->statements.size(); i++)
            foreach ( const QString &bindName, newJob->statements[i]->bindNames )
                bindingsSet << bindName;
        if ( !m_sqlHelper->defineBindValues(bindingsSet, newJob->bindings) ) {
            delete newJob;
            return;
        }
        m_jobs.enqueue(newJob);
        int statemntCount = newJob->statements.size();
        newJob->futureInterface.setProgressRange(0, 0);
        newJob->futureInterface.reportStarted();
        wake();
        Core::Storage::mainWindow()->addTask( newJob->futureInterface.future(),
                                              tr("%1 Queries Executed").arg(statemntCount),
                                              "Query.Executing" );
    }
    else
        delete newJob;
}

void DbConnection::startQueryDataNewJob( const QString &schemaName, const QString &tableName )
{
    QString queryString = "SELECT * FROM ";
    if ( (m_loader->supportedTopLevel() == Database::IDbLoader::MultiSchema ||
          m_loader->supportedTopLevel() == Database::IDbLoader::DbMultiSchema) &&
         !schemaName.isEmpty() )
        queryString += schemaName.toUpper() + ".";
    queryString += tableName.toUpper();

    DbJob *newJob = new DbJob(queryString);

    if ( newJob->statements.size() > 0 ) {
        m_jobs.enqueue(newJob);
        int statementCount = newJob->statements.size();
        newJob->futureInterface.setProgressRange(0, 0);
        newJob->futureInterface.reportStarted();
        wake();
        Core::Storage::mainWindow()->addTask( newJob->futureInterface.future(),
                                              tr("%1 Queries Executed").arg(statementCount),
                                              "Query.Executing" );
    }
    else
        delete newJob;
}

void DbConnection::waitDetailsForFinished()
{
    if ( m_detailsFuture.isRunning() )
        m_detailsFuture.waitForFinished();
}

void DbConnection::requestDetails( IDbCatalogItem *item )
{
    emit detailsRequested(item, this);
}

void DbConnection::wake()
{
    if ( m_jobItemFuture.isRunning() )
        return;

    if ( m_jobs.isEmpty() )
        return;

    Sql::SqlStatement *statement = m_jobs[0]->statements[m_jobs[0]->itemsExecuted];

    m_failSaveTransaction = m_transaction;
    if ( isTransactionStatement(statement) )
        if ( !m_failSaveTransaction )
            startTransaction();

    if ( DbAccessor::Ptr accessor = connectionInfo()->accessor() ) {
        if ( DbQuery::Ptr query = accessor->query() ) {
            DbJobItem *newJobItem = new DbJobItem( query,
                                                   statement->isSelectStatement,
                                                   statement->isBlockStatement,
                                                   statement->isCommitStatement,
                                                   statement->isRollbackStatement,
                                                   statement->statementText );

            m_jobItemFuture = QtConcurrent::run(&executeJobItem, newJobItem, m_jobs[0]->bindings);
            m_jobItemFutureWatcher.setFuture(m_jobItemFuture);
        }
    }
}

bool DbConnection::isTransactionStatement( Sql::SqlStatement *statement )
{
    if ( statement->isSelectStatement )
        return false;

    if ( statement->isDdlStatement )
        return m_loader->isDdlStatementTransactionSupported();

    return true;
}

void DbConnection::onJobItemExecuted()
{
    DbJobItem *job = m_jobItemFuture.result();

    if ( !job->successfullyExecuted ) {
        if ( !m_failSaveTransaction )
            rollbackTransaction();
    }
    else {
        if ( job->isCommitStatement || job->isRollbackStatenent )
            m_transaction = false;
    }

    emit statementExecuted(job->statementText, job->successfullyExecuted, job->isSelectStatement, job->resultMessage, job->queryData);
    emit transactionChanged(this);

    job->clear();
    delete job;
    m_jobs[0]->itemsExecuted++;
    m_jobs[0]->futureInterface.setProgressValue(m_jobs[0]->itemsExecuted);
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    if ( m_jobs[0]->itemsExecuted == m_jobs[0]->statements.size() ) {
        m_jobs[0]->futureInterface.reportFinished();
        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        delete m_jobs.dequeue();
    }

    wake();
}

} // Database
