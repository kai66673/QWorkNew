#include "mainwindow.h"

#include "texteditorsettings.h"
#include "pythonhighlighterformatsdata.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    new TextEditor::TextEditorSettings(this);

    m_highlighterFormatsManager = new HighlighterFormatsManager;

    m_highlighterFormatsManager->registerLanguageHighlighter(new PythonEditor::PythonHighlighterFormatData);

    m_editor = new PythonEditor::EditorWidget(this);
    m_editor->onSettingsChanged();

    setCentralWidget(m_editor);
}

MainWindow::~MainWindow()
{
    delete m_highlighterFormatsManager;
}
