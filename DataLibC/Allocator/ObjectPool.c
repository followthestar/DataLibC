//
// Created by 18030 on 2025/3/22.
//

#include <malloc.h>
#include <pthread.h>
#include <unistd.h>
#include "ObjectPool.h"
#include "DataLibC/DS/Queue/Queue.h"
#include "DataLibC/Thread/Tread.h"
#include "DataLibC/Tool/LogMacro.h"


typedef struct ObjectPool
{
    void* origin;
    CopyConstructor cctor;
    DefaultConstructor ctor;
    ObjectDestructor dtor;
    ResetObject reset;

    Queue pool;
    bool isPrototype;
    bool isThresholdMode;
    size_t threshold;
    size_t allocateAmount;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    pthread_t thread;
    bool isStop;
} ObjectPool;


static void* AllocateOnce(ObjectPool* pool)
{
    if (pool->isPrototype)
        return pool->cctor(pool->origin);
    return pool->ctor();
}

static void* AsyncAllocate(void* arg)
{
    ObjectPool* this = arg;
    while (!this->isStop)
    {
        pthread_mutex_lock(&this->mutex);
        if (this->pool.length < this->threshold)
        {
            pthread_mutex_unlock(&this->mutex);
            for (int i = 0; i < this->allocateAmount; ++i)
            {
                void* obj = AllocateOnce(this);
                pthread_mutex_lock(&this->mutex);
                Queue_Enqueue(&this->pool, &obj);
                pthread_cond_signal(&this->cond);
                pthread_mutex_unlock(&this->mutex);
            }
        }
        else
            pthread_mutex_unlock(&this->mutex);
        Thread_Sleep(1);
    }
    return NULL;
}

ObjectPool*
CreateObjectPool(void* origin, CopyConstructor cctor, DefaultConstructor ctor, ResetObject reset, ObjectDestructor dtor)
{
    if (ctor == NULL && (origin == NULL || cctor == NULL))
    {
        RuntimeErrorLog(InvalidArgument, "Origin and cctor are needed when choosing Copy the prototype object! Or ctor"
                        " is needed when choosing default construction!");
        return NULL;
    }

    if (reset == NULL || dtor == NULL)
    {
        RuntimeErrorLog(InvalidArgument, "Reset and Dstructor is needed!");
        return NULL;
    }

    ObjectPool* pool = malloc(sizeof(ObjectPool));

    pthread_mutex_init(&pool->mutex, NULL);
    pthread_cond_init(&pool->cond, NULL);
    if (!pool)
        return NULL;
    if (ctor)
        pool->isPrototype = false;
    else
        pool->isPrototype = true;
    pool->origin = origin;
    pool->cctor = cctor;
    pool->ctor = ctor;
    pool->reset = reset;
    pool->dtor = dtor;
    pool->isStop = true;
    pool->isThresholdMode = false;
    Queue_Init(&pool->pool, kPOINTER, NULL, NULL);


    return pool;
}

void* AcquireObject(ObjectPool* pool)
{
    pthread_mutex_lock(&pool->mutex);

    while (Queue_Empty(&pool->pool))
    {
        if (pool->isThresholdMode)
            pthread_cond_wait(&pool->cond, &pool->mutex);
        else
        {
            pthread_mutex_unlock(&pool->mutex);
            return AllocateOnce(pool);
        }
    }

    void* obj = *(void**)Queue_Front(&pool->pool);
    Queue_Dequeue(&pool->pool);
    pthread_mutex_unlock(&pool->mutex);
    return obj;
}

void EnableObjectPoolThreshold(ObjectPool* pool, size_t threshold, size_t allocateAmount)
{
    if (threshold == 0 || allocateAmount <= 1)
    {
        bool lastState = pool->isThresholdMode;
        pool->isThresholdMode = false;
        pthread_cond_broadcast(&pool->cond);
        pool->isStop = true;
        if (lastState)
            pthread_join(pool->thread, NULL);

        return;
    }

    pool->isThresholdMode = true;
    pool->threshold = threshold;
    pool->allocateAmount = allocateAmount;
    pool->isStop = false;

    pthread_create(&pool->thread, NULL, AsyncAllocate, pool);
}

void ReleaseObject(ObjectPool* pool, void* object)
{
    pthread_mutex_lock(&pool->mutex);
    pool->reset(object);
    Queue_Enqueue(&pool->pool, &object);
    pthread_mutex_unlock(&pool->mutex);
}

void DestroyObjectPool(ObjectPool* pool)
{
    pool->isStop = true;
    if (pool->isThresholdMode)
        pthread_join(pool->thread, NULL);
    while (!Queue_Empty(&pool->pool))
    {
        void* obj = *(void**)Queue_Front(&pool->pool);
        pool->dtor(obj);
        Queue_Dequeue(&pool->pool);
    }
    pthread_mutex_destroy(&pool->mutex);
    pthread_cond_destroy(&pool->cond);
    free(pool);
}
