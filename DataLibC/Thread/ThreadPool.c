//
// Created by 18030 on 2025/9/24.
//

#include "ThreadPool.h"

#include <pthread.h>

#include "ConcurrentQueue.h"

#define MAX_THREADS 16
#define MIN_THREADS 4

#include "Task_Internal.h"
#include "DataLibC/GC/GC.h"


typedef struct ThreadPool
{
    ConcurrentQueue* m_Tasks;
    pthread_t m_Workers[MAX_THREADS];
    int m_ThreadCount;
} ThreadPool;


static void* Worker(void* args)
{
    const ThreadPool* this = args;
    while (true)
    {
        TaskPair taskPair;
        if (!ConcurrentQueue_Front(this->m_Tasks, &taskPair))
            break;
        taskPair.task(taskPair.args);
    }
    return nullptr;
}

ThreadPool* ThreadPool_Create(int threadCount)
{
    ThreadPool* pool = malloc(sizeof(ThreadPool));
    if (threadCount < MIN_THREADS)
        threadCount = MIN_THREADS;

    if (threadCount > MAX_THREADS)
        threadCount = MAX_THREADS;

    pool->m_ThreadCount = threadCount;

    for (int i = 0; i < threadCount; i++)
        pthread_create(&pool->m_Workers[i], nullptr, Worker, pool);

    pool->m_Tasks = ConcurrentQueue_Create(true);
    return pool;
}

Task* ThreadPool_Submit(const ThreadPool* pool, const TaskFunc taskFunc, void* args)
{
    Task* task = GC_Malloc_Ext(sizeof(Task), Task_Finalizer);
    Task_Build(task, taskFunc, args);
    ConcurrentQueue_PushBack(pool->m_Tasks, PackagedTask, task);
    return task;
}

int ThreadPool_TaskAmount(const ThreadPool* pool)
{
    return ConcurretQueue_Size(pool->m_Tasks);
}

void ThreadPool_Destroy(const ThreadPool* pool)
{
    ConcurrentQueue_Stop(pool->m_Tasks);

    void* r;
    for (int i = 0; i < pool->m_ThreadCount; i++)
    {
        pthread_join(pool->m_Workers[i], &r);
    }
    ConcurrentQueue_Destroy(pool->m_Tasks);
}
