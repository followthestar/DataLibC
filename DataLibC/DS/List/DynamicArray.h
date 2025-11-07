//
// Created by 18030 on 2024/11/17.
//

#ifndef TEST_DYNAMICARRAY_H
#define TEST_DYNAMICARRAY_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/DS/TypeEnum.h"
#include "DataLibC/Tool/TypeMacro.h"

struct LIB_STRUCT DynamicArray
{
    PRIVATE DataType m_Type;
    PRIVATE size_t m_TypeSize;

    READ_ONLY size_t length;
    READ_ONLY size_t capacity;

    PRIVATE Byte* m_Array;
    PRIVATE bool m_Dirty;
    PRIVATE const char* m_FormatString;
    PRIVATE volatile size_t m_Validate;

};

/// Store a dynamic array of elements.
/// \attention DynamicArray is not supported non-trivial types.
typedef struct DynamicArray DynamicArray;

/// Initialize a DynamicArray object with given initial values.
/// \param arr The object to be initialized.
/// \param initSize The initial length of the array. Must match the amount of va_args.
/// \param type The element type stored in the container.
/// \param typeSize The size of the element type. Zero is supported for Builtin types.
/// \param ... Initial values
/// \return True if succeed, otherwise false.
API_MODULE bool DynamicArray_Init(DynamicArray* arr, size_t initSize, DataType type, size_t typeSize, ...);

/// Add a value at the end of the DynamicArray.
/// \param arr The array object.
/// \param value The value to be added.
API_MODULE void DynamicArray_Add(DynamicArray* arr, void* value);

/// Remove the element at index in the DynamicArray.
/// \param arr The array object.
/// \param index The element index.
/// \return True if succeed, otherwise false.
API_MODULE bool DynamicArray_RemoveAt(DynamicArray* arr, size_t index);

/// Insert into the DynamicArray at index.
/// \param arr The array object.
/// \param index The index within [0, length].
/// \param value The value to be inserted.
/// \return True if succeed, otherwise false.
API_MODULE bool DynamicArray_InsertAt(DynamicArray* arr, size_t index, void* value);

API_MODULE void* DynamicArray_ValueAt(DynamicArray* arr, size_t index);

API_MODULE void DynamicArray_Clear(DynamicArray* arr);

#endif //TEST_DYNAMICARRAY_H
