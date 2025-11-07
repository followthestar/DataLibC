//
// Created by 18030 on 2025/3/8.
//

#include <malloc.h>
#include <memory.h>
#include <assert.h>
#include "Animation.h"
#include "DataLibC/Allocator/MemoryPool.h"
#include "AnimationClip.h"
#include "DataLibC/Anim/Animation/Internal/AnimationState.h"
#include "DataLibC/DS/List/DynamicArray.h"
#include "DataLibC/Anim/Animation/Internal/BoundCurve.h"
#include "DataLibC/IO/Input/Input.h"
#include "DataLibC/DS/List/LinkedList.h"
#include "DataLibC/Tool/LogMacro.h"
#include "Internal/WrapCurve.h"

static MemoryPool* gKeyframePool = NULL;

typedef LinkedList AnimationList; // LinkekList<Animation*>

static LinkedList* gAnimationList = NULL;

extern const size_t gKeyframeSize;

typedef DynamicArray AnimationStates; // DynamicArray<AnimationState*>
typedef DynamicArray BoundCurves; // DynamicArray<BoundCurve>
typedef DynamicArray Animations; // DynamicArray<AnimationClip*>


typedef struct Animation
{
    bool m_PlayAutomatically;
    bool m_Active;
    void* m_Handle;
    AnimationBindHandler m_BindHandler;
    AnimationClip* m_Animation;
    Animations m_Animations;

    BoundCurves m_BoundCurves;
    AnimationStates m_AnimationStates;
    AnimationState* m_ActiveAnimationStates[32];
    int m_ActiveAnimationStatesSize;

    bool m_Enabled;
    bool m_DirtyRebind;
} Animation;

MemoryPool* GetKeyframePool()
{
    return gKeyframePool;
}

static void AddToAnimationList(Animation* animation)
{
    LinkedList_Remove(gAnimationList, &animation);
    if (animation->m_Active)
        LinkedList_AddLast(gAnimationList, &animation);
}

AnimationState* GetAnimationState(Animation* animation, const char* name);

AnimationState* GetAnimationStateFromClip(Animation* animation, AnimationClip* clip);

AnimationState* GetAnimationStateFromState(AnimationState* state);

void CrossFade(Animation* animation, const char* name, float time);

void CrossFadeState(Animation* animation, AnimationState* state, float time);

void BuildAnimationStates(Animation* animation);

void ReleaseAnimationStates(Animation* animation);

void UpdateAnimation(Animation* animation, float time);

void BlendAnimationOptimized(Animation* animation);

bool AnimationSystemInit()
{
    if (gKeyframePool != NULL)
        return false;
    gKeyframePool = CreateMemoryPool(gKeyframeSize);
    gAnimationList = malloc(sizeof(LinkedList));
    LinkedList_Init(gAnimationList, kPOINTER, NULL, NULL);
    return true;
}

void AnimationTick()
{
    float time = GetCurrTime();
    auto iter = GetIterator(gAnimationList);
    while (HasNext(iter))
    {
        Animation* animation = *(Animation**) Next(iter);
        if (animation->m_Enabled)
            UpdateAnimation(animation, time);
    }
}

Animation* CreateAnimation(void* handle, AnimationBindHandler bindHandler)
{
    Animation* animation = (Animation*) malloc(sizeof(Animation));
    animation->m_ActiveAnimationStatesSize = 0;
    animation->m_Animation = NULL;
    animation->m_Handle = handle;
    animation->m_BindHandler = bindHandler;
    DynamicArray_Init(&animation->m_AnimationStates, 0, kPOINTER, 0);
    DynamicArray_Init(&animation->m_Animations, 0, kPOINTER, 0);
    DynamicArray_Init(&animation->m_BoundCurves, 0, kCUSTOM, sizeof(BoundCurve));
    animation->m_PlayAutomatically = false;
    animation->m_Active = true;
    animation->m_DirtyRebind = false;
    memset(animation->m_ActiveAnimationStates, 0, sizeof(animation->m_ActiveAnimationStates));
    animation->m_Enabled = true;
    return animation;
}

void SetAnimationClip(Animation* animation, AnimationClip* clip)
{
    animation->m_Animation = clip;
}

AnimationClip* GetAnimationClip(Animation* animation)
{
    return animation->m_Animation;
}

bool IsAnimationPlaying(Animation* animation)
{
    if (animation->m_AnimationStates.length == 0)
        return false;
    auto iter = GetIterator(&animation->m_AnimationStates);
    while (HasNext(iter))
    {
        AnimationState* state = *(AnimationState**) Next(iter);
        if (state->m_Enabled)
            return true;
    }
    return false;
}

