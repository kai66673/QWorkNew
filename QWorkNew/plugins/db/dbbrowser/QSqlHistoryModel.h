#ifndef QSQLHISTORYMODEL_H
#define QSQLHISTORYMODEL_H

#include <QAbstractTableModel>
#include <QDateTime>

struct SqlHistoryInfo {
    SqlHistoryInfo( const QString &text_, const QDateTime &date_ )
        : text(text_)
        , date(date_)
    {}

    QString     text;
    QDateTime   date;
};

class SqlHistoryInfoList : public QObject
{
public:
    SqlHistoryInfoList( const QString &fileName_, QObject *parent = 0 )
        : QObject(parent)
        , fileName(fileName_)
    {
        load();
    }

    ~SqlHistoryInfoList() { save(); items.clear(); }

    SqlHistoryInfo &operator [] ( int index ) { return items[index]; }
    int  size() { return items.size(); }
    void clear() { items.clear(); }

    void remove( int index ) { items.removeAt(index); }
    void append() {
        SqlHistoryInfo sqlInfo("", QDateTime::currentDateTime());
        items.append(sqlInfo);
    }

private:
    void load();
    void save();

    QString fileName;
    QList <SqlHistoryInfo> items;
};

class QSqlHistoryModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit QSqlHistoryModel( SqlHistoryInfoList *sqlList_, QObject *parent = 0 );
    
    int rowCount( const QModelIndex &parent ) const;
    int columnCount( const QModelIndex &parent ) const;
    QVariant data( const QModelIndex &index, int role ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex &index ) const;

    bool removeRows( int position, int rows, const QModelIndex &parent = QModelIndex() );
    bool insertRows( int position, int rows, const QModelIndex &parent = QModelIndex() );

public slots:
    void addSql( const QString &text );

private:
    SqlHistoryInfoList *sqlList;
};

#endif // QSQLHISTORYMODEL_H
