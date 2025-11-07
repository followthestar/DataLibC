//
// Created by 18030 on 2024/11/23.
//

#ifndef TEST_HASH_TABLE_H
#define TEST_HASH_TABLE_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/DS/TypeEnum.h"
#include "Hash.h"

typedef struct DETAIL _typed_node _typed_node;

typedef _typed_node Key;
typedef _typed_node Value;

typedef struct DETAIL _hash_value _hash_value;


typedef struct DETAIL _hash_node _hash_node;

typedef _hash_node* node_ptr;
typedef node_ptr* bucket_ptr;
typedef size_t size_type;
typedef Hash* hasher;
typedef Predicate* pred;

typedef struct DETAIL _hash_table _hash_table;

typedef _hash_table HashTable;


#endif //TEST_HASH_TABLE_H
