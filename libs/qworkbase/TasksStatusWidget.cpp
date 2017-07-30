#include "TasksStatusWidget.h"

TasksStatusWidget::TasksStatusWidget( QWidget *parent )
    : QPushButton(parent)
    , m_dock(0)
{
    m_icons << QIcon(":/images/task_state_ready.png")
            << QIcon(":/images/task_state_exec.png")
            << QIcon(":/images/task_state_wait.png");
    setFlat(true);
    setStyleSheet("QPushButton { border: 0px solid white }");
}

void TasksStatusWidget::initStatus( QwCustomDockWidget *dock )
{
    m_dock = dock;
    connect(this, SIGNAL(clicked()), this, SLOT(onToggled()));
}

void TasksStatusWidget::onTasksStatusChanged( const Core::TasksStatus &status )
{
    QString title;
    if ( status.taskCount )
        title = tr("%1 Tasks Executed").arg(status.taskCount);
    else
        title = tr("No Tasks Executed");
    setText(title);
    setIcon(m_icons[status.state]);
}

void TasksStatusWidget::onToggled()
{
    if ( m_dock ) {
        if ( m_dock->isHidden() )
            m_dock->showAndSetCurrentIfTabified();
        else
            m_dock->hide();
    }
}
