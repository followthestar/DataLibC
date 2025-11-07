//
// Created by 18030 on 2025/3/16.
//

#include <math.h>
#include <malloc.h>
#include "AnimationState.h"
#include "DataLibC/IO/Input/Input.h"
#include "DataLibC/DS/Algorithm.h"

void AnimationState_Stop(AnimationState* state)
{
    if (state->m_Enabled && state->m_AutoCleanup)
        state->m_ShouldCleanup = true;

    state->m_Enabled = false;
    AnimationState_SetTime(state, 0.0f);
    state->m_FadeBlend = 0;
    state->m_StopWhenFadedOut = 0;
}

void AnimationState_SetTime(AnimationState* state, float time)
{
    state->m_Time = time;
}

void DeleteAnimationState(AnimationState* state)
{
    String_Release(&state->m_ParentName);
    String_Release(&state->m_Name);
    free(state);
}

void AnimationState_Init(
        AnimationState* state, const String* name, AnimationClip* clip, float globalTime, bool isClone)
{
    state->m_Clip = clip;
    state->m_IsClone = isClone;
    String_Init(&state->m_Name, 0, NULL);
    if (state->m_Clip)
    {
        state->m_CachedRange = GetAnimationClipRange(state->m_Clip);
        String_AppendString(&state->m_Name, GetAnimationClipName(state->m_Clip));

    }
    state->m_Weight = 0;
    state->m_FadeBlend = false;
    state->m_StopWhenFadedOut = false;
    state->m_IsFadingOut = false;
    state->m_AutoCleanup = false;
    state->m_ShouldCleanup = false;
    state->m_FadeOutLength = 0;
    state->m_Time = 0;
    state->m_LastGlobalTime = globalTime;
    state->m_Speed = 1.0f;
    state->m_Enabled = false;
    state->m_NeedRebind = true;

    state->m_StopTime = INFINITY;


    //?
    state->m_CachedIndex = 0;
}

void AnimationState_SetWeight(AnimationState* state, float weight)
{
    state->m_Weight = weight;
}

void AnimationState_SetEnabled(AnimationState* state, bool enabled)
{
    if (enabled && !state->m_Enabled)
        state->m_LastGlobalTime = GetCurrTime();
    state->m_Enabled = enabled;
}

void AnimationState_SetWeightTarget(AnimationState* state, float target, float length, bool stopWhenFaded)
{
    float newWeightDelta;
    if (length > 0.001f)
        newWeightDelta = (target - state->m_Weight) / length;
    else
        newWeightDelta = (target - state->m_Weight) * 100000.0f;
    bool ignoreWeightDelta =
            state->m_FadeBlend && CompareApproximately(state->m_WeightTarget, target, kReallySmallWeight) &&
            fabsf(state->m_WeightDelta) > fabsf(newWeightDelta);
    if (!ignoreWeightDelta)
        state->m_WeightDelta = newWeightDelta;

    if (CompareApproximately(state->m_WeightDelta, 0.0f, kReallySmallWeight))
        state->m_WeightDelta = 100000.0f;

    state->m_WeightTarget = target;

    state->m_FadeBlend = true;
    state->m_StopWhenFadedOut = stopWhenFaded;
    state->m_IsFadingOut = false;
}

void AnimationState_SetWeightTargetImmediate(AnimationState* state, float target, bool stopWhenFaded)
{
    state->m_Weight = target;
    state->m_StopWhenFadedOut = stopWhenFaded;
    state->m_FadeBlend = false;
    state->m_IsFadingOut = false;
}

bool AnimationState_ShouldUse(AnimationState* state)
{
    return state->m_Clip && state->m_Enabled && state->m_Weight > kReallySmallWeight;
}

bool UpdateAnimationState(AnimationState* state, float globalTime)
{
    const float deltaTime = globalTime - state->m_LastGlobalTime;
    state->m_LastGlobalTime = globalTime;

    float syncedSpeedDeltaTime = deltaTime * state->m_Speed;
    if (syncedSpeedDeltaTime != 0)
    {
        state->m_Time += syncedSpeedDeltaTime;
        if (state->m_Time > state->m_CachedRange.end)
        {
            state->m_Time = state->m_CachedRange.begin;
        }
        else if (state->m_Time < state->m_CachedRange.begin)
        {
            state->m_Time = state->m_CachedRange.end;
        }
    }
    return UpdateFading(state, deltaTime);
}

bool UpdateFading(AnimationState* state, float deltaTime)
{
    bool didStopAtEnd = false;
    if (!state->m_IsFadingOut)
    {
        const bool forward = state->m_Speed >= 0;
        const float dt = forward ? state->m_Time - (state->m_StopTime - state->m_FadeOutLength)
                                 : (state->m_StopTime + state->m_FadeOutLength) - state->m_Time;
        if (dt > 0)
        {
            // Here means the animation time is within the fading duration
            AnimationState_SetWeightTarget(state, 0.0F, state->m_FadeOutLength, true);
            state->m_IsFadingOut = true;
            didStopAtEnd = UpdateBlendingWeight(state, dt, state->m_FadeOutLength == 0);
        }
    }

    // Update blend target
    if (state->m_FadeBlend)
        didStopAtEnd = UpdateBlendingWeight(state, deltaTime, false);

    return didStopAtEnd;
}

bool UpdateBlendingWeight(AnimationState* state, float deltaTime, bool instantBlend)
{
    state->m_Weight += state->m_WeightDelta * deltaTime;
    // Stop blending and clamp when we reach the target
    if (instantBlend ||
        (state->m_WeightDelta > 0.0f && state->m_Weight > state->m_WeightTarget) ||
        (state->m_WeightDelta <= 0.0f && state->m_Weight < state->m_WeightTarget))
    {
        state->m_Weight = state->m_WeightTarget;
        state->m_FadeBlend = false;
        state->m_IsFadingOut = false;
        if (state->m_StopWhenFadedOut)
        {
            AnimationState_Stop(state);
            return true;
        }
    }
    return false;
}
