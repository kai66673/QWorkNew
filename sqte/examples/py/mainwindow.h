#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "pythoneditorwidget.h"

class HighlighterFormatsManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();


private:
    HighlighterFormatsManager *m_highlighterFormatsManager;

    PythonEditor::EditorWidget *m_editor;
};

#endif // MAINWINDOW_H
