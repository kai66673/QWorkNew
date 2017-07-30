#include "CppEditorSettingsWidget.h"
#include "ui_CppEditorSettingsWidget.h"

CppEditorSettingsWidget::CppEditorSettingsWidget( const QMap <QString, QVariant> &valuesMap, QWidget *parent )
    : ISettingsTabPage(parent)
    , ui(new Ui::CppEditorSettingsWidget)
{
    ui->setupUi(this);

    if (  valuesMap["ApplyToAll"].toBool() ) {
        ui->SHApplyToAllRadioButton->setChecked(true);
        ui->minSizeSpinBox->setEnabled(false);
    }
    else {
        ui->SHApplyToLessThenRadioButton->setChecked(true);
        ui->minSizeSpinBox->setEnabled(true);
    }
    ui->minSizeSpinBox->setValue(valuesMap["ApplyToLessSize"].toInt());

    connect(ui->SHApplyToLessThenRadioButton, SIGNAL(toggled(bool)), this, SLOT(onApplyToLess(bool)));
}

CppEditorSettingsWidget::~CppEditorSettingsWidget()
{
    delete ui;
}

void CppEditorSettingsWidget::changeEvent(QEvent *e)
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

void CppEditorSettingsWidget::fillSettings( QMap<QString, QVariant> &valueMap ) const
{
    valueMap["ApplyToAll"]      = ui->SHApplyToAllRadioButton->isChecked();
    valueMap["ApplyToLessSize"] = ui->minSizeSpinBox->value();
}

void CppEditorSettingsWidget::onApplyToLess( bool onoff )
{
    ui->minSizeSpinBox->setEnabled(onoff);
}
