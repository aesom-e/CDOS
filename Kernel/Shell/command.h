#include "../OS/types.h"

#ifndef SHELL_COMMAND_H
#define SHELL_COMMAND_H

typedef struct {
    char** arguments;
    byte numArguments;
} Command;

Command command_Parse(char* commandString);

void command_Free(Command command);

#endif