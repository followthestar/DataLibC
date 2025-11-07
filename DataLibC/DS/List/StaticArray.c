//
// Created by 18030 on 2024/11/13.
//

#include "StaticArray.h"
#include "DataLibC/DS/DefaultOperation.h"
#include "DataLibC/DS/Algorithm.h"

#include <assert.h>
#include <string.h>
#include <malloc.h>

static void* _StaticArrayElementPtr(StaticArray* arr, size_t index)
{
    return arr->m_Array + index * (arr->typeSize);
}

static void _StaticArrayAssign(StaticArray* arr, size_t index, void* value)
{
    memcpy(_StaticArrayElementPtr(arr, index), value, arr->typeSize);
}

static void _StaticArrayGenerateFormatString(StaticArray* arr)
{
    if (arr->m_FormatString)
        free((void*) arr->m_FormatString);
    size_t bufferSize = sizeof(char) * arr->length * 10;
    char* formatString = malloc(bufferSize);
    if (formatString == NULL)
    {
        RuntimeErrorLog(MallocFailed, "Failed to allocate memory!");
        return;
    }
    formatString[0] = '[';
    formatString[1] = '\0';
    int pos = 1;
    for (int i = 0; i < arr->length; i++)
    {
        if (pos >= bufferSize)
        {
            char* newMem = realloc(formatString, bufferSize * 2);
            if (newMem)
                formatString = newMem;
            else
            {
                RuntimeErrorLog(MallocFailed, "Memory reallocate failed!");
                formatString[0] = '\0';
                return;
            }
        }
        switch (arr->m_Type)
        {
            case kUNKNOWN:
                return;
            case kUCHAR:
                pos += sprintf(formatString + pos, "'%c'", *(unsigned char*) _StaticArrayElementPtr(arr, i));
                break;
            case kCHAR:
                pos += sprintf(formatString + pos, "'%c'", *(char*) _StaticArrayElementPtr(arr, i));
            case kUSHORT:
                pos += sprintf(formatString + pos, "%u", *(unsigned short*) _StaticArrayElementPtr(arr, i));
                break;
            case kSHORT:
                pos += sprintf(formatString + pos, "%d", *(short*) _StaticArrayElementPtr(arr, i));
                break;
            case kUINT:
                pos += sprintf(formatString + pos, "%u", *(unsigned*) _StaticArrayElementPtr(arr, i));
                break;
            case kINT:
                pos += sprintf(formatString + pos, "%d", *(int*) _StaticArrayElementPtr(arr, i));
                break;
            case kLLONG:
                pos += sprintf(formatString + pos, "%lld", *(long long*) _StaticArrayElementPtr(arr, i));
                break;
            case kFLOAT:
                pos += sprintf(formatString + pos, "%g", *(float*) _StaticArrayElementPtr(arr, i));
                break;
            case kDOUBLE:
                pos += sprintf(formatString + pos, "%g", *(double*) _StaticArrayElementPtr(arr, i));
                break;
        }
        if (i != arr->length - 1)
        {
            strcat(formatString + pos, ", ");
            pos += 2;
        }
    }
    strcat(formatString + pos, "]");

    arr->m_FormatString = formatString;
}

static int _StaticArrayDefaultComparator(StaticArray* arr, void* lhs, void* rhs)
{
    switch (arr->m_Type)
    {
        case kUCHAR:
            return *(unsigned char*) lhs == *(unsigned char*) rhs ?
                   0 : (*(unsigned char*) lhs < *(unsigned char*) rhs ? -1 : 1);
        case kCHAR:
            return *(char*) lhs - *(char*) rhs;
        case kUSHORT:
            return *(unsigned short*) lhs == *(unsigned short*) rhs ?
                   0 : (*(unsigned short*) lhs < *(unsigned short*) rhs ? -1 : 1);
        case kSHORT:
            return *(short*) lhs - *(short*) rhs;
        case kUINT:
            return *(unsigned*) lhs == *(unsigned*) rhs ?
                   0 : (*(unsigned*) lhs < *(unsigned*) rhs ? -1 : 1);
        case kINT:
            return *(int*) lhs - *(int*) rhs;
        case kLLONG:
            return *(long long*) lhs == *(long long*) rhs ?
                   0 : (*(long long*) lhs < *(long long*) rhs ? -1 : 1);
        case kFLOAT:
            return *(float*) lhs == *(float*) rhs ?
                   0 : (*(float*) lhs < *(float*) rhs ? -1 : 1);
        case kDOUBLE:
            return *(double*) lhs == *(double*) rhs ?
                   0 : (*(double*) lhs < *(double*) rhs ? -1 : 1);
        default:
            return 0;
    }
}

