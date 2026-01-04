#ifndef OS_IDT_H
#define OS_IDT_H

#include "../Constants/OSConstants.h"
#include "types.h"

typedef struct {
    word lowOffset;
    word selector;
    byte alwaysZero;
    byte flags;
    word highOffset;
} __attribute__((packed)) IDTGate;

extern IDTGate IDT[IDT_ENTRIES];

void idt_Init(void);

void idt_Register(byte num, dWord handler);

#endif