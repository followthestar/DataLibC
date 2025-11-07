//
// Created by 18030 on 2024/11/16.
//

#include "Algorithm.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

static void* _GetElemPtr(void* elements, size_t elemSize, size_t index)
{
    return elements + elemSize * index;
}

static int Partition(void* arr, int low, int high, size_t elementSize, Predicate* comparator)
{
    char* pivot = (char*) arr + high * elementSize;
    int i = low - 1;
    for (int j = low; j < high; j++)
    {
        if (comparator((char*) arr + j * elementSize, pivot))
        {
            i++;
            Swap((char*) arr + i * elementSize, (char*) arr + j * elementSize, elementSize);
        }
    }
    i++;
    Swap((char*) arr + i * elementSize, pivot, elementSize);
    return i;
}

static void QuickSortRecursive(void* arr, int low, int high, size_t elementSize, Predicate* comparator)
{
    if (low < high)
    {
        int pi = Partition(arr, low, high, elementSize, comparator);
        QuickSortRecursive(arr, low, pi - 1, elementSize, comparator);
        QuickSortRecursive(arr, pi + 1, high, elementSize, comparator);
    }
}

void QuickSort(void* elements, size_t arrayLength, size_t elementSize, Predicate* comparator)
{
    if (arrayLength <= 1)
        return;
    QuickSortRecursive(elements, 0, arrayLength - 1, elementSize, comparator);
}


void Swap(void* lhs, void* rhs, size_t elementSize)
{
    char temp[elementSize];
    memcpy(temp, lhs, elementSize);
    memcpy(lhs, rhs, elementSize);
    memcpy(rhs, temp, elementSize);
}

size_t NextPowerTwo(size_t origin)
{
    if (!(origin & (origin - 1)))
        return origin;
    origin--;
    for (int i = 1; i <= 64; i *= 2)
    {
        origin |= origin >> i;
    }
    return origin + 1;
}

ssize_t BinarySearch(void* elements, size_t arrayLength, size_t elementSize, Predicate* comparartor, void* value)
{
    if (arrayLength == 0)
        return -1;
    int left = 0, right = arrayLength - 1;
    int mid;

    while (left <= right)
    {
        mid = (left + right) / 2;
        if (comparartor(value, _GetElemPtr(elements, elementSize, mid)))
            right = mid - 1;
        else if (comparartor(_GetElemPtr(elements, elementSize, mid), value))
            left = mid + 1;
        else
            return mid;
    }
    return -1;
}

size_t LowerBound(void* elements, size_t arrayLength, size_t elementSize, Predicate* comparartor, void* value)
{
    size_t left = 0, right = arrayLength;
    size_t mid;

    while (left < right)
    {
        mid = (left + right) / 2;
        if (comparartor(_GetElemPtr(elements, elementSize, mid), value))
            left = mid + 1;
        else
            right = mid;
    }
    return left;
}

size_t UpperBound(void* elements, size_t arrayLength, size_t elementSize, Predicate* comparartor, void* value)
{
    size_t left = 0, right = arrayLength;
    size_t mid;

    while (left < right)
    {
        mid = (left + right) / 2;
        if (!comparartor(value, _GetElemPtr(elements, elementSize, mid)))
            left = mid + 1;
        else
            right = mid;
    }
    return left;
}

size_t NextPrime(size_t n)
{
    size_t candidate = n + 1;
    while (!IsPrime(candidate))
        candidate++;

    return candidate;
}

bool IsPrime(size_t num)
{
    if (num <= 1) return false;
    if (num <= 3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;
    for (int i = 5; i * i <= num; i += 6)
        if (num % i == 0 || num % (i + 2) == 0) return false;
    return true;
}

unsigned char* LZ77Decompress(unsigned char* data, size_t length, size_t* compressedSize)
{
    if (!data)
        return NULL;
    unsigned char* decompressed = NULL;


    return decompressed;
}

unsigned char* HaffmanDecoding(unsigned char* data, size_t length, size_t* decodingSize)
{
    return nullptr;
}

bool CompareApproximately(float a, float b, float epsilon)
{
    float diff = a - b;
    return fabsf(diff) < epsilon;
}
