//
// Created by 18030 on 2024/12/14.
//

#ifndef DATALIBC_IMAGE_PNG_H
#define DATALIBC_IMAGE_PNG_H

#include <stdio.h>
#include "DataLibC/Tool/ExportModuleMacro.h"
#include "Texture.h"

struct LIB_STRUCT PNGraphics
{

};

typedef struct PNGraphics PNGraphics;

Texture ReadFromPNG(FILE* file, bool flip, bool forceAlign);

#endif //DATALIBC_IMAGE_PNG_H
