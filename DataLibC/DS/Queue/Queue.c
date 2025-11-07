//
// Created by 18030 on 2024/11/20.
//

#include <assert.h>
#include "Queue.h"

bool Queue_Init(Queue* queue, const DataType type, Constructor* ctor, Destructor* dtor)
{
    assert(queue);
    if (!LinkedList_Init(&queue->m_Container, type, ctor, dtor))
        return false;
    queue->length = 0;
    return true;
}

bool Queue_Empty(const Queue* queue)
{
    assert(queue);
    return queue->length == 0;
}

void* Queue_Front(const Queue* queue)
{
    assert(queue);
    return LinkedList_First(&queue->m_Container)->m_Data;
}

const QueueNode* Queue_FrontNode(const Queue* queue)
{
    assert(queue);
    return LinkedList_First(&queue->m_Container);
}

void Queue_Dequeue(Queue* queue)
{
    assert(queue);
    const auto first = LinkedList_First(&queue->m_Container);
    LinkedList_RemoveNode(&queue->m_Container, first);
    queue->length--;
}

void Queue_Enqueue(Queue* queue, void* value)
{
    assert(queue);
    LinkedList_AddLast(&queue->m_Container, value);
    queue->length++;
}

void Queue_Clear(Queue* queue)
{
    assert(queue);
    LinkedList_Clear(&queue->m_Container);
    queue->length = 0;
}
