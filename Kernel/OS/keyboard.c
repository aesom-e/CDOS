#include "port.h"
#include "event.h"
#include "video.h"
#include "irq.h"
#include "keyboard.h"

char lowercaseKeys[] = "\0" // ESC (0x00)
                       "1234567890-="
                       "\0" // Backspace (0x0e)
                       "\0" // Tab (0x0f)
                       "qwertyuiop[]"
                       "\0" // Enter (0x1c)
                       "\0" // Left control (0x1d)
                       "asdfghjkl;'`"
                       "\0" // Left shift (0x2a)
                       "\\zxcvbnm,./"
                       "\0" // Right shift (0x36)
                       "*"
                       "\0" // Left alt (0x38)
                       " "
                       "\0" // CapsLock (0x3a)
                       "\0\0\0\0\0\0\0\0\0\0" // F1-F10 (0x3b-0x44)
                       "\0" // NumberLock (0x45)
                       "\0" // ScrollLock (0x46)
                       "789-456+1230."
                       "\0\0\0" // Unbound (0x54-0x56)
                       "\0\0"; // F11-F12 (0x57-0x58)

char uppercaseKeys[] = "\0" // ESC (0x00)
                       "!@#$%^&*()_+"
                       "\0" // Backspace (0x0e)
                       "\0" // Tab (0x0f)
                       "QWERTYUIOP{}"
                       "\0" // Enter (0x1c)
                       "\0" // Left control (0x1d)
                       "ASDFGHJKL:\"~"
                       "\0" // Left shift (0x2a)
                       "|ZXCVBNM<>?"
                       "\0" // Right shift (0x36)
                       "*"
                       "\0" // Left alt (0x38)
                       " "
                       "\0" // CapsLock (0x3a)
                       "\0\0\0\0\0\0\0\0\0\0" // F1-F10 (0x3b-0x44)
                       "\0" // NumberLock (0x45)
                       "\0" // ScrollLock (0x46)
                       "789-456+230."
                       "\0\0\0" // Unbound (0x54-0x56)
                       "\0\0"; // F11-F12 (0x57-0x58)

KeyboardStruct keyboard = {0};

void keyboard_AwaitResponse() {
    while(port_ByteIn(0x64) & 0x02);
}

void keyboard_UpdateLEDs() {
    // Get what state the LEDs should be in
    // Bit 0: ScrollLock, 1: NumLock, 2: CapsLock
    byte ledState = keyboard.state.scrollLock << 0 |
                    keyboard.state.numLock << 1 |
                    keyboard.state.capsLock << 2;

    keyboard_AwaitResponse();
    port_ByteOut(0x60, 0xed);
    keyboard_AwaitResponse();
    port_ByteOut(0x60, ledState);
}

