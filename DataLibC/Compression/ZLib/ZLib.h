//
// Created by 18030 on 2024/12/19.
//

#ifndef DATALIBC_ZLIB_H
#define DATALIBC_ZLIB_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/Compression/Compression.h"
#include <stddef.h>


struct LIB_STRUCT ZLib
{
    Byte mfCode;
    Byte fcBits;
    Byte* dataBlocks;
    UInt32 checkValue;
};

typedef struct ZLib ZLib;

API_MODULE ZLib* CreateZLibFromByteFlow(const Byte* data, size_t dataLength);

API_MODULE Byte* ZLib_Decompress(const ZLib* zlib, size_t* outLength);

API_MODULE void ZLib_Release(ZLib* zlib);


#endif //DATALIBC_ZLIB_H
