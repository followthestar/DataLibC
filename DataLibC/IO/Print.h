//
// Created by 18030 on 2025/9/19.
//

#ifndef PRINT_H
#define PRINT_H

#include "DataLibC/Tool/Magic.h"
#include "DataLibC/DS/TypeEnum.h"

#define TYPE_INFO(var) _Generic(var,\
    int: kINT,\
    unsigned int: kUINT,\
    float: kFLOAT,\
    double: kDOUBLE,\
    char: kCHAR,\
    unsigned char: kUCHAR,\
    short: kSHORT,\
    unsigned short: kUSHORT,\
    long long: kLLONG,\
    unsigned long long: kULLONG,\
    bool: kBOOL,\
    char*: kSTRING,\
    const char*: kSTRING,\
    void*: kPOINTER,\
    float*: kPOINTER,\
    int*: kPOINTER,\
    double*: kPOINTER,\
    bool*: kPOINTER,\
    const void*: kPOINTER,\
    default: kCUSTOM)

#define GET_TYPE(var) TYPE_INFO(var), (var)


API_MODULE void PrintAnyImpl(int size, ...);

API_MODULE void PrintFmtImpl(int size, ...);

/**
 * @brief Used to output any object to the standard output stream without using obvious placeholder.
 */
#define print_any(...) \
do{             \
if(sizeof(#__VA_ARGS__) == 1) println();\
else PrintAnyImpl(COUNT_ARG(__VA_ARGS__) __VA_OPT__( , EXPAND_ARGS(GET_TYPE, __VA_ARGS__)));\
}while(0)

#define print_fmt(...) \
    do{\
    if(sizeof(#__VA_ARGS__) == 1) println();\
    else PrintFmtImpl(COUNT_ARG(__VA_ARGS__) __VA_OPT__(, EXPAND_ARGS(GET_TYPE, __VA_ARGS__)));\
}while (0)

#endif //PRINT_H
