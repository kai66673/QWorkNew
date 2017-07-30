#ifndef DBQUERYMODEL_H
#define DBQUERYMODEL_H

#include <QAbstractTableModel>
#include <QVariant>
#include <QList>
#include <QColor>

namespace Database {

class DbQueryModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    DbQueryModel( QList <QList <QVariant> > *queryData_, QObject *parent = 0 );
    virtual ~DbQueryModel();

    int rowCount( const QModelIndex &parent ) const;
    int columnCount( const QModelIndex &parent ) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data( const QModelIndex &index, int role ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;

private:
    QList <QList <QVariant> > *queryData;

    bool alternatingRows;
    bool showNulls;
    QColor backgroundColor1;
    QColor backgroundColor2;
    QColor foregroundColor;
};

} // Database

#endif // DBQUERYMODEL_H
