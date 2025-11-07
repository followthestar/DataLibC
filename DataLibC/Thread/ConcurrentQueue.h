//
// Created by 18030 on 2025/9/24.

#ifndef CONCURRENTQUEUE_H
#define CONCURRENTQUEUE_H

#include "DataLibC/DS/Pair/Pair.h"
#include "DataLibC/Tool/ExportModuleMacro.h"


/**
 * @brief A standard task function call
 * @param args The arguments or context passed to the task
 * @return The result of this task
 */
typedef void* (* TaskFunc)(void* args);

typedef struct TaskPair
{
    TaskFunc task;
    void* args;
} TaskPair;


/// A data structure type used in concurrency programming
typedef struct ConcurrentQueue ConcurrentQueue;

/**
 * @brief Create a new concurrent queue
 * @param isBlocking Whether the queue is blocking or not
 * @return The new concurrent queue object
 */
API_MODULE ConcurrentQueue* ConcurrentQueue_Create(bool isBlocking);

/**
 * @brief Push a task into the concurrent queue
 * @param queue The concurrent queue
 * @param task The task to be pushed
 * @param args The arguments or context passed to the task
 */
API_MODULE void ConcurrentQueue_PushBack(ConcurrentQueue* queue, TaskFunc task, void* args);

/**
 * @brief Get the front task of the concurrent queue
 * @param queue The concurrent queue
 * @param outPair The <key, value> pair of front task of the concurrent queue
 * @return True if the task is successfully got, false otherwise
 * @attention If the queue is empty in blocking mode, the function will block until
 * a task is pushed into the queue
 */
API_MODULE bool ConcurrentQueue_Front(ConcurrentQueue* queue, TaskPair* outPair);

/**
 * @brief Notify all the threads waiting for tasks in this queue
 * @param queue The concurrent queue
 */
API_MODULE void ConcurrentQueue_Stop(ConcurrentQueue* queue);

/**
 * @brief Get the size of the concurrent queue
 * @param queue The concurrent queue
 * @return The size of the concurrent queue
 */
API_MODULE int ConcurretQueue_Size(ConcurrentQueue* queue);

/**
 * @brief Check whether the concurrent queue is empty
 * @param queue The concurrent queue
 * @return True if the concurrent queue is empty, false otherwise
 */
API_MODULE bool ConcurrentQueue_Empty(ConcurrentQueue* queue);

/**
 * @brief Destroy the concurrent queue
 * @param queue The concurrent queue
 * @warning This function only destroys the concurrent queue, and it will not check whether the queue is still active!
 * The destroy behaviour happens immediately without waiting for all the tasks to be finished! After calling this function,
 * the queue object is not usable. Setting the pointer to nullptr is necessary.
 */
API_MODULE void ConcurrentQueue_Destroy(ConcurrentQueue* queue);


#endif //CONCURRENTQUEUE_H
