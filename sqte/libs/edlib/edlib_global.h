#ifndef EDLIB_GLOBAL_H
#define EDLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EDLIB_LIBRARY)
#  define EDLIB_EXPORT Q_DECL_EXPORT
#else
#  define EDLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // EDLIB_GLOBAL_H
