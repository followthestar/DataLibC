//
// Created by 18030 on 2024/11/23.
//

#include <malloc.h>
#include <memory.h>
#include "hash_table_internal.h"
#include "DataLibC/DS/DefaultOperation.h"
#include "DataLibC/DS/Algorithm.h"

static constexpr size_t _hash_table_increment = 1;

typedef struct DETAIL _typed_node
{
    DataType type;
    void* data;
} _typed_node;

typedef struct DETAIL _hash_value
{
    Key _key;
    Value _val;
} _hash_value;


typedef struct DETAIL _hash_node
{
    _hash_table* _tbl;
    _hash_value _value;
    struct _hash_node* _nxt;
} _hash_node;

typedef struct DETAIL _hash_table
{
    PRIVATE bucket_ptr m_Buckets;
    PRIVATE size_type m_BucketCount;
    PRIVATE size_type m_ElementCount;
    PRIVATE hasher m_Hash;
    PRIVATE pred m_Equal;
    PRIVATE DataType m_KeyType;
    PRIVATE DataType m_ValueType;
    PRIVATE float m_MaxLoadFactor;
} HashTable;

static _typed_node _wrap_typed_node(DataType type, void* value)
{
    _typed_node node = {type, NULL};
    //TODO: We need to precoess user type here with constructor...
    size_t size = gTypeSize[type];
    if (!value)
        return node;
    node.data = malloc(size);
    memcpy(node.data, value, size);
    return node;
}

static void _hash_node_free(node_ptr node)
{
    if (node->_value._key.data)
        free(node->_value._key.data);
    if (node->_value._val.data)
        free(node->_value._val.data);
    free(node);
}

node_ptr _gen_hash_table_node(_hash_table* tbl, DataType keyType, DataType valueType, void* key, void* value)
{
    node_ptr result = malloc(sizeof(_hash_node));
    if (!result)
        return NULL;
    result->_tbl = tbl;
    result->_nxt = NULL;
    result->_value._key = _wrap_typed_node(keyType, key);
    result->_value._val = _wrap_typed_node(valueType, value);
    return result;
}

bool _init_hash_table(HashTable** hash_table, Hash* hash, Predicate* equal, DataType keyType, DataType valueType)
{
    *hash_table = malloc(sizeof(_hash_table));
    auto ht = *hash_table;
    if (!ht)
        return false;
    ht->m_KeyType = keyType;
    ht->m_ValueType = valueType;

    ht->m_Hash = hash;
    ht->m_Equal = equal;
    ht->m_ElementCount = 0;
    ht->m_MaxLoadFactor = 1.0f;
    if (!hash)
        ht->m_Hash = GetBuiltinHash(keyType);
    if (!equal)
        ht->m_Equal = GetBuiltinEqual(keyType);

    auto bucket = &ht->m_Buckets;
    *bucket = malloc(sizeof(node_ptr));
    if (*bucket == NULL)
        return false;
    (*bucket)[0] = NULL;
    ht->m_BucketCount = 1;

    return true;
}

static ssize_t _hash_table_key_in_bucket(HashTable* hash_table, size_t bkt, void* key)
{
    auto _bkt = hash_table->m_Buckets[bkt];
    if (!_bkt)
        return -1;
    auto iter = _bkt->_nxt;
    ssize_t pos = 0;
    while (iter)
    {
        if (hash_table->m_Equal(iter->_value._key.data, key))
            return pos;
        pos++;
        iter = iter->_nxt;
    }
    return -1;
}

static void _hash_table_bucket_append(HashTable* hash_table, node_ptr* bucket, node_ptr newNode)
{
    if (*bucket == NULL)
        *bucket = _gen_hash_table_node(hash_table, hash_table->m_KeyType, hash_table->m_ValueType, NULL, NULL);
    auto _bkt = *bucket;
    newNode->_nxt = _bkt->_nxt;
    _bkt->_nxt = newNode;
    hash_table->m_ElementCount++;
}

