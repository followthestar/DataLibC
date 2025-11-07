//
// Created by 18030 on 2025/10/12.
//

#ifndef COLOR32_H
#define COLOR32_H
#include "TypeMacro.h"

#define COLOR32(r,g,b,a) ((Color32){r,g,b,a})

#define COLOR32_RED COLOR32(255,0,0,255)

#define COLOR32_GREEN COLOR32(0,255,0,255)

#define COLOR32_BLUE COLOR32(0,0,255,255)

#define COLOR32_WHITE COLOR32(255,255,255,255)

#define COLOR32_BLACK COLOR32(0,0,0,255)

#define COLOR32_TRANSPARENT COLOR32(0,0,0,0)

typedef struct Color32
{
    UInt8 r;
    UInt8 g;
    UInt8 b;
    UInt8 a;
} Color32;


#endif //COLOR32_H
