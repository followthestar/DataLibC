//
// Created by 18030 on 2024/11/22.
//

#include <assert.h>
#include "SortedSet.h"

bool SortedSet_Init(SortedSet* set, DataType type, Predicate* pred)
{
    assert(set);
    if (!SortedVector_Init(&set->m_Container, pred, 0, type))
        return false;
    set->size = 0;
    return true;
}

bool SortedSet_Insert(SortedSet* set, void* value)
{
    assert(set);
    set->size = set->m_Container.length;
    auto pos = SortedVector_Find(&set->m_Container, value);
    if (pos != -1)
        return false;
    SortedVector_Add(&set->m_Container, value);
    set->size++;
    return true;
}

bool SortedSet_Contains(SortedSet* set, void* value)
{
    assert(set);
    assert(value);
    return SortedVector_Find(&set->m_Container, value) != -1;
}

bool SortedSet_Erase(SortedSet* set, void* value)
{
    assert(set);
    assert(value);
    //TODO

    return 0;
}

void SortedSet_Synchronize(SortedSet* set)
{
    assert(set);
    set->size = set->m_Container.length;
}
