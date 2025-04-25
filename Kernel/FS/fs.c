#include "disk.h"
#include "cluster.h"
#include "header.h"
#include "workingDirectory.h"
#include "fs.h"

void fs_Init(void) {
    fs_ReadDiskHeader();

    __WorkingDirectoryCluster = Disk.bootSector.rootCluster;
}