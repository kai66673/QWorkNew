#include <QTextTable>
#include <QColorDialog>

#include "CellsPropertiesDialog.h"
#include "ui_CellsPropertiesDialog.h"

CellsPropertiesDialog::CellsPropertiesDialog( QTextEdit *textEdit, QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::CellsPropertiesDialog)
    , m_textEdit(textEdit)
{
    ui->setupUi(this);

    QTextCursor c = m_textEdit->textCursor();
    QTextTableCellFormat f = c.currentTable()->cellAt(c).format().toTableCellFormat();
    QTextTableFormat tf = c.currentTable()->format();

    if ( f.hasProperty(QTextFormat::TableCellLeftPadding) ) {
        ui->leftPaddingCheckBox->setChecked(true);
        ui->leftPaddingSpinBox->setValue(f.leftPadding());
    } else {
        ui->leftPaddingSpinBox->setValue(tf.cellPadding());
        ui->leftPaddingSpinBox->setEnabled(false);
    }

    if ( f.hasProperty(QTextFormat::TableCellTopPadding) ) {
        ui->topPaddingCheckBox->setChecked(true);
        ui->topPaddingSpinBox->setValue(f.topPadding());
    } else {
        ui->topPaddingSpinBox->setValue(tf.cellPadding());
        ui->topPaddingSpinBox->setEnabled(false);
    }

    if ( f.hasProperty(QTextFormat::TableCellRightPadding) ) {
        ui->rightPaddingCheckBox->setChecked(true);
        ui->rightPaddingSpinBox->setValue(f.rightPadding());
    } else {
        ui->rightPaddingSpinBox->setValue(tf.cellPadding());
        ui->rightPaddingSpinBox->setEnabled(false);
    }

    if ( f.hasProperty(QTextFormat::TableCellBottomPadding) ) {
        ui->bottomPaddingCheckBox->setChecked(true);
        ui->bottomPaddingSpinBox->setValue(f.bottomPadding());
    } else {
        ui->bottomPaddingSpinBox->setValue(tf.cellPadding());
        ui->bottomPaddingSpinBox->setEnabled(false);
    }
#ifndef QTEXT_NOPATCHED
    bool l = f.borderStyleLeft()    > QTextFrameFormat::BorderStyle_None;
    bool t = f.borderStyleTop()     > QTextFrameFormat::BorderStyle_None;
    bool r = f.borderStyleRight()   > QTextFrameFormat::BorderStyle_None;
    bool b = f.borderStyleBottom()  > QTextFrameFormat::BorderStyle_None;

    ui->leftBorderButton->setChecked(l);
    ui->topBorderButton->setChecked(t);
    ui->rightBorderButton->setChecked(r);
    ui->bottomBorderButton->setChecked(b);

    ui->leftBorderStyleComboBox->setCurrentIndex(f.borderStyleLeft() - 1);
    ui->topBorderStyleComboBox->setCurrentIndex(f.borderStyleTop() - 1);
    ui->rightBorderStyleComboBox->setCurrentIndex(f.borderStyleRight() - 1);
    ui->bottomBorderStyleComboBox->setCurrentIndex(f.borderStyleBottom() - 1);

    ui->leftBorderWidthSpinBox->setValue(f.borderLeft());
    ui->topBorderWidthSpinBox->setValue(f.borderTop());
    ui->rightBorderWidthSpinBox->setValue(f.borderRight());
    ui->bottomBorderWidthSpinBox->setValue(f.borderBottom());

    m_oldLeftStyleIndex     = l ? f.borderStyleLeft()   - 1 : 0;
    m_oldTopStyleIndex     = t ? f.borderStyleTop()    - 1 : 0;
    m_oldRightStyleIndex    = r ? f.borderStyleRight()  - 1 : 0;
    m_oldBottomStyleIndex   = b ? f.borderStyleBottom() - 1 : 0;

    ui->leftBorderStyleComboBox->setEnabled(l);
    ui->topBorderStyleComboBox->setEnabled(t);
    ui->rightBorderStyleComboBox->setEnabled(r);
    ui->bottomBorderStyleComboBox->setEnabled(b);

    ui->leftBorderWidthSpinBox->setEnabled(l);
    ui->topBorderWidthSpinBox->setEnabled(t);
    ui->rightBorderWidthSpinBox->setEnabled(r);
    ui->bottomBorderWidthSpinBox->setEnabled(b);

    ui->leftBorderColorButton->setEnabled(l);
    ui->topBorderColorButton->setEnabled(t);
    ui->rightBorderColorButton->setEnabled(r);
    ui->bottomBorderColorButton->setEnabled(b);

    m_leftBorderColor = f.borderBrushLeft().color();
    updateColorButton(ui->leftBorderColorButton, m_leftBorderColor);
    m_topBorderColor = f.borderBrushTop().color();
    updateColorButton(ui->topBorderColorButton, m_topBorderColor);
    m_rightBorderColor = f.borderBrushRight().color();
    updateColorButton(ui->rightBorderColorButton, m_rightBorderColor);
    m_bottomBorderColor = f.borderBrushBottom().color();
    updateColorButton(ui->bottomBorderColorButton, m_bottomBorderColor);
#endif
    m_backgroundColor = f.background().color();
    updateColorButton(ui->backgroundColorButton, m_backgroundColor);
}

