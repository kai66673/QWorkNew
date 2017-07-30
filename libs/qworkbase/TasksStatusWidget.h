#ifndef QTASKSSTATUSWIDGET_H
#define QTASKSSTATUSWIDGET_H

#include <QPushButton>
#include <QList>
#include <QIcon>
#include <QAction>
#include <QDockWidget>

#include "progressmanager.h"
#include "QwDockWidget.h"

class TasksStatusWidget : public QPushButton
{
    Q_OBJECT

public:
    explicit TasksStatusWidget( QWidget *parent = 0 );
    void initStatus( QwCustomDockWidget *dock );
    
signals:
    
public slots:
    void onTasksStatusChanged( const Core::TasksStatus &status );
    void onToggled();

private:
    QList<QIcon> m_icons;
    QwCustomDockWidget *m_dock;
};

#endif // QTASKSSTATUSWIDGET_H
