#ifndef FS_MODIFY_H
#define FS_MODIFY_H

typedef enum {
    MODIFY_SUCCESS = 0,       // Success
    MODIFY_ERROR,             // General error
    MODIFY_INVALIDPATH,       // Invalid path passed into the function
    MODIFY_NOFILE,            // Tried to delete a non-existent file
    MODIFY_DIRECTORYNOTEMPTY, // Tried to remove a non-empty directory
    MODIFY_FILEEXISTS,        // Tried to create a file/directory which already exists
    MODIFY_INVALIDNAME,       // Tried to create a file/directory with an invalid name
                              // This means including an extension for a directory or not including one for a file
    MODIFY_NAMETOOLONG,       // The name was too long
} ModifyReturnCode;

ModifyReturnCode fs_CreateDirectory(const char* path);

ModifyReturnCode fs_CreateFile(const char* path);

ModifyReturnCode fs_RemoveDirectory(const char* path);

ModifyReturnCode fs_RemoveFile(const char* path);

#endif