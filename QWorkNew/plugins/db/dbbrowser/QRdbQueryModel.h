#ifndef QRDBQUERYMODEL_H
#define QRDBQUERYMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QVariant>
#include <QColor>

namespace Sql {
    struct SqlBindValue;
}

class QRdbQueryModel : public QAbstractTableModel
{
public:
    QRdbQueryModel( const QString &connName, const QString &qText, QStringList *bindNames_, QMap<QString, Sql::SqlBindValue> *bindings_, QObject *parent = 0 );

    int rowCount( const QModelIndex &parent ) const;
    int columnCount( const QModelIndex &parent ) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data( const QModelIndex &index, int role ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;

    QString getLastError() const { return lastError; }
    bool isHasError() const { return hasError; }

private:
    void setupModel();

    QString connectionName;
    QString queryText;

    QStringList headers;
    QList <QList <QVariant> > queryData;

    bool hasError;
    QString lastError;

    bool alternatingRows;
    bool showNulls;
    QColor backgroundColor1;
    QColor backgroundColor2;
    QColor foregroundColor;

    QStringList *bindNames;
    QMap<QString, Sql::SqlBindValue> *bindings;
};

#endif // QRDBQUERYMODEL_H
