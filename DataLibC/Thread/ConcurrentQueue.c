//
// Created by 18030 on 2025/9/24.
//

#include "ConcurrentQueue.h"
#include "DataLibC/DS/Queue/Queue.h"
#include <pthread.h>

#include "DataLibC/Tool/LogMacro.h"

typedef struct ConcurrentQueue
{
    Queue m_TaskQueue;
    pthread_cond_t m_NotEmpty;
    pthread_mutex_t m_Mutex;

    bool m_IsBlock;
    bool m_IsStop;
} ConcurrentQueue;

static void* TaskPairCtor(const void* value)
{
    TaskPair* p = malloc(sizeof(TaskPair));
    *p = *(TaskPair*)value;
    return p;
}

ConcurrentQueue* ConcurrentQueue_Create(const bool isBlocking)
{
    ConcurrentQueue* queue = malloc(sizeof(ConcurrentQueue));
    if (queue == nullptr)
    {
        RuntimeErrorLog(OutOfMemory, "Failed to create ConcurrentQueue!");
        return nullptr;
    }

    queue->m_IsBlock = isBlocking;
    queue->m_IsStop = false;
    Queue_Init(&queue->m_TaskQueue, kCUSTOM, TaskPairCtor, nullptr);

    if (pthread_mutex_init(&queue->m_Mutex, nullptr) != 0)
    {
        free(queue);
        RuntimeErrorLog(OutOfMemory, "Failed to init mutex for ConcurrentQueue!");
        return nullptr;
    }

    if (pthread_cond_init(&queue->m_NotEmpty, nullptr) != 0)
    {
        pthread_mutex_destroy(&queue->m_Mutex);
        free(queue);
        RuntimeErrorLog(OutOfMemory, "Failed to init condition variable for ConcurrentQueue!");
        return nullptr;
    }

    return queue;
}

void ConcurrentQueue_PushBack(ConcurrentQueue* queue, const TaskFunc task, void* args)
{
    pthread_mutex_lock(&queue->m_Mutex);
    TaskPair pair = { task, args };
    Queue_Enqueue(&queue->m_TaskQueue, &pair);
    pthread_cond_signal(&queue->m_NotEmpty);
    pthread_mutex_unlock(&queue->m_Mutex);
}

bool ConcurrentQueue_Front(ConcurrentQueue* queue, TaskPair* outPair)
{
    if (queue->m_IsBlock)
        pthread_mutex_lock(&queue->m_Mutex);
    else
    {
        if (pthread_mutex_trylock(&queue->m_Mutex) != 0)
            return false;
    }

    while (Queue_Empty(&queue->m_TaskQueue) && !queue->m_IsStop)
    {
        if (!queue->m_IsBlock)
        {
            pthread_mutex_unlock(&queue->m_Mutex);
            return false;
        }
        pthread_cond_wait(&queue->m_NotEmpty, &queue->m_Mutex);
    }

    if (Queue_Empty(&queue->m_TaskQueue))
    {
        pthread_mutex_unlock(&queue->m_Mutex);
        return false;
    }
    *outPair = *(TaskPair*)Queue_Front(&queue->m_TaskQueue);
    Queue_Dequeue(&queue->m_TaskQueue);
    pthread_mutex_unlock(&queue->m_Mutex);
    return true;
}

void ConcurrentQueue_Stop(ConcurrentQueue* queue)
{
    pthread_mutex_lock(&queue->m_Mutex);
    queue->m_IsStop = true;
    pthread_cond_broadcast(&queue->m_NotEmpty);
    pthread_mutex_unlock(&queue->m_Mutex);
}

int ConcurretQueue_Size(ConcurrentQueue* queue)
{
    pthread_mutex_lock(&queue->m_Mutex);
    const int size = (int)queue->m_TaskQueue.length;
    pthread_mutex_unlock(&queue->m_Mutex);
    return size;
}

bool ConcurrentQueue_Empty(ConcurrentQueue* queue)
{
    return ConcurretQueue_Size(queue) == 0;
}

void ConcurrentQueue_Destroy(ConcurrentQueue* queue)
{
    pthread_cond_destroy(&queue->m_NotEmpty);
    pthread_mutex_destroy(&queue->m_Mutex);
    Queue_Clear(&queue->m_TaskQueue);
    free(queue);
}
