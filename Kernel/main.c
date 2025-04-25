#include "Constants/OSConstants.h"
#include "OS/os.h"
#include "Shell/shell.h"
#include "FS/fs.h"
#include "Lib/time.h"

void main(void) {
    // Start the kernel
    os_Init();
    keyboard_Disable();
    video_Print("[Kernel Started]\n");

    // Set the timezone to EST
    time_SetSystemTime(TIMEZONE_EST);

    // Initialize the file system
    fs_Init();

    // Wait for the user to see the message then clear
    delay_Milliseconds(OS_BOOT_DELAY);
    video_Clear();

    // Pass handling on to the terminal
    terminal_Handle();
    os_Halt();
}