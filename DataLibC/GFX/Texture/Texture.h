//
// Created by 18030 on 2024/12/14.
//

#ifndef DATALIBC_TEXTURE_H
#define DATALIBC_TEXTURE_H

#include "DataLibC/Tool/ExportModuleMacro.h"

typedef enum TextureFormat
{
    ERR = -1,
    BMP,
    JPG,
    PNG
} TextureFormat;

typedef enum ImageChannel
{
    IC_BAD = -1,
    IC_BW = 1,
    IC_INDEX = 4,
    IC_GRAY = 8,
    IC_RGB = 24,
    IC_RGBA = 32
} ImageChannel;

struct LIB_STRUCT Texture
{
    unsigned id;
    TextureFormat textureFormat;
    int width;
    int height;
    ImageChannel channel;
    int depth;
    int channelCount;

    const unsigned char* pixels;
};

typedef struct Texture Texture;

/// Create a Texture object which contains basic info about an image and an ID maybe used by generating GPU texture buffer
/// \param filePath The path to your image file
/// \param flip Whether to flip the image data. (Default is false, the color info data will start from the bottom left corner
/// \param forceAlign If true, the color info data will retain padding bytes to ensure that each row is aligned with 4 bytes
/// \return A Texture object
API_MODULE Texture CreateTexture(const char* filePath, bool flip, bool forceAlign);

API_MODULE void DeleteTexture(Texture* texture);

#endif //DATALIBC_TEXTURE_H
