//
// Created by 18030 on 2024/11/15.
//

#include <stdio.h>
#include <memory.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "Misc.h"

const char* GetFormatPlaceHolder(DataType type)
{
    switch (type)
    {
        case kUNKNOWN:
            return NULL;
        case kUCHAR:
            return "%hhu";
        case kCHAR:
            return "%c";
        case kUSHORT:
            return "%u";
        case kSHORT:
            return "%d";
        case kUINT:
            return "%u";
        case kINT:
            return "%d";
        case kLLONG:
            return "%lld";
        case kFLOAT:
        case kDOUBLE:
            return "%g";
    }
}

void ProcessVaListInternal(void* buffer, DataType type, size_t argC, va_list argV)
{
    if (type == kUNKNOWN || type == kCUSTOM)
        return;
    char* data;
    for (int i = 0; i < argC; i++)
    {
        data = (char*) buffer + gTypeSize[type] * i;
        switch (type)
        {
            case kUCHAR:
                *(unsigned char*) data = va_arg(argV, unsigned);
                break;
            case kCHAR:
                *(char*) data = va_arg(argV, int);
                break;
            case kUSHORT:
                *(unsigned short*) data = va_arg(argV, int);
                break;
            case kSHORT:
                *(short*) data = va_arg(argV, int);
                break;
            case kUINT:
                *(unsigned*) data = va_arg(argV, unsigned);
                break;
            case kINT:
                *(int*) data = va_arg(argV, int);
                break;
            case kULLONG:
                *(unsigned long long*) data = va_arg(argV, unsigned long long);
                break;
            case kLLONG:
                *(long long*) data = va_arg(argV, long long);
                break;
            case kFLOAT:
                *(float*) data = va_arg(argV, double);
                break;
            case kDOUBLE:
                *(double*) data = va_arg(argV, double);
                break;
            default:
                break;
        }
    }
}

bool ReadAndCompare(FILE* file, size_t length, const char* str)
{
    char buf[length];
    fread(buf, sizeof(char), length, file);
    return memcmp(buf, str, length) == 0;
}

void SwapEndian(void* elem, size_t size)
{
    unsigned char* elem_ptr = elem;
    unsigned char temp;
    for (int i = 0; i < size / 2; ++i)
    {
        temp = elem_ptr[i];
        elem_ptr[i] = elem_ptr[size - i - 1];
        elem_ptr[size - i - 1] = temp;
    }
}

bool StringIsInteger(const char* str, int* out)
{
    if (!str)
        return false;
    size_t len = strlen(str);
    if (len == 0)
        return false;
    char* end = NULL;
    *out = strtol(str, &end, 10);
    return end == str + len;
}

