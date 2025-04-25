#include "varargs.h"

#ifndef LIB_PRINT_H
#define LIB_PRINT_H

void vprintf(const char* format, va_list argv);

void printf(const char* format, ...);

#endif