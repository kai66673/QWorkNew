#include <QFormLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QLineEdit>
#include <QDateEdit>
#include <QTimeEdit>
#include <QDateTimeEdit>
#include <QDoubleValidator>

#include "InputParametersDialog.h"

InputParametersDialog::InputParametersDialog( QList<RPML::ReportInputParameter *> *paramteters, const QString &title, QWidget *parent)
    : QDialog(parent)
    , m_paramteters(paramteters)
{
    if ( title.isEmpty() )
        setWindowTitle(tr("Input Parameters"));
    else
        setWindowTitle(title);


    QPushButton *okButton = new QPushButton(tr("Ok"));
    okButton->setDefault(true);
    connect(okButton, SIGNAL(clicked()), this, SLOT(onOk()));

    QPushButton *cancelButton = new QPushButton(tr("Cancel"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
//    formLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
    formLayout->setFormAlignment(Qt::AlignCenter);
    formLayout->setLabelAlignment(Qt::AlignRight);
    foreach ( RPML::ReportInputParameter *param, *m_paramteters ) {
        QHBoxLayout *widgetLayout = new QHBoxLayout();
        widgetLayout->setSpacing(1);
        widgetLayout->setContentsMargins(0, 0, 0, 0);
        QCheckBox *checkBox = new QCheckBox();
        checkBox->setChecked(true);
        checkBox->setFocusPolicy(Qt::NoFocus);
        if ( param->mandatory == RPML::ReportInputParameter::Required )
            checkBox->setEnabled(false);
        QWidget *editor;
        switch ( param->type ) {
            case RPML::ReportInputParameter::String: {
                QLineEdit *numEditor = new QLineEdit();
                numEditor->setText(param->defaultValue.toString());
                editor = numEditor;
                break;
            }
            case RPML::ReportInputParameter::Numeric: {
                QLineEdit *numEditor = new QLineEdit();
                numEditor->setValidator(new QDoubleValidator);
                numEditor->setText(param->defaultValue.toString());
                editor = numEditor;
                break;
            }
            case RPML::ReportInputParameter::Date: {
                QDateEdit *dateEdit = new QDateEdit;
                if ( !param->defaultValue.toString().isEmpty() )
                    dateEdit->setDate(QDate::fromString(param->defaultValue.toString(), "dd.MM.yyyy"));
                else
                    dateEdit->setDate(QDate::currentDate());
                editor = dateEdit;
                break;
            }
            case RPML::ReportInputParameter::Time: {
                QTimeEdit *timeEdit = new QTimeEdit;
                if ( !param->defaultValue.toString().isEmpty() )
                    timeEdit->setTime(QTime::fromString(param->defaultValue.toString(), "hh-mm-ss"));
                else
                    timeEdit->setTime(QTime::fromString("00-00-00", "hh-mm-ss"));
                editor = timeEdit;
                break;
            }
            case RPML::ReportInputParameter::DateTime: {
                QDateTimeEdit *dateTimeEdit = new QDateTimeEdit;
                if ( !param->defaultValue.toString().isEmpty() )
                    dateTimeEdit->setDateTime(QDateTime::fromString(param->defaultValue.toString(), "dd.MM.yyyy hh-mm-ss"));
                else {
                    dateTimeEdit->setDate(QDate::currentDate());
                    dateTimeEdit->setTime(QTime::fromString("00-00-00", "hh-mm-ss"));
                }
                editor = dateTimeEdit;
                break;
            }
            default:
                editor = new QWidget();
        }
        QSizePolicy sp(editor->sizePolicy());
        sp.setHorizontalStretch(1);
        editor->setSizePolicy(sp);
        widgetLayout->addWidget(editor);
        widgetLayout->addWidget(checkBox);
        QWidget *widget = new QWidget();
        widget->setLayout(widgetLayout);
        m_editors.push_back(editor);
        m_switchers.push_back(checkBox);
        connect(checkBox, SIGNAL(clicked(bool)), editor, SLOT(setEnabled(bool)));
        formLayout->addRow(param->description + ":", widget);
    }

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(formLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);
}

void InputParametersDialog::onOk()
{
    if ( !checkValues() )
        return;
    QDialog::accept();
}

bool InputParametersDialog::checkValues()
{
    m_values.clear();

    for (int i = 0; i < m_paramteters->size(); i++) {
        RPML::ReportInputParameter *param = m_paramteters->at(i);
        QWidget *editor = m_editors[i];
        if ( !m_switchers[i]->isChecked() )
            m_values.push_back(RPML::InputParameterValue());
        else {
            switch ( param->type ) {
                case RPML::ReportInputParameter::String:
                    if ( QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor) ) {
                        m_values.push_back(RPML::InputParameterValue(false, lineEdit->text().trimmed()));
                        break;
                    }
                    return false;
                case RPML::ReportInputParameter::Numeric:
                    if ( QLineEdit *lineEdit = qobject_cast<QLineEdit *>(editor) ) {
                        if ( lineEdit->text().isEmpty() ) {
                            lineEdit->setFocus();
                            return false;
                        }
                        m_values.push_back(RPML::InputParameterValue(false, lineEdit->text().trimmed()));
                        break;
                    }
                    return false;
                case RPML::ReportInputParameter::Date:
                    if ( QDateEdit *dateEdit = qobject_cast<QDateEdit *>(editor) ) {
                        m_values.push_back(RPML::InputParameterValue(false, dateEdit->date()));
                        break;
                    }
                    return false;
                case RPML::ReportInputParameter::Time:
                    if ( QTimeEdit *timeEdit = qobject_cast<QTimeEdit *>(editor) ) {
                        m_values.push_back(RPML::InputParameterValue(false, timeEdit->time()));
                        break;
                    }
                    return false;
                case RPML::ReportInputParameter::DateTime:
                    if ( QDateTimeEdit *dateTimeEdit = qobject_cast<QDateTimeEdit *>(editor) ) {
                        m_values.push_back(RPML::InputParameterValue(false, dateTimeEdit->dateTime()));
                        break;
                    }
                    return false;
                default:
                    return false;
            }
        }
    }

    return true;
}
