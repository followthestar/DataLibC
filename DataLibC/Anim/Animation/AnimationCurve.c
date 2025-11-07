//
// Created by 18030 on 2025/3/8.
//

#include <malloc.h>
#include <float.h>
#include <memory.h>
#include <math.h>

#include "AnimationCurve.h"
#include "DataLibC/DS/List/SortedVector.h"
#include "DataLibC/DS/Algorithm.h"

typedef DynamicArray KeyframeContainer;

typedef struct AnimationCurveCache
{
    int index;
    float time;
    float timeEnd;
    KeyframeValueType type;
    union
    {
        float coeff_f;
        Vector2 coeff_v2;
        Vector3 coeff_v3;
        void* coeff_o;
    } coeff[4];
} AnimationCurveCache;

typedef struct Keyframe
{
    float time;
    KeyframeValueType type;
    union
    {
        float floatValue;
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

enum
{
    kInternalPingPong = 0, kInternalRepeat = 1, kInternalClamp = 2
};

typedef struct AnimationCurve
{
    AnimationCurveCache m_Cache;
    AnimationCurveCache m_ClampCache;

    KeyframeValueType m_Type;
    KeyframeContainer m_Curve;
    int m_PreInfinity;
    int m_PostInfinity;
} AnimationCurve;

static void InvalidateCache(AnimationCurve* curve)
{
    curve->m_Cache.time = INFINITY;
    curve->m_Cache.index = 0;
    curve->m_ClampCache.time = INFINITY;
    curve->m_ClampCache.index = 0;
}

static void InitCache(AnimationCurveCache* cache, KeyframeValueType type)
{
    cache->type = type;
    cache->time = INFINITY;
    cache->index = 0;
    cache->timeEnd = 0;
    memset(cache->coeff, 0, sizeof(cache->coeff));
}

AnimationCurve* CreateAnimationCurve(KeyframeValueType type)
{
    AnimationCurve* curve = (AnimationCurve*) malloc(sizeof(AnimationCurve));
    curve->m_Type = type;
    DynamicArray_Init(&curve->m_Curve, 0, kPOINTER, 0);
    curve->m_PreInfinity = curve->m_PostInfinity = kInternalClamp;
    curve->m_ClampCache.type = type;

    return curve;
}

bool AddKeyFrame(AnimationCurve* curve, Keyframe* keyframe)
{
    if (!keyframe || keyframe->type != curve->m_Type)
        return false;

    InvalidateCache(curve);
    auto iter = LowerBound(curve->m_Curve.m_Array, curve->m_Curve.length,
                           sizeof(Keyframe*), KeyframeCompare, &keyframe);
    if (iter == curve->m_Curve.length ||
        KeyframeCompare(&keyframe, DynamicArray_ValueAt(&curve->m_Curve, iter)))
    {
        DynamicArray_InsertAt(&curve->m_Curve, iter, &keyframe);
        return true;
    }
    else
        return false;

}

bool IsValidAnimationCurve(AnimationCurve* curve)
{
    return curve->m_Curve.length >= 2;
}

Keyframe* GetKeyFrame(AnimationCurve* curve, int index)
{
    if (index >= 0 && index < curve->m_Curve.length)
        return *(Keyframe**) DynamicArray_ValueAt(&curve->m_Curve, index);
    return NULL;
}

int GetKeyFrameCount(AnimationCurve* curve)
{
    return (int) curve->m_Curve.length;
}

DynamicArrayIterator AnimationCurveBegin(AnimationCurve* curve)
{
    return GetIterator(&curve->m_Curve);
}

TimeRange AnimationCurveRange(AnimationCurve* curve)
{
    if (GetKeyFrameCount(curve) == 0)
        return (TimeRange) {.begin = 0, .end = 0};
    return (TimeRange) {.begin = GetKeyFrame(curve, 0)->time,
            .end = GetKeyFrame(curve, (int) curve->m_Curve.length - 1)->time};
}

KeyframeValueType AnimationCurveType(AnimationCurve* curve)
{
    return curve->m_Type;
}

void* TimeRange_Ctor(const void* obj)
{
    TimeRange* range = (TimeRange*) malloc(sizeof(TimeRange));
    *range = *(TimeRange*) obj;
    return range;
}

void static EvaluateCache(AnimationCurveCache* cache, float time, void* out)
{
    float t = time - cache->time;
    switch (cache->type)
    {
        case kKeyFrameFloat:
            *(float*) out =
                    (t * (t * (t * cache->coeff[0].coeff_f + cache->coeff[1].coeff_f) + cache->coeff[2].coeff_f)) +
                    cache->coeff[3].coeff_f;
            break;
        case kKeyFrameVector2:
        {
            Vector2 o = Vector2_Multiply(cache->coeff[0].coeff_v2, time);
            o = Vector2_Plus(o, cache->coeff[1].coeff_v2);
            o = Vector2_Multiply(o, time);
            o = Vector2_Plus(o, cache->coeff[2].coeff_v2);
            o = Vector2_Multiply(o, time);
            o = Vector2_Plus(o, cache->coeff[3].coeff_v2);
            *(Vector2*) out = o;
        }
            break;
        case kKeyFrameVector3:
            break;
        case kKeyFrameBool:
            break;
        case kKeyFrameObject:
            break;
    }

}

// When we look for the next index, how many keyframes do we just loop ahead instead of binary searching?
#define SEARCH_AHEAD 3

void FindIndexForSampling(AnimationCurve* curve, float curveT, int* lhs, int* rhs)
{
    int actualSize = (int) curve->m_Curve.length;
    AnimationCurveCache* cache = &curve->m_ClampCache;
    int cacheIndex = cache->index;
    if (cacheIndex != -1)
    {
        // We can not use the cache time or time end since that is in unwrapped time space!
        float time = (*(Keyframe**) DynamicArray_ValueAt(&curve->m_Curve, cacheIndex))->time;

        if (curveT > time)
        {
            for (int i = 0; i < SEARCH_AHEAD; i++)
            {
                int index = cacheIndex + i;
                if (index + 1 < actualSize &&
                    (*(Keyframe**) DynamicArray_ValueAt(&curve->m_Curve, index + 1))->time > curveT)
                {
                    *lhs = index;
                    *rhs = min(*lhs + 1, actualSize - 1);
                    return;
                }
            }
        }
        else
        {
            for (int i = 0; i < SEARCH_AHEAD; i++)
            {
                int index = cacheIndex - i;
                if (index >= 0 &&
                    curveT >= (*(Keyframe**) DynamicArray_ValueAt(&curve->m_Curve, index))->time)
                {
                    *lhs = index;
                    *rhs = min(*lhs + 1, actualSize - 1);
                    return;
                }
            }
        }
    }

    Keyframe temp = {.time = curveT};
    Keyframe* tempP = &temp;
    int index = (int) UpperBound(curve->m_Curve.m_Array, curve->m_Curve.length,
                                 sizeof(void*), KeyframeCompare, &tempP);
    *lhs = index - 1;
    *rhs = min(actualSize - 1, index);
}

static void SetupStepped(AnimationCurveCache* cache, const Keyframe* lhs, const Keyframe* rhs)
{
    switch (cache->type)
    {
        case kKeyFrameFloat:
            if (lhs->outSlope.floatOutSlope == INFINITY || rhs->inSlope.floatInSlope == INFINITY)
            {
                cache->coeff[0].coeff_f = 0;
                cache->coeff[1].coeff_f = 0;
                cache->coeff[2].coeff_f = 0;
                cache->coeff[3].coeff_f = lhs->floatValue;
            }
            break;
        case kKeyFrameVector2:
            for (int i = 0; i < 2; i++)
            {
                if ((VEC2_AS_ARRAY(lhs->outSlope.vector2OutSlope))[i] == INFINITY ||
                    (VEC2_AS_ARRAY(rhs->inSlope.vector2InSlope))[i] == INFINITY)
                {
                    (VEC2_AS_ARRAY(cache->coeff[0].coeff_v2))[i] = 0.0F;
                    (VEC2_AS_ARRAY(cache->coeff[1].coeff_v2))[i] = 0.0F;
                    (VEC2_AS_ARRAY(cache->coeff[2].coeff_v2))[i] = 0.0F;
                    (VEC2_AS_ARRAY(cache->coeff[3].coeff_v2))[i] = (VEC2_AS_ARRAY(lhs->vector2Value))[i];
                }
            }
            break;
        case kKeyFrameVector3:
            for (int i = 0; i < 3; i++)
            {
                if ((VEC3_AS_ARRAY(lhs->outSlope.vector3OutSlope))[i] == INFINITY ||
                    (VEC3_AS_ARRAY(rhs->inSlope.vector3InSlope))[i] == INFINITY)
                {
                    (VEC3_AS_ARRAY(cache->coeff[0].coeff_v3))[i] = 0.0F;
                    (VEC3_AS_ARRAY(cache->coeff[1].coeff_v3))[i] = 0.0F;
                    (VEC3_AS_ARRAY(cache->coeff[2].coeff_v3))[i] = 0.0F;
                    (VEC3_AS_ARRAY(cache->coeff[3].coeff_v3))[i] = (VEC3_AS_ARRAY(lhs->vector3Value))[i];
                }
            }
            break;
        case kKeyFrameBool:
            break;
        case kKeyFrameObject:
            break;
    }
}

void CalculateCacheData(AnimationCurve* curve, int lhsIndex, int rhsIndex, float timeOffset)
{
    const Keyframe* lhs = *(Keyframe**) DynamicArray_ValueAt(&curve->m_Curve, lhsIndex);
    const Keyframe* rhs = *(Keyframe**) DynamicArray_ValueAt(&curve->m_Curve, rhsIndex);
    AnimationCurveCache* cache = &curve->m_ClampCache;
    cache->index = lhsIndex;
    cache->time = lhs->time + timeOffset;
    cache->timeEnd = rhs->time + timeOffset;
    cache->index = lhsIndex;

    float dx, length;
    switch (curve->m_Type)
    {
        case kKeyFrameFloat:
        {
            float dy;
            float m1, m2, d1, d2;

            dx = rhs->time - lhs->time;
            dx = max(dx, 0.0001f);
            dy = rhs->floatValue - lhs->floatValue;
            length = 1.0f / (dx * dx);

            m1 = lhs->outSlope.floatOutSlope;
            m2 = rhs->inSlope.floatInSlope;
            d1 = m1 * dx;
            d2 = m2 * dx;

            cache->coeff[0].coeff_f = (d1 + d2 - dy - dy) * length / dx;
            cache->coeff[1].coeff_f = (dy + dy + dy - d1 - d1 - d2) * length;
            cache->coeff[2].coeff_f = m1;
            cache->coeff[3].coeff_f = lhs->floatValue;
        }
            break;
        case kKeyFrameVector2:
        {
            Vector2 dy;
            Vector2 m1, m2, d1, d2;

            dx = rhs->time - lhs->time;
            dx = max(dx, 0.0001f);
            dy = Vector2_Subtract(&rhs->vector2Value, &lhs->vector2Value);
            length = 1.0f / (dx * dx);

            m1 = lhs->outSlope.vector2OutSlope;
            m2 = rhs->inSlope.vector2InSlope;
            d1 = Vector2_Multiply(m1, dx);
            d2 = Vector2_Multiply(m2, dx);

            Vector2 temp1 = VEC2((d1.x + d2.x - dy.x * 2) * length / dx, (d1.y + d2.y - dy.y * 2) * length / dx);
            Vector2 temp2 = VEC2((dy.x * 3 - d1.x * 2 - d2.x) * length, (dy.y * 3 - d1.y * 2 - d2.y) * length);
            cache->coeff[0].coeff_v2 = temp1;
            cache->coeff[1].coeff_v2 = temp2;
            cache->coeff[2].coeff_v2 = m1;
            cache->coeff[3].coeff_v2 = lhs->vector2Value;
        }
            break;
        case kKeyFrameVector3:
            break;
        case kKeyFrameBool:
            break;
        case kKeyFrameObject:
            break;
    }
    SetupStepped(cache, lhs, rhs);
}

void AnimationCurveEvaluateClamp(AnimationCurve* curve, float time, void* out)
{
    //Test
    Keyframe temp = {.time = time};
    Keyframe* tp = &temp;
    int index = (int) UpperBound(curve->m_Curve.m_Array, curve->m_Curve.length,
                                 sizeof(void*), KeyframeCompare, &tp);
    if (index == curve->m_Curve.length)
        index--;
    Keyframe* kf = *(Keyframe**) DynamicArray_ValueAt(&curve->m_Curve, index);
    switch (kf->type)
    {
        case kKeyFrameFloat:
            *((float*) out) = kf->floatValue;
            break;
        case kKeyFrameVector2:
            *((Vector2*) out) = kf->vector2Value;
            break;
        case kKeyFrameVector3:
            *((Vector3*) out) = kf->vector3Value;
            break;
    }
    if (fabsf(time - kf->time) < 0.01f)
        curve->m_ClampCache.index = (curve->m_ClampCache.index + 1) % curve->m_Curve.length;
    return;

    if (time >= curve->m_ClampCache.time && time < curve->m_ClampCache.timeEnd)
    {
        EvaluateCache(&curve->m_ClampCache, time, out);
        return;
    }

    TimeRange range = AnimationCurveRange(curve);
    if (time > range.end)
    {
        curve->m_ClampCache.time = range.end;
        curve->m_ClampCache.timeEnd = INFINITY;
        memset(curve->m_ClampCache.coeff, 0, sizeof(curve->m_ClampCache.coeff));
        Keyframe* last = *(Keyframe**) DynamicArray_ValueAt(&curve->m_Curve, curve->m_Curve.length - 1);
        switch (curve->m_Type)
        {
            case kKeyFrameFloat:
                curve->m_ClampCache.coeff[3].coeff_f = last->floatValue;
                break;
            case kKeyFrameVector2:
                curve->m_ClampCache.coeff[3].coeff_v2 = last->vector2Value;
                break;
            case kKeyFrameVector3:
                curve->m_ClampCache.coeff[3].coeff_v3 = last->vector3Value;
                break;
            case kKeyFrameBool:
                // TODO

                break;
            case kKeyFrameObject:
                curve->m_ClampCache.coeff[3].coeff_o = last->objectValue;
                break;
        }
    }
    else if (time < range.begin)
    {
        curve->m_ClampCache.time = time - 1000.0f;
        curve->m_ClampCache.timeEnd = range.begin;
        memset(curve->m_ClampCache.coeff, 0, sizeof(curve->m_ClampCache.coeff));
        Keyframe* first = *(Keyframe**) DynamicArray_ValueAt(&curve->m_Curve, 0);
        switch (curve->m_Type)
        {
            case kKeyFrameFloat:
                curve->m_ClampCache.coeff[3].coeff_f = first->floatValue;
                break;
            case kKeyFrameVector2:
                curve->m_ClampCache.coeff[3].coeff_v2 = first->vector2Value;
                break;
            case kKeyFrameVector3:
                curve->m_ClampCache.coeff[3].coeff_v3 = first->vector3Value;
                break;
            case kKeyFrameBool:
                // TODO

                break;
            case kKeyFrameObject:
                curve->m_ClampCache.coeff[3].coeff_o = first->objectValue;
                break;
        }
    }
    else
    {
        int lhs, rhs;
        FindIndexForSampling(curve, time, &lhs, &rhs);
        CalculateCacheData(curve, lhs, rhs, 0);
    }

    EvaluateCache(&curve->m_ClampCache, time, out);
}
