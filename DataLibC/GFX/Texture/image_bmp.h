//
// Created by 18030 on 2024/12/14.
//

#ifndef DATALIBC_IMAGE_BMP_H
#define DATALIBC_IMAGE_BMP_H

#include <stdio.h>
#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/GFX/Texture/Texture.h"

typedef enum BitmapType
{
    BlackWhite,
    GrayScale,
    FalseColor,
    TrueColor
} BitmapType;

struct LIB_STRUCT Bitmap
{
    BitmapType type;
    size_t fileSize;
    size_t dataSize;
    int width;
    int height;
    ImageChannel channel;
};


typedef struct Bitmap BmpData;

API_MODULE Texture ReadFromBMP(FILE* file, bool flip, bool forceAlign);

#endif //DATALIBC_IMAGE_BMP_H
