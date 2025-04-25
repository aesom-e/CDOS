#include "../OS/types.h"
#include "../OS/memory.h"
#include "ata.h"
#include "disk.h"
#include "cluster.h"

dWord fs_ClusterToSector(dWord cluster) {
    return Disk.bootSector.dataStartSector + ((cluster - 2) * Disk.bootSector.sectorsPerCluster);
}

dWord fs_GetNextCluster(dWord currentCluster) {
    dWord FATSector = Disk.bootSector.FATStartSector + ((currentCluster * 4) / Disk.bootSector.bytesPerSector);
    byte* FATBuffer = memory_Callocate(Disk.bootSector.bytesPerSector);
    fs_ATAReadSector(FATSector, FATBuffer);

    dWord offset = (currentCluster * 4) % Disk.bootSector.bytesPerSector;
    dWord nextCluster = *(dWord*)(FATBuffer + offset) & 0x0fffffff;

    memory_Free(FATBuffer);
    return nextCluster;
}

dWord fs_CreateCluster(void) {
    dWord totalClusters = (Disk.bootSector.sectorsPerFAT*Disk.bootSector.numFATs - Disk.bootSector.dataStartSector);
    byte* FATBuffer = memory_Callocate(Disk.bootSector.bytesPerSector);

    dWord cluster;
    for(cluster=2;cluster<totalClusters;cluster++) {
        dWord FATSector = Disk.bootSector.FATStartSector + ((cluster * 4) / Disk.bootSector.bytesPerSector);
        dWord offset = (cluster * 4) % Disk.bootSector.bytesPerSector;
        fs_ATAReadSector(FATSector, FATBuffer);

        // Check for a free cluster
        dWord clusterValue = *(dWord*)(FATBuffer + offset) & 0x0fffffff;
        if(!clusterValue) {
            // Mark this cluster as the end of the chain
            *(dWord*)(FATBuffer + offset) = 0x0fffffff;
            fs_ATAWriteSector(FATSector, FATBuffer);
            memory_Free(FATBuffer);
            return cluster;
        }
    }

    memory_Free(FATBuffer);
    return 0;
}

void fs_DeleteCluster(dWord cluster) {
    byte* FATBuffer = memory_Callocate(Disk.bootSector.bytesPerSector);

    while(cluster>=2 && cluster<0x0ffffff8) {
        dWord FATSector = Disk.bootSector.FATStartSector + ((cluster * 4) / Disk.bootSector.bytesPerSector);
        dWord offset = (cluster * 4) % Disk.bootSector.bytesPerSector;
        fs_ATAReadSector(FATSector, FATBuffer);

        dWord nextCluster = *(dWord*)(FATBuffer + offset) & 0x0fffffff;

        // Free the current cluster
        *(dWord*)(FATBuffer+offset) = 0;
        fs_ATAWriteSector(FATSector, FATBuffer);

        // Move to the next cluster in the chain
        cluster = nextCluster;
    }

    memory_Free(FATBuffer);
}