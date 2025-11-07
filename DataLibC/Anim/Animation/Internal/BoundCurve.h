//
// Created by 18030 on 2025/3/17.
//

#ifndef DATALIBC_BOUNDCURVE_H
#define DATALIBC_BOUNDCURVE_H

#include "DataLibC/Anim/Animation/Keyframe.h"

typedef enum BoundCurveType
{
    kUnbind,
    kBindLocalPosition,
    kBindLocalRotation,
    kBindLocalScale,
    kBindLocalSkew,
    kBindEnabled
} BoundCurveType;

typedef struct BoundCurve
{
    // The ptr to the data we are writing into
    void* targetPtr;
    void* curve;
    BoundCurveType targetType;
} BoundCurve;

#endif //DATALIBC_BOUNDCURVE_H
