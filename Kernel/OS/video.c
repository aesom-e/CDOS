#include "../Constants/OSConstants.h"
#include "vga.h"
#include "memory.h"
#include "video.h"

char* VIDEO_MEMORY;

void video_Init(void) {
    VIDEO_MEMORY = (char*)VIDEO_ADDRESS;
}

void video_SetRawColouredByte(char data, VideoColour backgroundColour, VideoColour textColour, int offset) {
    VIDEO_MEMORY[offset]   = data;
    VIDEO_MEMORY[offset+1] = ((backgroundColour << 4) | textColour);
}

void video_SetRawByte(char data, int offset) {
    video_SetRawColouredByte(data, COLOUR_BLACK, COLOUR_LGREY, offset);
}

void video_Scroll(int* offset) {
    memory_Copy((byte*)(vga_GetPositionOffset((Position){0, 1}) + VIDEO_ADDRESS),
                (byte*)(vga_GetPositionOffset((Position){0, 0}) + VIDEO_ADDRESS),
                VGA_COLUMNS * (VGA_ROWS - 1) * 2);

    int i;
    for(i=0;i<VGA_COLUMNS;i++)
        video_SetRawByte(' ', vga_GetPositionOffset((Position){i, VGA_ROWS-1}));

    if(offset) *offset -= (2 * VGA_COLUMNS);
}

void video_Newline(int* offset) {
    Position cursorPos = vga_GetCursorPosition();
    cursorPos.y++;
    cursorPos.x = 0;
    vga_SetCursorPosition(cursorPos);

    if(offset) *offset = vga_GetPositionOffset(cursorPos);
}

void video_Print(const char* text) {
    int offset = vga_GetCursor();
    int i = 0;
    while(text[i]) {
        // Scroll if necessary
        if(offset >= VGA_ROWS * VGA_COLUMNS * 2) video_Scroll(&offset);

        switch(text[i]) {
            case '\r':
            case '\n':
                video_Newline(&offset);
                break;
            case '\b':
                offset -= 2;
                video_SetRawByte(' ', offset);
                break;
            default:
                video_SetRawByte(text[i], offset);
                offset += 2;
                break;
        }
        i++;
    }
    // Recheck if we need to scroll
    if(offset >= VGA_ROWS * VGA_COLUMNS * 2) video_Scroll(&offset);
    vga_SetCursor(offset);
}

void video_Clear(void) {
    int i;
    for(i=0;i<VGA_COLUMNS*VGA_ROWS;i++) {
        video_SetRawByte(' ', i*2);
    }
    vga_SetCursor(0);
}

void video_MoveCursor(char x, char y) {
    Position cursorPos = vga_GetCursorPosition();
    cursorPos.x += x;
    cursorPos.y += y;
    vga_SetCursorPosition(cursorPos);
}