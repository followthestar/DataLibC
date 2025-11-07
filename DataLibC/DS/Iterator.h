//
// Created by 18030 on 2024/11/17.
//

#ifndef TEST_ITERATOR_H
#define TEST_ITERATOR_H

#include <stddef.h>
#include "DataLibC/Tool/ExportModuleMacro.h"

typedef struct ListNode ListNode;
typedef struct LinkedList LinkedList;
typedef struct DynamicArray DynamicArray;
typedef struct SortedVector SortedVector;
typedef struct SortedSet SortedSet;
typedef struct HashSet HashSet;
typedef struct TreeSet TreeSet;
typedef struct SortedMap SortedMap;
typedef struct TreeMap TreeMap;

typedef struct _rb_tree_node _Rb_tree_node;

typedef struct DynamicArrayIterator SortedVectorIterator;
typedef struct DynamicArrayIterator SortedSetIterator;
typedef struct DynamicArrayIterator SortedMapIterator;
typedef struct StringIterator StringIterator;

#define GetIterator(x) _Generic((x), \
                            LinkedList*: LinkedList_Iterator, \
                            DynamicArray*: DynamicArray_Iterator,\
                            SortedVector*: SortedVector_Iterator,\
                            SortedSet*: SortedSet_Iterator,   \
                            SortedMap*: SortedMap_Iterator,   \
                            TreeSet*: TreeSet_Iterator,       \
                            TreeMap* : TreeMap_Iterator,       \
                            const char**: String_Iterator,\
                            char**: String_Iterator\
                            )(x)

#define HasNext(x) _Generic((x), \
                            ListIterator: ListIterator_HasNext, \
                            DynamicArrayIterator: DynamicArrayIterator_HasNext, \
                            _Rb_tree_iterator: _Rb_tree_iterator_HasNext,     \
                            StringIterator: StringIterator_HasNext\
                            )(x)

#define Next(x) _Generic((x), \
                            ListIterator: ListIterator_Next, \
                            DynamicArrayIterator: DynamicArrayIterator_Next, \
                            _Rb_tree_iterator: _Rb_tree_iterator_Next,  \
                            StringIterator: StringIterator_Next\
                            )(&(x))

#define Remove(x) _Generic((x), \
                            ListIterator: ListIterator_Remove, \
                            DynamicArrayIterator: DynamicArrayIterator_Remove\
                            )(&(x))

struct LIB_STRUCT StringIterator
{
    PRIVATE const char* m_String;
    PRIVATE size_t m_Index;
};

struct LIB_STRUCT ListIterator
{
    PRIVATE ListNode* m_Curr;
    PRIVATE ListNode* m_LastReturned;
    PRIVATE LinkedList* m_List;
    PRIVATE const size_t m_Validate;
};

typedef struct ListIterator ListIterator;

struct LIB_STRUCT DynamicArrayIterator
{
    PRIVATE int m_LastCursor;
    PRIVATE int m_NextCursor;
    PRIVATE DynamicArray* m_Array;
    PRIVATE const size_t m_Validate;
};

typedef struct DynamicArrayIterator DynamicArrayIterator;

typedef struct _hash_table_iterator _Hash_table_iterator;

typedef _Hash_table_iterator HashSetIterator;
typedef _Hash_table_iterator HashMapIterator;

typedef struct _rb_tree_iterator
{
    PRIVATE _Rb_tree_node* _M_Current;
} _Rb_tree_iterator;

typedef _Rb_tree_iterator TreeSetIterator;
typedef _Rb_tree_iterator TreeMapIterator;


API_MODULE bool ListIterator_HasNext(ListIterator iter);

API_MODULE void* ListIterator_Next(ListIterator* iter);

API_MODULE void ListIterator_Remove(ListIterator* iter);

API_MODULE ListIterator LinkedList_Iterator(LinkedList* list);

API_MODULE bool DynamicArrayIterator_HasNext(DynamicArrayIterator iter);

API_MODULE void* DynamicArrayIterator_Next(DynamicArrayIterator* iter);

API_MODULE void DynamicArrayIterator_Remove(DynamicArrayIterator* iter);

API_MODULE DynamicArrayIterator DynamicArray_Iterator(DynamicArray* arr);

API_MODULE SortedVectorIterator SortedVector_Iterator(SortedVector* vec);

API_MODULE SortedSetIterator SortedSet_Iterator(SortedSet* vec);

API_MODULE HashSetIterator HashSet_Iterator(HashSet* set);

API_MODULE TreeSetIterator TreeSet_Iterator(TreeSet* set);

API_MODULE SortedMapIterator SortedMap_Iterator(SortedMap* map);

API_MODULE TreeMapIterator TreeMap_Iterator(TreeMap* map);

API_MODULE StringIterator String_Iterator(void* str);

API_MODULE char StringIterator_Next(StringIterator* iter);

API_MODULE bool StringIterator_HasNext(StringIterator iter);

API_MODULE bool _Rb_tree_iterator_HasNext(_Rb_tree_iterator iter);

API_MODULE void* _Rb_tree_iterator_Next(_Rb_tree_iterator* iter);

#endif //TEST_ITERATOR_H
