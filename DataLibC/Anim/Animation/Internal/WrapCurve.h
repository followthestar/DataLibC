//
// Created by 18030 on 2025/3/20.
//

#ifndef DATALIBC_WRAPCURVE_H
#define DATALIBC_WRAPCURVE_H

#include "DataLibC/DS/String/String.h"
#include "DataLibC/Anim/Animation/AnimationCurve.h"

typedef struct FloatCurve
{
    String path;
    String attribute;
    AnimationCurve* curve;
} FloatCurve;

typedef struct Vector2Curve
{
    String path;
    AnimationCurve* curve;
} Vector2Curve;

#endif //DATALIBC_WRAPCURVE_H
