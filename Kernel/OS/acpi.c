#include "port.h"
#include "memory.h"
#include "video.h"
#include "acpi.h"

#define SLEEP_ENABLE (1 << 13)

word pm1aControlBlock = 0;
word sleepTypes[5] = {1, 2, 3, 4, 5};

void acpi_Init(void) {
    RSDPDescriptor* rsdp = NULL;
    dWord address;
    for(address=0xE0000;address<0x100000;address+=16) {
        if(memory_Equal((const void*)address, "RSD PTR ", 8)) {
            rsdp = (RSDPDescriptor*)address;
            break;
        }
    }

    if(!rsdp) return;

    ACPIHeader* rsdt = (ACPIHeader*)rsdp->rsdtAddress;
    dWord entries = (rsdt->length - sizeof(ACPIHeader)) / 4, i;
    dWord* entry = (dWord*)((dWord)rsdt + sizeof(ACPIHeader));

    for(i=0;i<entries;i++) {
        ACPIHeader* header = (ACPIHeader*)entry[i];
        if(memory_Equal(header->signature, "FACP", 4)) {
            FADT* fadt = (FADT*)header;
            pm1aControlBlock = (word)(fadt->pm1aCntBlk);

            // Parse the DSDT for sleep types
            ACPIHeader* dsdt = (ACPIHeader*)fadt->dsdt;
            byte* dsdtData = (byte*)dsdt;
            dWord j;
            byte sleepTypeInit[5] = {0};
            for(j=0;j<dsdt->length-10;j++) {
                if(memory_Equal((const void*)(dsdtData+j), "_S1_", 4)) {
                    sleepTypes[0] = (word)(*(byte*)(dsdtData+j+9) & 0x07);
                    sleepTypeInit[0] = 1;
                }
                if(memory_Equal((const void*)(dsdtData+j), "_S3_", 4)) {
                    sleepTypes[2] = (word)(*(byte*)(dsdtData+j+9) & 0x07);
                    sleepTypeInit[2] = 1;
                }
                if(memory_Equal((const void*)(dsdtData+j), "_S4_", 4)) {
                    sleepTypes[3] = (word)(*(byte*)(dsdtData+j+9) & 0x07);
                    sleepTypeInit[3] = 1;
                }
                if(memory_Equal((const void*)(dsdtData+j), "_S5_", 4)) {
                    sleepTypes[4] = (word)(*(byte*)(dsdtData+j+9) & 0x07);
                    sleepTypeInit[4] = 1;
                }
            }

            // Tell the user if any of the sleep types weren't found
            for(j=0;j<5;j++) {
                // S2 is rarely used, so don't even care about it
                if(j == 1) continue;

                if(!sleepTypeInit[j]) {
                    video_Print("[ACPI ");
                    switch(j) {
                        case 0: video_Print("Standby");   break;
                        case 2: video_Print("Suspend");   break;
                        case 3: video_Print("Hibernate"); break;
                        case 4: video_Print("Shutdown");  break;
                    }
                    video_Print(" Code Not Found]\n");
                }
            }

            video_Print("[ACPI Initialized]\n");
            return;
        }
    }
}

void acpi_Sleep(ACPISleepSignal sleepSignal) {
    if(!pm1aControlBlock) return;

    if(sleepSignal > 4) return;
    word sleepType = sleepTypes[sleepSignal];

    // Send the signal
    port_WordOut(pm1aControlBlock, sleepType | SLEEP_ENABLE);
}