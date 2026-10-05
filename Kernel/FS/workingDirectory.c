#include "../Constants/FSConstants.h"
#include "../OS/memory.h"
#include "../Lib/string.h"
#include "workingDirectory.h"

dWord __WorkingDirectoryCluster = 0;

char WorkingDirectoryString[WORKING_DIR_MAX_LEN] = "C:\\";

char* fs_GetFullPath(const char* relativePathRaw) {
    char* relativePath = strupr(relativePathRaw);

    // Check if a full path was passed in
    if(relativePath[0] >= 'A' && relativePath[0] <= 'Z' && relativePath[1] == ':' && relativePath[2] == '\\')
        return relativePath;

    // Check if an improper drive with a full path was passed in
    if(relativePath[1] == ':' && relativePath[2] == '\\') {
        memory_Free(relativePath);
        return strdup(WorkingDirectoryString);
    }

    // Replace all '/'s with '\'s
    int i, len = strlen(relativePath);
    for(i=0;i<len;i++) {
        if(relativePath[i] == '/') relativePath[i] = '\\';
    }

    // Copy the path over
    char* path;
    if(relativePath[0] == '\\') {
        path = memory_Callocate(strlen(relativePath) + 3);
        path[0] = WorkingDirectoryString[0];
        path[1] = ':';
        strcat(path, relativePath);
    } else {
        path = memory_Callocate(strlen(relativePath) + strlen(WorkingDirectoryString) + 2);
        strcpy(path, WorkingDirectoryString);
        if(WorkingDirectoryString[strlen(WorkingDirectoryString)-1] != '\\')
            strcat(path, "\\");
        strcat(path, relativePath);
    }

    // Handle "." and ".."
    char* truePath = memory_Callocate(strlen(path) + 1);
    char* token, *stack[MAX_SUBDIRS_IN_STRING];
    int stackIndex = 0;
    token = strtok(path, '\\');
    while(token) {
        if(strequ(token, "."));
        else if(strequ(token, "..")) {
            if(stackIndex) stackIndex--;
        } else
            stack[stackIndex++] = token;
        token = strtok(NULL, '\\');
    }

    // Reconstruct the path
    if(!stackIndex) {
        truePath[0] = WorkingDirectoryString[0];
        truePath[1] = ':';
        truePath[2] = '\\';
    } else {
        // Check if a drive was specified
        if(stack[0][0] >= 'A' && stack[0][0] <= 'Z' && stack[0][1] == ':') {
            truePath[0] = stack[0][0];
            truePath[1] = ':';
            truePath[2] = '\\';
        } else {
            truePath[0] = WorkingDirectoryString[0];
            truePath[1] = ':';
            truePath[2] = '\\';
        }

        for(i=1;i<stackIndex;i++) {
            strcat(truePath, stack[i]);
            if(i != stackIndex-1) strcat(truePath, "\\");
        }
    }

    // Ensure that no 2 '\'s are next to each other
    len = strlen(truePath);
    for(i=0;i<len-1;) {
        if(truePath[i] == '\\' && truePath[i+1] == '\\') {
            strcpy(&truePath[i], &truePath[i+1]);
        } else i++;
    }

    // Remove the final '\' if exists unless it's the '\' at the end of "[Drive]:\"
    if(truePath[strlen(truePath)-1] == '\\' && strlen(truePath) > 3)
        truePath[strlen(truePath)-1] = 0;

    memory_Free(relativePath);
    memory_Free(path);
    return truePath;
}

char* fs_GetWorkingDirectory(void) {
    return WorkingDirectoryString;
}

byte fs_ChangeWorkingDirectory(const char* newWorkingDirectory) {
    if(strlen(newWorkingDirectory) > WORKING_DIR_MAX_LEN) return 0;

    // Get the full path of newWorkingDirectory
    char* fullWorkingDirectory = fs_GetFullPath(newWorkingDirectory);
    strcpy(WorkingDirectoryString, fullWorkingDirectory);
    memory_Free(fullWorkingDirectory);
    return 1;
}