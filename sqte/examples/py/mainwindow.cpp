#include "mainwindow.h"

#include "edlib.h"
#include "pythoneditorwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_editor = EditorsManager::createPythonEditor(this);
    setCentralWidget(m_editor);
}
