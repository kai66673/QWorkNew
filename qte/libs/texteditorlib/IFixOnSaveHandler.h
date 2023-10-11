#ifndef IFIXONSAVEHANDLER
#define IFIXONSAVEHANDLER

#include <QTextDocument>

#include "texteditor_global.h"

namespace TextEditor {

class TEXTEDITOR_EXPORT IFixOnSaveHandler
{
public:
    virtual bool fixOnSave(QTextDocument *document) const = 0;
};

}   // namespace TextEditor

#endif // IFIXONSAVEHANDLER

