#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QMap>
#include <QVariant>

#include "SettingsManager.h"

QT_BEGIN_NAMESPACE
class QListWidget;
class QListWidgetItem;
class QStackedWidget;
QT_END_NAMESPACE

class ISettings;

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    SettingsDialog( QMap <QString, ISettings *> *settingsMap_, QWidget *parent = 0 );
    ~SettingsDialog();

private slots:
    void onSettingsPageChanged( QListWidgetItem *current, QListWidgetItem *previous );
    void onAccept();

private:
    QMap <QString, ISettings *> *settingsMap;
    QList <ISettings *> settingsList;

    QListWidget *contentsWidget;
    QStackedWidget *m_pagesStackedWidget;
    QList<ISettingsTabPage *> m_tabPages;
};

#endif // SETTINGSDIALOG_H
