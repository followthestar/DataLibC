//
// Created by 18030 on 2024/11/14.
//

#ifndef TEST_LOGMACRO_H
#define TEST_LOGMACRO_H

#include <stdio.h>

enum OperationError
{
    InitializationFailed,
    IndexOutOfRange,
    ReachMaxLength,
    InvalidParameterType,
    InvalidIterator,
    IllegalState,
    IterationOutOfBounds,
    NoSuchValue,
    InvalidArgument
};

enum IOError
{
    OpenFileFailed,
    ReadFileFailed,
    WriteFileFailed,
    InvalidFormat,
    UnknownData
};

enum MemoryError
{
    Uninitialize,
    MallocFailed,
    OutOfMemory,
    MemoryOutOfRange
};

#define TypeErrorMsg "The value size '%zu' is expected but '%zu' is provided!"
#define ERROR_STYLE "\033[38;5;1m"
#define DEFAULT_STYLE "\033[0m"
#define RuntimeErrorLog(type, msg, ...) \
    fprintf(stderr, ERROR_STYLE"%s, Error at %s: %d\n"msg"\n"DEFAULT_STYLE, #type, __FILE__, __LINE__, ##__VA_ARGS__)


#endif //TEST_LOGMACRO_H
