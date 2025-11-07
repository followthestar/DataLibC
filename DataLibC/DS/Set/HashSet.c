//
// Created by 18030 on 2024/11/23.
//

#include <assert.h>
#include <malloc.h>
#include "HashSet.h"
#include "DataLibC/DS/Hash/hash_table_internal.h"
#include "DataLibC/Tool/LogMacro.h"

bool HashSet_Init(HashSet* set, DataType type, Hash* hash, Predicate* equal)
{
    assert(set);
    if (!_init_hash_table(&set->m_HashTable, hash, equal, type, type))
    {
        RuntimeErrorLog(InitializationFailed, "Failed to init a HashSet!");
        return false;
    }
    set->size = 0;
    return true;
}

void HashSet_Insert(HashSet* set, void* key)
{
    assert(set);
    assert(key);
    if (_hash_table_insert(set->m_HashTable, key, NULL))
        set->size++;
}

float HashSet_MaxLoadFactor(HashSet* set)
{
    assert(set);
    return _hash_table_max_load_factor(set->m_HashTable);
}

float HashSet_LoadFactor(HashSet* set)
{
    assert(set);
    return _hash_table_load_factor(set->m_HashTable);
}

bool HashSet_Empty(HashSet* set)
{
    assert(set);
    return _hash_table_empty(set->m_HashTable);
}

size_t HashSet_BucketCount(HashSet* set)
{
    assert(set);
    return _hash_table_bucket_count(set->m_HashTable);
}

bool HashSet_Contains(HashSet* set, void* key)
{
    assert(set);
    return _hash_table_contains(set->m_HashTable, key);
}

void HashSet_Detroy(HashSet* set)
{
    assert(set);
    _hash_table_destroy(set->m_HashTable);
    set->m_HashTable = NULL;
}

size_t HashSet_BucketSize(HashSet* set, size_t index)
{
    assert(set);
    return _hash_table_bucket_size(set->m_HashTable, index);
}
