//
// Created by 18030 on 2025/9/24.
//

#include "GC.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "DataLibC/DS/Queue/Queue.h"
#include "DataLibC/IO/Print.h"
#include "DataLibC/Tool/LogMacro.h"

#define MIN_GC_COLLECT_LIMIT 15


typedef struct GC_Block
{
    size_t size;
    bool marked;
    Finalizer finalizer;
    struct GC_Block* next;
} GC_Block;

typedef struct GC_VM
{
    bool m_HasInit;

    void* m_DataStart;
    void* m_DataEnd;
    void* m_BssStart;
    void* m_BssEnd;
    void* m_StackBottom;

    GC_Block* m_HeapHead;

    int m_CollectCount;
    bool m_IsCollecting;
    int m_AllocateCount;
    int m_ActiveObject;

    //TODO
    time_t m_LastCollectTime;

    Queue m_HeapTraceQueue;
} GC_VM;

static GC_VM* s_VM = nullptr;
#define VM (s_VM)

bool EnsureGCSystemInitInternal() { return VM != nullptr && VM->m_HasInit; }

static GC_Block* GC_FindBlock(const void* ptr)
{
    GC_Block* heap = VM->m_HeapHead;
    while (heap != nullptr)
    {
        const char* obj_start = (char*)heap + sizeof(GC_Block);
        const char* obj_end = obj_start + heap->size;
        if (ptr >= (void*)obj_start && ptr < (void*)obj_end)
        {
            if (!heap->marked)
                return heap;
            return nullptr;
        }
        heap = heap->next;
    }
    return nullptr;
}

static void GC_MarkSegment(void* start, const void* end)
{
    void** iter = start;
    while (iter < (void**)end)
    {
        const void* potentialPtr = *iter;
        if (potentialPtr == nullptr)
        {
            iter++;
            continue;
        }

        GC_Block* heap = GC_FindBlock(potentialPtr);
        if (heap != nullptr && !heap->marked)
            Queue_Enqueue(&VM->m_HeapTraceQueue, &heap);
        iter++;
    }
}

static void GC_MarkStackFrame()
{
    void* current = __builtin_frame_address(0);
    void* bottom = VM->m_StackBottom;
    void* start = current < bottom ? current : bottom;
    void* end = current > bottom ? current : bottom;

    GC_MarkSegment(start, end);
}

static void GC_MarkAll()
{
    GC_MarkSegment(VM->m_DataStart, VM->m_DataEnd);
    GC_MarkSegment(VM->m_BssStart, VM->m_BssEnd);
    GC_MarkStackFrame();
    while (!Queue_Empty(&VM->m_HeapTraceQueue))
    {
        void** ptr = Queue_Front(&VM->m_HeapTraceQueue);
        Queue_Dequeue(&VM->m_HeapTraceQueue);
        GC_Block* heap = *(GC_Block**)ptr;
        if (heap->marked)
            continue;
        heap->marked = true;
        void* start = (char*)heap + sizeof(GC_Block);
        const void* end = (char*)heap + sizeof(GC_Block) + heap->size;
        GC_MarkSegment(start, end);
    }
    assert(Queue_Empty(&VM->m_HeapTraceQueue));
}

static int GC_Sweep()
{
    GC_Block* block = VM->m_HeapHead;
    GC_Block* prev = nullptr;
    int freeCount = 0;
    while (block != nullptr)
    {
        if (block->marked)
        {
            block->marked = false;
            prev = block;
            block = block->next;
        }
        else
        {
            GC_Block* to_free = block;
            block = block->next;
            if (prev == nullptr)
                VM->m_HeapHead = block;
            else
                prev->next = block;

            if (to_free->finalizer != nullptr)
                to_free->finalizer((char*)to_free + +sizeof(GC_Block));

            free(to_free);
            freeCount++;
        }
    }
    return freeCount;
}

