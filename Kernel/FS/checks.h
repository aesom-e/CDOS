#ifndef FS_CHECKS_H
#define FS_CHECKS_H

#include "../OS/types.h"

/**
* \brief Checks if the specified path exists on disk
* \param path The path to check
* \return 1 if the path exists, 0 if not
*/
byte fs_Exists(const char* path);

/**
* \brief Checks if the specified path is a directory
* \param path The path to check
* \return 1 if the path is a directory, 0 if not
*/
byte fs_IsDirectory(const char* path);

/**
* \brief Checks if the specified directory is empty
* \param path The path to check
* \return 1 if the directory is empty, 0 if not
*/
byte fs_DirectoryIsEmpty(const char* path);

#endif