#include <QFontDialog>
#include <QColorDialog>
#include <QToolButton>

#include "Core.h"
#include "QueryViewSettingsWidget.h"
#include "ui_QueryViewSettingsWidget.h"

QueryViewSettingsWidget::QueryViewSettingsWidget( const QMap <QString, QVariant> &valuesMap, QWidget *parent )
    : ISettingsTabPage(parent)
    , ui(new Ui::QueryViewSettingsWidget)
{
    ui->setupUi(this);

    ui->maxRowsSpinBox->setValue(valuesMap["MaxRowsCount"].toInt());
    ui->defaultRowHeightSpinBox->setValue(valuesMap["DefaultRowHeight"].toInt());
    ui->showVerticalHeaderCheckBox->setChecked(valuesMap["ShowVerticalHeader"].toBool());
    ui->resizeColumnsToContentsCheckBox->setChecked(valuesMap["AutoResizeColumns"].toBool());
    ui->maxColumnWidthSpinBox->setValue(valuesMap["MaxColumnsWidth"].toInt());
    ui->maxColumnWidthSpinBox->setEnabled(ui->resizeColumnsToContentsCheckBox->isChecked());
    ui->showNullcheckBox->setChecked(valuesMap["ShowNull"].toBool());
    ui->sqlHistoryCheckBox->setChecked(valuesMap["SqlHistory"].toBool());
    connect(ui->resizeColumnsToContentsCheckBox, SIGNAL(clicked(bool)), ui->maxColumnWidthSpinBox, SLOT(setEnabled(bool)));

    font = qvariant_cast<QFont>(valuesMap["Font"]);
    ui->alternatingRowsCheckBox->setChecked(valuesMap["AlternatingRows"].toBool());
    backgroundColor1 = qvariant_cast<QColor>(valuesMap["BackgroundColor1"]);
    backgroundColor2 = qvariant_cast<QColor>(valuesMap["BackgroundColor2"]);
    foregroundColor  = qvariant_cast<QColor>(valuesMap["ForegroundColor"]);
    ui->color2ToolButton->setEnabled(ui->alternatingRowsCheckBox->isChecked());
    connect(ui->alternatingRowsCheckBox, SIGNAL(clicked(bool)), ui->color2ToolButton, SLOT(setEnabled(bool)));

    ui->splitterLineEdit->setText(valuesMap["SplitterSymbol"].toString());

    ui->color0ToolButton->setStyleSheet(QString("QToolButton { background-color: %1; }").arg(foregroundColor.name()));
    ui->color1ToolButton->setStyleSheet(QString("QToolButton { background-color: %1; }").arg(backgroundColor1.name()));
    ui->color2ToolButton->setStyleSheet(QString("QToolButton { background-color: %1; }").arg(backgroundColor2.name()));

    connect(ui->fontPushButton, SIGNAL(clicked()), this, SLOT(onFontClicked()));

    connect(ui->color0ToolButton, SIGNAL(clicked()), this, SLOT(onForegroundColorClicked()));
    connect(ui->color1ToolButton, SIGNAL(clicked()), this, SLOT(onBackgroundColor1Clicked()));
    connect(ui->color2ToolButton, SIGNAL(clicked()), this, SLOT(onBackgroundColor2Clicked()));
}

QueryViewSettingsWidget::~QueryViewSettingsWidget()
{
    delete ui;
}

void QueryViewSettingsWidget::changeEvent(QEvent *e)
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

void QueryViewSettingsWidget::fillSettings( QMap <QString, QVariant> &valuesMap ) const
{
    valuesMap["MaxRowsCount"]           = ui->maxRowsSpinBox->value();
    valuesMap["DefaultRowHeight"]       = ui->defaultRowHeightSpinBox->value();
    valuesMap["ShowVerticalHeader"]     = ui->showVerticalHeaderCheckBox->isChecked();
    valuesMap["AutoResizeColumns"]      = ui->resizeColumnsToContentsCheckBox->isChecked();
    valuesMap["MaxColumnsWidth"]        = ui->maxColumnWidthSpinBox->value();
    valuesMap["Font"]                   = font;
    valuesMap["AlternatingRows"]        = ui->alternatingRowsCheckBox->isChecked();
    valuesMap["BackgroundColor1"]       = backgroundColor1;
    valuesMap["BackgroundColor2"]       = backgroundColor2;
    valuesMap["ForegroundColor"]        = foregroundColor;
    valuesMap["SplitterSymbol"]         = ui->splitterLineEdit->text();
    valuesMap["ShowNull"]               = ui->showNullcheckBox->isChecked();
    valuesMap["SqlHistory"]             = ui->sqlHistoryCheckBox->isChecked();

}

void QueryViewSettingsWidget::onFontClicked()
{
    bool ok;
    QFont newFont = QFontDialog::getFont(&ok, font, Core::Storage::mainWindow()->widget());
    if ( ok )
        font = newFont;
}

void QueryViewSettingsWidget::onForegroundColorClicked()
{
    QColor newColor = QColorDialog::getColor(foregroundColor, Core::Storage::mainWindow()->widget());
    if ( newColor.isValid() ) {
        foregroundColor = newColor;
        ui->color0ToolButton->setStyleSheet(QString("QToolButton { background-color: %1; }").arg(foregroundColor.name()));
    }
}

void QueryViewSettingsWidget::onBackgroundColor1Clicked()
{
    QColor newColor = QColorDialog::getColor(backgroundColor1, Core::Storage::mainWindow()->widget());
    if ( newColor.isValid() ) {
        backgroundColor1 = newColor;
        ui->color1ToolButton->setStyleSheet(QString("QToolButton { background-color: %1; }").arg(backgroundColor1.name()));
    }
}

void QueryViewSettingsWidget::onBackgroundColor2Clicked()
{
    QColor newColor = QColorDialog::getColor(backgroundColor2, Core::Storage::mainWindow()->widget());
    if ( newColor.isValid() ) {
        backgroundColor2 = newColor;
        ui->color2ToolButton->setStyleSheet(QString("QToolButton { background-color: %1; }").arg(backgroundColor2.name()));
    }
}
