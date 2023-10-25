#include "edlib.h"
#include "highlighterformatsmanager.h"
#include "pythonhighlighterformatsdata.h"
#include "pythoneditorwidget.h"

EditorsManager* EditorsManager::m_instance = nullptr;

EditorsManager::EditorsManager()
{
    m_highlighterFormatsManager = new HighlighterFormatsManager;
    m_highlighterFormatsManager->registerLanguageHighlighter(new PythonEditor::PythonHighlighterFormatData);
}

EditorsManager::~EditorsManager()
{
    delete m_highlighterFormatsManager;
}

EditorsManager *EditorsManager::instanse()
{
    return m_instance? m_instance : m_instance = new EditorsManager();
}

////////////////////////////////////////////////////////////////
/// static API
PythonEditor::EditorWidget *EditorsManager::createPythonEditor(QWidget *parent)
{
    instanse();
    PythonEditor::EditorWidget *widget = new PythonEditor::EditorWidget(parent);
    widget->onSettingsChanged();
    return widget;
}
