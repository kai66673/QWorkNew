#ifndef SEARCHRESULTTABWIDGET_H
#define SEARCHRESULTTABWIDGET_H

#include "StyleManager.h"

#include <QTabWidget>
#include <QTabBar>

QT_BEGIN_NAMESPACE
class QToolBar;
QT_END_NAMESPACE

class SearchResultTabBar: public QTabBar, public Utils::StyleManagedWidget
{
    Q_OBJECT

public:
    SearchResultTabBar(int qssWatcherIndex, QWidget *parent = 0)
        : QTabBar(parent)
        , Utils::StyleManagedWidget(this, qssWatcherIndex)
    { }
};

class SearchResultTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit SearchResultTabWidget(int qssWatcherIndex, QToolBar *corner, QWidget *parent = 0);
    void updateTabsGemetry();

    
public slots:
    void addWidget( const QIcon & icon, const QString & text, QWidget *widget );
    void removeWidget( int index );
    void setCurrentSearchProgressWidget( QWidget *progressWidget );
};

#endif // SEARCHRESULTTABWIDGET_H