bool IsAnimationPlayingClip(Animation* animation, const char* clipName)
{
    AnimationState* state = GetAnimationState(animation, clipName);
    if (state && state->m_Enabled)
        return true;

    for (int i = 0; i < animation->m_ActiveAnimationStatesSize; i++)
    {
        AnimationState* sta = animation->m_ActiveAnimationStates[i];
        if (sta->m_IsClone && String_Equals(&sta->m_ParentName, clipName) && sta->m_Enabled)
            return true;
    }
    return false;
}

void StopAnimation(Animation* animation)
{
    auto iter = GetIterator(&animation->m_AnimationStates);
    while (HasNext(iter))
    {
        AnimationState* state = *(AnimationState**) Next(iter);
        AnimationState_Stop(state);
    }

    //TODO
}


void StopAnimationClip(Animation* animation, const char* clipName)
{
    auto iter = GetIterator(&animation->m_AnimationStates);
    while (HasNext(iter))
    {
        AnimationState* state = *(AnimationState**) Next(iter);
        if (state->m_IsClone && String_Equals(&state->m_ParentName, clipName)
            || !state->m_IsClone && String_Equals(&state->m_Name, clipName))
            AnimationState_Stop(state);
    }
    //TODO ?
}

static void PlayAnimationState(Animation* animation, AnimationState* fadeIn)
{
    CrossFadeState(animation, fadeIn, 0);
}

void PlayAnimationClip(Animation* animation, const char* clipName)
{
    CrossFade(animation, clipName, 0);
}

void PlayAnimation(Animation* animation)
{
    AnimationClip* clip = animation->m_Animation;
    if (clip == NULL)
        return;
    auto state = GetAnimationStateFromClip(animation, clip);
    if (state)
        PlayAnimationState(animation, state);
    else
        RuntimeErrorLog(NoSuchValue, "Default clip could not be found in attached animations list!\n");
}

int GetAnimationClipCount(Animation* animation)
{
    return (int) animation->m_Animations.length;
}

void AddAnimationClip(Animation* animation, AnimationClip* clip, bool isLoop)
{
    AddAnimationClipEx(animation, clip, String_ToString(GetAnimationClipName(clip)),
                       INT_MIN, INT_MAX, isLoop);
}

void AddAnimationClipEx(
        Animation* animation, AnimationClip* clip, const char* newName, int firstFrame, int lastFrame, bool loop)
{
    AnimationClip* newClip = clip;
//    bool isNewClip = loop || firstFrame != INT_MIN || lastFrame != INT_MAX;
//    if (isNewClip || !String_Equals(GetAnimationClipName(clip), newName))
//    {
//        newClip = CreateAnimationClip(newName);
//
//
//        if (isNewClip)
//        {
//
//        }
//    }

    int i;
    for (i = 0; i < animation->m_Animations.length; i++)
    {
        AnimationClip* curr = *(AnimationClip**) DynamicArray_ValueAt(&animation->m_Animations, i);
        if (curr && String_Equals(GetAnimationClipName(curr), newName))
            break;
    }
    if (i == animation->m_Animations.length)
        DynamicArray_Add(&animation->m_Animations, &newClip);
    else
        *(AnimationClip**) DynamicArray_ValueAt(&animation->m_Animations, i) = newClip;

    if (animation->m_AnimationStates.length != 0)
    {
        auto iter = GetIterator(&animation->m_AnimationStates);
        while (HasNext(iter))
        {
            AnimationState* state = *(AnimationState**) Next(iter);
            if (String_Equals(&state->m_Name, newName))
            {
                DeleteAnimationState(state);
                Remove(iter);
                break;
            }
        }
        AnimationState* state = malloc(sizeof(AnimationState));
        memset(state, 0, sizeof(AnimationState));
        AnimationState_Init(state, GetAnimationClipName(newClip), newClip, GetCurrTime(), false);
        DynamicArray_Add(&animation->m_AnimationStates, &state);
    }
}

void SetAnimationEnabled(Animation* animation, bool enabled)
{
    animation->m_Enabled = enabled;
}

AnimationState* GetAnimationState(Animation* animation, const char* name)
{
    BuildAnimationStates(animation);
    auto iter = GetIterator(&animation->m_AnimationStates);
    while (HasNext(iter))
    {
        AnimationState* state = *(AnimationState**) Next(iter);
        if (String_Equals(&state->m_Name, name))
            return state;
    }
    return NULL;
}

