#include "TextEditorSettingsWidget.h"
#include "ui_TextEditorSettingsWidget.h"
#include "HighlighterFormatsManager.h"

TextEditorSettingsWidget::TextEditorSettingsWidget( const QMap <QString, QVariant> &valuesMap, QWidget *parent )
    : ISettingsTabPage(parent)
    , ui(new Ui::TextEditorSettingsWidget)
{
    ui->setupUi(this);

    /// Font & Colors
    QFont font = qvariant_cast<QFont>(valuesMap["Font"]);
    ui->fontComboBox->setCurrentFont(font);
    ui->fontSizeSpinBox->setValue(font.pointSize());
    ui->antialiasCheckBox->setChecked(valuesMap["Antialias"].toBool());
    ui->zoomSpinBox->setValue(valuesMap["Zoom"].toInt());

    /// Tabs & Indentation
    ui->tabsToSpacesCheckBox->setChecked(valuesMap["TabToSpaces"].toBool());
    ui->autoIndentCheckBox->setChecked(valuesMap["AutoIndentation"].toBool());
    ui->backspaceIndentCheckBox->setChecked(valuesMap["BackspaceIndentation"].toBool());
    ui->tabSizeSpinBox->setValue(valuesMap["TabSize"].toInt());
    ui->indentSizeSpinBox->setValue(valuesMap["IndentSize"].toInt());

    /// Behavior
    ui->mouseNavigationCheckBox->setChecked(valuesMap["MouseNavigation"].toBool());
    ui->mouseZoomingcheckBox->setChecked(valuesMap["ScrollWheelZooming"].toBool());
    ui->activateOnMouseEnterCheckBox->setChecked(valuesMap["ActivateDocOnMouseEnter"].toBool());

    /// Text Wrapping
    ui->textWrappingCheckBox->setChecked(valuesMap["Wrapping"].toBool());
    ui->showRightMarginCheckBox->setChecked(valuesMap["ShowRightMargin"].toBool());
    ui->rightMarginColumnSpinBox->setValue(valuesMap["RightMarginColumn"].toInt());

    ui->rightMarginColumnSpinBox->setEnabled(ui->showRightMarginCheckBox->isChecked());
    connect(ui->showRightMarginCheckBox, SIGNAL(clicked(bool)), ui->rightMarginColumnSpinBox, SLOT(setEnabled(bool)));

    /// Display Settings
    ui->showLineNumbersCheckBox->setChecked(valuesMap["ShowLineNumbers"].toBool());
    ui->showFoldingMarkersCheckBox->setChecked(valuesMap["ShowFoldingMarkers"].toBool());
    ui->markTextChangesCheckBox->setChecked(valuesMap["MarkTextChanges"].toBool());
    ui->showWitespacesCheckBox->setChecked(valuesMap["ShowWhitespaces"].toBool());
    ui->centerCursorOnScrollCheckBox->setChecked(valuesMap["CenterCursorOnScroll"].toBool());
    ui->highlightCurrentLineCheckBox->setChecked(valuesMap["HighlightCurrentLine"].toBool());
    ui->highlightBlocksCheckBox->setChecked(valuesMap["HighlightBlocks"].toBool());
    ui->animateMatchingParenthesesCheckBox->setChecked(valuesMap["AnimateMatchingParentheses"].toBool());

    m_highlighterConfigureWidget = HighlighterFormatsManager::highlighterFormatsConfigureWidget();
    if ( m_highlighterConfigureWidget ) {
        ui->tabWidget->addTab(m_highlighterConfigureWidget, tr("Syntax Highlighter"));
        connect(this, SIGNAL(fontChanged(QFont)), m_highlighterConfigureWidget, SIGNAL(fontChanged(QFont)));
        connect(ui->fontComboBox, SIGNAL(currentFontChanged(QFont)), this, SLOT(onFontChanged()));
        connect(ui->fontSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(onFontChanged()));
    }

}

TextEditorSettingsWidget::~TextEditorSettingsWidget()
{
    delete ui;
}

void TextEditorSettingsWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void TextEditorSettingsWidget::onFontChanged()
{
    QFont font = ui->fontComboBox->currentFont();
    font.setPointSize(ui->fontSizeSpinBox->value());
    emit fontChanged(font);
}

void TextEditorSettingsWidget::fillSettings( QMap<QString, QVariant> &valuesMap ) const
{
    /// Font & Colors
    QFont font = ui->fontComboBox->currentFont();
    font.setPointSize(ui->fontSizeSpinBox->value());
    valuesMap["Font"] = font;
    valuesMap["Antialias"] = ui->antialiasCheckBox->isChecked();
    valuesMap["Zoom"] = ui->zoomSpinBox->value();

    /// Tab & Indentation
    valuesMap["TabToSpaces"]                = ui->tabsToSpacesCheckBox->isChecked();
    valuesMap["AutoIndentation"]            = ui->autoIndentCheckBox->isChecked();
    valuesMap["BackspaceIndentation"]       = ui->backspaceIndentCheckBox->isChecked();
    valuesMap["TabSize"]                    = ui->tabSizeSpinBox->value();
    valuesMap["IndentSize"]                 = ui->indentSizeSpinBox->value();

    /// Behavior
    valuesMap["MouseNavigation"]            = ui->mouseNavigationCheckBox->isChecked();
    valuesMap["ScrollWheelZooming"]         = ui->mouseZoomingcheckBox->isChecked();
    valuesMap["ActivateDocOnMouseEnter"]    = ui->activateOnMouseEnterCheckBox->isChecked();

    /// Text Wrapping
    valuesMap["Wrapping"]                   = ui->textWrappingCheckBox->isChecked();
    valuesMap["ShowRightMargin"]            = ui->showRightMarginCheckBox->isChecked();
    valuesMap["RightMarginColumn"]          = ui->rightMarginColumnSpinBox->value();

    /// Display Settings
    valuesMap["ShowLineNumbers"]            = ui->showLineNumbersCheckBox->isChecked();
    valuesMap["ShowFoldingMarkers"]         = ui->showFoldingMarkersCheckBox->isChecked();
    valuesMap["MarkTextChanges"]            = ui->markTextChangesCheckBox->isChecked();
    valuesMap["ShowWhitespaces"]            = ui->showWitespacesCheckBox->isChecked();
    valuesMap["CenterCursorOnScroll"]       = ui->centerCursorOnScrollCheckBox->isChecked();
    valuesMap["HighlightCurrentLine"]       = ui->highlightCurrentLineCheckBox->isChecked();
    valuesMap["HighlightBlocks"]            = ui->highlightBlocksCheckBox->isChecked();
    valuesMap["AnimateMatchingParentheses"] = ui->animateMatchingParenthesesCheckBox->isChecked();

    if ( m_highlighterConfigureWidget )
        m_highlighterConfigureWidget->save();
}
