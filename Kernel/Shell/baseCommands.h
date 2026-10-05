#ifndef SHELL_BASECOMMANDS_H
#define SHELL_BASECOMMANDS_H

#include "command.h"

typedef int (*CommandFunc)(Command);

CommandFunc shell_GetCommandFunc(Command command);

#endif