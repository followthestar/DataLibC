//
// Created by 18030 on 2024/12/6.
//

#include <assert.h>
#include "SortedMap.h"
#include "DataLibC/DS/Pair/Pair.h"

bool SortedMap_Init(SortedMap* map, DataType keyType, DataType valueType, Constructor* valueCtor, Destructor* valueDtor, Predicate* keyLess)
{
    assert(map);
    if (!SortedVector_Init(&map->m_Container, keyLess, 0, kPAIR))
        return false;
    map->size = 0;
    map->m_ValueCtor = valueCtor;
    map->m_ValueDtor = valueDtor;
    INIT_ASSIGN(map, m_KeyType, keyType);
    INIT_ASSIGN(map, m_ValueType, valueType);
    return true;
}

bool SortedMap_Insert(SortedMap* map, void* key, void* value)
{
    assert(map);
    assert(key);
    Pair pair = MakePair(map->m_KeyType, key, map->m_ValueType,
                         map->m_ValueCtor, map->m_ValueDtor, value);
    auto pos = SortedVector_Find(&map->m_Container, &pair);
    if (pos != -1)
    {
        FreePair(pair);
        return false;
    }
    SortedVector_Add(&map->m_Container, &pair);
    map->size++;
    return true;
}

bool SortedMap_Contains(SortedMap* map, void* key)
{
    assert(map);
    assert(key);
    return SortedMap_Find(map, key) != NULL;
}

const Pair* SortedMap_Find(SortedMap* map, void* key)
{
    assert(map);
    Pair pair = MakePair(map->m_KeyType, key, map->m_ValueType,
                         map->m_ValueCtor, map->m_ValueDtor,NULL);
    auto pos = SortedVector_Find(&map->m_Container, &pair);
    if (pos != -1)
    {
        FreePair(pair);
        return SortedVector_ElementAt(&map->m_Container, pos);
    }
    return NULL;
}

bool SortedMap_SetValue(SortedMap* map, void* key, void* value)
{
    assert(map);
    assert(key);
    auto old = (Pair*) SortedMap_Find(map, key);
    Pair pair = MakePair(map->m_KeyType, key, map->m_ValueType,
                         map->m_ValueCtor, map->m_ValueDtor,value);
    if (old)
    {
        FreePair(*old);
        *old = pair;
        return false;
    }

    SortedVector_Add(&map->m_Container, &pair);
    map->size++;
    return true;
}

void* SortedMap_KeyAt(SortedMap* map, void* key)
{
    assert(map);
    auto p = SortedMap_Find(map,key);
    if (p)
        return Second(*p);
    return NULL;
}
