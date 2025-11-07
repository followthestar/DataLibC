#include "BitSet.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "DataLibC/Tool/TypeMacro.h"

typedef struct BitSet
{
    int bitCount;
    UInt8* data;
} BitSet;

static int BytesInNeed(const int bits)
{
    return (bits + 7) / 8;
}

static void ClearRemainder(const BitSet* bitSet)
{
    assert(bitSet);
    assert(bitSet->data);
    const int byteCount = BytesInNeed(bitSet->bitCount);
    const int remainder = bitSet->bitCount % 8;
    if (remainder != 0)
    {
        bitSet->data[byteCount - 1] &= (1U << remainder) - 1;
    }
}

BitSet* BitSet_Create(const int initSize, const bool initValue)
{
    if (initSize <= 0)
    {
        return nullptr;
    }

    BitSet* bs = malloc(sizeof(BitSet));
    if (!bs)
        return nullptr;

    const int byteCount = BytesInNeed(initSize);
    bs->data = malloc(byteCount);
    if (!bs->data)
    {
        free(bs);
        return nullptr;
    }

    bs->bitCount = initSize;
    const UInt8 fillByte = initValue ? 0xFF : 0x00;
    memset(bs->data, fillByte, byteCount);

    // Clear the excess bits
    if (initValue)
        ClearRemainder(bs);

    return bs;
}

void BitSet_Fill(BitSet* bitSet, const bool value)
{
    if (!bitSet || !bitSet->data)
        return;

    const UInt8 fillByte = value ? 0xFF : 0x00;
    const int byteCount = BytesInNeed(bitSet->bitCount);
    memset(bitSet->data, fillByte, byteCount);

    if (value)
        ClearRemainder(bitSet);
}

void BitSet_Set(BitSet* bitSet, const int index, const bool value)
{
    if (!bitSet || !bitSet->data || index < 0 || index >= bitSet->bitCount)
        return;

    const int byteIndex = index / 8;
    const int bitIndex = index % 8;

    if (value)
        bitSet->data[byteIndex] |= 1U << bitIndex;
    else
        bitSet->data[byteIndex] &= ~(1U << bitIndex);
}

bool BitSet_Get(BitSet* bitSet, const int index)
{
    if (!bitSet || !bitSet->data || index < 0 || index >= bitSet->bitCount)
        return false;

    const int byteIndex = index / 8;
    const int bitIndex = index % 8;
    return (bitSet->data[byteIndex] & 1U << bitIndex) != 0;
}

int BitSet_Size(const BitSet* bitSet)
{
    return bitSet->bitCount;
}

void BitSet_Destroy(BitSet* bitSet)
{
    if (bitSet)
    {
        free(bitSet->data);
        free(bitSet);
    }
}
