//
// Created by 18030 on 2024/11/13.
//

#ifndef TEST_STATICARRAY_H
#define TEST_STATICARRAY_H

#include <stddef.h>
#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/Tool/TypeMacro.h"
#include "DataLibC/Tool/LogMacro.h"
#include "DataLibC/DS/TypeEnum.h"

/// Ordinary array with unchangeable capacity
struct LIB_STRUCT StaticArray
{
    PRIVATE const size_t typeSize;
    READ_ONLY const size_t maxLength;
    READ_ONLY size_t length;
    PRIVATE Byte* m_Array;
    PRIVATE DataType m_Type;

    PRIVATE bool m_Dirty;
    PRIVATE const char* m_FormatString;
};

typedef struct StaticArray StaticArray;

typedef bool Predicate(void*, void*);

/// Initialize a StaticArray with the max amount of elements and size for each element
/// \param arr  The target array object. (NULL is not allowed)
/// \param maxLength The max amount of elements
/// \param typeSize The size of One element
/// \return True if initialize successfully, otherwise false
API_MODULE bool StaticArray_Init(StaticArray* arr, DataType type, size_t maxLength);

/// Destroy a StaticArray and clear its memory.
/// \param arr The array to be delete
API_MODULE void StaticArray_Delete(StaticArray* arr);

/// Add an element to a StaticArray. (Ensure the element type is matched to the array!)
/// \param arr The target array object. (NULL is not allowed)
/// \param value The pointer of the value that will be added
/// \return The index of inserted value. -1 when operation failed
API_MODULE int StaticArray_Add(StaticArray* arr, void* value);

/// Get the element pointer with given index
/// \param arr  The target array object. (NULL is not allowed)
/// \param index    The index of the element
/// \return The void pointer type of the element
API_MODULE void* StaticArray_Get(StaticArray* arr, int index);

/// Insert element into the StaticArray
/// \param arr  The target array object. (NULL is not allowed)
/// \param value    The pointer to the element to be inserted
/// \param index    The index position to be inserted
/// \return True if succeed, otherwise false
API_MODULE bool StaticArray_Insert(StaticArray* arr, void* value, int index);

/// Determine if the StaticArray is empty
/// \param arr The target array object
/// \return True if it is empty, otherwise false
API_MODULE bool StaticArray_IsEmpty(StaticArray* arr);

/// Determine if the StaticArray is full
/// \param arr The target array object
/// \return True if it is full, otherwise false
API_MODULE bool StaticArray_IsFull(StaticArray* arr);

/// Remove the element in a StaticArray at index
/// \param arr  The target array object. (NULL is not allowed)
/// \param value The index of element
/// \return True if removed, otherwise false
API_MODULE bool StaticArray_RemoveAt(StaticArray* arr, int index);

/// Remove the first element that is equal to the given value
/// \param arr  The target array object. (NULL is not allowed)
/// \param value The value to be compared
/// \return True if removed, otherwise false
API_MODULE bool StaticArray_RemoveElement(StaticArray* arr, void* value);

/// Reset elements only. The length will be remained
/// \param arr  The target array object. (NULL is not allowed)
API_MODULE void StaticArray_Clear(StaticArray* arr);

/// Empty the StaticArray and its elements and length will be reset
/// \param arr The target array object. (NULL is not allowed)
API_MODULE void StaticArray_Reset(StaticArray* arr);

/// Set the element at index to the given value
/// \param arr  The target array object. (NULL is not allowed)
/// \param index The index of element to be set value
/// \param value The value to be set
API_MODULE void StaticArray_Set(StaticArray* arr, int index, void* value);

/// Set all elements in the StaticArray
/// \param arr The target array object. (NULL is not allowed)
/// \param value The value to be set
API_MODULE void StaticArray_SetAll(StaticArray* arr, void* value);

/// Fill all the elements with given value. The length will be set to its maxLength
/// \param arr The target array object. (NULL is not allowed)
/// \param value The value to be filled in
API_MODULE void StaticArray_FillAll(StaticArray* arr, void* value);

/// Fill the elements within the index range of [start, start + length) with the given value
/// \param arr The target array object. (NULL is not allowed)
/// \param start The starting index
/// \param length The filling length
/// \param value The value to be filled in
API_MODULE void StaticArray_Fill(StaticArray* arr, int start, int length, void* value);

/// Find the first element equals to the given value
/// \param arr  The target array object. (NULL is now allowed)
/// \param value The value to be compared
/// \return Index of first found element. If not, return -1
API_MODULE int StaticArray_Find(StaticArray* arr, void* value);

/// Sort the StaticArray inplace
/// \param arr  The target array object. (NULL is not allowed)
/// \param pred The predicate defines the comparision rules
API_MODULE void StaticArray_Sort(StaticArray* arr, Predicate* pred);

/// Create a new ordered StaticArray from the given StaticArray. User must manage the return value!!!
/// \param arr  The original array object. (NULL is not allowed)
/// \param pred The predicate defines the comparison rules
/// \return True if sorded successfully, otherwise false
API_MODULE StaticArray* StaticArray_MakeSort(StaticArray* arr, Predicate* pred);

/// Create a format string of StaticArray
/// \param arr The target array object. (NULL is not allowed)
/// \return The format string filled with array elements
API_MODULE const char* StaticArray_ToString(StaticArray* arr);

/// A helper version of StaticArray_Add. It will wrap a rvalue automatically
#define StaticArray_AddValue(obj, value) (obj == NULL ? -1 : (((obj)->typeSize == sizeof(value)) ? \
    StaticArray_Add(obj, ARG_CONVERT(value)) : \
    (RuntimeErrorLog(InvalidParameterType, TypeErrorMsg,\
    (obj)->typeSize, sizeof(value)), -1)))


#if __STDC_VERSION__ > 201710L

/// A helper version of StaticArray_Insert. It will wrap a rvalue automatically
#define StaticArray_InsertValue(obj, value, index) (obj == NULL ? false : (((obj)->typeSize == sizeof(value)) ? \
    StaticArray_Insert(obj, ARG_CONVERT(value), index) : \
    (RuntimeErrorLog(InvalidParameterType, TypeErrorMsg,\
    (obj)->typeSize, sizeof(value)), false)))

/// A helper version of StaticArray_RemoveElement. It will wrap a rvalue automatically
#define StaticArray_RemoveValue(obj, value) (obj == NULL ? false : (((obj)->typeSize == sizeof(value)) ? \
    StaticArray_RemoveElement(obj, ARG_CONVERT(value)) : \
    (RuntimeErrorLog(InvalidParameterType, TypeErrorMsg,\
    (obj)->typeSize, sizeof(value)), false)))

#endif

#endif //TEST_STATICARRAY_H

