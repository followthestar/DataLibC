//
// Created by 18030 on 2024/11/15.
//

#include "DefaultOperation.h"
#include "DataLibC/DS/Pair/Pair.h"

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long long ulonglong;
typedef long long longlong;

#define _builtin_equal_impl(x) \
static bool _default_equal_##x(void* lhs, void* rhs) \
{return *(x*)lhs == *(x*)rhs;}

#define _builtin_equal(x)   _default_equal_##x

static bool _DefaultUnsignedIntPred(void* lhs, void* rhs)
{
    return *(unsigned*) lhs < *(unsigned*) rhs;
}

static bool _DefaultIntPred(void* lhs, void* rhs)
{
    return *(int*) lhs < *(int*) rhs;
}

static bool _DefaultFloatPred(void* lhs, void* rhs)
{
    return *(float*) lhs < *(float*) rhs;
}

static bool _DefaultDoublePred(void* lhs, void* rhs)
{
    return *(double*) lhs < *(double*) rhs;
}

static bool _DefaultLongLongPred(void* lhs, void* rhs)
{
    return *(long long*) lhs < *(long long*) rhs;
}

static bool _DefaultShortPred(void* lhs, void* rhs)
{
    return *(short*) lhs < *(short*) rhs;
}

static bool _DefaultCharPred(void* lhs, void* rhs)
{
    return *(char*) lhs < *(char*) rhs;
}

static bool _DefaultUnsignedCharPred(void* lhs, void* rhs)
{
    return *(unsigned char*) lhs < *(unsigned char*) rhs;
}

static bool _DefaultUnsignedLongLongPred(void* lhs, void* rhs)
{
    return *(unsigned long long*) lhs < *(unsigned long long*) rhs;
}

static bool _DefaultUnsignedShortPred(void* lhs, void* rhs)
{
    return *(unsigned short*) lhs < *(unsigned short*) rhs;
}

static bool _DefaultStringLess(void* lhs, void* rhs)
{
    static char* _l;
    static char* _r;
    _l = lhs;
    _r = rhs;
    while (*_l && *_r == *_l)
        _l++, _r++;
    return *_l < *_r;
}

static bool _DefaultPairLess(void* lhs, void* rhs)
{
    return GetBuiltinLess(FirstType(*(Pair*) (lhs)))(First(*(Pair*) (lhs)), First(*(Pair*) (rhs)));
}

static int _DefaultUnsignedIntComp(void* lhs, void* rhs)
{
    if (_DefaultUnsignedIntPred(lhs, rhs))
        return -1;
    if (_DefaultUnsignedIntPred(rhs, lhs))
        return 1;
    return 0;
}

static int _DefaultIntComp(void* lhs, void* rhs)
{
    if (_DefaultIntPred(lhs, rhs))
        return -1;
    if (_DefaultIntPred(rhs, lhs))
        return 1;
    return 0;
}

static int _DefaultFloatComp(void* lhs, void* rhs)
{
    if (_DefaultFloatPred(lhs, rhs))
        return -1;
    if (_DefaultFloatPred(rhs, lhs))
        return 1;
    return 0;
}

static int _DefaultDoubleComp(void* lhs, void* rhs)
{
    if (_DefaultDoublePred(lhs, rhs))
        return -1;
    if (_DefaultDoublePred(rhs, lhs))
        return 1;
    return 0;
}

static int _DefaultLongLongComp(void* lhs, void* rhs)
{
    if (_DefaultLongLongPred(lhs, rhs))
        return -1;
    if (_DefaultLongLongPred(rhs, lhs))
        return 1;
    return 0;
}

static int _DefaultShortComp(void* lhs, void* rhs)
{
    if (_DefaultShortPred(lhs, rhs))
        return -1;
    if (_DefaultShortPred(rhs, lhs))
        return 1;
    return 0;
}

static int _DefaultCharComp(void* lhs, void* rhs)
{
    if (_DefaultCharPred(lhs, rhs))
        return -1;
    if (_DefaultCharPred(rhs, lhs))
        return 1;
    return 0;
}

static int _DefaultUnsignedCharComp(void* lhs, void* rhs)
{
    if (_DefaultUnsignedCharPred(lhs, rhs))
        return -1;
    if (_DefaultUnsignedCharPred(rhs, lhs))
        return 1;
    return 0;
}

static int _DefaultUnsignedLongLongComp(void* lhs, void* rhs)
{
    if (_DefaultUnsignedLongLongPred(lhs, rhs))
        return -1;
    if (_DefaultUnsignedLongLongPred(rhs, lhs))
        return 1;
    return 0;
}

static int _DefaultUnsignedShortComp(void* lhs, void* rhs)
{
    if (_DefaultUnsignedShortPred(lhs, rhs))
        return -1;
    if (_DefaultUnsignedShortPred(rhs, lhs))
        return 1;
    return 0;
}

static int _DefaultUnsignedPairComp(void* lhs, void* rhs)
{
    if (_DefaultPairLess(lhs, rhs))
        return -1;
    if (_DefaultPairLess(rhs, lhs))
        return 1;
    return 0;
}

_builtin_equal_impl(char)

_builtin_equal_impl(short)

_builtin_equal_impl(int)

_builtin_equal_impl(longlong)

_builtin_equal_impl(unsigned)

_builtin_equal_impl(uchar)

_builtin_equal_impl(ushort)

_builtin_equal_impl(ulonglong)

_builtin_equal_impl(float)

_builtin_equal_impl(double)

static bool _default_equal_string(void* lhs, void* rhs)
{
    static char* _l;
    static char* _r;
    _l = lhs;
    _r = rhs;
    while (*_l && *_r == *_l)
        _l++, _r++;
    return *_l == *_r;
}


Predicate* gDefaultLess[] = {
        _DefaultUnsignedCharPred, _DefaultCharPred, _DefaultUnsignedShortPred,
        _DefaultShortPred, _DefaultUnsignedIntPred, _DefaultIntPred,
        _DefaultUnsignedLongLongPred, _DefaultLongLongPred, _DefaultFloatPred,
        _DefaultDoublePred, _DefaultStringLess, _DefaultPairLess, NULL
};

Comparator* gDefaultComparator[] = {
        _DefaultUnsignedCharComp, _DefaultCharComp, _DefaultUnsignedShortComp,
        _DefaultShortComp, _DefaultUnsignedIntComp, _DefaultIntComp,
        _DefaultUnsignedLongLongComp, _DefaultLongLongComp, _DefaultFloatComp,
        _DefaultDoubleComp, NULL, _DefaultUnsignedPairComp, NULL
};

Predicate* gDefaultEqual[] = {
        _builtin_equal(uchar), _builtin_equal(char), _builtin_equal(ushort),
        _builtin_equal(short), _builtin_equal(unsigned), _builtin_equal(int),
        _builtin_equal(ulonglong), _builtin_equal(longlong), _builtin_equal(float),
        _builtin_equal(double), _builtin_equal(string)
};

Predicate* GetBuiltinEqual(size_t type)
{
    return gDefaultEqual[type];
}

Predicate* GetBuiltinLess(size_t type)
{
    return gDefaultLess[type];
}
