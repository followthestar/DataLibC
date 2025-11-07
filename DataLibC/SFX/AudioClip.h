//
// Created by 18030 on 2024/12/15.
//

#ifndef DATALIBC_AUDIOCLIP_H
#define DATALIBC_AUDIOCLIP_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include <stddef.h>

typedef enum AudioFormat
{
    INVALID,
    WAV,
    MAP3,
    FLAC
} AudioFormat;

struct LIB_STRUCT AudioClip
{
    AudioFormat format;
    size_t length;
    size_t formatPtr;
    const unsigned char* data;
};

typedef struct AudioClip AudioClip;
#endif //DATALIBC_AUDIOCLIP_H
