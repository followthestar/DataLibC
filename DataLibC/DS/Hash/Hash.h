//
// Created by 18030 on 2024/11/23.
//

#ifndef TEST_HASH_H
#define TEST_HASH_H

#include <stddef.h>
#include "DataLibC/Tool/ExportModuleMacro.h"

typedef size_t Hash(void* value);

typedef Hash* _hashImpl[11];

Hash* GetBuiltinHash(size_t type);


#endif //TEST_HASH_H
