#ifndef SETTINGS_GLOBAL_H
#define SETTINGS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SETTINGS_LIBRARY)
#  define SETTINGSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SETTINGSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SETTINGS_GLOBAL_H
