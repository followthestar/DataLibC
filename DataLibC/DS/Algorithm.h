//
// Created by 18030 on 2024/11/16.
//

#include <stddef.h>
#include "DataLibC/Tool/ExportModuleMacro.h"

#ifndef TEST_ALGORITHM_H
#define TEST_ALGORITHM_H

#ifdef __GNUC__
#define min(x, y) ({typeof(x) _x = x; typeof(y) _y = y; _x < _y ? _x : _y;})
#define max(x, y) ({typeof(x) _x = x; typeof(y) _y = y; _x > _y ? _x : _y;})
#else
#define min(x, y) (x) < (y) ? (x) : (y)
#define max(x, y) (x) > (y) ? (x) : (y)
#endif

typedef bool Predicate(void*, void*);

typedef int Comparator(void*, void*);

API_MODULE void Swap(void* lhs, void* rhs, size_t elementSize);

API_MODULE void QuickSort(void* elements, size_t arrayLength, size_t elementSize, Predicate* comparator);

API_MODULE size_t NextPowerTwo(size_t origin);

API_MODULE ssize_t
BinarySearch(void* elements, size_t arrayLength, size_t elementSize, Predicate* comparartor, void* value);

API_MODULE size_t
LowerBound(void* elements, size_t arrayLength, size_t elementSize, Predicate* comparartor, void* value);

API_MODULE size_t
UpperBound(void* elements, size_t arrayLength, size_t elementSize, Predicate* comparartor, void* value);

API_MODULE size_t NextPrime(size_t n);

API_MODULE bool IsPrime(size_t num);

API_MODULE unsigned char* LZ77Decompress(unsigned char* data, size_t length, size_t* compressedSize);

API_MODULE unsigned char* HaffmanEncoding(unsigned char* data, size_t length, size_t* encodingLength);

API_MODULE unsigned char* HaffmanDecoding(unsigned char* data, size_t length, size_t* decodingSize);

API_MODULE bool CompareApproximately(float a, float b, float epsilon);

#endif //TEST_ALGORITHM_H
