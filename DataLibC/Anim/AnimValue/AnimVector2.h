//
// Created by 18030 on 2025/3/26.
//

#ifndef DATALIBC_ANIMVECTOR2_H
#define DATALIBC_ANIMVECTOR2_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/LA/Vector/Vector2.h"
#include "DataLibC/Anim/Interface/IAnimated.h"

typedef void (* AnimCallback)(void* handle);

typedef void (* AnimUpdateCallback)(void* handle);

/// Animated vector2 value which implements the IAnimated interface.
typedef struct _AnimVector2 AnimVector2;

/// Create an animated vector2 value from begin to end and duration time in seconds.
/// \param begin The start value of the animated vector2.
/// \param end The target value of the animated vector2.
/// \param duration The duration time of the animation in seconds.
/// \param effect The effect of the animation.
/// \param handle The handle used to pass to the callback.
/// \param callback When the animation is finished, the callback will be called.
/// \return The created animated vector2 object.
/// \attention InputSystem must be initialized and only in the logical loop can the object be updated.
API_MODULE AnimVector2*
AnimVector2_Create(
        Vector2 begin, Vector2 end, float duration, AnimEffect effect, void* handle, AnimUpdateCallback updateCallback,
        AnimCallback finishedCallback);

/// Get the current value of the animated vector2.
/// \param self The animated vector2 object.
/// \return The current value of the animated vector2.
API_MODULE Vector2 AnimVector2_GetValue(AnimVector2* self);

#endif //DATALIBC_ANIMVECTOR2_H
