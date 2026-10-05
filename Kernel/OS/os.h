#ifndef OS_H
#define OS_H

// This header just includes everything else from OS/
#include "acpi.h"
#include "cursor.h"
#include "delay.h"
#include "event.h"
#include "halt.h"
#include "idt.h"
#include "irq.h"
#include "isr.h"
#include "keyboard.h"
#include "memory.h"
#include "port.h"
#include "registers.h"
#include "types.h"
#include "vga.h"
#include "video.h"

/**
* \brief Initializes all the different parts of the OS
*/
void os_Init(void);

/**
* \brief Halts the OS
*/
_Noreturn void os_Halt(void);

#endif