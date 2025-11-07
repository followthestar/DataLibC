//
// Created by 18030 on 2025/9/27.
//

#include "Task.h"
#include "Task_Internal.h"
#include "DataLibC/GC/GC.h"
#include "DataLibC/IO/Print.h"

void* PackagedTask(void* args)
{
    Task* task = args;

    pthread_mutex_lock(&task->mutex);
    task->state = Running;
    pthread_mutex_unlock(&task->mutex);

    void* result = task->task(task->args);
    pthread_mutex_lock(&task->mutex);
    task->result = result;
    task->state = Done;
    pthread_cond_signal(&task->done);
    pthread_mutex_unlock(&task->mutex);
    return nullptr;
}

void Task_Build(Task* task, const TaskFunc taskFunc, void* args)
{
    task->task = taskFunc;
    task->args = args;
    task->state = Ready;
    task->result = nullptr;
    pthread_mutex_init(&task->mutex, nullptr);
    pthread_cond_init(&task->done, nullptr);
}

void Task_Finalizer(void* task)
{
    Task* t = task;
    pthread_cond_destroy(&t->done);
    pthread_mutex_destroy(&t->mutex);
    print_any("Finalizer was called by task!");
}

Task* Task_Run(const TaskFunc taskFunc, void* args)
{
    Task* task = GC_Malloc_Ext(sizeof(Task), Task_Finalizer);
    Task_Build(task, taskFunc, args);
    if (pthread_create(&task->thread, NULL, PackagedTask, task) != 0)
        return nullptr;

    pthread_detach(task->thread);
    return task;
}

void* Task_Get(Task* task)
{
    pthread_mutex_lock(&task->mutex);

    while (task->state != Done)
    {
        pthread_cond_wait(&task->done, &task->mutex);
    }

    void* result = task->result;
    pthread_mutex_unlock(&task->mutex);
    return result;
}
