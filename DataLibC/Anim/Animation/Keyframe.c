//
// Created by 18030 on 2025/3/8.
//

#include <stddef.h>
#include <memory.h>
#include "Keyframe.h"
#include "DataLibC/Allocator/MemoryPool.h"

typedef struct Keyframe
{
    float time;
    KeyframeValueType type;
    union
    {
        float floatValue;
        bool boolValue;
        Vector2 vector2Value;
        Vector3 vector3Value;
        void* objectValue;
    };
    union
    {
        float floatInSlope;
        Vector2 vector2InSlope;
        Vector3 vector3InSlope;
        void* objectInSlope;
    } inSlope;
    union
    {
        float floatOutSlope;
        Vector2 vector2OutSlope;
        Vector3 vector3OutSlope;
        void* objectOutSlope;
    } outSlope;
} Keyframe;

const size_t gKeyframeSize = sizeof(Keyframe);


bool KeyframeCompare(void* k1, void* k2)
{
    return (*(Keyframe**) k1)->time < (*(Keyframe**) k2)->time;
}

MemoryPool* GetKeyframePool();

static void KeyframeInitInternal(Keyframe* keyframe, float time)
{
    keyframe->time = time;
    memset(&keyframe->inSlope, 0, sizeof(keyframe->inSlope));
    memset(&keyframe->outSlope, 0, sizeof(keyframe->outSlope));
}


Keyframe* CreateKeyframeFloat(float time, float value)
{
    Keyframe* keyframe = MemoryPoolAllocate(GetKeyframePool());
    keyframe->type = kKeyFrameFloat;
    keyframe->floatValue = value;
    KeyframeInitInternal(keyframe, time);
    return keyframe;
}

Keyframe* CreateKeyframeBool(float time, bool value)
{
    Keyframe* keyframe = MemoryPoolAllocate(GetKeyframePool());
    keyframe->type = kKeyFrameBool;
    keyframe->boolValue = value;
    KeyframeInitInternal(keyframe, time);
    return keyframe;
}


Keyframe* CreateKeyframeVector2(float time, Vector2 value)
{
    Keyframe* keyframe = MemoryPoolAllocate(GetKeyframePool());
    keyframe->type = kKeyFrameVector2;
    keyframe->vector2Value = value;
    KeyframeInitInternal(keyframe, time);
    return keyframe;
}

Keyframe* CreateKeyframeVector3(float time, Vector3 value)
{
    Keyframe* keyframe = MemoryPoolAllocate(GetKeyframePool());
    keyframe->type = kKeyFrameVector3;
    keyframe->vector3Value = value;
    KeyframeInitInternal(keyframe, time);
    return keyframe;
}

Keyframe* CreateKeyframeObject(float time, void* value)
{
    Keyframe* keyframe = MemoryPoolAllocate(GetKeyframePool());
    keyframe->type = kKeyFrameObject;
    keyframe->objectValue = value;
    KeyframeInitInternal(keyframe, time);
    return keyframe;
}

void DeleteKeyframe(Keyframe* keyframe)
{
    MemoryPoolDeallocate(GetKeyframePool(), keyframe);
}

Keyframe* CopyFromKeyframe(Keyframe* keyframe)
{
    Keyframe* newKeyframe = MemoryPoolAllocate(GetKeyframePool());
    *newKeyframe = *keyframe;
    return newKeyframe;
}

KeyframView ViewKeyframe(Keyframe* keyframe)
{
    return (KeyframView) {.time = keyframe->time, .type = keyframe->type, .value = &keyframe->objectValue};
}



