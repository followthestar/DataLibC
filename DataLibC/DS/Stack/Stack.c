//
// Created by 18030 on 2024/11/23.
//

#include <assert.h>
#include "Stack.h"

bool Stack_Init(Stack* stack, DataType type, Constructor* ctor, Destructor* dtor)
{
    assert(stack);
    if (!LinkedList_Init(&stack->m_Container, type, ctor, dtor))
        return false;
    stack->size = 0;
    return true;
}

bool Stack_Empty(Stack* stack)
{
    assert(stack);
    return stack->size == 0;
}

void Stack_Push(Stack* stack, void* value)
{
    assert(stack);
    LinkedList_AddFirst(&stack->m_Container, value);
    stack->size++;
}

void Stack_Pop(Stack* stack)
{
    assert(stack);
    auto top = LinkedList_First(&stack->m_Container);
    LinkedList_RemoveNode(&stack->m_Container, top);
    stack->size--;
}

void* Stack_Top(Stack* stack)
{
    assert(stack);
    return LinkedList_First(&stack->m_Container)->m_Data;
}

const StackNode* Stack_TopNode(Stack* stack)
{
    return LinkedList_First(&stack->m_Container);
}
