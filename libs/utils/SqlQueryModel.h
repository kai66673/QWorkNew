#ifndef SQLQUERYMODEL_H
#define SQLQUERYMODEL_H

#include <QAbstractTableModel>
#include <QSqlQuery>
#include <QStringList>
#include <QVariantList>

#include "utils_global.h"

namespace Utils {

class QTCREATOR_UTILS_EXPORT SqlQueryModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit SqlQueryModel( QObject *parent = 0 );
    
    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const;
    Qt::ItemFlags flags( const QModelIndex &index ) const;
    QVariant data( const QModelIndex &index, int role ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;


    bool setQuery( QSqlQuery *query, int maxRowsCount = 2000 );
    QString errorDescription() const { return m_errorDescription; }

private:
    QList <QVariantList> m_data;
    QStringList m_headers;
    QString m_errorDescription;
};

}   // namespace Utils

#endif // SQLQUERYMODEL_H
