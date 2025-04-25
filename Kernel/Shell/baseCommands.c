#include "../Constants/shellConstants.h"
#include "../Lib/string.h"
#include "../Lib/print.h"
#include "../OS/os.h"
#include "../FS/fs.h"
#include "terminal.h"
#include "baseCommands.h"

#define returnCmd(__cmd) do { memory_Free(commandName); return __cmd; } while(0)

// Commands
int cls(Command command);
int echo(Command command);
int help(Command command);
int haltCmd(Command command);
int dir(Command command);
int cd(Command command);
int cat(Command command);
int make(Command command);
int del(Command command);
int mkdir(Command command);
int rmdir(Command command);

CommandFunc shell_GetCommandFunc(Command command) {
    if(!command.arguments[0] || !command.numArguments) return NULL;

    char* commandName = strlwr(command.arguments[0]);

    if(strequ(commandName, "cls") || strequ(commandName, "clear"))
        returnCmd(cls);

    if(strequ(commandName, "echo"))
        returnCmd(echo);

    if(strequ(commandName, "help"))
        returnCmd(help);

    if(strequ(commandName, "halt"))
        returnCmd(haltCmd);

    if(strequ(commandName, "dir") || strequ(commandName, "ls"))
        returnCmd(dir);

    if(strequ(commandName, "cd"))
        returnCmd(cd);

    if(strequ(commandName, "cat"))
        returnCmd(cat);

    if(strequ(commandName, "make") || strequ(commandName, "touch"))
        returnCmd(make);

    if(strequ(commandName, "del") || strequ(commandName, "rm"))
        returnCmd(del);

    if(strequ(commandName, "mkdir"))
        returnCmd(mkdir);

    if(strequ(commandName, "rmdir"))
        returnCmd(rmdir);

    memory_Free(commandName);
    return NULL;
}

int cls(Command command) {
    video_Clear();

    return RETURN_SUCCESS;
}

int echo(Command command) {
    byte i;
    for(i=1;i<command.numArguments;i++)
        printf("%s%s", command.arguments[i], (i < command.numArguments-1 ? " " : ""));

    return RETURN_SUCCESS;
}

int help(Command command) {
    video_Clear();
    printf("Help menu\n\n"
           "CLS               | Clears the screen\n"
           "CLEAR             | Same as CLS\n"
           "ECHO []           | Prints whatever is passed into it\n"
           "DIR [Directory]   | Lists the files in the current directory \n"
           "                  | or whatever directory has been passed in\n"
           "LS [Directory]    | Same as DIR\n"
           "CD <Directory>    | Changes the current working directory\n"
           "CAT <File>        | Prints the contents of the passed in file\n"
           "MAKE <File>       | Creates a new blank file\n"
           "TOUCH <File>      | Same as MAKE\n"
           "DEL <File>        | Removes the file passed in\n"
           "RM <File>         | Same as DEL\n"
           "MKDIR <Directory> | Creates a new blank directory\n"
           "RMDIR <Directory> | Removes an empty directory\n");

    return RETURN_SUCCESS;
}

int haltCmd(Command command) {
    os_Halt();
    return RETURN_ERROR;
}

int dir(Command command) {
    char* path = NULL;
    if(command.numArguments > 1) {
        char* relativePath = strupr(command.arguments[1]);
        path = fs_GetFullPath(relativePath);
        memory_Free(relativePath);
    }
    else path = fs_GetWorkingDirectory();

    Directory dir = fs_OpenDirectory(path);

    if(!dir.exists) {
        printf("Directory %s doesn't exist\n", path);
        return RETURN_SUCCESS;
    }

    printf(" Directory of %s\n\n", path);
    if(!dir.numFiles) {
        printf("Directory is empty\n");
    } else {
        int i;
        // First, print all directories
        for(i=0;i<dir.numFiles;i++) {
            if(!(dir.files[i].attributes & ATTRIBUTE_DIRECTORY)) continue;

            // Print the name
            printf("%s", dir.files[i].name);

            // Put everything in line
            int len = strlen(dir.files[i].name);
            for(;len<23;len++) video_Print(" ");

            // Print <DIR> and the last write time
            video_Print("<DIR>");
            char* writeTime = time_GetTimeAsString(dir.files[i].lastWriteTime);
            printf(" %s\n", writeTime);
            memory_Free(writeTime);
        }

        // Then print all the files
        for(i=0;i<dir.numFiles;i++) {
            if(dir.files[i].attributes & ATTRIBUTE_DIRECTORY) continue;

            // Print the name
            printf("%s", dir.files[i].name);

            // Get the size of the file
            char fileSizeStr[11];
            int fileSizeStrLen;
            itoa(dir.files[i].size, fileSizeStr, 10, 0);
            fileSizeStrLen = strlen(fileSizeStr);

            // Format so everything is in line
            int len = strlen(dir.files[i].name);
            for(;len<28-fileSizeStrLen;len++) video_Print(" ");

            // Print the file size
            printf("%s", fileSizeStr);

            // Print the write time
            char* writeTime = time_GetTimeAsString(dir.files[i].lastWriteTime);
            printf(" %s\n", writeTime);
            memory_Free(writeTime);
        }
    }
    fs_CloseDirectory(dir);

    return RETURN_SUCCESS;
}

