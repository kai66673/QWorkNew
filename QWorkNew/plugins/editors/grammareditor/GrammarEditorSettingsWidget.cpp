#include "GrammarEditorSettingsWidget.h"
#include "ui_GrammarEditorSettingsWidget.h"

GrammarEditorSettingsWidget::GrammarEditorSettingsWidget( const QMap<QString, QVariant> &valuesMap, QWidget *parent )
    : ISettingsTabPage(parent)
    , ui(new Ui::GrammarEditorSettingsWidget)
{
    Q_UNUSED(valuesMap)
    ui->setupUi(this);

    ui->commandLineEdit->setText(valuesMap["Command"].toString());
    ui->argumentsLineEdit->setText(valuesMap["Arguments"].toString());
    ui->runOnSaveCheckBox->setChecked(valuesMap["RunOnSave"].toBool());
}

GrammarEditorSettingsWidget::~GrammarEditorSettingsWidget()
{
    delete ui;
}

void GrammarEditorSettingsWidget::fillSettings( QMap<QString, QVariant> &valueMap ) const
{
    valueMap["Command"]     = ui->commandLineEdit->text().trimmed();
    valueMap["Arguments"]   = ui->argumentsLineEdit->text().trimmed();
    valueMap["RunOnSave"]   = ui->runOnSaveCheckBox->isChecked();
}