static void _StaticArraySortInternal(StaticArray* arr, Predicate* com)
{
    assert(com);
    QuickSort(arr->m_Array, arr->length, arr->typeSize, com);
}

bool StaticArray_Init(StaticArray* arr, DataType type, size_t maxLength)
{
    assert(arr);
    assert(type != kUNKNOWN);
    INIT_ASSIGN(arr, maxLength, maxLength);
    INIT_ASSIGN(arr, typeSize, gTypeSize[type]);
    arr->m_Array = malloc(arr->typeSize * maxLength);
    if (arr->m_Array == NULL)
    {
        RuntimeErrorLog(MallocFailed, "Failed to allocate memory to StaticArray");
        StaticArray_Delete(arr);
        return false;
    }
    arr->length = 0;
    arr->m_Type = type;
    arr->m_Dirty = true;
    arr->m_FormatString = NULL;
    return true;
}

void StaticArray_Delete(StaticArray* arr)
{
    assert(arr);
    if (arr->m_Array != NULL)
        free(arr->m_Array);
    if (arr->m_FormatString)
        free((void*) arr->m_FormatString);
    arr->length = 0;
    arr->m_Array = NULL;
    arr->m_FormatString = NULL;
    arr->m_Dirty = true;
    INIT_ASSIGN(arr, maxLength, 0);
    INIT_ASSIGN(arr, typeSize, 0);
}

int StaticArray_Add(StaticArray* arr, void* value)
{
    assert(arr);
    assert(value);
    if (!StaticArray_Insert(arr, value, arr->length))
        return -1;
    return arr->length - 1;
}

void* StaticArray_Get(StaticArray* arr, int index)
{
    assert(arr);
    if (index < 0 || index >= arr->length)
    {
        RuntimeErrorLog(IndexOutOfRange, "The index must be in range [0, %zu)!", arr->length);
        return NULL;
    }
    return _StaticArrayElementPtr(arr, index);
}

bool StaticArray_Insert(StaticArray* arr, void* value, int index)
{
    assert(arr);
    assert(value);
    if (StaticArray_IsFull(arr))
    {
        RuntimeErrorLog(ReachMexLength, "The array is full!");
        return false;
    }
    if (index < 0 || index > arr->length)
    {
        RuntimeErrorLog(IndexOutOfRange, "The index must be in range [0, %zu]!", arr->length);
        return false;
    }
    memmove(_StaticArrayElementPtr(arr, index + 1), _StaticArrayElementPtr(arr, index),
            arr->typeSize * (arr->length - index));
    _StaticArrayAssign(arr, index, value);
    arr->length++;
    arr->m_Dirty = true;
    return true;
}

bool StaticArray_IsEmpty(StaticArray* arr)
{
    assert(arr);
    return arr->length == 0;
}

bool StaticArray_IsFull(StaticArray* arr)
{
    assert(arr);
    return arr->length == arr->maxLength;
}

bool StaticArray_RemoveAt(StaticArray* arr, int index)
{
    assert(arr);
    if (index < 0 || index >= arr->length)
    {
        RuntimeErrorLog(IndexOutOfRange, "The index must be in range [0, %zu)!", arr->length);
        return false;
    }
    memmove(_StaticArrayElementPtr(arr, index), _StaticArrayElementPtr(arr, index + 1),
            arr->typeSize * (arr->length - index - 1));
    arr->length--;
    arr->m_Dirty = true;
    return true;
}

