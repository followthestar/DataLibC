//
// Created by 18030 on 2024/11/16.
//

#ifndef TEST_GENERICTOOL_H
#define TEST_GENERICTOOL_H

#include "DataLibC/Tool/ExportModuleMacro.h"

#if __STDC_VERSION__ > 201710L

typedef struct StaticArray StaticArray;
typedef struct String String;

API_MODULE const char* StaticArray_ToString(StaticArray* arr);

API_MODULE const char* String_ToString(String* str);

#define ToString(obj) _Generic((obj), \
                StaticArray*: StaticArray_ToString,\
                String*:      String_ToString\
                )(obj)

#endif

#endif //TEST_GENERICTOOL_H
