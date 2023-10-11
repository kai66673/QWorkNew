#include "LogFilterDialog.h"
#include "ui_LogFilterDialog.h"

LogFilterDialog::LogFilterDialog( Core::LogFilter *filter, QSet<QString> &registeredTags, QWidget *parent )
    : QDialog(parent)
    , ui(new Ui::LogFilterDialog)
    , m_filter(filter)
{
    ui->setupUi(this);

    ui->informationsCheckBox->setChecked(m_filter->informations);
    ui->warningsCheckBox->setChecked(m_filter->warnings);
    ui->errorsCheckBox->setChecked(m_filter->errors);

    foreach ( const QString &tagId, registeredTags )
        ui->registeredTagComboBox->addItem(tagId);

    switch ( m_filter->tagFilterType ) {
        case Core::LogFilter::AllTags:
            ui->allTagsRadioButton->setChecked(true);
            break;
        case Core::LogFilter::AllRegisteredTags:
            ui->allRegisteredTagsRadioButton->setChecked(true);
            break;
        case Core::LogFilter::AllUnregisteredTags:
            ui->allUnregisteredTagsRadioButton->setChecked(true);
            break;
        case Core::LogFilter::AnyRegisteredTag:
            ui->registeredTagRadioButton->setChecked(true);
            ui->registeredTagComboBox->setEnabled(true);
            break;
        case Core::LogFilter::AnyTag:
            ui->anyTagRadioButton->setChecked(true);
            ui->anyTagLineEdit->setEnabled(true);
            break;
    }

    ui->registeredTagComboBox->setCurrentIndex(ui->registeredTagComboBox->findText(m_filter->selectedTagId, Qt::MatchFixedString));
    ui->anyTagLineEdit->setText(m_filter->commonTagId);

    connect(ui->okPushButton, SIGNAL(clicked()), this, SLOT(onAccept()));

    connect(ui->registeredTagRadioButton,   SIGNAL(toggled(bool)), ui->registeredTagComboBox,   SLOT(setEnabled(bool)));
    connect(ui->anyTagRadioButton,          SIGNAL(toggled(bool)), ui->anyTagLineEdit,          SLOT(setEnabled(bool)));
}

LogFilterDialog::~LogFilterDialog()
{
    delete ui;
}

void LogFilterDialog::onAccept()
{
    m_filter->informations = ui->informationsCheckBox->isChecked();
    m_filter->warnings = ui->warningsCheckBox->isChecked();
    m_filter->errors = ui->errorsCheckBox->isChecked();

    if ( ui->allTagsRadioButton->isChecked() ) {
        m_filter->tagFilterType = Core::LogFilter::AllTags;
    } else if ( ui->allRegisteredTagsRadioButton->isChecked() ) {
        m_filter->tagFilterType = Core::LogFilter::AllRegisteredTags;
    } else if ( ui->allUnregisteredTagsRadioButton->isChecked() ) {
        m_filter->tagFilterType = Core::LogFilter::AllUnregisteredTags;
    } else if ( ui->registeredTagRadioButton->isChecked() ) {
        m_filter->tagFilterType = Core::LogFilter::AnyRegisteredTag;
    } else if ( ui->anyTagRadioButton->isChecked() ) {
        m_filter->tagFilterType = Core::LogFilter::AnyTag;
    }

    m_filter->selectedTagId = ui->registeredTagComboBox->currentText();
    m_filter->commonTagId = ui->anyTagLineEdit->text().trimmed();

    accept();
}
