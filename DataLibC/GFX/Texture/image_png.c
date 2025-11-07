//
// Created by 18030 on 2024/12/14.
//

#include <string.h>
#include <stdlib.h>
#include "image_png.h"
#include "DataLibC/Tool/Misc.h"
#include "DataLibC/Tool/LogMacro.h"
#include "DataLibC/DS/Algorithm.h"
#include "DataLibC/Compression/ZLib/ZLib.h"

#define PNG_IHDR "IHDR"
#define PNG_PLTE "PLTE"
#define PNG_IDAT "IDAT"
#define PNG_IEND "IEND"

#define PNG_8_BIT 8
#define PNG_RGB 2
#define PNG_RGBA 6

typedef enum PNG_CHUNK_TYPE
{
    IHDR,
    PLTE,
    IDAT,
    IEND,
    IGNORE
} PNG_CHUNK_TYPE;

typedef enum FilterType
{
    FILTER_NONE,
    FILTER_SUB,
    FILTER_UP,
    FILTER_AVERAGE,
    FILTER_PAETH,
    FILTER_AVG_FIRST,
    FILTER_PAETH_FIRST
} FilterType;

static FilterType first_row_filter[] = {
        FILTER_NONE,
        FILTER_SUB,
        FILTER_NONE,
        FILTER_AVG_FIRST,
        FILTER_PAETH_FIRST
};

static PNG_CHUNK_TYPE SwitchPNGChunkType(unsigned char* chunkType)
{
    if (memcmp(chunkType, PNG_IHDR, 4) == 0)
        return IHDR;
    if (memcmp(chunkType, PNG_PLTE, 4) == 0)
        return PLTE;
    if (memcmp(chunkType, PNG_IDAT, 4) == 0)
        return IDAT;
    if (memcmp(chunkType, PNG_IEND, 4) == 0)
        return IEND;
    return IGNORE;
}

static int FilterPaeth(int left, int upper, int upperLeft)
{
    int p = left + upper - upperLeft;
    int pa = abs(p - left);
    int pb = abs(p - upper);
    int pc = abs(p - upperLeft);
    if (pa <= pb && pa <= pc) return left;
    if (pb <= pc) return upper;
    return upperLeft;
}

static void ExtractPixelsFromData(Texture* out, unsigned char* data, size_t size, bool forceAlign)
{
    out->pixels = NULL;
    if (!data)
        return;

    unsigned char* pixels;
    int byte = out->depth == 16 ? 2 : 1;
    int filterBytes = out->channelCount * byte;
    size_t row_bytes = out->width * out->channelCount * byte;
    size_t row_size = forceAlign ? (row_bytes + 3) / 4 * 4 : row_bytes;

    size_t img_width_bytes = (((out->channelCount * out->width * out->depth) + 7) >> 3);
    size_t img_len = (img_width_bytes + 1) * out->height;

    // we used to check for exact match between raw_len and img_len on non-interlaced PNGs,
    // but issue #276 reported a PNG in the wild that had extra data at the end (all zeros),
    // so just check for raw_len < img_len always.
    if (size < img_len)
    {
        RuntimeErrorLog(InvalidFormat, "Invalid PNG format!");
        return;
    }


    pixels = malloc(row_size * out->height);
    if (!pixels)
    {
        RuntimeErrorLog(MallocFailed, "Failed to allocate png pixels memory!");
        return;
    }
    memset(pixels, 0, row_size * out->height);

    //TODO...
    unsigned char* currRow;
    unsigned char* priorRow;
    FilterType filter;
    for (int i = 0; i < out->height; ++i)
    {
        currRow = pixels + i * row_size;
        filter = *data++;
        if (filter > 4)
        {
            RuntimeErrorLog("invalid filter", "Corrupt PNG");
            free(pixels);
            return;
        }

        priorRow = currRow - row_size;

        if (i == 0) filter = first_row_filter[filter];

        for (int j = 0; j < filterBytes; ++j)
        {
            switch (filter)
            {
                case FILTER_NONE:
                case FILTER_SUB:
                case FILTER_AVG_FIRST:
                case FILTER_PAETH_FIRST:
                    currRow[j] = data[j];
                    break;
                case FILTER_UP:
                    currRow[j] = data[j] + priorRow[j];
                    break;
                case FILTER_AVERAGE:
                    currRow[j] = data[j] + (priorRow[j] >> 1);
                    break;
                case FILTER_PAETH:
                    currRow[j] = data[j] + FilterPaeth(0, priorRow[j], 0);
                    break;
            }
        }

        if (out->depth == 8 || out->depth == 16)
        {
            data += out->channelCount;
            currRow += out->channelCount;
            priorRow += out->channelCount;
        }
        else
        {
            data += 1;
            currRow += 1;
            priorRow += 1;
        }

        //TODO...
        if (out->depth != 16)
        {
            int leftPixelsBytes = (out->width - 1) * filterBytes;
            switch (filter)
            {
                case FILTER_NONE:
                    memcpy(currRow, data, leftPixelsBytes);
                    break;
                case FILTER_SUB:
                    for (int j = 0; j < leftPixelsBytes; ++j)
                        currRow[j] = data[j] + currRow[j - filterBytes];
                    break;
                case FILTER_UP:
                    for (int j = 0; j < leftPixelsBytes; ++j)
                        currRow[j] = data[j] + priorRow[j];
                    break;
                case FILTER_AVERAGE:
                    for (int j = 0; j < leftPixelsBytes; ++j)
                        currRow[j] = data[j] + ((priorRow[j] + currRow[j - filterBytes]) >> 1);
                    break;
                case FILTER_PAETH:
                    for (int j = 0; j < leftPixelsBytes; ++j)
                        currRow[j] = data[j]
                                     + FilterPaeth(currRow[j - filterBytes], priorRow[j], priorRow[j - filterBytes]);
                    break;
                case FILTER_AVG_FIRST:
                    for (int j = 0; j < leftPixelsBytes; ++j)
                        currRow[j] = data[j] + (currRow[j - filterBytes] >> 1);
                    break;
                case FILTER_PAETH_FIRST:
                    for (int j = 0; j < leftPixelsBytes; ++j)
                        currRow[j] = data[j] + FilterPaeth(currRow[j - filterBytes], 0, 0);
                    break;
            }
            data += leftPixelsBytes;
        }

    }

    out->pixels = pixels;
}

