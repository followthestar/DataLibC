//
// Created by 18030 on 2025/2/13.
//

#include <malloc.h>
#include "AnimBool.h"
#include "DataLibC/Anim/Interface/IAnimated.h"

void AddIAnimated_Internal(IAnimated* animated);

struct _AnimBool
{
    IAnimated super;
    bool value;
    float duration;
    float progress;
};

static bool AnimBool_Update(AnimBool* self, float deltaTime)
{
    if (self->progress >= 1.0f)
    {
        self->progress = 1.0f;
        self->value = !self->value;
        return true;
    }
    self->progress += deltaTime / self->duration;
    return false;
}

static const IAnimatedVTable AnimInt_VTable = {
        .UpdateValue = (bool (*)(IAnimated*, float)) AnimBool_Update
};

AnimBool* AnimBool_Create(bool startValue, float duration)
{
    AnimBool* animBool = (AnimBool*) malloc(sizeof(AnimBool));
    animBool->super.vtable = &AnimInt_VTable;
    animBool->progress = 0;
    animBool->value = startValue;
    animBool->duration = duration;

    AddIAnimated_Internal((IAnimated*) animBool);
    return animBool;
}

bool AnimBool_GetValue(AnimBool* self)
{
    return self->value;
}