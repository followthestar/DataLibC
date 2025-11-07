//
// Created by 18030 on 2025/3/26.
//

#include <malloc.h>
#include "AnimVector2.h"
#include "DataLibC/Anim/Interface/IAnimated.h"

void AddIAnimated_Internal(IAnimated* animated);

struct _AnimVector2
{
    IAnimated super;
    Vector2 start;
    Vector2 end;
    float duration;
    float progress;
    float accumulateTime;
    AnimEffect curveType;

    void* handle;
    AnimCallback callback;
    AnimCallback updateCallback;
};

static bool AnimVector2_Update(AnimVector2* self, float deltaTime)
{
    if (self->progress >= 1.0f)
    {
        self->progress = 1.0f;
        if (self->callback && self->handle)
            self->callback(self->handle);
        return true;
    }
    if (self->updateCallback && self->handle)
        self->updateCallback(self->handle);
    self->accumulateTime += deltaTime;
    self->progress = self->accumulateTime / self->duration;
    self->progress = caclProcess(self->progress, self->curveType);
    return false;
}

static const IAnimatedVTable AnimVector2_VTable = {
        .UpdateValue = (bool (*)(IAnimated*, float)) AnimVector2_Update
};

AnimVector2*
AnimVector2_Create(
        Vector2 begin, Vector2 end, float duration, AnimEffect effect, void* handle, AnimUpdateCallback updateCallback,
        AnimCallback callback)
{
    AnimVector2* self = (AnimVector2*) malloc(sizeof(AnimVector2));
    self->super.vtable = &AnimVector2_VTable;
    self->start = begin;
    self->end = end;
    self->duration = duration;
    self->curveType = effect;
    self->accumulateTime = 0;
    self->handle = handle;
    self->callback = callback;
    self->updateCallback = updateCallback;
    self->progress = 0;

    AddIAnimated_Internal(&self->super);
    return self;
}

Vector2 AnimVector2_GetValue(AnimVector2* self)
{
    Vector2 result = Vector2_Lerp(self->start, self->end, self->progress);
    return result;
}
