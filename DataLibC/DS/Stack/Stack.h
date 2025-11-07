//
// Created by 18030 on 2024/11/23.
//

#ifndef TEST_STACK_H
#define TEST_STACK_H

#include <complex.h>
#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/DS/List/LinkedList.h"

struct LIB_STRUCT Stack
{
    PRIVATE LinkedList m_Container;
    READ_ONLY size_t size;
};

typedef struct Stack Stack;
typedef ListNode StackNode;

API_MODULE bool Stack_Init(Stack* stack, DataType type, Constructor* ctor, Destructor* dtor);

API_MODULE bool Stack_Empty(Stack* stack);

API_MODULE void Stack_Push(Stack* stack, void* value);

API_MODULE void Stack_Pop(Stack* stack);

API_MODULE void* Stack_Top(Stack* stack);

API_MODULE const StackNode* Stack_TopNode(Stack* stack);

#endif //TEST_STACK_H