static void GC_CollectInternal()
{
    GC_MarkAll();
    int freeCount = GC_Sweep();
    VM->m_CollectCount++;
    VM->m_ActiveObject -= freeCount;
    print_fmt("[GC] {} garbage items are being freed!", freeCount);
}

void GC_Init(void* dataStart, void* dataEnd, void* bssStart, void* bssEnd, void* frameAddress)
{
    if (VM != nullptr)
        return;
    VM = (GC_VM*)malloc(sizeof(GC_VM));
    VM->m_HasInit = true;
    VM->m_CollectCount = 0;
    VM->m_IsCollecting = false;
    VM->m_DataStart = dataStart;
    VM->m_DataEnd = dataEnd;
    VM->m_BssStart = bssStart;
    VM->m_BssEnd = bssEnd;
    VM->m_ActiveObject = 0;
    VM->m_AllocateCount = 0;
    VM->m_LastCollectTime = time(nullptr);
    VM->m_HeapHead = nullptr;
    VM->m_StackBottom = frameAddress;
    Queue_Init(&VM->m_HeapTraceQueue, kPOINTER, nullptr, nullptr);


    atexit(GC_Cleanup);
}

void* GC_Malloc(const size_t size)
{
    return GC_Malloc_Ext(size, nullptr);
}

void* GC_Malloc_Ext(const size_t size, const Finalizer finalizer)
{
    const size_t totalSize = size + sizeof(GC_Block);
    GC_Block* block = malloc(totalSize);
    if (block == nullptr)
    {
        GC_ForceCollect();
        block = malloc(totalSize);
        if (block == nullptr)
        {
            RuntimeErrorLog(OutOfMemory, "Failed to allocate new memery before the memory has run out!");
            return nullptr;
        }
    }
    block->size = size;
    block->marked = false;
    block->next = VM->m_HeapHead;
    block->finalizer = finalizer;
    VM->m_HeapHead = block;
    VM->m_AllocateCount++;
    VM->m_ActiveObject++;
    return (char*)block + sizeof(GC_Block);
}

void GC_Collect()
{
    if (VM == nullptr || !VM->m_HasInit)
    {
        RuntimeErrorLog(IllegalState, "You need to initialize GC before calling this!");
        return;
    }

    if (VM->m_IsCollecting)
    {
        RuntimeErrorLog(IllegalState, "Don't call GC_Collect() in multiple threads!");
        return;
    }

    if (VM->m_ActiveObject < MIN_GC_COLLECT_LIMIT)
        return;

    const time_t currTime = time(nullptr);
    if (currTime - VM->m_LastCollectTime < 1)
        return;
    VM->m_LastCollectTime = currTime;
    VM->m_IsCollecting = true;
    GC_CollectInternal();
    VM->m_IsCollecting = false;
}

void GC_ForceCollect()
{
    if (VM == nullptr || !VM->m_HasInit)
    {
        RuntimeErrorLog(IllegalState, "You need to initialize GC before calling this!");
        return;
    }

    if (VM->m_IsCollecting)
    {
        RuntimeErrorLog(IllegalState, "Don't call GC_Collect() in multiple threads!");
        return;
    }

    if (VM->m_ActiveObject == 0)
        return;

    VM->m_LastCollectTime = time(nullptr);
    VM->m_IsCollecting = true;
    GC_CollectInternal();
    VM->m_IsCollecting = false;
}

static void GC_CleanAllObject()
{
    int cleanAmount = 0;
    GC_Block* iter = VM->m_HeapHead;
    while (iter != nullptr)
    {
        GC_Block* block = iter;
        iter = iter->next;
        if (block->finalizer != nullptr)
            block->finalizer((char*)block + sizeof(GC_Block));
        free(block);
        cleanAmount++;
    }
    assert(cleanAmount == VM->m_ActiveObject);
    print_fmt("[GC] {} garbage items were freed in final gc!", cleanAmount);
}

void GC_Cleanup()
{
    if (VM == nullptr)
        return;
    GC_CleanAllObject();

    free(VM);
}
