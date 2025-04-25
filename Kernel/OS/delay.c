#include "port.h"
#include "irq.h"
#include "registers.h"
#include "delay.h"

#define PIT_COMMAND_PORT 0x43
#define PIT_CHANNEL_ZERO 0x40
#define PIT_OUTPUT       0x00
#define PIT_FREQUENCY    1193182

volatile dWord tickCounter = 0;

void delay_TimerHandler(Registers* in) {
    tickCounter++;
}

void delay_Init(void) {
    dWord interval = PIT_FREQUENCY / 1000; // 1000Hz or 1ms interval

    // Initialize the PIT
    port_ByteOut(PIT_COMMAND_PORT, 0x36);
    port_ByteOut(PIT_CHANNEL_ZERO, (byte)(interval & 0xff));
    port_ByteOut(PIT_CHANNEL_ZERO, (byte)((interval >> 8) & 0xff));

    // Register IRQ0 with the timer interrupt
    irq_Register(IRQ0, delay_TimerHandler);
}

void delay_Milliseconds(dWord ms) {
    dWord targetTick = tickCounter + ms;

    while(tickCounter < targetTick) asm volatile("hlt"); // Await the next interrupt
}