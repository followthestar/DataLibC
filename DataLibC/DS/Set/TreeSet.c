//
// Created by 18030 on 2024/11/27.
//

#include <assert.h>
#include "TreeSet.h"
#include "DataLibC/DS/Tree/tree_internal.h"
#include "DataLibC/DS/DefaultOperation.h"

void TreeSet_Init(TreeSet* set, DataType type, Predicate* comp)
{
    assert(set);
    if (!comp)
        comp = gDefaultLess[type];
    _rb_tree_init(&set->_M_tree, type, comp);
    set->size = 0;
}

void TreeSet_Insert(TreeSet* set, void* value)
{
    assert(set);
    if (_rb_tree_insert(set->_M_tree, value))
        set->size++;
}

bool TreeSet_Contains(TreeSet* set, void* value)
{
    assert(set);
    assert(value);
    return _rb_tree_containes(set->_M_tree, value);
}

void TreeSet_Destroy(TreeSet* set)
{
    assert(set);
    _rb_tree_destroy(set->_M_tree);
}

