//
// Created by 18030 on 2025/2/27.
//

#ifndef DATALIBC_COLOR_H
#define DATALIBC_COLOR_H

#include <stdint.h>
#include "DataLibC/Tool/ExportModuleMacro.h"

#define COLOR(r, g, b, a) (Color) {r, g, b, a}
#define COLOR32(r, g, b, a) (Color32) {r, g, b, a}

#define COLOR_WHITE COLOR(1.0f, 1.0f, 1.0f, 1.0f)
#define COLOR_BLACK COLOR(0.0f, 0.0f, 0.0f, 1.0f)
#define COLOR_RED COLOR(1.0f, 0.0f, 0.0f, 1.0f)
#define COLOR_GREEN COLOR(0.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_BLUE COLOR(0.0f, 0.0f, 1.0f, 1.0f)

typedef struct _Color Color;
typedef struct _Color32 Color32;

struct LIB_STRUCT _Color
{
    float r;
    float g;
    float b;
    float a;
};

struct LIB_STRUCT _Color32
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

#endif //DATALIBC_COLOR_H
