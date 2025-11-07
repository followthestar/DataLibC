//
// Created by 18030 on 2024/12/6.
//

#ifndef TEST_SORTEDMAP_H
#define TEST_SORTEDMAP_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/DS/List/SortedVector.h"
#include "DataLibC/DS/Allocation.h"

typedef struct Pair Pair;

struct LIB_STRUCT SortedMap
{
    PRIVATE SortedVector m_Container;
    READ_ONLY size_t size;
    READ_ONLY const DataType m_KeyType;
    READ_ONLY const DataType m_ValueType;
    READ_ONLY Constructor* m_ValueCtor;
    READ_ONLY Destructor* m_ValueDtor;
};

typedef struct SortedMap SortedMap;

/// Initialize a SortedMap object
/// \param map The map object
/// \param keyType The type of KEY stored in the map
/// \param valueType The type of VALUE stored in the map
/// \param keyLess A predicate which presents the less-relationship between two keys. It must be a binary function and
/// you need to process the comparison on the first member variable of the param rather the param self because the param
/// should be converted to Pair (which has two members 'first' and 'second') type safely. (NULL means using default)
/// \return True if succeed, otherwise false
API_MODULE bool SortedMap_Init(
        SortedMap* map, DataType keyType, DataType valueType, Constructor* valueCtor, Destructor* valueDtor,
        Predicate* keyLess);

API_MODULE bool SortedMap_Insert(SortedMap* map, void* key, void* value);

API_MODULE bool SortedMap_Contains(SortedMap* map, void* key);

API_MODULE bool SortedMap_SetValue(SortedMap* map, void* key, void* value);

API_MODULE const Pair* SortedMap_Find(SortedMap* map, void* key);

API_MODULE void* SortedMap_KeyAt(SortedMap* map, void* key);

#endif //TEST_SORTEDMAP_H
