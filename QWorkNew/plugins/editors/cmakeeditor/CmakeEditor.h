#ifndef CMAKEEDITOR_H
#define CMAKEEDITOR_H

#include "PlainTextEditor.h"

#include "cmakeeditor_global.h"

namespace Cmake {

class CmakeEditorWidget;

class CMAKEEDITORSHARED_EXPORT CmakeEditor: public TextEditor::PlainTextEditor
{
    Q_OBJECT

public:
    CmakeEditor(CmakeEditorWidget *editorWidget);
    virtual ~CmakeEditor();

    QString id() const;
};

}   // namespace CmakeEditor

#endif // CMAKEEDITOR_H
