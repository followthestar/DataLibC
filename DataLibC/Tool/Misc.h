//
// Created by 18030 on 2024/11/15.
//

#include "DataLibC/DS/TypeEnum.h"
#include <stdio.h>

#ifndef TEST_MISC_H
#define TEST_MISC_H


const char* GetFormatPlaceHolder(DataType type);

void ProcessVaListInternal(void* buffer, DataType type, size_t argC, va_list argV);

bool ReadAndCompare(FILE* file, size_t length, const char* str);

void SwapEndian(void* elem, size_t size);

bool StringIsInteger(const char* str, int* out);


#endif //TEST_MISC_H
