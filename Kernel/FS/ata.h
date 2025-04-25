#include "../OS/types.h"

// Private
#ifndef FS_ATA_H
#define FS_ATA_H

void fs_ATAReadSector(dWord lba, byte* buffer);

void fs_ATAWriteSector(dWord lba, byte* buffer);

#endif