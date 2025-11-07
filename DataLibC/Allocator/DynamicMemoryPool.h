//
// Created by 18030 on 2025/3/8.
//

#ifndef DATALIBC_DYNAMICMEMORYPOOL_H
#define DATALIBC_DYNAMICMEMORYPOOL_H

#include <stddef.h>
#include "DataLibC/Tool/ExportModuleMacro.h"

/// Provide a dynamic element size memory pool.
typedef struct DynamicMemoryPool DynamicMemoryPool;

API_MODULE DynamicMemoryPool* CreateDynamicMemoryPool();

API_MODULE void* DynamicMemoryPoolAllocate(DynamicMemoryPool* pool, size_t allocateSize);

API_MODULE void DynamicMemoryPoolDeallocate(DynamicMemoryPool* pool, void* ptr);

API_MODULE void ReleaseDynamicMemoryPool(DynamicMemoryPool* pool);

#endif //DATALIBC_DYNAMICMEMORYPOOL_H
