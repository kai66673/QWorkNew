#ifndef CPPTOOLSUTILS_H
#define CPPTOOLSUTILS_H

#include "cpptools_global.h"

namespace CppTools {

class CppToolsUtils
{
public:
    CppToolsUtils();
    ~CppToolsUtils();
};

QString CPPTOOLS_EXPORT correspondingHeaderOrSource(const QString &fileName, bool *wasHeader = 0);

}   // namespace CppTools

#endif // CPPTOOLSUTILS_H
