#include <QHeaderView>
#include <QLineEdit>
#include <QComboBox>

#include "QwGuiPythonModule.h"
#include "Core.h"

namespace Python {

QwItemDelegate::QwItemDelegate( QList<HeaderInfo> *headers, QList<QList<QVariant> > *data, QObject *parent )
    : QItemDelegate(parent)
    , m_headers(headers)
    , m_data(data)
{
}

QWidget *QwItemDelegate::createEditor( QWidget *parent, const QStyleOptionViewItem &option,
                                       const QModelIndex &index ) const
{
    Q_UNUSED(option)

    int type = m_headers->at(index.column()).type;
    if ( type == 2 || type == 3 ) {
        QComboBox *combo = new QComboBox(parent);
        if ( type == 3 )
            combo->setEditable(true);
        combo->addItems(m_headers->at(index.column()).comboValues);
        return combo;
    }

    return new QLineEdit(parent);
}

void QwItemDelegate::setEditorData( QWidget *editor, const QModelIndex &index ) const
{
    int type = m_headers->at(index.column()).type;
    if ( type == 2 || type == 3 ) {
        QComboBox *combo = static_cast<QComboBox *>(editor);
        if ( type == 3 )
            combo->lineEdit()->setText((*m_data)[index.row()][index.column()].toString().toUpper());
        else
            combo->setCurrentIndex(combo->findText((*m_data)[index.row()][index.column()].toString()));
    }
    else {
        QLineEdit *edit = static_cast<QLineEdit *>(editor);
        edit->setText((*m_data)[index.row()][index.column()].toString());
    }
}

void QwItemDelegate::setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
    int type = m_headers->at(index.column()).type;
    if ( type == 2 || type == 3 ) {
        QComboBox *combo = static_cast<QComboBox *>(editor);
        if ( type == 3 )
            model->setData(index, combo->lineEdit()->text().toUpper().trimmed());
        else
            model->setData(index, combo->currentText().toUpper().trimmed());
    }
    else {
        QLineEdit *edit = static_cast<QLineEdit *>(editor);
        model->setData(index, edit->text());
    }
}

void QwItemDelegate::updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option,
                                           const QModelIndex &index ) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

QwTableModel::QwTableModel( QList<HeaderInfo> *headers, QObject *parent )
    : QAbstractTableModel(parent)
    , m_headers(headers)
{
}

int QwTableModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return m_data.size();
}

int QwTableModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return m_headers->size();
}

QVariant QwTableModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    if ( index.row() >= m_data.size() || index.row() < 0)
        return QVariant();

    if ( m_headers->at(index.column()).type == 1 ) {
        if ( role == Qt::CheckStateRole )
            return m_data[index.row()][index.column()].toBool() ? Qt::Checked : Qt::Unchecked;
    } else if ( role == Qt::DisplayRole ) {
        return m_data[index.row()][index.column()];
    }

    return QVariant();
}

QVariant QwTableModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( role == Qt::DisplayRole && orientation == Qt::Horizontal ) {
        if ( section >= 0 && section < m_headers->size() )
            return m_headers->at(section).caption;
    }

    return QVariant();
}

Qt::ItemFlags QwTableModel::flags( const QModelIndex &index ) const
{
    if ( !index.isValid() )
        return Qt::ItemIsEnabled;

    if ( index.column() < 0 || index.column() >= m_headers->size() )
        return Qt::ItemIsEnabled;

    if ( m_headers->at(index.column()).type == 1 )
        return QAbstractTableModel::flags(index) | Qt::ItemIsUserCheckable;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool QwTableModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
    Q_UNUSED(role)

    if ( !index.isValid() )
        return false;

    if ( index.column() < 0 || index.column() >= m_headers->size() )
        return false;
    if ( index.row() < 0 || index.row() >= m_data.size() )
        return false;

    m_data[index.row()][index.column()] = value;
    emit dataChanged(index, index);
    return true;
}

bool QwTableModel::insertRows( int position, int rows, const QModelIndex &parent )
{
    beginInsertRows(parent, position, position + rows - 1);
    for (int i = position; i < position + rows; i++) {
        QVariantList rowData;
        for (int j = 0; j < m_headers->size(); j++) {
            if ( m_headers->at(j).type == 1 )
                rowData << false;
            else
                rowData << "";
        }
        m_data.insert(i, rowData);
    }
    endInsertRows();

    return true;
}

bool QwTableModel::removeRows( int position, int rows, const QModelIndex &parent )
{
    beginRemoveRows(parent, position, position + rows - 1);
    for (int i = position; i < position + rows; i++)
        m_data.removeAt(position);
    endRemoveRows();

    return true;
}

