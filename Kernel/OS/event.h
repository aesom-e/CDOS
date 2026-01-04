#ifndef OS_EVENT_H
#define OS_EVENT_H

#include "types.h"
#include "keyboard.h"

typedef struct {
    KeyboardStruct* keyboard;
    byte stateDetails;
} EventState;

// The return value of an EventFunc dictates if it should cancel the default handling of the event
typedef byte (*EventFunc)(EventState*);

typedef enum {
    EVENT_KEY_ENTER_DOWN,
    EVENT_KEY_ENTER_UP,
    EVENT_KEY_LCTRL_DOWN,
    EVENT_KEY_LCTRL_UP,
    EVENT_KEY_RCTRL_DOWN,
    EVENT_KEY_RCTRL_UP,
    EVENT_KEY_LSHIFT_DOWN,
    EVENT_KEY_LSHIFT_UP,
    EVENT_KEY_RSHIFT_DOWN,
    EVENT_KEY_RSHIFT_UP,
    EVENT_KEY_LALT_DOWN,
    EVENT_KEY_LALT_UP,
    EVENT_KEY_RALT_DOWN,
    EVENT_KEY_RALT_UP,
    EVENT_KEY_LGUI_DOWN,
    EVENT_KEY_LGUI_UP,
    EVENT_KEY_RGUI_DOWN,
    EVENT_KEY_RGUI_UP,
    EVENT_KEY_ESC_DOWN,
    EVENT_KEY_ESC_UP,
    EVENT_KEY_UPARROW_DOWN,
    EVENT_KEY_UPARROW_UP,
    EVENT_KEY_DOWNARROW_DOWN,
    EVENT_KEY_DOWNARROW_UP,
    EVENT_KEY_LEFTARROW_DOWN,
    EVENT_KEY_LEFTARROW_UP,
    EVENT_KEY_RIGHTARROW_DOWN,
    EVENT_KEY_RIGHTARROW_UP,
    EVENT_KEY_F1_DOWN,
    EVENT_KEY_F1_UP,
    EVENT_KEY_F2_DOWN,
    EVENT_KEY_F2_UP,
    EVENT_KEY_F3_DOWN,
    EVENT_KEY_F3_UP,
    EVENT_KEY_F4_DOWN,
    EVENT_KEY_F4_UP,
    EVENT_KEY_F5_DOWN,
    EVENT_KEY_F5_UP,
    EVENT_KEY_F6_DOWN,
    EVENT_KEY_F6_UP,
    EVENT_KEY_F7_DOWN,
    EVENT_KEY_F7_UP,
    EVENT_KEY_F8_DOWN,
    EVENT_KEY_F8_UP,
    EVENT_KEY_F9_DOWN,
    EVENT_KEY_F9_UP,
    EVENT_KEY_F10_DOWN,
    EVENT_KEY_F10_UP,
    EVENT_KEY_F11_DOWN,
    EVENT_KEY_F11_UP,
    EVENT_KEY_F12_DOWN,
    EVENT_KEY_F12_UP,
    EVENT_KEY_NORMAL_DOWN,
    EVENT_KEY_NORMAL_UP,
    EVENT_PROGRAM_QUIT,
    EVENT_RESERVED // This is so the compiler knows the number of events
} EventType;

extern EventState currentState;

void event_Register(EventType type, EventFunc function);

void event_Deregister(EventType type);

void event_DeregisterAll(void);

byte event_Handle(EventType type);

#endif