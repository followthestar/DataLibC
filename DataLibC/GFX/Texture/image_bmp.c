//
// Created by 18030 on 2024/12/14.
//

#include "image_bmp.h"
#include <windows.h>

static constexpr int sRgbQuadsAmount[] = {[IC_BW] = 2, [IC_INDEX] = 16, [IC_GRAY] = 256};

static void ExtractColorInfoWithRGBQuad(Texture* out, FILE* file, bool flip, bool forceAlign)
{
    int rgbQuadAmount = sRgbQuadsAmount[out->channel];
    RGBQUAD rgbQuads[rgbQuadAmount];
    fread(rgbQuads, sizeof(RGBQUAD), rgbQuadAmount, file);

    unsigned int bytesPerLine = ((out->width * out->channel + 31) / 32) * 4;
    size_t imageDataSize = out->width * out->height * 3;
    size_t dataSize = forceAlign ? bytesPerLine * out->height : imageDataSize;
    unsigned char* data = malloc(sizeof(unsigned char) * dataSize);
    if (!data)
    {
        out->textureFormat = ERR;
        return;
    }

    unsigned char lineBuffer[bytesPerLine];
    for (int i = 0; i < out->height; ++i)
    {
        fread(lineBuffer, 1, bytesPerLine, file);
        for (int pixel = 0; pixel < out->width; ++pixel)
        {
            int byteIndex = pixel * out->channel / 8;
            int bitShift = (8 - out->channel) - ((pixel % (8 / out->channel)) * out->channel);
            unsigned char index = (lineBuffer[byteIndex] >> bitShift) & ((1 << out->channel) - 1);
            if (forceAlign)
            {
                data[i * bytesPerLine + pixel * 3] = rgbQuads[index].rgbRed;
                data[i * bytesPerLine + pixel * 3 + 1] = rgbQuads[index].rgbGreen;
                data[i * bytesPerLine + pixel * 3 + 2] = rgbQuads[index].rgbBlue;
            }
            else
            {
                data[(i * out->width + pixel) * 3] = rgbQuads[index].rgbRed;
                data[(i * out->width + pixel) * 3 + 1] = rgbQuads[index].rgbGreen;
                data[(i * out->width + pixel) * 3 + 2] = rgbQuads[index].rgbBlue;
            }
        }
    }
    if (flip)
    {
        unsigned char tempLine[bytesPerLine];
        for (int i = 0; i < out->height / 2; ++i)
        {
            memcpy(tempLine, &data[i * bytesPerLine], bytesPerLine);
            memcpy(&data[i * bytesPerLine], &data[(out->height - 1 - i) * bytesPerLine], bytesPerLine);
            memcpy(&data[(out->height - 1 - i) * bytesPerLine], tempLine, bytesPerLine);
        }
    }
    out->pixels = data;
}

static void ExtractColorInfoWithoutRGBQuad(Texture* out, FILE* file, bool flip, bool forceAlign)
{
    unsigned int bytesPerLine = ((out->width * out->channel + 31) / 32) * 4;
    size_t imageDataSize = out->width * out->height * (out->channel / 4);
    size_t dataSize = forceAlign ? bytesPerLine * out->height : imageDataSize;
    unsigned char* data = malloc(sizeof(unsigned char) * dataSize);
    if (!data)
    {
        out->textureFormat = ERR;
        return;
    }
    unsigned char lineBuffer[bytesPerLine];
    int bytesPerPixel = out->channel / 8;
    for (int i = 0; i < out->height; ++i)
    {
        fread(lineBuffer, 1, bytesPerLine, file);
        if (forceAlign)
        {
            for (int pixel = 0; pixel < out->width; ++pixel)
            {
                int byteIndex = pixel * bytesPerPixel;
                data[i * bytesPerLine + byteIndex] = lineBuffer[byteIndex + 2];
                data[i * bytesPerLine + byteIndex + 1] = lineBuffer[byteIndex + 1];
                data[i * bytesPerLine + byteIndex + 2] = lineBuffer[byteIndex];
                if (out->channel == IC_RGBA)
                    data[i * bytesPerLine + byteIndex + 3] = lineBuffer[byteIndex + 3];
            }
        }
        else
        {
            for (int pixel = 0; pixel < out->width; ++pixel)
            {
                int byteIndex = (i * out->width + pixel) * bytesPerPixel;
                data[byteIndex] = lineBuffer[pixel * bytesPerPixel + 2];
                data[byteIndex + 1] = lineBuffer[pixel * bytesPerPixel + 1];
                data[byteIndex + 2] = lineBuffer[pixel * bytesPerPixel];
                if (out->channel == IC_RGBA)
                    data[byteIndex + 3] = lineBuffer[pixel * bytesPerPixel + 3];
            }
        }
    }
    if (flip)
    {
        unsigned char tempLine[bytesPerLine];
        for (int i = 0; i < out->height / 2; ++i)
        {
            memcpy(tempLine, &data[i * bytesPerLine], bytesPerLine);
            memcpy(&data[i * bytesPerLine], &data[(out->height - 1 - i) * bytesPerLine], bytesPerLine);
            memcpy(&data[(out->height - 1 - i) * bytesPerLine], tempLine, bytesPerLine);
        }
    }
    out->pixels = data;
}


Texture ReadFromBMP(FILE* file, bool flip, bool forceAlign)
{
    Texture out = {.textureFormat = BMP};
    BITMAPFILEHEADER bmfh;
    BITMAPINFOHEADER bmih;
    fread(&bmfh, sizeof bmfh, 1, file);
    fread(&bmih, sizeof bmih, 1, file);
    out.id = 0;
    out.height = bmih.biHeight;
    out.width = bmih.biWidth;
    out.channel = (ImageChannel) bmih.biBitCount;
    out.depth = 8;
    out.channelCount = out.channel / 8;
    if (out.channel == IC_RGB || out.channel == IC_RGBA)
        ExtractColorInfoWithoutRGBQuad(&out, file, flip, forceAlign);
    else
        ExtractColorInfoWithRGBQuad(&out, file, flip, forceAlign);
    return out;
}
