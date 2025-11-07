//
// Created by 18030 on 2025/9/24.
//

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "Task.h"
#include "DataLibC/Tool/ExportModuleMacro.h"


///  A ThreadPool object manages the threads used to execute asynchronized tasks.
typedef struct ThreadPool ThreadPool;


/**
 * @brief Create a new thread pool
 * @param threadCount The max number of threads in the thread pool
 * @return The new thread pool object
 */
API_MODULE ThreadPool* ThreadPool_Create(int threadCount);

/**
 * @brief Submit a task to the thread pool
 * @param pool The thread pool
 * @param taskFunc The task function to be called
 * @param args The arguments or context passed to the task
 * @return The task object. Use Task_Get() to wait for task completion to return result
 */
API_MODULE Task* ThreadPool_Submit(const ThreadPool* pool, TaskFunc taskFunc, void* args);

/**
 * @brief Get the number of left tasks in the thread pool
 * @param pool The thread pool
 * @return The number of left tasks in the thread pool
 */
API_MODULE int ThreadPool_TaskAmount(const ThreadPool* pool);

/**
 * @brief Destroy the thread pool
 * @param pool The thread pool
 * @attention This function destroys all the threads in the pool and waits for all the tasks to be finished.
 */
API_MODULE void ThreadPool_Destroy(const ThreadPool* pool);

#endif //THREADPOOL_H
