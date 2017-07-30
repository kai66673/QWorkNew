#include <QColorDialog>

#include "TablePropertiesDialog.h"
#include "ui_TablePropertiesDialog.h"
#include "richeditor_constants.h"
//#include "QxUiEditReportDialog.h"

ColumnWidthsModel::ColumnWidthsModel( const QList<qreal> &widths, QObject *parent )
    : QAbstractTableModel(parent)
    , m_widths(widths)
{
}

ColumnWidthsModel::ColumnWidthsModel( int columns, QObject *parent )
    : QAbstractTableModel(parent)
{
    qreal w = 100.0 / columns;
    for (int i = 0; i < columns; i++)
        m_widths.push_back(w);
}

int ColumnWidthsModel::rowCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return m_widths.size();
}

int ColumnWidthsModel::columnCount( const QModelIndex &parent ) const
{
    Q_UNUSED(parent)
    return 1;
}

QVariant ColumnWidthsModel::data( const QModelIndex &index, int role ) const
{
    if ( !index.isValid() )
        return QVariant();

    int row = index.row();

    if ( row >= 0 && row < rowCount() && index.column() == 0 &&
         (role == Qt::EditRole || role == Qt::DisplayRole) ) {
        return m_widths[row];
    }

    return QVariant();
}

QVariant ColumnWidthsModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( role == Qt::DisplayRole && orientation == Qt::Vertical ) {
        return tr("Column %1").arg(section + 1);
    }

    return QVariant();
}

Qt::ItemFlags ColumnWidthsModel::flags( const QModelIndex &index ) const
{
    if ( !index.isValid() )
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool ColumnWidthsModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
    if ( role != Qt::EditRole || !index.isValid() || rowCount() == 1 )
        return false;

    int row = index.row();
    if ( row < 0 || row >= rowCount() || index.column() != 0 )
        return false;

    qreal oldWidth = m_widths[row];
    qreal newWidth = value.toDouble();
    int reflectRow = row == rowCount() - 1 ? row - 1 : row + 1;

    if ( newWidth < 1.0 || m_widths[reflectRow] + oldWidth - newWidth < 1.0 )
        return false;

    m_widths[row] = newWidth;
    m_widths[reflectRow] += oldWidth - newWidth;

    if ( reflectRow > row )
        emit dataChanged(index, index.sibling(reflectRow, 0));
    else
        emit dataChanged(index.sibling(reflectRow, 0), index);
    return true;
}

void ColumnWidthsModel::updateColumnCount( int columns )
{
    beginResetModel();
    m_widths.clear();
    qreal w = 100.0 / columns;
    for (int i = 0; i < columns; i++)
        m_widths.push_back(w);
    endResetModel();
}

TablePropertiesDialog::TablePropertiesDialog( QTextEdit *textEdit, QTextTable *table, QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::TablePropertiesDialog)
    , m_textEdit(textEdit)
    , m_table(table)
    , m_dataSourceId(0)
    , m_model(0)
{
    ui->setupUi(this);

    if ( !m_table ) {
        updateColorButton(ui->borderColorButton, Qt::darkGray);
        m_model = new ColumnWidthsModel(ui->columnsSpinBox->value());
        ui->columnWidthstableView->setModel(m_model);
        ui->columnWidthstableView->horizontalHeader()->hide();
        return;
    }

    QTextTableFormat f = m_table->format();
    if ( f.hasProperty(RichEditor::DataTableId) ) {
        m_dataSourceId = f.intProperty(RichEditor::DataTableId);
        ui->headerRowsSpinBox->setEnabled(false);
    }

    ui->rowsSpinBox->setValue(m_table->rows());
    ui->rowsSpinBox->setEnabled(false);
    ui->columnsSpinBox->setValue(m_table->columns());
    ui->columnsSpinBox->setEnabled(false);

    ui->headerRowsSpinBox->setValue(f.headerRowCount());
    ui->headerRowsSpinBox->setMaximum(m_table->rows());
    ui->spacingSpinBox->setValue(f.cellSpacing());
    ui->paddingSpinBox->setValue(f.cellPadding());
    ui->widthSpinBox->setValue(f.border());

    ui->leftMarginSpinBox->setValue(f.leftMargin());
    ui->topMarginSpinBox->setValue(f.topMargin());
    ui->rightMarginSpinBox->setValue(f.rightMargin());
    ui->bottomMarginSpinBox->setValue(f.bottomMargin());

    bool borderStyleSpecified = f.borderStyle() > QTextFrameFormat::BorderStyle_None;

    ui->borderStyleComboBox->setCurrentIndex(f.borderStyle() - 1);
    m_oldBorderStyleIndex = borderStyleSpecified ? f.borderStyle() - 1 : 0;
    m_borderColor = f.borderBrush().color();
    updateColorButton(ui->borderColorButton, m_borderColor);

    QVector<QTextLength> constraints = f.columnWidthConstraints();
    QList<qreal> widths;
    for (int i = 0; i < constraints.size(); i++)
        widths.push_back(constraints[i].rawValue());
    m_model = new ColumnWidthsModel(widths);
    ui->columnWidthstableView->setModel(m_model);
    ui->columnWidthstableView->horizontalHeader()->hide();

    connect(ui->rowsSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onRowsCountChanged(int)));
    on_widthSpinBox_valueChanged(f.border());
}

