//
// Created by 18030 on 2025/3/8.
//

#ifndef DATALIBC_ANIMATIONCURVE_H
#define DATALIBC_ANIMATIONCURVE_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "Keyframe.h"
#include "DataLibC/DS/Iterator.h"

/// An AnimationCurve is a collection of Keyframes that define a curve, representing one attribute of an animation.
typedef struct AnimationCurve AnimationCurve;

typedef struct TimeRange
{
    float begin;
    float end;
} TimeRange;

API_MODULE void* TimeRange_Ctor(const void* obj);

/// Create a new AnimationCurve.
/// \param type The type of the animation curve. This must be the same as the type of the keyframes those will be added to the curve.
/// \return A new AnimationCurve with the given type.
API_MODULE AnimationCurve* CreateAnimationCurve(KeyframeValueType type);

/// Add a keyframe to the animation curve.
/// \param curve The animation curve object.
/// \param keyframe The keyframe object to add, which must be of the same type as the animation curve.
/// \return True if the keyframe is added successfully, false otherwise.
API_MODULE bool AddKeyFrame(AnimationCurve* curve, Keyframe* keyframe);

/// Check if the animation curve is valid. (A valid animation curve has at least two keyframes)
/// \param curve The animation curve object.
/// \return True if the animation curve is valid, false otherwise.
API_MODULE bool IsValidAnimationCurve(AnimationCurve* curve);

/// Get a keyframe from the animation curve.
/// \param curve The animation curve object.
/// \param index The index of the keyframe to get.
/// \return The keyframe object at the given index, or NULL if the index is out of range.
API_MODULE Keyframe* GetKeyFrame(AnimationCurve* curve, int index);

/// Get the number of keyframes in the animation curve.
/// \param curve The animation curve object.
/// \return The number of keyframes in the animation curve.
API_MODULE int GetKeyFrameCount(AnimationCurve* curve);

/// Get an iterator to the first keyframe in the animation curve.
/// \param curve The animation curve object.
/// \return An iterator to the first keyframe in the animation curve.
API_MODULE DynamicArrayIterator AnimationCurveBegin(AnimationCurve* curve);

/// Get a time range of the animation curve.
/// \param curve The animation curve object.
/// \return A time range of the animation curve.
API_MODULE TimeRange AnimationCurveRange(AnimationCurve* curve);

/// Get the type of the animation curve.
/// \param curve The animation curve object.
/// \return The type of the animation curve.
API_MODULE KeyframeValueType AnimationCurveType(AnimationCurve* curve);

/// Evaluate a lerped value with given time
/// \param curve
/// \param out
API_MODULE void AnimationCurveEvaluateClamp(AnimationCurve* curve, float time, void* out);

#endif //DATALIBC_ANIMATIONCURVE_H
