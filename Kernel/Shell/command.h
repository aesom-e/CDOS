#ifndef SHELL_COMMAND_H
#define SHELL_COMMAND_H

#include "../OS/types.h"

typedef struct {
    char** arguments;
    byte numArguments;
} Command;

Command command_Parse(char* commandString);

void command_Free(Command command);

#endif