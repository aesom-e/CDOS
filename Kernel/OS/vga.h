#include "types.h"
#include "port.h"

#ifndef OS_VGA_H
#define OS_VGA_H

typedef struct {
    byte x;
    byte y;
} Position;

void vga_SetCursor(int offset);

int vga_GetCursor(void);

int vga_GetPositionOffset(Position pos);

void vga_SetCursorPosition(Position cursorPos);

Position vga_GetCursorPosition();

#endif