void StaticArray_Reset(StaticArray* arr)
{
    assert(arr);
    arr->length = 0;
    memset(arr->m_Array, 0, arr->typeSize * arr->maxLength);
    arr->m_Array = NULL;
    arr->m_Dirty = true;
}

void StaticArray_Clear(StaticArray* arr)
{
    assert(arr);
    memset(arr->m_Array, 0, arr->typeSize * arr->maxLength);
    arr->m_Dirty = true;
}

bool StaticArray_RemoveElement(StaticArray* arr, void* value)
{
    assert(arr);
    assert(value);
    int index = StaticArray_Find(arr, value);
    if (index == -1)
    {
        RuntimeErrorLog(NoSuchValue, "The value is not found!");
        return false;
    }
    return StaticArray_RemoveAt(arr, index);
}

int StaticArray_Find(StaticArray* arr, void* value)
{
    assert(arr);
    assert(value);

    for (size_t i = 0; i < arr->length; i++)
    {
        if (_StaticArrayDefaultComparator(arr, _StaticArrayElementPtr(arr, i), value) == 0)
            return (int) i;
    }
    return -1;
}

const char* StaticArray_ToString(StaticArray* arr)
{
    assert(arr);
    if (arr->m_Dirty)
    {
        _StaticArrayGenerateFormatString(arr);
        arr->m_Dirty = false;
    }
    return arr->m_FormatString;
}

void StaticArray_Set(StaticArray* arr, int index, void* value)
{
    assert(arr);
    if (index < 0 || index >= arr->length)
    {
        RuntimeErrorLog(IndexOutOfRange, "Failed to set value at index %d", index);
        return;
    }
    _StaticArrayAssign(arr, index, value);
    arr->m_Dirty = true;
}

void StaticArray_SetAll(StaticArray* arr, void* value)
{
    assert(arr);
    for (size_t i = 0; i < arr->length; i++)
        _StaticArrayAssign(arr, i, value);
}

void StaticArray_Fill(StaticArray* arr, int start, int length, void* value)
{
    assert(arr);
    if (start < 0 || start + length > arr->maxLength)
    {
        RuntimeErrorLog(IndexOutOfRange, "Cannot fill the array with range [%d, %d]", start, start + length);
        return;
    }
    for (int i = start; i < start + length; i++)
        _StaticArrayAssign(arr, i, value);
    arr->m_Dirty = true;
    if (arr->length < start + length)
        arr->length = start + length;
}

void StaticArray_FillAll(StaticArray* arr, void* value)
{
    assert(arr);
    StaticArray_Fill(arr, 0, arr->maxLength, value);
}

void StaticArray_Sort(StaticArray* arr, Predicate* pred)
{
    assert(arr);
    Predicate* com = pred;
    if (pred == NULL)
    {
        if (arr->m_Type == kCUSTOM)
        {
            RuntimeErrorLog(InvalidParameterType, "Sorting custom type must be provided with a comparator!");
            return;
        }
        com = gDefaultLess[arr->m_Type];
    }
    _StaticArraySortInternal(arr, com);
    arr->m_Dirty = true;
}

StaticArray* StaticArray_MakeSort(StaticArray* arr, Predicate* pred)
{
    assert(arr);
    StaticArray* maked = malloc(sizeof(StaticArray));
    if (!StaticArray_Init(maked, arr->m_Type, arr->maxLength))
    {
        RuntimeErrorLog(MallocFailed, "Failed to make a new StaticArray!");
        StaticArray_Clear(maked);
        free(maked);
        return NULL;
    }
    maked->length = arr->length;
    memcpy(maked->m_Array, arr->m_Array, arr->length * arr->typeSize);
    StaticArray_Sort(maked, pred);
    return maked;
}
