#include "../Constants/shellConstants.h"
#include "../OS/registers.h"
#include "../OS/memory.h"
#include "../OS/keyboard.h"
#include "../OS/event.h"
#include "../OS/cursor.h"
#include "../OS/vga.h"
#include "../OS/video.h"
#include "../FS/fs.h"
#include "../Lib/string.h"
#include "input.h"

char* input, *previousInputs[NUM_PREVIOUS_INPUTS], *savedInput;
volatile byte continueInput = 0;
word inputIndex = 0, inputLen, numPreviousInputs = 0, scrollPos = 0;
byte inputStartLine = 0;

byte input_End(EventState* state) {
    continueInput = 0;
    keyboard_Disable();
    cursor_Disable();
    return 0;
}

byte input_Key(EventState* state) {
    // If the user tries to type more keys than allowed, stop it
    if(inputIndex == inputLen-1 && state->stateDetails != '\b') return 1;

    switch(state->stateDetails) {
        case '\b':
            if(inputIndex) input[--inputIndex] = 0;
            break;
        case '\t': {
            byte i;
            // Stop the OS from handling a tab if the required number of spaces can't be filled
            if(inputIndex >= inputLen-3) return 1;
            for(i=0;i<4;i++) input[inputIndex++] = ' ';
            break;
        }
        default:
            input[inputIndex++] = state->stateDetails;
            break;
    }

    return 0;
}
byte input_ArrowUp(EventState* state) {
    if(scrollPos >= numPreviousInputs) return 1;
    if(scrollPos < numPreviousInputs) scrollPos++;

    // Save the input
    if(scrollPos == 1)
        savedInput = strdup(input);

    // Set the input to whatever the previous input was
    int len = strlen(input);
    strcpy(input, previousInputs[numPreviousInputs-scrollPos]);
    Position cursorPos = vga_GetCursorPosition();
    cursorPos.y = inputStartLine;
    cursorPos.x = strlen(fs_GetWorkingDirectory()) + 1;
    vga_SetCursorPosition(cursorPos);
    int i;
    for(i=0;i<len;i++)
        video_SetRawByte(' ', (cursorPos.y * 80 + cursorPos.x + i)*2);
    vga_SetCursorPosition(cursorPos);

    video_Print(input);
    keyboard.charsPrinted = strlen(input);
    keyboard.cursorPos = keyboard.charsPrinted;
    inputIndex = keyboard.charsPrinted;

    return 1;
}

byte input_ArrowDown(EventState* state) {
    if(!scrollPos) return 1;
    scrollPos--;

    int len = strlen(input);
    // Handle if the user scrolled back to 0
    if(!scrollPos) {
        strcpy(input, savedInput);
        memory_Free(savedInput);
    } else strcpy(input, previousInputs[numPreviousInputs-scrollPos]);

    // Show the new input
    Position cursorPos = vga_GetCursorPosition();
    cursorPos.y = inputStartLine;
    cursorPos.x = strlen(fs_GetWorkingDirectory()) + 1;
    vga_SetCursorPosition(cursorPos);
    int i;
    for(i=0;i<len;i++)
        video_SetRawByte(' ', (cursorPos.y * 80 + cursorPos.x + i)*2);
    vga_SetCursorPosition(cursorPos);

    video_Print(input);
    keyboard.charsPrinted = strlen(input);
    keyboard.cursorPos = keyboard.charsPrinted;
    inputIndex = keyboard.charsPrinted;

    return 1;
}

void input_StorePreviousInput(void) {
    if(!input[0]) return;
    if(scrollPos && strequ(input, previousInputs[numPreviousInputs-scrollPos]))
        return;

    if(numPreviousInputs < NUM_PREVIOUS_INPUTS-1) {
        previousInputs[numPreviousInputs++] = strdup(input);
        return;
    }

    memory_Free(previousInputs[0]);
    int i;
    for(i=0;i<NUM_PREVIOUS_INPUTS-1;i++) previousInputs[i] = previousInputs[i+1];
    previousInputs[NUM_PREVIOUS_INPUTS-1] = strdup(input);
}

char* input_Get(word maxLen) {
    // Allocate memory
    input = memory_Callocate(maxLen+1);

    // Set up the event handlers
    event_Register(EVENT_KEY_ENTER_DOWN, input_End);
    event_Register(EVENT_KEY_NORMAL_DOWN, input_Key);
    event_Register(EVENT_KEY_UPARROW_DOWN, input_ArrowUp);
    event_Register(EVENT_KEY_DOWNARROW_DOWN, input_ArrowDown);

    // Initialize the environment
    continueInput = 1;
    inputIndex = 0;
    inputLen = maxLen;
    inputStartLine = vga_GetCursorPosition().y;
    keyboard.charsPrinted = 0;
    keyboard.cursorPos = 0;

    // Read keyboard input until the enter key is hit
    keyboard_Enable();
    cursor_Enable();
    while(continueInput);

    // Deregister the events
    event_Deregister(EVENT_KEY_ENTER_DOWN);
    event_Deregister(EVENT_KEY_NORMAL_DOWN);
    event_Deregister(EVENT_KEY_UPARROW_DOWN);
    event_Deregister(EVENT_KEY_DOWNARROW_DOWN);

    input_StorePreviousInput();
    scrollPos = 0;
    return input;
}