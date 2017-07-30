#ifndef KARELDOCUMENTCASEFIXHANDLER_H
#define KARELDOCUMENTCASEFIXHANDLER_H

#include "IFixOnSaveHandler.h"

namespace Karel {

class KarelDocumentCaseFixHandler : public TextEditor::IFixOnSaveHandler
{
public:
    virtual bool fixOnSave(QTextDocument *document) const;
};

}   // namespace Karel

#endif // KARELDOCUMENTCASEFIXHANDLER_H
