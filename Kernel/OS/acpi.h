#ifndef OS_ACPI_H
#define OS_ACPI_H

#include "types.h"

typedef struct {
    char signature[8];
    byte checksum;
    char oemId[6];
    byte revision;
    dWord rsdtAddress;
} __attribute__((packed)) RSDPDescriptor;

typedef struct {
    char signature[4];
    dWord length;
    byte revision;
    byte checksum;
    char oemId[6];
    char oemTableId[8];
    dWord oemRevision;
    dWord creatorId;
    dWord creatorRevision;
} __attribute__((packed)) ACPIHeader;

typedef struct {
    ACPIHeader header;
    
    dWord firmwareCtrl;      // Address of FACS table
    dWord dsdt;              // Address of DSDT table 
    
    byte reserved;

    byte preferredPmProfile; // System type
    word sciInt;             // SCI number
    dWord smiCmd;            // SMI command port
    byte acpiEnable;         // Value to write to smiCmd to enable ACPI
    byte acpiDisable;        // Value to disable ACPI
    byte s4BiosReq;          // Value to request S4 BIOS sleep
    byte pstateCnt;          // Processor performance state control

    dWord pm1aEvtBlk;        // PM1a Event Register Block
    dWord pm1bEvtBlk;        // PM1b Event Register Block
    dWord pm1aCntBlk;        // PM1a Control Register Block 
    dWord pm1bCntBlk;        // PM1b Control Register Block

    dWord pm2CntBlk;         // PM2 Control Register Block
    dWord pmTmrBlk;          // Power Management Timer Block
    dWord gpe0Blk;           // General-Purpose Event 0 Register Block
    dWord gpe1Blk;           // General-Purpose Event 1 Register Block

    byte pm1EvtLen;          // Length of PM1 Event Registers
    byte pm1CntLen;          // Length of PM1 Control Registers
    byte pm2CntLen;          // Length of PM2 Control Registers
    byte pmTmrLen;           // Length of PM Timer
    byte gpe0BlkLen;         // Length of GPE0 Block
    byte gpe1BlkLen;         // Length of GPE1 Block

    byte gpe1Base;           // Starting index for GPE1

    byte cstCnt;             // C-State control support

    word pLvl2Lat;           // C2 state latency (in microseconds)
    word pLvl3Lat;           // C3 state latency (in microseconds)

    word flushSize;     
    
    // CPU cache flush size
    word flushStride;        // CPU cache flush stride

    byte dutyOffset;         // Processor duty cycle offset
    byte dutyWidth;          // Processor duty cycle width

    byte dayAlrm;            // RTC day alarm index
    byte monAlrm;            // RTC month alarm index
    byte century;            // RTC century index (for full date info)

    word iapcBootArch;       // Boot architecture flags

    byte reserved2;
    dWord flags;             // Features flags (ACPI features supported)
} __attribute__((packed)) FADT;

typedef enum {
    ACPI_SLEEP_STANDBY   = 0, // S1
    ACPI_SLEEP_SUSPEND   = 2, // S3
    ACPI_SLEEP_HIBERNATE = 3, // S4
    ACPI_SLEEP_SHUTDOWN  = 4  // S5
} ACPISleepSignal;

/**
* \brief Initializes ACPI
*/
void acpi_Init(void);

/**
* \brief Sends an \b ACPISleepSignal
* \param sleepSignal The \b ACPISleepSignal to send
*/
void acpi_Sleep(ACPISleepSignal sleepSignal);

#endif