#ifndef GRAMMAREDITORSETTINGSWIDGET_H
#define GRAMMAREDITORSETTINGSWIDGET_H

#include <QWidget>

#include "SettingsManager.h"

namespace Ui {
class GrammarEditorSettingsWidget;
}

class GrammarEditorSettingsWidget: public ISettingsTabPage
{
    Q_OBJECT

public:
    explicit GrammarEditorSettingsWidget( const QMap <QString, QVariant> &valuesMap, QWidget *parent = 0 );
    ~GrammarEditorSettingsWidget();

    void fillSettings( QMap <QString, QVariant> &valueMap ) const;

private:
    Ui::GrammarEditorSettingsWidget *ui;
};

#endif // GRAMMAREDITORSETTINGSWIDGET_H
