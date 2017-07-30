#ifndef SQLDEFINEBINDINGSDIALOG_H
#define SQLDEFINEBINDINGSDIALOG_H

#include <QDialog>
#include <QAbstractTableModel>
#include <QItemDelegate>
#include <QList>
#include <QSet>
#include <QMap>
#include <QVariant>

#include "SqlBindHelper.h"
#include "IMainWindow.h"

namespace Ui {
class SqlDefineBindingsDialog;
}

struct SqlBinding {
    SqlBinding()
        : bindName("")
        , bindType(0)
        , bindValue("")
    {}

    QString bindName;
    int     bindType;
    QString bindValue;
};

class SqlDefineBindingsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SqlDefineBindingsDialog( const QSet<QString> &bindNames,
                                      const QMap<QString, Sql::SqlBindValue> &bindingsHistory);
    ~SqlDefineBindingsDialog();

    void getBindings( QMap<QString, Sql::SqlBindValue> &bindings, QMap<QString, Sql::SqlBindValue> &bindingsHistory );
    
private slots:
    void tryAccept();

private:
    Ui::SqlDefineBindingsDialog *ui;
    QList<SqlBinding> bindData;
};

class SqlBindingsDelegate : public QItemDelegate
{
public:
    SqlBindingsDelegate( QList<SqlBinding> *bindData_, QObject *parent = 0 );

    QWidget *createEditor( QWidget *parent, const QStyleOptionViewItem &option,
                           const QModelIndex &index ) const;
    void setEditorData( QWidget *editor, const QModelIndex &index ) const;
    void setModelData( QWidget *editor, QAbstractItemModel *model,
                       const QModelIndex &index ) const;

    void updateEditorGeometry( QWidget *editor,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index ) const;

private:
    QList<SqlBinding> *bindData;
};


class SqlBindingsModel: public QAbstractTableModel
{
public:
    SqlBindingsModel( QList<SqlBinding> *bindData_, QObject *parent = 0 );

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

private:
    QList<SqlBinding> *bindData;
};

#endif // SQLDEFINEBINDINGSDIALOG_H
