//
// Created by 18030 on 2025/1/16.
//

#include <malloc.h>
#include "AnimFloat.h"


void AddIAnimated_Internal(IAnimated* animated);

struct _AnimFloat
{
    IAnimated super;
    float minValue;
    float maxValue;
    float duration;
    float progress;
    float accumulateTime;
    AnimEffect curveType;
};


static bool AnimFloat_Update(AnimFloat* self, float deltaTime)
{
    if (self->progress >= 1.0f)
    {
        self->progress = 1.0f;
        return true;
    }
    self->accumulateTime += deltaTime;
    self->progress = self->accumulateTime / self->duration;
    self->progress = caclProcess(self->progress, self->curveType);
    return false;
}

static const IAnimatedVTable AnimFloat_VTable = {
        .UpdateValue = (bool (*)(IAnimated*, float)) AnimFloat_Update
};

AnimFloat* AnimFloat_Create(float minValue, float maxValue, float duration, AnimEffect effect)
{
    AnimFloat* animFloat = (AnimFloat*) malloc(sizeof(AnimFloat));
    animFloat->super.vtable = &AnimFloat_VTable;
    animFloat->progress = 0;
    animFloat->minValue = minValue;
    animFloat->maxValue = maxValue;
    animFloat->duration = duration;
    animFloat->curveType = effect;
    animFloat->accumulateTime = 0;
    AddIAnimated_Internal((IAnimated*) animFloat);
    return animFloat;
}

float AnimFloat_GetValue(AnimFloat* self)
{
    if (self->progress >= 1.0f)
        return self->maxValue;
    float curr = self->minValue + (self->maxValue - self->minValue) * self->progress;
    return curr;
}