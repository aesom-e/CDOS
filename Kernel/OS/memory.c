#include "../Constants/OSConstants.h"
#include "memory.h"

typedef struct memoryNode {
    dWord size;
    byte used;
    struct memoryNode* previous;
    struct memoryNode* next;
} MemoryNode;

MemoryNode* memoryStart;

void memory_Copy(byte* source, byte* destination, int num) {
    int i;
    for(i=0;i<num;i++) {
        *(destination+i) = *(source+i);
    }
}

void memory_Init(void) {
    memoryStart = (MemoryNode*)HEAP_LOCATION;
    memoryStart->size = HEAP_SIZE - sizeof(MemoryNode);
    memoryStart->next = NULL;
    memoryStart->previous = NULL;
}

MemoryNode* memory_FindBestBlockPos(dWord size) {
    MemoryNode* bestNode = NULL;
    dWord bestBlockSize = HEAP_SIZE + 1;

    MemoryNode* currentNode = memoryStart;
    while(currentNode) {
        // Check if the current block can be used and is smaller than the current best
        if(!currentNode->used
           && currentNode->size >= (size + sizeof(MemoryNode))
           && currentNode->size <= bestBlockSize) {
            bestNode = currentNode;
            bestBlockSize = currentNode->size;
        }

        currentNode = currentNode->next;
    }
    return bestNode;
}

void* memory_Allocate(dWord size) {
    MemoryNode* bestNode = memory_FindBestBlockPos(size);
    if(!bestNode) return NULL;

    // Check if we need to split the block
    dWord remainingSize = bestNode->size - size - sizeof(MemoryNode);
    if(remainingSize >= sizeof(MemoryNode)) {
        // Create a new block for the remaining memory
        MemoryNode* newNode = (MemoryNode*)((byte*)bestNode + sizeof(MemoryNode) + size);
        newNode->size = remainingSize;
        newNode->used = 0;
        newNode->next = bestNode->next;
        newNode->previous = bestNode;

        if(bestNode->next) bestNode->next->previous = newNode;
        bestNode->next = newNode;
        bestNode->size = size;
    }
    bestNode->used = 1;

    return (void*)((byte*)bestNode + sizeof(MemoryNode));
}

void* memory_Callocate(dWord size) {
    byte* ret = memory_Allocate(size);
    if(!ret) return NULL;

    dWord i;
    for(i=0;i<size;i++) ret[i] = 0;
    return ret;
}

MemoryNode* memory_Merge(MemoryNode* currentNode, byte left) {
    MemoryNode* neighbourNode = left ? currentNode->next : currentNode->previous;

    if(neighbourNode && !neighbourNode->used) {
        if(left) {
            currentNode->size += neighbourNode->size + sizeof(MemoryNode);
            currentNode->next = neighbourNode->next;
            if(currentNode->next) currentNode->next->previous = neighbourNode;
        } else {
            neighbourNode->size += currentNode->size + sizeof(MemoryNode);
            neighbourNode->next = currentNode->next;
            if(currentNode->next) currentNode->next->previous = neighbourNode;
            currentNode = neighbourNode;
        }
    }

    return currentNode;
}

void __memory_Free(void* memory) {
    if(!memory) return;

    MemoryNode* currentNode = (MemoryNode*)((byte*)memory - sizeof(MemoryNode));
    if(!currentNode) return;

    currentNode->used = 0;
    currentNode = memory_Merge(currentNode, 1);
    memory_Merge(currentNode, 0);
}