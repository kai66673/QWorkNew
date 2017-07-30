#ifndef QWORKBASE_GLOBAL_H
#define QWORKBASE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QWORKBASE_LIBRARY)
#  define QWORKBASESHARED_EXPORT Q_DECL_EXPORT
#else
#  define QWORKBASESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QWORKBASE_GLOBAL_H
