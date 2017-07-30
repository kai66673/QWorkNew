#include <QComboBox>

#include "SqlDefineBindingsDialog.h"
#include "ui_SqlDefineBindingsDialog.h"

#include "Core.h"

SqlDefineBindingsDialog::SqlDefineBindingsDialog( const QSet<QString> &bindNames,
                                                  const QMap<QString, Sql::SqlBindValue> &bindingsHistory )
    : QDialog(Core::Storage::mainWindow()->widget())
    , ui(new Ui::SqlDefineBindingsDialog)
{
    ui->setupUi(this);

    foreach ( QString name, bindNames ) {
        SqlBinding item;
        item.bindName = name;
        if ( bindingsHistory.find(name) != bindingsHistory.end() ) {
            item.bindType = bindingsHistory[name].type;
            item.bindValue = bindingsHistory[name].value;
        }
        bindData.append(item);
    }

    connect(ui->okPushButton, SIGNAL(clicked()), this, SLOT(tryAccept()));

    SqlBindingsModel *model = new SqlBindingsModel(&bindData, this);
    ui->bindingsTableView->setModel(model);
    ui->bindingsTableView->setItemDelegateForColumn(1, new SqlBindingsDelegate(&bindData, ui->bindingsTableView));
}

SqlDefineBindingsDialog::~SqlDefineBindingsDialog()
{
    delete ui;
}

void SqlDefineBindingsDialog::getBindings( QMap<QString, Sql::SqlBindValue> &bindings, QMap<QString, Sql::SqlBindValue> &bindingsHistory )
{
    bindings.clear();
    for (int i = 0; i < bindData.size(); i++){
        Sql::SqlBindValue newBind(bindData.at(i).bindType, bindData.at(i).bindValue);
        bindingsHistory[bindData.at(i).bindName] = newBind;
        bindings[bindData.at(i).bindName] = newBind;
    }
}

void SqlDefineBindingsDialog::tryAccept()
{
    for (int i = 0; i < bindData.size(); i++)
        if ( bindData[i].bindValue.isEmpty() ) {
            Core::Storage::warning(tr("Not All Parameters Defined!"));
            return;
        }

    accept();
}

SqlBindingsDelegate::SqlBindingsDelegate( QList<SqlBinding> *bindData_, QObject *parent )
    : QItemDelegate(parent)
    , bindData(bindData_)
{
}

QWidget *SqlBindingsDelegate::createEditor( QWidget *parent,
                                            const QStyleOptionViewItem &option,
                                            const QModelIndex &index ) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    QComboBox *typeCombo = new QComboBox(parent);
    typeCombo->addItem(tr("<NUMERIC>"));
    typeCombo->addItem(tr("<LITERAL>"));
    return typeCombo;
}

void SqlBindingsDelegate::setEditorData( QWidget *editor, const QModelIndex &index ) const
{
    QComboBox *typeCombo = static_cast<QComboBox *>(editor);
    typeCombo->setCurrentIndex(bindData->at(index.row()).bindType);
    return;
}

void SqlBindingsDelegate::setModelData( QWidget *editor, QAbstractItemModel *model,
                                        const QModelIndex &index ) const
{
    QComboBox *typeCombo = static_cast<QComboBox *>(editor);
    model->setData(index, typeCombo->currentIndex(), Qt::EditRole);
    return;
}

void SqlBindingsDelegate::updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option,
                                                const QModelIndex &index ) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

SqlBindingsModel::SqlBindingsModel( QList<SqlBinding> *bindData_, QObject *parent )
    :QAbstractTableModel(parent)
    , bindData(bindData_)
{
}

int SqlBindingsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return bindData->size();
}

int SqlBindingsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant SqlBindingsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if ( index.row() >= bindData->size() || index.row() < 0 )
        return QVariant();

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0: return bindData->at(index.row()).bindName;
            case 1: {
                int ty = bindData->at(index.row()).bindType;
                switch ( ty ) {
                    case 0: return tr("<NUMERIC>");
                    default: return tr("<LITERAL>");
                }
            }
            case 2: return bindData->at(index.row()).bindValue;
        }
    }

    if (role == Qt::EditRole) {
        switch (index.column()) {
            case 0: return bindData->at(index.row()).bindName;
            case 1: return bindData->at(index.row()).bindType;
            case 2: return bindData->at(index.row()).bindValue;
        }
    }

    return QVariant();
}

QVariant SqlBindingsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0: return tr("Name");
            case 1: return tr("Type");
            case 2: return tr("Value");
        }
    }

    return QVariant();
}

Qt::ItemFlags SqlBindingsModel::flags(const QModelIndex &index) const
{
    if ( !index.isValid() )
        return Qt::ItemIsEnabled;

    if ( index.column() != 0 )
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool SqlBindingsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    switch (index.column()) {
        case 0:
            (*bindData)[index.row()].bindName = value.toString();
            break;
        case 1:
            (*bindData)[index.row()].bindType = value.toInt();
            break;
        case 2:
            (*bindData)[index.row()].bindValue = value.toString().trimmed();
            break;
    }

    emit dataChanged(index, index);

    return true;
}
