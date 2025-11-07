//
// Created by 18030 on 2024/11/27.
//

#ifndef TEST_TREESET_H
#define TEST_TREESET_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/DS/Tree/tree.h"
#include "DataLibC/DS/TypeEnum.h"
#include <stddef.h>

struct LIB_STRUCT TreeSet
{
    PRIVATE _Rb_tree* _M_tree;
    READ_ONLY size_t size;
};

typedef struct TreeSet TreeSet;

API_MODULE void TreeSet_Init(TreeSet* set, DataType type, Predicate* comp);

API_MODULE void TreeSet_Insert(TreeSet* set, void* value);

API_MODULE bool TreeSet_Contains(TreeSet* set, void* value);

API_MODULE void TreeSet_Destroy(TreeSet* set);


//Only Test
void TreeSet_InOrderTest(TreeSet* set);


#endif //TEST_TREESET_H
