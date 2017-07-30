#ifndef __ASPROJECT_GLOBAL_H
#define __ASPROJECT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ASPROJECT_LIBRARY)
#  define ASPROJECT_EXPORT Q_DECL_EXPORT
#else
#  define ASPROJECT_EXPORT Q_DECL_IMPORT
#endif

#endif // __ASPROJECT_GLOBAL_H
