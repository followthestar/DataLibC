//
// Created by 18030 on 2024/12/14.
//

#include "image_jpg.h"

#define JPG_MARK    0xFF
#define JPG_APP0    0xE0
#define JPG_SOI     0xD8
#define JPG_EOI     0xD9
#define JPG_DQT     0xDB
#define JPG_DHT     0xC4
#define JPG_SOS     0xDA


Texture ReadFromJPG(FILE* file, bool flip, bool forceAlign)
{
    Texture result = {.textureFormat = JPG, .channel = IC_RGB};
    fseek(file, 2, SEEK_CUR);

    unsigned char buf[2];
    unsigned short segmentLength;
    while (fread(buf, sizeof(char), 2, file) == 2)
    {
        if (buf[0] != JPG_MARK)
        {
            result.textureFormat = ERR;
            break;
        }

        if (buf[1] == JPG_EOI)
            break;

        fread(&segmentLength, sizeof(unsigned short), 1, file);
        segmentLength = (segmentLength << 8) | (segmentLength >> 8) - 2;

        switch (buf[1])
        {
            case JPG_APP0:
                break;
            case JPG_DHT:
                break;
            case JPG_SOS:
                break;
            case JPG_DQT:

                
                break;
            default:
                fseek(file, segmentLength, SEEK_CUR);
        }
    }

    return result;
}
