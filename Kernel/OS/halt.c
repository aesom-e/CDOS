#include "video.h"
#include "delay.h"
#include "acpi.h"
#include "halt.h"

_Noreturn void halt(void) {
    // Try to use ACPI to power off
    acpi_Sleep(ACPI_SLEEP_SHUTDOWN);

    video_Print("[ACPI Failed To Shutdown]\n[System Halted]\n");
    while(1);
}