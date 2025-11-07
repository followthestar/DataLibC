//
// Created by 18030 on 2024/11/17.
//

#ifndef TEST_LINKEDLIST_H
#define TEST_LINKEDLIST_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/DS/TypeEnum.h"
#include "DataLibC/Tool/TypeMacro.h"
#include "DataLibC/DS/Iterator.h"
#include "DataLibC/DS/Allocation.h"

typedef struct LinkedList LinkedList;
typedef struct ListNode ListNode;

struct LIB_STRUCT ListNode
{
    READ_ONLY LinkedList* list;
    PRIVATE Byte* m_Data;
    PRIVATE DataType m_Type;
    PRIVATE size_t m_TypeSize;

    READ_ONLY ListNode* next;
    READ_ONLY ListNode* prev;
};


struct LIB_STRUCT LinkedList
{
    PRIVATE DataType m_NodeType;

    READ_ONLY size_t count;

    PRIVATE ListNode* m_Head;
    PRIVATE ListNode* m_Tail;

    PRIVATE Constructor* m_Ctor;
    PRIVATE Destructor* m_Dtor;

    PRIVATE volatile size_t m_Validate;
};

API_MODULE bool LinkedList_Init(LinkedList* list, DataType type, Constructor* ctor, Destructor* dtor);

API_MODULE ListNode* LinkedList_AddLast(LinkedList* list, void* value);

API_MODULE ListNode* LinkedList_AddFirst(LinkedList* list, void* value);

API_MODULE ListNode* LinkedList_First(const LinkedList* list);

API_MODULE ListNode* LinkedList_Last(const LinkedList* list);

API_MODULE void LinkedList_AddLastNode(LinkedList* list, ListNode* node);

API_MODULE void LinkedList_AddFirstNode(LinkedList* list, ListNode* node);

API_MODULE void LinkedList_AddAfter(LinkedList* list, const ListNode* node, const ListNode* newNode);

API_MODULE void LinkedList_AddBefore(LinkedList* list, const ListNode* node, const ListNode* newNode);

API_MODULE void LinkedList_Clear(LinkedList* list);

API_MODULE void LinkedList_Remove(LinkedList* list, void* value);

API_MODULE void LinkedList_RemoveNode(LinkedList* list, ListNode* node);

API_MODULE void LinkedList_Reverse(LinkedList* list);


/// A helper version of StaticArray_Add. It will wrap a rvalue automatically
#define LinkedList_AddLastValue(obj, value) (obj == NULL ? NULL : (((obj)->m_NodeSize == sizeof(value)) ? \
    LinkedList_AddLast(obj, ARG_CONVERT(value)) : \
    (RuntimeErrorLog(InvalidParameterType, TypeErrorMsg,\
    (obj)->m_NodeSize, sizeof(value)), NULL)))

#endif //TEST_LINKEDLIST_H
