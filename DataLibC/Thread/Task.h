//
// Created by 18030 on 2025/9/27.
//

#ifndef TASK_H
#define TASK_H

#include "DataLibC/Tool/ExportModuleMacro.h"

/**
 * A Task object is gettable and awaitable.
 * @attention Its memory is managed by GC. DON'T FREE THE MEMORY MANUALLY!
 */
typedef struct Task Task;

/**
 * @brief A standard task function call
 * @param args The arguments or context passed to the task
 * @return The result of this task
 */
typedef void* (* TaskFunc)(void* args);

/**
 * @brief Create a new task
 * @param taskFunc The task function to be called
 * @param args The arguments or context passed to the task
 * @return The new task object
 */
API_MODULE Task* Task_Run(TaskFunc taskFunc, void* args);

/**
 * @brief Wait for the task to be finished and get the result
 * @param task The task object
 * @return The result of this task
 */
API_MODULE void* Task_Get(Task* task);

/**
 * @brief Wait for the task to be finished
 * @param task The task object
 */
API_MODULE void Task_Wait(Task* task);

#endif //TASK_H
