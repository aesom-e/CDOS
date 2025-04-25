#include "../Constants/OSConstants.h"
#include "vga.h"

void vga_SetCursor(int offset) {
    offset /= 2; // Because each part of the VGA memory is split into 2 bytes, 1 for text, and 1 for colour
    port_ByteOut(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    port_ByteOut(VGA_DATA_REGISTER, (byte)(offset >> 8));
    port_ByteOut(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    port_ByteOut(VGA_DATA_REGISTER, (byte)(offset & 0xff));
}

int vga_GetCursor() {
    port_ByteOut(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    int offset = port_ByteIn(VGA_DATA_REGISTER) << 8;
    port_ByteOut(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    offset += port_ByteIn(VGA_DATA_REGISTER);
    return offset * 2;
}

int vga_GetPositionOffset(Position pos) {
    return (pos.y * VGA_COLUMNS + pos.x) * 2;
}

void vga_SetCursorPosition(Position cursorPos) {
    vga_SetCursor(vga_GetPositionOffset(cursorPos));
}

Position vga_GetCursorPosition() {
    port_ByteOut(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
    int offset = port_ByteIn(VGA_DATA_REGISTER) << 8;
    port_ByteOut(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
    offset += port_ByteIn(VGA_DATA_REGISTER);

    Position ret = {offset % VGA_COLUMNS, offset / VGA_COLUMNS};
    return ret;
}