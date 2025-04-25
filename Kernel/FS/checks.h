#include "../OS/types.h"

#ifndef FS_CHECKS_H
#define FS_CHECKS_H

byte fs_Exists(const char* path);

byte fs_IsDirectory(const char* path);

byte fs_DirectoryIsEmpty(const char* path);

#endif