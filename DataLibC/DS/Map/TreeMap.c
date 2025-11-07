//
// Created by 18030 on 2025/2/26.
//

#include <assert.h>
#include "TreeMap.h"
#include "DataLibC/DS/Tree/tree_internal.h"
#include "DataLibC/DS/DefaultOperation.h"
#include "DataLibC/DS/Pair/Pair.h"

void TreeMap_Init(
    TreeMap* map, DataType keyType, DataType valueType, Constructor* valueCtor, Destructor* valueDtor,
    Predicate* comp)
{
    assert(map);
    if (!comp)
        comp = GetBuiltinLess(kPAIR);
    _rb_tree_init_entry_node(&map->_M_tree, comp);
    map->_M_keyType = keyType;
    map->_M_valueType = valueType;
    map->_M_valueCtor = valueCtor;
    map->_M_valueDtor = valueDtor;
    map->size = 0;
}

bool TreeMap_Insert(TreeMap* map, const void* key, void* value)
{
    assert(map);
    Pair pair = MakePair(map->_M_keyType, key, map->_M_valueType,
                         map->_M_valueCtor, map->_M_valueDtor, value);
    if (_rb_tree_insert(map->_M_tree, &pair))
    {
        map->size++;
        return true;
    }
    return false;
}

void* TreeMap_ValueAt(const TreeMap* map, const void* key)
{
    assert(map);
    assert(key);
    Pair pair = MakePair(map->_M_keyType, key, map->_M_valueType,
                         map->_M_valueCtor, map->_M_valueDtor, NULL);
    auto ret = _rb_tree_value_at(map->_M_tree, &pair);
    FreePair(pair);
    return ret;
}

void TreeMap_Destroy(TreeMap* map)
{
    assert(map);
    _rb_tree_destroy(map->_M_tree);
    map->size = 0;
    map->_M_tree = NULL;
}

void TreeMap_Clear(TreeMap* map)
{
    assert(map);
    _rb_tree_clear(map->_M_tree);
    map->size = 0;
}

bool TreeMap_Erase(TreeMap* map, void* key)
{
    assert(map);
    Pair pair = MakePair(map->_M_keyType, key, map->_M_valueType,
                         map->_M_valueCtor, map->_M_valueDtor, NULL);
    bool res = _rb_tree_erase(map->_M_tree, &pair);
    FreePair(pair);
    if (res)
    {
        map->size--;
        return true;
    }
    return false;
}
