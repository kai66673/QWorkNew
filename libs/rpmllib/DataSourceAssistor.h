#ifndef DATASOURCEASSISTOR_H
#define DATASOURCEASSISTOR_H

#include <QList>
#include <QString>
#include <QAbstractItemModel>
#include <QMimeData>

namespace RPML {

struct DataSourceObjectItem
{
    DataSourceObjectItem( int type_, const QString &name_, bool expandable_, bool selectable_, DataSourceObjectItem *parent_ )
        : type(type_)
        , name(name_)
        , expandable(expandable_)
        , selectable(selectable_)
        , parent(parent_)
    {}

    int type;
    QString name;
    bool expandable;
    bool selectable;

    DataSourceObjectItem *parent;
    QList<DataSourceObjectItem *> childrens;
};

class IDataSourceAssistor
{
public:
    IDataSourceAssistor();
    virtual ~IDataSourceAssistor();

    virtual void loadChildrens( DataSourceObjectItem *parent ) = 0;
    virtual QString iconName( int type ) const = 0;
    virtual QMimeData *mimeData( QList<DataSourceObjectItem *> &items );
};

class DataSourceObjectModel: public QAbstractItemModel
{
public:
    DataSourceObjectModel( IDataSourceAssistor *assistor, QObject *parent = 0 );
    virtual ~DataSourceObjectModel();

    virtual QVariant data( const QModelIndex &index, int role) const;
    virtual Qt::ItemFlags flags( const QModelIndex &index ) const;
    virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    virtual QModelIndex index( int row, int column, const QModelIndex &parent = QModelIndex() ) const;
    virtual QModelIndex parent( const QModelIndex &index ) const;
    virtual int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    virtual int columnCount( const QModelIndex &parent = QModelIndex() ) const;
    virtual bool hasChildren ( const QModelIndex & parent ) const;
    virtual bool canFetchMore( const QModelIndex & parent ) const;
    virtual void fetchMore( const QModelIndex & parent );

    virtual QMimeData *mimeData( const QModelIndexList &indexes ) const;

private:
    void setupModel();

    IDataSourceAssistor *m_assistor;
    DataSourceObjectItem *m_rootItem;
};

}   // namespace RPML

#endif // DATASOURCEASSISTOR_H
