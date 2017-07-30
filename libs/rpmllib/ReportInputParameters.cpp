#include <QComboBox>
#include <QLineEdit>
#include <QDate>

#include "ReportInputParameters.h"

namespace RPML {

ReportInputParameterTableView::ReportInputParameterTableView( QWidget *parent )
    : QTableView(parent)
{
}

void ReportInputParameterTableView::mouseDoubleClickEvent( QMouseEvent *event )
{
    Q_UNUSED(event)
    QModelIndex index = currentIndex();
    if ( index.isValid() ) {
        int row = index.row();
        if ( row != -1 )
            emit insertInputParameter(row);
    }
}

ReportInputParameterDelegate::ReportInputParameterDelegate( QObject *parent )
    : QItemDelegate(parent)
{
}

QWidget *ReportInputParameterDelegate::createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    Q_UNUSED(option)

    switch ( index.column() ) {
        case 2: {
            QComboBox *combo = new QComboBox(parent);
            combo->addItem("String");
            combo->addItem("Numeric");
            combo->addItem("Date");
            combo->addItem("Time");
            combo->addItem("Date/Time");
            return combo;
        }
        case 3: {
            QComboBox *combo = new QComboBox(parent);
            combo->addItem("Required");
            combo->addItem("Optional");
            return combo;
        }
    }

    return new QLineEdit(parent);
}

void ReportInputParameterDelegate::setEditorData( QWidget *editor, const QModelIndex &index ) const
{
    switch ( index.column() ) {
        case 2:
        case 3: {
            QComboBox *combo = static_cast<QComboBox *>(editor);
            int comboIndex = index.model()->data(index, Qt::EditRole).toInt();
            combo->setCurrentIndex(comboIndex);
            return;
        }
    }

    QString editValue = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit *edit = static_cast<QLineEdit *>(editor);
    edit->setText(editValue);
}

void ReportInputParameterDelegate::setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
    switch ( index.column() ) {
        case 2:
        case 3: {
            QComboBox *combo = static_cast<QComboBox *>(editor);
            int value = combo->currentIndex();
            model->setData(index, value, Qt::EditRole);
            return;
        }
    }
    QLineEdit *edit = static_cast<QLineEdit *>(editor);
    model->setData(index, edit->text(), Qt::EditRole);
}

void ReportInputParameterDelegate::updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    Q_UNUSED(index)
    editor->setGeometry(option.rect);
}

ReportInputParametersModel::ReportInputParametersModel( QList<ReportInputParameter *> &parameters, QObject *parent )
    : QAbstractTableModel(parent)
    , m_parameters(parameters)
{
}

int ReportInputParametersModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return m_parameters.size();
}

int ReportInputParametersModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return 5;
}

Qt::ItemFlags ReportInputParametersModel::flags( const QModelIndex &index ) const
{
    if ( !index.isValid() )
        return Qt::ItemIsEnabled;

    return ( index.column() != 0 ?
                 Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable :
                 Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

QVariant ReportInputParametersModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    if ( index.row() < 0 || index.row() >= m_parameters.size() )
        return QVariant();

    if ( index.column() < 0 || index.column() >= columnCount() )
        return QVariant();

    if ( role == Qt::EditRole ) {
        switch ( index.column() ) {
            case 0: return m_parameters[index.row()]->id;
            case 1: return m_parameters[index.row()]->description;
            case 2: return m_parameters[index.row()]->type;
            case 3: return m_parameters[index.row()]->mandatory;
            case 4: return m_parameters[index.row()]->defaultValue;
        }
    } else if ( role == Qt::DisplayRole ) {
        switch ( index.column() ) {
            case 0: return m_parameters[index.row()]->id;
            case 1: return m_parameters[index.row()]->description;
            case 2: return typeDescription(m_parameters[index.row()]->type);
            case 3: return mandatoryDescription(m_parameters[index.row()]->mandatory);
            case 4: return m_parameters[index.row()]->defaultValue;
        }
    }

    return QVariant();
}

QVariant ReportInputParametersModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( orientation == Qt::Horizontal && role == Qt::DisplayRole ) {
        switch ( section ) {
            case 0: return tr("Id");
            case 1: return tr("Description");
            case 2: return tr("Type");
            case 3: return tr("Mandatory");
            case 4: return tr("Default Value");
        }
    }

    return QVariant();
}