int cd(Command command) {
    if(command.numArguments < 2) {
        printf("CD <Directory>\n");
        return RETURN_SUCCESS;
    }

    if(!fs_DirectoryExists(command.arguments[1])) {
        char* truePath = fs_GetFullPath(command.arguments[1]);
        printf("Directory %s doesn't exist\n", truePath);
        memory_Free(truePath);
        return RETURN_SUCCESS;
    }

    return !fs_ChangeWorkingDirectory(command.arguments[1]);
}

int cat(Command command) {
    if(command.numArguments < 2) {
        printf("CAT <File>\n");
        return RETURN_SUCCESS;
    }

    if(!fs_FileExists(command.arguments[1])) {
        char* truePath = fs_GetFullPath(command.arguments[1]);
        printf("File %s doesn't exist\n", truePath);
        memory_Free(truePath);
        return RETURN_SUCCESS;
    }

    File file = fs_OpenFile(command.arguments[1]);
    char* contents = fs_ReadFileContents(file);
    printf("%s", contents);
    memory_Free(contents);
    fs_CloseFile(file);

    return RETURN_SUCCESS;
}

int make(Command command) {
    if(command.numArguments < 2) {
        printf("MAKE <File>\n");
        return RETURN_SUCCESS;
    }

    switch(fs_CreateFile(command.arguments[1])) {
        case MODIFY_SUCCESS: {
            char* truePath = fs_GetFullPath(command.arguments[1]);
            printf("%s Created\n", truePath);
            memory_Free(truePath);
            return RETURN_SUCCESS;
        }
        case MODIFY_INVALIDPATH: {
            char* truePath = fs_GetFullPath(command.arguments[1]);
            printf("Invalid path: %s\n", truePath);
            memory_Free(truePath);
            return RETURN_SUCCESS;
        }
        case MODIFY_FILEEXISTS: {
            char* truePath = fs_GetFullPath(command.arguments[1]);
            printf("%s already exists\n", truePath);
            memory_Free(truePath);
            return RETURN_SUCCESS;
        }
        case MODIFY_INVALIDNAME: {
            char* truePath = fs_GetFullPath(command.arguments[1]);
            printf("No file extension in %s\n", truePath);
            memory_Free(truePath);
            return RETURN_SUCCESS;
        }
        case MODIFY_NAMETOOLONG: {
            char* truePath = fs_GetFullPath(command.arguments[1]);
            printf("%s Has too long of a name\n", truePath);
            memory_Free(truePath);
            return RETURN_SUCCESS;
        }
        case MODIFY_ERROR:
            printf("Unknown error occurred\n");
            return RETURN_SUCCESS;
    }

    return RETURN_ERROR;
}

int del(Command command) {
    if(command.numArguments < 2) {
        printf("DEL <File>\n");
        return RETURN_SUCCESS;
    }

    switch(fs_RemoveFile(command.arguments[1])) {
        case MODIFY_SUCCESS: {
            char* truePath = fs_GetFullPath(command.arguments[1]);
            printf("Deleted %s\n", truePath);
            memory_Free(truePath);
            return RETURN_SUCCESS;
        }
        case MODIFY_INVALIDPATH: {
            char* truePath = fs_GetFullPath(command.arguments[1]);
            printf("Invalid path: %s\n", truePath);
            memory_Free(truePath);
            return RETURN_SUCCESS;
        }
        case MODIFY_NOFILE: {
            char* truePath = fs_GetFullPath(command.arguments[1]);
            printf("%s doesn't exist\n", truePath);
            memory_Free(truePath);
            return RETURN_SUCCESS;
        }
        case MODIFY_INVALIDNAME: {
            char* truePath = fs_GetFullPath(command.arguments[1]);
            printf("No file extension in %s\n", truePath);
            memory_Free(truePath);
            return RETURN_SUCCESS;
        }
        case MODIFY_NAMETOOLONG: {
            char* truePath = fs_GetFullPath(command.arguments[1]);
            printf("%s Has too long of a name\n", truePath);
            memory_Free(truePath);
            return RETURN_SUCCESS;
        }
        case MODIFY_ERROR:
            printf("Unknown error occurred\n");
            return RETURN_SUCCESS;
    }

    return RETURN_ERROR;
}

int mkdir(Command command) {
    if(command.numArguments < 2) {
        printf("MKDIR <Directory>\n");
        return RETURN_SUCCESS;
    }

    return RETURN_ERROR;
}

int rmdir(Command command) {
    if(command.numArguments < 2) {
        printf("RMDIR <Directory>\n");
        return RETURN_SUCCESS;
    }

    return RETURN_ERROR;
}