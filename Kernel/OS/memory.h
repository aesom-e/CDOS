#ifndef OS_MEMORY_H
#define OS_MEMORY_H

#include "types.h"

void memory_Copy(byte* source, byte* destination, dWord num);

byte memory_Equal(const void* a, const void* b, dWord size);

void memory_Init(void);

void* memory_Allocate(dWord size);

void* memory_Callocate(dWord size);

#define memory_Free(memory) do { if((memory)) { __memory_Free((memory)); (memory) = NULL; } } while(0)

void __memory_Free(void* memory);

#endif