#ifndef CONSTANTS_OSCONSTANTS_H
#define CONSTANTS_OSCONSTANTS_H

#define OS_BOOT_DELAY 200

#define IDT_ENTRIES 256

#define HEAP_LOCATION 0x00100000
#define HEAP_SIZE     0x00e00000

#define VGA_CTRL_REGISTER 0x03d4
#define VGA_DATA_REGISTER 0x03d5
#define VGA_OFFSET_LOW    0x0f
#define VGA_OFFSET_HIGH   0x0e
#define VGA_ROWS          25
#define VGA_COLUMNS       80

#define VIDEO_ADDRESS 0xb8000

#endif