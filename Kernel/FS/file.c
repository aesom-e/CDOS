#include "../OS/memory.h"
#include "../Lib/string.h"
#include "disk.h"
#include "ata.h"
#include "cluster.h"
#include "workingDirectory.h"
#include "file.h"

#define min(a, b) (((a)>(b)) ? (b) : (a))

void fs_FormatFile(File* file, struct __DirectoryEntry* entry) {
    // Format the name
    int i, j;
    for(i=0;i<8&&entry->name[i]!=' ';i++) file->name[i] = entry->name[i];
    if(entry->extension[0] != ' ') {
        file->name[i++] = '.';
        for(j=0;j<3&&entry->extension[j]!=' ';j++) file->name[i++] = entry->extension[j];
    }
    file->name[i] = 0;

    // Get the creation and write time
    file->createTime    = time_GetFATTime(entry->createDate, entry->createTime);
    file->lastWriteTime = time_GetFATTime(entry->lastWriteDate, entry->lastWriteTime);
    time_AdjustTimeZone(&file->createTime, TIMEZONE_SYSTEM);
    time_AdjustTimeZone(&file->lastWriteTime, TIMEZONE_SYSTEM);

    // Copy the rest of the data from the entry to the file
    file->attributes   = entry->attributes;
    file->reserved     = entry->reserved;
    file->firstCluster = (entry->firstClusterHigh << 16) | entry->firstClusterLow;
    file->size         = entry->fileSize;
}

Directory fs_OpenDirectoryByCluster(struct __Disk* disk, dWord cluster) {
    dWord sector = fs_ClusterToSector(cluster);
    byte* buffer = memory_Callocate(disk->bootSector.bytesPerSector * disk->bootSector.sectorsPerCluster);
    fs_ATAReadSector(sector, buffer);

    int i, numEntries = 0;
    for(i=0;i<disk->bootSector.bytesPerSector * disk->bootSector.sectorsPerCluster;i+=32) {
        struct __DirectoryEntry* entry = (struct __DirectoryEntry*)(buffer + i);
        if(!entry->name[0])              break;    // No more entries
        if(entry->name[0] == (char)0xe5) continue; // Deleted entry
        if(entry->attributes == 0x0f)    continue; // Long name entry

        numEntries++;
    }

    Directory dir = {
            .files     = memory_Callocate(sizeof(File) * numEntries),
            .numFiles  = numEntries,
            .exists    = 1,
            .__cluster = cluster
    };

    numEntries = 0;
    for(i=0;i<disk->bootSector.bytesPerSector * disk->bootSector.sectorsPerCluster;i+=32) {
        struct __DirectoryEntry* entry = (struct __DirectoryEntry*)(buffer + i);
        if(!entry->name[0])              break;    // No more entries
        if(entry->name[0] == (char)0xe5) continue; // Deleted entry
        if(entry->attributes == 0x0f)    continue; // Long name entry

        // Create the File from the entry
        File* file = &(dir.files[numEntries++]);
        fs_FormatFile(file, entry);
        file->__disk = (dWord)&disk;
    }

    memory_Free(buffer);
    return dir;
}

Directory fs_OpenDirectory(const char* pathRaw) {
    Directory dir = {.exists=0};
    char* path = fs_GetFullPath(pathRaw);

    struct __Disk* disk = Disks[path[0]];
    if(!disk || !disk->exists) {
        memory_Free(path);
        return dir;
    }

    // Check if the call is for the root directory
    if(!path[3]) return fs_OpenDirectoryByCluster(disk, disk->bootSector.rootCluster);

    char* temp = strdup(path + 3);
    if(!temp) return dir;
    char* nextDir = strtok(temp, '\\');
    dWord cluster = disk->bootSector.rootCluster;
    while(nextDir) {
        Directory currentDir = fs_OpenDirectoryByCluster(disk, cluster);

        word i;
        byte found = 0;
        for(i=0;i<currentDir.numFiles;i++) {
            if(currentDir.files[i].attributes & ATTRIBUTE_DIRECTORY
            && strequ(currentDir.files[i].name, nextDir)) {
                cluster = currentDir.files[i].firstCluster;
                found = 1;
                break;
            }
        }

        fs_CloseDirectory(currentDir);
        if(!found) {
            memory_Free(temp);
            memory_Free(path);
            return dir;
        }

        nextDir = strtok(NULL, '\\');
    }

    memory_Free(temp);
    memory_Free(path);
    return fs_OpenDirectoryByCluster(disk, cluster);
}

File fs_OpenFile(const char* pathRaw) {
    File file = {0};
    char* path = fs_GetFullPath(pathRaw);

    // Split the path into the directory and file name
    char* lastBackslash = 0;
    int i, len = strlen(path);
    for(i=0;i<len;i++) {
        if(path[i] == '\\') lastBackslash = &path[i];
    }
    if(!lastBackslash) {
        memory_Free(path);
        return file;
    }
    char* directory = strdup(path);
    directory[lastBackslash-path+1] = 0;
    char* fileName = lastBackslash+1;

    Directory dir = fs_OpenDirectory(directory);
    memory_Free(directory);
    if(!dir.exists) {
        memory_Free(path);
        return file;
    }

    for(i=0;i<dir.numFiles;i++) {
        if(strequ(dir.files[i].name, fileName) && !(dir.files[i].attributes & ATTRIBUTE_DIRECTORY)) {
            file = dir.files[i];
            fs_CloseDirectory(dir);
            memory_Free(path);
            return file;
        }
    }

    memory_Free(path);
    return file;
}

char* fs_ReadFileContents(File file) {
    if(!file.name[0] || !file.size) return NULL;

    char* contents = memory_Callocate(((file.size / 512)+1) * 512 + 1);

    dWord cluster   = file.firstCluster;
    dWord bytesRead = 0;
    byte* buffer    = (byte*)contents;
    while(bytesRead < file.size) {
        dWord sector = fs_ClusterToSector(cluster);
        fs_ATAReadSector(sector, buffer);

        dWord bytesToRead = min(((struct __Disk*)file.__disk)->bootSector.bytesPerSector, file.size - bytesRead);
        buffer += bytesToRead;
        bytesRead += bytesToRead;

        cluster = fs_GetNextCluster(cluster);
    }

    return contents;
}

void fs_CloseDirectory(Directory directory) {
    memory_Free(directory.files);
    directory.numFiles = 0;
}

void fs_CloseFile(File file) {
    // This does nothing right now
    return;
}

byte fs_FileExists(const char* pathRaw) {
    char* path = fs_GetFullPath(pathRaw);

    File file = fs_OpenFile(path);
    if(!file.name[0]) {
        fs_CloseFile(file);
        return 0;
    }
    fs_CloseFile(file);

    memory_Free(path);
    return 1;
}

byte fs_DirectoryExists(const char* pathRaw) {
    char* path = fs_GetFullPath(pathRaw);

    Directory dir = fs_OpenDirectory(path);
    if(!dir.exists) {
        fs_CloseDirectory(dir);
        return 0;
    }
    fs_CloseDirectory(dir);

    memory_Free(path);
    return 1;
}