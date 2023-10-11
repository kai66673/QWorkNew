#ifndef DIAGNOSTICMESSAGE_H
#define DIAGNOSTICMESSAGE_H

#include <QString>

#include "utils_global.h"

namespace Utils {

class QTCREATOR_UTILS_EXPORT DiagnosticMessage
{
public:
    enum Level {
        Warning,
        Error,
        Fatal
    };

public:
    DiagnosticMessage(int level, const QString &fileName,
                      unsigned line, unsigned column,
                      const QString &text,
                      unsigned length = 0)
        : _level(level),
          _fileName(fileName),
          _line(line),
          _column(column),
          _length(length),
          _text(text)
    { }

    int level() const
    { return _level; }

    bool isWarning() const
    { return _level == Warning; }

    bool isError() const
    { return _level == Error; }

    bool isFatal() const
    { return _level == Fatal; }

    QString fileName() const
    { return _fileName; }

    unsigned line() const
    { return _line; }

    unsigned column() const
    { return _column; }

    unsigned length() const
    { return _length; }

    QString text() const
    { return _text; }

    bool operator==(const DiagnosticMessage &other) const;
    bool operator!=(const DiagnosticMessage &other) const;

private:
    int _level;
    QString _fileName;
    unsigned _line;
    unsigned _column;
    unsigned _length;
    QString _text;
};

}   // namespace Utils

#endif // DIAGNOSTICMESSAGE_H
