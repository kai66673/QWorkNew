#ifndef BASEWALKER_H
#define BASEWALKER_H

#include <QVariant>

#include "sqlengine_global.h"

namespace Sql {

class SQL_EXPORT BaseWalkReflector
{
public:
    virtual bool isWalkCanceled() const = 0;
    virtual void invokeSendData( const QVariant &data ) = 0;
};

class SQL_EXPORT BaseWalker
{
public:
    virtual ~BaseWalker() {}
    virtual void walk( BaseWalkReflector *reflector ) = 0;
};

}   // namespace Sql

#endif // BASEWALKER_H
