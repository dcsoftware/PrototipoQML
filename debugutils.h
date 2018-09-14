#ifndef DEBUGUTILS_H
#define DEBUGUTILS_H

#ifdef DEBUG
#include <QtDebug>

#define DEBUG_PRINT(str)            qDebug() << str;

#else

#define DEBUG_PRINT(str)

#endif

#endif // DEBUGUTILS_H
