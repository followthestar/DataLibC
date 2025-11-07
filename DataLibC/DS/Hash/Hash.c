//
// Created by 18030 on 2024/11/23.
//

#include "Hash.h"

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long long ulonglong;
typedef long long longlong;

#define _hash_default_impl_integer(x) \
static size_t _hash_##x(void* value){return (size_t)(*(x*)value);}

_hash_default_impl_integer(int)

_hash_default_impl_integer(short)

_hash_default_impl_integer(char)

_hash_default_impl_integer(unsigned)

_hash_default_impl_integer(ushort)

_hash_default_impl_integer(uchar)

_hash_default_impl_integer(ulonglong)

_hash_default_impl_integer(longlong)

#define _hash(x) _hash_##x


static size_t _hash_float(void* value)
{

}

static size_t _hash_double(void* value)
{

}

static size_t _hash_string(void* value)
{

}


_hashImpl _defaultHash = {
        _hash(uchar), _hash(char), _hash(ushort), _hash(short),
        _hash(unsigned), _hash(int), _hash(ulonglong), _hash(longlong),
        _hash(float), _hash(double), _hash(string)
};


Hash* GetBuiltinHash(size_t type)
{
    return _defaultHash[type];
}
