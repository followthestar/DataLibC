//
// Created by 18030 on 2025/3/9.
//

#include <malloc.h>
#include <string.h>
#include "AnimationClip.h"
#include "DataLibC/DS/List/DynamicArray.h"
#include "DataLibC/Tool/Misc.h"
#include "DataLibC/DS/String/String.h"
#include <float.h>
#include <math.h>

#include "Internal/WrapCurve.h"

typedef struct AnimationClip
{
    float m_SampleRate;
    bool m_Compressed;
    String m_ClipName;

    DynamicArray m_FloatCurves;
    DynamicArray m_PositionCurves;
    DynamicArray m_RotationCurves;
    DynamicArray m_ScaleCurves;
    DynamicArray m_SkewCurves;
} AnimationClip;

AnimationClip* CreateAnimationClip(const char* clipName)
{
    AnimationClip* clip = (AnimationClip*) malloc(sizeof(AnimationClip));
    String_Init(&clip->m_ClipName, strlen(clipName), clipName);
    DynamicArray_Init(&clip->m_PositionCurves, 0, kPOINTER, 0);
    DynamicArray_Init(&clip->m_RotationCurves, 0, kPOINTER, 0);
    DynamicArray_Init(&clip->m_ScaleCurves, 0, kPOINTER, 0);
    DynamicArray_Init(&clip->m_SkewCurves, 0, kPOINTER, 0);
    DynamicArray_Init(&clip->m_FloatCurves, 0, kPOINTER, 0);

    return clip;
}

void SetAnimationCurve(AnimationClip* clip, const char* path, const char* property, AnimationCurve* curve)
{
    // Here we will differentiate the curve type and create the curve object.
    if (IsStringStartWith(property, "LocalPosition"))
    {

    }
    else if (IsStringStartWith(property, "LocalRotation"))
    {

    }
    else if (IsStringStartWith(property, "LocalScale"))
    {

    }
    else if (IsStringStartWith(property, "LocalSkew"))
    {

    }


}

bool AddPositionCurve(AnimationClip* clip, const char* path, AnimationCurve* curve)
{
    if (AnimationCurveType(curve) != kKeyFrameVector2)
        return false;
    Vector2Curve* positionCurve = (Vector2Curve*) malloc(sizeof(Vector2Curve));
    String_Init(&positionCurve->path, strlen(path), path);
    positionCurve->curve = curve;
    DynamicArray_Add(&clip->m_PositionCurves, &positionCurve);
    return true;
}

bool AddRotationCurve(AnimationClip* clip, const char* path, AnimationCurve* curve)
{
    if (AnimationCurveType(curve) != kKeyFrameFloat)
        return false;
    FloatCurve* rotationCurve = (FloatCurve*) malloc(sizeof(FloatCurve));
    String_Init(&rotationCurve->path, strlen(path), path);
    rotationCurve->curve = curve;
    String_Init(&rotationCurve->attribute, strlen("LocalRotation"), "LocalRotation");
    DynamicArray_Add(&clip->m_RotationCurves, &rotationCurve);
    return true;
}

bool AddScaleCurve(AnimationClip* clip, const char* path, AnimationCurve* curve)
{
    if (AnimationCurveType(curve) != kKeyFrameVector2)
        return false;
    Vector2Curve* scaleCurve = (Vector2Curve*) malloc(sizeof(Vector2Curve));
    String_Init(&scaleCurve->path, strlen(path), path);
    scaleCurve->curve = curve;
    DynamicArray_Add(&clip->m_ScaleCurves, &scaleCurve);
    return true;
}

bool AddSkewCurve(AnimationClip* clip, const char* path, AnimationCurve* curve)
{
    if (AnimationCurveType(curve) != kKeyFrameVector2)
        return false;
    Vector2Curve* skewCurve = (Vector2Curve*) malloc(sizeof(Vector2Curve));
    String_Init(&skewCurve->path, strlen(path), path);
    skewCurve->curve = curve;
    DynamicArray_Add(&clip->m_SkewCurves, &skewCurve);
    return true;
}

const String* GetAnimationClipName(AnimationClip* clip)
{
    return &clip->m_ClipName;
}

TimeRange GetAnimationClipRange(AnimationClip* clip)
{
    TimeRange range = {INFINITY, -INFINITY};

    auto iter1 = GetIterator(&clip->m_FloatCurves);
    while (HasNext(iter1))
    {
        FloatCurve* curve = *(FloatCurve**) Next(iter1);
        TimeRange curRange = AnimationCurveRange(curve->curve);
        range.begin = fminf(range.begin, curRange.begin);
        range.end = fmaxf(range.end, curRange.end);
    }

    auto iter2 = GetIterator(&clip->m_PositionCurves);
    while (HasNext(iter2))
    {
        Vector2Curve* curve = *(Vector2Curve**) Next(iter2);
        TimeRange curRange = AnimationCurveRange(curve->curve);
        range.begin = fminf(range.begin, curRange.begin);
        range.end = fmaxf(range.end, curRange.end);
    }

    auto iter3 = GetIterator(&clip->m_RotationCurves);
    while (HasNext(iter3))
    {
        FloatCurve* curve = *(FloatCurve**) Next(iter3);
        TimeRange curRange = AnimationCurveRange(curve->curve);
        range.begin = fminf(range.begin, curRange.begin);
        range.end = fmaxf(range.end, curRange.end);
    }

    auto iter4 = GetIterator(&clip->m_ScaleCurves);
    while (HasNext(iter4))
    {
        Vector2Curve* curve = *(Vector2Curve**) Next(iter4);
        TimeRange curRange = AnimationCurveRange(curve->curve);
        range.begin = fminf(range.begin, curRange.begin);
        range.end = fmaxf(range.end, curRange.end);
    }

    auto iter5 = GetIterator(&clip->m_SkewCurves);
    while (HasNext(iter5))
    {
        Vector2Curve* curve = *(Vector2Curve**) Next(iter5);
        TimeRange curRange = AnimationCurveRange(curve->curve);
        range.begin = fminf(range.begin, curRange.begin);
        range.end = fmaxf(range.end, curRange.end);
    }

    return range;
}

DynamicArray* GetPositionCurves(AnimationClip* clip)
{
    return &clip->m_PositionCurves;
}

DynamicArray* GetRotationCurves(AnimationClip* clip)
{
    return &clip->m_RotationCurves;
}

DynamicArray* GetScaleCurves(AnimationClip* clip)
{
    return &clip->m_ScaleCurves;
}

DynamicArray* GetSkewCurves(AnimationClip* clip)
{
    return &clip->m_SkewCurves;
}
