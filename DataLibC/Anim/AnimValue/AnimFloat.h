//
// Created by 18030 on 2025/1/16.
//

#ifndef DATALIBC_ANIMFLOAT_H
#define DATALIBC_ANIMFLOAT_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/Anim/Interface/IAnimated.h"

/// Animated float value which implements IAnimValue interface.
typedef struct _AnimFloat AnimFloat;

/// Create an animated float value within the range of [minValue, maxValue] and duration time
/// in seconds.
/// \param minValue The minimum value of the animated float value.
/// \param maxValue The maximum value of the animated float value.
/// \param duration The duration time of the animated float value.
/// \return The created animated float object.
/// \attention InputSystem must be initialized and only in the logical loop can the object be updated.
API_MODULE AnimFloat* AnimFloat_Create(float minValue, float maxValue, float duration, AnimEffect effect);

/// Get the current value of the animated float.
/// \param self The animated float object.
/// \return The current value of the animated float.
API_MODULE float AnimFloat_GetValue(AnimFloat* self);

#endif //DATALIBC_ANIMFLOAT_H
