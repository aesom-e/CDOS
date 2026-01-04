// Private
#ifndef FS_CLUSTER_H
#define FS_CLUSTER_H

#include "../OS/types.h"

/**
* \brief Converts the specified cluster into a sector
* \param cluster The cluster to convert
* \return The LBA of the converted sector
*/
dWord fs_ClusterToSector(dWord cluster);

/**
* \brief Gets the cluster that follows the input cluster
* \param currentCluster The input cluster
* \return The cluster that follows \b currentCluster
*/
dWord fs_GetNextCluster(dWord currentCluster);

/**
* \brief Creates a new cluster on disk
* \return The LBA of the new cluster
*/
dWord fs_CreateCluster(void);

/**
* \brief Deletes the specified cluster off the disk
* \param cluster The LBA of the cluster to delete
*/
void fs_DeleteCluster(dWord cluster);

#endif