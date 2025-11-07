//
// Created by 18030 on 2025/3/16.
//

#ifndef DATALIBC_ANIMATIONSTATE_H
#define DATALIBC_ANIMATIONSTATE_H

#include "DataLibC/Anim/Animation/AnimationClip.h"
#include "DataLibC/DS/String/String.h"
#include "DataLibC/Anim/Animation/Animation.h"

#define kReallySmallWeight 0.0001f
#define kReallySmallFadeTime 0.001f

typedef struct AnimationState
{
    float m_Time;
    float m_StopTime;
    float m_LastGlobalTime;
    bool m_Enabled;
    bool m_IsClone;
    bool m_AutoCleanup;
    bool m_ShouldCleanup;
    bool m_StopWhenFadedOut;
    bool m_IsFadingOut;
    bool m_FadeBlend;
    float m_Speed;
    float m_Weight;
    float m_FadeOutLength;
    float m_WeightTarget;
    float m_WeightDelta;

    bool m_NeedRebind;

    String m_Name;
    String m_ParentName;

    AnimationClip* m_Clip;
    TimeRange m_CachedRange;
    int m_CachedIndex;

} AnimationState;

void AnimationState_Init(
        AnimationState* state, const String* name, AnimationClip* clip, float globalTime, bool isClone);

void AnimationState_Stop(AnimationState* state);

void AnimationState_SetEnabled(AnimationState* state, bool enabled);

void AnimationState_SetTime(AnimationState* state, float time);

void AnimationState_SetWeight(AnimationState* state, float weight);

void AnimationState_SetWeightTarget(AnimationState* state, float target, float length, bool stopWhenFaded);

void AnimationState_SetWeightTargetImmediate(AnimationState* state, float target, bool stopWhenFaded);

bool AnimationState_ShouldUse(AnimationState* state);

void DeleteAnimationState(AnimationState* state);

bool UpdateAnimationState(AnimationState* state, float globalTime);

bool UpdateFading(AnimationState* state, float deltaTime);

bool UpdateBlendingWeight(AnimationState* state, float deltaTime, bool instantBlend);

#endif //DATALIBC_ANIMATIONSTATE_H