bool ReportInputParametersModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
    if ( !index.isValid() )
        return false;

    if ( index.row() >= m_parameters.size() || index.row() < 0 )
        return false;

    if ( role != Qt::EditRole && role != Qt::DisplayRole )
        return false;

    if ( index.column() < 0 || index.column() > columnCount() )
        return false;

    switch (index.column()) {
        case 1: {
            QString valStr = value.toString().trimmed();
            if ( m_parameters[index.row()]->description == valStr )
                return false;
            m_parameters[index.row()]->description = value.toString().trimmed();
            break;
        }
        case 2: {
            int valInt = value.toInt();
            if ( m_parameters[index.row()]->type == valInt )
                return false;
            m_parameters[index.row()]->type = valInt;
            if ( !checkDefaultValue(valInt, m_parameters[index.row()]->defaultValue.toString() ) )
                clearDefaultValue(index.row());
            break;
        }
        case 3:
            if ( m_parameters[index.row()]->mandatory == value.toInt() )
                return false;
            m_parameters[index.row()]->mandatory = value.toInt();
            break;
        case 4: {
            QString valString = value.toString().trimmed();
            if ( !checkDefaultValue(m_parameters[index.row()]->type, valString) )
                return false;
            if ( m_parameters[index.row()]->defaultValue == valString )
                return false;
            m_parameters[index.row()]->defaultValue = valString;
            break;
        }
    }

    emit dataChanged(index, index);
    return true;
}

void ReportInputParametersModel::startResetModel()
{
    beginResetModel();
}

void ReportInputParametersModel::finishResetModel()
{
    endResetModel();
}

void ReportInputParametersModel::addInputParameter()
{
    int id = 0;
    if ( !m_parameters.isEmpty() ) {
        id = m_parameters[0]->id;
        for (int i = 1; i < m_parameters.size(); i++)
            if ( id < m_parameters[i]->id )
                id = m_parameters[i]->id;
    }
    id++;

    QString description = tr("Parameter %1").arg(id);

    int count = m_parameters.size();
    beginInsertRows(QModelIndex(), count, count);
    m_parameters.push_back(new ReportInputParameter(id, description));
    endInsertRows();
}

void ReportInputParametersModel::delInputParameter( int row )
{
    ReportInputParameter *delPar = m_parameters[row];

    beginRemoveRows(QModelIndex(), row, row);
    m_parameters.removeAt(row);
    endRemoveRows();

    delete delPar;
}

QString ReportInputParametersModel::typeDescription( int t ) const
{
    switch ( t ) {
        case ReportInputParameter::String:      return tr("String");
        case ReportInputParameter::Numeric:     return tr("Numeric");
        case ReportInputParameter::Date:        return tr("Date");
        case ReportInputParameter::Time:        return tr("Time");
        case ReportInputParameter::DateTime:    return tr("Date/Time");
    }

    return QString();
}

QString ReportInputParametersModel::mandatoryDescription( int m ) const
{
    switch ( m ) {
        case ReportInputParameter::Required:    return tr("Required");
        case ReportInputParameter::Optional:    return tr("Optional");
    }

    return QString();
}

bool ReportInputParametersModel::checkDefaultValue( int type, const QString &valString ) const
{
    if ( !valString.isEmpty() ) {
        switch ( type ) {
            case ReportInputParameter::Numeric: {
                bool ok;
                valString.toDouble(&ok);
                if ( !ok )
                    return false;
                break;
            }
            case ReportInputParameter::Date: {
                QDate d = QDate::fromString(valString, "dd.MM.yyyy");
                if ( !d.isValid() )
                    return false;
                break;
            }
            case ReportInputParameter::Time: {
                QTime t = QTime::fromString(valString, "hh-mm-ss");
                if ( !t.isValid() )
                    return false;
                break;
            }
            case ReportInputParameter::DateTime: {
                QDateTime dt = QDateTime::fromString(valString, "dd.MM.yyyy hh-mm-ss");
                if ( !dt.isValid() )
                    return false;
                break;
            }
        }
    }

    return true;
}

void ReportInputParametersModel::clearDefaultValue( int row )
{
    setData(index(row, 4), QVariant(), Qt::EditRole);
}

}   // namespace RPML
