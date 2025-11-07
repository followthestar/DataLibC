//
// Created by 18030 on 2024/12/7.
//

#ifndef TEST_ALLOCATION_H
#define TEST_ALLOCATION_H

#include <stdlib.h>
#include "TypeEnum.h"

typedef void* Allocator(size_t size);


typedef void* Constructor(const void* data);

typedef void Destructor(void* p);

Constructor* GetBuiltinCtor(DataType type);

Destructor* GetBuiltinDtor(DataType type);


#define BASE_NEW(T) malloc(sizeof(T))
#define BASE_INIT(T, ptr) memset(ptr, 0, sizeof(T))

#define BASE_DELETE(T, ptr) free(ptr)

#endif //TEST_ALLOCATION_H
