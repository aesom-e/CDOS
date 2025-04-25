#include "command.h"

#ifndef SHELL_BASECOMMANDS_H
#define SHELL_BASECOMMANDS_H

typedef int (*CommandFunc)(Command);

CommandFunc shell_GetCommandFunc(Command command);

#endif