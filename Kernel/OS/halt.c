#include "video.h"
#include "delay.h"
#include "port.h"
#include "halt.h"

_Noreturn void halt(void) {
    video_Print("[System Halted]");
    delay_Milliseconds(1000);

    // Try to use ACPI to power off
    port_WordOut(0xB004, 0x2000);

    while(1);
}