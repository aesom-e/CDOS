#ifndef LIB_PRINT_H
#define LIB_PRINT_H

#include "varargs.h"

void vprintf(const char* format, va_list argv);

void printf(const char* format, ...);

#endif