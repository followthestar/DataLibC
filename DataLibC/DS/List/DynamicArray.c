//
// Created by 18030 on 2024/11/17.
//

#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <memory.h>
#include <string.h>
#include "DynamicArray.h"
#include "DataLibC/DS/Algorithm.h"
#include "DataLibC/Tool/LogMacro.h"
#include "DataLibC/Tool/Misc.h"

void* _DynamicArray_GetElemPtr(DynamicArray* arr, size_t index)
{
    return arr->m_Array + arr->m_TypeSize * index;
}

static void _DynamicArray_Assign(DynamicArray* arr, size_t index, void* value)
{
    memcpy(_DynamicArray_GetElemPtr(arr, index), value, arr->m_TypeSize);
}

static bool _DynamicArray_Expand(DynamicArray* arr, size_t newAmount)
{
    void* temp = realloc(arr->m_Array, newAmount * arr->m_TypeSize);
    if (!temp)
        return false;
    arr->m_Array = temp;
    arr->capacity = newAmount;
    return true;
}

bool _DynamicArray_Init_Internal(DynamicArray* arr, size_t initSize, DataType type, size_t typeSize, va_list args)
{
    assert(arr);
    assert(type != kUNKNOWN);
    arr->m_Type = type;
    if (type == kCUSTOM)
        arr->m_TypeSize = typeSize;
    else
        arr->m_TypeSize = gTypeSize[type];
    arr->length = initSize;
    size_t cap = initSize == 0 ? 8 : initSize;
    arr->capacity = NextPowerTwo(cap * 2);
    arr->m_Array = malloc(sizeof(Byte) * arr->capacity * arr->m_TypeSize);
    if (!arr->m_Array)
    {
        RuntimeErrorLog(MallocFailed, "Failed to initialize DynamicArray memory!");
        return false;
    }
    memset(arr->m_Array, 0, arr->capacity * arr->m_TypeSize);
    ProcessVaListInternal(arr->m_Array, arr->m_Type, initSize, args);
    arr->m_Dirty = true;
    arr->m_Validate = 0;
    return true;
}

bool DynamicArray_Init(DynamicArray* arr, size_t initSize, DataType type, size_t typeSize, ...)
{
    assert(arr);
    assert(type != kUNKNOWN);

    va_list args;
    va_start(args, typeSize);
    bool ret = _DynamicArray_Init_Internal(arr, initSize, type, typeSize, args);
    va_end(args);
    return ret;
}

void DynamicArray_Add(DynamicArray* arr, void* value)
{
    assert(arr);
    if (arr->length == arr->capacity)
    {
        if (!_DynamicArray_Expand(arr, arr->capacity * 1.5f))
        {
            RuntimeErrorLog(MallocFailed, "Failed to add  value!");
            return;
        }
        arr->m_Validate++;
    }
    _DynamicArray_Assign(arr, arr->length, value);
    arr->length++;
    arr->m_Dirty = true;
}

bool DynamicArray_RemoveAt(DynamicArray* arr, size_t index)
{
    assert(arr);
    if (index >= arr->length)
    {
        RuntimeErrorLog(IndexOutOfRange, "Index out of range! %zu is not in [0, %zu)!", index, arr->length);
        return false;
    }
    memmove(_DynamicArray_GetElemPtr(arr, index), _DynamicArray_GetElemPtr(arr, index + 1),
            arr->m_TypeSize * (arr->length - index - 1));
    arr->length--;
    arr->m_Dirty = true;
    arr->m_Validate++;
    return true;
}

bool DynamicArray_InsertAt(DynamicArray* arr, size_t index, void* value)
{
    assert(arr);
    if (index > arr->length)
    {
        RuntimeErrorLog(IndexOutOfRange, "Index out of range!");
        return false;
    }
    if (arr->length == arr->capacity)
    {
        if (!_DynamicArray_Expand(arr, arr->capacity * 1.5f))
        {
            RuntimeErrorLog(MallocFailed, "Failed to add  value!");
            return false;
        }
    }
    memmove(_DynamicArray_GetElemPtr(arr, index + 1), _DynamicArray_GetElemPtr(arr, index),
            arr->m_TypeSize * (arr->length - index));
    _DynamicArray_Assign(arr, index, value);
    arr->length++;
    arr->m_Dirty = true;
    arr->m_Validate++;
    return true;
}

void* DynamicArray_ValueAt(DynamicArray* arr, size_t index)
{
    assert(arr);
    return _DynamicArray_GetElemPtr(arr, index);
}

void DynamicArray_Clear(DynamicArray* arr)
{
    assert(arr);
    memset(arr->m_Array, 0, arr->length * arr->m_TypeSize);
    arr->length = 0;
    arr->m_Dirty = true;
    arr->m_Validate++;
}


