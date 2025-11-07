//
// Created by 18030 on 2025/3/8.
//

#ifndef DATALIBC_KEYFRAME_H
#define DATALIBC_KEYFRAME_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/LA/Vector/Vector3.h"
#include "DataLibC/LA/Vector/Vector2.h"

typedef enum KeyframeValueType
{
    kKeyFrameFloat,
    kKeyFrameVector2,
    kKeyFrameVector3,
    kKeyFrameBool,
    kKeyFrameObject,
} KeyframeValueType;

typedef struct KeyframView
{
    float time;
    KeyframeValueType type;
    const void* value;
} KeyframView;

/// Compare two Keyframe objects.
/// \param k1 A pointer to Keyframe*. (Means Keyframe** k1)
/// \param k2 A pointer to Keyframe*  (Means Keyframe** k2)
/// \return True if the time of k1 is less than k2.
API_MODULE bool KeyframeCompare(void* k1, void* k2);

/// A Keyframe represents a single key point in the time line.
typedef struct Keyframe Keyframe;

/// Create a Keyframe with float value.
/// \param time The time of the key point.
/// \param value The value of the key point.
/// \return The created Keyframe object.
API_MODULE Keyframe* CreateKeyframeFloat(float time, float value);

/// Create a Keyframe with bool value.
/// \param time The time of the key point.
/// \param value The value of the key point.
/// \return The created Keyframe object.
API_MODULE Keyframe* CreateKeyframeBool(float time, bool value);

/// Create a Keyframe with Vector2 value.
/// \param time The time of the key point.
/// \param value The value contains [x, y] of the key point.
/// \return The created Keyframe object.
API_MODULE Keyframe* CreateKeyframeVector2(float time, Vector2 value);

/// Create a Keyframe with Vector3 value.
/// \param time The time of the key point.
/// \param value The value contains [x, y, z] of the key point.
/// \return The created Keyframe object.
API_MODULE Keyframe* CreateKeyframeVector3(float time, Vector3 value);

/// Create a Keyframe with object value.
/// \param time The time of the key point.
/// \param value The value of the key point, which should be a pointer to the object.
/// \return The created Keyframe object.
API_MODULE Keyframe* CreateKeyframeObject(float time, void* value);

/// Delete a Keyframe object.
/// \param keyframe The Keyframe object to be deleted.
API_MODULE void DeleteKeyframe(Keyframe* keyframe);

/// Copy a Keyframe object.
/// \param keyframe The Keyframe object to be copied.
/// \return The copied Keyframe object.
API_MODULE Keyframe* CopyFromKeyframe(Keyframe* keyframe);

/// Get the view of a Keyframe object.
/// \param keyframe The keyframe you want to view
/// \return The view of the Keyframe object. The member "value" is a const pointer to the value of the real value in the keyframe.
API_MODULE KeyframView ViewKeyframe(Keyframe* keyframe);

#endif //DATALIBC_KEYFRAME_H
