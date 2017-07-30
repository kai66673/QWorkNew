#include <QFontDialog>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QColorDialog>
#include <QSettings>

#include "ReportPreferencesDialog.h"
#include "Core.h"
#include "ui_ReportPreferencesDialog.h"

namespace RPML {

ReportPreferencesDialog::ReportPreferencesDialog( RPML::ReportData *reportData, QWidget *parent )
    : QwDialog( "ReportPreferencesDialog",
                Core::Storage::mainWindow()->orgName(),
                Core::Storage::mainWindow()->appName(),
                parent )
    , ui(new Ui::ReportPreferencesDialog)
    , m_reportData(reportData)
{
    ui->setupUi(this);

    m_newDefaultFont = m_reportData->m_defaultFont;

    Utils::PageFormat::Decoration header = m_reportData->m_pageFormat->header();
    ui->headerHeightSpinBox->setValue(header.height);
    ui->headerHeightSpinBox->setSuffix(tr("mm"));

    Utils::PageFormat::Decoration footer = m_reportData->m_pageFormat->footer();
    ui->footerHeightSpinBox->setValue(footer.height);
    ui->footerHeightSpinBox->setSuffix(tr("mm"));

    ui->defaultFontSampleTextEdit->document()->setDefaultFont(m_reportData->m_defaultFont);

    m_headerMainWidget = new QMainWindow();
    m_headerEditor = new RichEditor::RichEditorWidget(m_headerMainWidget);
    m_headerEditor->setHtml(m_reportData->m_headerHtml);
    m_headerMainWidget->setCentralWidget(m_headerEditor);
    m_headerMainWidget->addToolBar(m_headerEditor->textToolBar());
    m_headerMainWidget->addToolBar(m_headerEditor->paraToolBar());
    m_headerMainWidget->addToolBar(m_headerEditor->tableToolBar());
    QVBoxLayout *headerLayout = new QVBoxLayout();
    headerLayout->addWidget(m_headerMainWidget);
    QWidget *headerWidget = new QWidget();
    headerWidget->setLayout(headerLayout);
    ui->prefsTabWidget->addTab(headerWidget, tr("Header"));

    m_footerMainWidget = new QMainWindow();
    m_footerEditor = new RichEditor::RichEditorWidget(m_footerMainWidget);
    m_footerEditor->setHtml(m_reportData->m_footerHtml);
    m_footerMainWidget->setCentralWidget(m_footerEditor);
    m_footerMainWidget->addToolBar(m_footerEditor->textToolBar());
    m_footerMainWidget->addToolBar(m_footerEditor->paraToolBar());
    m_footerMainWidget->addToolBar(m_footerEditor->tableToolBar());
    QVBoxLayout *footerLayout = new QVBoxLayout();
    footerLayout->addWidget(m_footerMainWidget);
    QWidget *footerWidget = new QWidget();
    footerWidget->setLayout(footerLayout);
    ui->prefsTabWidget->addTab(footerWidget, tr("Footer"));

    ui->leftMarginSpinBox->setSuffix("mm");
    ui->topMarginSpinBox->setSuffix("mm");
    ui->rightMarginSpinBox->setSuffix("mm");
    ui->bottomMarginSpinBox->setSuffix("mm");
    ui->headerRulerWitdhSpinBox->setSuffix("px");
    ui->footerRulerWitdhSpinBox->setSuffix("px");
    ui->headerSpacingSpinBox->setSuffix("mm");
    ui->footerSpacingSpinBox->setSuffix("mm");

    ui->paperSizeComboBox->setCurrentIndex(m_reportData->m_pageFormat->formatId());
    ui->portraitButton->setChecked(m_reportData->m_pageFormat->isPortrait());
    ui->lanscapeButton->setChecked(!m_reportData->m_pageFormat->isPortrait());
    Utils::PageFormat::Margin mmMargins = m_reportData->m_pageFormat->mmMargins();
    ui->leftMarginSpinBox->setValue(mmMargins.left);
    ui->topMarginSpinBox->setValue(mmMargins.top);
    ui->rightMarginSpinBox->setValue(mmMargins.right);
    ui->bottomMarginSpinBox->setValue(mmMargins.bottom);

    m_newHeaderRuleColor = header.ruleColor;
    m_newFooterRuleColor = footer.ruleColor;
    ui->headerRulerWitdhSpinBox->setValue(header.ruleWidth);
    ui->footerRulerWitdhSpinBox->setValue(footer.ruleWidth);
    updateColorButton(ui->headerRulerColorButton, m_newHeaderRuleColor);
    updateColorButton(ui->footerRulerColorButton, m_newFooterRuleColor);

    ui->headerSpacingSpinBox->setValue(header.spacing);
    ui->footerSpacingSpinBox->setValue(footer.spacing);

    bool he = (header.height > 0);
    ui->prefsTabWidget->setTabEnabled(1, he);
    ui->headerRulerWitdhSpinBox->setEnabled(he);
    ui->headerRulerColorButton->setEnabled(he);
    ui->headerSpacingSpinBox->setEnabled(he);

    bool fe = (footer.height > 0);
    ui->prefsTabWidget->setTabEnabled(2, fe);
    ui->footerRulerWitdhSpinBox->setEnabled(fe);
    ui->footerRulerColorButton->setEnabled(fe);
    ui->footerSpacingSpinBox->setEnabled(fe);

    updateMaximumMarginValues( m_reportData->m_pageFormat->formatId(),
                               m_reportData->m_pageFormat->isPortrait() );
}

ReportPreferencesDialog::~ReportPreferencesDialog()
{
    delete ui;
}

void ReportPreferencesDialog::saveDialogGeomtry()
{
    QwDialog::saveDialogGeomtry();

    QSettings settings(m_orgName, m_appName);
    settings.beginGroup(m_className);
    settings.setValue("HeaderTabState", m_headerMainWidget->saveState());
    settings.setValue("FooterTabState", m_footerMainWidget->saveState());
    settings.endGroup();
}

void ReportPreferencesDialog::restoreDialogGeomtry()
{
    QwDialog::restoreDialogGeomtry();

    QSettings settings(m_orgName, m_appName);
    settings.beginGroup(m_className);
    m_headerMainWidget->restoreState(settings.value("HeaderTabState").toByteArray());
    m_footerMainWidget->restoreState(settings.value("FooterTabState").toByteArray());
    settings.endGroup();
}

void ReportPreferencesDialog::on_headerHeightSpinBox_valueChanged( double arg1 )
{
    ui->prefsTabWidget->setTabEnabled(1, arg1 > 0.0001);
    ui->headerRulerWitdhSpinBox->setEnabled(arg1 > 0.0001);
    ui->headerRulerColorButton->setEnabled(arg1 > 0.0001);
}

void ReportPreferencesDialog::on_footerHeightSpinBox_valueChanged( double arg1 )
{
    ui->prefsTabWidget->setTabEnabled(2, arg1 > 0.0001);
    ui->footerRulerWitdhSpinBox->setEnabled(arg1 > 0.0001);
    ui->footerRulerColorButton->setEnabled(arg1 > 0.0001);
}

void ReportPreferencesDialog::on_changeDefaultFontButton_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont( &ok, ui->defaultFontSampleTextEdit->document()->defaultFont(), this );
    if ( ok ) {
        font.setOverline(false);
        font.setStrikeOut(false);
        ui->defaultFontSampleTextEdit->document()->setDefaultFont(font);
        m_reportData->m_reportDocument->setDefaultFont(font);
        m_newDefaultFont = font;
    }
}

