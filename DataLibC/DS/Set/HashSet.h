//
// Created by 18030 on 2024/11/23.
//

#ifndef TEST_HASHSET_H
#define TEST_HASHSET_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/DS/Hash/hash_table.h"

/**
 * @brief A standard container composed of unique keys (containing
 * at most one of each key value) in which the elements' keys are
 * the elements themselves.
 */
struct LIB_STRUCT HashSet
{
    PRIVATE HashTable* m_HashTable;
    READ_ONLY size_t size;
};

typedef struct HashSet HashSet;

API_MODULE bool HashSet_Init(HashSet* set, DataType type, Hash* hash, Predicate* equal);

API_MODULE void HashSet_Insert(HashSet* set, void* key);

API_MODULE float HashSet_LoadFactor(HashSet* set);

API_MODULE float HashSet_MaxLoadFactor(HashSet* set);

API_MODULE size_t HashSet_BucketCount(HashSet* set);

API_MODULE size_t HashSet_BucketSize(HashSet* set, size_t index);

API_MODULE size_t HashSet_SetBucketSize(HashSet* set, size_t index, size_t size);

API_MODULE bool HashSet_Empty(HashSet* set);

API_MODULE void HashSet_Clear(HashSet* set);

API_MODULE bool HashSet_Contains(HashSet* set, void* key);

API_MODULE void HashSet_Detroy(HashSet* set);


#endif //TEST_HASHSET_H