void keyboard_HandleSpecialKey(byte scanCode) {
    if(scanCode <= 0x6d) {
        // Key pressed
        keyboard.secondaryKeys[scanCode] = 1;
        switch(scanCode) {
            case 0x1c:
                // Enter
                if(event_Handle(EVENT_KEY_ENTER_DOWN)) return;
                video_Newline(NULL);
                keyboard.charsPrinted = 0;
                break;
            case 0x1d:
                // Right ctrl
                keyboard.state.ctrlHeld = 1;
                if(event_Handle(EVENT_KEY_RCTRL_DOWN)) return;
                break;
            case 0x35:
                // Keypad '/'
                currentState.stateDetails = '/';
                if(event_Handle(EVENT_KEY_NORMAL_DOWN)) return;
                if(keyboard.state.enabled) return;
                video_Print("/");
                keyboard.charsPrinted++;
                keyboard.cursorPos++;
                break;
            case 0x38:
                // Right alt
                keyboard.state.altHeld = 1;
                if(event_Handle(EVENT_KEY_RALT_DOWN)) return;
                break;
            case 0x48:
                // Up arrow key
                if(event_Handle(EVENT_KEY_UPARROW_DOWN)) return;
                break;
            case 0x4b:
                // Left arrow key
                if(event_Handle(EVENT_KEY_LEFTARROW_DOWN)) return;
                if(keyboard.state.enabled) return;
                if(keyboard.cursorPos > 0) {
                    keyboard.cursorPos--;
                    video_MoveCursor(-1, 0);
                }
                break;
            case 0x4d:
                // Right arrow key
                if(event_Handle(EVENT_KEY_RIGHTARROW_DOWN)) return;
                if(keyboard.state.enabled) return;
                if(keyboard.cursorPos < keyboard.charsPrinted) {
                    keyboard.cursorPos++;
                    video_MoveCursor(1, 0);
                }
                break;
            case 0x50:
                // Down arrow key
                if(event_Handle(EVENT_KEY_DOWNARROW_DOWN)) return;
                break;
            case 0x5b:
                // Left GUI
                keyboard.state.GUIHeld = 1;
                if(event_Handle(EVENT_KEY_LGUI_DOWN)) return;
                break;
            case 0x5c:
                // Right GUI
                keyboard.state.GUIHeld = 1;
                if(event_Handle(EVENT_KEY_RGUI_DOWN)) return;
                break;
        }
    } else {
        // Key released
        keyboard.secondaryKeys[scanCode] = 0;
        switch(scanCode) {
            case 0x1c:
                // Enter
                if(event_Handle(EVENT_KEY_ENTER_UP)) return;
                break;
            case 0x1d:
                // Right ctrl
                keyboard.state.ctrlHeld = keyboard.keys[0x1d];
                if(event_Handle(EVENT_KEY_RCTRL_UP)) return;
                break;
            case 0x35:
                // Keypad '/'
                currentState.stateDetails = '/';
                if(event_Handle(EVENT_KEY_NORMAL_DOWN)) return;
                break;
            case 0x38:
                // Right alt
                keyboard.state.altHeld = keyboard.keys[0x38];
                if(event_Handle(EVENT_KEY_RALT_UP)) return;
                break;
            case 0x48:
                // Up arrow key
                if(event_Handle(EVENT_KEY_UPARROW_UP)) return;
                break;
            case 0x4b:
                // Left arrow key
                if(event_Handle(EVENT_KEY_LEFTARROW_UP)) return;
                break;
            case 0x4d:
                // Right arrow key
                if(event_Handle(EVENT_KEY_RIGHTARROW_UP)) return;
                break;
            case 0x50:
                // Down arrow key
                if(event_Handle(EVENT_KEY_DOWNARROW_UP)) return;
                break;
            case 0x5b:
                // Left GUI
                keyboard.state.GUIHeld = keyboard.secondaryKeys[0x5c];
                if(event_Handle(EVENT_KEY_LGUI_UP)) return;
                break;
            case 0x5c:
                // Right GUI
                keyboard.state.GUIHeld = keyboard.secondaryKeys[0x5b];
                if(event_Handle(EVENT_KEY_RGUI_UP)) return;
                break;
        }
    }
}

