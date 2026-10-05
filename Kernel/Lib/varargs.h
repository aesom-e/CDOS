#ifndef LIB_VARARGS_H
#define LIB_VARARGS_H

typedef char* va_list;

#define _VA_ALIGN(size) (((size) + sizeof(int) - 1) & ~(sizeof(int) - 1))

#define va_start(list, lastArg) (list = (va_list)(&(lastArg) + 1))

#define va_arg(list, type) (*(type*)((list += _VA_ALIGN(sizeof(type))) - _VA_ALIGN(sizeof(type))))

#define va_end(list) (list = NULL)

#endif