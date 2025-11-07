//
// Created by 18030 on 2024/12/7.
//

#ifndef TEST_PAIR_H
#define TEST_PAIR_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/DS/TypeEnum.h"
#include "DataLibC/DS/Allocation.h"

typedef struct TValue TValue;

struct LIB_STRUCT Pair
{
    PRIVATE TValue* first;
    PRIVATE TValue* second;
};

typedef struct Pair Pair;

API_MODULE Pair
MakePair(DataType firstType, const void* first, DataType secondType, Constructor* ctor, Destructor* dtor, void* second);

API_MODULE DataType FirstType(Pair pair);

API_MODULE DataType SecondType(Pair pair);

API_MODULE void* First(Pair pair);

API_MODULE void* Second(Pair pair);

API_MODULE void FreePair(Pair pair);

typedef struct IntPair
{
    int first;
    int second;
} IntPair;

#endif //TEST_PAIR_H
