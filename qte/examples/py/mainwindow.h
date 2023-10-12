#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <simplemainwindow.h>

#include "pythoneditorwidget.h"

class HighlighterFormatsManager;
class SettingsManager;

class MainWindow : public SimpleMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();


private:
    SettingsManager *m_settingsManager;
    HighlighterFormatsManager *m_highlighterFormatsManager;

    PythonEditor::EditorWidget *m_editor;
};

#endif // MAINWINDOW_H
