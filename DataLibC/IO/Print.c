//
// Created by 18030 on 2025/9/19.
//

#include "Print.h"

#include <stdarg.h>
#include <stdio.h>

#include "DataLibC/Compression/Compression.h"
#include "DataLibC/DS/Algorithm.h"
#include "DataLibC/Tool/LogMacro.h"
#include "DataLibC/DS/TypeEnum.h"
#include "DataLibC/Tool/TypeMacro.h"

static int ExtractAndPrintFromVA(va_list* list, char* buffer, const int start, const int maxSize)
{
    const DataType type = va_arg(*list, DataType);
    if (IsConvertibleToInt(type))
    {
        const int v = va_arg(*list, int);
        return snprintf(buffer + start, maxSize, "%d ", v);
    }
    if (IsConvertibleToDouble(type))
    {
        const double v = va_arg(*list, double);
        return snprintf(buffer + start, maxSize, "%g ", v);
    }
    if (type == kUINT)
    {
        const unsigned int v = va_arg(*list, unsigned int);
        return snprintf(buffer + start, maxSize, "%u ", v);
    }
    if (type == kSTRING)
    {
        const char* v = va_arg(*list, char*);
        return snprintf(buffer + start, maxSize, "%s ", v);
    }
    if (type == kPOINTER)
    {
        const void* v = va_arg(*list, void*);
        return snprintf(buffer + start, maxSize, "%p ", v);
    }
    if (type == kBOOL)
    {
        const bool v = va_arg(*list, int);
        return snprintf(buffer + start, maxSize, "%s ", v ? "true" : "false");
    }
    return 0;
}

void PrintAnyImpl(const int size, ...)
{
    va_list list;
    va_start(list, size);
    constexpr int MAX_LEN = 512;
    char fmtBuffer[MAX_LEN] = { };
    int len = 0;
    for (int i = 0; i < size; i++)
    {
        if (len >= MAX_LEN - 2)
        {
            RuntimeErrorLog(IndexOutOfRange, "The format string you passed is too long to output!");
            break;
        }

        len += ExtractAndPrintFromVA(&list, fmtBuffer, len, MAX_LEN - len);
    }
    len = min(len, MAX_LEN - 2);
    fmtBuffer[len] = '\n';
    fmtBuffer[len + 1] = '\0';
    printf(fmtBuffer);
    va_end(list);
}

void PrintFmtImpl(const int size, ...)
{
    va_list list;
    va_start(list, size);
    const DataType fmtType = va_arg(list, DataType);
    if (fmtType != kSTRING)
    {
        RuntimeErrorLog(TypeError, "The first argument must be a format string when calling print_fmt()!");
        goto clean;
    }
    const char* fmt = va_arg(list, char*);
    int processedObject = 1;
    char fmtChar;
    bool brace = false;
    constexpr int MAX_LEN = 512;
    char fmtBuffer[MAX_LEN] = { };
    int len = 0;
    foreach(fmtChar in fmt)
    {
        if (len >= MAX_LEN - 1)
        {
            RuntimeErrorLog(IndexOutOfRange, "The format string you passed is too long to output!");
            break;
        }

        if (fmtChar == '{')
        {
            if (brace != false)
            {
                RuntimeErrorLog(InvalidFormat, "The format string is invalid! '{' mismatched in %s", fmt);
                goto clean;
            }
            brace = true;
        }
        else if (fmtChar == '}')
        {
            if (brace != true)
            {
                RuntimeErrorLog(InvalidFormat, "The format string is invalid! '}' mismatched in %s", fmt);
                goto clean;
            }
            brace = false;

            if (processedObject == size)
            {
                RuntimeErrorLog(InvalidArgument, "The format string is invalid! Too few arguments passed in %s", fmt);
                goto clean;
            }

            len += ExtractAndPrintFromVA(&list, fmtBuffer, len, MAX_LEN - len);
            processedObject++;
        }
        else
        {
            fmtBuffer[len++] = fmtChar;
        }
    }
    len = min(len, MAX_LEN - 1);
    fmtBuffer[len] = '\n';
    printf(fmtBuffer);
clean:
    va_end(list);
}
