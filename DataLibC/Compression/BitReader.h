//
// Created by 18030 on 2024/12/20.
//

#ifndef DATALIBC_BITREADER_H
#define DATALIBC_BITREADER_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "Compression.h"


struct LIB_STRUCT BitReader
{
    Byte* dataPtr;
    Byte currBit;
    BitDirection dir;
};


typedef struct BitReader BitReader;

/// Create a BitReader which used to read data from a bit flow
/// \param data The bytes flow of data
/// \param direction Which direction do you want to read? (FromLow: bit0 to bit7; FromHigh: bit7 to bit0)
/// \return The BitReader object
API_MODULE BitReader CreateBitRead(Byte* data, BitDirection direction);

API_MODULE Byte ReadBits(BitReader* bitReader, Byte count);

#endif //DATALIBC_BITREADER_H
