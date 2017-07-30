#include <QStringList>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <QDebug>

#include "QRdbQueryModel.h"
#include "SettingsManager.h"
#include "dbbrowser_constants.h"
#include "SqlBindHelper.h"

QRdbQueryModel::QRdbQueryModel( const QString &connName, const QString &qText, QStringList *bindNames_, QMap<QString, Sql::SqlBindValue> *bindings_, QObject *parent )
    : QAbstractTableModel(parent)
    , connectionName(connName)
    , queryText(qText)
    , bindNames(bindNames_)
    , bindings(bindings_)
{
    setupModel();

    SettingsManager *settingManager = SettingsManager::instance();

    alternatingRows  = settingManager->value(SqlBrowser::C_SQL_QUERY_SETTINGS_ID, "AlternatingRows").toBool();
    showNulls        = settingManager->value(SqlBrowser::C_SQL_QUERY_SETTINGS_ID, "ShowNull").toBool();
    backgroundColor1 = qvariant_cast<QColor>(settingManager->value(SqlBrowser::C_SQL_QUERY_SETTINGS_ID, "BackgroundColor1"));
    backgroundColor2 = qvariant_cast<QColor>(settingManager->value(SqlBrowser::C_SQL_QUERY_SETTINGS_ID, "BackgroundColor2"));
    foregroundColor  = qvariant_cast<QColor>(settingManager->value(SqlBrowser::C_SQL_QUERY_SETTINGS_ID, "ForegroundColor"));
}

int QRdbQueryModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent);
    return queryData.size();
}

int QRdbQueryModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent);
    return headers.size();
}

Qt::ItemFlags QRdbQueryModel::flags(const QModelIndex &index) const
{
    if ( !index.isValid() )
        return Qt::ItemIsEnabled;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant QRdbQueryModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
        return QVariant();

    if ( index.row() < 0 || index.row() >= queryData.size() )
        return QVariant();

    if ( index.column() < 0 || index.column() >= headers.size() )
        return QVariant();

    if ( role == Qt::ForegroundRole )
        return foregroundColor;

    if ( role == Qt::BackgroundRole ) {
        if ( (index.row() % 2) && alternatingRows )
            return backgroundColor2;
        else
            return backgroundColor1;
    }

    if ( role == Qt::DisplayRole || role == Qt::EditRole ) {
        if ( queryData[index.row()][index.column()].isNull() && showNulls )
            return QString("{null}");
        return queryData[index.row()][index.column()];
    }

    return QVariant();
}

QVariant QRdbQueryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( role != Qt::DisplayRole )
        return QVariant();

    if ( orientation == Qt::Horizontal) {
        return headers[section];
    }

    if ( orientation == Qt::Vertical ) {
        return (section + 1);
    }

    return QVariant();
}

void QRdbQueryModel::setupModel()
{
    hasError = true;

    QSqlDatabase db = QSqlDatabase::database(connectionName);

    if ( !db.isOpen() ) {
        lastError = tr("Database connection is not opened.");
        return;
    }

    QSet<QString> bindNamesSet;
    for (int i = 0; i < bindNames->size(); i++)
        bindNamesSet << bindNames->at(i);

    QSqlQuery query(db);
    query.setForwardOnly(true);

    bool queryResult;

    if ( !bindNamesSet.isEmpty() ) {
        query.prepare(queryText);
        foreach ( QString bindName, bindNamesSet ) {
            if ( (*bindings)[bindName].type == 0 )
                query.bindValue(bindName, (*bindings)[bindName].value.toInt());
            else
                query.bindValue(bindName, (*bindings)[bindName].value);
        }
        queryResult = query.exec();
    }
    else {
        queryResult = query.exec(queryText);
    }
    if ( !queryResult ) {
        lastError = query.lastError().text();
        return;
    }

    int colIndex = 0;
    int colCount = query.record().count();
    for (; colIndex < colCount; colIndex++) {
        headers << query.record().fieldName(colIndex);
    }

    const int maxRowCount = SettingsManager::instance()->value(SqlBrowser::C_SQL_QUERY_SETTINGS_ID, "MaxRowsCount").toInt();

    int rowIndex = 0;
    while ( query.next() && rowIndex < maxRowCount ) {
        QList <QVariant> record;
        for (colIndex = 0; colIndex < colCount; colIndex++) {
            record << query.value(colIndex);
        }
        queryData << record;
        rowIndex++;
    }

    hasError = false;
}
