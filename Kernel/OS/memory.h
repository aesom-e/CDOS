#include "types.h"

#ifndef OS_MEMORY_H
#define OS_MEMORY_H

void memory_Copy(byte* source, byte* destination, int num);

void memory_Init(void);

void* memory_Allocate(dWord size);

void* memory_Callocate(dWord size);

#define memory_Free(memory) do { if((memory)) { __memory_Free((memory)); (memory) = NULL; } } while(0)

void __memory_Free(void* memory);

#endif