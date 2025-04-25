#include "../OS/types.h"

// Private
#ifndef FS_CLUSTER_H
#define FS_CLUSTER_H

dWord fs_ClusterToSector(dWord cluster);

dWord fs_GetNextCluster(dWord currentCluster);

dWord fs_CreateCluster(void);

void fs_DeleteCluster(dWord cluster);

#endif