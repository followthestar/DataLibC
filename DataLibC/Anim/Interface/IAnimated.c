//
// Created by 18030 on 2025/2/13.
//
#include "IAnimated.h"

void AddIAnimatedDestroy_Internal(IAnimated* animated);

float easeInOutCubic(float t)
{
    if (t < 0.5)
        return 4 * t * t * t;
    else
        t -= 1;
    return 1 + 4 * t * t * t;
}

void IAnimated_Destory(void* self)
{
    AddIAnimatedDestroy_Internal(self);
}

float easeOutCubic(float t)
{
    t -= 1;
    return 1 + t * t * t;
}

float caclProcess(float originProcess, AnimEffect effect)
{
    switch (effect)
    {
        case kEaseIn:
        {
            float p = easeInOutCubic(originProcess);
            return p < 0 ? 0 : (p > 1 ? 1 : p);
        }
        case kEaseOut:
        {
            float p = easeOutCubic(originProcess);
            return p < 0 ? 0 : (p > 1 ? 1 : p);
        }
        default:
            break;
    }
    return originProcess;
}

