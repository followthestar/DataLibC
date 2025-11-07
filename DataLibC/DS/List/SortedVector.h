//
// Created by 18030 on 2024/11/19.
//

#ifndef TEST_SORTEDVECTOR_H
#define TEST_SORTEDVECTOR_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/DS/List/DynamicArray.h"

typedef DynamicArray Container;

typedef int Comparator(void* lhs, void* rhs);

typedef bool Predicate(void* lhs, void* rhs);

struct LIB_STRUCT SortedVector
{
    PRIVATE Container m_Container;
    PRIVATE Predicate* m_Pred;
    READ_ONLY size_t length;
};

typedef struct SortedVector SortedVector;

/// Initialize a SortedVector object with given initial values.
/// \param vec The vector object.
/// \param pred The binary predicate function. (NULL means using default)
/// \param cmp The comparator. (NULL means using default)
/// \param initSize The initial length of the SortedVector. Must match the amount of va_args
/// \param type The type of elements stored in the container.
/// \param ... The initial values.
/// \return True if succeed, otherwise false.
API_MODULE bool
SortedVector_Init(SortedVector* vec, Predicate* pred, size_t initSize, DataType type, ...);

/// Add a value to the SortedVector.
/// \param vec The vector object.
/// \param value The value to be added.
API_MODULE void SortedVector_Add(SortedVector* vec, void* value);

/// Find a value in the SortedVector.
/// \param vec The vector object.
/// \param value The value to be found.
/// \return If found, return its index in the container, other wise the length of the container.
API_MODULE ssize_t SortedVector_Find(SortedVector* vec, void* value);

API_MODULE const void* SortedVector_ElementAt(SortedVector* vec, size_t index);

API_MODULE void SortedVector_RemoveAt(SortedVector* vec, size_t index);

API_MODULE void SortedVector_Synchronize(SortedVector* vec);

#endif //TEST_SORTEDVECTOR_H
