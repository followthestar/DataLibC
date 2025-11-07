//
// Created by 18030 on 2024/11/19.
//

#include "SortedVector.h"
#include <assert.h>
#include <stdarg.h>
#include "DataLibC/DS/DefaultOperation.h"
#include "DataLibC/DS/Algorithm.h"
#include "DataLibC/Tool/LogMacro.h"

bool _DynamicArray_Init_Internal(DynamicArray* arr, size_t initSize, DataType type, va_list args);

void* _DynamicArray_GetElemPtr(DynamicArray* arr, size_t index);

bool SortedVector_Init(SortedVector* vec, Predicate* pred, size_t initSize, DataType type, ...)
{
    assert(vec);
    va_list args;
    va_start(args, type);
    const bool ret = _DynamicArray_Init_Internal(&vec->m_Container, initSize, type, args);
    va_end(args);
    vec->m_Pred = pred ? pred : GetBuiltinLess(type);
    vec->length = initSize;
    QuickSort(vec->m_Container.m_Array, vec->length, vec->m_Container.m_TypeSize, vec->m_Pred);
    return ret;
}

void SortedVector_Add(SortedVector* vec, void* value)
{
    assert(vec);
    vec->length = vec->m_Container.length;
    size_t pos = LowerBound(vec->m_Container.m_Array, vec->length,
        vec->m_Container.m_TypeSize, vec->m_Pred, value);
    if (DynamicArray_InsertAt(&vec->m_Container, pos, value))
        vec->length++;
}

ssize_t SortedVector_Find(SortedVector* vec, void* value)
{
    assert(vec);
    vec->length = vec->m_Container.length;
    return BinarySearch(vec->m_Container.m_Array, vec->length,
        vec->m_Container.m_TypeSize, vec->m_Pred, value);
}

const void* SortedVector_ElementAt(SortedVector* vec, size_t index)
{
    assert(vec);
    vec->length = vec->m_Container.length;
    if (index >= vec->length)
    {
        RuntimeErrorLog(IndexOutOfRange, "Index out of range in the SortedVector!");
        return NULL;
    }
    return _DynamicArray_GetElemPtr(&vec->m_Container, index);
}

void SortedVector_RemoveAt(SortedVector* vec, size_t index)
{
    assert(vec);
    //TODO
}

void SortedVector_Synchronize(SortedVector* vec)
{
    assert(vec);
    vec->length = vec->m_Container.length;
}
