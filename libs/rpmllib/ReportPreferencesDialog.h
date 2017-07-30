#ifndef REPORTPREFERENCESDIALOG_H
#define REPORTPREFERENCESDIALOG_H

#include <QToolButton>

#include "QwDialog.h"
#include "ReportData.h"
#include "RichEditorWidget.h"

namespace Ui {
class ReportPreferencesDialog;
}

namespace RPML {

class ReportPreferencesDialog : public QwDialog
{
    Q_OBJECT
    
public:
    explicit ReportPreferencesDialog( RPML::ReportData *reportData, QWidget *parent = 0 );
    ~ReportPreferencesDialog();

protected:
    virtual void saveDialogGeomtry();
    virtual void restoreDialogGeomtry();

private slots:
    void on_headerHeightSpinBox_valueChanged( double arg1 );
    void on_footerHeightSpinBox_valueChanged( double arg1 );
    void on_changeDefaultFontButton_clicked();
    void on_okButton_clicked();
    void on_cancelButton_clicked();

    void on_paperSizeComboBox_currentIndexChanged( int index );
    void on_portraitButton_toggled( bool checked );
    void on_lanscapeButton_toggled( bool checked );

    void on_headerRulerColorButton_clicked();
    void on_footerRulerColorButton_clicked();

private:
    void updateMaximumMarginValues( int formatId, bool isPortrait );
    void updateColorButton( QToolButton *button, const QColor &color = QColor() );

    Ui::ReportPreferencesDialog *ui;

    RPML::ReportData *m_reportData;
    RichEditor::RichEditorWidget *m_headerEditor;
    RichEditor::RichEditorWidget *m_footerEditor;

    QFont  m_newDefaultFont;
    QColor m_newHeaderRuleColor;
    QColor m_newFooterRuleColor;

    QMainWindow *m_headerMainWidget;
    QMainWindow *m_footerMainWidget;
};

}   // namespace RPML

#endif // REPORTPREFERENCESDIALOG_H
