//
// Created by 18030 on 2025/3/8.
//

#ifndef DATALIBC_MEMORYPOOL_H
#define DATALIBC_MEMORYPOOL_H

#include <stddef.h>
#include "DataLibC/Tool/ExportModuleMacro.h"

/// Provide a fixed element size memory pool.
typedef struct MemoryPool MemoryPool;

/// \brief Create a memory pool.
/// \param elementSize The size of each element.
/// \return A pointer to the memory pool.
API_MODULE MemoryPool* CreateMemoryPool(size_t elementSize);

/// \brief Allocate a memory block from the pool.
/// \param pool The pool to allocate from.
/// \return A pointer to the allocated memory.
API_MODULE void* MemoryPoolAllocate(MemoryPool* pool);

/// \brief Deallocate a memory to the pool.
/// \param pool The pool to deallocate to.
/// \param ptr The memory to deallocate.
API_MODULE void MemoryPoolDeallocate(MemoryPool* pool, void* ptr);

/// \brief Release the memory pool.
/// \param pool The pool to release.
/// \attention After releasing the pool, all the object in this pool will also be released and this wonld not call the
/// destructor for each objet.
API_MODULE void ReleaseMemoryPool(MemoryPool* pool);

#endif //DATALIBC_MEMORYPOOL_H
