#ifndef FAVORITES_GLOBAL_H
#define FAVORITES_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FAVORITES_LIBRARY)
#  define FAVORITES_EXPORT Q_DECL_EXPORT
#else
#  define FAVORITES_EXPORT Q_DECL_IMPORT
#endif

#endif // FAVORITES_GLOBAL_H
