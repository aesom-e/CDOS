#include "../Constants/shellConstants.h"
#include "../FS/workingDirectory.h"
#include "../Lib/lib.h"
#include "../OS/os.h"
#include "input.h"
#include "command.h"
#include "baseCommands.h"
#include "terminal.h"

char* commandString = NULL;

void terminal_TakeInput(void) {
    // Print the working dir and ">"
    if(vga_GetCursorPosition().x != 0) video_Newline(NULL);
    printf("%s>", fs_GetWorkingDirectory());

    // Take input
    commandString = input_Get(COMMAND_MAX_LEN);
    video_Newline(NULL);
}

void terminal_Handle(void) {
    while(1) {
        terminal_TakeInput();
        if(!commandString) continue;
        Command command = command_Parse(commandString);
        memory_Free(commandString);

        // Create a newline if necessary
        if(vga_GetCursorPosition().x != 0) video_Newline(NULL);

        // Run the command requested if found
        CommandFunc func = shell_GetCommandFunc(command);
        if(func) {
            int code = func(command);
            switch(code) {
                case RETURN_SUCCESS: /* Nothing needs to be done here */                               break;
                case RETURN_ERROR:   printf("Error In Execution Of Previous Program");          break;
                default:             printf("Error %d In Execution Of Previous Program", code); break;
            }
        } else {
            if(command.arguments[0]) printf("Unknown command: %s\n", command.arguments[0]);
        }

        command_Free(command);
    }
}