// Private
#ifndef FS_ATA_H
#define FS_ATA_H

#include "../OS/types.h"

/**
* \brief Reads the specified ATA sector into the given buffer
* \param lba The Logical Block Address to read
* \param buffer The 512 byte buffer to fill with the read data
*/
void fs_ATAReadSector(dWord lba, byte* buffer);

/**
* \brief Writes the given data into the specified ATA sector
* \param lba The Logical Block Address to write
* \param buffer The 512 byte buffer to write to disk
*/
void fs_ATAWriteSector(dWord lba, byte* buffer);

#endif