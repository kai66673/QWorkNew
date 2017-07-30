#include <QSqlError>
#include <QSqlRecord>

#include "SqlQueryModel.h"

namespace Utils {

SqlQueryModel::SqlQueryModel( QObject *parent )
    : QAbstractTableModel( parent )
    , m_errorDescription("")
{
}

int SqlQueryModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return m_data.size();
}

int SqlQueryModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return m_headers.size();
}

Qt::ItemFlags SqlQueryModel::flags( const QModelIndex &index ) const
{
    if ( !index.isValid() )
        return Qt::ItemIsEnabled;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant SqlQueryModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    if ( index.column() < 0 || index.column() >= columnCount() )
        return QVariant();

    if ( index.row() < 0 || index.row() >= rowCount() )
        return QVariant();

    if ( role == Qt::DisplayRole )
        return m_data[index.row()][index.column()];

    return QVariant();
}

QVariant SqlQueryModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( role == Qt::DisplayRole && orientation == Qt::Horizontal &&
         section >= 0 && section < columnCount() ) {
        return m_headers[section];
    }

    return QVariant();
}

bool SqlQueryModel::setQuery( QSqlQuery *query , int maxRowsCount )
{
    if ( maxRowsCount <= 0 )
        maxRowsCount = 2000;

    query->setForwardOnly(true);
    if ( !query->exec() ) {
        m_errorDescription = query->lastError().text();
        return false;
    }

    int colCount = query->record().count();
    for (int i = 0; i < colCount; i++)
        m_headers << query->record().fieldName(i);

    beginResetModel();

    m_data.clear();
    int rowCount = 0;
    while ( query->next() && rowCount < maxRowsCount ) {
        QVariantList record;
        for (int i = 0; i < colCount; i++)
            record << query->value(i);
        m_data << record;
        rowCount++;
    }

    endResetModel();

    return true;
}

}   // namespace Utils
