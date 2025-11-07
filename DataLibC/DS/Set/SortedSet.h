//
// Created by 18030 on 2024/11/22.
//

#ifndef TEST_SORTEDSET_H
#define TEST_SORTEDSET_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/DS/List/SortedVector.h"


struct LIB_STRUCT SortedSet
{
    PRIVATE SortedVector m_Container;
    READ_ONLY size_t size;
};

typedef struct SortedSet SortedSet;

API_MODULE bool SortedSet_Init(SortedSet* set, DataType type, Predicate* pred);

API_MODULE bool SortedSet_Insert(SortedSet* set, void* value);

API_MODULE bool SortedSet_Contains(SortedSet* set, void* value);

API_MODULE bool SortedSet_Erase(SortedSet* set, void* value);

/// \brief Synchronize the set with the underlying container.
/// \param set The set to synchronize.
/// \attention After removing elements through the iterator, please remember to call this
/// function to synchronize the set. Otherwise, the element count will be incorrect.
API_MODULE void SortedSet_Synchronize(SortedSet* set);

#endif //TEST_SORTEDSET_H
