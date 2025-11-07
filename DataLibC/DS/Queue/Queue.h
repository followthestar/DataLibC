//
// Created by 18030 on 2024/11/20.
//

#ifndef TEST_QUEUE_H
#define TEST_QUEUE_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/DS/List/LinkedList.h"


/// A kind of Container Adapter implemented with LinkedList which supports adding element at tail and deleting element
/// at head but not supports iterator. (First in First out)
struct LIB_STRUCT Queue
{
    PRIVATE LinkedList m_Container;
    READ_ONLY size_t length;
};

typedef struct Queue Queue;
typedef ListNode QueueNode;

/// Initialize a Queue object.
/// \param queue The queue object.
/// \param type The type of elements stored in the Queue.
/// \return True if succeed, otherwise false.
API_MODULE bool Queue_Init(Queue* queue, DataType type, Constructor* ctor, Destructor* dtor);

/// Check whether a queue is empty.
/// \param queue The queue object.
/// \return True if it is empty, otherwise false.
API_MODULE bool Queue_Empty(const Queue* queue);

/// Get the first element in the Queue.
/// \param queue The queue object.
/// \return The pointer to the first value.
/// \tips Please check whether the queue is empty firstly otherwise an UB would happen.
API_MODULE void* Queue_Front(const Queue* queue);

/// Get the first node in the Queue.
/// \param queue The queue object.
/// \return The first node in the queue. NULL means the queue is empty.
API_MODULE const QueueNode* Queue_FrontNode(const Queue* queue);

/// Remove the first node in the Queue.
/// \param queue The queue object.
/// \tips Please check whether the queue is empty firstly otherwise an UB would happen.
API_MODULE void Queue_Dequeue(Queue* queue);

/// Add a new node with given value to the Queue.
/// \param queue The queue object.
/// \param value The value of the new node.
API_MODULE void Queue_Enqueue(Queue* queue, void* value);


/**
 * @brief Clear all the elements in the queue, and the destructor will be called for each element.
 * @param queue The queue object to be cleared.
 * @attention Remember to calling this function before release the Queue object memory unless ensuring the queue is empty.
 */
API_MODULE void Queue_Clear(Queue* queue);

#endif //TEST_QUEUE_H
