//
// Created by 18030 on 2024/11/15.
//

#ifndef TEST_TYPEENUM_H
#define TEST_TYPEENUM_H

#include <stddef.h>

#define INTV(x) &(int){x}
#define FLTV(x) &(float){x}
#define DBLV(x) &(double){x}
#define CHRV(x) &(char){x}
#define LNGV(x) &(long long){x}
#define ULNGV(x) &(unsigned long long){x}
#define CUSV(type, ...) &(type){x __VA_OPT__(, __VA_ARGS__)}

typedef bool Predicate(void*, void*);

typedef int Comparator(void*, void*);

enum DataType
{
    kUNKNOWN = -1,
    kUCHAR,
    kCHAR,
    kUSHORT,
    kSHORT,
    kUINT,
    kINT,
    kULLONG,
    kPOINTER = kULLONG,
    kLLONG,
    kFLOAT,
    kDOUBLE,
    kSTRING,
    kPAIR,
    kCUSTOM,

    kBOOL
};

typedef enum DataType DataType;

extern size_t gTypeSize[];

static bool IsConvertibleToInt(const DataType type)
{
    return type == kINT || type == kCHAR || type == kSHORT || type == kUSHORT
           || type == kUCHAR;
}

static bool IsConvertibleToDouble(const DataType type)
{
    return type == kFLOAT || type == kDOUBLE;
}


#endif //TEST_TYPEENUM_H
