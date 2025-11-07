//
// Created by 18030 on 2024/12/19.
//

#include <stdlib.h>
#include <string.h>
#include "ZLib.h"
#include "DataLibC/Tool/Misc.h"
#include "DataLibC/Tool/LogMacro.h"
#include "DataLibC/Compression/BitReader.h"
#include "DataLibC/Compression/HaffmanTree.h"
#include "DataLibC/DS/String/String.h"

typedef enum HaffmanKind
{
    NoneHaffman,
    StaticHaffman,
    DynamicHaffman,
    Unavailable
} HaffmanKind;

const int codeOrder[] = {16, 17, 18, 0, 8, 7,
                         9, 6, 10, 5, 11, 4,
                         12, 3, 13, 2, 14, 1,
                         15};

typedef struct CodeExtension
{
    Byte extraBits;
    UInt16 startValue;
} CodeExtension;

static CodeExtension lengthExt[31] = {
        (CodeExtension) {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7}, {0, 8}, {0, 9}, {0, 10}, {1, 11}, {1, 13},
        (CodeExtension) {1, 15}, {1, 17}, {2, 19}, {2, 23}, {2, 27}, {2, 31}, {3, 35}, {3, 43}, {3, 51}, {3, 59},
        (CodeExtension) {4, 67}, {4, 83}, {4, 99}, {4, 115}, {5, 131}, {5, 163}, {5, 195}, {5, 227}, {0, 258}, {0, 0},
        {0, 0}
};

static CodeExtension distanceExt[32] = {
        (CodeExtension) {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 5}, {1, 7}, {2, 9}, {2, 13}, {3, 17}, {3, 25},
        (CodeExtension) {4, 33}, {4, 49}, {5, 65}, {5, 97}, {6, 129}, {6, 193}, {7, 257}, {7, 385}, {8, 513}, {8, 769},
        (CodeExtension) {9, 1025}, {9, 1537}, {10, 2049}, {10, 3073}, {11, 4097}, {11, 6145}, {12, 8193}, {12, 12289},
        {13, 16385}, {13, 24577}, {0, 0}, {0, 0}
};

ZLib* CreateZLibFromByteFlow(const Byte* data, size_t dataLength)
{
    if (!data || dataLength <= 6)
        return NULL;
    ZLib* zlib = malloc(sizeof(ZLib));
    if (!zlib)
        return NULL;


    zlib->mfCode = data[0];
    zlib->fcBits = data[1];
    zlib->checkValue = *(int*) (data + dataLength - 4);
    SwapEndian(&zlib->checkValue, sizeof zlib->checkValue);

    size_t offset = 0;

    if (zlib->fcBits & 0x20)
        offset = 4;

    zlib->dataBlocks = malloc(dataLength - offset - 6);
    if (!zlib->dataBlocks)
        return NULL;

    memcpy(zlib->dataBlocks, data + offset + 2, dataLength - offset - 6);
    return zlib;
}

static void DecodedFromRunLengthEncoding(HaffmanTree* tree, BitReader* reader, Byte* out, size_t count)
{
    UInt16 lastCode;
    int decodedNum = 0;
    while (decodedNum != count)
    {
        UInt16 decode;
        DecodeFromHaffmanTree(tree, reader, &decode, 1);
        switch (decode)
        {
            case 16:
            {
                Byte repeat = ReadBits(reader, 2) + 3;
                for (int i = 0; i < repeat; ++i)
                {
                    out[decodedNum + i] = lastCode;
                }
                decodedNum += repeat;
            }
                break;
            case 17:
                // We need to process next 3 bits which represent amount of number 0 (amount + 3)
            {
                Byte zeroAmount = ReadBits(reader, 3) + 3;
                memset(out + decodedNum, 0, zeroAmount);
                decodedNum += zeroAmount;
            }
                break;
            case 18:
                // We need to process next 7 bits which represent amount of number 0 (amount + 11)
            {
                Byte zeroAmount = ReadBits(reader, 7) + 11;
                memset(out + decodedNum, 0, zeroAmount);
                decodedNum += zeroAmount;
            }
                break;
            default:
                out[decodedNum++] = decode;
                lastCode = decode;
        }
    }
}

static HaffmanTree* static_literal_length = NULL;
static HaffmanTree* static_distance = NULL;

#pragma region Static Huffman
static const Byte static_literal_length_lengths[] = {
        // Literal/Lenght Codes
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,  // 0-15
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,  // 16-31
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,  // 32-47
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,  // 48-63
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,  // 64-79
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,  // 80-95
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,  // 96-111
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,  // 112-127
        8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,  // 128-143
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,   // 144-159
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,  // 160-175
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,  // 176-191
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,  // 192-207
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,  // 208-223
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,  // 224-239
        9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,  // 240-255
        7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,  // 256-271
        7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8  // 272-287
};


static const Byte static_distance_lengths[] = {
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,  // 0-15
        5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,  // 16-31
};
#pragma endregion

