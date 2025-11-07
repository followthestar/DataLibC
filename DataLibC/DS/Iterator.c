//
// Created by 18030 on 2024/11/17.
//

#include "Iterator.h"
#include "DataLibC/DS/List/LinkedList.h"
#include "DataLibC/DS/List/DynamicArray.h"
#include "DataLibC/DS/List/SortedVector.h"
#include "DataLibC/DS/Set/SortedSet.h"
#include "DataLibC/DS/Map/SortedMap.h"
#include "DataLibC/DS/Map/TreeMap.h"
#include "DataLibC/DS/Set/TreeSet.h"
#include "DataLibC/DS/Hash/hash_table_internal.h"
#include "DataLibC/Tool/LogMacro.h"

typedef struct LIB_STRUCT _hash_table_iterator { } _Hash_table_iterator;


static bool _ListIterator_Verify(ListIterator iter)
{
    return iter.m_Validate == iter.m_List->m_Validate;
}

void _ListNode_RemoveFromList(ListNode* node);

static bool _DynamicArrayIterator_Verify(DynamicArrayIterator iter)
{
    return iter.m_Validate == iter.m_Array->m_Validate;
}

void* ListIterator_Next(ListIterator* iter)
{
    if (!ListIterator_HasNext(*iter))
    {
        RuntimeErrorLog(IterationOutOfBounds, "The iterator has no more next value!");
        return NULL;
    }
    void* data = iter->m_Curr->m_Data;
    iter->m_LastReturned = iter->m_Curr;
    iter->m_Curr = iter->m_Curr->next;
    return data;
}

bool ListIterator_HasNext(ListIterator iter)
{
    if (!_ListIterator_Verify(iter))
    {
        RuntimeErrorLog(InvalidIterator, "The iterator is invalid!");
        return false;
    }
    return iter.m_Curr != iter.m_List->m_Tail;
}

ListIterator LinkedList_Iterator(LinkedList* list)
{
    ListIterator iter = { LinkedList_First(list), NULL, list, list->m_Validate };
    return iter;
}

void ListIterator_Remove(ListIterator* iter)
{
    if (!_ListIterator_Verify(*iter))
    {
        RuntimeErrorLog(InvalidIterator, "The iterator is invalid!");
        return;
    }
    if (!iter->m_LastReturned)
    {
        RuntimeErrorLog(IllegalState, "The iterator has not been moved!");
        return;
    }
    LinkedList_RemoveNode(iter->m_List, iter->m_LastReturned);
    iter->m_LastReturned = NULL;
    PLUS_ASSIGN(iter, m_Validate, 1);
}

DynamicArrayIterator DynamicArray_Iterator(DynamicArray* arr)
{
    DynamicArrayIterator iter = { -1, 0, arr, arr->m_Validate };
    return iter;
}

bool DynamicArrayIterator_HasNext(DynamicArrayIterator iter)
{
    if (!_DynamicArrayIterator_Verify(iter))
    {
        RuntimeErrorLog(InvalidIterator, "The iterator is invalid!");
        return false;
    }
    return iter.m_NextCursor != iter.m_Array->length;
}

void* DynamicArrayIterator_Next(DynamicArrayIterator* iter)
{
    if (!_DynamicArrayIterator_Verify(*iter))
    {
        RuntimeErrorLog(InvalidIterator, "The iterator is invalid!");
        return NULL;
    }
    if (!DynamicArrayIterator_HasNext(*iter))
    {
        RuntimeErrorLog(IterationOutOfBounds, "The iterator has no more next value!");
        return NULL;
    }
    void* data = iter->m_Array->m_Array + iter->m_Array->m_TypeSize * iter->m_NextCursor;
    iter->m_LastCursor = iter->m_NextCursor;
    iter->m_NextCursor++;
    return data;
}

void DynamicArrayIterator_Remove(DynamicArrayIterator* iter)
{
    if (!_DynamicArrayIterator_Verify(*iter))
    {
        RuntimeErrorLog(InvalidIterator, "The iterator is invalid!");
        return;
    }
    if (iter->m_LastCursor == -1)
    {
        RuntimeErrorLog(IllegalState, "The iterator has not been moved!");
        return;
    }
    DynamicArray_RemoveAt(iter->m_Array, iter->m_LastCursor);
    iter->m_LastCursor = -1;
    iter->m_NextCursor--;
    PLUS_ASSIGN(iter, m_Validate, 1);
}

SortedVectorIterator SortedVector_Iterator(SortedVector* vec)
{
    return DynamicArray_Iterator(&vec->m_Container);
}

SortedSetIterator SortedSet_Iterator(SortedSet* vec)
{
    return DynamicArray_Iterator(&vec->m_Container.m_Container);
}

SortedMapIterator SortedMap_Iterator(SortedMap* map)
{
    return DynamicArray_Iterator(&map->m_Container.m_Container);
}

_Rb_tree_iterator _tree_iterator_internal(_Rb_tree* tree);

TreeSetIterator TreeSet_Iterator(TreeSet* set)
{
    return _tree_iterator_internal(set->_M_tree);
}

TreeMapIterator TreeMap_Iterator(TreeMap* map)
{
    return _tree_iterator_internal(map->_M_tree);
}

StringIterator String_Iterator(void* str)
{
    const StringIterator iter = { *(char**)str, 0 };
    return iter;
}

char StringIterator_Next(StringIterator* iter)
{
    if (!StringIterator_HasNext(*iter))
    {
        RuntimeErrorLog(IterationOutOfBounds, "The iterator has no more next value!");
        return 0;
    }
    const char c = iter->m_String[iter->m_Index];
    iter->m_Index++;
    return c;
}

bool StringIterator_HasNext(StringIterator iter)
{
    return iter.m_String[iter.m_Index] != '\0';
}



