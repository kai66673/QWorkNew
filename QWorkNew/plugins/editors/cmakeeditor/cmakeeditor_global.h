#ifndef CMAKEEDITOR_GLOBAL_H
#define CMAKEEDITOR_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CMAKEEDITOR_LIBRARY)
#  define CMAKEEDITORSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CMAKEEDITORSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CMAKEEDITOR_GLOBAL_H
