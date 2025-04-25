#include "../OS/port.h"
#include "../OS/halt.h"
#include "../Lib/print.h"
#include "ata.h"

#define ATA_PRIMARY_PORT 0x1f0
#define ATA_CONTROL_PORT 0x3f6
#define ATA_CACHE_FLUSH  0xe7
#define ATA_READ         0x20
#define ATA_WRITE        0x30

typedef enum {
    ATA_ERROR          = 0b00000001,
    ATA_INDEX          = 0b00000010,
    ATA_CORRECTEDERROR = 0b00000100,
    ATA_DATAREQUEST    = 0b00001000,
    ATA_SERVICEREQUEST = 0b00010000,
    ATA_DEVICEFAULT    = 0b00100000,
    ATA_DEVICEREADY    = 0b01000000,
    ATA_BUSY           = 0b10000000
} ATAStatus;

typedef enum {
    WAITFOR_NOTBUSY,
    WAITFOR_DEVICEREADY,
    WAITFOR_DATAREADY
} ATAWaitFor;

void fs_ATAWait(ATAWaitFor waitFor) {
    while(1) {
        byte status = port_ByteIn(ATA_CONTROL_PORT);
        if(status & ATA_ERROR || status & ATA_DEVICEFAULT) {
            printf("[ATA Error]\n");
            halt();
        }
        if(!(status & ATA_BUSY)) {
            if(waitFor == WAITFOR_NOTBUSY)                                   return;
            if(waitFor == WAITFOR_DEVICEREADY && (status & ATA_DEVICEREADY)) return;
            if(waitFor == WAITFOR_DATAREADY   && (status & ATA_DATAREQUEST)) return;
        }
    }
}

void fs_ATAFlush(void) {
    port_ByteOut(ATA_PRIMARY_PORT + 7, ATA_CACHE_FLUSH);
    fs_ATAWait(WAITFOR_DEVICEREADY);
}

void fs_ATAReadSector(dWord lba, byte* buffer) {
    port_ByteOut(ATA_PRIMARY_PORT + 6, 0xe0 | ((lba >> 24) & 0x0f)); // Select the drive and LBA
    port_ByteOut(ATA_PRIMARY_PORT + 2, 1);                           // Select the number of sectors to read
    port_ByteOut(ATA_PRIMARY_PORT + 3, (byte)(lba & 0xff));          // Send the low byte of the LBA
    port_ByteOut(ATA_PRIMARY_PORT + 4, (byte)((lba >> 8) & 0xff));   // Send the middle byte of the LBA
    port_ByteOut(ATA_PRIMARY_PORT + 5, (byte)((lba >> 16) & 0xff));  // Send the high byte of the LBA
    port_ByteOut(ATA_PRIMARY_PORT + 7, ATA_READ);                    // Send the read command

    // Wait for the disk to be ready
    fs_ATAWait(WAITFOR_DATAREADY);

    // Read 512 bytes from the data port
    word i;
    for(i=0;i<256;i++) ((word*)buffer)[i] = port_WordIn(ATA_PRIMARY_PORT);
}

void fs_ATAWriteSector(dWord lba, byte* buffer) {
    port_ByteOut(ATA_PRIMARY_PORT + 6, 0xe0 | ((lba >> 24) & 0x0f)); // Select the drive and LBA
    port_ByteOut(ATA_PRIMARY_PORT + 2, 1);                           // Select the number of sectors to write
    port_ByteOut(ATA_PRIMARY_PORT + 3, (byte)(lba & 0xff));          // Send the low byte of the LBA
    port_ByteOut(ATA_PRIMARY_PORT + 4, (byte)((lba >> 8) & 0xff));   // Send the middle byte of the LBA
    port_ByteOut(ATA_PRIMARY_PORT + 5, (byte)((lba >> 16) & 0xff));  // Send the high byte of the LBA
    port_ByteOut(ATA_PRIMARY_PORT + 7, ATA_WRITE);                   // Send the write command

    // Wait for the disk to be ready
    fs_ATAWait(WAITFOR_DATAREADY);

    // Write 512 bytes to the data port
    word i;
    for(i=0;i<256;i++) port_WordOut(ATA_PRIMARY_PORT, ((word*)buffer)[i]);

    // Flush the cache
    fs_ATAFlush();
}