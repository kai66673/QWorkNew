#ifndef __SQLENGINE_GLOBAL_H

#define __SQLENGINE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SQLENGINE_BUILD_LIB)
#  define SQL_EXPORT Q_DECL_EXPORT
#else
#  define SQL_EXPORT Q_DECL_IMPORT
#endif

#endif // __SQLENGINE_GLOBAL_H
