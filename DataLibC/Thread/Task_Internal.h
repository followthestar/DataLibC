//
// Created by 18030 on 2025/9/27.
//

#ifndef TASK_INTERNAL_H
#define TASK_INTERNAL_H

#include <pthread.h>

#include "Task.h"

typedef enum TaskState
{
    Ready,
    Running,
    Done
} TaskState;

typedef struct Task
{
    TaskFunc task;
    void* args;
    TaskState state;
    pthread_t thread;
    pthread_mutex_t mutex;
    pthread_cond_t done;
    void* result;
} Task;

void Task_Build(Task* task, TaskFunc taskFunc, void* args);

void Task_Finalizer(void* task);

void* PackagedTask(void* args);

#endif //TASK_INTERNAL_H
