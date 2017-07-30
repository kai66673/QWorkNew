#ifndef RPMLLIB_GLOBAL_H
#define RPMLLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RPML_LIBRARY)
#  define RPML_EXPORT Q_DECL_EXPORT
#else
#  define RPML_EXPORT Q_DECL_IMPORT
#endif

#endif // RPMLLIB_GLOBAL_H
