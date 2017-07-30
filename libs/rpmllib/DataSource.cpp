#include <QScopedPointer>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include "DataSource.h"
#include "DataSourceConnectParametersDialog.h"
#include "DbPostgresDataSourceProvider.h"
#include "DbSqliteDataSourceProvider.h"
#include "DbLinterDataSourceProvider.h"
#include "Core.h"
#include "DbDataSourceQueryEditDialog.h"
#include "InputParametersDialog.h"

namespace RPML {

DataSource::DataSource( int id, const QString &description, RPML::IDataSourceProvider *provider,
                        const QList<ConnectParameter> &connectParameters )
    : m_id(id)
    , m_description(description)
    , m_provider(provider)
    , m_connectParameters(connectParameters)
{
    m_query = m_provider->createDataSourceQuery();
    m_connectName = QString("DS_%1").arg((qlonglong)this);
}

DataSource::~DataSource()
{
    if ( m_query )
        delete m_query;
}

bool DataSource::editQuery( QList<ReportInputParameter *> *parameters, QString &errorMessage )
{
    if ( !m_query )
        return false;

    if ( !m_provider->connect(this, errorMessage) )
        return false;

    QScopedPointer<IDataSourceAssistor> assistor(m_provider->createDataSourceAssistor(this));

    bool edited = m_query->edit(parameters, m_connectName, assistor.data(), errorMessage);

    m_provider->disconnect(this);

    return edited;
}

DataSourceResult *DataSource::fetch( QList<ReportInputParameter *> *parameters,
                                     QList<InputParameterValue> &values,
                                     QString &errorMessage )
{
    if ( m_query ) {
        if ( !m_provider->connect(this, errorMessage) )
            return 0;
        DataSourceResult *result = m_query->fetch(this, parameters, values, errorMessage);
        m_provider->disconnect(this);
        return result;
    }

    errorMessage = "Empty Data Source Query" + m_id;
    qDebug() << "!TODO!" << errorMessage;
    return 0;

}

QStringList *DataSource::queryFields()
{
    return m_query ? m_query->fields() : 0;
}

QString IDataSourceQuery::toText() const
{
    return QString();
}

DataSourceManager::Ptr DataSourceManager::m_instance(new DataSourceManager());

DataSourceManager::DataSourceManager()
{
    loadDataSourceProviders();
}

DataSourceManager *DataSourceManager::instance()
{
    return m_instance.data();
}

DataSourceManager::~DataSourceManager()
{
    qDeleteAll(m_providers);
}

DataSource *DataSourceManager::createDataSource( int id )
{
    if ( !id )
        return 0;

    DataSourceManager *dataSourceManager = instance();
    if ( dataSourceManager->m_providers.isEmpty() )
        return 0;

    DataSourceConnectParametersDialog dsConnectParametersDialog(&dataSourceManager->m_providers, Core::Storage::mainWindow()->widget());
    if ( dsConnectParametersDialog.exec() == QDialog::Accepted ) {
        return new DataSource( id, QObject::tr("[Edit Me]"), dsConnectParametersDialog.selectedProvider(),
                               dsConnectParametersDialog.dataSourceConnectParameters() );
    }
    return 0;
}

IDataSourceProvider *DataSourceManager::providerForTypeId( int typeId )
{
    if ( typeId ) {
        DataSourceManager *dataSourceManager = instance();
        foreach ( IDataSourceProvider *provider, dataSourceManager->m_providers ) {
            if ( provider->typeId() == typeId )
                return provider;
        }
    }

    return 0;
}

bool DataSourceManager::fillInputParameterValues( QList<ReportInputParameter *> *parameters,
                                                  QList<InputParameterValue> &values,
                                                  QWidget *parent )
{
    values.clear();

    if ( parameters->isEmpty() )
        return true;

    InputParametersDialog paramsDialog(parameters, QObject::tr("Input Parameters"), parent);
    if ( paramsDialog.exec() == QDialog::Accepted ) {
        values = paramsDialog.values();
        return true;
    }

    return false;
}

void DataSourceManager::loadDataSourceProviders()
{
    m_providers.push_back(new DbPostgresDataSourceProvider());
    m_providers.push_back(new DbSqliteDataSourceProvider());
    m_providers.push_back(new DbLinterDataSourceProvider());
}

void DbDataSourceQuery::save( QDomDocument *document, QDomElement *dataSourceElement ) const
{
    QDomElement queryElement = document->createElement("query");
    QDomCDATASection queryData = document->createCDATASection(m_queryText);
    queryElement.appendChild(queryData);
    dataSourceElement->appendChild(queryElement);

    QDomElement fieldsElement = document->createElement("fields");
    for (int i = 0; i < m_fields.size(); i++) {
        QDomElement fieldElement = document->createElement("field");
        fieldElement.setAttribute("value", m_fields[i]);
        fieldsElement.appendChild(fieldElement);
    }
    queryElement.appendChild(fieldsElement);
}

bool DbDataSourceQuery::load( QDomElement *dataSourceElement )
{
    m_queryText = "";
    m_fields.clear();

    QDomElement queryElement = dataSourceElement->firstChildElement("query");

    if ( queryElement.isNull() )
        return true;

    m_queryText = queryElement.text().replace("\r\n", "\n");;

    QDomElement fieldsElement = queryElement.firstChildElement("fields");
    if ( !fieldsElement.isNull() ) {
        QDomElement fieldElement = fieldsElement.firstChildElement("field");
        while ( !fieldElement.isNull() ) {
            m_fields.push_back(fieldElement.attribute("value"));
            fieldElement = fieldElement.nextSiblingElement("field");
        }
    }

    return true;
}

bool DbDataSourceQuery::edit( QList<ReportInputParameter *> *parameters,
                              const QString &connectName,
                              IDataSourceAssistor *assistor,
                              QString &errorMessage )
{
    errorMessage.clear();

    DbDataSourceQueryEditDialog dbQueryDialog( parameters,
                                               connectName,
                                               assistor,
                                               m_queryText,
                                               Core::Storage::mainWindow()->widget() );

    bool edited = ( dbQueryDialog.exec() == QDialog::Accepted );

    if ( edited ) {
        m_queryText = dbQueryDialog.queryText();
        m_fields = dbQueryDialog.columnNames();
    }

    return edited;
}

DataSourceResult *DbDataSourceQuery::fetch( DataSource *source,
                                            QList<ReportInputParameter *> *parameters,
                                            QList<InputParameterValue> &values,
                                            QString &errorMessage ) const
{
    QString parseText = m_queryText.trimmed();

    QList<int> paramsList;

    {   // Romove MetaSQL comments
        int pos = 0;
        while ( (pos = parseText.indexOf("/*{{!", pos)) != -1 ) {
            int numPos = pos + 5;
            int paramId = 0;
            while ( parseText[numPos].isDigit() ) {
                paramId = paramId * 10 + parseText[numPos].toLatin1() - '0';
                numPos++;
            }
            if ( numPos ) {
                int paramIndex = -1;
                for (int i = 0; i < parameters->size(); i++) {
                    if ( parameters->at(i)->id == paramId ) {
                        paramIndex = i;
                        break;
                    }
                }
                if ( paramIndex != -1 ) {
                    if ( !values[paramIndex].disabled ) {
                        int endPos = parseText.indexOf("}}*/", numPos);
                        if ( endPos != -1 ) {
                            // Replace Comments With Spaces
                            int k;
                            for (k = pos; k <= numPos; k++)
                                parseText[k] = ' ';
                            for (k = 0, pos = endPos; k < 4; k++, pos++)
                                parseText[pos] = ' ';
                            paramsList << paramId;
                            continue;
                        }
                    }
                }
            }

            // Remove Comments;
            int endPos = parseText.indexOf("}}*/", numPos);
            if ( endPos == -1 ) {
                parseText.remove(pos, -1);
                break;
            }
            parseText.remove(pos, endPos + 5 - pos);
        }
    }

    QSqlQuery query(QSqlDatabase::database(source->connectName()));
    query.prepare(parseText);
    for (int i = 0; i < parameters->size(); i++) {
        int pid = parameters->at(i)->id;
        if ( !values[i].disabled && paramsList.contains(pid) ) {
            if ( parameters->at(i)->type == ReportInputParameter::Numeric )
                query.bindValue(QString(":P%1").arg(pid), values[i].value.toInt());
            else
                query.bindValue(QString(":P%1").arg(pid), values[i].value.toString());
        }
    }

    query.setForwardOnly(true);
    if ( !query.exec() ) {
        errorMessage = query.lastError().text();
        return 0;
    }

    DataSourceResult *result = new DataSourceResult();

    int i = 0;
    int colCount = query.record().count();
    QVariantList *emptyRow = new QVariantList();
    for (; i < colCount; i++)
        *emptyRow << QVariant();
    result->m_data.push_back(emptyRow);

    while ( query.next() ) {
        QVariantList *row = new QVariantList();
        for (i = 0; i < colCount; i++)
            *row << query.value(i);
        result->m_data.push_back(row);
    }

    return result;
}

DataSourcesModel::DataSourcesModel( QList<DataSource *> &sources, QObject *parent )
    : QAbstractTableModel(parent)
    , m_sources(sources)
{

}

int DataSourcesModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return m_sources.size();
}

int DataSourcesModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return 3;
}

Qt::ItemFlags DataSourcesModel::flags( const QModelIndex &index ) const
{
    if ( !index.isValid() )
        return Qt::ItemIsEnabled;

    return ( index.column() == 2 ?
                 Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable :
                 Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

QVariant DataSourcesModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    if ( index.row() < 0 || index.row() >= m_sources.size() )
        return QVariant();

    if ( index.column() < 0 || index.column() >= columnCount() )
        return QVariant();

    if ( role == Qt::EditRole || role == Qt::DisplayRole) {
        switch ( index.column() ) {
            case 0: return m_sources[index.row()]->m_id;
            case 1: return m_sources[index.row()]->m_provider->typeDescription();
            case 2: return m_sources[index.row()]->m_description;
        }
    }

    return QVariant();
}

QVariant DataSourcesModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( orientation == Qt::Horizontal && role == Qt::DisplayRole ) {
        switch ( section ) {
            case 0: return tr("Id");
            case 1: return tr("Type");
            case 2: return tr("Description");
        }
    }

    return QVariant();
}

bool DataSourcesModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
    if ( index.isValid() && index.column() == 2 && role == Qt::EditRole ) {
        QString strValue = value.toString().trimmed();
        if ( m_sources[index.row()]->m_description != strValue ) {
            m_sources[index.row()]->m_description = strValue;
            emit dataChanged(index, index);
            return true;
        }
    }

    return false;
}

void DataSourcesModel::startResetModel()
{
    beginResetModel();
}

void DataSourcesModel::finishResetModel()
{
    endResetModel();
}

void DataSourcesModel::addDataSource( DataSource *dataSource )
{
    int pos = m_sources.size();

    beginInsertRows(QModelIndex(), pos, pos);
    m_sources.push_back(dataSource);
    endInsertRows();
}

void DataSourcesModel::delDataSource( int index )
{
    DataSource *delData = m_sources[index];

    beginRemoveRows(QModelIndex(), index, index);
    m_sources.removeAt(index);
    endRemoveRows();

    delete delData;
}

}   // namespace RPML
