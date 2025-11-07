//
// Created by 18030 on 2025/3/9.
//

#ifndef DATALIBC_ANIMATIONCLIP_H
#define DATALIBC_ANIMATIONCLIP_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "AnimationCurve.h"
#include "DataLibC/DS/String/String.h"

/// An AnimationClip contains a series of AnimationCurves that are bound to a property by a name path.
/// The property path is decided by the user, thus user should process the animation logic by themselves.
typedef struct AnimationClip AnimationClip;

/// Create a new AnimationClip.
/// \param clipName The name of the AnimationClip.
/// \return The created AnimationClip.
API_MODULE AnimationClip* CreateAnimationClip(const char* clipName);

/// Get the name of the AnimationClip.
/// \param clip The AnimationClip to get the name.
/// \return The name of the AnimationClip.
API_MODULE const String* GetAnimationClipName(AnimationClip* clip);

/// Returns the smallest and largest keyframe time of any channel in the animation
/// \param clip The AnimationClip to get the time range.
/// \return The time range of this clip. If no keyframe is found, the range will be (_Infinite, _Infinite)
API_MODULE TimeRange GetAnimationClipRange(AnimationClip* clip);

/// Add a curve to the AnimationClip if the given curve is not exist in the AnimationClip.
/// \param clip The AnimationClip to add the AnimationCurve.
/// \param path The path of the object to bind the AnimationCurve. Such as "Base/Child/Object". This is decided by the user.
/// \param property The concrete property of the object to bind the curve. Such as "position.x", which may means the x component of Vector3.
/// \param curve The AnimationCurve to add.
API_MODULE void SetAnimationCurve(AnimationClip* clip, const char* path, const char* property, AnimationCurve* curve);

/// Add a position curve to the AnimationClip if the given curve is not exist in the AnimationClip.
/// \param clip The AnimationClip to add the AnimationCurve.
/// \param path The path of the object to bind the AnimationCurve.
/// \param curve The AnimationCurve to add.
API_MODULE bool AddPositionCurve(AnimationClip* clip, const char* path, AnimationCurve* curve);

/// Add a rotation curve to the AnimationClip if the given curve is not exist in the AnimationClip.
/// \param clip The AnimationClip to add the AnimationCurve.
/// \param path The path of the object to bind the AnimationCurve.
/// \param curve The AnimationCurve to add.
API_MODULE bool AddRotationCurve(AnimationClip* clip, const char* path, AnimationCurve* curve);

/// Add a scale curve to the AnimationClip if the given curve is not exist in the AnimationClip.
/// \param clip The AnimationClip to add the AnimationCurve.
/// \param path The path of the object to bind the AnimationCurve.
/// \param curve The AnimationCurve to add.
API_MODULE bool AddScaleCurve(AnimationClip* clip, const char* path, AnimationCurve* curve);

/// Add a skew curve to the AnimationClip if the given curve is not exist in the AnimationClip.
/// \param clip The AnimationClip to add the AnimationCurve.
/// \param path The path of the object to bind the AnimationCurve.
/// \param curve The AnimationCurve to add.
API_MODULE bool AddSkewCurve(AnimationClip* clip, const char* path, AnimationCurve* curve);

/// Get all position curves in the AnimationClip.
/// \param clip The AnimationClip to get the curves.
/// \return A pointer to a DynamicArray object.
API_MODULE DynamicArray* GetPositionCurves(AnimationClip* clip);

/// Get all rotation curves in the AnimationClip.
/// \param clip The AnimationClip to get the curves.
/// \return A pointer to a DynamicArray object.
API_MODULE DynamicArray* GetRotationCurves(AnimationClip* clip);

/// Get all scale curves in the AnimationClip.
/// \param clip The AnimationClip to get the curves.
/// \return A pointer to a DynamicArray object.
API_MODULE DynamicArray* GetScaleCurves(AnimationClip* clip);

/// Get all skew curves in the AnimationClip.
/// \param clip The AnimationClip to get the curves.
/// \return A pointer to a DynamicArray object.
API_MODULE DynamicArray* GetSkewCurves(AnimationClip* clip);


#endif //DATALIBC_ANIMATIONCLIP_H