TablePropertiesDialog::~TablePropertiesDialog()
{
    delete ui;
}

void TablePropertiesDialog::updateColorButton( QToolButton *button, QColor color )
{
    if ( color.isValid() ) {
        QPixmap pix(16, 16);
        pix.fill(color);
        button->setIcon(QIcon(pix));
    } else {
        button->setIcon(QIcon());
    }
}

void TablePropertiesDialog::on_borderColorButton_clicked()
{
    QColor newColor = QColorDialog::getColor(m_borderColor, this);
    if ( newColor.isValid() ) {
        m_borderColor = newColor;
        updateColorButton(ui->borderColorButton, newColor);
    }
}

void TablePropertiesDialog::onRowsCountChanged( int value )
{
    ui->headerRowsSpinBox->setMaximum(value);
}

static QTextFrameFormat::BorderStyle fromInt( int index )
{
    switch ( index ) {
        case 0: return QTextFrameFormat::BorderStyle_Dotted;
        case 1: return QTextFrameFormat::BorderStyle_Dashed;
        case 2: return QTextFrameFormat::BorderStyle_Solid;
        case 3: return QTextFrameFormat::BorderStyle_Double;
        case 4: return QTextFrameFormat::BorderStyle_DotDash;
        case 5: return QTextFrameFormat::BorderStyle_DotDotDash;
        case 6: return QTextFrameFormat::BorderStyle_Groove;
        case 7: return QTextFrameFormat::BorderStyle_Ridge;
        case 8: return QTextFrameFormat::BorderStyle_Inset;
        case 9: return QTextFrameFormat::BorderStyle_Outset;
    }

    return QTextFrameFormat::BorderStyle_None;
}

void TablePropertiesDialog::on_okButton_clicked()
{
    QTextTableFormat f;

    f.setHeaderRowCount(ui->headerRowsSpinBox->value());
    f.setCellSpacing(ui->spacingSpinBox->value());
    f.setCellPadding(ui->paddingSpinBox->value());
    f.setBorder(ui->widthSpinBox->value());

    f.setLeftMargin(ui->leftMarginSpinBox->value());
    f.setTopMargin(ui->topMarginSpinBox->value());
    f.setRightMargin(ui->rightMarginSpinBox->value());
    f.setBottomMargin(ui->bottomMarginSpinBox->value());

    f.setBorderStyle(fromInt(ui->borderStyleComboBox->currentIndex()));
    f.setBorderBrush(m_borderColor);

    QVector<QTextLength> constraints;
    for (int i = 0; i < m_model->rowCount(); i++)
        constraints.push_back(QTextLength(QTextLength::PercentageLength, m_model->data(m_model->index(i, 0)).toDouble()));
    f.setColumnWidthConstraints(constraints);

    if ( !m_table ) {
        int columnCount = ui->columnsSpinBox->value();
        int rowsCount = ui->rowsSpinBox->value();
        QTextTable *newTable = m_textEdit->textCursor().insertTable(rowsCount, columnCount, f);
        QTextTableCellFormat cellFormat = newTable->cellAt(0, 0).format().toTableCellFormat();
        cellFormat.setBackground(Qt::white);
        for (int r = 0; r < rowsCount; r++)
            for (int c = 0; c < columnCount; c++)
                newTable->cellAt(r, c).setFormat(cellFormat);
    }
    else {
        if ( m_dataSourceId )
            f.setProperty(RichEditor::DataTableId, m_dataSourceId);
        m_table->setFormat(f);
    }

    accept();
}

void TablePropertiesDialog::on_columnsSpinBox_valueChanged(int arg1)
{
    if ( m_model )
        m_model->updateColumnCount(arg1);
}

void TablePropertiesDialog::on_widthSpinBox_valueChanged(double arg1)
{
    ui->borderStyleComboBox->setEnabled(arg1 > 0);
    ui->borderColorButton->setEnabled(arg1 > 0);
}