static void DecodeFromTree(HaffmanTree* literal_length, HaffmanTree* distance, BitReader* reader, String* out)
{
// Decode left bits
    UInt16 currCode = 0;
    UInt16 repeatLength = 0;
    UInt16 repeatDistance = 0;

    int j = 0;
    while (1)
    {
        DecodeFromHaffmanTree(literal_length, reader, &currCode, 1);
        if (currCode == 256)
            break;
        else if (currCode > 256)
        {
            // We need to handle extra length and the next symbol is distance
            currCode -= 257;
            if (!lengthExt[currCode].extraBits)
                repeatLength = lengthExt[currCode].startValue;
            else
                repeatLength = lengthExt[currCode].startValue
                               + ReadBits(reader, lengthExt[currCode].extraBits);

            DecodeFromHaffmanTree(distance, reader, &currCode, 1);
            // We need to handle extra distance and repeat the character
            repeatDistance = distanceExt[currCode].startValue;
            if (distanceExt[currCode].extraBits)
            {
                auto extraBits = distanceExt[currCode].extraBits;
                if (extraBits <= 8)
                    repeatDistance += ReadBits(reader, distanceExt[currCode].extraBits);
                else
                {
                    UInt16 value = ReadBits(reader, 8);
                    value |= ((UInt16) ReadBits(reader, extraBits - 8) << 8);
                    repeatDistance += value;
                }
            }

            for (int i = 0; i < repeatLength; ++i)
                String_AppendChar(out, String_CharAt(out, out->length - repeatDistance));

        }
        else
        {
            // Just write this symbol
            String_AppendChar(out, (char) currCode);
        }
        j++;
    }
}

static void Decompress_StaticHaffman_Internal(BitReader* reader, String* out)
{
    if (!static_literal_length)
    {
        static_literal_length = malloc(sizeof(HaffmanTree));
        *static_literal_length = GenerateHaffmanTreeFromCodeLengthSequence(static_literal_length_lengths, 288);
    }

    if (!static_distance)
    {
        static_distance = malloc(sizeof(HaffmanTree));
        *static_distance = GenerateHaffmanTreeFromCodeLengthSequence(static_distance_lengths, 32);
    }

    DecodeFromTree(static_literal_length, static_distance, reader, out);
}

static void Decompress_DynamicHaffman_Internal(BitReader* reader, String* out)
{
    UInt16 numCL1, numCL2, numCCL;

    numCL1 = ReadBits(reader, 5) + 257;
    numCL2 = ReadBits(reader, 5) + 1;
    numCCL = ReadBits(reader, 4) + 4;

    // Standard CCL contains 19 codes
    Byte originCCL[numCCL];
    for (int i = 0; i < numCCL; ++i)
    {
        originCCL[i] = ReadBits(reader, 3);
    }
    Byte CCL[19];
    for (int i = 0; i < numCCL; ++i)
        CCL[codeOrder[i]] = originCCL[i];

    for (int i = numCCL; i < 19; ++i)
        CCL[codeOrder[i]] = 0;


    auto CCLTree = GenerateHaffmanTreeFromCodeLengthSequence(CCL, 19);

    Byte decodedCL1[numCL1];
    Byte decodedCL2[numCL2];
    DecodedFromRunLengthEncoding(&CCLTree, reader, decodedCL1, numCL1);
    DecodedFromRunLengthEncoding(&CCLTree, reader, decodedCL2, numCL2);

    auto literal_length = GenerateHaffmanTreeFromCodeLengthSequence(decodedCL1, numCL1);
    auto distance = GenerateHaffmanTreeFromCodeLengthSequence(decodedCL2, numCL2);

    DecodeFromTree(&literal_length, &distance, reader, out);

    DestroyHaffmanTree(&CCLTree);
    DestroyHaffmanTree(&literal_length);
    DestroyHaffmanTree(&distance);
}

Byte* ZLib_Decompress(const ZLib* zlib, size_t* outLength)
{
    bool isFinalBlcok = false;
    HaffmanKind kind;

    BitReader reader = CreateBitRead(zlib->dataBlocks, FromLow);


    String out;
    String_Init(&out, 0, NULL);
    do
    {
        isFinalBlcok = ReadBits(&reader, 1);
        kind = ReadBits(&reader, 2);
        switch (kind)
        {
            case NoneHaffman:
                RuntimeErrorLog(InvalidFormat, "The NoneHaffman encoding is not supported yet!");
                *outLength = 0;
                break;
            case StaticHaffman:
                Decompress_StaticHaffman_Internal(&reader, &out);
                break;
            case DynamicHaffman:
                Decompress_DynamicHaffman_Internal(&reader, &out);
                break;
            case Unavailable:
                RuntimeErrorLog(InvalidFormat, "The data contains invalid format!");
        }

    } while (!isFinalBlcok);
    *outLength = out.length;
    return (Byte*) String_ToString(&out);
}

void ZLib_Release(ZLib* zlib)
{
    if (zlib->dataBlocks)
        free(zlib->dataBlocks);
}
