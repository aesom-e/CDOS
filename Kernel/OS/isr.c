#include "types.h"
#include "port.h"
#include "irq.h"
#include "idt.h"
#include "video.h"
#include "isr.h"

char* exceptionMessages[] = {
        "Division By Zero",
        "Debug",
        "Non Maskable Interrupt",
        "Breakpoint",
        "Overflow Detected",
        "Out Of Bounds",
        "Invalid Opcode",
        "No Coprocessor",
        "Double Fault",
        "Coprocessor Segment Overrun",
        "Bad TSS",
        "Segment Not Present",
        "Stack Fault",
        "General Protection Fault",
        "Page Fault",
        "Unknown Interrupt",
        "Floating Point",
        "Alignment Check",
        "Machine Check",

        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
};

void isr_Init(void) {
    // Initialize the internal ISRs
    idt_Register(0,  (dWord)isr0);
    idt_Register(1,  (dWord)isr1);
    idt_Register(2,  (dWord)isr2);
    idt_Register(3,  (dWord)isr3);
    idt_Register(4,  (dWord)isr4);
    idt_Register(5,  (dWord)isr5);
    idt_Register(6,  (dWord)isr6);
    idt_Register(7,  (dWord)isr7);
    idt_Register(8,  (dWord)isr8);
    idt_Register(9,  (dWord)isr9);
    idt_Register(10, (dWord)isr10);
    idt_Register(11, (dWord)isr11);
    idt_Register(12, (dWord)isr12);
    idt_Register(13, (dWord)isr13);
    idt_Register(14, (dWord)isr14);
    idt_Register(15, (dWord)isr15);
    idt_Register(16, (dWord)isr16);
    idt_Register(17, (dWord)isr17);
    idt_Register(18, (dWord)isr18);
    idt_Register(19, (dWord)isr19);
    idt_Register(20, (dWord)isr20);
    idt_Register(21, (dWord)isr21);
    idt_Register(22, (dWord)isr22);
    idt_Register(23, (dWord)isr23);
    idt_Register(24, (dWord)isr24);
    idt_Register(25, (dWord)isr25);
    idt_Register(26, (dWord)isr26);
    idt_Register(27, (dWord)isr27);
    idt_Register(28, (dWord)isr28);
    idt_Register(29, (dWord)isr29);
    idt_Register(30, (dWord)isr30);
    idt_Register(31, (dWord)isr31);

    // Reprogram the PIC
    port_ByteOut(0x20, 0x11);
    port_ByteOut(0xA0, 0x11);
    port_ByteOut(0x21, 0x20);
    port_ByteOut(0xA1, 0x28);
    port_ByteOut(0x21, 0x04);
    port_ByteOut(0xA1, 0x02);
    port_ByteOut(0x21, 0x01);
    port_ByteOut(0xA1, 0x01);
    port_ByteOut(0x21, 0x00);
    port_ByteOut(0xA1, 0x00);

    // Install the IRQs
    idt_Register(32, (dWord)irq0);
    idt_Register(33, (dWord)irq1);
    idt_Register(34, (dWord)irq2);
    idt_Register(35, (dWord)irq3);
    idt_Register(36, (dWord)irq4);
    idt_Register(37, (dWord)irq5);
    idt_Register(38, (dWord)irq6);
    idt_Register(39, (dWord)irq7);
    idt_Register(40, (dWord)irq8);
    idt_Register(41, (dWord)irq9);
    idt_Register(42, (dWord)irq10);
    idt_Register(43, (dWord)irq11);
    idt_Register(44, (dWord)irq12);
    idt_Register(45, (dWord)irq13);
    idt_Register(46, (dWord)irq14);
    idt_Register(47, (dWord)irq15);

    // Re-enable interrupts
    idt_Init();
    asm volatile("sti");
}

void isr_Handler(Registers* in) {
    video_Newline(NULL);
    video_Print("[Exception: ");
    video_Print(exceptionMessages[in->interruptNumber]);
    video_Print("]\n");
    video_Print("[System Halted]\n");
    asm volatile("hlt");
}