typedef struct IDATChunk
{
    unsigned chunkLength;
    unsigned char* chunkData;
} IDATChunk;

static unsigned char* CombineIDATChunks(IDATChunk* chunks, size_t count, size_t* combinedSize)
{
    *combinedSize = 0;
    if (!chunks)
        return NULL;

    for (int i = 0; i < count; ++i)
        *combinedSize += chunks[i].chunkLength;

    unsigned char* combined = malloc(*combinedSize);
    unsigned char* start = combined;
    for (int i = 0; i < count; ++i)
    {
        memcpy(start, chunks[i].chunkData, chunks[i].chunkLength);
        start += chunks[i].chunkLength;
    }
    return combined;
}

static void ProcessFlip(Texture* out, bool flip, bool forceAlign)
{
    if (!flip)
        return;
    int byte = out->depth == 16 ? 2 : 1;
    size_t row_bytes = out->width * out->channelCount * byte;
    size_t row_size = forceAlign ? (row_bytes + 3) / 4 * 4 : row_bytes;
    unsigned char temp[row_size];
    for (int i = 0; i < out->height / 2; ++i)
    {
        memcpy(temp, out->pixels + i * row_size, row_size);
        memcpy((void*) out->pixels + i * row_size, out->pixels + (out->height - 1 - i) * row_size, row_size);
        memcpy((void*) out->pixels + (out->height - 1 - i) * row_size, temp, row_size);
    }

}

Texture ReadFromPNG(FILE* file, bool flip, bool forceAlign)
{
    Texture out = {.textureFormat = PNG, .channel = IC_BAD};
    fseek(file, 8, SEEK_CUR);
    unsigned chunkLength;
    unsigned char chunkType[4];
    bool useAdam7;

    IDATChunk* dataChunks = NULL;
    size_t dataChunksLength = 0;

    bool endFlag = false;

    while (fread(&chunkLength, sizeof chunkLength, 1, file) == 1 && !endFlag)
    {
        SwapEndian(&chunkLength, sizeof chunkLength);

        fread(chunkType, sizeof(unsigned char), 4, file);
        switch (SwitchPNGChunkType(chunkType))
        {
            case IHDR:
            {
                unsigned info[2];
                fread(info, sizeof info, 1, file);
                SwapEndian(info, sizeof(unsigned));
                SwapEndian(info + 1, sizeof(unsigned));
                out.width = (int) info[0];
                out.height = (int) info[1];
                unsigned char color[2];
                fread(color, sizeof color, 1, file);
                out.depth = color[0];
                if (color[0] == PNG_8_BIT)
                {
                    if (color[1] == PNG_RGB)
                    {
                        out.channel = IC_RGB;
                        out.channelCount = 3;
                    }
                    else if (color[1] == PNG_RGBA)
                    {
                        out.channel = IC_RGBA;
                        out.channelCount = 4;
                    }
                }
                else
                {
                    endFlag = true;
                }

                if (out.channel == IC_BAD)
                {
                    RuntimeErrorLog(InvalidFormat, "This kind of PNG not currently supported!");
                }
                // Just skip 2 bytes (Better encoding method in the future?)
                fseek(file, 2, SEEK_CUR);
                fread(&useAdam7, sizeof useAdam7, 1, file);

            }
                break;
            case PLTE:
                break;
            case IDAT:
                IDATChunk* newChunk = realloc(dataChunks, sizeof(IDATChunk) * (dataChunksLength + 1));
                unsigned char* newData = malloc(sizeof(unsigned char) * chunkLength);
                if (!newData || !newChunk)
                {
                    RuntimeErrorLog(MallocFailed, "Failed to decode png data!");
                    endFlag = true;
                    break;
                }
                dataChunks = newChunk;
                dataChunks[dataChunksLength].chunkLength = chunkLength;
                dataChunks[dataChunksLength].chunkData = newData;
                dataChunksLength++;

                fread(newData, sizeof(unsigned char), chunkLength, file);

                break;
            case IEND:
                size_t size;
                unsigned char* combined = CombineIDATChunks(dataChunks, dataChunksLength, &size);
                auto zlib = CreateZLibFromByteFlow(combined, size);
                size_t outSize;
                auto decompressed = ZLib_Decompress(zlib, &outSize);

                ExtractPixelsFromData(&out, decompressed, outSize, forceAlign);
                ProcessFlip(&out, flip, forceAlign);
                free(combined);
                free(decompressed);
                ZLib_Release(zlib);
                endFlag = true;
            default:
                fseek(file, (int) chunkLength, SEEK_CUR);
        }

        // Just skip CRC code
        fseek(file, 4, SEEK_CUR);
    }
    if (dataChunksLength && dataChunks)
    {
        for (int i = 0; i < dataChunksLength; ++i)
        {
            free(dataChunks[i].chunkData);
        }
        free(dataChunks);
    }

    return out;
}
