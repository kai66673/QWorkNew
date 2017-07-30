#ifndef CMAKEEDITORWIDGET_H
#define CMAKEEDITORWIDGET_H

#include "PlainTextEditor.h"
#include "uncommentselection.h"

#include "cmakeeditor_global.h"

namespace Cmake {

class CMAKEEDITORSHARED_EXPORT CmakeEditorWidget: public TextEditor::PlainTextEditorWidget
{
    Q_OBJECT

public:
    CmakeEditorWidget(IDocument *document, QWidget *parent = 0);
    virtual ~CmakeEditorWidget();

protected:
    TextEditor::BaseTextEditor *createEditor();
    virtual Core::ILink *findLinkAt(const QTextCursor &cursor, bool resolveTarget = true);
};

}   // namespace CmakeEditor

#endif // CMAKEEDITORWIDGET_H
