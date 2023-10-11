#include <QtGui>
#include <QListWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QHBoxLayout>

#include "SettingsManager.h"
#include "SettingsDialog.h"

SettingsDialog::SettingsDialog( QMap <QString, ISettings *> *settingsMap_, QWidget *parent )
    : QDialog(parent)
    , settingsMap(settingsMap_)
{
    contentsWidget = new QListWidget;
    contentsWidget->setViewMode(QListView::IconMode);
    contentsWidget->setIconSize(QSize(128, 128));
    contentsWidget->setMovement(QListView::Static);
    contentsWidget->setMaximumWidth(164);
    contentsWidget->setMinimumWidth(164);
    contentsWidget->setMinimumHeight(464);
    contentsWidget->setSpacing(5);

    m_pagesStackedWidget = new QStackedWidget;

    foreach (ISettings *settings, *settingsMap) {
        ISettingsTabPage *tabPage = settings->createSettingsTabPage(this);
        if ( tabPage ) {
            QListWidgetItem *item = new QListWidgetItem(contentsWidget);
            item->setIcon(QIcon(settings->iconName()));
            item->setText(settings->title());
            item->setTextAlignment(Qt::AlignHCenter);
            item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            m_pagesStackedWidget->addWidget(tabPage);
            settingsList.append(settings);
            m_tabPages.append(tabPage);
        }
    }

    QPushButton *okButton = new QPushButton(tr("Ok"));
    QPushButton *cancelButton = new QPushButton(tr("Cancel"));

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(okButton);
    buttonsLayout->addWidget(cancelButton);

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(contentsWidget);
    horizontalLayout->addWidget(m_pagesStackedWidget);

    QVBoxLayout *verticalLayout = new QVBoxLayout;
    verticalLayout->addLayout(horizontalLayout);
    verticalLayout->addLayout(buttonsLayout);

    setLayout(verticalLayout);

    setWindowTitle(tr("Settings"));

    connect(contentsWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(onSettingsPageChanged(QListWidgetItem*,QListWidgetItem*)));

    connect(okButton, SIGNAL(clicked()), this, SLOT(onAccept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    contentsWidget->setCurrentRow(0);
}

SettingsDialog::~SettingsDialog()
{
    settingsList.clear();
}

void SettingsDialog::onSettingsPageChanged( QListWidgetItem *current, QListWidgetItem *previous )
{
    if ( !current )
        current = previous;

    m_pagesStackedWidget->setCurrentIndex(contentsWidget->row(current));
}

void SettingsDialog::onAccept()
{
    for (int i = 0; i < settingsList.size(); i++)
        settingsList[i]->applySettings(m_tabPages[i]);

    accept();
}
