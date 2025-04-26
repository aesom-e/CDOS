#include "../OS/memory.h"
#include "../Lib/string.h"
#include "workingDirectory.h"
#include "checks.h"
#include "file.h"
#include "cluster.h"
#include "disk.h"
#include "ata.h"
#include "modify.h"

void fs_FormatDirectoryEntry(File* file, struct __DirectoryEntry* directoryEntry) {
    // Get the name and extension
    int i;
    for(i=0;i<8;i++) directoryEntry->name[i] = file->name[i];
    char* extension = strchr(file->name, '.');
    if(extension) {
        for(i=0;i<3;i++) directoryEntry->extension[i] = extension[i+1];
    }

    // Get the times
    Time createTime    = file->createTime;
    Time lastWriteTime = file->lastWriteTime;
    time_AdjustTimeZone(&createTime, -TIMEZONE_SYSTEM);
    time_AdjustTimeZone(&lastWriteTime, -TIMEZONE_SYSTEM);

    // Get the first cluster
    directoryEntry->firstClusterHigh = file->firstCluster << 16;
    directoryEntry->firstClusterLow  = file->firstCluster & 0xffff;

    // Copy the rest
    directoryEntry->attributes = file->attributes;
    directoryEntry->reserved   = file->reserved;
    directoryEntry->fileSize   = file->size;
}

byte fs_AddFileToParentDirectory(char* parentDirectory, File* file) {
    if(!fs_DirectoryExists(parentDirectory)) return 0;

    // Construct the FAT32 entry
    struct __DirectoryEntry fileEntry = {0};
    fs_FormatDirectoryEntry(file, &fileEntry);

    Directory dir = fs_OpenDirectory(parentDirectory);
    if(!dir.__cluster) {
        fs_CloseDirectory(dir);
        return 0;
    }

    while(dir.__cluster < 0x0ffffff8) {
        dWord sector = fs_ClusterToSector(dir.__cluster);

        // Read all sectors in the cluster
        int i;
        for(i=0;i<Disk.bootSector.sectorsPerCluster;i++) {
            byte* sectorBuffer = memory_Callocate(Disk.bootSector.bytesPerSector);
            fs_ATAReadSector(sector+i, sectorBuffer);

            // Search for an empty directory entry
            int j;
            for(j=0;j<Disk.bootSector.bytesPerSector;j+=32) {
                if(sectorBuffer[j] == 0x00 || sectorBuffer[j] == 0xe5) {
                    memory_Copy((byte*)&fileEntry, &(sectorBuffer[j]), sizeof(struct __DirectoryEntry));
                    fs_ATAWriteSector(sector+i, sectorBuffer);
                    memory_Free(sectorBuffer);
                    fs_CloseDirectory(dir);
                    return 1;
                }
            }
        }

        // Move to the next cluster
        dir.__cluster = fs_GetNextCluster(dir.__cluster);
    }

    // No space
    fs_CloseDirectory(dir);
    return 0;
}

void fs_UpdateDirectoryEntry(Directory* dir) {
    dWord cluster = dir->__cluster;
    byte* buffer = memory_Callocate(Disk.bootSector.bytesPerSector * Disk.bootSector.sectorsPerCluster);

    // Reconstruct the sector
    int i;
    for(i=0;i<dir->numFiles;i++) {
        struct __DirectoryEntry entry = {0};
        fs_FormatDirectoryEntry(&dir->files[i], &entry);
        memory_Copy((byte*)&entry, buffer+(i*32), sizeof(struct __DirectoryEntry));
    }
    fs_ATAWriteSector(fs_ClusterToSector(cluster), buffer);

    memory_Free(buffer);
}

ModifyReturnCode fs_CreateDirectory(const char* pathRaw) {
    char* path = fs_GetFullPath(pathRaw);

    if(fs_Exists(path)) {
        memory_Free(path);
        return MODIFY_FILEEXISTS;
    }

    // Get the name of the new directory
    char* dirName = strrchr(path, '\\');
    if(!dirName) {
        memory_Free(path);
        return MODIFY_INVALIDPATH;
    }
    dirName++;

    // Ensure the name isn't too long
    if(strlen(dirName) > 8) {
        memory_Free(path);
        return MODIFY_NAMETOOLONG;
    }

    // Construct the new directory which acts in FAT as a file
    File newDirectory = {0};
    strcpy(newDirectory.name, dirName);
    newDirectory.attributes = ATTRIBUTE_DIRECTORY;
    newDirectory.firstCluster = fs_CreateCluster();

    // Add the file to the parent directory
    *dirName = 0;
    if(!fs_AddFileToParentDirectory(path, &newDirectory)) {
        memory_Free(path);
        return MODIFY_ERROR;
    }

    memory_Free(path);
    return MODIFY_SUCCESS;
}

