#ifndef CPPEDITORSETTINGSWIDGET_H
#define CPPEDITORSETTINGSWIDGET_H

#include <QWidget>

#include "SettingsManager.h"

namespace Ui {
    class CppEditorSettingsWidget;
}

class CppEditorSettingsWidget: public ISettingsTabPage
{
    Q_OBJECT

public:
    CppEditorSettingsWidget( const QMap <QString, QVariant> &valuesMap, QWidget *parent = 0 );
    ~CppEditorSettingsWidget();

    void fillSettings( QMap <QString, QVariant> &valueMap ) const;

private slots:
    void onApplyToLess( bool onoff );

protected:
    void changeEvent(QEvent *e);

private:
    Ui::CppEditorSettingsWidget *ui;
};

#endif // CPPEDITORSETTINGSWIDGET_H
