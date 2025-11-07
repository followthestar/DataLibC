//
// Created by 18030 on 2024/12/21.
//

#ifndef DATALIBC_BITWRITER_H
#define DATALIBC_BITWRITER_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "Compression.h"

struct LIB_STRUCT BitWriter
{
    Byte* dataPtr;
    Byte* writePtr;
    Byte currBit;
    BitDirection dir;
    size_t maxSize;
};

typedef struct BitWriter BitWriter;

API_MODULE BitWriter CreateBitWriter(Byte* outStream, size_t maxSize, BitDirection direction);

API_MODULE void WriteBit(BitWriter* writer, Byte bit);

#endif //DATALIBC_BITWRITER_H