ModifyReturnCode fs_CreateFile(const char* pathRaw) {
    char* path = fs_GetFullPath(pathRaw);

    if(fs_Exists(path)) {
        memory_Free(path);
        return MODIFY_FILEEXISTS;
    }

    // Get the name of the file
    char* fileName = strrchr(path, '\\');
    if(!fileName) {
        memory_Free(path);
        return MODIFY_INVALIDPATH;
    }
    fileName++;

    // Check that the name is not too long
    if(strlen(fileName) > 12) {
        memory_Free(path);
        return MODIFY_NAMETOOLONG;
    }
    char* extensionStart = strchr(fileName, '.');
    if(!extensionStart) {
        memory_Free(path);
        return MODIFY_INVALIDNAME;
    }
    if(extensionStart-fileName > 8 || strlen(extensionStart+1) > 3) {
        memory_Free(path);
        return MODIFY_NAMETOOLONG;
    }

    // Construct the file
    File newFile = {0};
    strcpy(newFile.name, fileName);
    newFile.firstCluster = fs_CreateCluster();

    // Add the file to the parent directory
    *fileName = 0;
    if(!fs_AddFileToParentDirectory(path, &newFile)) {
        memory_Free(path);
        return MODIFY_ERROR;
    }

    memory_Free(path);
    return MODIFY_SUCCESS;
}

// Note that this code was essentially copied from fs_RemoveFile because directories are treated as files within FAT
ModifyReturnCode fs_RemoveDirectory(const char* pathRaw) {
    char* path = fs_GetFullPath(pathRaw);

    // Get the directory that holds the file
    char* parentPath    = strdup(path);
    char* lastBackSlash = strrchr(parentPath, '\\');
    if(!lastBackSlash) {
        memory_Free(parentPath);
        memory_Free(path);
        return MODIFY_INVALIDPATH;
    }

    // Cut the path off at after the last backslash to get the parent directory
    // After the directory is found, the path will be put back together so the file name
    // can be searched for
    char fileFirstCharacter = *(lastBackSlash+1);
    *(lastBackSlash+1) = 0;

    Directory dir = fs_OpenDirectory(parentPath);
    if(!dir.exists) {
        memory_Free(parentPath);
        memory_Free(path);
        return MODIFY_INVALIDPATH;
    }

    // Put the file name back together
    *(lastBackSlash+1) = fileFirstCharacter;

    // Locate the dir within the parent directory
    byte found = 0;
    word fileIndex;
    for(fileIndex=0;fileIndex<dir.numFiles;fileIndex++) {
        if((dir.files[fileIndex].attributes & ATTRIBUTE_DIRECTORY)
        && strequ(dir.files[fileIndex].name, lastBackSlash+1)) {
            found = 1;
            break;
        }
    }
    if(!found) {
        fs_CloseDirectory(dir);
        memory_Free(parentPath);
        memory_Free(path);
        return MODIFY_NOFILE;
    }

    // Free the file's cluster
    File* file = &dir.files[fileIndex];
    dWord cluster = file->firstCluster;
    while(cluster < 0x0ffffff8 && cluster) {
        dWord nextCluster = fs_GetNextCluster(cluster);
        fs_DeleteCluster(cluster);
        cluster = nextCluster;
    }

    // Mark the entry as deleted
    file->name[0] = 0xe5;
    fs_UpdateDirectoryEntry(&dir);

    fs_CloseDirectory(dir);
    memory_Free(parentPath);
    memory_Free(path);
    return MODIFY_SUCCESS;
}

ModifyReturnCode fs_RemoveFile(const char* pathRaw) {
    char* path = fs_GetFullPath(pathRaw);

    // Get the directory that holds the file
    char* parentPath    = strdup(path);
    char* lastBackSlash = strrchr(parentPath, '\\');
    if(!lastBackSlash) {
        memory_Free(parentPath);
        memory_Free(path);
        return MODIFY_INVALIDPATH;
    }

    // Cut the path off at after the last backslash to get the parent directory
    // After the directory is found, the path will be put back together so the file name
    // can be searched for
    char fileFirstCharacter = *(lastBackSlash+1);
    *(lastBackSlash+1) = 0;

    Directory dir = fs_OpenDirectory(parentPath);
    if(!dir.exists) {
        memory_Free(parentPath);
        memory_Free(path);
        return MODIFY_INVALIDPATH;
    }

    // Put the file name back together
    *(lastBackSlash+1) = fileFirstCharacter;

    // Locate the file within the parent directory
    byte found = 0;
    word fileIndex;
    for(fileIndex=0;fileIndex<dir.numFiles;fileIndex++) {
        if(!(dir.files[fileIndex].attributes & ATTRIBUTE_DIRECTORY)
        && strequ(dir.files[fileIndex].name, lastBackSlash+1)) {
            found = 1;
            break;
        }
    }
    if(!found) {
        fs_CloseDirectory(dir);
        memory_Free(parentPath);
        memory_Free(path);
        return MODIFY_NOFILE;
    }

    // Free the file's cluster
    File* file = &dir.files[fileIndex];
    dWord cluster = file->firstCluster;
    while(cluster < 0x0ffffff8 && cluster) {
        dWord nextCluster = fs_GetNextCluster(cluster);
        fs_DeleteCluster(cluster);
        cluster = nextCluster;
    }

    // Mark the entry as deleted
    file->name[0] = 0xe5;
    fs_UpdateDirectoryEntry(&dir);

    fs_CloseDirectory(dir);
    memory_Free(parentPath);
    memory_Free(path);
    return MODIFY_SUCCESS;
}