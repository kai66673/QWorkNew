#include "DiagnosticMessage.h"

namespace Utils {

bool DiagnosticMessage::operator==(const DiagnosticMessage &other) const
{
    return
            _line == other._line &&
            _column == other._column &&
            _length == other._length &&
            _level == other._level &&
            _fileName == other._fileName &&
            _text == other._text;
}

bool DiagnosticMessage::operator!=(const DiagnosticMessage &other) const
{
    return !operator==(other);
}

}   // namespace Utils

