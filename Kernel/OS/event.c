#include "event.h"

EventState currentState = {0};
EventFunc eventFunctions[EVENT_RESERVED] = {0};

void event_Register(EventType type, EventFunc function) {
    eventFunctions[type] = function;
}

void event_Deregister(EventType type) {
    eventFunctions[type] = NULL;
}

void event_DeregisterAll(void) {
    int i;
    for(i=0;i<EVENT_RESERVED;i++) eventFunctions[i] = NULL;
}

byte event_Handle(EventType type) {
    if(eventFunctions[type]) return eventFunctions[type](&currentState);
    return 0;
}