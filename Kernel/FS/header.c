#include "../OS/types.h"
#include "../OS/memory.h"
#include "ata.h"
#include "disk.h"
#include "header.h"

void fs_ReadDiskHeader(void) {
    // Read the master boot record
    byte* mbr = memory_Callocate(512);
    fs_ATAReadSector(0, mbr);

    dWord partitionStart = *(dWord*)(mbr + 0x1c6);

    // Read the boot sector
    byte* bootSector = memory_Callocate(512);
    fs_ATAReadSector(partitionStart, bootSector);

    // Parse the boot sector
    Disk.bootSector.bytesPerSector      = *(word*)(bootSector + 11);
    Disk.bootSector.sectorsPerCluster   = bootSector[13];
    Disk.bootSector.reservedSectorCount = *(word*)(bootSector + 14);
    Disk.bootSector.numFATs             = bootSector[16];
    Disk.bootSector.sectorsPerFAT       = *(dWord*)(bootSector + 36);
    Disk.bootSector.rootCluster         = *(dWord*)(bootSector + 44);
    Disk.bootSector.FATStartSector      = partitionStart + Disk.bootSector.reservedSectorCount;
    Disk.bootSector.dataStartSector     = Disk.bootSector.FATStartSector +
                                          (Disk.bootSector.numFATs * Disk.bootSector.sectorsPerFAT);
    Disk.exists = 1;

    // Cleanup
    memory_Free(mbr);
    memory_Free(bootSector);
}