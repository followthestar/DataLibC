//
// Created by 18030 on 2025/2/13.
//

#ifndef DATALIBC_ANIMBOOL_H
#define DATALIBC_ANIMBOOL_H

#include "DataLibC/Tool/ExportModuleMacro.h"

/// Animated boolean value which implements the IAnimated interface.
typedef struct _AnimBool AnimBool;

/// Create an animated boolean value from [startValue] to its opposite value and duration time in seconds.
/// \param startValue The start value of the animated boolean.
/// \param duration The duration time of the animation in seconds.
/// \return The created animated boolean object.
/// \attention InputSystem must be initialized and only in the logical loop can the object be updated.
API_MODULE AnimBool* AnimBool_Create(bool startValue, float duration);

/// Get the current value of the animated boolean.
/// \param self The animated boolean object.
/// \return The current value of the animated boolean.
API_MODULE bool AnimBool_GetValue(AnimBool* self);

#endif //DATALIBC_ANIMBOOL_H
