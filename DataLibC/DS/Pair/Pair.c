//
// Created by 18030 on 2024/12/7.
//

#include <malloc.h>
#include <stdio.h>
#include "Pair.h"
#include "DataLibC/DS/Allocation.h"


typedef struct LIB_STRUCT TValue
{
    DataType _M_type;
    void* _M_value;
    Constructor* _M_constructor;
    Destructor* _M_destructor;
} TValue;

Pair
MakePair(DataType firstType, const void* first, DataType secondType, Constructor* ctor, Destructor* dtor, void* second)
{
    Pair result;
    TValue* firstT = malloc(sizeof(TValue));
    firstT->_M_type = firstType;
    if (first)
        firstT->_M_value = GetBuiltinCtor(firstType)(first);
    else
        firstT->_M_value = NULL;

    TValue* secondT = malloc(sizeof(TValue));
    secondT->_M_type = secondType;
    secondT->_M_constructor = ctor;
    secondT->_M_destructor = dtor;
    if (second)
    {
        if (ctor)
            secondT->_M_value = ctor(second);
        else
        {
            if (secondType == kCUSTOM)
            {
                fprintf(stderr, "Custom type must be constructed by constructor!\n");
                secondT->_M_value = NULL;
            }
            else
                secondT->_M_value = GetBuiltinCtor(secondType)(second);
        }
    }
    else
        secondT->_M_value = NULL;
    result.first = firstT;
    result.second = secondT;
    return result;
}

DataType FirstType(Pair pair)
{
    return pair.first->_M_type;
}

DataType SecondType(Pair pair)
{
    return pair.first->_M_type;
}

void* First(Pair pair)
{
    return pair.first->_M_value;
}

void* Second(Pair pair)
{
    return pair.second->_M_value;
}

void FreePair(Pair pair)
{
    if (pair.first)
    {
        if (pair.first->_M_value)
            free(pair.first->_M_value);
        free(pair.first);
    }
    if (pair.second)
    {
        if (pair.second->_M_value)
        {
            if (pair.second->_M_destructor)
                pair.second->_M_destructor(pair.second->_M_value);
            else
                free(pair.second->_M_value);
        }
        free(pair.second);
    }
}