AnimationState* GetAnimationStateFromClip(Animation* animation, AnimationClip* clip)
{
    BuildAnimationStates(animation);
    auto iter = GetIterator(&animation->m_AnimationStates);
    while (HasNext(iter))
    {
        AnimationState* state = *(AnimationState**) Next(iter);
        if (state->m_Clip == clip)
            return state;
    }
    return NULL;
}

void BuildAnimationStates(Animation* animation)
{
    if (animation->m_AnimationStates.length != 0)
        return;
    if (animation->m_Animations.length == 0)
        return;

    ReleaseAnimationStates(animation);

    float time = GetCurrTime();
    auto iter = GetIterator(&animation->m_Animations);
    while (HasNext(iter))
    {
        AnimationClip* clip = *(AnimationClip**) Next(iter);
        if (clip != NULL)
        {
            AnimationState* state = malloc(sizeof(AnimationState));
            memset(state, 0, sizeof(AnimationState));
            AnimationState_Init(state, GetAnimationClipName(clip), clip, time, false);
            DynamicArray_Add(&animation->m_AnimationStates, &state);
        }

    }
    AddToAnimationList(animation);
}

void ReleaseAnimationStates(Animation* animation)
{
    auto iter = GetIterator(&animation->m_AnimationStates);
    while (HasNext(iter))
    {
        AnimationState* state = *(AnimationState**) Next(iter);
        DeleteAnimationState(state);
    }
    DynamicArray_Clear(&animation->m_AnimationStates);
}

void CrossFade(Animation* animation, const char* name, float time)
{
    auto state = GetAnimationState(animation, name);
    if (state == NULL)
    {
        RuntimeErrorLog(NoSuchValue, "Animation '%s' not found!", name);
        return;
    }
    CrossFadeState(animation, state, time);
}

void CrossFadeState(Animation* animation, AnimationState* playState, float time)
{
    auto iter = GetIterator(&animation->m_AnimationStates);
    while (HasNext(iter))
    {
        AnimationState* state = *(AnimationState**) Next(iter);
        if (state == playState)
        {
            AnimationState_SetEnabled(state, true);
            if (time > kReallySmallFadeTime)
                AnimationState_SetWeightTarget(state, 1.0F, time, false);
            else
                AnimationState_SetWeightTargetImmediate(state, 1.0F, false);
            state->m_FadeOutLength = time;
        }
        else
        {
            if (time > kReallySmallFadeTime)
            {
                AnimationState_SetWeightTarget(state, 0, time, true);
            }
            else
            {
                AnimationState_Stop(state);
                AnimationState_SetWeight(state, 0);
            }
        }
    }
}

static void BindCurves(void* handle, AnimationBindHandler handler, AnimationClip* clip, BoundCurves* outBoundCurves)
{
    auto pos = GetPositionCurves(clip);
    auto rot = GetRotationCurves(clip);
    auto scale = GetScaleCurves(clip);
    auto skew = GetSkewCurves(clip);

    auto iter1 = GetIterator(pos);
    while (HasNext(iter1))
    {
        auto curve = *(Vector2Curve**) Next(iter1);
        BoundCurve bound;
        bound.targetType = kBindLocalPosition;
        bound.targetPtr = handler(handle, String_ToString(&curve->path), "LocalPosition");
        bound.curve = curve->curve;
        if (bound.targetPtr == NULL)
            continue;
        DynamicArray_Add(outBoundCurves, &bound);
    }

    auto iter2 = GetIterator(rot);
    while (HasNext(iter2))
    {
        auto curve = *(FloatCurve**) Next(iter2);
        BoundCurve bound;
        bound.targetType = kBindLocalRotation;
        bound.targetPtr = handler(handle, String_ToString(&curve->path), "LocalRotation");
        bound.curve = curve->curve;
        if (bound.targetPtr == NULL)
            continue;
        DynamicArray_Add(outBoundCurves, &bound);
    }

    auto iter3 = GetIterator(scale);
    while (HasNext(iter3))
    {
        auto curve = *(Vector2Curve**) Next(iter3);
        BoundCurve bound;
        bound.targetType = kBindLocalScale;
        bound.targetPtr = handler(handle, String_ToString(&curve->path), "LocalScale");
        bound.curve = curve->curve;
        if (bound.targetPtr == NULL)
            continue;
        DynamicArray_Add(outBoundCurves, &bound);
    }

    auto iter4 = GetIterator(skew);
    while (HasNext(iter4))
    {
        auto curve = *(Vector2Curve**) Next(iter4);
        BoundCurve bound;
        bound.targetType = kBindLocalSkew;
        bound.targetPtr = handler(handle, String_ToString(&curve->path), "LocalSkew");
        bound.curve = curve->curve;
        if (bound.targetPtr == NULL)
            continue;
        DynamicArray_Add(outBoundCurves, &bound);
    }
}

