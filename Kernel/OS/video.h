#ifndef OS_VIDEO_H
#define OS_VIDEO_H

typedef enum {
    COLOUR_BLACK    = 0x0,
    COLOUR_BLUE     = 0x1,
    COLOUR_GREEN    = 0x2,
    COLOUR_CYAN     = 0x3,
    COLOUR_RED      = 0x4,
    COLOUR_MAGENTA  = 0x5,
    COLOUR_BROWN    = 0x6,
    COLOUR_LGREY    = 0x7,
    COLOUR_GREY     = 0x8,
    COLOUR_LBLUE    = 0x9,
    COLOUR_LGREEN   = 0xa,
    COLOUR_LCYAN    = 0xb,
    COLOUR_LRED     = 0xc,
    COLOUR_LMAGENTA = 0xd,
    COLOUR_YELLOW   = 0xe,
    COLOUR_WHITE    = 0xf
} VideoColour;

void video_Init(void);

void video_SetRawColouredByte(char data, VideoColour backgroundColour, VideoColour textColour, int offset);

void video_SetRawByte(char data, int offset);

void video_Scroll(int* offset);

void video_Newline(int* offset);

void video_Print(const char* text);

void video_Clear();

void video_MoveCursor(char x, char y);

#endif