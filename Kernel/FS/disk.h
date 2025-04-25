#include "../OS/types.h"

#ifndef FS_DISK_H
#define FS_DISK_H

typedef struct {
    word  bytesPerSector;
    byte  sectorsPerCluster;
    word  reservedSectorCount;
    byte  numFATs;
    dWord sectorsPerFAT;
    dWord rootCluster;
    dWord FATStartSector;
    dWord dataStartSector;
} DiskBootSector;

struct __Disk {
    DiskBootSector bootSector;
    word           exists;
};

extern struct __Disk Disk;

extern struct __Disk* Disks[];

#endif