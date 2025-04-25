#include "../OS/memory.h"
#include "workingDirectory.h"
#include "file.h"

byte fs_Exists(const char* pathRaw) {
    char* path = fs_GetFullPath(pathRaw);

    File file = fs_OpenFile(path);
    if(file.name[0]) {
        fs_CloseFile(file);
        memory_Free(path);
        return 1;
    }
    Directory dir = fs_OpenDirectory(path);
    if(dir.exists) {
        fs_CloseDirectory(dir);
        memory_Free(path);
        return 1;
    }

    memory_Free(path);
    return 0;
}

byte fs_IsDirectory(const char* pathRaw) {
    char* path = fs_GetFullPath(pathRaw);

    Directory dir = fs_OpenDirectory(path);
    if(dir.exists) {
        fs_CloseDirectory(dir);
        memory_Free(path);
        return 1;
    }

    memory_Free(path);
    return 0;
}

byte fs_DirectoryIsEmpty(const char* pathRaw) {
    char* path = fs_GetFullPath(pathRaw);

    Directory dir = fs_OpenDirectory(path);
    if(dir.exists && !dir.numFiles) {
        fs_CloseDirectory(dir);
        memory_Free(path);
        return 1;
    }

    memory_Free(path);
    return 0;
}