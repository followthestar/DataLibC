//
// Created by 18030 on 2025/2/26.
//

#ifndef DATALIBC_TREEMAP_H
#define DATALIBC_TREEMAP_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/DS/Tree/tree.h"
#include "DataLibC/DS/TypeEnum.h"
#include "DataLibC/DS/Allocation.h"
#include <stddef.h>

struct LIB_STRUCT TreeMap
{
    PRIVATE _Rb_tree* _M_tree;
    PRIVATE DataType _M_keyType;
    PRIVATE DataType _M_valueType;
    READ_ONLY Constructor* _M_valueCtor;
    READ_ONLY Destructor* _M_valueDtor;
    READ_ONLY size_t size;
};

/// Used to store key-value pairs with RBTree.
typedef struct TreeMap TreeMap;

API_MODULE void TreeMap_Init(
        TreeMap* map, DataType keyType, DataType valueType, Constructor* valueCtor, Destructor* valueDtor,
        Predicate* comp);

API_MODULE bool TreeMap_Insert(TreeMap* map, const void* key, void* value);

API_MODULE bool TreeMap_Contains(TreeMap* map, void* value);

API_MODULE void* TreeMap_ValueAt(const TreeMap* map, const void* key);

API_MODULE void TreeMap_Destroy(TreeMap* map);

API_MODULE void TreeMap_Clear(TreeMap* map);

API_MODULE bool TreeMap_Erase(TreeMap* map, void* key);

#endif //DATALIBC_TREEMAP_H
