#ifndef CELLSPROPERTIESDIALOG_H
#define CELLSPROPERTIESDIALOG_H

#include <QDialog>
#include <QTextBrowser>
#include <QToolButton>
#include <QColor>

namespace Ui {
class CellsPropertiesDialog;
}

class CellsPropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CellsPropertiesDialog( QTextEdit *textEdit, QWidget *parent = 0 );
    ~CellsPropertiesDialog();

private slots:
    void on_okButton_clicked();

    void on_leftBorderButton_toggled(bool checked);
    void on_topBorderButton_toggled(bool checked);
    void on_rightBorderButton_toggled(bool checked);
    void on_bottomBorderButton_toggled(bool checked);

    void on_leftBorderColorButton_clicked();
    void on_topBorderColorButton_clicked();
    void on_rightBorderColorButton_clicked();
    void on_bottomBorderColorButton_clicked();

    void on_leftPaddingCheckBox_toggled(bool checked);
    void on_topPaddingCheckBox_toggled(bool checked);
    void on_rightPaddingCheckBox_toggled(bool checked);
    void on_bottomPaddingCheckBox_toggled(bool checked);

    void on_backgroundColorButton_clicked();

private:
    void updateColorButton( QToolButton *button, QColor color = QColor() );

    Ui::CellsPropertiesDialog *ui;
    QTextEdit *m_textEdit;

    int m_oldLeftStyleIndex;
    int m_oldTopStyleIndex;
    int m_oldRightStyleIndex;
    int m_oldBottomStyleIndex;

    QColor m_leftBorderColor;
    QColor m_topBorderColor;
    QColor m_rightBorderColor;
    QColor m_bottomBorderColor;
    QColor m_backgroundColor;
};

#endif // CELLSPROPERTIESDIALOG_H