static void _hash_table_rebuild(HashTable* hash_table)
{
    size_t ext = hash_table->m_BucketCount * 2 + _hash_table_increment;
    ext = NextPrime(ext);
    bucket_ptr _new_bucket_ptr = malloc(sizeof(node_ptr) * ext);
    memset(_new_bucket_ptr, 0, sizeof(node_ptr) * ext);

    bucket_ptr _old_bucket_ptr = hash_table->m_Buckets;
    hash_table->m_Buckets = _new_bucket_ptr;
    size_t _bkt;
    for (int i = 0; i < hash_table->m_BucketCount; i++)
    {
        auto item = _old_bucket_ptr[i] ? _old_bucket_ptr[i]->_nxt : NULL;
        if (item)
        {
            _bkt = hash_table->m_Hash(item->_value._key.data) % ext;
            if (!_new_bucket_ptr[_bkt])
                _new_bucket_ptr[_bkt] = _old_bucket_ptr[i];
            else
            {
                item->_nxt = _new_bucket_ptr[_bkt]->_nxt;
                _new_bucket_ptr[_bkt]->_nxt = item;
                free(_old_bucket_ptr[i]);
            }
        }
        if (_old_bucket_ptr[i] && !_old_bucket_ptr[i]->_nxt)
            _hash_node_free(_old_bucket_ptr[i]);
    }
    hash_table->m_BucketCount = ext;

    free(_old_bucket_ptr);
}

static void _hash_table_insert_internal(HashTable* hash_table, void* key, node_ptr node)
{
    float _factor = hash_table->m_MaxLoadFactor * (float) hash_table->m_BucketCount;
    size_t _hk = hash_table->m_Hash(key);
    size_t _bkt = _hk % hash_table->m_BucketCount;

    if (_factor <= (float) hash_table->m_ElementCount)
    {
        _hash_table_rebuild(hash_table);
        _bkt = _hk % hash_table->m_BucketCount;
    }

    _hash_table_bucket_append(hash_table, &hash_table->m_Buckets[_bkt], node);
}

bool _hash_table_insert(HashTable* hash_table, void* key, void* value)
{
    if (_hash_table_contains(hash_table, key))
        return false;
    auto _node_gen = _gen_hash_table_node(
            hash_table, hash_table->m_KeyType,
            hash_table->m_ValueType, key, value);
    if (!_node_gen)
        return false;
    _hash_table_insert_internal(hash_table, key, _node_gen);

    return true;
}

float _hash_table_load_factor(HashTable* hash_table)
{
    return (float) hash_table->m_ElementCount / (float) hash_table->m_BucketCount;
}

float _hash_table_max_load_factor(HashTable* hash_table)
{
    return hash_table->m_MaxLoadFactor;
}

bool _hash_table_empty(HashTable* hash_table)
{
    return hash_table->m_ElementCount == 0;
}

size_t _hash_table_bucket_count(HashTable* hash_table)
{
    return hash_table->m_BucketCount;
}

bool _hash_table_contains(HashTable* hash_table, void* key)
{
    size_t _bkt = hash_table->m_Hash(key);
    _bkt %= hash_table->m_BucketCount;
    return _hash_table_key_in_bucket(hash_table, _bkt, key) != -1;
}

void _hash_table_destroy(HashTable* hash_table)
{
    for (int i = 0; i < hash_table->m_BucketCount; i++)
    {

        auto iter = hash_table->m_Buckets[i] ? hash_table->m_Buckets[i]->_nxt : NULL;
        typeof(iter) next;
        while (iter)
        {
            next = iter->_nxt;
            _hash_node_free(iter);
            iter = next;
        }
    }
    free(hash_table->m_Buckets);
    free(hash_table);
}

size_t _hash_table_bucket_size(HashTable* hash_table, size_t index)
{
    if (index > hash_table->m_BucketCount)
        return 0;
    size_t count = 0;
    auto iter = hash_table->m_Buckets[index] ? hash_table->m_Buckets[index]->_nxt : NULL;
    while (iter)
    {
        count++;
        iter = iter->_nxt;
    }
    return count;
}
