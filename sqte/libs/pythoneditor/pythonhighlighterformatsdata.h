#pragma once

#include "highlighterformatsmanager.h"

namespace PythonEditor {

class PythonHighlighterFormatData: public HighlighterFormatData
{
public:
    PythonHighlighterFormatData();

    virtual QString languageId() const;
};

}   // namespace PythonEditor
