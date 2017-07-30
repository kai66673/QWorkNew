#include <QTableWidget>
#include <QToolBar>

#include <QDebug>

#include "SearchResultTabWidget.h"

SearchResultTabWidget::SearchResultTabWidget(int qssWatcherIndex, QToolBar *corner, QWidget *parent)
    : QTabWidget(parent)
{
    setCornerWidget(corner, Qt::TopLeftCorner);
    setTabsClosable(true);
    setTabBar(new SearchResultTabBar(qssWatcherIndex, this));
}

void SearchResultTabWidget::updateTabsGemetry()
{
    for (int i = 0; i < count(); i++) {
        const QString title = tabText(i);
        setTabText(i, title);

    }
}

void SearchResultTabWidget::addWidget( const QIcon &icon, const QString &text, QWidget *widget )
{
    setCurrentIndex(addTab(widget, icon, text));
}

void SearchResultTabWidget::removeWidget( int index )
{
    QWidget *w = widget(index);
    removeTab(index);
    w->deleteLater();
}

void SearchResultTabWidget::setCurrentSearchProgressWidget( QWidget *progressWidget )
{
    tabBar()->setTabButton(count() - 1, QTabBar::LeftSide, progressWidget);
}
