//
// Created by 18030 on 2025/9/24.

#ifndef GC_H
#define GC_H

#include "DataLibC/DS/TypeEnum.h"
#include "DataLibC/Tool/ExportModuleMacro.h"

#ifdef ENABLE_DEFAULT_GC_MALLOC
#define malloc(size) GC_Malloc(size)
#define free(ptr) GC_Free(ptr)
#endif

#define GC_AUTO_INIT() \
do { \
extern char __data_start__[]; \
extern char __data_end__[]; \
extern char __bss_start__[];\
extern char __bss_end__[];\
void* gc_stack_bottom;\
__asm__ volatile ("":"=r"(gc_stack_bottom) : "0"(__builtin_frame_address(0)));\
GC_Init((void*)__data_start__, (void*)__data_end__, (void*)__bss_start__, (void*)__bss_end__, gc_stack_bottom);\
} while(0)

/**
 * @brief A Finalizer is used to dispose the resources that are not used anymore.
 * @param handle The handle pointer of resource or context
 */
typedef void (* Finalizer)(void* handle);

/**
 * @brief Call this function before any GC operation.
 */
API_MODULE

void GC_Init(void* dataStart, void* dataEnd, void* bssStart, void* bssEnd, void* frameAddres);

/**
 * @brief Allocate memory managed by garbage collection.
 * @param size The size of memory to allocate.
 * @return The pointer to the allocated memory.
 * @attention Normally, you don't need to free the memory allocated from this function unless you really want to do that.
 */
API_MODULE void* GC_Malloc(size_t size);

/**
 * @brief Allocate memory managed by garbage collection with a finalizer.
 * @param size The size of memory to allocate.
 * @param finalizer If the finalizer is not nullptr, it will be called when the memory is collected by GC.
 * @return The pointer to the allocated memory.
 * @attention Normally, you don't need to free the memory allocated from this function unless you really want to do that.
 */
API_MODULE void* GC_Malloc_Ext(size_t size, Finalizer finalizer);

/**
 * @brief Free memory allocated from GC_Malloc() manually.
 * @param ptr The pointer to the memory to free.
 */
API_MODULE

void GC_Free(void* ptr);

/**
 * @brief Try to collect garbage if GC is available, and then free the memory.
 */
API_MODULE void GC_Collect();

/**
 * @brief Force to collect garbage and free the memory immediately.
 */
API_MODULE void GC_ForceCollect();

/**
 * @brief Call this function to clean up GC resources.
 */
API_MODULE void GC_Cleanup();

#endif //GC_H
