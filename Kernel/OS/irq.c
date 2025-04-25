#include "port.h"
#include "irq.h"

InterruptHandler interruptHandlers[256] = {0};

void irq_Handler(Registers* in) {
    if(interruptHandlers[in->errorCode]) {
        InterruptHandler handler = interruptHandlers[in->errorCode];
        handler(in);
    }

    if(in->interruptNumber >= IRQ8) port_ByteOut(0xA0, 0x20);
    port_ByteOut(0x20, 0x20);
}

void irq_Register(byte num, InterruptHandler handler) {
    interruptHandlers[num] = handler;
}