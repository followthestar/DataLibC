//
// Created by 18030 on 2024/11/17.
//

#include <assert.h>
#include <stdlib.h>
#include <memory.h>
#include "LinkedList.h"
#include "DataLibC/Tool/LogMacro.h"
#include "DataLibC/DS/DefaultOperation.h"

static void _ListNode_Init(ListNode* node, LinkedList* list, ListNode* prev, ListNode* next, DataType type, void* data)
{
    assert(node);
    node->next = next;
    node->prev = prev;
    node->list = list;
    node->m_Data = data;
    node->m_Type = type;
    node->m_TypeSize = gTypeSize[type];

    if (prev)
        prev->next = node;
    if (next)
        next->prev = node;
}

static void _ListNode_RemoveOnly(LinkedList* list, ListNode* node)
{
    if (node->prev)
        node->prev->next = node->next;
    if (node->next)
        node->next->prev = node->prev;
    if (node->list != list)
    {
        if (node->list)
        {
            node->list->count--;
            list->count++;
        }
        node->list = list;
    }
}

void _ListNode_RemoveFromList(ListNode* node)
{
    assert(node);
    if (!node->list)
        return;
    if (node->prev)
        node->prev->next = node->next;
    if (node->next)
        node->next->prev = node->prev;
    node->list->count--;
    node->list->m_Validate++;
    free(node);
}

bool LinkedList_Init(LinkedList* list, DataType type, Constructor* ctor, Destructor* dtor)
{
    assert(list);
    assert(type != kUNKNOWN);
    if (type == kCUSTOM && ctor == NULL)
    {
        RuntimeErrorLog(InitializationFailed, "A custom type must has a contructor!");
        return false;
    }
    list->m_NodeType = type;
    list->m_Head = malloc(sizeof(ListNode));
    list->m_Tail = malloc(sizeof(ListNode));
    if (!list->m_Head || !list->m_Tail)
    {
        RuntimeErrorLog(MallocFailed, "Failed to initialize a list!");
        return false;
    }
    list->m_Head->m_Data = NULL;
    list->m_Tail->m_Data = NULL;
    list->m_Ctor = ctor;
    list->m_Dtor = dtor;
    if (!ctor)
        list->m_Ctor = GetBuiltinCtor(type);
    if (!dtor)
        list->m_Dtor = GetBuiltinDtor(type);
    _ListNode_Init(list->m_Head, list, NULL, list->m_Tail, type, NULL);
    _ListNode_Init(list->m_Tail, list, list->m_Head, NULL, type, NULL);
    list->m_Validate = 0;
    list->count = 0;
    return true;
}

ListNode* LinkedList_First(const LinkedList* list)
{
    assert(list);
    return list->m_Head->next;
}

ListNode* LinkedList_Last(const LinkedList* list)
{
    assert(list);
    return list->m_Tail->prev;
}

ListNode* LinkedList_AddLast(LinkedList* list, void* value)
{
    assert(list);
    ListNode* newNode = malloc(sizeof(ListNode));
    if (!newNode)
    {
        RuntimeErrorLog(MallocFailed, "Failed to create a new node!");
        return NULL;
    }
    void* nodeData = list->m_Ctor(value);
    _ListNode_Init(newNode, list, list->m_Tail->prev, list->m_Tail, list->m_NodeType, nodeData);
    list->count++;
    return newNode;
}

ListNode* LinkedList_AddFirst(LinkedList* list, void* value)
{
    assert(list);
    ListNode* newNode = malloc(sizeof(ListNode));
    if (!newNode)
    {
        RuntimeErrorLog(MallocFailed, "Failed to create a new node!");
        return NULL;
    }
    void* nodeData = list->m_Ctor(value);
    _ListNode_Init(newNode, list, list->m_Head, list->m_Head->next, list->m_NodeType, nodeData);
    list->count++;
    return newNode;
}

void LinkedList_RemoveNode(LinkedList* list, ListNode* node)
{
    assert(list);
    if (list != node->list)
        return;
    list->m_Dtor(node->m_Data);
    _ListNode_RemoveFromList(node);
}

void LinkedList_Reverse(LinkedList* list)
{
    assert(list);
    if (list->count <= 1)
        return;
    auto node = LinkedList_First(list)->next;
    for (int i = 1; i < list->count; i++)
    {
        auto insert = node;
        node = node->next;
        LinkedList_AddFirstNode(list, insert);
    }
}

void LinkedList_AddFirstNode(LinkedList* list, ListNode* node)
{
    assert(list);
    assert(node);

    _ListNode_RemoveOnly(list, node);

    auto first = LinkedList_First(list);
    node->next = first;
    first->prev = node;
    list->m_Head->next = node;
    node->prev = list->m_Head;
    list->m_Validate++;
}

void LinkedList_AddLastNode(LinkedList* list, ListNode* node)
{
    assert(list);
    assert(node);

    _ListNode_RemoveOnly(list, node);

    auto last = LinkedList_Last(list);
    node->prev = last;
    last->next = node;
    list->m_Tail->prev = node;
    node->next = list->m_Tail;
    list->m_Validate++;
}

void LinkedList_Clear(LinkedList* list)
{
    assert(list);
    auto iter = LinkedList_Iterator(list);
    while (HasNext(iter))
    {
        Next(iter);
        Remove(iter);
    }
}

void LinkedList_Remove(LinkedList* list, void* value)
{
    assert(list);
    static Comparator* comp = NULL;
    comp = gDefaultComparator[list->m_NodeType];
    auto iter = LinkedList_Iterator(list);
    while (HasNext(iter))
    {
        auto dataPtr = Next(iter);
        if (comp(dataPtr, value) == 0)
        {
            Remove(iter);
            return;
        }
    }
}




