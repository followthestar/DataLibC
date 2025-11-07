//
// Created by 18030 on 2025/1/7.
//

#ifndef DATALIBC_IANIMATED_H
#define DATALIBC_IANIMATED_H

#include "DataLibC/Tool/ExportModuleMacro.h"

typedef struct _IAnimated IAnimated;
typedef struct _IAnimatedVTable IAnimatedVTable;


typedef enum AnimEffect
{
    kLinear,
    kEaseIn,
    kEaseOut,
} AnimEffect;

struct _IAnimatedVTable
{
    bool (* const UpdateValue)(IAnimated* self, float deltaTime);
};

float easeInOutCubic(float t);

float easeOutCubic(float t);

float caclProcess(float originProcess, AnimEffect effect);

LIB_INTERFACE _IAnimated
{
    const IAnimatedVTable* vtable;
};

/// Destroy the IAnimated object
/// \param self The IAnimated object to be destroyed
API_MODULE void IAnimated_Destory(void* self);


#endif //DATALIBC_IANIMATED_H