QList<QList<QVariant> > *QwTableModel::matrixData()
{
    return &m_data;
}

QwTableView::QwTableView( const QVariant &initData, QWidget *parent )
    : QTableView(parent)
    , m_valid(false)
    , m_model(0)
{
    m_valid = validateInitData(initData);
    if ( isValid() ) {
        m_model = new QwTableModel(&m_headers);
        setModel(m_model);
        QwItemDelegate *delegate = new QwItemDelegate(&m_headers, m_model->matrixData());
        setItemDelegate(delegate);
    }
}

bool QwTableView::insertRow( int position )
{
    if ( isValid() ) {
        if ( position < 0 || position > m_model->rowCount(QModelIndex()) ) {
            PythonScriptManager::getInstance()->pythonReport(tr("Row Number Out of Range"));
            return false;
        }
        return m_model->insertRows(position, 1);
    }

    return false;
}

bool QwTableView::insertRow( int position, const QVariant &data )
{
    if ( isValid() ) {
        if ( position < 0 || position > m_model->rowCount(QModelIndex()) ) {
            PythonScriptManager::getInstance()->pythonReport(tr("Row Number Out of Range"));
            return false;
        }
        if ( !data.canConvert(QVariant::List) ) {
            PythonScriptManager::getInstance()->pythonReport(tr("Invalid Row Data"));
            return false;
        }
        QVariantList rowData = data.toList();
        if ( rowData.size() != m_headers.size() ) {
            PythonScriptManager::getInstance()->pythonReport(tr("Wrong Length of Row Data List"));
            return false;
        }
        for (int i = 0; i < rowData.size(); i++) {
            if ( m_headers[i].type == 1 && !rowData[i].canConvert(QVariant::Bool) ) {
                PythonScriptManager::getInstance()->pythonReport(tr("Invalid Bool Data In Row List at Index %1").arg(i));
                return false;
            } else if ( !rowData[i].canConvert(QVariant::String) ) {
                PythonScriptManager::getInstance()->pythonReport(tr("Invalid String Data In Row List at Index %1").arg(i));
                return false;
            }
        }
        if ( !m_model->insertRows(position, 1) )
            return false;
        for (int j = 0; j < rowData.size(); j++)
            m_model->setData(m_model->index(position, j), rowData[j]);
        return true;
    }

    return false;
}

bool QwTableView::removeRow( int position )
{
    if ( isValid() ) {
        if ( position < 0 || position >= m_model->rowCount(QModelIndex()) ) {
            PythonScriptManager::getInstance()->pythonReport(tr("Row Number Out of Range"));
            return false;
        }
        return m_model->removeRows(position, 1);
    }

    return false;
}

int QwTableView::rowCount() const
{
    if ( isValid() )
        return m_model->rowCount(QModelIndex());
    return 0;
}

int QwTableView::columnCount() const
{
    if ( isValid() )
        return m_model->columnCount(QModelIndex());
    return 0;
}

QVariant QwTableView::getData(int row, int column) const
{
    if ( !isValid() )
        return QVariant();

    if ( row < 0 || row >= m_model->rowCount(QModelIndex()) ) {
        PythonScriptManager::getInstance()->pythonReport(tr("Row Number Out of Range"));
        return QVariant();
    }

    if ( column < 0 || column >= m_model->columnCount(QModelIndex()) ) {
        PythonScriptManager::getInstance()->pythonReport(tr("Column Number Out of Range"));
        return QVariant();
    }

    return m_model->data(m_model->index(row, column), Qt::DisplayRole);
}

bool QwTableView::setData( int row, int column, const QVariant &data )
{
    if ( !isValid() )
        return false;

    if ( row < 0 || row >= m_model->rowCount(QModelIndex()) ) {
        PythonScriptManager::getInstance()->pythonReport(tr("Row Number Out of Range"));
        return false;
    }

    if ( column < 0 || column >= m_model->columnCount(QModelIndex()) ) {
        PythonScriptManager::getInstance()->pythonReport(tr("Column Number Out of Range"));
        return false;
    }

    if ( m_headers[column].type == 1 && !data.canConvert(QVariant::Bool) ) {
        PythonScriptManager::getInstance()->pythonReport(tr("Invalid Bool Value"));
        return false;
    } else if ( !data.canConvert(QVariant::String) ) {
        PythonScriptManager::getInstance()->pythonReport(tr("Invalid String Value"));
        return false;
    }

    return m_model->setData(m_model->index(row, column), data);
}

