#ifndef TEXTEDITORSETTINGSWIDGET_H
#define TEXTEDITORSETTINGSWIDGET_H

#include <QWidget>
#include <QMap>
#include <QVariant>

#include "SettingsManager.h"
#include "HighlighterFormatsConfigureWidget.h"

namespace Ui {
    class TextEditorSettingsWidget;
}

class TextEditorSettingsWidget: public ISettingsTabPage
{
    Q_OBJECT

public:
    TextEditorSettingsWidget( const QMap <QString, QVariant> &valuesMap, QWidget *parent = 0 );
    ~TextEditorSettingsWidget();

    void fillSettings( QMap<QString, QVariant> &valueList ) const;

protected:
    void changeEvent(QEvent *e);

private slots:
    void onFontChanged();

signals:
    void fontChanged(const QFont &font);

private:
    Ui::TextEditorSettingsWidget *ui;
    HighlighterFormatsConfigureWidget *m_highlighterConfigureWidget;
};

#endif // TEXTEDITORSETTINGSWIDGET_H
