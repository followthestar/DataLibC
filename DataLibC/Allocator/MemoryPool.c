//
// Created by 18030 on 2025/3/8.
//

#include <malloc.h>
#include <memory.h>
#include "MemoryPool.h"

typedef unsigned char byte;
constexpr unsigned short OBJECT_AMOUNT_IN_BLOCK = 16;

typedef struct MemoryBlock MemoryBlock;

typedef struct MemoryBlock
{
    MemoryBlock* next;
    void* freeNode;
    void* endData;
    byte data[];
} MemoryBlock;

typedef struct MemoryPool
{
    size_t objectSize;
    size_t blockSize;
    size_t blockCount;

    MemoryBlock* header;
    void* freeNode;
} MemoryPool;

static void AllocateNewBlock(MemoryPool* pool)
{
    size_t blockSize = pool->blockSize + sizeof(MemoryBlock);
    MemoryBlock* block = malloc(blockSize);
    memset(block, 0, blockSize);
    block->next = pool->header;
    block->endData = block->data + pool->blockSize;
    block->freeNode = block->data;
    pool->header = block;
    pool->blockCount++;
}

MemoryPool* CreateMemoryPool(size_t elementSize)
{
    MemoryPool* pool = malloc(sizeof(MemoryPool));
    pool->objectSize = elementSize;
    pool->blockSize = elementSize * OBJECT_AMOUNT_IN_BLOCK;
    AllocateNewBlock(pool);
    pool->freeNode = pool->header->freeNode;
    return pool;
}

void* MemoryPoolAllocate(MemoryPool* pool)
{
    void* ret = pool->freeNode;
    pool->header->freeNode += pool->objectSize;
    if (pool->header->freeNode == pool->header->endData)
    {
        // Current block is full, allocate a new block
        AllocateNewBlock(pool);
    }
    pool->freeNode = pool->header->freeNode;
    return ret;
}

void ReleaseMemoryPool(MemoryPool* pool)
{
    auto iter = pool->header;
    while (iter)
    {
        auto next = iter->next;
        free(iter);
        iter = next;
    }
    free(pool);
}

void MemoryPoolDeallocate(MemoryPool* pool, void* ptr)
{

}
