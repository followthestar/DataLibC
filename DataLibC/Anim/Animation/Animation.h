//
// Created by 18030 on 2025/3/8.
//

#ifndef DATALIBC_ANIMATION_H
#define DATALIBC_ANIMATION_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "AnimationClip.h"

/// Used to controll an animation clip.
typedef struct Animation Animation;

/// This used to provide a method for users to bind the animated data to the animation curves by themselves.
/// \param handle The handle object which provides the data.
/// \param path The path of the AnimationCurve. You can decide how to bind data through this parameter.
/// \param property The property of the AnimationCurve. Such as LocalPosition, Rotation, Scale, Skew and so on.
/// \return The data pointer that will be binded to the AnimationCurve. You need to return it according to the path and property.
typedef void* (* AnimationBindHandler)(void* handle, const char* path, const char* property);

/// \brief Initialize the animation system.
/// \return True if the animation system is initialized successfully, false otherwise.
/// \attention This function must be called before any other animation related operation. [eg. Keyframe, AnimationCurve, AnimationClip]
API_MODULE bool AnimationSystemInit();

/// Call this function every frame in your logical loop to update the animation system.
API_MODULE void AnimationTick();

/// \brief Create a new Animation object.
/// \param handle The handle object which provides the data.
/// \param bindHandler The handler function which defines what data should be binded to the AnimationCurve.
/// \return The Animation object.
API_MODULE Animation* CreateAnimation(void* handle, AnimationBindHandler bindHandler);

/// Adds an animation clip to the animation. If it already exists this will do nothing.
/// \param animation The Animation object you want to add.
/// \param clip The AnimationClip object you want to add.
API_MODULE void AddAnimationClip(Animation* animation, AnimationClip* clip, bool isLoop);

/// \brief Adds an animation clip with name newName to the animation.
/// If newName is not the clip's name or the animation clip needs to be clipped a new instance of the clip will be created.
/// \param animation The Animation object you want to add.
/// \param clip The AnimationClip object you want to add.
/// \param newName The name of the animation clip you want to add.
/// \param firstFrame The first frame of the animation clip.
/// \param lastFrame The last frame of the animation clip.
/// \param loop If true, the animation will loop.
API_MODULE void AddAnimationClipEx(
        Animation* animation, AnimationClip* clip, const char* newName, int firstFrame, int lastFrame, bool loop);

/// \brief Get the number of animation clips attached to the Animation object.
/// \param animation The Animation object you want to get.
/// \return The number of animation clips in attached animations list.
API_MODULE int GetAnimationClipCount(Animation* animation);

/// Is any animation playing?
/// \param animation The Animation object you want to check.
/// \return True if the animation is playing, false otherwise.
API_MODULE bool IsAnimationPlaying(Animation* animation);

/// Is the specified animation clip attached to the Animation object being played?
/// \param animation The Animation object you want to check.
/// \param clipName The name of the animation clip you want to check.
/// \return True if the animation clip is playing, false otherwise.
API_MODULE bool IsAnimationPlayingClip(Animation* animation, const char* clipName);

/// Stop all the animation clips attached to the Animation object.
/// \param animation The Animation object you want to stop.
API_MODULE void StopAnimation(Animation* animation);

/// Stop the specified animation clip attached to the Animation object.
/// \param animation The Animation object you want to stop.
/// \param clipName The name of the animation clip you want to stop.
API_MODULE void StopAnimationClip(Animation* animation, const char* clipName);

/// Set the animation clip to current Animation object.
/// \param animation The Animation object you want to set.
/// \param clip The animation clip you want to set.
API_MODULE void SetAnimationClip(Animation* animation, AnimationClip* clip);

/// Get the animation clip of the Animation object.
/// \param animation The Animation object you want to get.
/// \return The AnimationClip object that is attached to the Animation object.
API_MODULE AnimationClip* GetAnimationClip(Animation* animation);

/// Play the animation currently attached to the Animation object.
/// \param animation The Animation object you want to play.
API_MODULE void PlayAnimation(Animation* animation);

/// Play the specified animation clip attached to the Animation object.
/// \param animation The Animation object you want to play.
/// \param clipName The name of the animation clip you want to play.
API_MODULE void PlayAnimationClip(Animation* animation, const char* clipName);

/// Enable or disable the animation.
/// \param animation The Animation object you want to enable or disable.
/// \param enabled If false is set, this Animation will not be updated.
API_MODULE void SetAnimationEnabled(Animation* animation, bool enabled);

#endif //DATALIBC_ANIMATION_H