void ReportPreferencesDialog::on_okButton_clicked()
{
    Utils::PageFormat::Decoration header;
    header.height = ui->headerHeightSpinBox->value();
    header.ruleColor = m_newHeaderRuleColor;
    header.ruleWidth = ui->headerRulerWitdhSpinBox->value();
    header.spacing = ui->headerSpacingSpinBox->value();

    Utils::PageFormat::Decoration footer;
    footer.height = ui->footerHeightSpinBox->value();
    footer.ruleColor = m_newFooterRuleColor;
    footer.ruleWidth = ui->footerRulerWitdhSpinBox->value();
    footer.spacing = ui->footerSpacingSpinBox->value();

    m_reportData->m_pageFormat->update( ui->paperSizeComboBox->currentIndex(),
                                        ui->portraitButton->isChecked(),
                                        Utils::PageFormat::Margin ( ui->leftMarginSpinBox->value(),
                                                                    ui->topMarginSpinBox->value(),
                                                                    ui->rightMarginSpinBox->value(),
                                                                    ui->bottomMarginSpinBox->value() ),
                                        header, footer );

    m_reportData->m_defaultFont = m_newDefaultFont;
    m_reportData->m_headerHtml = m_headerEditor->toHtml();
    m_reportData->m_footerHtml = m_footerEditor->toHtml();

    m_reportData->m_reportDocument->setModified(true);
    QwDialog::accept();
}

void ReportPreferencesDialog::on_cancelButton_clicked()
{
    QwDialog::reject();
}

void ReportPreferencesDialog::on_paperSizeComboBox_currentIndexChanged( int index )
{
    updateMaximumMarginValues(index, ui->portraitButton->isChecked());
}

void ReportPreferencesDialog::on_portraitButton_toggled( bool checked )
{
    updateMaximumMarginValues(ui->paperSizeComboBox->currentIndex(), checked);
}

void ReportPreferencesDialog::on_lanscapeButton_toggled( bool checked )
{
    updateMaximumMarginValues(ui->paperSizeComboBox->currentIndex(), !checked);
}

void ReportPreferencesDialog::updateMaximumMarginValues( int formatId, bool isPortrait )
{
    QSizeF paperSize = Utils::PageFormat::sizeForFormatId(formatId, isPortrait);

    double mw = paperSize.width() / 3.0;
    double mh = paperSize.height() / 3.0;

    ui->leftMarginSpinBox->setMaximum(mw);
    ui->rightMarginSpinBox->setMaximum(mw);
    ui->topMarginSpinBox->setMaximum(mh);
    ui->bottomMarginSpinBox->setMaximum(mh);
}

void ReportPreferencesDialog::updateColorButton( QToolButton *button, const QColor &color )
{
    if ( color.isValid() ) {
        QPixmap pix(16, 16);
        pix.fill(color);
        button->setIcon(QIcon(pix));
    } else {
        button->setIcon(QIcon());
    }
}

void ReportPreferencesDialog::on_headerRulerColorButton_clicked()
{
    QColor newColor = QColorDialog::getColor(m_newHeaderRuleColor, this);
    if ( newColor.isValid() ) {
        m_newHeaderRuleColor = newColor;
        updateColorButton(ui->headerRulerColorButton, newColor);
    }
}

void ReportPreferencesDialog::on_footerRulerColorButton_clicked()
{
    QColor newColor = QColorDialog::getColor(m_newFooterRuleColor, this);
    if ( newColor.isValid() ) {
        m_newFooterRuleColor = newColor;
        updateColorButton(ui->footerRulerColorButton, newColor);
    }
}

}   // namespace RPML
