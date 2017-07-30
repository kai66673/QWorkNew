#include <QVBoxLayout>
#include <QVariant>

#include "BaseTextSearchParamsWidget.h"
#include "SettingsManager.h"

BaseTextSearchParamsWidget::BaseTextSearchParamsWidget(QWidget *parent) :
    QWidget(parent)
{
    m_caseSensetiveCheckBox    = new QCheckBox(QObject::tr("&Case Sensitive"));
    m_wholeWordsOnlyCheckBox   = new QCheckBox(QObject::tr("&Whole Words Only"));
    m_useRegExpCheckBox        = new QCheckBox(QObject::tr("Use &Regular Expressions"));

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(m_caseSensetiveCheckBox);
    layout->addWidget(m_wholeWordsOnlyCheckBox);
    layout->addWidget(m_useRegExpCheckBox);
    layout->addStretch();

    setLayout(layout);

    bool value;
    SettingsManager *settingsManager = SettingsManager::instance();

    value = settingsManager->value("Find_Base", "CaseSensetive").toBool();
    m_caseSensetiveCheckBox->setChecked(value);

    value = settingsManager->value("Find_Base", "WholeWordsOnly").toBool();
    m_wholeWordsOnlyCheckBox->setChecked(value);

    value = settingsManager->value("Find_Base", "UseRegularExpressions").toBool();
    m_useRegExpCheckBox->setChecked(value);
}
