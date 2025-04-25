#include "registers.h"
#include "types.h"

#ifndef OS_KEYBOARD_H
#define OS_KEYBOARD_H

typedef struct {
    byte uppercase;
    byte ctrlHeld;
    byte altHeld;
    byte shiftHeld;
    byte GUIHeld;
    byte capsLock;
    byte numLock;
    byte scrollLock;
    byte enabled;
} KeyboardState;

typedef struct {
    KeyboardState state;
    byte keys[256];
    byte secondaryKeys[256];
    word charsPrinted;
    word cursorPos;
} KeyboardStruct;

extern KeyboardStruct keyboard;

void keyboard_UpdateLEDs(void);

void keyboard_Init(void);

void keyboard_Enable(void);

void keyboard_Disable(void);

#endif