#ifndef OS_REGISTERS_H
#define OS_REGISTERS_H

#include "types.h"

typedef struct {
    dWord ds, edi, esi, ebp, ebx, edx, ecx, eax;
    dWord interruptNumber, errorCode;
    dWord epi, cs, eFlags, useresp, ss;
} Registers;

#endif