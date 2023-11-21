%Module edlib

%Import QtWidgets/QtWidgetsmod.sip

class EditorsManager {

%TypeHeaderCode
#include "../libs/edlib/edlib.h"
#include "../libs/pythoneditor/pythoneditorwidget.h"
%End

public:
    static PythonEditor::EditorWidget *createPythonEditor(QWidget *parent = nullptr);
};
