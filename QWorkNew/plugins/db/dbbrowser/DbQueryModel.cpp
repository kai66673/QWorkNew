#include "DbQueryModel.h"
#include "dbbrowser_constants.h"
#include "SettingsManager.h"

namespace Database {

DbQueryModel::DbQueryModel( QList<QList<QVariant> > *queryData_, QObject *parent )
    : QAbstractTableModel(parent)
    , queryData(queryData_)
{
    SettingsManager *settingManager = SettingsManager::instance();

    alternatingRows  = settingManager->value(SqlBrowser::C_SQL_QUERY_SETTINGS_ID, "AlternatingRows").toBool();
    showNulls        = settingManager->value(SqlBrowser::C_SQL_QUERY_SETTINGS_ID, "ShowNull").toBool();
    backgroundColor1 = qvariant_cast<QColor>(settingManager->value(SqlBrowser::C_SQL_QUERY_SETTINGS_ID, "BackgroundColor1"));
    backgroundColor2 = qvariant_cast<QColor>(settingManager->value(SqlBrowser::C_SQL_QUERY_SETTINGS_ID, "BackgroundColor2"));
    foregroundColor  = qvariant_cast<QColor>(settingManager->value(SqlBrowser::C_SQL_QUERY_SETTINGS_ID, "ForegroundColor"));
}

DbQueryModel::~DbQueryModel()
{
    delete queryData;
}

int DbQueryModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent);
    return queryData->size() - 1;
}

int DbQueryModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent);
    return queryData->at(0).size();
}

Qt::ItemFlags DbQueryModel::flags(const QModelIndex &index) const
{
    if ( !index.isValid() )
        return Qt::ItemIsEnabled;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant DbQueryModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
        return QVariant();

    if ( index.row() < 0 || index.row() >= queryData->size() - 1 )
        return QVariant();

    if ( index.column() < 0 || index.column() >= queryData->at(0).size() )
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
        if ( (*queryData)[index.row()+1][index.column()].isNull() && showNulls )
            return QString("{null}");
        return (*queryData)[index.row()+1][index.column()];
    }

    return QVariant();
}

QVariant DbQueryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( role != Qt::DisplayRole )
        return QVariant();

    if ( orientation == Qt::Horizontal) {
        return queryData->at(0).at(section);
    }

    if ( orientation == Qt::Vertical ) {
        return (section + 1);
    }

    return QVariant();
}

} // Database
