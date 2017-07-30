#ifndef TABLEPROPERTIESDIALOG_H
#define TABLEPROPERTIESDIALOG_H

#include <QDialog>
#include <QTextBrowser>
#include <QTextTable>
#include <QToolButton>
#include <QAbstractTableModel>

class ColumnWidthsModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    ColumnWidthsModel( const QList<qreal> &widths, QObject *parent = 0 );
    ColumnWidthsModel( int columns, QObject *parent = 0 );

    int rowCount( const QModelIndex &parent = QModelIndex() ) const;
    int columnCount( const QModelIndex &parent = QModelIndex() ) const;
    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex &index ) const;

    bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole );

    void updateColumnCount( int columns );

private:
    QList<qreal> m_widths;
};

namespace Ui {
class TablePropertiesDialog;
}

class TablePropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TablePropertiesDialog( QTextEdit *textEdit, QTextTable *table = 0, QWidget *parent = 0 );
    ~TablePropertiesDialog();

private slots:
    void on_borderColorButton_clicked();

    void onRowsCountChanged(int value);
    void on_columnsSpinBox_valueChanged(int arg1);

    void on_okButton_clicked();

    void on_widthSpinBox_valueChanged(double arg1);

private:
    void updateColorButton( QToolButton *button, QColor color = QColor() );

    Ui::TablePropertiesDialog *ui;

    QTextEdit *m_textEdit;
    QTextTable *m_table;
    int m_dataSourceId;

    int m_oldBorderStyleIndex;
    QColor m_borderColor;

    ColumnWidthsModel *m_model;
};

#endif // TABLEPROPERTIESDIALOG_H
