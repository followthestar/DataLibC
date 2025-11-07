//
// Created by 18030 on 2024/12/14.
//

#ifndef DATALIBC_IMAGE_JPG_H
#define DATALIBC_IMAGE_JPG_H

#include <stdio.h>
#include "DataLibC/Tool/ExportModuleMacro.h"
#include "Texture.h"

struct LIB_STRUCT JPEG
{

};

typedef struct JPEG JPEG;

API_MODULE Texture ReadFromJPG(FILE* file, bool flip, bool forceAlign);

#endif //DATALIBC_IMAGE_JPG_H
