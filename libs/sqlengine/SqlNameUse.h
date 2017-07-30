#ifndef SQLNAMEUSE_H
#define SQLNAMEUSE_H

#include "sqlengine_global.h"

namespace Sql {
namespace CheckName {

struct SQL_EXPORT Use {
    unsigned line;
    unsigned column;
    unsigned length;
    unsigned kind;

    enum Kind {
        Schema = 0,
        Table,
        TableAlias,
        Field,
        Index,
        Trigger,
        Constraint,
        Bind,
        Identifier,
        Label,
        Function,
        Cursor,
        StorageMap,
        Area,
        Module
    };

    Use(unsigned line = 0, unsigned column = 0, unsigned length = 0, unsigned kind = Table)
        : line(line), column(column), length(length), kind(kind) {}
};

}   // namespace CheckName
}   // namespace Sql

#endif // SQLNAMEUSE_H