static void RebuildStateForEverything(Animation* animation)
{
    if (animation->m_Handle && animation->m_BindHandler)
    {
        auto iter = GetIterator(&animation->m_AnimationStates);
        while (HasNext(iter))
        {
            AnimationState* state = *(AnimationState**) Next(iter);
            AnimationClip* clip = state->m_Clip;
            if (clip == NULL)
                continue;
            BindCurves(animation->m_Handle, animation->m_BindHandler, clip, &animation->m_BoundCurves);
        }
    }
    animation->m_ActiveAnimationStatesSize = 0;
    animation->m_DirtyRebind = false;
}

void SampleInternal(Animation* animation)
{
    if (animation->m_DirtyRebind)
    {
        RebuildStateForEverything(animation);
    }

    assert(animation->m_DirtyRebind == false);
    if (animation->m_BoundCurves.length != 0)
    {
        BlendAnimationOptimized(animation);
    }
}

void UpdateAnimation(Animation* animation, float time)
{
    bool needsUpdate = false;
    int stoppedAnimationCount = 0;
    AnimationState* stoppedAnimations[animation->m_AnimationStates.length];
    memset(stoppedAnimations, 0, sizeof(AnimationState*) * animation->m_AnimationStates.length);

    // Update animation states
    for (int i = 0; i < animation->m_AnimationStates.length;)
    {
        AnimationState* state = *(AnimationState**) DynamicArray_ValueAt(&animation->m_AnimationStates, i);
        if (state->m_Enabled)
        {
            if (UpdateAnimationState(state, time))
            {
                if (!state->m_ShouldCleanup)
                    stoppedAnimations[stoppedAnimationCount++] = state;
            }
        }

        if (AnimationState_ShouldUse(state))
            needsUpdate = true;

        animation->m_DirtyRebind |= state->m_NeedRebind;
        state->m_NeedRebind = false;

        if (state->m_ShouldCleanup)
        {
            DeleteAnimationState(state);
            DynamicArray_RemoveAt(&animation->m_AnimationStates, i);
            animation->m_ActiveAnimationStatesSize = 0;
        }
        else
            i++;
    }

    if (stoppedAnimationCount > 0)
    {
        // Here may occur one unexpected phenomen, that is, the animation has stopped and its weight is 0, thus the frame
        // might be set to the initial state but what we want is to set the frame to the last state when the animation stops.
        // TODO

        needsUpdate = true;
    }

    // Only do blending if it is really necessary
    if (needsUpdate)
    {
        SampleInternal(animation);
    }

    //TODO ??

}


void BlendAnimationOptimized(Animation* animation)
{
    assert(animation->m_BoundCurves.length != 0);
    auto iter = GetIterator(&animation->m_BoundCurves);
    while (HasNext(iter))
    {
        BoundCurve* curve = Next(iter);

        switch (curve->targetType)
        {
            case kBindLocalRotation:
            {
                auto stateIter = GetIterator(&animation->m_AnimationStates);
                float result = 0;
                while (HasNext(stateIter))
                {
                    AnimationState* state = *(AnimationState**) Next(stateIter);
                    AnimationCurve* floatCur = curve->curve;
                    float sample;
                    AnimationCurveEvaluateClamp(floatCur, state->m_Time, &sample);
                    result = result + sample;
                }
                *(float*) curve->targetPtr = result;
            }
                break;
            case kBindLocalSkew:
            case kBindLocalScale:
            case kBindLocalPosition:
            {
                auto stateIter = GetIterator(&animation->m_AnimationStates);
                Vector2 result = VEC2_ZERO;
                while (HasNext(stateIter))
                {
                    AnimationState* state = *(AnimationState**) Next(stateIter);
                    AnimationCurve* vec2Cur = curve->curve;
                    Vector2 sample;
                    AnimationCurveEvaluateClamp(vec2Cur, state->m_Time, &sample);
                    result = Vector2_Plus(result, sample);
                }
                *(Vector2*) curve->targetPtr = result;
            }
                break;
            case kUnbind:

                break;
            case kBindEnabled:
                break;
        }
    }

}