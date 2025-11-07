//
// Created by 18030 on 2025/1/7.
//

#ifndef DATALIBC_ANIMINT_H
#define DATALIBC_ANIMINT_H

#include "DataLibC/Tool/ExportModuleMacro.h"

/// Animated integer value which implements the IAnimated interface.
typedef struct _AnimInt AnimInt;

/// Create an animated integer value within the range of [minValue, maxValue] and duration time in seconds.
/// \param minValue The minimum value of the animated integer.
/// \param maxValue The maximum value of the animated integer.
/// \param duration The duration time of the animation in seconds.
/// \return The created animated integer object.
/// \attention InputSystem must be initialized and only in the logical loop can the object be updated.
API_MODULE AnimInt* AnimInt_Create(int minValue, int maxValue, float duration);

/// Get the current value of the animated integer.
/// \param self The animated integer object.
/// \return The current value of the animated integer.
API_MODULE int AnimInt_GetValue(AnimInt* self);

#endif //DATALIBC_ANIMINT_H
