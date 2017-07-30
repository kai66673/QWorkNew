#ifndef QUERYVIEWSETTINGSWIDGET_H
#define QUERYVIEWSETTINGSWIDGET_H

#include <QWidget>
#include <QMap>
#include <QVariant>

#include "SettingsManager.h"

namespace Ui {
    class QueryViewSettingsWidget;
}

class QueryViewSettingsWidget: public ISettingsTabPage
{
    Q_OBJECT

public:
    QueryViewSettingsWidget( const QMap <QString, QVariant> &valuesMap, QWidget *parent = 0 );
    ~QueryViewSettingsWidget();

    void fillSettings( QMap <QString, QVariant> &valuesMap ) const;

protected:
    void changeEvent(QEvent *e);

public slots:
    void onFontClicked();
    void onForegroundColorClicked();
    void onBackgroundColor1Clicked();
    void onBackgroundColor2Clicked();

private:
    Ui::QueryViewSettingsWidget *ui;
    QFont font;
    QColor backgroundColor1;
    QColor backgroundColor2;
    QColor foregroundColor;
};

#endif // QUERYVIEWSETTINGSWIDGET_H
