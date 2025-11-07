//
// Created by 18030 on 2024/12/20.
//

#include <assert.h>
#include "BitReader.h"


static Byte bits[] = {0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1};


static Byte GetBitMask(const BitReader* reader, Byte count)
{
    if (count == 0)
        return 0;
    Byte mask;
    if (reader->dir)
        mask = bits[reader->currBit];
    else
        mask = bits[8 - reader->currBit - 1];
    for (int i = 0; i < count - 1; ++i)
    {
        if (reader->dir)
            mask |= mask >> 1;
        else
            mask |= mask << 1;
    }
    return mask;
}

Byte ReadBits(BitReader* bitReader, Byte count)
{
    assert(count <= 8);
    if (count == 0)
        return 0;
    if (bitReader->currBit + count < 8)
    {
        auto mask = GetBitMask(bitReader, count);
        bitReader->currBit += count;
        return (*bitReader->dataPtr & mask)
                >> (bitReader->dir ? (8 - bitReader->currBit) : (bitReader->currBit - count));
    }
    else
    {
        auto left = 8 - bitReader->currBit;
        auto mask = GetBitMask(bitReader, left);
        Byte value = *bitReader->dataPtr & mask;
        bitReader->dataPtr++;
        bitReader->currBit = 0;
        mask = GetBitMask(bitReader, count - left);
        bitReader->currBit = count - left;
        Byte value2 = *bitReader->dataPtr & mask;
        return bitReader->dir ? ((value << bitReader->currBit) | (value2 >> (8 - bitReader->currBit)))
                              : ((value >> (8 - left) | value2 << left));
    }
}

BitReader CreateBitRead(Byte* data, BitDirection direction)
{
    BitReader result = {data, 0, direction};
    return result;
}
