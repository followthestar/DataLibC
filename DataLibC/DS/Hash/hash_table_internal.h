//
// Created by 18030 on 2024/11/24.
//

#ifndef TEST_HASH_TABLE_INTERNAL_H
#define TEST_HASH_TABLE_INTERNAL_H

#include "hash_table.h"

bool _init_hash_table(HashTable** hash_table, Hash* hash, Predicate* equal, DataType keyType, DataType valueType);

bool _hash_table_insert(HashTable* hash_table, void* key, void* value);

float _hash_table_load_factor(HashTable* hash_table);

float _hash_table_max_load_factor(HashTable* hash_table);

bool _hash_table_empty(HashTable* hash_table);

size_t _hash_table_bucket_count(HashTable* hash_table);

bool _hash_table_contains(HashTable* hash_table, void* key);

void _hash_table_destroy(HashTable* hash_table);

size_t _hash_table_bucket_size(HashTable* hash_table, size_t index);

#endif //TEST_HASH_TABLE_INTERNAL_H