QVariantList QwTableView::getRowData( int row ) const
{
    if ( !isValid() )
        return QVariantList();

    if ( row < 0 || row >= m_model->rowCount(QModelIndex()) ) {
        PythonScriptManager::getInstance()->pythonReport(tr("Row Number Out of Range"));
        return QVariantList();
    }

    return m_model->matrixData()->at(row);
}

bool QwTableView::setRowData( int row, const QVariant &rowData )
{
    if ( !isValid() )
        return false;

    if ( row < 0 || row >= m_model->rowCount(QModelIndex()) ) {
        PythonScriptManager::getInstance()->pythonReport(tr("Row Number Out of Range"));
        return false;
    }

    if ( !rowData.canConvert(QVariant::List) ) {
        PythonScriptManager::getInstance()->pythonReport(tr("Invalid Row List Data"));
        return false;
    }

    QVariantList dataList = rowData.toList();
    if ( dataList.size() != m_headers.size() ) {
        PythonScriptManager::getInstance()->pythonReport(tr("Invalid Length of Row List Data"));
        return false;
    }

    int i;
    for (i = 0; i < dataList.size(); i++) {
        if ( m_headers[i].type == 1 && !dataList[i].canConvert(QVariant::Bool) ) {
            PythonScriptManager::getInstance()->pythonReport(tr("Invalid Bool Value of Row Data for Column %1").arg(i));
            return false;
        } else if ( !dataList[i].canConvert(QVariant::String) ) {
            PythonScriptManager::getInstance()->pythonReport(tr("Invalid String Value of Row Data for Column %1").arg(i));
            return false;
        }
    }
    for (i = 0; i < dataList.size(); i++)
        m_model->setData(m_model->index(row, i), dataList[i]);

    return true;
}

bool QwTableView::isValid( bool showMessage ) const
{
    if ( !m_valid && showMessage )
        PythonScriptManager::getInstance()->pythonReport(tr("Table View Not Properly Initialized!"));
    return m_valid;
}

bool QwTableView::validateInitData( const QVariant &initData )
{
    if ( !initData.isValid() || !initData.canConvert(QVariant::List) )
        return false;

    QVariantList headers = initData.toList();
    foreach ( const QVariant &header, headers )
        if ( !validateHeaderData(header) )
            return false;

    return true;
}

bool QwTableView::validateHeaderData( const QVariant &header )
{
    if ( !header.isValid() || !header.canConvert(QVariant::List) )
        return false;

    QVariantList items = header.toList();
    if ( items.size() != 2 && items.size() != 3 )
        return false;

    HeaderInfo hi;
    hi.type = items[0].toInt();
    hi.caption = items[1].toString();

    if ( hi.type == 2 || hi.type == 3 ) {
        if ( items.size() == 3 ) {
            if ( !items[2].canConvert(QVariant::List) )
                return false;
            hi.comboValues = items[2].toStringList();
        }
    }

    m_headers.append(hi);
    return true;
}

QwGuiPythonModule::QwGuiPythonModule()
    : IPythonModule("QwGuiUtils", "guiUtils", "Python Qt Gui Helper Classes")
{
    m_functions << new PythonModuleFunction("TableView", "Create Editable Table View");
}

QwGuiPythonModule::~QwGuiPythonModule()
{
    qDeleteAll(m_functions);
}

QList<PythonModuleFunction *> QwGuiPythonModule::functions() const
{
    return m_functions;
}

QString QwGuiPythonModule::sourceCodeForAssist() const
{
    return QString("from PythonQt import QtGui\n"
                   "class QwTableView(QtGui.QTableView):\n"
                   "    def insertRow(self, position):\n"
                   "        return True\n"
                   "    def removeRow(self, position):\n"
                   "        return True\n"
                   "    def rowCount(self):\n"
                   "        return 1\n"
                   "    def columnCount(self):\n"
                   "        return 1\n"
                   "    def getData(self, row, column):\n"
                   "        return None\n"
                   "    def setData(self, row, column, data):\n"
                   "        return True\n"
                   "    def getRowData(self, row):\n"
                   "        return ['', '', '']\n"
                   "    def setRowData(self, row, rowData):\n"
                   "        return True\n"
                   "class QwGuiUtils():\n"
                   "    StringCell = 0\n"
                   "    CheckBoxCell = 1\n"
                   "    ComboBoxCell = 2\n"
                   "    EditableComboBoxCell = 3\n"
                   "    def TableView(self, initData, parent = None):\n"
                   "        return QwTableView()\n");
}

QwTableView *QwGuiPythonModule::TableView( const QVariant &initData, QWidget *parent )
{
    QwTableView *table = new QwTableView(initData, parent);
    return table;
}

}   // namespace Python
