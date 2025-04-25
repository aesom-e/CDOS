#include "idt.h"

struct {
    word limit;
    dWord base;
} __attribute__((packed)) IDTRegister;

IDTGate IDT[IDT_ENTRIES];

void idt_Init(void) {
    IDTRegister.base = (dWord)&IDT;
    IDTRegister.limit = IDT_ENTRIES * sizeof(IDTGate) - 1;
    asm volatile("lidt (%0)" : : "r" (&IDTRegister));
}

void idt_Register(byte num, dWord handler) {
    IDT[num].lowOffset = (handler & 0xffff);
    IDT[num].selector = 0x08;
    IDT[num].alwaysZero = 0;
    IDT[num].flags = 0x8e;
    IDT[num].highOffset = ((handler >> 16) & 0xffff);
}