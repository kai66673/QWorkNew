#ifndef KARELSEMANTICINFO_H
#define KARELSEMANTICINFO_H

#include "KarelDocument.h"

namespace Karel {

class SemanticInfo
{
public:
    struct Use {
        unsigned line;
        unsigned column;
        unsigned length;
        unsigned kind;

        enum Kind {
            UserType = 0,
            Constant,
            Variable,
            TypeField,
            Routine,
            ProgramName
        };

        Use(unsigned line = 0, unsigned column = 0, unsigned length = 0, unsigned kind = UserType)
            : line(line), column(column), length(length), kind(kind)
        { }
    };

    SemanticInfo();

    unsigned revision;
    Document::Ptr doc;
    QList<Utils::DiagnosticMessage> diagnosticMessages;
};

}   // namespace Karel

#endif // KARELSEMANTICINFO_H
