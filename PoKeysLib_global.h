#ifndef POKEYSLIB_GLOBAL_H
#define POKEYSLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(POKEYSLIB_LIBRARY)
#  define POKEYSLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define POKEYSLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // POKEYSLIB_GLOBAL_H
