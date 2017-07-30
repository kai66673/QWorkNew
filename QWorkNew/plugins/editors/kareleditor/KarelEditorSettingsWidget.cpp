#include "KarelEditorSettingsWidget.h"
#include "ui_KarelEditorSettingsWidget.h"

KarelEditorSettingsWidget::KarelEditorSettingsWidget(const QMap<QString, QVariant> &valuesMap, QWidget *parent)
    : ISettingsTabPage(parent)
    , ui(new Ui::KarelEditorSettingsWidget)
{
    ui->setupUi(this);

    ui->applyOnSaveCheckBox->setChecked(valuesMap["ApplyOnSave"].toBool());

    ui->builtingCheckBox->setChecked(valuesMap["ApplyToBuiltings"].toBool());
    ui->keywordsCheckBox->setChecked(valuesMap["ApplyToKeywords"].toBool());
    ui->typesCheckBox->setChecked(valuesMap["ApplyToTypes"].toBool());
    ui->directivesCheckBox->setChecked(valuesMap["ApplyToDirectives"].toBool());
    ui->systemsCheckBox->setChecked(valuesMap["ApplyToSystems"].toBool());

    ui->builtingsComboBox->setCurrentIndex(valuesMap["BuiltingsCaseType"].toInt());
    ui->keywordsComboBox->setCurrentIndex(valuesMap["KeywordsCaseType"].toInt());
    ui->typesComboBox->setCurrentIndex(valuesMap["TypesCaseType"].toInt());
    ui->directivesComboBox->setCurrentIndex(valuesMap["DirectivesCaseType"].toInt());
    ui->systemsComboBox->setCurrentIndex(valuesMap["SystemsCaseType"].toInt());

    onApplyOnSaveChanged(ui->applyOnSaveCheckBox->isChecked());

    connect(ui->applyOnSaveCheckBox, SIGNAL(toggled(bool)), this, SLOT(onApplyOnSaveChanged(bool)));

    connect(ui->builtingCheckBox, SIGNAL(toggled(bool)), ui->builtingsComboBox, SLOT(setEnabled(bool)));
    connect(ui->keywordsCheckBox, SIGNAL(toggled(bool)), ui->keywordsComboBox, SLOT(setEnabled(bool)));
    connect(ui->typesCheckBox, SIGNAL(toggled(bool)), ui->typesComboBox, SLOT(setEnabled(bool)));
    connect(ui->directivesCheckBox, SIGNAL(toggled(bool)), ui->directivesComboBox, SLOT(setEnabled(bool)));
    connect(ui->systemsCheckBox, SIGNAL(toggled(bool)), ui->systemsComboBox, SLOT(setEnabled(bool)));
}

KarelEditorSettingsWidget::~KarelEditorSettingsWidget()
{
    delete ui;
}

void KarelEditorSettingsWidget::fillSettings(QMap<QString, QVariant> &valueMap) const
{
    valueMap["ApplyOnSave"] = ui->applyOnSaveCheckBox->isChecked();

    valueMap["ApplyToBuiltings"] = ui->builtingCheckBox->isChecked();
    valueMap["ApplyToKeywords"] = ui->keywordsCheckBox->isChecked();
    valueMap["ApplyToTypes"] = ui->typesCheckBox->isChecked();
    valueMap["ApplyToDirectives"] = ui->directivesCheckBox->isChecked();
    valueMap["ApplyToSystems"] = ui->systemsCheckBox->isChecked();

    valueMap["BuiltingsCaseType"] = ui->builtingsComboBox->currentIndex();
    valueMap["KeywordsCaseType"] = ui->keywordsComboBox->currentIndex();
    valueMap["TypesCaseType"] = ui->typesComboBox->currentIndex();
    valueMap["DirectivesCaseType"] = ui->directivesComboBox->currentIndex();
    valueMap["SystemsCaseType"] = ui->systemsComboBox->currentIndex();
}

void KarelEditorSettingsWidget::onApplyOnSaveChanged(bool onoff)
{
    ui->builtingCheckBox->setEnabled(onoff);
    ui->keywordsCheckBox->setEnabled(onoff);
    ui->typesCheckBox->setEnabled(onoff);
    ui->directivesCheckBox->setEnabled(onoff);
    ui->systemsCheckBox->setEnabled(onoff);

    if ( onoff ) {
        ui->builtingsComboBox->setEnabled(ui->builtingCheckBox->isChecked());
        ui->keywordsComboBox->setEnabled(ui->keywordsCheckBox->isChecked());
        ui->typesComboBox->setEnabled(ui->typesCheckBox->isChecked());
        ui->directivesComboBox->setEnabled(ui->directivesCheckBox->isChecked());
        ui->systemsComboBox->setEnabled(ui->systemsCheckBox->isChecked());
    } else {
        ui->builtingsComboBox->setEnabled(false);
        ui->keywordsComboBox->setEnabled(false);
        ui->typesComboBox->setEnabled(false);
        ui->directivesComboBox->setEnabled(false);
        ui->systemsComboBox->setEnabled(false);
    }
}
