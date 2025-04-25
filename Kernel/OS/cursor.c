#include "../Constants/OSConstants.h"
#include "port.h"
#include "vga.h"
#include "cursor.h"

void cursor_Enable(void) {
    // Get the position of the cursor
    int cursorPos = vga_GetCursor();

    // Select the cursor start register
    port_ByteOut(VGA_CTRL_REGISTER, 0x0a);
    // Set the cursor start position, clearing bit 5
    port_ByteOut(VGA_DATA_REGISTER, cursorPos & 0x1f);

    // Select the cursor end register
    port_ByteOut(VGA_CTRL_REGISTER, 0x0b);
    // Set the cursor end position
    port_ByteOut(VGA_DATA_REGISTER, cursorPos & 0x1f);
}

void cursor_Disable(void) {
    // Select the cursor start register
    port_ByteOut(VGA_CTRL_REGISTER, 0x0a);
    // Set bit 5 to disable the cursor
    port_ByteOut(VGA_DATA_REGISTER, 0x20);
}