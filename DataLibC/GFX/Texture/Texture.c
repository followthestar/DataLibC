//
// Created by 18030 on 2024/12/14.
//

#include "Texture.h"
#include "DataLibC/Tool/LogMacro.h"
#include "DataLibC/GFX/Texture/image_bmp.h"
#include "image_jpg.h"
#include "image_png.h"
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>


#define BMP_SIGNATURE "BM"
#define JPG_SIGNATURE "\xFF\xD8\xFF"
#define PNG_SIGNATURE "\x89PNG\x0D\x0A\x1A\x0A"

static Texture error = {-1, ERR, 0, 0, 0, 0, 0, NULL};


static TextureFormat SwitchTextureFormat_Internal(const unsigned char* header)
{
    if (memcmp(header, BMP_SIGNATURE, 2) == 0)
        return BMP;
    if (memcmp(header, JPG_SIGNATURE, 3) == 0)
        return JPG;
    if (memcmp(header, PNG_SIGNATURE, 8) == 0)
        return PNG;
    return ERR;
}

Texture CreateTexture(const char* filePath, bool flip, bool forceAlign)
{
    FILE* texFile = fopen(filePath, "rb");
    if (!texFile)
    {
        RuntimeErrorLog(OpenFileFailed, "Failed to open picture file at path %s!", filePath);
        return error;
    }
    unsigned char header[8];
    size_t bytesRead = fread(header, 1, 8, texFile);
    if (bytesRead < 8)
    {
        RuntimeErrorLog(InvalidFormat, "The format of the picture file at path %s is invalid!", filePath);
        fclose(texFile);
        return error;
    }
    fseek(texFile, 0, SEEK_SET);

    Texture texture;
    switch (SwitchTextureFormat_Internal(header))
    {
        case ERR:
            RuntimeErrorLog(UnknownData, "The format of the picture file at path %s is unknown!", filePath);
            texture = error;
            break;
        case BMP:
            texture = ReadFromBMP(texFile, flip, forceAlign);
            break;
        case JPG:
            texture = ReadFromJPG(texFile, flip, forceAlign);
            break;
        case PNG:
            texture = ReadFromPNG(texFile, flip, forceAlign);
            break;
    }
    fclose(texFile);
    return texture;
}

void DeleteTexture(Texture* texture)
{
    texture->id = 0;
    free((void*) texture->pixels);
    texture->pixels = NULL;
}
