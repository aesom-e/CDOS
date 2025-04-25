#include "../Lib/lib.h"
#include "../OS/memory.h"
#include "command.h"

Command command_Parse(char* commandString) {
    Command command = {0};

    int i, len = strlen(commandString), numArgs = 1;
    for(i=0;i<len;i++) {
        if(commandString[i] == ' ') numArgs++;
    }
    command.numArguments = numArgs;
    command.arguments = memory_Callocate(numArgs * sizeof(char*));

    i = 0;
    char* tempString = strdup(commandString);
    char* argument = strtok(tempString, ' ');
    while(argument) {
        command.arguments[i++] = strdup(argument);
        argument = strtok(NULL, ' ');
    }
    memory_Free(tempString);

    return command;
}

void command_Free(Command command) {
    if(!command.arguments) return;

    byte i;
    for(i=0;i<command.numArguments;i++) {
        if(command.arguments[i]) memory_Free(command.arguments[i]);
    }
    memory_Free(command.arguments);
    command.numArguments = 0;
}