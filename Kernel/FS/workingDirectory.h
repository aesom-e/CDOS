#ifndef FS_WORKINGDIRECTORY_H
#define FS_WORKINGDIRECTORY_H

#include "../OS/types.h"

extern dWord __WorkingDirectoryCluster;

char* fs_GetFullPath(const char* relativePath);

char* fs_GetWorkingDirectory(void);

// Returns 1 if the change was successful, 0 if not
byte fs_ChangeWorkingDirectory(const char* newWorkingDirectory);

#endif