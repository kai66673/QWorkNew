#ifndef KARELEDITORSETTINGSWIDGET_H
#define KARELEDITORSETTINGSWIDGET_H

#include <QWidget>

#include "SettingsManager.h"

namespace Ui {
class KarelEditorSettingsWidget;
}

class KarelEditorSettingsWidget : public ISettingsTabPage
{
    Q_OBJECT

public:
    explicit KarelEditorSettingsWidget(const QMap <QString, QVariant> &valuesMap,QWidget *parent = 0);
    ~KarelEditorSettingsWidget();

    virtual void fillSettings(QMap<QString, QVariant> &valueMap) const;

private slots:
    void onApplyOnSaveChanged(bool onoff);

private:
    Ui::KarelEditorSettingsWidget *ui;
};

#endif // KARELEDITORSETTINGSWIDGET_H
