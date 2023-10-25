#pragma once

#include "edlib_global.h"

#include <QWidget>

class HighlighterFormatsManager;
namespace PythonEditor {
    class EditorWidget;
}


class EDLIB_EXPORT EditorsManager {
private:
    EditorsManager();
    ~EditorsManager();

    EditorsManager(const EditorsManager&) = delete;
    EditorsManager& operator=(const EditorsManager&) = delete;

    static EditorsManager *instanse();
    static EditorsManager *m_instance;
    HighlighterFormatsManager *m_highlighterFormatsManager;

public: // static API
    static PythonEditor::EditorWidget *createPythonEditor(QWidget *parent = nullptr);
};
