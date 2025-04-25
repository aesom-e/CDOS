#include "../OS/types.h"
#include "../Lib/time.h"

#ifndef FS_FILE_H
#define FS_FILE_H

// This is mandated by FAT32
struct __DirectoryEntry {
    char  name[8];
    char  extension[3];
    byte  attributes;
    byte  reserved;
    byte  createTimeMs;
    word  createTime;
    word  createDate;
    word  lastAccessDate;
    word  firstClusterHigh;
    word  lastWriteTime;
    word  lastWriteDate;
    word  firstClusterLow;
    dWord fileSize;
} __attribute__((packed));

// These next definitions are OS-specific
typedef enum {
    ATTRIBUTE_READONLY    = 0b00000001,
    ATTRIBUTE_HIDDEN      = 0b00000010,
    ATTRIBUTE_SYSTEM      = 0b00000100,
    ATTRIBUTE_VOLUMELABEL = 0b00001000,
    ATTRIBUTE_DIRECTORY   = 0b00010000,
    ATTRIBUTE_ARCHIVE     = 0b00100000
} FileAttribute;

typedef struct {
    char  name[13];
    byte  attributes;
    byte  reserved;
    Time  createTime;
    Time  lastWriteTime;
    dWord firstCluster;
    dWord size;
    dWord __disk;
} File;

typedef struct {
    File* files;
    word  numFiles;
    byte  exists;
    dWord __cluster;
} Directory;

Directory fs_OpenDirectory(const char* path);

File fs_OpenFile(const char* path);

char* fs_ReadFileContents(File file);

void fs_CloseDirectory(Directory directory);

void fs_CloseFile(File file);

byte fs_FileExists(const char* path);

byte fs_DirectoryExists(const char* path);

#endif