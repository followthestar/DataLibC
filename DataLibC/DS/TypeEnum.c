//
// Created by 18030 on 2024/11/15.
//


#include "TypeEnum.h"
#include "DataLibC/DS/Pair/Pair.h"

size_t gTypeSize[] = {sizeof(unsigned char), sizeof(char), sizeof(unsigned short),
                      sizeof(short), sizeof(unsigned), sizeof(int), sizeof(unsigned long long),
                      sizeof(unsigned long long), sizeof(float), sizeof(double),
                      sizeof(char*), sizeof(Pair), 0};