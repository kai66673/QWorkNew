#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QString>
#include <QList>
#include <QWidget>
#include <QDomDocument>
#include <QDomElement>
#include <QAbstractTableModel>
#include <QScopedPointer>

#include "ReportInputParameters.h"
#include "DataSourceAssistor.h"

namespace RPML {

class IDataSourceProvider;
class IDataSourceQuery;
class ReportData;
class DataSourcesModel;
class ReportDesignerWidget;
class DataSourceResult;
class ReportSelectDataSourceColumnDialog;
class ReportDataTableDialog;

class DataSource
{
    friend class ReportData;
    friend class ReportSelectDataSourceColumnDialog;
    friend class ReportDataTableDialog;
    friend class DataSourcesModel;
    friend class ReportDesignerWidget;
    friend class IDataSourceQuery;

public:
    struct ConnectParameter {
        ConnectParameter( const QString &name_, const QString &value_ )
            : name(name_)
            , value(value_)
        {}

        QString name;
        QString value;
    };

    DataSource( int id, const QString &description, IDataSourceProvider *provider,
                const QList<ConnectParameter> &connectParameters );

    virtual ~DataSource();

    bool editQuery( QList<ReportInputParameter *> *parameters, QString &errorMessage );
    DataSourceResult *fetch( QList<ReportInputParameter *> *parameters,
                             QList<InputParameterValue> &values,
                             QString &errorMessage );

    inline QString connectName() const { return m_connectName; }
    inline QList<ConnectParameter> *connectParameters() { return &m_connectParameters; }

    QStringList *queryFields();

private:
    int m_id;
    QString m_description;
    IDataSourceProvider *m_provider;
    QList<ConnectParameter> m_connectParameters;
    IDataSourceQuery *m_query;

    QString m_connectName;
};

class IDataSourceConnectWidget: public QWidget
{
public:
    IDataSourceConnectWidget( QWidget *parent = 0 )
        : QWidget(parent)
    {}

    virtual bool checkFillConnectParameters( QList<DataSource::ConnectParameter> &connectParameters ) = 0;
};

class DataSourceResult
{
public:
    DataSourceResult() {}
    virtual ~DataSourceResult() {
        qDeleteAll(m_data);
    }

    QList<QVariantList *> m_data;
};

class IDataSourceQuery
{
public:
    virtual ~IDataSourceQuery() {}

    virtual void save( QDomDocument *document, QDomElement *dataSourceElement ) const = 0;
    virtual bool load( QDomElement *dataSourceElement ) = 0;
    virtual QString toText() const;
    virtual QStringList *fields() = 0;
    virtual bool edit( QList<ReportInputParameter *> *parameters,
                       const QString &connectName,
                       IDataSourceAssistor *assistor,
                       QString &errorMessage ) = 0;

    virtual DataSourceResult *fetch( DataSource *source,
                                     QList<ReportInputParameter *> *parameters,
                                     QList<InputParameterValue> &values,
                                     QString &errorMessage ) const = 0;
};

class DbDataSourceQuery: public IDataSourceQuery
{
public:
    DbDataSourceQuery()
        : m_queryText("")
    {}

    virtual void save( QDomDocument *document, QDomElement *dataSourceElement ) const;
    virtual bool load( QDomElement *dataSourceElement );

    virtual QString toText() const { return m_queryText; }
    virtual QStringList *fields() { return &m_fields; }
    virtual bool edit( QList<ReportInputParameter *> *parameters,
                       const QString &connectName,
                       IDataSourceAssistor *assistor,
                       QString &errorMessage );

    virtual DataSourceResult *fetch( DataSource *source,
                                     QList<ReportInputParameter *> *parameters,
                                     QList<InputParameterValue> &values,
                                     QString &errorMessage ) const;
private:
    QString m_queryText;
    QStringList m_fields;
};

class IDataSourceProvider
{
public:
    virtual ~IDataSourceProvider() {}

    virtual int typeId() const = 0;
    virtual QString typeDescription() const = 0;
    virtual IDataSourceConnectWidget *createConnectWidget( QWidget *parent = 0 ) const = 0;
    virtual IDataSourceQuery *createDataSourceQuery() const = 0;

    virtual bool connect( DataSource *source, QString &errorMessage ) const = 0;
    virtual void disconnect( DataSource *source ) const = 0;

    virtual IDataSourceAssistor *createDataSourceAssistor( DataSource * /*source*/ ) const { return 0; }
};

class DataSourceManager
{
    typedef QScopedPointer<DataSourceManager> Ptr;
    DataSourceManager();
    static Ptr m_instance;

public:
    static DataSourceManager *instance();
    virtual ~DataSourceManager();

    static DataSource *createDataSource( int id );
    static IDataSourceProvider *providerForTypeId( int typeId );
    static bool fillInputParameterValues( QList<ReportInputParameter *> *parameters,
                                          QList<InputParameterValue> &values,
                                          QWidget *parent );

private:
    void loadDataSourceProviders();
    QList<IDataSourceProvider *> m_providers;
};

class DataSourcesModel: public QAbstractTableModel
{
public:
    DataSourcesModel( QList<DataSource *> &sources, QObject *parent = 0 );

    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;

    bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole );

    void startResetModel();
    void finishResetModel();

    void addDataSource( DataSource *dataSource );
    void delDataSource( int index );

private:
    QList<DataSource *> &m_sources;
};

}   // namespace RPML

#endif // DATASOURCE_H
