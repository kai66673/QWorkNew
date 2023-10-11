#ifndef ILINK_H
#define ILINK_H

#include <QString>

#include "corelib_global.h"

namespace Core {

class CORELIB_EXPORT ILink
{
public:
    virtual ~ILink() {}
    ILink( int begin, int end );

    bool isEqual( ILink *other ) const;
    int begin() const { return m_begin; }
    int end() const { return m_end; }

    virtual bool open() const = 0;

protected:
    int m_begin;
    int m_end;
};

class CORELIB_EXPORT TextTargetLink: public ILink
{
public:
    TextTargetLink( int begin, int end, const QString &fileName, int line, int column );

    virtual bool open() const;

private:
    QString m_fileName;  // Target file
    int m_line;          // Target line
    int m_column;        // Target column

};

}   // namespace Core

#endif // ILINK_H
