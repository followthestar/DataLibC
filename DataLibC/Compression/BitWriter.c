//
// Created by 18030 on 2024/12/21.
//

#include "BitWriter.h"
#include "DataLibC/Tool/LogMacro.h"

static Byte bits[] = {0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x2, 0x1};

BitWriter CreateBitWriter(Byte* outStream, size_t maxSize, BitDirection direction)
{
    BitWriter out;
    out.currBit = 0;
    out.dataPtr = out.writePtr = outStream;
    out.dir = direction;
    out.maxSize = maxSize;
    return out;
}

void WriteBit(BitWriter* writer, Byte bit)
{
    if (writer->writePtr - writer->dataPtr == writer->maxSize)
    {
        RuntimeErrorLog(MemoryOutOfRange, "The writing buffer is full!");
        return;
    }
    if (bit)
        bit = 1;

    if (writer->dir)
    {
        *writer->writePtr |= (bit << (8 - writer->currBit - 1));
    }
    else
    {
        *writer->writePtr |= (bit << writer->currBit);
    }
    writer->currBit++;

    if (writer->currBit == 8)
    {
        writer->writePtr++;
        writer->currBit = 0;
    }
}
