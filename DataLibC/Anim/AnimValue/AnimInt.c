//
// Created by 18030 on 2025/1/7.
//

#include <malloc.h>
#include "AnimInt.h"
#include "DataLibC/Anim/Interface/IAnimated.h"

void AddIAnimated_Internal(IAnimated* animated);

struct _AnimInt
{
    IAnimated super;
    int minValue;
    int maxValue;
    float duration;
    float progress;
};

static bool AnimInt_Update(AnimInt* self, float deltaTime)
{
    if (self->progress >= 1.0f)
    {
        self->progress = 1.0f;
        return true;
    }
    self->progress += deltaTime / self->duration;
    return false;
}

static const IAnimatedVTable AnimInt_VTable = {
        .UpdateValue = (bool (*)(IAnimated*, float)) AnimInt_Update
};

AnimInt* AnimInt_Create(int minValue, int maxValue, float duration)
{
    AnimInt* animInt = (AnimInt*) malloc(sizeof(AnimInt));
    animInt->super.vtable = &AnimInt_VTable;
    animInt->progress = 0;
    animInt->minValue = minValue;
    animInt->maxValue = maxValue;
    animInt->duration = duration;

    AddIAnimated_Internal((IAnimated*) animInt);
    return animInt;
}

int AnimInt_GetValue(AnimInt* self)
{
    float curr = (float) self->minValue + (float) (self->maxValue - self->minValue) * self->progress;
    return curr > (float) self->maxValue ? self->maxValue : (int) curr;
}