byte awaitingKey = 0;
static void keyboard_Callback(Registers* registers) {
    byte scanCode = port_ByteIn(0x60);

    if(awaitingKey) {
        keyboard_HandleSpecialKey(scanCode);
        awaitingKey = 0;
        return;
    }

    // Handle the scanCode
    if(scanCode <= 0x58) {
        // Key press down
        char key = (keyboard.state.uppercase ? uppercaseKeys[scanCode-1] : lowercaseKeys[scanCode-1]);
        keyboard.keys[scanCode] = 1;
        if(!key) {
            // Special key, handle differently
            switch(scanCode) {
                case 0x01:
                    // Escape
                    if(event_Handle(EVENT_KEY_ESC_DOWN)) return;
                    break;
                case 0x0e:
                    // Backspace
                    currentState.stateDetails = '\b';
                    if(event_Handle(EVENT_KEY_NORMAL_DOWN)) return;
                    if(keyboard.state.enabled) return;
                    if(keyboard.cursorPos > 0) {
                        keyboard.charsPrinted--;
                        keyboard.cursorPos--;
                    }
                    else break;
                    video_Print("\b");
                    break;
                case 0x0f:
                    // Tab
                    currentState.stateDetails = '\t';
                    if(event_Handle(EVENT_KEY_NORMAL_DOWN)) return;
                    if(keyboard.state.enabled) return;
                    video_Print("    ");
                    keyboard.charsPrinted += 4;
                    keyboard.cursorPos += 4;
                    break;
                case 0x1c:
                    // Enter
                    if(event_Handle(EVENT_KEY_ENTER_DOWN)) return;
                    if(keyboard.state.enabled) return;
                    video_Newline(NULL);
                    keyboard.charsPrinted = 0;
                    break;
                case 0x1d:
                    // Left control
                    keyboard.state.ctrlHeld = 1;
                    if(event_Handle(EVENT_KEY_LCTRL_DOWN)) return;
                    break;
                case 0x2a:
                    // Left shift
                    keyboard.state.shiftHeld = 1;
                    keyboard.state.uppercase = !keyboard.state.capsLock;
                    if(event_Handle(EVENT_KEY_LSHIFT_DOWN)) return;
                    break;
                case 0x36:
                    // Right shift
                    keyboard.state.shiftHeld = 1;
                    keyboard.state.uppercase = !keyboard.state.capsLock;
                    if(event_Handle(EVENT_KEY_RSHIFT_DOWN)) return;
                    break;
                case 0x38:
                    // Left alt
                    keyboard.state.altHeld = 1;
                    if(event_Handle(EVENT_KEY_LALT_DOWN)) return;
                    break;
                case 0x3a:
                    // CapsLock
                    keyboard.state.capsLock = !keyboard.state.capsLock;
                    keyboard.state.uppercase = keyboard.state.capsLock | keyboard.keys[0x2a] | keyboard.keys[0x36];
                    break;
                case 0x3b:
                    // F1
                    if(event_Handle(EVENT_KEY_F1_DOWN)) return;
                    break;
                case 0x3c:
                    // F2
                    if(event_Handle(EVENT_KEY_F2_DOWN)) return;
                    break;
                case 0x3d:
                    // F3
                    if(event_Handle(EVENT_KEY_F3_DOWN)) return;
                    break;
                case 0x3e:
                    // F4

                    // Handle Alt+F4
                    if(keyboard.state.altHeld) {
                        if(event_Handle(EVENT_PROGRAM_QUIT))
                            return;
                    }

                    // Otherwise, handle normally
                    if(event_Handle(EVENT_KEY_F4_DOWN)) return;
                    break;
                case 0x3f:
                    // F5
                    if(event_Handle(EVENT_KEY_F5_DOWN)) return;
                    break;
                case 0x40:
                    // F6
                    if(event_Handle(EVENT_KEY_F6_DOWN)) return;
                    break;
                case 0x41:
                    // F7
                    if(event_Handle(EVENT_KEY_F7_DOWN)) return;
                    break;
                case 0x42:
                    // F8
                    if(event_Handle(EVENT_KEY_F8_DOWN)) return;
                    break;
                case 0x43:
                    // F9
                    if(event_Handle(EVENT_KEY_F9_DOWN)) return;
                    break;
                case 0x44:
                    // F10
                    if(event_Handle(EVENT_KEY_F10_DOWN)) return;
                    break;
                case 0x45:
                    // NumberLock
                    keyboard.state.numLock = 1;
                    break;
                case 0x46:
                    // ScrollLock
                    keyboard.state.scrollLock = 1;
                    break;
                case 0x57:
                    // F11
                    if(event_Handle(EVENT_KEY_F11_DOWN)) return;
                    break;
                case 0x58:
                    // F12
                    if(event_Handle(EVENT_KEY_F12_DOWN)) return;
                    break;
            }
            keyboard_UpdateLEDs();
        } else {
            currentState.stateDetails = key;
            if(event_Handle(EVENT_KEY_NORMAL_DOWN)) return;
            if(keyboard.state.enabled) return;
            video_Print((const char[]){key, 0});
            keyboard.charsPrinted++;
            keyboard.cursorPos++;
        }
    } else if(scanCode <= 0xd8) {
        // Key released
        scanCode -= 0x80;
        char key = (keyboard.state.uppercase ? uppercaseKeys[scanCode-1] : lowercaseKeys[scanCode-1]);
        keyboard.keys[scanCode] = 0;
        if(!key) {
            // Special key, handle differently
            switch(scanCode) {
                case 0x01:
                    // Escape
                    if(event_Handle(EVENT_KEY_ESC_UP)) return;
                    break;
                case 0x0e:
                    // Backspace
                    currentState.stateDetails = '\b';
                    if(event_Handle(EVENT_KEY_NORMAL_UP)) return;
                    break;
                case 0x0f:
                    // Tab
                    currentState.stateDetails = '\t';
                    if(event_Handle(EVENT_KEY_NORMAL_UP)) return;
                    break;
                case 0x1c:
                    // Enter
                    if(event_Handle(EVENT_KEY_ENTER_UP)) return;
                    break;
                case 0x1d:
                    // Left control
                    keyboard.state.ctrlHeld = keyboard.secondaryKeys[0x1d];
                    if(event_Handle(EVENT_KEY_LCTRL_UP)) return;
                    break;
                case 0x2a:
                    // Left shift
                    keyboard.state.shiftHeld = keyboard.keys[0x36];
                    keyboard.state.uppercase = keyboard.state.capsLock & !keyboard.keys[0x36];
                    if(event_Handle(EVENT_KEY_LSHIFT_UP)) return;
                    break;
                case 0x36:
                    // Right shift
                    keyboard.state.shiftHeld = keyboard.keys[0x2a];
                    keyboard.state.uppercase = keyboard.state.capsLock & !keyboard.keys[0x2a];
                    if(event_Handle(EVENT_KEY_RSHIFT_UP)) return;
                    break;
                case 0x38:
                    // Left alt
                    keyboard.state.altHeld = keyboard.secondaryKeys[0x38];
                    if(event_Handle(EVENT_KEY_LALT_UP)) return;
                    break;
                case 0x3b:
                    // F1
                    if(event_Handle(EVENT_KEY_F1_UP)) return;
                    break;
                case 0x3c:
                    // F2
                    if(event_Handle(EVENT_KEY_F2_UP)) return;
                    break;
                case 0x3d:
                    // F3
                    if(event_Handle(EVENT_KEY_F3_UP)) return;
                    break;
                case 0x3e:
                    // F4
                    if(event_Handle(EVENT_KEY_F4_UP)) return;
                    break;
                case 0x3f:
                    // F5
                    if(event_Handle(EVENT_KEY_F5_UP)) return;
                    break;
                case 0x40:
                    // F6
                    if(event_Handle(EVENT_KEY_F6_UP)) return;
                    break;
                case 0x41:
                    // F7
                    if(event_Handle(EVENT_KEY_F7_UP)) return;
                    break;
                case 0x42:
                    // F8
                    if(event_Handle(EVENT_KEY_F8_UP)) return;
                    break;
                case 0x43:
                    // F9
                    if(event_Handle(EVENT_KEY_F9_UP)) return;
                    break;
                case 0x44:
                    // F10
                    if(event_Handle(EVENT_KEY_F10_UP)) return;
                    break;
                case 0x45:
                    // NumberLock
                    keyboard.state.numLock = 0;
                    break;
                case 0x46:
                    // ScrollLock
                    keyboard.state.scrollLock = 0;
                    break;
                case 0x57:
                    // F11
                    if(event_Handle(EVENT_KEY_F11_UP)) return;
                    break;
                case 0x58:
                    // F12
                    if(event_Handle(EVENT_KEY_F12_UP)) return;
                    break;
            }
            keyboard_UpdateLEDs();
        } else {
            currentState.stateDetails = key;
            if(event_Handle(EVENT_KEY_NORMAL_UP)) return;
        }
    } else if(scanCode == 0xe0) {
        awaitingKey = 1;
        return;
    }
}

void keyboard_Init() {
    irq_Register(IRQ1, keyboard_Callback);
    // Register the keyboard with the event handler
    currentState.keyboard = &keyboard;
}

void keyboard_Enable() {
    keyboard.state.enabled = 0;
}

void keyboard_Disable() {
    keyboard.state.enabled = 1;
}