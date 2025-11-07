//
// Created by 18030 on 2024/12/7.
//

#include "Allocation.h"
#include "DataLibC/DS/Pair/Pair.h"
#include <string.h>
#include <stdlib.h>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long long ulonglong;
typedef long long longlong;

#define _builtin_ctor_impl(x) \
static void* _default_##x##_ctor(const void* value) \
{                                                                           \
    x* v = malloc(sizeof(x));                                               \
    *v = *(x*)value;                                                        \
    return v;                                                               \
}

#define _builtin_ctor(x) _default_##x##_ctor

_builtin_ctor_impl(char)

_builtin_ctor_impl(short)

_builtin_ctor_impl(int)

_builtin_ctor_impl(longlong)

_builtin_ctor_impl(uchar)

_builtin_ctor_impl(ushort)

_builtin_ctor_impl(unsigned)

_builtin_ctor_impl(ulonglong)

_builtin_ctor_impl(float)

_builtin_ctor_impl(double)

void* _default_string_ctor(const void* str)
{
    if (!str)
        return NULL;
    return strdup(str);
}

_builtin_ctor_impl(Pair)


#define _builtin_dtor_impl(x) static void _default_##x##_dtor(void* p){free(p);}

#define _builtin_dtor(x) _default_##x##_dtor

_builtin_dtor_impl(char)

_builtin_dtor_impl(short)

_builtin_dtor_impl(int)

_builtin_dtor_impl(longlong)

_builtin_dtor_impl(uchar)

_builtin_dtor_impl(ushort)

_builtin_dtor_impl(unsigned)

_builtin_dtor_impl(ulonglong)

_builtin_dtor_impl(float)

_builtin_dtor_impl(double)

_builtin_dtor_impl(string)

static void _default_Pair_dtor(void* p)
{
    FreePair(*(Pair*) p);
}

_builtin_dtor_impl(Custom)

Constructor* gDefaultCtor[] = {
        _builtin_ctor(uchar), _builtin_ctor(char), _builtin_ctor(ushort),
        _builtin_ctor(short), _builtin_ctor(unsigned), _builtin_ctor(int),
        _builtin_ctor(ulonglong), _builtin_ctor(longlong), _builtin_ctor(float),
        _builtin_ctor(double), _builtin_ctor(string), _builtin_ctor(Pair),
        NULL
};

Destructor* gDefaultDtor[] = {
        _builtin_dtor(uchar), _builtin_dtor(char), _builtin_dtor(ushort),
        _builtin_dtor(short), _builtin_dtor(unsigned), _builtin_dtor(int),
        _builtin_dtor(ulonglong), _builtin_dtor(longlong), _builtin_dtor(float),
        _builtin_dtor(double), _builtin_dtor(string), _builtin_dtor(Pair),
        _builtin_dtor(Custom)
};

Constructor* GetBuiltinCtor(DataType type)
{
    return gDefaultCtor[type];
}

Destructor* GetBuiltinDtor(DataType type)
{
    return gDefaultDtor[type];
}
