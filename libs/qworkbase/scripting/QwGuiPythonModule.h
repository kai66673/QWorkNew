#ifndef QWGUIPYTHONMODULE_H
#define QWGUIPYTHONMODULE_H

#include <QAbstractTableModel>
#include <QTableView>
#include <QItemDelegate>

#include "PythonScriptManager.h"

namespace Python {

struct HeaderInfo {
    int type;           // 0 - String
                        // 1 - CheckBox
                        // 2 - ComboBox; 3 - editable ComboBox
    QString caption;
    QStringList comboValues;
};

class QwItemDelegate: public QItemDelegate
{
public:
    QwItemDelegate( QList<HeaderInfo> *headers, QList<QList<QVariant> > *data, QObject *parent = 0 );

    QWidget *createEditor( QWidget *parent, const QStyleOptionViewItem &option,
                           const QModelIndex &index ) const;
    void setEditorData( QWidget *editor, const QModelIndex &index ) const;
    void setModelData( QWidget *editor, QAbstractItemModel *model,
                       const QModelIndex &index ) const;

    void updateEditorGeometry( QWidget *editor,
                               const QStyleOptionViewItem &option,
                               const QModelIndex &index ) const;

private:
    QList<HeaderInfo> *m_headers;
    QList<QList<QVariant> > *m_data;
};

class QwTableModel: public QAbstractTableModel
{
public:
    QwTableModel( QList<HeaderInfo> *headers, QObject *parent = 0 );

    int rowCount( const QModelIndex &parent ) const;
    int columnCount( const QModelIndex &parent ) const;
    QVariant data( const QModelIndex &index, int role ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex &index ) const;

    bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole );
    bool insertRows( int position, int rows, const QModelIndex &parent = QModelIndex() );
    bool removeRows( int position, int rows, const QModelIndex &parent = QModelIndex() );

    QList<QList<QVariant> > *matrixData();

private:
    QList<HeaderInfo> *m_headers;
    QList<QList<QVariant> > m_data;
};

class QwTableView: public QTableView
{
    Q_OBJECT

public:
    QwTableView( const QVariant &initData, QWidget *parent = 0 );

public slots:
    bool insertRow( int position );
    bool insertRow( int position, const QVariant &data );
    bool removeRow( int position );
    int  rowCount() const;
    int  columnCount() const;

    QVariant getData(int row, int column) const;
    bool setData( int row, int column, const QVariant &data );
    QVariantList getRowData( int row ) const;
    bool setRowData( int row, const QVariant &rowData );

private:
    bool isValid( bool showMessage = true ) const;
    bool validateInitData( const QVariant &initData );
    bool validateHeaderData( const QVariant &header );

    bool m_valid;
    QList<HeaderInfo> m_headers;
    QwTableModel *m_model;
};

class QwGuiPythonModule : public IPythonModule
{
    Q_OBJECT
    Q_ENUMS(TableCellContentType)

public:
    enum TableCellContentType {
        StringCell,
        CheckBoxCell,
        ComboBoxCell,
        EditableComboBoxCell
    };

    QwGuiPythonModule();
    virtual ~QwGuiPythonModule();

    virtual QList<PythonModuleFunction *> functions() const;
    virtual QString sourceCodeForAssist() const;

public slots:
    QwTableView *TableView( const QVariant &initData, QWidget *parent = 0 );

private:
    QList<PythonModuleFunction *> m_functions;
};

}   // namespace Python

#endif // QWGUIPYTHONMODULE_H
