#include "halt.h"
#include "os.h"

void os_Init(void) {
    video_Init();
    isr_Init();
    delay_Init();
    keyboard_Init();
    memory_Init();
    acpi_Init();
}

_Noreturn void os_Halt(void) {
    halt();
}