CellsPropertiesDialog::~CellsPropertiesDialog()
{
    delete ui;
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

void CellsPropertiesDialog::on_okButton_clicked()
{
    QTextCursor c = m_textEdit->textCursor();
    QTextTable *t = c.currentTable();
    QTextTableCellFormat f = c.currentTable()->cellAt(c).format().toTableCellFormat();

    if ( ui->leftPaddingCheckBox->isChecked() )
        f.setLeftPadding(ui->leftPaddingSpinBox->value());
    else
        f.clearProperty(QTextFormat::TableCellLeftPadding);
    if ( ui->topPaddingCheckBox->isChecked() )
        f.setTopPadding(ui->topPaddingSpinBox->value());
    else
        f.clearProperty(QTextFormat::TableCellTopPadding);
    if ( ui->rightPaddingCheckBox->isChecked() )
        f.setRightPadding(ui->rightPaddingSpinBox->value());
    else
        f.clearProperty(QTextFormat::TableCellRightPadding);
    if ( ui->bottomPaddingCheckBox->isChecked() )
        f.setBottomPadding(ui->bottomPaddingSpinBox->value());
    else
        f.clearProperty(QTextFormat::TableCellBottomPadding);
#ifndef QTEXT_NOPATCHED
    f.setBorderStyleLeft(fromInt(ui->leftBorderStyleComboBox->currentIndex()));
    f.setBorderStyleTop(fromInt(ui->topBorderStyleComboBox->currentIndex()));
    f.setBorderStyleRight(fromInt(ui->rightBorderStyleComboBox->currentIndex()));
    f.setBorderStyleBottom(fromInt(ui->bottomBorderStyleComboBox->currentIndex()));

    f.setBorderLeft(ui->leftBorderWidthSpinBox->value());
    f.setBorderTop(ui->topBorderWidthSpinBox->value());
    f.setBorderRight(ui->rightBorderWidthSpinBox->value());
    f.setBorderBottom(ui->bottomBorderWidthSpinBox->value());

    f.setBorderBrushLeft(m_leftBorderColor);
    f.setBorderBrushTop(m_topBorderColor);
    f.setBorderBrushRight(m_rightBorderColor);
    f.setBorderBrushBottom(m_bottomBorderColor);
#endif
    f.setBackground(m_backgroundColor);

    if ( c.hasComplexSelection() ) {
        int firstRow;
        int numRows;
        int firstColumn;
        int numColumns;
        c.selectedTableCells(&firstRow, &numRows, &firstColumn, &numColumns);
        for (int r = 0; r < numRows; r++) {
            int fcol = firstColumn;
            for (int c = 0; c < numColumns; c++) {
                t->cellAt(firstRow, fcol).setFormat(f);
                fcol++;
            }
            firstRow++;
        }
    }
    else
        t->cellAt(c).setFormat(f);

    accept();
}

void CellsPropertiesDialog::on_leftBorderButton_toggled(bool checked)
{
    ui->leftBorderStyleComboBox->setEnabled(checked);
    if ( checked ) {
        ui->leftBorderStyleComboBox->setCurrentIndex(m_oldLeftStyleIndex);
    } else {
        m_oldLeftStyleIndex = ui->leftBorderStyleComboBox->currentIndex();
        ui->leftBorderStyleComboBox->setCurrentIndex(-1);
    }
    ui->leftBorderWidthSpinBox->setEnabled(checked);
    ui->leftBorderColorButton->setEnabled(checked);
}

void CellsPropertiesDialog::on_topBorderButton_toggled(bool checked)
{
    ui->topBorderStyleComboBox->setEnabled(checked);
    if ( checked ) {
        ui->topBorderStyleComboBox->setCurrentIndex(m_oldTopStyleIndex);
    } else {
        m_oldTopStyleIndex = ui->topBorderStyleComboBox->currentIndex();
        ui->topBorderStyleComboBox->setCurrentIndex(-1);
    }
    ui->topBorderWidthSpinBox->setEnabled(checked);
    ui->topBorderColorButton->setEnabled(checked);
}

void CellsPropertiesDialog::on_rightBorderButton_toggled(bool checked)
{
    ui->rightBorderStyleComboBox->setEnabled(checked);
    if ( checked ) {
        ui->rightBorderStyleComboBox->setCurrentIndex(m_oldRightStyleIndex);
    } else {
        m_oldRightStyleIndex = ui->rightBorderStyleComboBox->currentIndex();
        ui->rightBorderStyleComboBox->setCurrentIndex(-1);
    }
    ui->rightBorderWidthSpinBox->setEnabled(checked);
    ui->rightBorderColorButton->setEnabled(checked);
}

void CellsPropertiesDialog::on_bottomBorderButton_toggled(bool checked)
{
    ui->bottomBorderStyleComboBox->setEnabled(checked);
    if ( checked ) {
        ui->bottomBorderStyleComboBox->setCurrentIndex(m_oldBottomStyleIndex);
    } else {
        m_oldBottomStyleIndex = ui->bottomBorderStyleComboBox->currentIndex();
        ui->bottomBorderStyleComboBox->setCurrentIndex(-1);
    }
    ui->bottomBorderWidthSpinBox->setEnabled(checked);
    ui->bottomBorderColorButton->setEnabled(checked);
}

void CellsPropertiesDialog::updateColorButton( QToolButton *button, QColor color )
{
    if ( color.isValid() ) {
        QPixmap pix(16, 16);
        pix.fill(color);
        button->setIcon(QIcon(pix));
    } else {
        button->setIcon(QIcon());
    }
}

void CellsPropertiesDialog::on_leftBorderColorButton_clicked()
{
    QColor newColor = QColorDialog::getColor(m_leftBorderColor, this);
    if ( newColor.isValid() ) {
        m_leftBorderColor = newColor;
        updateColorButton(ui->leftBorderColorButton, newColor);
    }
}

void CellsPropertiesDialog::on_topBorderColorButton_clicked()
{
    QColor newColor = QColorDialog::getColor(m_topBorderColor, this);
    if ( newColor.isValid() ) {
        m_topBorderColor = newColor;
        updateColorButton(ui->topBorderColorButton, newColor);
    }
}

void CellsPropertiesDialog::on_rightBorderColorButton_clicked()
{
    QColor newColor = QColorDialog::getColor(m_rightBorderColor, this);
    if ( newColor.isValid() ) {
        m_rightBorderColor = newColor;
        updateColorButton(ui->rightBorderColorButton, newColor);
    }
}

void CellsPropertiesDialog::on_bottomBorderColorButton_clicked()
{
    QColor newColor = QColorDialog::getColor(m_bottomBorderColor, this);
    if ( newColor.isValid() ) {
        m_bottomBorderColor = newColor;
        updateColorButton(ui->bottomBorderColorButton, newColor);
    }
}

void CellsPropertiesDialog::on_leftPaddingCheckBox_toggled(bool checked)
{
    ui->leftPaddingSpinBox->setEnabled(checked);
}

void CellsPropertiesDialog::on_topPaddingCheckBox_toggled(bool checked)
{
    ui->topPaddingSpinBox->setEnabled(checked);
}

void CellsPropertiesDialog::on_rightPaddingCheckBox_toggled(bool checked)
{
    ui->rightPaddingSpinBox->setEnabled(checked);
}

void CellsPropertiesDialog::on_bottomPaddingCheckBox_toggled(bool checked)
{
    ui->bottomPaddingSpinBox->setEnabled(checked);
}

void CellsPropertiesDialog::on_backgroundColorButton_clicked()
{
    QColor newColor = QColorDialog::getColor(m_backgroundColor, this);
    if ( newColor.isValid() ) {
        m_backgroundColor = newColor;
        updateColorButton(ui->backgroundColorButton, newColor);
    }
}
